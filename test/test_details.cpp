#include <primordial/details/mult128.hpp>
#include <gtest/gtest.h>
#include <iterator>
#include <limits>
#include <type_traits>
#include <cmath>

#include "demangle.hpp"

using primordial::umult_overflow;
using primordial::umult_overflow_longdiv;

template<typename T>
struct UMultOverflowFictureTestFixture : public ::testing::Test {};
TYPED_TEST_SUITE_P(UMultOverflowFictureTestFixture);


TYPED_TEST_P(UMultOverflowFictureTestFixture, test_without_overflow)
{
    /*
    constexpr TypeParam max_v = sqrt(127.0);
    for (TypeParam k=0; k!=max_v; ++k)
        for (TypeParam j=0; k!=max_v; ++j)
        {
            TypeParam result;
            EXPECT_FALSE(umult_overflow(k, j, result));
            EXPECT_EQ(result, k*j);
        }
    */
}

TYPED_TEST_P(UMultOverflowFictureTestFixture, test_with_overflow)
{
    TypeParam const a = TypeParam(1) << (std::numeric_limits<TypeParam>::digits / 2);
    TypeParam result;
    EXPECT_TRUE(umult_overflow(a, a, result));    
}


REGISTER_TYPED_TEST_SUITE_P(UMultOverflowFictureTestFixture, test_without_overflow, test_with_overflow);
using Types = testing::Types<unsigned char, unsigned short, unsigned, unsigned long, unsigned long long>;
INSTANTIATE_TYPED_TEST_SUITE_P(_, UMultOverflowFictureTestFixture, Types);



