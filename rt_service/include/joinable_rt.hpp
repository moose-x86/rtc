#pragma once

#include <vector>
#include <future>

#include "rt_service.hpp"

namespace rtc
{

template<typename rt_service = ::rtc::rt_service<>>
class joinable_rt : rt_service, std::vector<std::shared_future<rtc::intersection>>
{
  using vector_base = std::vector<std::shared_future<rtc::intersection>>;

public:
  using trace_result = typename vector_base::value_type;
  using vector_base::size;
  using vector_base::clear;

  explicit joinable_rt(rt_service rt) : rt_service{std::move(rt)}
  {
    vector_base::reserve(rt_service::queue_capacity);
  }

  [[nodiscard]] rtc_hot rtc_inline auto trace_ray(const rtc::math_ray& r) -> trace_result
  {
    return vector_base::emplace_back(rt_service::trace_ray(r));
  }

  rtc_hot rtc_inline auto join() noexcept
  {
    /*protected rt_service::notify_scheduler(); */
    std::for_each(vector_base::begin(),
                  vector_base::end(),
                  [](const auto& f) noexcept { while(is_not_ready(f)) });
    vector_base::clear();
  }

  joinable_rt(joinable_rt&&) noexcept = default;
  auto operator=(joinable_rt&&) noexcept -> joinable_rt& = default;
  joinable_rt(const joinable_rt&) noexcept = delete;
  auto operator=(const joinable_rt&) noexcept -> joinable_rt& = delete;

  ~joinable_rt() noexcept
  {
    join();
  }

private:
  static auto is_not_ready(const trace_result& f) -> bool
  {
    constexpr std::chrono::nanoseconds zero{0};
    return f.valid() && (f.wait_for(zero) != std::future_status::ready);
  }
};

}
