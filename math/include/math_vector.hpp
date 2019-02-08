#pragma once

#include <cmath>
#include <string>
#include <stdexcept>
#include <limits>
#include "float_compare.hpp"
#include "math_point.hpp"
#include "math_axis.hpp"
#include "utility.hpp"

namespace rtc
{

struct math_vector
{
public:
  using value_type = rtc_float;

  constexpr math_vector() noexcept {}
  constexpr math_vector(rtc_float px, rtc_float py, rtc_float pz) noexcept : e{px, py, pz}
  {}

  constexpr math_vector(const rtc_float v[3]) noexcept : math_vector(v[0], v[1], v[2])
  {}

  constexpr explicit math_vector(const math_point& p) noexcept : e{p.x(), p.y(), p.z()} {}

  constexpr auto x() const noexcept { return e[0]; }
  constexpr auto y() const noexcept { return e[1]; }
  constexpr auto z() const noexcept { return e[2]; }

  constexpr auto begin() noexcept { return std::begin(e); }
  constexpr auto end() noexcept { return std::end(e); }

  constexpr auto cbegin() const noexcept { return std::cbegin(e); }
  constexpr auto cend() const noexcept { return std::cend(e); }

  constexpr auto& axis(rtc::axis v) noexcept { return e[int(v)]; }
  constexpr auto axis(rtc::axis v) const noexcept { return e[int(v)]; }

private:
  value_type e[3]{};
};

inline bool operator==(const math_vector& v1, const math_vector& v2) noexcept
{
  constexpr int eps = 1;
  return fequal(v1.x(), v2.x(), eps) && fequal(v1.y(), v2.y(), eps) && fequal(v1.z(), v2.z(), eps);
}

inline bool operator!=(const math_vector& v1, const math_vector& v2) noexcept
{
  return !(v1 == v2);
}

inline constexpr math_vector operator+(const math_vector& v1, const math_vector& v2) noexcept
{
  return {v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z()};
}

inline constexpr math_vector operator-(const math_vector& v1, const math_vector& v2) noexcept
{
  return {v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z()};
}

inline constexpr math_vector operator*(const math_vector& v, const rtc_float c) noexcept
{
  return {c * v.x(), c * v.y(), c * v.z()};
}

inline std::ostream& operator<<(std::ostream& s, const math_vector& v) noexcept
{
  return s << "[" << v.x() << ", " << v.y() << ", " << v.z() << "]v";
}

rtc_pure inline constexpr rtc_float dot(const math_vector& v1, const math_vector& v2) noexcept
{
  return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}

rtc_pure inline constexpr rtc_float lengthSQ(const math_vector& v) noexcept
{
  return dot(v, v);
}

inline rtc_float length(const math_vector& v) noexcept
{
  return std::sqrt(lengthSQ(v));
}

inline constexpr math_vector operator/(const math_vector& v, const rtc_float c) noexcept
{
  return {v.x()/c, v.y()/c, v.z()/c};
}

inline constexpr math_vector operator/(const rtc_float c, const math_vector& v) noexcept
{
  return { c/v.x(), c/v.y(), c/v.z() };
}

rtc_hot rtc_pure inline math_vector normalize(const math_vector& v) noexcept
{
  const auto length = lengthSQ(v);

  if(rtc_likely(length > 0.0f))
    return v / std::sqrt(length);

  return math_vector(std::numeric_limits<rtc_float>::max(),
                     std::numeric_limits<rtc_float>::max(),
                     std::numeric_limits<rtc_float>::max() );
}

rtc_hot rtc_pure inline constexpr math_vector cross(const math_vector& v1, const math_vector& v2) noexcept
{
  return { v1.y() * v2.z() - v1.z() * v2.y(),
           v1.z() * v2.x() - v1.x() * v2.z(),
           v1.x() * v2.y() - v1.y() * v2.x() };
}

inline constexpr math_vector operator*(const rtc_float c, const math_vector& v) noexcept
{
  return v * c;
}

inline math_vector operator-(const math_vector& v) noexcept
{
  return -1 * v;
}

inline constexpr math_point operator+(const math_vector& v, const math_point& p) noexcept
{
  return math_point{v.x() + p.x(), v.y() + p.y(), v.z() + p.z()};
}

inline constexpr math_vector operator-(const math_point& p1, const math_point& p2) noexcept
{
  return { p1.x() - p2.x(),
           p1.y() - p2.y(),
           p1.z() - p2.z() };
}

inline constexpr math_point operator+(const math_point& p, const math_vector& v) noexcept
{
  return v + p;
}

inline constexpr math_point operator-(const math_vector& v, const math_point& p) noexcept
{
  return {v.x() - p.x(), v.y() - p.y(), v.z() - p.z()};
}

inline constexpr math_point operator-(const math_point& p, const math_vector& v) noexcept
{
  return {p.x() - v.x(), p.y() - v.y(), p.z() - v.z()};
}

inline rtc_float cos(const math_vector& v1, const math_vector& v2) noexcept
{
  return dot(v1, v2) / (length(v1) * length(v2));
}

inline axis axis_of_biggest_vec_comp(const math_vector& v) noexcept
{
  const auto a = std::fabs(v.x());
  const auto b = std::fabs(v.y());
  const auto c = std::fabs(v.z());

  return a > b ? (a > c ? axis::x : axis::z) : (b > c ? axis::y : axis::z);
}

inline constexpr math_vector operator/(const math_point& p, const math_vector& v) noexcept
{
  return {p.x()/v.x(), p.y()/v.y(), p.z()/v.z()};
}

}

