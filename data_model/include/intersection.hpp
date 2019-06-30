#pragma once

#include <iomanip>

#include "math_ray.hpp"
#include "scene_model.hpp"
#include "utility.hpp"

namespace rtc
{
constexpr struct wildcard
{
  explicit wildcard() = default;
} _{};

class intersection
{
 public:
  constexpr intersection() noexcept {}
  constexpr intersection(wildcard, rtc_float t) noexcept : object{std::in_place, 0, t} {}
  constexpr intersection(std::uint32_t i, rtc_float t) noexcept : object{std::in_place, i, t} {}

  bool is_none() const noexcept;
  bool is_present() const noexcept;

  explicit operator bool() const noexcept;
  bool is_with(std::uint32_t) const noexcept;

  bool operator==(const intersection&) const noexcept;
  bool operator!=(const intersection&) const noexcept;
  bool operator<(const rtc::intersection&) const noexcept;
  rtc_float operator-(const intersection&) const noexcept;

  friend std::ostream& operator<<(std::ostream&, intersection&) noexcept;

  rtc_pure const rtc::triangle3d& triangle(const rtc::scene_model&) const noexcept;
  rtc_pure const rtc::surface_material& attribute(const rtc::scene_model& data) const noexcept;

  rtc_pure std::optional<math_ray> reflect(const math_ray&, const rtc::scene_model&) const noexcept;
  rtc_pure std::optional<math_ray> refract(const math_ray&, const rtc::scene_model&) const noexcept;

  math_point hit_point(const math_ray&) const noexcept;
  const rtc::color& color(const rtc::scene_model&) const noexcept;
  math_vector normal_vector(const rtc::math_ray&, const rtc::scene_model&) const noexcept;

  rtc_pure bool is_reflective(const rtc::scene_model&) const noexcept;
  rtc_pure bool is_refractive(const rtc::scene_model&) const noexcept;

 private:
  rtc::optional_tuple<std::uint32_t, rtc_float> object;
};

constexpr intersection no_intersection{};
std::ostream& operator<<(std::ostream& s, intersection& i) noexcept;

}  // namespace rtc
