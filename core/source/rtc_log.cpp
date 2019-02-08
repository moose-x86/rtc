#include "rtc_log.hpp"

std::unique_ptr<std::ofstream> rtc::log::syslog{nullptr};
std::mutex rtc::log::syslog_mutex{};

const int __tmp = []() {

#define __FILE__ "syslog"
#define __LINE__ 1

 DEBUG << R"( ____ _____ ____   ______   ______  _     ___   ____ )";
 DEBUG << R"(|  _ \_   _/ ___| / ___\ \ / / ___|| |   / _ \ / ___|)";
 DEBUG << R"(| |_) || || |     \___ \\ V /\___ \| |  | | | | |  _ )";
 DEBUG << R"(|  _ < | || |___   ___) || |  ___) | |__| |_| | |_| |)";
 DEBUG << R"(|_| \_\|_| \____| |____/ |_| |____/|_____\___/ \____|)";
 DEBUG;
 return 1;

}();
