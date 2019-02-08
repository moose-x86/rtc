#pragma once

#include <tuple>
#include <memory>
#include <future>
#include <optional>
#include <utility>
#include <array>
#include <cassert>
#include <random>
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>

#include "kdtree_rt.hpp"
#include "math_ray.hpp"
#include "scene_model.hpp"
#include "rtc_log.hpp"
#include "scoped_timer.hpp"
#include "utility.hpp"
#include "intersection.hpp"

namespace rtc
{

template<typename _rt = ::rtc::kdtree_rt>
class rt_service
{
  template<typename T>
  using thread_safe_queue = boost::lockfree::queue<T, boost::lockfree::fixed_sized<true>>;

public:
  static constexpr std::size_t queue_capacity{256};

  using storage = typename _rt::storage;
  using ray_tracer = typename _rt::rt_search;
  using trace_result = std::future<rtc::intersection>;

  rt_service(std::shared_ptr<const rtc::scene_model>);
  ~rt_service();

  rt_service(rt_service&&) noexcept = default;
  rt_service(const rt_service&) = delete;
  rt_service& operator=(rt_service&&) noexcept = default;
  rt_service& operator=(const rt_service&) = delete;

  [[nodiscard]] rtc_hot auto trace_ray(const rtc::math_ray&) -> trace_result;
  auto thread_number() const noexcept
  {
    return !_rt::is_thread_safe ? 1u : std::max(1u, 3 * (boost::thread::hardware_concurrency() / 4));
  }

private:
  rt_service();
  struct control_block
  {
    std::unique_ptr<const ray_tracer> rt_search;

    boost::thread_group threads;
    std::array<rtc::math_ray, queue_capacity> ray_slots;
    std::array<std::promise<rtc::intersection>, queue_capacity> promise_slots;
    thread_safe_queue<std::uint16_t> free_slots{queue_capacity}, used_slots{queue_capacity};
  };

  static void thread_execution_loop(std::size_t, control_block&);
  std::unique_ptr<control_block> control{std::make_unique<control_block>()};
};

template<typename T>
rt_service<T>::rt_service()
{
  RELEASE << "Spawning threads... ";

  for(std::uint32_t i{0}; i < thread_number(); ++i)
      control->threads.create_thread([i, p = control.get()](){ thread_execution_loop(i, *p); });

  for(auto i{0}; i < queue_capacity; control->free_slots.push(i++)){}
}

template<typename T>
rt_service<T>::rt_service(std::shared_ptr<const rtc::scene_model> sc) : rt_service<T>()
{
  if(rtc_unlikely(!sc))
    throw std::runtime_error{"Pointer to scene_model is null."};

  storage s{*sc};
  control->rt_search = std::make_unique<ray_tracer>(std::move(s), std::move(sc));
}

template<typename T>
rt_service<T>::~rt_service()
{
  if(control)
  {
    control->threads.interrupt_all();
    control->threads.join_all();
  }
}

template<typename T>
auto rt_service<T>::trace_ray(const rtc::math_ray& ray) -> trace_result
{
  if(rtc_unlikely(!control))
    throw std::runtime_error{"ray_trace is not able to trace, probably it was moved from"};

  std::promise<rtc::intersection> p{};
  auto result{p.get_future()};
  const auto thread_fn = [this, &ray, &p](auto free_slot_index) {
    control->ray_slots[free_slot_index] = ray;
    control->promise_slots[free_slot_index] = std::move(p);

    while(!control->used_slots.push(free_slot_index)){}
  };

//  DEBUG << "Adding to queue";
  while(!control->free_slots.consume_one(thread_fn)){}

  //DEBUG << "Added to queue";
  return result;
}

template<typename T>
void rt_service<T>::thread_execution_loop(std::size_t thread_id, control_block& cb)
{
  std::random_device rd{};
  std::uint32_t work{}, total{}, stols{};

  auto random_sleep = [gen = std::mt19937(rd()),
                       urand = std::uniform_int_distribution<>(200, 800)] () mutable
  {
    return urand(gen);
  };

  const auto thread_fn = [&cb](const auto slot_index)
  {
    const auto ray{cb.ray_slots[slot_index]};
    auto promise{std::move(cb.promise_slots[slot_index])};
    auto intersection{cb.rt_search->trace_ray(ray)};

    promise.set_value(std::move(intersection));
    while(!cb.free_slots.push(slot_index)){}
  };

  try
  {
    SCOPE_TIME_COUNTER;
    DEBUG << "Thread[" << thread_id << "] was started!!";

    while(boost::this_thread::interruption_point(), true)
    {
      if(!cb.used_slots.consume_one(thread_fn))
      {
       // DEBUG << "Thread[" << thread_id << "] no work!!";
        ++stols;
        if(cb.used_slots.empty())
          random_sleep();
      }
      else
      {
       // DEBUG << "Thread[" << thread_id << "] work done!!";
        ++work;
      }
      ++total;
    }
  }
  catch(const boost::thread_interrupted&)
  {
    RELEASE << "Thread[" << thread_id << "] was interrupted!!"
            << " number of work: " << work
            << " total num: " << total
            << " stols num: " << stols
            << " work load: " << (rtc_float)work/(rtc_float)total
            << " work stols: " << (rtc_float)stols/(rtc_float)total;
  }
}

}
