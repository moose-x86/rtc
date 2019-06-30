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

  [[nodiscard]] auto min_boundary() const noexcept -> auto& { return pmin; }
  [[nodiscard]] auto max_boundary() const noexcept -> auto& { return pmax; }
  auto min_boundary() noexcept -> auto& { return pmin; }
  auto max_boundary() noexcept -> auto& { return pmax; }

  [[nodiscard]] auto diagonal() const noexcept { return pmax - pmin; }
  [[nodiscard]] auto maximum_extent() const noexcept -> rtc::axis;
  [[nodiscard]] auto surface_area() const noexcept -> rtc_float;

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
