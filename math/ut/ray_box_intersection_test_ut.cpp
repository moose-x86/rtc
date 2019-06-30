#include <gtest/gtest.h>
#include "ray_box_intersection_test.hpp"

namespace rtc::ut
{

auto create_bbox()
{
  std::vector<rtc::math_point> v
  {
     {0, 10, 0}, {10, 2 , -10}, {-10, 2, 3}, {0, 10, 10}, {0, -10, 2}
  };

  return rtc::ray_box_intersection_test{v};
}


TEST(ray_box_intersection_test_ut, test_intersection_on_xaxis_from_right)
{
  auto bbox = create_bbox();

  rtc::math_ray ray{
     rtc::math_vector{-1, 0, 0},
     rtc::math_point{30, 0, 0}
  };

  auto intersection_range = bbox.intersection_values_for(ray);

  ASSERT_EQ(std::get<0>(*intersection_range), 20);
  ASSERT_EQ(std::get<1>(*intersection_range), 40);
}

TEST(ray_box_intersection_test_ut, test_intersection_on_xaxis_from_left)
{
  auto bbox = create_bbox();

  rtc::math_ray ray{
     rtc::math_vector{1, 0, 0},
     rtc::math_point{-30, 0, 0}
  };

  auto intersection_range = bbox.intersection_values_for(ray);

  ASSERT_EQ(std::get<0>(*intersection_range), 20);
  ASSERT_EQ(std::get<1>(*intersection_range), 40);
}

TEST(ray_box_intersection_test_ut, test_intersection_in_xaxis_and_yaxis)
{
  auto bbox = create_bbox();

  rtc::math_ray ray{
     rtc::math_vector{-1, 3, 0},
     rtc::math_point{12, -10, 0}
  };

  auto intersection_range = bbox.intersection_values_for(ray);

  ASSERT_EQ(std::get<0>(*intersection_range), 2);
  ASSERT_NEAR(std::get<1>(*intersection_range), 6.6667, 0.0001);
}

TEST(ray_box_intersection_test_ut, test_no_intesection)
{
  auto bbox = create_bbox();

  rtc::math_ray ray{
     rtc::math_vector{1, 3, 0},
     rtc::math_point{12, -10, 0}
  };

  auto intersection_range = bbox.intersection_values_for(ray);
  ASSERT_TRUE(!intersection_range.has_value());
}

TEST(ray_box_intersection_test_ut, test_no_intesection_on_axisz)
{
  auto bbox = create_bbox();

  rtc::math_ray ray{
     rtc::math_vector{-1, 3, -1},
     rtc::math_point{12, -10, -11}
  };

  auto intersection_range = bbox.intersection_values_for(ray);
  ASSERT_TRUE(!intersection_range.has_value());
}

TEST(ray_box_intersection_test_ut, test_no_intesection_on_axisz_ray_towards_box)
{
  auto bbox = create_bbox();

  rtc::math_ray ray{
     rtc::math_vector{-1, 3, 1},
     rtc::math_point{12, -10, -11}
  };

  auto intersection_range = bbox.intersection_values_for(ray);
  ASSERT_EQ(std::get<0>(*intersection_range), 2);
  ASSERT_NEAR(std::get<1>(*intersection_range), 6.6667, 0.0001);
}

}
