
#include <primordial/quantity.hpp>
#include <primordial/quantity_io.hpp>
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

TEST(QuantityTests, test_constructor)
{
    EXPECT_TRUE(false) << "unit test needs to be implemented.";
}

TEST(QuantityTests, test_comparison)
{
    EXPECT_TRUE(false) << "unit test needs to be implemented.";
}