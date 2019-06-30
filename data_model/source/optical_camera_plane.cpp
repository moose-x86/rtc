#include "optical_camera_plane.hpp"

#include "rtc_log.hpp"

namespace rtc
{
optical_camera_plane::optical_camera_plane(const rtc::camera& c) noexcept
    : corner{c.screen.surface.upper_left_corner},
      view_point{c.view_point},
      x_vec{(c.screen.surface.upper_right_corner - corner) / c.screen.resolution.x},
      y_vec{(c.screen.surface.lower_left_corner - corner) / c.screen.resolution.y}
{
}

auto optical_camera_plane::emit_ray(std::uint32_t x, std::uint32_t y) const noexcept -> rtc::math_ray
{
  const rtc::math_point screen_point{x * x_vec + y * y_vec + corner};
  return {screen_point - view_point, view_point};
}

}  // namespace rtc
