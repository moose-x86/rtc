#pragma once

#include <functional>
#include <memory>
#include <optional>

#include "bitmap.hpp"
#include "color.hpp"
#include "fast_pow.hpp"
#include "scene_model.hpp"
#include "utility.hpp"

namespace rtc
{
template <typename rt_types, typename shadows, typename C>
struct backward_al
{
  using rt_paramters = rt_types;
  using rt_shadows = shadows;
  using rt_color = C;
  using optional_color = typename rt_shadows::optional_color;

  explicit backward_al(std::shared_ptr<const rtc::scene_model> s) : scene{std::move(s)}, rt_s{scene} {}

  auto prework(rtc::bitmap&) {}
  auto postwork(rtc::bitmap&) {}

  template <std::size_t depth = 10, typename rt_serv>
  [[nodiscard]] auto make_color(const rtc::math_ray& ray, const rtc::color& background, rt_serv& rt) noexcept
      -> rtc::color
  {
    if constexpr (depth > 0)
    {
      auto future_intersect = rt.trace_ray(ray);

      if (auto object = future_intersect.get())
      {
        optional_color reflect{}, refract{};
#if 1
        if (auto r = object.refract(ray, *scene))
          refract = make_color<depth - 1>(r.value(), background, rt);
#endif

#if 1
        if (auto r = object.reflect(ray, *scene))
          reflect = make_color<depth - 1>(r.value(), background, rt);
#endif

        return rt_s.compute_color(ray, object, reflect, refract, rt);
      }
    }

    return background;
  }

 private:
  const std::shared_ptr<const rtc::scene_model> scene;
  rt_shadows rt_s;
};

}  // namespace rtc
