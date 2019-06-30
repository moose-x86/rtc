#include <memory>

#include "kd_tree.hpp"
#include "ray_tracer.hpp"
#include "scene_model.hpp"

namespace rtc
{
struct kdtree_rt : private rtc::ray_tracer<rtc::kd_tree>
{
  static constexpr bool is_thread_safe{true};

 private:
  using base_t = rtc::ray_tracer<rtc::kd_tree>;

 public:
  explicit kdtree_rt(const std::shared_ptr<const rtc::scene_model>& sc) : base_t{rtc::kd_tree{*sc}, sc} {}

  using base_t::trace_ray;
};

}  // namespace rtc
