
#include <primordial/powermap.hpp>
#include <gtest/gtest.h>
#include<gmock/gmock-matchers.h>
#include <iterator>
#include <limits>
#include <type_traits>

#include "demangle.hpp"

using ::testing::ElementsAre;

using primordial::NQ;

namespace primordial
{

TEST(test_powermap, get_power_in)
{
    EXPECT_EQ(get_power_in(2, NQ(10,27)), 1);
    EXPECT_EQ(get_power_in(3, NQ(10,27)), -3);
    EXPECT_EQ(get_power_in(5, NQ(10,27)), 1);
    EXPECT_EQ(get_power_in(7, NQ(10,27)), 0);
}

TEST(test_powermap, num_dims)
{
    EXPECT_EQ(num_dims(NQ(10,27)), 3);
    EXPECT_EQ(num_dims(NQ(1,1)), 0);  
    EXPECT_EQ(num_dims(NQ(16,27)), 2);  
}

TEST(test_powermap, power_maps)
{
    EXPECT_THAT((power_map<NQ::zero()>), ElementsAre());
    EXPECT_THAT((power_map<NQ{2,1}>), ElementsAre(1));
    EXPECT_THAT((power_map<NQ{1,2}>), ElementsAre(-1));
    EXPECT_THAT((power_map<NQ{3,2}>), ElementsAre(-1,1));
    EXPECT_THAT((power_map<NQ{2,3}>), ElementsAre(1,-1));
    EXPECT_THAT((power_map<NQ{9,5}>), ElementsAre(0,2,-1));
}
}