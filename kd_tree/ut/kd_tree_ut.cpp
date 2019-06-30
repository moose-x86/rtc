#include "gtest/gtest.h"

#include "kd_tree.hpp"
#include "scene_model.hpp"
#include "brs.hpp"
#include "point_in_triangle_test.hpp"
#include "math_plane.hpp"
#include "rtc_log.hpp"
#include "scoped_timer.hpp"
#include "ray_tracer.hpp"

namespace rtc::ut
{

TEST(kd_tree_ut, room_test_found_1032)
{
  auto scene = std::make_shared<rtc::brs>("./room.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{-6.788838F, 2.300512F, 4.642227F};
  const rtc::math_ray ray{v, scene->optical_system.view_point};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(1032));
}

TEST(kd_tree_ut, room_test_found_)
{
  auto scene = std::make_shared<rtc::brs>("./room.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{-5.145645F, -0.382301F, 5.584966F};
  const rtc::math_ray ray{v, scene->optical_system.view_point};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(843));
}

TEST(kd_tree_ut, room_test_found_2)
{
  auto scene = std::make_shared<rtc::brs>("./room.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{-4.396932F, -0.778016F, 6.014521F};
  const rtc::math_ray ray{v, scene->optical_system.view_point};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(619));
}

TEST(kd_tree_ut, kitchen_test_found)
{
  auto scene = std::make_shared<rtc::brs>("./kitchen.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{2.139743F, -0.249007F, -1.756500F};
  const rtc::math_ray ray{v, {2.435057F, 2.308806F, -0.50000F}};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(6695));
}

TEST(kd_tree_ut, DISABLED_kitchen_test_found_2)
{
  auto scene = std::make_shared<rtc::brs>("./kitchen.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{-3.624093F, 0.389207F, 3.397013F};
  const rtc::math_ray ray{v, scene->optical_system.view_point};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(6721));
}

TEST(kd_tree_ut, ulica_test_found)
{
  auto scene = std::make_shared<rtc::brs>("./ulica.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{35.994164F, 12.552099F, 10.576778F};
  const rtc::math_ray ray{v, scene->optical_system.view_point};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(15391));
}

TEST(kd_tree_ut, ulica_test_found_2)
{
  auto scene = std::make_shared<rtc::brs>("./ulica.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{-12.702606F, -25.481249F, 39.070004F};
  const rtc::math_ray ray{v, {78.202606F, 28.281248F, 25.230000F}};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(15637));
}

TEST(kd_tree_ut, ulica_test_found_3)
{
  auto scene = std::make_shared<rtc::brs>("./ulica.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{-22.110607F, -23.501247F, 21.443001F};
  const rtc::math_ray ray{v, {78.202606F, 28.281248F, 25.230000F}};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(31642));
}

TEST(kd_tree_ut, DISABLED_cornell_box_test)
{
  auto scene = std::make_shared<rtc::brs>("./cornell_box.xml");
  rtc::kd_tree acc{*scene};

  const rtc::math_vector v{1.066667F, 0.951870F, 3.365898F};
  const rtc::math_ray ray{v, scene->optical_system.view_point};

  rtc::ray_tracer<rtc::kd_tree> finder{std::move(acc), scene};
  const auto intersect = finder.trace_ray(ray);

  ASSERT_TRUE(intersect && intersect.is_with(2));
}

}

