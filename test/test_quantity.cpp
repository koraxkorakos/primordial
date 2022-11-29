
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
   struct CGS : ::primordial::unit_system<CGS,Length, Mass, Time>{};

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

    constexpr auto MKS_meter = MKS::base_unit<Length>();
    constexpr auto MKS_kilogram = MKS::base_unit<Mass>();
    constexpr auto MKS_second = MKS::base_unit<Time>();

    struct CGS_unit_streamer : primordial::unit_streamer_interface
    {
        virtual void streamput(std::ostream &os, unsigned k) const override
        {
            switch(k)
            {
            case 0: os << "cm"; break;
            case 1: os << 'g'; break;
            case 2: os << 's'; break;
            default: os.setstate(std::ios::failbit);
            }            
        }
    };

    constexpr auto CGS_centimeter = CGS::base_unit<Length>();
    constexpr auto CGS_gram = CGS::base_unit<Mass>();
    constexpr auto CGS_second = CGS::base_unit<Time>();

}

namespace primordial
{
    template <>
    struct unit_streamer_trait<MKS> : std::type_identity<MKS_unit_streamer>{};

    template <>
    struct unit_streamer_trait<CGS> : std::type_identity<CGS_unit_streamer>{};

}

TEST(QuantityTests, test_constructor)
{
    using primordial::quantity;

    quantity<MKS_meter,double> m{1};
    quantity<MKS_kilogram,double> g{0.001};
    quantity<MKS_second,double> s{1};
}

TEST(QuantityTests, test_mult)
{
    using primordial::quantity;

    std::cerr << demangle(typeid(quantity<MKS_meter,double>{20.0}).name()) << std::endl;
    std::cerr << demangle(typeid(quantity<MKS_kilogram,double>{20.0}).name()) << std::endl;

    std::cerr << demangle(typeid(MKS_kilogram * MKS_meter).name()) << std::endl;

    //auto const result = quantity<MKS_meter,double>{0.5} * quantity<MKS_kilogram,double>{20.0};
    //EXPECT_NEAR(result.scalar, 10.0, 10e-8);
}


TEST(QuantityTests, test_div)
{
    EXPECT_TRUE(false) << "unit test needs to be implemented.";
}

TEST(QuantityTests, test_plus)
{
    EXPECT_TRUE(false) << "unit test needs to be implemented.";
}

TEST(QuantityTests, test_minus)
{
    EXPECT_TRUE(false) << "unit test needs to be implemented.";
}

TEST(QuantityTests, test_relatve_absolute)
{
    EXPECT_TRUE(false) << "unit test needs to be implemented.";
}

TEST(QuantityTests, test_equality_comparison)
{
    EXPECT_TRUE(false) << "unit test needs to be implemented.";
}

TEST(QuantityTests, test_equality_realtional_comparison)
{
    EXPECT_TRUE(false) << "unit test needs to be implemented.";
}

TEST(QuantityTests, test_value)
{
    EXPECT_TRUE(false) << "unit test needs to be implemented.";
}

TEST(QuantityTests, test_scalar_conversion_constructor)
{
    EXPECT_TRUE(false) << "unit test needs to be implemented.";
}

TEST(QuantityTests, test_scalar_conversion)
{
    EXPECT_TRUE(false) << "unit test needs to be implemented.";
}

TEST(QuantityTests, test_assignment)
{
    EXPECT_TRUE(false) << "unit test needs to be implemented.";
}