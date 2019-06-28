#pragma once

#include <limits>

#include "math_ray.hpp"
#include "math_vector.hpp"
#include "utility.hpp"

namespace rtc
{
class math_plane
{
 public:
  constexpr math_plane(const rtc::math_vector& n, const rtc::math_point& p) noexcept : e{n.x(), n.y(), n.z(), {}}
  {
    e[3] = -(n.x() * p.x() + n.y() * p.y() + n.z() * p.z());
  }

  math_plane(const rtc::math_point& p1, const rtc::math_point& p2, const rtc::math_point& p3) noexcept
      : math_plane{cross(p1 - p2, p3 - p2), p2}
  {
  }

  rtc_pure rtc_inline [[nodiscard]] auto intersection_value(const rtc::math_ray& ray) const noexcept
  {
    const auto& u = ray.direction();
    const auto d{u.x() * e[0] + u.y() * e[1] + u.z() * e[2]};

    const auto is_not_parralel_to_plane = std::abs(d) > std::numeric_limits<rtc_float>::epsilon();

    if (rtc_likely(is_not_parralel_to_plane))
      {
        const auto& p = ray.origin();
        return -(e[0] * p.x() + e[1] * p.y() + e[2] * p.z() + e[3]) / d;
      }

    return infinity;
  }

  inline static constexpr const rtc_float infinity{std::numeric_limits<rtc_float>::max()};

 private:
  std::array<rtc_float, 4> e;
};

}  // namespace rtc
