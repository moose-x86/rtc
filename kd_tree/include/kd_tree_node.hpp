#pragma once

#include <memory>
#include "kd_tree.hpp"

namespace rtc
{

struct kd_tree::tree_node
{
  struct axis_data { rtc_float value; rtc::axis split; };
  union
  {
    axis_data axis{};
    std::unique_ptr<kd_tree::value_type> triangles;
  };

  std::unique_ptr<tree_node> left{}, right{};

  tree_node() : triangles{nullptr} {}
  ~tree_node() //without this there is leak
  {
    if(is_leaf())
      triangles.reset();
  }

  [[nodiscard]] auto is_leaf() const noexcept -> bool { return !left && !right; }
};

}
