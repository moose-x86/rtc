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
  constexpr intersection() noexcept = default;
  constexpr intersection(wildcard, rtc_float t) noexcept : object{std::in_place, 0, t} {}
  constexpr intersection(std::uint32_t i, rtc_float t) noexcept : object{std::in_place, i, t} {}

  [[nodiscard]] auto is_none() const noexcept -> bool;
  [[nodiscard]] auto is_present() const noexcept -> bool;

  explicit operator bool() const noexcept;
  [[nodiscard]] auto is_with(std::uint32_t) const noexcept -> bool;

  auto operator==(const intersection&) const noexcept -> bool;
  auto operator!=(const intersection&) const noexcept -> bool;
  auto operator<(const rtc::intersection&) const noexcept -> bool;
  auto operator-(const intersection&) const noexcept -> rtc_float;

  friend auto operator<<(std::ostream&, intersection&) noexcept -> std::ostream&;

  rtc_pure [[nodiscard]] auto triangle(const rtc::scene_model&) const noexcept -> const rtc::triangle3d&;
  rtc_pure [[nodiscard]] auto attribute(const rtc::scene_model& data) const noexcept -> const rtc::surface_material&;

  rtc_pure [[nodiscard]] auto reflect(const math_ray&, const rtc::scene_model&) const noexcept
      -> std::optional<math_ray>;
  rtc_pure [[nodiscard]] auto refract(const math_ray&, const rtc::scene_model&) const noexcept
      -> std::optional<math_ray>;

  [[nodiscard]] auto hit_point(const math_ray&) const noexcept -> math_point;
  [[nodiscard]] auto color(const rtc::scene_model&) const noexcept -> const rtc::color&;
  [[nodiscard]] auto normal_vector(const rtc::math_ray&, const rtc::scene_model&) const noexcept -> math_vector;

  rtc_pure [[nodiscard]] auto is_reflective(const rtc::scene_model&) const noexcept -> bool;
  rtc_pure [[nodiscard]] auto is_refractive(const rtc::scene_model&) const noexcept -> bool;

 private:
  rtc::optional_tuple<std::uint32_t, rtc_float> object;
};

constexpr intersection no_intersection{};
auto operator<<(std::ostream& s, intersection& i) noexcept -> std::ostream&;

}  // namespace rtc
