#include "gtest/gtest.h"
#include "float_compare.hpp"

namespace rtc::ut
{

TEST(float_compare_ut, test_zero_compare)
{
  ASSERT_TRUE(rtc::fequal(0.0, 0.0));
}

TEST(float_compare_ut, test_for_sqrt2)
{
  auto x = 1.0/std::sqrt(2)/std::sqrt(2);

  ASSERT_TRUE(rtc::fequal(x, 0.5));
}

TEST(float_compare_ut, test_for_constexpr)
{
  auto b = rtc::fequal(1.0, 1.0);
}


}
