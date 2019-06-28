#pragma once

#include "math_point.hpp"
#include "math_vector.hpp"

namespace rtc
{
class math_ray
{
 public:
  constexpr math_ray() noexcept = default;
  constexpr math_ray(const rtc::math_vector& direction,
                     const rtc::math_point& start) noexcept
      : u{direction}, p{start}
  {
  }

  constexpr auto operator[](const rtc_float t) const noexcept -> rtc::math_point
  {
    return u * t + p;
  }

  [[nodiscard]] constexpr auto origin() const noexcept -> const rtc::math_point&
  {
    return p;
  }
  [[nodiscard]] constexpr auto direction() const noexcept
      -> const rtc::math_vector&
  {
    return u;
  }

 private:
  rtc::math_vector u;
  rtc::math_point p;
};

inline auto operator<<(std::ostream& ss, const math_ray& ray) noexcept
    -> std::ostream&
{
  return ss << "[ " << ray.direction() << ", " << ray.origin() << " ]r";
}

}  // namespace rtc
