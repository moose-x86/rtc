#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>

#include "utility.hpp"

namespace rtc
{
template <typename T>
inline auto fequal(const T a, const T b, std::uint16_t ulp = 1) noexcept -> bool
{
  static_assert(std::is_floating_point_v<T>, "this is only for rtc_floating point");
  return std::abs(a - b) <= std::max(std::abs(a), std::abs(b)) * std::numeric_limits<T>::epsilon() * ulp;
}

template <typename T>
inline auto fgreater(const T a, const T b, std::uint16_t ulp = 1) noexcept -> bool
{
  return (a > b) && !fequal(a, b, ulp);
}

template <typename T>
inline auto fless(const T a, const T b, std::uint16_t ulp = 1) noexcept -> bool
{
  return (a < b) && !fequal(a, b, ulp);
}

}  // namespace rtc
