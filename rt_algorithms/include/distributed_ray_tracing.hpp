#pragma once

#include <memory>
#include <optional>

#include "color.hpp"
#include "fast_pow.hpp"
#include "intersection.hpp"
#include "math_ray.hpp"

namespace rtc
{
template <typename = void>
struct distributed_ray_tracing_shadows
{
 public:
  using optional_color = std::optional<rtc::color>;
  explicit distributed_ray_tracing_shadows(std::shared_ptr<const rtc::scene_model> sc) : scene(std::move(sc)) {}

  template <typename rt_serv>
  auto compute_color(const rtc::math_ray& ray,
                     const rtc::intersection& object,
                     const optional_color& reflected,
                     const optional_color& refracted,
                     rt_serv& rt) -> rtc::color
  {
    const auto& m = object.attribute(*scene);
    const auto n = object.normal_vector(ray, *scene);

#if 1
    rtc::color illumination{m.ka * scene->ambient};
    for (const auto& light : scene->lights)
    {
      // TODO: Extract this into rtc::shadow_ray class probably with ctor which takes reference to
      //      Illumination
      const auto l = light.position - object.hit_point(ray);

      if ((cos(n, l) > 0) || object.is_refractive(*scene))
      {
        const auto [i, f] = get_intersection_with_light_ray(object, ray, l, light, rt);

        if (i.is_none() || !(i < intersection(_, 1.0F)))
        {
          const auto nl = normalize(l);
          const auto R = 2.0F * dot(nl, n) * n - nl;

          const auto dot_ln = dot(n, nl) < 0 ? m.kts * dot(nl, -n) : dot(nl, n);
          const auto V = normalize(scene->optical_system.view_point - object.hit_point(ray));
          const auto d = inverse_square_factor(light, l);

          illumination += f * ((m.kd * dot_ln + m.ks * rtc::pow(dot(R, V), 300)) / d) * light.light_color;
        }
      }
    }
#else
    rtc::color illumination{1, 1, 1};
#endif

    rtc::color r = rtc::clamp(object.color(*scene) * illumination);

    if (refracted)
      r = (1 - m.kts) * r + m.kts * refracted.value();

    if (reflected)
      r = !m.mirror ? ((1.0F - m.ks) * r + m.ks * reflected.value()) : reflected.value();

    return r;
  }

 private:
  const std::shared_ptr<const rtc::scene_model> scene;

  template <typename _rt>
  auto get_intersection_with_light_ray(const rtc::intersection& object,
                                       const rtc::math_ray& ray,
                                       const rtc::math_vector& L,
                                       const rtc::light& light,
                                       _rt& rt) const -> std::tuple<intersection, float>
  {
    rtc::math_ray shadow_ray{L, object.hit_point(ray)};
    rtc::intersection intersect{}, priv{object};
    rtc_float acc{1};
    const auto length = rtc::length(L);

    do
    {
      if (intersect)
      {
        const auto ray_hit = intersect.hit_point(shadow_ray);
        shadow_ray = {light.position - ray_hit, ray_hit};

        acc *= std::pow(0.5 * (intersect.attribute(*scene).kts + intersect.attribute(*scene).ktd),
                        length * (intersect - priv));
        priv = intersect;
      }

      intersect = rt.trace_ray(shadow_ray).get();
    } while (intersect.is_present() && intersect.is_refractive(*scene) && intersect < intersection(_, 1.0F));

    assert(intersect.is_present() && intersect.is_refractive(*scene) && !(intersect < intersection{_, 1.0F}) ||
           intersect.is_present() && !intersect.is_refractive(*scene) && !(intersect < intersection{_, 1.0F}) ||
           intersect.is_present() && !intersect.is_refractive(*scene) && (intersect < intersection{_, 1.0F}) ||
           intersect.is_none());

    return {intersect, std::clamp<rtc_float>(acc, 0, 1)};
  }
};

}  // namespace rtc
