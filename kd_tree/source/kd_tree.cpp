#include <cmath>
#include <memory>
#include <map>
#include <set>
#include <algorithm>
#include <boost/range/irange.hpp>

#include "rtc_log.hpp"
#include "scoped_timer.hpp"
#include "kd_tree_node.hpp"
#include "bounding_edge.hpp"

namespace rtc
{

kd_tree::kd_tree(const rtc::scene_model& ss) : bbox{ss.points}
{
  SCOPE_TIME_COUNTER;
  const auto& p = ss.points;
  const auto& t = ss.triangles;
  const std::uint32_t max_depth = 1.3F * std::log2(t.size()) + 8;
  auto make_bbox = [](auto& p, auto& tr) noexcept {
    return rtc::bounding_box{p[tr.vertex_a()], p[tr.vertex_b()], p[tr.vertex_c()]};
  };

  rtc::bounding_box node_bbox{p};
  std::vector<std::uint32_t> v(t.size());
  std::vector<bounding_box> primitive_bboxes;
  edge_buffer_array_t edge_buffer{edge_buffer_t(2 * t.size()), edge_buffer_t(2 * t.size()), edge_buffer_t(2 * t.size())};

  primitive_bboxes.reserve(t.size());

  std::for_each(t.begin(), t.end(), [&](auto& t){ primitive_bboxes.push_back(make_bbox(p, t)); });
  std::generate(v.begin(), v.end(), [i = 0]() mutable { return i++; });

  build_tree(root, node_bbox, std::move(v), primitive_bboxes, edge_buffer, max_depth);
}

auto kd_tree::cbegin(const rtc::math_ray& ray) const noexcept -> kd_tree::const_iterator
{
  if(auto range = bbox.intersection_values_for(ray))
  {
    const auto [tmin, tmax] = range.value();
    return const_iterator{ray, {root.get(), tmin, tmax}};
  }

  return const_iterator{};
}

auto kd_tree::cend(const rtc::math_ray&) const noexcept -> kd_tree::const_iterator
{
  return const_iterator{};
}

kd_tree::~kd_tree() = default;
kd_tree::kd_tree(kd_tree&&) noexcept = default;
auto kd_tree::operator=(kd_tree&&) noexcept -> kd_tree& = default;

void kd_tree::build_tree(std::unique_ptr<tree_node>& node,
                            rtc::bounding_box node_bbox,
                            std::vector<std::uint32_t> tr,
                            const std::vector<rtc::bounding_box>& primitive_bboxes,
                            edge_buffer_array_t& edge_buffer,
                            const std::uint32_t depth,
                            std::uint32_t bad_refines)
{
  const auto create_leaf_node = [&node, &tr] {
     node = std::make_unique<tree_node>();
     node->triangles = std::make_unique<kd_tree::value_type>();
     node->triangles->insert(node->triangles->end(), tr.begin(), tr.end());
  };

  if(tr.size() <= 1 || !depth)
    return create_leaf_node();

  const auto [axis, offset, cost, old_cost] =
            compute_node_split_paramters(edge_buffer, tr, node_bbox, primitive_bboxes);

  bad_refines += (cost > old_cost) ? 1 : 0;

  auto no_use_of_spliting_node = [old_cost = old_cost, cost = cost, axis = axis, &bad_refines, &tr]{
    return ((cost > 4*old_cost) && (tr.size() < 16)) || (axis == -1) || (bad_refines == 3);
  };

  if(no_use_of_spliting_node())
    return create_leaf_node();

  auto [left_set, right_set] = split_triangles(std::move(tr), edge_buffer, axis, offset);
  bounding_box left_bbox{node_bbox},
               right_bbox{node_bbox};

  node = std::make_unique<tree_node>();
  node->axis = { edge_buffer[axis][offset].value, static_cast<rtc::axis>(axis) };

  left_bbox.max_boundary().axis(node->axis.split) =
       right_bbox.min_boundary().axis(node->axis.split) = node->axis.value;

  build_tree(node->left , left_bbox , std::move(left_set) , primitive_bboxes, edge_buffer, depth - 1, bad_refines);
  build_tree(node->right, right_bbox, std::move(right_set), primitive_bboxes, edge_buffer, depth - 1, bad_refines);
}

auto kd_tree::compute_node_split_paramters(
                      edge_buffer_array_t& edge_buffer,
                      const std::vector<std::uint32_t>& tr,
                      const rtc::bounding_box& node_bbox,
                      const std::vector<rtc::bounding_box>& primitive_bboxes) -> std::tuple<int, int, rtc_float, rtc_float>
{
  constexpr int isect_cost{80};
  constexpr int traversal_cost{1};
  constexpr rtc_float empty_bonus{0.5F};
  const auto node_diagonal{node_bbox.diagonal()};
  const rtc_float invTotalSA{1.0F/node_bbox.surface_area()};
  const rtc_float old_cost{isect_cost * static_cast<rtc_float>(tr.size())};

  int retries{0};
  int best_axis{-1};
  int best_offset{-1};
  rtc::axis axis{node_bbox.maximum_extent()};
  rtc_float best_cost{std::numeric_limits<rtc_float>::max()};

  do
  {
    const rtc::axis axis2{next(axis)}, axis3{next(axis2)};
    int below{}, above(tr.size());

    for(const auto i : boost::irange<std::size_t>(0, tr.size()))
    {
	  const rtc::bounding_box& box{ primitive_bboxes[tr[i]] };
      edge_buffer[int(axis)][2 * i] = { box.min_boundary().axis(axis), tr[i], bounding_edge_point::start };
      edge_buffer[int(axis)][2 * i + 1] = { box.max_boundary().axis(axis), tr[i], bounding_edge_point::end };
    }

    std::sort(&edge_buffer[int(axis)][0], &edge_buffer[int(axis)][0] + 2*tr.size());

    for(const auto i : boost::irange<std::size_t>(0, 2*tr.size()))
    {
      const rtc::bounding_edge_point& bp{edge_buffer[int(axis)][i]};
      const auto edge_t = bp.value;

      if(bp.type == bounding_edge_point::end) --above;
      if(bp.type == bounding_edge_point::start) ++below;

      if(edge_t > node_bbox.min_boundary().axis(axis) && edge_t < node_bbox.max_boundary().axis(axis))
      {
        const rtc_float belowSA{2 * (node_diagonal.axis(axis2) * node_diagonal.axis(axis3) +
                                (edge_t - node_bbox.min_boundary().axis(axis)) *
                                (node_diagonal.axis(axis2) + node_diagonal.axis(axis3)))};

        const rtc_float aboveSA{2 * (node_diagonal.axis(axis2) * node_diagonal.axis(axis3) +
                                (node_bbox.max_boundary().axis(axis) - edge_t) *
                                (node_diagonal.axis(axis2) + node_diagonal.axis(axis3)))};


        const rtc_float pBelow{belowSA * invTotalSA};
        const rtc_float pAbove{aboveSA * invTotalSA};
        const rtc_float eb{(above == 0 || below == 0) ? empty_bonus : 0};
        const rtc_float cost{traversal_cost + isect_cost*(1 - eb)*(pBelow * below + pAbove * above)};

        if(cost < best_cost)
        {
          best_cost = cost;
          best_axis = int(axis);
          best_offset = i;
        }
      }
    }
  }
  while((best_axis == -1) && ((axis = next(axis), retries++) < 2));

  return { best_axis, best_offset, best_cost, old_cost };
}

auto kd_tree::split_triangles(
         std::vector<std::uint32_t>&& tr_init,
         const edge_buffer_array_t& edge_buffer,
         const std::uint32_t best_axis,
         const std::uint32_t best_offset) -> vector_tuple<std::uint32_t, std::uint32_t>
{
  std::vector<std::uint32_t> tr{std::move(tr_init)}, left_set{}, right_set{};

  left_set.reserve(best_offset);
  right_set.reserve(2*tr.size() - best_offset);

  for(auto i = 0; i < best_offset; ++i)
  {
    if(edge_buffer[best_axis][i].type == bounding_edge_point::start)
    {
      left_set.emplace_back(edge_buffer[best_axis][i].tr_index);
    }
  }

  for(auto i = best_offset + 1; i < 2*tr.size(); ++i)
  {
    if(edge_buffer[best_axis][i].type == bounding_edge_point::end)
    {
      right_set.emplace_back(edge_buffer[best_axis][i].tr_index);
    }
  }

  return { std::move(left_set), std::move(right_set) };
}

}
