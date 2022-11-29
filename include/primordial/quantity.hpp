#pragma once

#include <primordial/nq.hpp>
#include <primordial/unit_system.hpp>
#include <primordial/powermap.hpp>
#include <concepts>

namespace primordial
{           
    ///\brief tag to distinguish between relative and absolute quantities
    enum quantity_kind 
    {
        relative,
        absolute
    };

    template <unit_type auto U, arithmetic_type S, quantity_kind kind = quantity_kind::relative>  class quantity;                    

    template <unit_type auto U, arithmetic_type S, quantity_kind kind> 
    constexpr auto relative_cast(quantity<U, S, kind> const & q)
    {
        return quantity<U, S, quantity_kind::relative>{q.get_cofactor()};
    }

    template <unit_type auto U, arithmetic_type S, quantity_kind kind> 
    constexpr auto absolute_cast(quantity<U, S, kind> const &q)
    {
        return quantity<U, S, quantity_kind::absolute>{q.get_cofactor()};
    }


////////////////////////////////////////////////////////////////////////////////////////////////////////////

    namespace details
    {
        constexpr bool plus_allowed(quantity_kind a, quantity_kind b)        
        {
            return ! (a == b) && a == quantity_kind::absolute;
        }

        constexpr bool minus_allowed(quantity_kind a, quantity_kind b)        
        {
            return ! (a != b) && a == quantity_kind::relative;
        }

    }

    ///\brief (pure) quantity in the base system `system`. 
    /// Exponents are kept of track in `exponents` as positive prime number power (numerator) and negative
    /// prime number powers in the denominator.If both are one than the quantity is a scalar.
    ///\par Scalars
    /// Scalars can be constructed by implicite conversion from `value_type`. Ths allows code like.
    ///\a 
    ///\code
    ///  enum class MKS{ m, k, s, last_tag = mks::S};
    ///  using mks_units = make_unit_system<MKS,>;
    ///  5 * meter(5) * 2
    ///\endcode
    /// via conversion constructor
    ///\tparam S scalar should be a field or at least a ring if you do  not care for overflow
    ///\tparam S scalar should be a field or at least a ring if you do  not care for overflow
    ///\todo concept for system
    template <unit_type auto U, arithmetic_type S, quantity_kind kind_> // S could also be array
    class quantity {
    public:
        using value_type = S;
        using unit_t = decltype(U);
        using system = typename unit_t::system;
        static constexpr quantity_kind kind = kind_;

        value_type cofactor;
        static constexpr auto unit = U;

        constexpr quantity() : cofactor{} {}
        explicit(unit_t::exponents != NQ{}) quantity(S const &s) : cofactor{s} {}
        template <typename S_RHS> explicit quantity(quantity<U,S_RHS,kind> const&rhs): quantity{rhs.cofactor}{}

        constexpr auto operator<=>(const quantity&) const = default; 

        constexpr quantity operator-(){return quantity{-cofactor}; }  
        constexpr quantity operator+(){ return *this; }  
        constexpr bool operator!() const { return !(bool)cofactor; }          
        constexpr explicit operator bool() const { return (bool) cofactor; }                  

        template <typename S_RHS, quantity_kind rhs_kind> 
        constexpr std::enable_if_t<details::plus_allowed(kind,rhs_kind), quantity&>
            operator+=(quantity<U,S_RHS> const &other)
        { 
            cofactor = this->value + other.cofactor; 
            return *this; 
        }  

        template <typename S_RHS, quantity_kind rhs_kind> 
            constexpr std::enable_if_t<details::minus_allowed(kind,rhs_kind), quantity&>
            operator-=(quantity<U,S_RHS> const &other)
        {
             cofactor = this->value - other.cofactor; 
             return *this; 
        }  

        template <typename S_RHS, quantity_kind rhs_kind> 
        friend constexpr std::enable_if_t<details::plus_allowed(kind,rhs_kind), 
                                          quantity<U,std::common_type_t<S,S_RHS>,kind>>        
            operator+(quantity const &lhs, quantity<U, S_RHS, rhs_kind> const &rhs)
        {
            return { lhs.cofactor() + rhs.cofactor() };
        }

        template <typename S_RHS, quantity_kind rhs_kind> 
        friend constexpr std::enable_if_t<details::minus_allowed(kind,rhs_kind), 
                                          quantity<U,std::common_type_t<S,S_RHS>,kind>>        
            operator-(quantity const &lhs, quantity<U, S_RHS, rhs_kind> const &rhs)
        {
            using C = std::common_type_t<S,S_RHS>;            
            return { lhs.cofactor() - rhs.cofactor() };
        }

        template <unit_type auto U_rhs, typename S_RHS, quantity_kind kind> 
        friend constexpr auto operator*(quantity const &lhs, quantity<U_rhs, S_RHS, kind> const &rhs)
        {
            using C = std::common_type_t<S,S_RHS>;            
            return quantity<U * U_rhs, C, kind>{lhs.cofactor * rhs.cofactor};
        }

        template <unit_type auto U_rhs, typename S_RHS> 
        friend constexpr auto operator/(quantity const &lhs, quantity<U_rhs, S_RHS, kind> const &rhs)
        {
            using C = std::common_type_t<S,S_RHS>;            
            return quantity<U / U_rhs, C, kind>{lhs.cofactor / rhs.cofactor};
        }

        template <unit_type auto U2, arithmetic_type S2>
        friend constexpr bool operator<=>(quantity const &lhs, quantity<U,S2,kind> const &rhs) 
            requires requires(S a, S2 b){ {a<=>b} -> std::convertible_to<bool>; } 
            && requires { {U==U2} -> std::convertible_to<bool>; } 
        {
            return lhs.value <=> rhs.value;
        }

    };



}