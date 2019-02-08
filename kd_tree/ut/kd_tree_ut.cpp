#include "gtest/gtest.h"

#include "kd_tree.hpp"
#include "scene_model.hpp"
#include "brs.hpp"
#include "point_in_triangle_test.hpp"
#include "math_plane.hpp"
#include "rtc_log.hpp"
#include "scoped_timer.hpp"
#include "ray_tracer.hpp"

namespace rtc
{
namespace ut
{

TEST(kd_tree_ut, room_test_found_1032)
{
  auto scene = std::make_shared<rtc::brs>("./room.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{-6.788838f, 2.300512f, 4.642227f};
  const rtc::math_ray ray{v, scene->optical_system.view_point};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(1032));
}

TEST(kd_tree_ut, room_test_found_)
{
  auto scene = std::make_shared<rtc::brs>("./room.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{-5.145645f, -0.382301f, 5.584966f};
  const rtc::math_ray ray{v, scene->optical_system.view_point};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(843));
}

TEST(kd_tree_ut, room_test_found_2)
{
  auto scene = std::make_shared<rtc::brs>("./room.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{-4.396932f, -0.778016f, 6.014521f};
  const rtc::math_ray ray{v, scene->optical_system.view_point};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(619));
}

TEST(kd_tree_ut, kitchen_test_found)
{
  auto scene = std::make_shared<rtc::brs>("./kitchen.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{2.139743f, -0.249007f, -1.756500f};
  const rtc::math_ray ray{v, {2.435057f, 2.308806f, -0.50000f}};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(6695));
}

TEST(kd_tree_ut, kitchen_test_found_2)
{
  auto scene = std::make_shared<rtc::brs>("./kitchen.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{-3.624093f, 0.389207f, 3.397013f};
  const rtc::math_ray ray{v, scene->optical_system.view_point};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(6721));
}

TEST(kd_tree_ut, ulica_test_found)
{
  auto scene = std::make_shared<rtc::brs>("./ulica.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{35.994164f, 12.552099f, 10.576778f};
  const rtc::math_ray ray{v, scene->optical_system.view_point};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(15391));
}

TEST(kd_tree_ut, ulica_test_found_2)
{
  auto scene = std::make_shared<rtc::brs>("./ulica.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{-12.702606f, -25.481249f, 39.070004f};
  const rtc::math_ray ray{v, {78.202606f, 28.281248f, 25.230000f}};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(15637));
}

TEST(kd_tree_ut, ulica_test_found_3)
{
  auto scene = std::make_shared<rtc::brs>("./ulica.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{-22.110607f, -23.501247f, 21.443001f};
  const rtc::math_ray ray{v, {78.202606f, 28.281248f, 25.230000f}};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(31642));
}

TEST(kd_tree_ut, DISABLED_cornell_box_test)
{
  auto scene = std::make_shared<rtc::brs>("./cornell_box.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{1.066667f, 0.951870f, 3.365898f};
  const rtc::math_ray ray{v, scene->optical_system.view_point};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(2));
}

}
}

