#pragma once


#include <primordial/concepts.hpp>
#include <primordial/meta.hpp>
#include <primordial/nq.hpp>
#include <primordial/prime.hpp>


namespace primordial
{

// CONCEPTS //////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    concept dimension_type = /*ostreamable<T> &&*/ default_constructible<T>; ///< maybe add restrictions later

    namespace details
    {
        struct unit_system_base;
    }    

    template <typename T>
    concept unit_system_type = default_constructible<T> &&  std::derived_from<T, details::unit_system_base>;

    template <typename T> 
    concept unit_type = default_constructible<T> 
                     && unit_system_type<typename T::system> && std::is_same<decltype(T::exponents), NQ>::value;


// FORWARD DECLARATIONS //////////////////////////////////////////////////////////////////////////////////////

    template <typename D, dimension_type... Dims> 
    struct unit_system;

    template <unit_system_type system, NQ q> 
    struct unit;

//  DECLARATIONS //////////////////////////////////////////////////////////////////////////////////////////////

    namespace details
    {
        class unit_system_base
        {        
            unit_system_base() = default;
            unit_system_base(unit_system_base const &) = default;
            template <typename D, dimension_type... Dims> friend class primordial::unit_system;
        };
    }   

    template <unit_system_type sys, NQ q> 
    struct unit 
    {
    // reflectio9n
        static constexpr NQ exponents = q;
        using system = sys;
        constexpr bool is_scalar() const { return q != NQ::zero(); }                      ///< test for scalar
        explicit constexpr operator bool() const { return !is_scalar(); }                 ///< test for not scalar

        template <NQ q2> using rebind = unit<sys, q2>;

    // group operations
        constexpr auto inv() const { return unit<system, -q>(); }                         ///< inversion function
        constexpr auto operator~() const { return inv(); }                                ///< operator for inverse

        template <typename sys2>
        constexpr auto operator*=(unit<sys2, NQ::zero()>) const { return *this; }         ///< scalar multiplication (even with other system possible)

        template <typename sys2>
        constexpr auto operator/=(unit<sys2, NQ::zero()>) const { return *this; }         ///< scalar division (even with other system possible)

        template <NQ q_rhs>
        friend constexpr auto operator*(unit lhs, unit<system,q_rhs> rhs){ return unit<system,lhs.exponents + rhs.exponents>{}; }
        
        template <NQ q_rhs>
        friend constexpr auto operator/(unit lhs, unit<system,q_rhs> rhs){ return unit<system,rhs.exponents - rhs.exponents>{}; }

    // equality  / inequality operators
        friend constexpr bool operator==(unit, unit){ return true; }                           ///< \overload
        friend constexpr bool operator!=(unit, unit){ return false; }                          ///< \overload

        template <unit_system_type sys2>
        friend constexpr bool operator==(unit, unit<sys2,NQ{}>){ return q == NQ{}; }           ///< \overload

        template <unit_system_type sys2>
        friend constexpr bool operator!=(unit, unit<sys2,NQ{}>){ return q != NQ{}; }           ///< \overload


        template <NQ q_rhs>
        friend constexpr bool operator==(unit, unit<system,q_rhs>){ return false; }            ///< \overload

        template <NQ q_rhs>
        friend constexpr bool operator!=(unit, unit<system,q_rhs>){ return true; }             ///< \overload    
    };

    /// poison defective type
    template <unit_system_type system> struct unit<system, NQ::zero()>{};    

    class default_unit_streamer;

    ///\brief 
    /// - CRTP
    ///\tparam D derived class
    template <typename D, dimension_type... Dims> 
    struct unit_system : details::unit_system_base
    {
        using dimensions = meta::type_list<Dims...>;

        template <dimension_type T>
        static constexpr unsigned prime = nth_prime(1 + meta::pos<T, Dims...>::value);

        template <dimension_type T>
        using base_unit = unit<D, NQ{prime<T>}>;

        // hide this in derived class if you wnat nicer unit names
        using unit_streamer = default_unit_streamer;
    };

}

