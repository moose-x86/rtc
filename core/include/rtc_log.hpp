#pragma once

#include <fstream>
#include <iomanip>
#include <memory>
#include <mutex>
#include <string>

#include "stream_operators.hpp"
#include "utility.hpp"

namespace rtc
{
class log
{
 public:
  log() noexcept = default;

  log(const std::string& file, const std::uint32_t line) : log()
  {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    syslog_mutex.lock();

    if (rtc_unlikely(!syslog)) syslog = std::make_unique<std::ofstream>("rsyslog");

    f = syslog->flags();
    system_clock::time_point zero{};
    system_clock::time_point now{system_clock::now()};
    const std::time_t here{system_clock::to_time_t(now)};

    *syslog
        // << "["
        // << std::put_time(std::localtime(std::addressof(here)), "%H:%M:%S")
        // << ":"
        // << duration_cast<milliseconds>(now - zero).count() %
        // milliseconds{1s}.count()
        << "] DEBUG_LOG [" << file.substr(file.find_last_of('/') + 1) << ":" << line << "] ";
  }

  ~log() noexcept
  try
    {
      std::unique_lock<std::mutex> l{syslog_mutex, std::adopt_lock};

      *syslog << std::endl;
      syslog->flags(f);
    }
  catch (...)
    {
    }

  auto stream() noexcept -> std::ostream& { return *syslog; }

 private:
  std::ios_base::fmtflags f{};

  static std::mutex syslog_mutex;
  static std::unique_ptr<std::ofstream> syslog;
};

struct null_stream final
{
};

template <typename T>
inline auto operator<<(null_stream, const T&) noexcept -> null_stream
{
  return {};
}

}  // namespace rtc

#ifdef NDEBUG
#define DEBUG \
  rtc::null_stream {}
#else
#define DEBUG rtc::log{__FILE__, __LINE__}.stream()
#endif

#define RELEASE rtc::log{__FILE__, __LINE__}.stream()
