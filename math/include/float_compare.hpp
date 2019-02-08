#pragma once

#include <cstdint>
#include <cmath>
#include <limits>
#include <type_traits>
#include <algorithm>

#include "utility.hpp"

namespace rtc
{

template<typename T>
inline bool fequal(const T a, const T b, std::uint16_t ulp = 1) noexcept
{
  static_assert(std::is_floating_point_v<T>, "this is only for rtc_floating point");
  return std::abs(a - b) <= std::max(std::abs(a), std::abs(b)) * std::numeric_limits<T>::epsilon() * ulp;
}

template<typename T>
inline bool fgreater(const T a, const T b, std::uint16_t ulp = 1) noexcept
{
  return (a > b) && !fequal(a, b, ulp);
}

template<typename T>
inline bool fless(const T a, const T b, std::uint16_t ulp = 1) noexcept
{
  return (a < b) && !fequal(a, b, ulp);
}

}


