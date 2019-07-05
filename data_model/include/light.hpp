#pragma once

#include <pthread.h>
#include "color.hpp"
#include "math_point.hpp"
#include "math_vector.hpp"

namespace rtc
{
struct light
{
  color light_color{};
  math_point position{};
  std::uint32_t sampling{};

  enum class light_type
  {
    point           = 1,
    spherical       = 2,
    circle_plane    = 4,
    rectangle_plane = 8,
  } type{light_type::point};

  union
  {
    rtc_float radius{};
    struct { rtc_float x, y; } side;
  };

  struct
  {
    rtc_float a, b, c;
  } inv_square{0.75, 0, 0};
};

inline auto inverse_square_factor(const light& l, const math_vector& direction) -> float
{
  const auto x{length(direction)};
  return std::max<rtc_float>(1.0, x * (l.inv_square.a * x + l.inv_square.b) + l.inv_square.c);
}

} // namespace crtc

