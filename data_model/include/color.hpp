#pragma once
#include <cmath>
#include <iostream>
#include "float_compare.hpp"

namespace rtc
{

struct color
{
  using value_type = rtc_float;

  constexpr color() noexcept = default;
  constexpr color(value_type _a, value_type _b, value_type _c) noexcept : c{_a, _b, _c} {}

  constexpr auto& red() noexcept   { return c[0]; }
  constexpr auto& green() noexcept { return c[1]; }
  constexpr auto& blue() noexcept  { return c[2]; }
  constexpr auto red() const noexcept   { return c[0]; }
  constexpr auto green() const noexcept { return c[1]; }
  constexpr auto blue() const  noexcept { return c[2]; }

  template<typename T>
  constexpr auto red() const noexcept { return static_cast<T>(red()); }
  template<typename T>
  constexpr auto green() const noexcept { return static_cast<T>(green()); }
  template<typename T>
  constexpr auto blue() const noexcept { return static_cast<T>(blue()); }

  constexpr auto begin() const noexcept { return std::begin(c); }
  constexpr auto end() const noexcept { return std::end(c); }

  constexpr bool is_normalized() const noexcept
  {
    return (red() > 0.f) && (red() <= 1.f) && (green() > 0.f) && (green() <= 1.f) &&
           (blue() > 0.f) && (blue() <= 1.f);
  }

private:
  value_type c[3]{};
};

inline bool operator==(const color& v1, const color& v2) noexcept
{
  return fequal(v1.red(), v2.red()) && fequal(v1.green(), v2.green()) && fequal(v1.blue(), v2.blue());
}

inline bool operator!=(const color& v1, const color& v2) noexcept
{
  return !(v1 == v2);
}

inline std::ostream& operator<<(std::ostream& s, const color& c) noexcept
{
  return s << "[" << c.red() << ", " << c.green() << ", " << c.blue() << "]c";
}

inline color operator*(const rtc_float c, const color& col) noexcept
{
  return { c * col.red(), c * col.green(), c * col.blue() };
}

inline color operator+(const color& c1, const color& c2) noexcept
{
  return { c1.red() +  c2.red(), c1.green() + c2.green(), c1.blue()  + c2.blue() };
}

inline color& operator+=(color& c1, const color& c2) noexcept
{
  return (c1 = c1 + c2);
}

inline color operator*(const color& c1, const color& c2) noexcept
{
  return { c1.red() *  c2.red(), c1.green() * c2.green(), c1.blue()  * c2.blue() };
}

inline rtc::color clamp_normal(const color& c) noexcept
{
  rtc::color cr{};

  cr.red() = std::clamp(c.red(), rtc_float{}, rtc_float{1.0});
  cr.green() = std::clamp(c.green(), rtc_float{}, rtc_float{1.0});
  cr.blue() = std::clamp(c.blue(), rtc_float{}, rtc_float{1.0});

  return cr;
}

inline rtc::color clamp(const color& c) noexcept
{
  rtc::color cr{};

  cr.red() = std::clamp(c.red(), rtc_float{}, rtc_float{255.0});
  cr.green() = std::clamp(c.green(), rtc_float{}, rtc_float{255.0});
  cr.blue() = std::clamp(c.blue(), rtc_float{}, rtc_float{255.0});

  return cr;
}


constexpr color black{};
constexpr color white{255, 255, 255};

} // namespace rtc
