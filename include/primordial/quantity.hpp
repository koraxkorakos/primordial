#pragma once

#include <primordial/nq.hpp>
#include <primordial/dims.hpp>
#include <primordial/powermap.hpp>

namespace primordial
{           
    template <unit_type, NQ, arithmetic_type>  class quantity;                    
    template <unit_type u, arithmetic_type a>  class quantity<u,NQ::defective(),a>{}; ///< poison this one
    
    namespace details
    {
        template <arithmetic_type S, bool is_proper = true> // not zero or defective
        struct qantity_base
        {
            constexpr S const get_cofactor() const{ return co_factor_; }
            constexpr void set_cofactor_(S v){ co_factor_= v; }        
            S co_factor_ = S{};
        };

        template <arithmetic_type S> 
        struct qantity_base<S, false>
        {
            constexpr S get_cofactor() const{ return S{}; }
            constexpr void set_cofactor_(S v){}        
        };        
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
    template <unit_type unit, NQ exponents, arithmetic_type S> // S could also be array
    class quantity : private details::qantity_base<S,(bool) exponents.num_>{
        using inherited = details::qantity_base<S,(bool) exponents.num_ >;
    public:
        using value_type = S;
        using unit_t = unit;
        static constexpr NQ exp = exponents;

        constexpr quantity(){}
        explicit(exponents = NQ{}) quantity(S s){ this->set_cofactor(s); }

        constexpr bool is_defective() const{ return !exponents.valid(); } // overflows
        constexpr bool is_scalar() const{ return exponents == NQ::zero(); }

        using inherited::get_cofactor;
        using inherited::set_cofactor;

        constexpr auto operator<=>(const quantity&) const = default;

        constexpr quantity operator-(){return quantity{-this->value()}; }  
        constexpr quantity operator+(){ return *this; }  
        constexpr bool operator!(){ return !*this; }          

        template <typename S_RHS> 
        constexpr quantity &operator+=(quantity<unit,exponents,S_RHS> const &other)
        { 
            set_cofactor(this->value + other.get_cofactor()); return *this; 
        }  

        template <typename S_RHS> 
        quantity &operator-=(quantity<unit,exponents,S_RHS> const &other)
        {
             set_cofactor(this->value - other.get_cofactor()); return *this; 
        }  

        template <typename S_RHS> 
        friend auto operator+(quantity const &lhs, quantity<unit, exponents, S_RHS> const &rhs)
        {
            using C = std::common_type_t<S,S_RHS>;
            return quantity<unit,exponents,C>(lhs.get_cofactor()) += rhs;
        }

        template <typename S_RHS> 
        friend auto operator-(quantity const &lhs, quantity<unit, exponents, S_RHS> const &rhs)
        {
            using C = std::common_type_t<S,S_RHS>;            
            return quantity<unit,exponents,C>(lhs.get_cofactor()) -= rhs;
        }

        template <NQ base_rhs, typename S_RHS> 
        friend auto operator*(quantity const &lhs, quantity<unit, base_rhs, S_RHS> const &rhs)
        {
            using C = std::common_type_t<S,S_RHS>;            
            using resulting_unit = quantity<unit, exponents + base_rhs, C>; // type of result
            return resulting_unit(lhs.get_cofactor() * rhs.get_cofactor());
        }

        template <NQ base_rhs, typename S_RHS> 
        friend auto  operator/(quantity const &lhs, quantity<unit, base_rhs, S_RHS> const &rhs)
        {
            using C = std::common_type_t<S,S_RHS>;            
            using resulting_unit = quantity<unit, exponents - base_rhs, C>;
            return resulting_unit(lhs.get_cofactor() / rhs.get_cofactor());
        }
    };
}