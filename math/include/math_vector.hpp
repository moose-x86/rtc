#pragma once

#include <array>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

#include "float_compare.hpp"
#include "math_axis.hpp"
#include "math_point.hpp"
#include "utility.hpp"

namespace rtc
{
struct math_vector
{
 public:
  using value_type = rtc_float;

  constexpr math_vector() noexcept = default;
  constexpr math_vector(rtc_float px, rtc_float py, rtc_float pz) noexcept : e{px, py, pz} {}

  constexpr explicit math_vector(const std::array<value_type, 3> v) noexcept : math_vector(v[0], v[1], v[2]) {}

  constexpr explicit math_vector(const math_point& p) noexcept : e{p.x(), p.y(), p.z()} {}

  [[nodiscard]] constexpr auto x() const noexcept { return e[0]; }
  [[nodiscard]] constexpr auto y() const noexcept { return e[1]; }
  [[nodiscard]] constexpr auto z() const noexcept { return e[2]; }

  constexpr auto begin() noexcept { return std::begin(e); }
  constexpr auto end() noexcept { return std::end(e); }

  [[nodiscard]] constexpr auto cbegin() const noexcept { return std::cbegin(e); }
  [[nodiscard]] constexpr auto cend() const noexcept { return std::cend(e); }

  constexpr decltype(auto) axis(rtc::axis v) noexcept { return e[int(v)]; }
  [[nodiscard]] constexpr auto axis(rtc::axis v) const noexcept { return e[int(v)]; }

 private:
  std::array<value_type, 3> e{};
};

inline auto operator==(const math_vector& v1, const math_vector& v2) noexcept -> bool
{
  constexpr int eps = 1;
  return fequal(v1.x(), v2.x(), eps) && fequal(v1.y(), v2.y(), eps) && fequal(v1.z(), v2.z(), eps);
}

inline auto operator!=(const math_vector& v1, const math_vector& v2) noexcept -> bool { return !(v1 == v2); }

inline constexpr auto operator+(const math_vector& v1, const math_vector& v2) noexcept -> math_vector
{
  return {v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z()};
}

inline constexpr auto operator-(const math_vector& v1, const math_vector& v2) noexcept -> math_vector
{
  return {v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z()};
}

inline constexpr auto operator*(const math_vector& v, const rtc_float c) noexcept -> math_vector
{
  return {c * v.x(), c * v.y(), c * v.z()};
}

inline auto operator<<(std::ostream& s, const math_vector& v) noexcept -> std::ostream&
{
  return s << "[" << v.x() << ", " << v.y() << ", " << v.z() << "]v";
}

rtc_pure inline constexpr auto dot(const math_vector& v1, const math_vector& v2) noexcept -> rtc_float
{
  return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

rtc_pure inline constexpr auto lengthSQ(const math_vector& v) noexcept -> rtc_float { return dot(v, v); }

inline auto length(const math_vector& v) noexcept -> rtc_float { return std::sqrt(lengthSQ(v)); }

inline constexpr auto operator/(const math_vector& v, const rtc_float c) noexcept -> math_vector
{
  return {v.x() / c, v.y() / c, v.z() / c};
}

inline constexpr auto operator/(const rtc_float c, const math_vector& v) noexcept -> math_vector
{
  return {c / v.x(), c / v.y(), c / v.z()};
}

rtc_hot rtc_pure inline auto normalize(const math_vector& v) noexcept -> math_vector
{
  const auto length = lengthSQ(v);

  if (rtc_likely(length > 0.0F))
    return v / std::sqrt(length);

  return math_vector(std::numeric_limits<rtc_float>::max(),
                     std::numeric_limits<rtc_float>::max(),
                     std::numeric_limits<rtc_float>::max());
}

rtc_hot rtc_pure inline constexpr auto cross(const math_vector& v1, const math_vector& v2) noexcept -> math_vector
{
  return {v1.y() * v2.z() - v1.z() * v2.y(), v1.z() * v2.x() - v1.x() * v2.z(), v1.x() * v2.y() - v1.y() * v2.x()};
}

inline constexpr auto operator*(const rtc_float c, const math_vector& v) noexcept -> math_vector { return v * c; }

inline auto operator-(const math_vector& v) noexcept -> math_vector { return -1 * v; }

inline constexpr auto operator+(const math_vector& v, const math_point& p) noexcept -> math_point
{
  return math_point{v.x() + p.x(), v.y() + p.y(), v.z() + p.z()};
}

inline constexpr auto operator-(const math_point& p1, const math_point& p2) noexcept -> math_vector
{
  return {p1.x() - p2.x(), p1.y() - p2.y(), p1.z() - p2.z()};
}

inline constexpr auto operator+(const math_point& p, const math_vector& v) noexcept -> math_point { return v + p; }

inline constexpr auto operator-(const math_vector& v, const math_point& p) noexcept -> math_point
{
  return {v.x() - p.x(), v.y() - p.y(), v.z() - p.z()};
}

inline constexpr auto operator-(const math_point& p, const math_vector& v) noexcept -> math_point
{
  return {p.x() - v.x(), p.y() - v.y(), p.z() - v.z()};
}

inline auto cos(const math_vector& v1, const math_vector& v2) noexcept -> rtc_float
{
  return dot(v1, v2) / (length(v1) * length(v2));
}

inline auto axis_of_biggest_vec_comp(const math_vector& v) noexcept -> axis
{
  const auto a = std::fabs(v.x());
  const auto b = std::fabs(v.y());
  const auto c = std::fabs(v.z());

  return a > b ? (a > c ? axis::x : axis::z) : (b > c ? axis::y : axis::z);
}

inline constexpr auto operator/(const math_point& p, const math_vector& v) noexcept -> math_vector
{
  return {p.x() / v.x(), p.y() / v.y(), p.z() / v.z()};
}

}  // namespace rtc
