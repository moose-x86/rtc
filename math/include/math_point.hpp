#pragma once

#include <ostream>
#include <iterator>
#include <limits>
#include <cmath>
#include <tuple>
#include "float_compare.hpp"
#include "math_axis.hpp"

namespace rtc
{

class math_point
{
public:
  using value_type = rtc_float;

  constexpr math_point() noexcept = default;
  constexpr math_point(rtc_float x, rtc_float y, rtc_float z) noexcept: e{x, y, z}
  {}

  constexpr auto begin() const noexcept { return std::cbegin(e); }
  constexpr auto end() const noexcept { return std::cend(e); }

  constexpr auto& x() noexcept { return e[0]; }
  constexpr auto& y() noexcept { return e[1]; }
  constexpr auto& z() noexcept { return e[2]; }
  constexpr auto x() const noexcept { return e[0]; }
  constexpr auto y() const noexcept { return e[1]; }
  constexpr auto z() const noexcept { return e[2]; }

  constexpr auto& axis(rtc::axis v) noexcept { return e[int(v)]; }
  constexpr auto axis(rtc::axis v) const noexcept { return e[int(v)]; }

private:
  value_type e[3]{};
};

inline bool operator==(const math_point& v1, const math_point& v2) noexcept
{
    return fequal(v1.x(), v2.x()) &&
           fequal(v1.y(), v2.y()) &&
           fequal(v1.z(), v2.z());
}

inline bool operator!=(const math_point& p1, const math_point& p2) noexcept
{
  return !(p1 == p2);
}

inline std::ostream& operator<<(std::ostream& s, const math_point& p)
{
  return s << "[" << p.x() << ", " << p.y() << ", " << p.z() << "]p";
}

constexpr inline math_point operator+(const math_point& p1, const math_point& p2) noexcept
{
  return {p1.x() + p2.x(), p1.y() + p2.y(), p1.z() + p2.z()};
}

constexpr inline math_point operator*(const rtc_float c, const math_point& p2) noexcept
{
  return {c * p2.x(), c * p2.y(), c * p2.z()};
}

constexpr inline math_point operator/(const math_point& p1, const rtc_float c) noexcept
{
  return {p1.x()/c, p1.y()/c, p1.z()/c};
}

constexpr inline bool operator<(const math_point& p1, const math_point& p2) noexcept
{
   return std::make_tuple(p1.x(), p1.y(), p1.z()) < std::make_tuple(p2.x(), p2.y(), p2.z());
}

}
