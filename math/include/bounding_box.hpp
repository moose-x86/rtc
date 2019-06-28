#pragma once

#include <vector>

#include "math_axis.hpp"
#include "math_point.hpp"
#include "math_vector.hpp"

namespace rtc
{
class bounding_box
{
 public:
  explicit bounding_box(const std::vector<math_point>&) noexcept;
  bounding_box(std::initializer_list<math_point>) noexcept;

  auto& min_boundary() const noexcept { return pmin; }
  auto& max_boundary() const noexcept { return pmax; }
  auto& min_boundary() noexcept { return pmin; }
  auto& max_boundary() noexcept { return pmax; }

  [[nodiscard]] auto diagonal() const noexcept { return pmax - pmin; }
  auto maximum_extent() const noexcept -> rtc::axis;
  rtc_float surface_area() const noexcept;

 private:
  template <typename B, typename E>
  bounding_box(B b, E e) noexcept;

  rtc::math_point pmin, pmax;
};

inline auto operator<<(std::ostream& ss, const bounding_box& bb) noexcept -> std::ostream&
{
  return ss << "[ " << bb.min_boundary() << " , " << bb.max_boundary() << " ]bb";
}

}  // namespace rtc
