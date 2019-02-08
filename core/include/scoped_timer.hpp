#pragma once

#include <chrono>
#include "rtc_log.hpp"
namespace rtc
{

struct scoped_timer
{
public:
  scoped_timer(const char* function, const std::uint32_t line) noexcept try {
     fn_name = function;
     ln = line;
     now = std::chrono::high_resolution_clock::now();
  } catch(...){}

  ~scoped_timer() noexcept try
  {
    std::chrono::duration<rtc_float> fs = std::chrono::high_resolution_clock::now() - now;
    rtc::log(fn_name, ln).stream() << "Timer duration of scope was " << fs.count() << " [sec]";
  } catch(...){}

private:
  std::uint32_t ln{};
  std::string fn_name{};
  std::chrono::high_resolution_clock::time_point now{};
};

#define COMBINE_TEMP(X, Y) X##Y
#define COMBINE(X, Y) COMBINE_TEMP(X, Y)

#ifndef NDEBUG
 #define SCOPE_TIME_COUNTER ::rtc::scoped_timer COMBINE(__st_, __LINE__){__FILE__, __LINE__};
#else
 #define SCOPE_TIME_COUNTER
#endif
}
