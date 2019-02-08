#include "gtest/gtest.h"
#include "rt_service.hpp"
#include "brs.hpp"
#include <memory>

namespace rtc
{
namespace ut
{

TEST(rt_service_ut, basic_move_test)
{
  auto data = std::make_shared<rtc::brs>("room.xml");
  rtc::rt_service<> r{data};

  auto rt_service{std::move(r)};
}

TEST(rt_service_ut, basic_test)
{
  auto data = std::make_shared<rtc::brs>("room.xml");
  rtc::rt_service<> rt{data};

  const rtc::math_vector v{-4.396932f, -0.778016f, 6.014521f};
  const rtc::math_ray ray{v, data->optical_system.view_point};

  auto intersection = rt.trace_ray(ray).get();

  ASSERT_TRUE(intersection.is_present());
  ASSERT_TRUE(intersection.is_with(619));
}

}
}
