
#include <primordial/unit_system.hpp>
#include <primordial/unit_system_io.hpp>
#include <gtest/gtest.h>
#include <iterator>
#include <limits>
#include <type_traits>

#include "demangle.hpp"

using primordial::testing::demangle;

namespace{

   class Length{};
   class Mass{};   
   class Time{};   

   struct MKS : ::primordial::unit_system<MKS,Length, Mass, Time>{};
   struct MKS2 : ::primordial::unit_system<MKS,Length, Mass, Time>{};
}

TEST(UnitTests, test_unit_system)
{
    using namespace primordial;


    EXPECT_TRUE((std::is_same_v<MKS::dimensions, meta::type_list<Length, Mass, Time>>)) << "got: " << demangle(typeid(MKS::dimensions).name());
    EXPECT_EQ(MKS::prime<Length>, 2);
    EXPECT_EQ(MKS::prime<Mass>, 3);
    EXPECT_EQ(MKS::prime<Time>, 5);

    EXPECT_TRUE((std::is_same_v<MKS::base_unit_type<Length>, unit<MKS,NQ{2}>>)) << "got: " << demangle(typeid(MKS::base_unit<Length>).name());
    EXPECT_TRUE((std::is_same_v<MKS::base_unit_type<Mass>, unit<MKS,NQ{3}>>)) << "got: " << demangle(typeid(MKS::base_unit<Mass>).name());
    EXPECT_TRUE((std::is_same_v<MKS::base_unit_type<Time>, unit<MKS,NQ{5}>>)) << "got: " << demangle(typeid(MKS::base_unit<Time>).name());
}

TEST(UnitTests, test_unit_comparison)
{
    using namespace primordial;

    EXPECT_TRUE((unit<MKS,NQ{2,3}>() == unit<MKS,NQ{2,3}>()));
    EXPECT_FALSE((unit<MKS,NQ{2,3}>() == unit<MKS,NQ{3,2}>()));
    EXPECT_FALSE((unit<MKS,NQ{2,3}>() != unit<MKS,NQ{2,3}>()));
    EXPECT_TRUE((unit<MKS,NQ{2,3}>() != unit<MKS,NQ{3,2}>()));

    EXPECT_TRUE((unit<MKS,NQ{1,1}>() == unit<MKS,NQ{1,1}>()));
    EXPECT_FALSE((unit<MKS,NQ{1,1}>() != unit<MKS,NQ{1,1}>()));

    EXPECT_TRUE((unit<MKS,NQ{}>() == unit<MKS2,NQ{}>()));
    EXPECT_FALSE((unit<MKS,NQ{}>() != unit<MKS2,NQ{}>()));

    EXPECT_FALSE((unit<MKS,NQ{}>() == unit<MKS2,NQ{2}>()));
    EXPECT_FALSE((unit<MKS2,NQ{2}>() == unit<MKS,NQ{1}>()));
    EXPECT_TRUE((unit<MKS,NQ{}>() != unit<MKS2,NQ{2}>()));
    EXPECT_TRUE((unit<MKS2,NQ{2}>() != unit<MKS,NQ{1}>()));

    EXPECT_FALSE((unit<MKS,NQ{2}>() == unit<MKS,NQ{3}>()));
    EXPECT_FALSE((unit<MKS,NQ{3}>() == unit<MKS,NQ{2}>()));
    EXPECT_TRUE((unit<MKS,NQ{2}>() != unit<MKS,NQ{3}>()));
    EXPECT_TRUE((unit<MKS,NQ{3}>() != unit<MKS,NQ{2}>()));

    EXPECT_FALSE((unit<MKS,NQ{}>() !=  unit<MKS,NQ{}>()));
    EXPECT_FALSE((unit<MKS,NQ{}>()!=  unit<MKS2,NQ{}>()));
    EXPECT_FALSE((unit<MKS2,NQ{}>()!=  unit<MKS,NQ{}>()));

}

TEST(UnitTests, test_unit_ostreaming)
{
    using primordial::NQ;
    using primordial::unit;
    {
        std::stringstream s;
        s << unit<MKS,NQ{}>{};
        EXPECT_EQ(s.str(), "");
    }
}