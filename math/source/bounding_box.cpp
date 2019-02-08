#include "bounding_box.hpp"

namespace rtc
{

template<typename B, typename E>
bounding_box::bounding_box(B b, E e) noexcept :
    pmin{std::numeric_limits<math_point::value_type>::max(),
         std::numeric_limits<math_point::value_type>::max(),
         std::numeric_limits<math_point::value_type>::max()},
    pmax{std::numeric_limits<math_point::value_type>::lowest(),
         std::numeric_limits<math_point::value_type>::lowest(),
         std::numeric_limits<math_point::value_type>::lowest()}
{
  for(auto p = b; p != e; ++p)
  {
    pmin.x() = std::min(pmin.x(), p->x());
    pmin.y() = std::min(pmin.y(), p->y());
    pmin.z() = std::min(pmin.z(), p->z());
    pmax.x() = std::max(pmax.x(), p->x());
    pmax.y() = std::max(pmax.y(), p->y());
    pmax.z() = std::max(pmax.z(), p->z());
  }

  constexpr int eps = 2;

  pmin.x() -= eps*std::numeric_limits<math_point::value_type>::epsilon();
  pmin.y() -= eps*std::numeric_limits<math_point::value_type>::epsilon();
  pmin.z() -= eps*std::numeric_limits<math_point::value_type>::epsilon();
  pmax.x() += eps*std::numeric_limits<math_point::value_type>::epsilon();
  pmax.y() += eps*std::numeric_limits<math_point::value_type>::epsilon();
  pmax.z() += eps*std::numeric_limits<math_point::value_type>::epsilon();
}

bounding_box::bounding_box(const std::vector<rtc::math_point>& points) noexcept :
                                                     bounding_box(points.begin(), points.end())
{
}

bounding_box::bounding_box(std::initializer_list<math_point> points) noexcept :
                                                     bounding_box(points.begin(), points.end())
{
}

rtc::axis bounding_box::maximum_extent() const noexcept
{
  const auto v = diagonal();
  return v.x() > v.y() ? (v.x() > v.z() ? axis::x : axis::z) : (v.y() > v.z() ? axis::y : axis::z);
}

rtc_float bounding_box::surface_area() const noexcept
{
  const auto v = diagonal();
  return 2.0f * (v.x()*v.y() + v.x()*v.z() + v.y()*v.z());
}
}
