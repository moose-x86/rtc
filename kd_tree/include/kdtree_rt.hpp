#include "kd_tree.hpp"
#include "ray_tracer.hpp"

namespace rtc
{

struct kdtree_rt
{
  using storage = ::rtc::kd_tree;
  using rt_search = ::rtc::ray_tracer<::rtc::kd_tree>;

  static constexpr bool is_thread_safe{true};
};

}
