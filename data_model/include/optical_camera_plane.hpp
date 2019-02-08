#pragma once

#include "math_ray.hpp"
#include "camera.hpp"

namespace rtc
{

struct optical_camera_plane
{
public:
  optical_camera_plane( const rtc::camera&) noexcept;
  rtc::math_ray emit_ray(std::uint32_t, std::uint32_t) const noexcept;

private:
  rtc::math_point corner;
  rtc::math_point view_point;
  rtc::math_vector x_vec, y_vec;
};

}
