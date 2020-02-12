#include <boost/fiber/algo/round_robin.hpp>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/operations.hpp>
#include <future>
#include <memory>

#include "backward_al.hpp"
#include "bitmap.hpp"
#include "distributed_ray_tracing.hpp"
#include "joinable_rt.hpp"
#include "optical_camera_plane.hpp"
#include "scene_model.hpp"
#include "scoped_timer.hpp"

namespace rtc
{
template <typename rt_algorithm = backward_al<::rtc::kdtree_rt, distributed_ray_tracing_shadows<>, void>>
class render_engine
{
 public:
  explicit render_engine(std::shared_ptr<const rtc::scene_model>);

  render_engine(render_engine&&) noexcept = default;
  render_engine(const render_engine&) = delete;
  auto operator=(render_engine&&) noexcept -> render_engine& = default;
  auto operator=(const render_engine&) -> render_engine& = delete;

  rtc_hot auto bitmap() -> rtc::bitmap;
  ~render_engine() = default;

 private:
  using rt_service = ::rtc::rt_service<typename rt_algorithm::rt_paramters>;
  using joinable_rt = rtc::joinable_rt<rt_service>;

  static constexpr std::uint16_t fiber_num{rt_service::queue_capacity};
  struct : joinable_rt
  {
    template <typename>
    friend class render_engine;

   public:
    using joinable_rt::joinable_rt;
    [[nodiscard]] rtc_hot rtc_inline auto trace_ray(const rtc::math_ray& r)
    {
      auto f = joinable_rt::trace_ray(r);

      if (joinable_rt::size() >= fiber_num)
      {
        this->join();
      }

      boost::this_fiber::yield();
      return f;
    }

   private:
    auto join()
    {
      if (!rt_algorithm::rt_paramters::is_thread_safe)
      {
        joinable_rt::join();
      }

      joinable_rt::clear();
    }
  } join_rt;

  const std::shared_ptr<const rtc::scene_model> scene;

  auto make_rgb(const rtc::color& c) -> rtc::color_rgb
  {
    // assert(!c.is_normalized());

    using type = rtc::color_rgb::value_type;
    return {c.red<type>(), c.green<type>(), c.blue<type>()};
  }
};

template <typename T>
render_engine<T>::render_engine(std::shared_ptr<const rtc::scene_model> s) : join_rt{rt_service{s}}, scene{std::move(s)}
{
}

template <typename rt_algorithm>
auto render_engine<rt_algorithm>::bitmap() -> rtc::bitmap
{
  using namespace boost::fibers;
  const auto& res = scene->optical_system.screen.resolution;

  use_scheduling_algorithm<algo::round_robin>();

  std::uint32_t pindex{};
  rtc::bitmap bmp(res.x, res.y);
  std::array<fiber, fiber_num> fibers{};
  const rtc::optical_camera_plane op{scene->optical_system};

  rt_algorithm rt_alg{scene};
  rt_alg.prework(bmp);

  auto fiber_fn = [&] {
    while (rtc_likely(pindex < bmp.pixel_amount()))
    {
      auto pixel{bmp.begin() + (pindex++)};
      auto primary{op.emit_ray(pixel->x, pixel->y)};

      auto c = rt_alg.make_color(primary, rtc::black, join_rt);

      DEBUG << "pixel[" << pixel->x << "," << pixel->y << "]"
            << "ray " << primary.direction() << " color: " << c;
      bmp.assign(pixel->x, pixel->y, make_rgb(c));
    }
  };

  std::generate(fibers.begin(), fibers.end(), [&] { return fiber{launch::post, fiber_fn}; });

  std::for_each(fibers.begin(), fibers.end(), [](auto& f) { f.join(); });
  rt_alg.postwork(bmp);

  return bmp;
}

}  // namespace rtc
