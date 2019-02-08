#pragma once

#include <stack>
#include "kd_tree.hpp"
#include "utility.hpp"

namespace rtc
{

class kd_tree::const_iterator
{
  using node_t = std::tuple<tree_node*, rtc_float, rtc_float>;

public:
  const_iterator() = default;
  rtc_hot const_iterator(const rtc::math_ray& r, node_t node);

  rtc_hot const_iterator& operator++() noexcept;
  const value_type& operator*() const noexcept;
  bool operator!=(const const_iterator&) const noexcept;
  bool operator==(const const_iterator&) const noexcept;
  const_iterator& triangle_hit_value(const rtc_float t) noexcept;

private:
  rtc::math_ray ray;
  std::stack<node_t> nodes;
  tree_node* current_node{};
  rtc_float nearest_intersect_ray_value = std::numeric_limits<rtc_float>::max();

  std::tuple<tree_node*, tree_node*, rtc_float> get_children_and_split_value(const tree_node* const, const math_ray&) const noexcept;
};

}
