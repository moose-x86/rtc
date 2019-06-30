#include "ray_box_intersection_test.hpp"

namespace rtc
{
ray_box_intersection_test::ray_box_intersection_test(const std::vector<rtc::math_point>& points) noexcept : bbox{points}
{
}

auto ray_box_intersection_test::intersection_values_for(const rtc::math_ray& ray) const noexcept
    -> ray_intersection_tuple
{
  const auto invRayDir = 1.F / ray.direction();
  rtc_float t_in = 0, t_out = std::numeric_limits<rtc_float>::max();

  for (const auto x : {axis::x, axis::y, axis::z})
  {
    auto t_far = (bbox.max_boundary().axis(x) - ray.origin().axis(x)) * invRayDir.axis(x);
    auto t_near = (bbox.min_boundary().axis(x) - ray.origin().axis(x)) * invRayDir.axis(x);

    if (t_near > t_far)
      std::swap(t_near, t_far);

    t_in = std::max(t_near, t_in);
    t_out = std::min(t_far, t_out);

    if (t_in > t_out)
      return std::nullopt;
  }

  return std::make_tuple(t_in, t_out);
}

}  // namespace rtc
