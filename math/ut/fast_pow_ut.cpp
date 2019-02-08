#include <gtest/gtest.h>
#include "fast_pow.hpp"
#include <cmath>

namespace rtc
{
namespace ut
{

TEST(fast_pow_ut, test_for_n_zero)
{
    auto x = rtc::pow(128, 0);
    ASSERT_EQ(x, 1);
}

TEST(fast_pow_ut, test_for_256)
{
    auto x = rtc::pow(2, 8);
    ASSERT_EQ(x, 256);
}

TEST(fast_pow_ut, test_for_27)
{
    auto x = rtc::pow(3, 3);
    ASSERT_EQ(x, 27);
}

TEST(fast_pow_ut, test_for_32768)
{
    auto x = rtc::pow(2, 15);
    ASSERT_EQ(x, 32768);
}

TEST(fast_pow_ut, test_for_2_pow_30)
{
    auto x = rtc::pow(2, 30);
    ASSERT_EQ(x, std::pow(2, 30));
}

}
}
