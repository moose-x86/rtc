#include "rtc/image_composer.hpp"
#include "rtc/gpu/uniform_grid.hpp"
#include "rtc/ray_tracer.hpp"

rtc::br b{"room.xml"};
rtc::graphics_view</*typename T = rtc::backward<>*/> img{std::ref(b)}; //ref returns reference_wrapper

using basic = rtc::backward
<
 rtc::kdtree_rt,
 rtc::disitributed_ray_tracing_lights<>,
 rtc::color_compute_algorithm<>
>; //algorithm for using backtracing

struct rtc::uniformgrid_rt
{
  using storage = ...;
  using rt_search = ....;

  bool is_thread_safe{true|false};
};

struct rtc::kdtree_rt
{
  using storage = ...;
  using rt_search = ....;

  bool is_thread_safe{true|false};
};

struct rtc::cl::kdtree_rt
{
  using storage = ...;
  using rt_search = ....;

  bool is_thread_safe{true|false};
};

/**
 * ray_tracer creates tracing_data_type and tracing_algorithm_type
 * creates cpu threads if possible and uses queue to coordinate work between threads
 * if gpu - thread should be one and algorithm should be designed in such way
 * that if algorithm is gpu it should buffer work until it is able to run it on graphic card
 * cpu algorithm can not buffer - just compute on demand
 */
using basic_forward = rtc::forward
<
  rtc::kdtree_rt,
  rtc::disitributed_ray_tracing_lights<>,
  rtc::color_compute_algorithm<>
>;

using gpu_forward = rtc::backward
<
  rtc::cl::kdtree_rt,
  rtc::disitributed_ray_tracing_lights<>,
  rtc::color_compute_algorithm<>
>;

rtc::graphics_view<basic_forward> cpu_gv{};
rtc::graphics_view<gpu_forward> cl_gv{};

auto bitmap = cpu_gv.bitmap();
auto f = cpu_gv.future_bitmap();
auto g = f.get();

rtc::aligned_model bb{std::move(b)};
rtc::rt_service<rtc::cl::kdtree_rt> r{std::ref(bb)};
rtc::rt_service<> r{std::ref(bb)};
rtc::rt_service<rtc::uniformgrid_rt> r{std::ref(bb)};

rtc::math_ray ray{rtc::math_vector{1, 1, 1}, rtc::math_point{1, 1, 1}};
std::future<rtc::intersection> triangle_from_brs = r.trace_ray(rtc::async, ray, rtc::normal_ray);
std::future<rtc::intersection> triangle_from_brs = r.trace_ray(rtc::deffered, ray, rtc::normal_ray);

bool is_delayed = r.trace_ray([](auto& o){/*callback*/}. ray, rtc::normal_ray);

rtc::color_compute<> color{};
rtc::disitributed_rt_lights<> light{std::ref(bb), std::ref(r)};

/*
 * is is object which will be used by image_composer, which holds rtc::backward/forward which keep state of processing
 * there are many pixel_composers but only one ray_tracer
 */
auto p = rtc::make_pixel_composer(std::ref(b),
                                  std::ref(light),
                                  std::ref(color));

auto pixel = p.make_color(rtc::bitmap_pixel{10, 10},
                          rtc::black_color,
                          rtc::incremental);

for(auto next_p = p;
    nextp != rtc::back_color;
    next_p = p.next_traced_pixel());

try
{
  for(auto next_p = p; nextp; next_p = p.next_traced_pixel(rtc::exception));
}
catch(const reached_limit_of_tracing&)
{}


