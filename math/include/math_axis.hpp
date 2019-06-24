#pragma once

#include <cassert>
#include <cstdint>
#include <iostream>

namespace rtc
{
enum class axis : std::uint8_t
{
  x = 0,
  y = 1,
  z = 2
};

inline constexpr auto next(axis v) noexcept -> axis { return static_cast<axis>((int(v) + 1) % 3); }

inline constexpr auto operator<<(std::ostream& ss, const axis a) noexcept -> std::ostream&
{
  switch (a)
    {
      case axis::x:
        return ss << "axis::x";
      case axis::y:
        return ss << "axis::y";
      case axis::z:
        return ss << "axis::z";
    };

  return ss;
}

}  // namespace rtc
