#pragma once

#include "utility.hpp"

namespace rtc
{

struct bounding_edge_point
{
  rtc_float value;
  enum range_t
  {
    start,
    end
  } type;
  std::uint32_t tr_index;

  bounding_edge_point() noexcept {}
  bounding_edge_point(rtc_float a, std::uint32_t tr, range_t b) noexcept : value(a), type(b), tr_index(tr) {}
};

inline bool operator<(const bounding_edge_point& a, const bounding_edge_point& b) noexcept
{
  return (a.value == b.value) ? (a.type < b.type) : (a.value < b.value);
}

}
