#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "math_plane.hpp"
#include <memory>

namespace rtc::ut
{

TEST(math_plane_ut, test_get_intesection)
{
  rtc::math_plane plane{rtc::math_vector{0, 0, 1}, rtc::math_point{0, 0, 0}};
  rtc::math_ray ray{rtc::math_vector{0, 0, -1}, rtc::math_point{0, 0, 10}};

  ASSERT_EQ(plane.intersection_value(ray), 10.0F);
}

TEST(math_plane_ut, test_get_intesection_no_intersection)
{
  rtc::math_plane plane{rtc::math_vector{0, 0, 1}, rtc::math_point{0, 0, 0}};
  rtc::math_ray ray{rtc::math_vector{1, 0, 0}, rtc::math_point{0, 0, 10}};

  ASSERT_EQ(plane.intersection_value(ray), rtc::math_plane::infinity);
}

TEST(math_plane_ut, test_get_intesection_in_zero)
{
  rtc::math_plane plane{rtc::math_vector{0, 0, 1}, rtc::math_point{0, 0, 0}};
  rtc::math_ray ray{rtc::math_vector{0, -1, -1}, rtc::math_point{0, 10, 10}};

  const auto t = plane.intersection_value(ray);

  ASSERT_EQ(ray[t], (rtc::math_point{0, 0, 0}));
}

}
