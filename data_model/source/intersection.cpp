#include "intersection.hpp"

namespace rtc
{

math_point intersection::hit_point(const math_ray& ray) const noexcept
{
  return ray[ std::get<1>(object.value()) ];
}

const rtc::color& intersection::color(const rtc::scene_model& data) const noexcept
{
  return attribute(data).material_color;
}

math_vector intersection::normal_vector(const rtc::math_ray& r,
                                        const rtc::scene_model& data) const noexcept
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
    if(cos_n < 0.0f)
       n = -n;

    //data.normals[std::get<0>(object.value())] = n;
    return n;
  }

 // return data.normals[std::get<0>(object.value())];
}

bool intersection::is_present() const noexcept
{
  return object.has_value();
}

bool intersection::is_none() const noexcept
{
  return !is_present();
}

intersection::operator bool() const noexcept
{
  return is_present();
}

bool intersection::is_reflective(const rtc::scene_model& data) const noexcept
{
  return attribute(data).mirror || (attribute(data).ks != 0.0f);
}

bool intersection::is_refractive(const rtc::scene_model& data) const noexcept
{
  return attribute(data).kts != 0.0f;
}

rtc_pure std::optional<math_ray> intersection::reflect(const math_ray& r, const rtc::scene_model& sc) const noexcept
{
  if(is_reflective(sc))
  {
    const auto n = normal_vector(r, sc);
    const auto l = normalize(-r.direction());

    return {{ 2.0f * dot(l, n) * n - l, hit_point(r) }};
  }

  return std::nullopt;
}

rtc_pure std::optional<math_ray> intersection::refract(const math_ray& r, const rtc::scene_model& sc) const noexcept
{
  if(is_refractive(sc))
  {
    const auto eta{1/attribute(sc).eta};
    const auto n{normal_vector(r, sc)};
    const auto v{normalize(-r.direction())};

    const auto cosThetaI{dot(v, n)};
    const auto sin2ThetaI{std::max<rtc_float>(0.f, 1.f - cosThetaI * cosThetaI)};
    const auto sin2ThetaT{eta * eta * sin2ThetaI};

    if(sin2ThetaT < 1)
    {
      const auto cosThetaT = std::sqrt(1.f - sin2ThetaT);
      return {{ eta*(-v) + (eta * cosThetaI - cosThetaT) * n, hit_point(r) }};
    }
  }

  return std::nullopt;
}

rtc_pure const rtc::triangle3d& intersection::triangle(const rtc::scene_model& data) const noexcept
{
  return data.triangles[std::get<0>(object.value())];
}

rtc_pure const rtc::surface_material& intersection::attribute(const rtc::scene_model& data) const noexcept
{
  assert(std::get<0>(object.value()) < data.triangles.size());
  assert(data.material_id[std::get<0>(object.value())] < data.materials.size());
  return data.materials[data.material_id[std::get<0>(object.value())]];
}

bool intersection::is_with(std::uint32_t t) const noexcept
{
  return is_present() && (std::get<0>(object.value()) == t);
}

bool intersection::operator==(const intersection& i) const noexcept
{
   return object == i.object;
}

bool intersection::operator!=(const intersection& i) const noexcept
{
   return object != i.object;
}

bool intersection::operator<(const rtc::intersection& i) const noexcept
{
  return std::get<1>(object.value()) < std::get<1>(i.object.value());
}

rtc_float intersection::operator-(const intersection& i) const noexcept
{
  return std::get<1>(i.object.value()) - std::get<1>(object.value());
}

std::ostream& operator<<(std::ostream& s, intersection& i) noexcept
{
  if(i)
    return s << "[triangle: " << std::get<0>(i.object.value()) << ", intersect_value: " << std::get<1>(i.object.value()) << "]";

  return s << "[no value]";
}

}
