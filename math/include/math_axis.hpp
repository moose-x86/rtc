#pragma once
#include <cassert>

namespace rtc
{

enum class axis : std::uint8_t
{
  x = 0, y = 1, z = 2
};

inline constexpr axis next(axis v) noexcept
{
  return static_cast<axis>((int(v) + 1) % 3) ;
}

inline constexpr std::ostream& operator<<(std::ostream& ss, const axis a) noexcept
{
  switch(a)
  {
    case axis::x: return ss << "axis::x";
    case axis::y: return ss << "axis::y";
    case axis::z: return ss << "axis::z";
  };

  return ss;
}

}
