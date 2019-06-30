#include "gtest/gtest.h"
#include "math_vector.hpp"

namespace rtc::ut
{

TEST(vector_ut, dot_product_test)
{
  rtc::math_vector v1{0, 0, 1}, v2{0, 1, 0};

  ASSERT_EQ(rtc::dot(v1, v2), 0.0F);
}

TEST(vector_ut, dot_product_test2)
{
  rtc::math_vector v1{0, 0, 1}, v2{0, 1, 0};

  ASSERT_EQ(rtc::cross(v1, v2), (rtc::math_vector{-1, 0, 0}));
}

TEST(vector_ut, length)
{
  rtc::math_vector v{1, 1, 1};

  ASSERT_EQ(rtc::length(v), std::sqrt(3.0F));
}

TEST(vector_ut, normalize)
{
  rtc::math_vector v{10, 10, 2};

  ASSERT_EQ(rtc::length(rtc::normalize(v)), 1.0F);
}


}
