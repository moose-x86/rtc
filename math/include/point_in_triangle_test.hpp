#pragma once

#include <tuple>

#include "math_vector.hpp"

namespace rtc
{
class point_in_triangle_test
{
 public:
  point_in_triangle_test(const math_point&, const math_point&, const math_point&) noexcept;
  [[nodiscard]] bool triangle_contains(const math_point&) const noexcept;

 private:
  [[nodiscard]] auto get_projected_point(const rtc::math_point&) const noexcept;
  rtc_hot void compute_all_needed_factors(const math_point&, const math_point&, const math_point&) noexcept;

  std::array<rtc_float, 6> a{};
  rtc::axis biggest_axis;
};

}  // namespace rtc
