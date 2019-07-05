#pragma once

#include "camera.hpp"
#include "math_ray.hpp"

namespace rtc
{
struct optical_camera_plane
{
 public:
  explicit optical_camera_plane(const rtc::camera&) noexcept;
  [[nodiscard]] auto emit_ray(std::uint32_t, std::uint32_t) const noexcept -> rtc::math_ray;

 private:
  rtc::math_point corner;
  rtc::math_point view_point;
  rtc::math_vector x_vec, y_vec;
};

}  // namespace rtc