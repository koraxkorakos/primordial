
#include <primordial/nq.hpp>
//#include <primordial/details/mult128.hpp>
#include <gtest/gtest.h>
#include <iterator>
#include <limits>
#include <type_traits>

#include "demangle.hpp"

using primordial::NQ;

TEST(NqTests, test_default_constrtuctor)
{
   NQ const nq;
   EXPECT_EQ(nq.num(), 1);
   EXPECT_EQ(nq.den(), 1);
}

TEST(NqTests, test_NQ_zero)
{
   EXPECT_EQ(NQ::zero().num(), 1);
   EXPECT_EQ(NQ::zero().den(), 1);
}

TEST(NqTests, test_NQ_defective)
{
   EXPECT_EQ(NQ::defective().num(), 0);
   EXPECT_EQ(NQ::defective().den(), 0);
}

TEST(NqTests, test_NQ_constructor_with_one_argument)
{
   NQ const nq{1}; 
   EXPECT_EQ(nq.num(), 1);
   EXPECT_EQ(nq.den(), 1);
}

TEST(NqTests,  test_NQ_constructor_with_one_argument_not_normalized)
{
   NQ const nq{2}; 
   EXPECT_EQ(nq.num(), 2);
   EXPECT_EQ(nq.den(), 1);
}

TEST(NqTests,  test_NQ_constructor)
{
   NQ const nq{4,6};
   EXPECT_EQ(nq.num(), 2);
   EXPECT_EQ(nq.den(), 3);
}

TEST(NqTests,  test_NQ_bool_conversion)
{
   EXPECT_TRUE(NQ::defective());
   EXPECT_FALSE(NQ(1));
   EXPECT_TRUE(NQ(2));
   EXPECT_TRUE(NQ(1,3));
   EXPECT_TRUE(NQ(2,3));
}
