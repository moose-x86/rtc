#pragma once

#include <vector>
#include "math_point.hpp"
#include "math_vector.hpp"
#include "math_axis.hpp"

namespace rtc
{
class bounding_box
{
public:
  bounding_box(const std::vector<math_point>&) noexcept;
  bounding_box(std::initializer_list<math_point>) noexcept;

  auto& min_boundary() const noexcept { return pmin; }
  auto& max_boundary() const noexcept { return pmax; }
  auto& min_boundary() noexcept { return pmin; }
  auto& max_boundary() noexcept { return pmax; }

  auto diagonal() const noexcept { return pmax - pmin; }
  rtc::axis maximum_extent() const noexcept;
  rtc_float surface_area() const noexcept;

private:
  template<typename B, typename E>
  bounding_box(B b, E e) noexcept;

  rtc::math_point pmin, pmax;
};

inline std::ostream& operator<<(std::ostream& ss, const bounding_box& bb) noexcept
{
   return ss << "[ " << bb.min_boundary() << " , " << bb.max_boundary() << " ]bb";
}

}
