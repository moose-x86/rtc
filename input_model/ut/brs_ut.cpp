#include "gtest/gtest.h"
#include "brs.hpp"

namespace rtc
{
namespace ut
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
  ASSERT_EQ(b.points.size(), 907u);

  ASSERT_EQ(b.points[251], ( rtc::math_point{1.23f, 0.682f, 1.718f} ));
  ASSERT_EQ(b.points[415], ( rtc::math_point{0.362f, 0.06f, 1.798f} ));
  ASSERT_EQ(b.points[534], ( rtc::math_point{-1.99f, 1.0f, 2.406f} ));
}

TEST(brs_test, test_load_triangle)
{
  const rtc::brs b{"./room.xml"};

  ASSERT_EQ(1034u, b.triangles.size());

  ASSERT_EQ((rtc::triangle3d{198, 212, 210}), b.triangles[345]);
  ASSERT_EQ((rtc::triangle3d{299, 296, 295}), b.triangles[543]);
  ASSERT_EQ((rtc::triangle3d{381, 379, 382}), b.triangles[666]);
}

TEST(brs_test, test_load_lights)
{
  const rtc::brs b{"./room.xml"};

  ASSERT_EQ(b.lights.size(), 2u);
}

TEST(brs_test, test_load_materials)
{
  const rtc::brs b{"./room.xml"};

  ASSERT_EQ(b.materials.size(), 28u);

  rtc::surface_material s1{0.2f, 0.0f, 0.0f,
                           0.0f, 1.0f, 0.0f,
                           1.0f, 1.0f, 0.0f,
                           0.0f, 1.0f, 1.0f,
                           false, true, true,
                           true, true, true,
                           {199, 199, 199},
                           surface_material::shading_type::guard};

  ASSERT_EQ(s1, b.materials[0]);

  rtc::surface_material s2{0.3f, 0.0f, 0.0f,
                           0.0f, 0.2f, 0.0f,
                           1.0f, 1.0f, 0.0f,
                           0.0f, 1.0f, 1.0f,
                           false, true, true,
                           false, true, true,
                           {210, 200, 195},
                           surface_material::shading_type::guard};

  ASSERT_EQ(s2, b.materials[10]);
}

TEST(brs_test, test_load_view_point_data)
{
  const rtc::brs b{"./room.xml"};

  ASSERT_EQ(b.optical_system.screen.resolution.x, 1920u);
  ASSERT_EQ(b.optical_system.screen.resolution.y, 1200u);

  ASSERT_NE((rtc::math_point{}), b.optical_system.view_point);

  ASSERT_NE((rtc::math_point{}), b.optical_system.screen.surface.lower_left_corner);
  ASSERT_NE((rtc::math_point{}), b.optical_system.screen.surface.upper_left_corner);
  ASSERT_NE((rtc::math_point{}), b.optical_system.screen.surface.upper_right_corner);
}

TEST(brs_test, test_load_ambient_data)
{
  const rtc::brs b{"./room.xml"};

  ASSERT_EQ((rtc::color{0.6f, 0.6f, 0.6f}), b.ambient);
}

}
}



