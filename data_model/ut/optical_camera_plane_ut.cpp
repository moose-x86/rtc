#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "optical_camera_plane.hpp"
#include "camera.hpp"

using namespace ::testing;

namespace rtc::ut
{
constexpr rtc::camera optical_system
{
  {4.3800f, 1.5f, -3.98f},
  {
    {
      {-3.9924f, 4.0755f, -0.2463f},
      { 2.7924f, 4.0755f, 3.6463f},
      {-3.9924f, -1.0755f, -0.2463f}
    },
    {1024, 768}
  }
};

TEST(optical_camera_plane_ut, basic_test)
{
  const rtc::optical_camera_plane plane{optical_system};
  const auto ray = plane.emit_ray(0, 0);

  ASSERT_EQ(ray.origin(), optical_system.view_point);
  ASSERT_EQ(ray.direction(), rtc::math_vector(-8.372400f, 2.575500f, 3.733700f));
}

TEST(optical_camera_plane_ut, basic_test_1)
{
  const rtc::optical_camera_plane plane{optical_system};
  const auto ray = plane.emit_ray(4, 0);

  ASSERT_EQ(ray.origin(), optical_system.view_point);

  ASSERT_THAT(ray.direction().x(), FloatEq(-8.345897f));
  ASSERT_THAT(ray.direction().y(), FloatEq(2.575500f));
  ASSERT_THAT(ray.direction().z(), FloatNear(3.74891f, 0.000006f));
}

TEST(optical_camera_plane_ut, basic_test_2)
{
  const rtc::optical_camera_plane plane{optical_system};

  const auto ray = plane.emit_ray(393, 67);

  ASSERT_EQ(ray.origin(), optical_system.view_point);
  ASSERT_EQ(ray.direction(), rtc::math_vector(-5.768468f, 2.126129f, 5.227637f));
}

}


