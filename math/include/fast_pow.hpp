#pragma once

#include "utility.hpp"

namespace rtc
{

template<typename T>
rtc_pure rtc_unroll inline constexpr T pow(T x, std::uint32_t n) noexcept(noexcept(T{1}) &&
                                                                 noexcept(x *= x) &&
                                                                 noexcept(T{std::declval<const T&>()}))
{
  T acc{1};

  while(rtc_likely(n > 0))
  {
    if(n % 5 == 0)
      n = (x *= x *x *x *x, n / 5);

    if(n % 3 == 0)
      n = (x *= x *x, n / 3);

    n = (n & 1) ? (acc *= x, n - 1) : (x *= x, n >> 1);
  }

  return acc;
}

}
