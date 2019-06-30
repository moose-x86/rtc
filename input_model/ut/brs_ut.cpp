#include "gtest/gtest.h"
#include "brs.hpp"

namespace rtc::ut
{

TEST(brs_test, test_construction_no_throw)
{
  ASSERT_NO_THROW(rtc::brs{"./room.xml"});
}

TEST(brs_test, test_construction_throws_no_file)
{
  ASSERT_THROW(rtc::brs{"./room_x.xml"}, std::runtime_error);
}

TEST(brs_test, test_load_point)
{
  const rtc::brs b{"./room.xml"};
  ASSERT_EQ(b.points.size(), 907U);

  ASSERT_EQ(b.points[251], ( rtc::math_point{1.23F, 0.682F, 1.718F} ));
  ASSERT_EQ(b.points[415], ( rtc::math_point{0.362F, 0.06F, 1.798F} ));
  ASSERT_EQ(b.points[534], ( rtc::math_point{-1.99F, 1.0F, 2.406F} ));
}

TEST(brs_test, test_load_triangle)
{
  const rtc::brs b{"./room.xml"};

  ASSERT_EQ(1034U, b.triangles.size());

  ASSERT_EQ((rtc::triangle3d{198, 212, 210}), b.triangles[345]);
  ASSERT_EQ((rtc::triangle3d{299, 296, 295}), b.triangles[543]);
  ASSERT_EQ((rtc::triangle3d{381, 379, 382}), b.triangles[666]);
}

TEST(brs_test, test_load_lights)
{
  const rtc::brs b{"./room.xml"};

  ASSERT_EQ(b.lights.size(), 2U);
}

TEST(brs_test, test_load_materials)
{
  const rtc::brs b{"./room.xml"};

  ASSERT_EQ(b.materials.size(), 28U);

  rtc::surface_material s1{0.2F, 0.0F, 0.0F,
                           0.0F, 1.0F, 0.0F,
                           1.0F, 1.0F, 0.0F,
                           0.0F, 1.0F, 1.0F,
                           false, true, true,
                           true, true, true,
                           {199, 199, 199},
                           surface_material::shading_type::guard};

  ASSERT_EQ(s1, b.materials[0]);

  rtc::surface_material s2{0.3F, 0.0F, 0.0F,
                           0.0F, 0.2F, 0.0F,
                           1.0F, 1.0F, 0.0F,
                           0.0F, 1.0F, 1.0F,
                           false, true, true,
                           false, true, true,
                           {210, 200, 195},
                           surface_material::shading_type::guard};

  ASSERT_EQ(s2, b.materials[10]);
}

TEST(brs_test, test_load_view_point_data)
{
  const rtc::brs b{"./room.xml"};

  ASSERT_EQ(b.optical_system.screen.resolution.x, 1920U);
  ASSERT_EQ(b.optical_system.screen.resolution.y, 1200U);

  ASSERT_NE((rtc::math_point{}), b.optical_system.view_point);

  ASSERT_NE((rtc::math_point{}), b.optical_system.screen.surface.lower_left_corner);
  ASSERT_NE((rtc::math_point{}), b.optical_system.screen.surface.upper_left_corner);
  ASSERT_NE((rtc::math_point{}), b.optical_system.screen.surface.upper_right_corner);
}

TEST(brs_test, test_load_ambient_data)
{
  const rtc::brs b{"./room.xml"};

  ASSERT_EQ((rtc::color{0.6F, 0.6F, 0.6F}), b.ambient);
}

}



