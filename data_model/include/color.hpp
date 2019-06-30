#pragma once

#include <array>
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

  constexpr auto red() noexcept -> auto& { return c[0]; }
  constexpr auto green() noexcept -> auto& { return c[1]; }
  constexpr auto blue() noexcept -> auto& { return c[2]; }
  [[nodiscard]] constexpr auto red() const noexcept { return c[0]; }
  [[nodiscard]] constexpr auto green() const noexcept { return c[1]; }
  [[nodiscard]] constexpr auto blue() const noexcept { return c[2]; }

  template <typename T>
  constexpr auto red() const noexcept
  {
    return static_cast<T>(red());
  }
  template <typename T>
  constexpr auto green() const noexcept
  {
    return static_cast<T>(green());
  }
  template <typename T>
  constexpr auto blue() const noexcept
  {
    return static_cast<T>(blue());
  }

  [[nodiscard]] constexpr auto begin() const noexcept { return std::begin(c); }
  [[nodiscard]] constexpr auto end() const noexcept { return std::end(c); }

  [[nodiscard]] constexpr auto is_normalized() const noexcept -> bool
  {
    return (red() > 0.F) && (red() <= 1.F) && (green() > 0.F) && (green() <= 1.F) && (blue() > 0.F) && (blue() <= 1.F);
  }

 private:
  std::array<value_type, 3> c{};
};

inline auto operator==(const color& v1, const color& v2) noexcept -> bool
{
  return fequal(v1.red(), v2.red()) && fequal(v1.green(), v2.green()) && fequal(v1.blue(), v2.blue());
}

inline auto operator!=(const color& v1, const color& v2) noexcept -> bool { return !(v1 == v2); }

inline auto operator<<(std::ostream& s, const color& c) noexcept -> std::ostream&
{
  return s << "[" << c.red() << ", " << c.green() << ", " << c.blue() << "]c";
}

inline auto operator*(const rtc_float c, const color& col) noexcept -> color
{
  return {c * col.red(), c * col.green(), c * col.blue()};
}

inline auto operator+(const color& c1, const color& c2) noexcept -> color
{
  return {c1.red() + c2.red(), c1.green() + c2.green(), c1.blue() + c2.blue()};
}

inline auto operator+=(color& c1, const color& c2) noexcept -> color& { return (c1 = c1 + c2); }

inline auto operator*(const color& c1, const color& c2) noexcept -> color
{
  return {c1.red() * c2.red(), c1.green() * c2.green(), c1.blue() * c2.blue()};
}

inline auto clamp_normal(const color& c) noexcept -> rtc::color
{
  rtc::color cr{};

  cr.red() = std::clamp(c.red(), rtc_float{}, rtc_float{1.0});
  cr.green() = std::clamp(c.green(), rtc_float{}, rtc_float{1.0});
  cr.blue() = std::clamp(c.blue(), rtc_float{}, rtc_float{1.0});

  return cr;
}

inline auto clamp(const color& c) noexcept -> rtc::color
{
  rtc::color cr{};

  cr.red() = std::clamp(c.red(), rtc_float{}, rtc_float{255.0});
  cr.green() = std::clamp(c.green(), rtc_float{}, rtc_float{255.0});
  cr.blue() = std::clamp(c.blue(), rtc_float{}, rtc_float{255.0});

  return cr;
}

constexpr color black{};
constexpr color white{255, 255, 255};

}  // namespace rtc
