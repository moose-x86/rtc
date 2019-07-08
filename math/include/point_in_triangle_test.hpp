#pragma once

#include "math_vector.hpp"
#include <tuple>

namespace rtc
{

class point_in_triangle_test
{
public:
  point_in_triangle_test(const math_point&, const math_point&, const math_point&) noexcept;
  [[nodiscard]] auto triangle_contains(const math_point& ) const noexcept -> bool;

private:
  rtc_hot void compute_all_needed_factors(const math_point&, const math_point&, const math_point&) noexcept;
  [[nodiscard]] auto get_projected_point(const rtc::math_point&) const noexcept -> std::tuple<rtc_float, rtc_float>;

  std::array<rtc_float, 6> a{};
  rtc::axis biggest_axis;
};

}
