#include "point_in_triangle_test.hpp"

#include "math_vector.hpp"

namespace rtc
{
point_in_triangle_test::point_in_triangle_test(const math_point& p1, const math_point& p2,
                                               const math_point& p3) noexcept
    : a{}, biggest_axis{}
{
  const auto n = cross(p1 - p2, p3 - p2);
  biggest_axis = rtc::axis_of_biggest_vec_comp(n);

  compute_all_needed_factors(p1, p2, p3);
}

auto point_in_triangle_test::triangle_contains(const math_point& p) const noexcept -> bool
{
  const auto [x, y] = get_projected_point(p);

  const rtc_float vec_x = x - a[4];
  const rtc_float vec_y = y - a[5];
  const rtc_float ac = a[0] * vec_x + a[1] * vec_y;
  const rtc_float bc = a[2] * vec_x + a[3] * vec_y;

  return (ac >= 0.0F) && (bc >= 0.0F) && (ac + bc <= 1.0F);
}

void point_in_triangle_test::compute_all_needed_factors(const math_point& p1, const math_point& p2,
                                                        const math_point& p3) noexcept
{
  using namespace std;

  const auto [x1, y1] = get_projected_point(p1);
  const auto [x2, y2] = get_projected_point(p2);
  const auto [x3, y3] = get_projected_point(p3);
  const rtc_float c = 1.0F / ((x1 - x2) * (y3 - y2) - (x3 - x2) * (y1 - y2));

  a[0] = c * (y3 - y2);
  a[1] = c * (x2 - x3);
  a[2] = c * (y2 - y1);
  a[3] = c * (x1 - x2);
  a[4] = x2;
  a[5] = y2;
}

auto point_in_triangle_test::get_projected_point(const math_point& p) const noexcept -> std::tuple<rtc_float, rtc_float>
{
  switch (biggest_axis)
  {
    case rtc::axis::x:
      return {p.y(), p.z()};
    case rtc::axis::y:
      return {p.x(), p.z()};
    case rtc::axis::z:
      return {p.x(), p.y()};
  }

  return {};
}

}  // namespace rtc
