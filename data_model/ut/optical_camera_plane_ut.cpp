#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "optical_camera_plane.hpp"
#include "camera.hpp"

using namespace ::testing;

namespace rtc::ut
{
constexpr rtc::camera optical_system
{
  {4.3800F, 1.5F, -3.98F},
  {
    {
      {-3.9924F, 4.0755F, -0.2463F},
      { 2.7924F, 4.0755F, 3.6463F},
      {-3.9924F, -1.0755F, -0.2463F}
    },
    {1024, 768}
  }
};

TEST(optical_camera_plane_ut, basic_test)
{
  const rtc::optical_camera_plane plane{optical_system};
  const auto ray = plane.emit_ray(0, 0);

  ASSERT_EQ(ray.origin(), optical_system.view_point);
  ASSERT_EQ(ray.direction(), rtc::math_vector(-8.372400F, 2.575500F, 3.733700F));
}

TEST(optical_camera_plane_ut, basic_test_1)
{
  const rtc::optical_camera_plane plane{optical_system};
  const auto ray = plane.emit_ray(4, 0);

  ASSERT_EQ(ray.origin(), optical_system.view_point);

  ASSERT_THAT(ray.direction().x(), FloatEq(-8.345897F));
  ASSERT_THAT(ray.direction().y(), FloatEq(2.575500F));
  ASSERT_THAT(ray.direction().z(), FloatNear(3.74891F, 0.000006F));
}

TEST(optical_camera_plane_ut, basic_test_2)
{
  const rtc::optical_camera_plane plane{optical_system};

  const auto ray = plane.emit_ray(393, 67);

  ASSERT_EQ(ray.origin(), optical_system.view_point);
  ASSERT_EQ(ray.direction(), rtc::math_vector(-5.768468F, 2.126129F, 5.227637F));
}

}


