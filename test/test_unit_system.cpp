
#include <primordial/unit_system.hpp>
#include <primordial/unit_system_io.hpp>
#include <primordial/powermap.hpp>// DEBUG
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

    struct MKS_unit_streamer : primordial::unit_streamer_interface
    {
        virtual void streamput(std::ostream &os, unsigned k) const override
        {
            switch(k)
            {
            case 0: os << 'm'; break;
            case 1: os << "kg"; break;
            case 2: os << 's'; break;
            default: os.setstate(std::ios::failbit);
            }            
        }
    };

   struct MKS2 : ::primordial::unit_system<MKS,Length, Mass, Time>{};


}

namespace primordial
{
    template <>
    struct unit_streamer_trait<MKS> : std::type_identity<MKS_unit_streamer>{};
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
    EXPECT_FALSE((unit<MKS,NQ{2,3}>() != unit<MKS,NQ{2,3}>()));

    EXPECT_TRUE((unit<MKS,NQ{1,1}>() == unit<MKS,NQ{1,1}>()));
    EXPECT_FALSE((unit<MKS,NQ{1,1}>() != unit<MKS,NQ{1,1}>()));

    EXPECT_TRUE((unit<MKS,NQ{}>() == unit<MKS2,NQ{}>()));
    EXPECT_FALSE((unit<MKS,NQ{}>() != unit<MKS2,NQ{}>()));

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
        EXPECT_TRUE(s);
        EXPECT_EQ(s.str(), "");
    }

    {
        std::stringstream s;
        s << unit<MKS,NQ{2}>{};
        EXPECT_TRUE(s);        
        EXPECT_EQ(s.str(), "m");
    }

    {
        std::stringstream s;
        s << unit<MKS,NQ{3}>{};
                EXPECT_TRUE(s);
        EXPECT_EQ(s.str(), "kg");
    }

    {
        std::stringstream s;
        s << unit<MKS,NQ{5}>{};
        EXPECT_TRUE(s);
        EXPECT_EQ(s.str(), "s");
    }

    {
        std::stringstream s;
        s << unit<MKS,NQ{2,25}>{};
        EXPECT_EQ(s.str(), "m/s^2");
    }
}