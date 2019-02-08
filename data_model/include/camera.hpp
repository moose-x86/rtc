#pragma once
#include "math_point.hpp"
#include <ostream>

namespace rtc
{

struct camera
{
  math_point view_point;

  struct
  {
    struct {
      math_point upper_left_corner;
      math_point upper_right_corner;
      math_point lower_left_corner;
    } surface;

    struct{
      std::uint32_t x{800}, y{600};
    } resolution;
  } screen;
};


inline std::ostream& operator<<(std::ostream& os, const rtc::camera& c) noexcept
{
  return os << "[ view{ " << c.view_point << " }, "
            << "screen { "
            << "surface {"
            << "upper left {" << c.screen.surface.upper_left_corner << "}, "
            << "upper right {" << c.screen.surface.upper_right_corner << "}, "
            << "lower left {" << c.screen.surface.lower_left_corner << "} "
            << "},"
            << " resolution {"
            << "x: " << c.screen.resolution.x << ", "
            << "y: " << c.screen.resolution.y << "}}]cam";
}

}