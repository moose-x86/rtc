#pragma once

#include <color.hpp>
#include <cstring>

namespace rtc
{

struct surface_material
{
  rtc_float kd{.1f};
  rtc_float ktd{};
  rtc_float ks{};
  rtc_float kts{};
  rtc_float ka{0.3};
  rtc_float kf{};
  rtc_float gs{};
  rtc_float gm{};
  rtc_float selfLuminance{};
  rtc_float heatLam{};
  rtc_float ft{};
  rtc_float eta{1};

  //rtc_float ambient_coef{.1};
  //rtc_float diffuse_coef{.1};
  //rtc_float specular_coef{};
  //rtc_float reflective_coef{};
  //rtc_float refractive_coef{};
  //rtc_float refractive_eta{1};
  //std::uint32_t phong_pow{300};
  //brdf_type brdf{};

  bool oneSheet{};
  bool reflection{};
  bool indirect{};
  bool mirror{};
  bool shadowcast{true};
  bool shadowfall{true};

  color material_color{};

  enum class shading_type
  {
    flat  = 1,
    guard = 2,
    phong = 4
  } shading{};
};


inline bool operator==(const surface_material& s1, const surface_material& s2) noexcept
{
  static_assert(std::is_standard_layout<surface_material>::value,
                "This operator require sm to be standard layout");

  return std::memcmp(std::addressof(s1), std::addressof(s2), sizeof(s1)) == 0;
}

inline bool operator!=(const surface_material& s1, const surface_material& s2) noexcept
{
  return !(s1 == s2);
}

inline std::ostream& operator<<(std::ostream& s, const surface_material& sm)
{
  return s << "["
           << "kd: " << sm.kd
           << ", ktd: " << sm.ktd
           << ", ks: " << sm.ks
           << ", kts: " << sm.kts
           << ", ka: " << sm.ka
           << ", kf: " << sm.kf
           << ", gs: " << sm.gs
           << ", gm: " << sm.gm
           << ", selfLuminance: " << sm.selfLuminance
           << ", heatLam: " << sm.heatLam
           << ", ft: " << sm.ft
           << ", eta: " << sm.eta
           << ", oneSheet: " << std::boolalpha << sm.oneSheet
           << ", reflection: " << sm.reflection
           << ", indirect: " << sm.indirect
           << ", mirror: " << sm.mirror
           << ", shadowcast: " << sm.shadowcast
           << ", shadowfall: " << sm.shadowfall
           << ", color: " << sm.material_color
           << "]sm";
}


} //namespace libRTC

