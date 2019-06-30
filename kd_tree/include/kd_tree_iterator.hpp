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

  rtc_hot auto operator++() noexcept -> const_iterator&;
  auto operator*() const noexcept -> const value_type&;
  auto operator!=(const const_iterator&) const noexcept -> bool;
  auto operator==(const const_iterator&) const noexcept -> bool;
  auto triangle_hit_value(const rtc_float t) noexcept -> const_iterator&;

 private:
  rtc::math_ray ray;
  std::stack<node_t> nodes;
  tree_node* current_node{};
  rtc_float nearest_intersect_ray_value = std::numeric_limits<rtc_float>::max();

  auto get_children_and_split_value(const tree_node* const, const math_ray&) const noexcept
      -> std::tuple<tree_node*, tree_node*, rtc_float>;
};

}  // namespace rtc
