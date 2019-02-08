#include "kd_tree.hpp"
#include "kd_tree_iterator.hpp"
#include "kd_tree_node.hpp"
#include "utility.hpp"

namespace rtc
{

const kd_tree::value_type& kd_tree::const_iterator::operator*() const noexcept
{
  assert(current_node);
  return *current_node->triangles;
}

bool kd_tree::const_iterator::operator==(const kd_tree::const_iterator& i) const noexcept
{
  return current_node == i.current_node;
}

bool kd_tree::const_iterator::operator!=(const kd_tree::const_iterator& i) const noexcept
{
  return !(*this == i);
}

kd_tree::const_iterator::const_iterator(const rtc::math_ray& r, node_t node)
{
  nodes.push(node);
  ray = { 1.0f / r.direction(), r.origin() };

  (*this).operator++();
}

kd_tree::const_iterator& kd_tree::const_iterator::operator++() noexcept
{
  if(rtc_unlikely(nodes.empty()))
    return current_node = nullptr, *this;

  auto [node, tmin, tmax] = nodes.top();
  nodes.pop();

  while(rtc_likely(node != nullptr))
  {
    if(rtc_unlikely(nearest_intersect_ray_value < tmin))
    {
      current_node = nullptr;
      break;
    }

    if(!node->is_leaf())
    {
      const auto [near, far, tsplit] = get_children_and_split_value(node, ray);

      if((tsplit > tmax) || (tsplit <= 0))
      {
        node = near;
      }
      else if(tsplit < tmin)
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

std::tuple<kd_tree::tree_node*, kd_tree::tree_node*, rtc_float>
kd_tree::const_iterator::get_children_and_split_value(const tree_node* const node, const math_ray& r) const noexcept
{
  const auto axis = node->axis.split;
  const auto tsplit = (node->axis.value - r.origin().axis(axis)) * r.direction().axis(axis);
  const bool left_is_near = (r.origin().axis(axis) < node->axis.value) ||
                            (r.origin().axis(axis) == node->axis.value && r.direction().axis(axis) <= 0);

  if(left_is_near)
  {
    return { node->left.get(), node->right.get(), tsplit };
  }
  else
  {
    return { node->right.get(), node->left.get(), tsplit };
  }
}

kd_tree::const_iterator& kd_tree::const_iterator::triangle_hit_value(const rtc_float t) noexcept
{
  return nearest_intersect_ray_value = t, *this;
}

}
