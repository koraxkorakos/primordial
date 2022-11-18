
#include <primordial/meta.hpp>
//#include <primordial/details/mult128.hpp>
#include <gtest/gtest.h>
#include <iterator>
#include <limits>
#include <type_traits>

#include "demangle.hpp"

TEST(meta_tests, test_conditional_t)
{
   EXPECT_TRUE((std::is_same_v<primordial::meta::conditional_t<true,int,void>,std::conditional_t<true,int,void>>));
   EXPECT_TRUE((std::is_same_v<primordial::meta::conditional_t<false,int,void>,std::conditional_t<false,int,void>>));    
}

TEST(meta_tests, test_pos)
{
   EXPECT_EQ((primordial::meta::pos<void, void>::value), 0);
   EXPECT_EQ((primordial::meta::pos<void, int, void>::value), 1);
   EXPECT_EQ((primordial::meta::pos<void, int, int, void, void>::value), 2);
}

TEST(meta_tests, test_at)
{
   EXPECT_TRUE((std::is_same_v<void, primordial::meta::get<0, void>>));
   EXPECT_TRUE((std::is_same_v<void, primordial::meta::get<0, void, char>>));
   EXPECT_TRUE((std::is_same_v<void, primordial::meta::get<1, char, void>>));      
   EXPECT_TRUE((std::is_same_v<void, primordial::meta::get<1, char, void, void>>));      
   EXPECT_TRUE((std::is_same_v<void, primordial::meta::get<2, char, int, void, void>>));      
}
