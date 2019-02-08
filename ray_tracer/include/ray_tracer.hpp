#pragma once

#include <memory>
#include <optional>

#include "scene_model.hpp"
#include "math_plane.hpp"
#include "point_in_triangle_test.hpp"
#include "utility.hpp"
#include "intersection.hpp"
#include "begin_end_concept.hpp"
#include "utility.hpp"
#include "rtc_log.hpp"
#include "fast_pow.hpp"

namespace rtc
{

template<typename _storage>
class ray_tracer
{
  static_assert(has_ray_begin_and_end_v<_storage>, "Storage type, must have begin & end iterator");
  using shared_scene = std::shared_ptr<const rtc::scene_model>;

public:
  ray_tracer(_storage s, shared_scene sc) noexcept : storage{ std::move(s) }, scene{ std::move(sc) }
  {}

  rtc_hot auto trace_ray(const rtc::math_ray&) const noexcept -> rtc::intersection;

private:
  _storage storage;
  shared_scene scene;

  rtc_hot auto compute_intersection(
		             const rtc::math_ray&,
		             const std::uint32_t,
		             const rtc_float) const noexcept -> std::tuple<rtc_float, bool>;

};

template<typename T>
auto ray_tracer<T>::compute_intersection(
                           const rtc::math_ray& ray,
                           const std::uint32_t triangle_index,
                           const rtc_float ray_triangle_hit_value) const noexcept -> std::tuple<rtc_float, bool>
{
  static auto create_isect_test = [](const auto& p, const auto& t, const auto ti) {
    return point_in_triangle_test{ p[t[ti].vertex_a()], p[t[ti].vertex_b()], p[t[ti].vertex_c()] };
  };

  static auto create_plane_test = [](const auto& p, const auto& t, const auto ti) {
    return rtc::math_plane{ p[t[ti].vertex_a()], p[t[ti].vertex_b()], p[t[ti].vertex_c()] };
  };

  const auto t_temp = create_plane_test(scene->points,
                                        scene->triangles,
                                        triangle_index).intersection_value(ray);

  assert(!std::isnan(t_temp));

  constexpr auto ulp{3};
  constexpr rtc_float dt = 5.0f/rtc::pow(10.0f, std::numeric_limits<rtc_float>::digits10 - 1);

  if(fgreater(t_temp, dt, ulp) && fless(t_temp, ray_triangle_hit_value, ulp))
  {
    const auto pt = create_isect_test(scene->points, scene->triangles, triangle_index);
    return {t_temp, pt.triangle_contains(ray[t_temp])};
  }

  return {{}, false};
}

template<typename T>
auto ray_tracer<T>::trace_ray(const rtc::math_ray& ray) const noexcept -> rtc::intersection
{
  rtc::intersection intersect{};
  auto current_hit = std::numeric_limits<rtc_float>::max();

  for(auto e = storage.cbegin(ray); e != storage.cend(ray); ++e)
  {
    for(const auto ti : *e)
    {
      auto [new_hit, computed] = compute_intersection(ray, ti, current_hit);
      if(computed)
      {
        e.triangle_hit_value(current_hit = new_hit);
        intersect = { ti, current_hit };
      }
    }
  }

  return intersect;
}

}
