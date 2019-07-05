#include "kd_tree_iterator.hpp"

#include "kd_tree.hpp"
#include "kd_tree_node.hpp"
#include "utility.hpp"

namespace rtc
{
auto kd_tree::const_iterator::operator*() const noexcept -> const value_type&
{
  assert(current_node);
  return *current_node->triangles;
}

auto kd_tree::const_iterator::operator==(const kd_tree::const_iterator& i) const noexcept -> bool
{
  return current_node == i.current_node;
}

auto kd_tree::const_iterator::operator!=(const kd_tree::const_iterator& i) const noexcept -> bool
{
  return !(*this == i);
}

kd_tree::const_iterator::const_iterator(const rtc::math_ray& r, node_t node)
{
  nodes.push(node);
  ray = {1.0F / r.direction(), r.origin()};

  (*this).operator++();
}

auto kd_tree::const_iterator::operator++() noexcept -> const_iterator&
{
  if (rtc_unlikely(nodes.empty()))
    return current_node = nullptr, *this;

  auto [node, tmin, tmax] = nodes.top();
  nodes.pop();

  while (rtc_likely(node != nullptr))
  {
    if (rtc_unlikely(nearest_intersect_ray_value < tmin))
    {
      current_node = nullptr;
      break;
    }

    if (!node->is_leaf())
    {
      const auto [near, far, tsplit] = get_children_and_split_value(node, ray);

      if ((tsplit > tmax) || (tsplit <= 0))
      {
        node = near;
      }
      else if (tsplit < tmin)
      {
        node = far;
      }
      else
      {
        nodes.push({far, tsplit, tmax});
        node = near;
        tmax = tsplit;
      }
    }
    else
    {
      return current_node = node, *this;
    }
  }

  return *this;
}

auto kd_tree::const_iterator::get_children_and_split_value(const tree_node* const node, const math_ray& r) const
    noexcept -> std::tuple<tree_node*, tree_node*, rtc_float>
{
  const auto axis = node->axis.split;
  const auto tsplit = (node->axis.value - r.origin().axis(axis)) * r.direction().axis(axis);
  const bool left_is_near = (r.origin().axis(axis) < node->axis.value) ||
                            (r.origin().axis(axis) == node->axis.value && r.direction().axis(axis) <= 0);

  if (left_is_near)
  {
    return {node->left.get(), node->right.get(), tsplit};
  }
  else
  {
    return {node->right.get(), node->left.get(), tsplit};
  }
}

auto kd_tree::const_iterator::triangle_hit_value(const rtc_float t) noexcept -> const_iterator&
{
  return nearest_intersect_ray_value = t, *this;
}

}  // namespace rtc
