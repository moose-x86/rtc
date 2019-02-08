#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "point_in_triangle_test.hpp"
#include <memory>

namespace rtc
{
namespace ut
{

TEST(point_in_triangle_test_ut, test_flat_on_z_axis)
{
  rtc::math_point p1{0, 0, 0};
  rtc::math_point p2{10, 0, 0};
  rtc::math_point p3{0, 10, 0};
  const rtc::point_in_triangle_test test{p1, p2, p3};

  ASSERT_TRUE (test.triangle_contains({1, 1, 0}));
  ASSERT_TRUE (test.triangle_contains({0, 0, 0}));
  ASSERT_FALSE(test.triangle_contains({-1, 0, 0}));
  ASSERT_FALSE(test.triangle_contains({11, 0, 0}));
}

TEST(point_in_triangle_test_ut, test_flat_on_y_axis)
{
  rtc::math_point p1{0, 0, 0};
  rtc::math_point p2{10, 0, 0};
  rtc::math_point p3{0, 0, 10};
  const rtc::point_in_triangle_test test{p1, p2, p3};

  ASSERT_TRUE (test.triangle_contains({1, 0, 1}));
  ASSERT_TRUE (test.triangle_contains({0, 0, 0}));
  ASSERT_FALSE(test.triangle_contains({-1, 0, 0}));
  ASSERT_FALSE(test.triangle_contains({0, 0, 11}));
}

TEST(point_in_triangle_test_ut, test_flat_on_x_axis)
{
  rtc::math_point p1{0, 0, 0};
  rtc::math_point p2{0, 10, 0};
  rtc::math_point p3{0, 0, 10};
  const rtc::point_in_triangle_test test{p1, p2, p3};

  ASSERT_TRUE (test.triangle_contains({0, 1, 1}));
  ASSERT_TRUE (test.triangle_contains({0, 0, 0}));
  ASSERT_FALSE(test.triangle_contains({0, -1, 0}));
  ASSERT_FALSE(test.triangle_contains({0, 0, 11}));
}

TEST(point_in_triangle_test_ut, test_triangle_contains_gemoetric_center)
{
  rtc::math_point p1{9, 0, 0};
  rtc::math_point p2{0, 9, 0};
  rtc::math_point p3{0, 0, 9};
  const rtc::point_in_triangle_test test{p1, p2, p3};

  ASSERT_TRUE (test.triangle_contains({3, 3, 3}));
  ASSERT_TRUE (test.triangle_contains({2, 2, 2}));
}

}
}
