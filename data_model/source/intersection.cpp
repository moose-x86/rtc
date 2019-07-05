#include "intersection.hpp"

namespace rtc
{

auto intersection::hit_point(const math_ray& ray) const noexcept -> math_point
{
  return ray[ std::get<1>(object.value()) ];
}

auto intersection::color(const rtc::scene_model& data) const noexcept -> const rtc::color&
{
  return attribute(data).material_color;
}

auto intersection::normal_vector(const rtc::math_ray& r,
                                        const rtc::scene_model& data) const noexcept -> math_vector
{
  //TODO: this is only temp solution, as those normals should be stored in
  //      brs.xml file
//  if(data.normals[std::get<0>(object.value())] == rtc::math_vector{})
  {
    const auto& tr = data.triangles[std::get<0>(object.value())];
    const auto& p1 = data.points[tr.vertex_a()];
    const auto& p2 = data.points[tr.vertex_b()];
    const auto& p3 = data.points[tr.vertex_c()];

    auto n = normalize(cross(p1 - p2, p3 - p2));
    auto v = normalize(-r.direction());

    const auto cos_n = dot(n, v);
    if(cos_n < 0.0F)
       n = -n;

    //data.normals[std::get<0>(object.value())] = n;
    return n;
  }

 // return data.normals[std::get<0>(object.value())];
}

auto intersection::is_present() const noexcept -> bool
{
  return object.has_value();
}

auto intersection::is_none() const noexcept -> bool
{
  return !is_present();
}

intersection::operator bool() const noexcept
{
  return is_present();
}

auto intersection::is_reflective(const rtc::scene_model& data) const noexcept -> bool
{
  return attribute(data).mirror || (attribute(data).ks != 0.0F);
}

auto intersection::is_refractive(const rtc::scene_model& data) const noexcept -> bool
{
  return attribute(data).kts != 0.0F;
}

rtc_pure auto intersection::reflect(const math_ray& r, const rtc::scene_model& sc) const noexcept -> std::optional<math_ray>
{
  if(is_reflective(sc))
  {
    const auto n = normal_vector(r, sc);
    const auto l = normalize(-r.direction());

    return {{ 2.0F * dot(l, n) * n - l, hit_point(r) }};
  }

  return std::nullopt;
}

rtc_pure auto intersection::refract(const math_ray& r, const rtc::scene_model& sc) const noexcept -> std::optional<math_ray>
{
  if(is_refractive(sc))
  {
    const auto eta{1/attribute(sc).eta};
    const auto n{normal_vector(r, sc)};
    const auto v{normalize(-r.direction())};

    const auto cosThetaI{dot(v, n)};
    const auto sin2ThetaI{std::max<rtc_float>(0.F, 1.F - cosThetaI * cosThetaI)};
    const auto sin2ThetaT{eta * eta * sin2ThetaI};

    if(sin2ThetaT < 1)
    {
      const auto cosThetaT = std::sqrt(1.F - sin2ThetaT);
      return {{ eta*(-v) + (eta * cosThetaI - cosThetaT) * n, hit_point(r) }};
    }
  }

  return std::nullopt;
}

rtc_pure auto intersection::triangle(const rtc::scene_model& data) const noexcept -> const rtc::triangle3d&
{
  return data.triangles[std::get<0>(object.value())];
}

rtc_pure auto intersection::attribute(const rtc::scene_model& data) const noexcept -> const rtc::surface_material&
{
  assert(std::get<0>(object.value()) < data.triangles.size());
  assert(data.material_id[std::get<0>(object.value())] < data.materials.size());

  return data.materials[data.material_id[std::get<0>(object.value())]];
}

auto intersection::is_with(std::uint32_t t) const noexcept -> bool
{
  return is_present() && (std::get<0>(object.value()) == t);
}

auto intersection::operator==(const intersection& i) const noexcept -> bool
{
   return object == i.object;
}

auto intersection::operator!=(const intersection& i) const noexcept -> bool
{
   return object != i.object;
}

auto intersection::operator<(const rtc::intersection& i) const noexcept -> bool
{
  return std::get<1>(object.value()) < std::get<1>(i.object.value());
}

auto intersection::operator-(const intersection& i) const noexcept -> rtc_float
{
  return std::get<1>(i.object.value()) - std::get<1>(object.value());
}

auto operator<<(std::ostream& s, intersection& i) noexcept -> std::ostream&
{
  if(i.is_present())
    return s << "[triangle: " << std::get<0>(i.object.value()) << ", value: " << std::get<1>(i.object.value()) << "]";

  return s << "[no value]";
}

}
