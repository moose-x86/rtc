#pragma once

#include "math_vector.hpp"
#include <tuple>

namespace rtc
{

class point_in_triangle_test
{
public:
  point_in_triangle_test(const math_point&, const math_point&, const math_point&) noexcept;
  bool triangle_contains(const math_point& ) const noexcept;

private:
  std::tuple<rtc_float, rtc_float> get_projected_point(const rtc::math_point&) const noexcept;
  rtc_hot void compute_all_needed_factors(const math_point&, const math_point&, const math_point&) noexcept;

  rtc_float a[6];
  rtc::axis biggest_axis;
};

}
