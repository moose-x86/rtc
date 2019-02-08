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
                     const rtc::math_point& start) noexcept : u{direction}, p{start}{}

  constexpr rtc::math_point operator[](const rtc_float t) const noexcept
  {
    return u * t + p;
  }

  constexpr const rtc::math_point & origin() const noexcept { return p; }
  constexpr const rtc::math_vector& direction() const noexcept { return u; }

private:
  rtc::math_vector u;
  rtc::math_point p;
};

inline std::ostream& operator<<(std::ostream& ss, const math_ray& ray) noexcept
{
  return ss << "[ " << ray.direction() << ", " << ray.origin() << " ]r";
}

}
