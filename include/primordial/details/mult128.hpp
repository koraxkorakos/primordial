#pragma once

#include <concepts>
#include <cstdint>
#include <limits>
#include <primordial/details/type_traits.hpp>

namespace primordial
{

///\overload
constexpr bool umult_overflow(uintmax_t a, uintmax_t b, uintmax_t &result);

///\brief 64bit multiplication with overflow detection
///\retval true `a * b` did  overflow
///\retval false otherwise
///\post `result == a * b`
template <std::unsigned_integral T> 
constexpr bool umult_overflow(T a, T b, T &result);


///\brief 64bit multiplication with overflow detection
///\par algorithm
/// - long division
///\retval true `a * b` did  overflow
///\retval false otherwise
///\post `result == a * b`
template <std::unsigned_integral T>
constexpr bool umult_overflow_longdiv(T a, T b, T &result);

// IMPLEMENTATION ////////////////////////////////////////////////////////////////////////////////////////////////

template <std::unsigned_integral T>
constexpr bool umult_overflow(T a, T b, T &result) 
{
    constexpr auto digits = std::numeric_limits<T>::digits;
    using D = uint_type_t<digits * 2>;    
    auto const r = D{a} * D{b};
    result = static_cast<T>(r);
    return r >> digits;
}

template <std::unsigned_integral T>
constexpr bool umult_overflow_longdiv(T a, T b, T &result)
{
    constexpr int digits = std::numeric_limits<T>::digits;
    constexpr int half_digits = digits >> 1;
    constexpr T mask = ~(~T() >> half_digits);
    using H = uint_type_t<half_digits>;

    result = a * b; 

    auto const a_lo = static_cast<H>(a);
    auto const a_hi = static_cast<H>(a >> half_digits);
    auto const b_lo = static_cast<H>(b);
    auto const b_hi = static_cast<H>(b >> half_digits);

    auto const ll = a_lo * b_lo;
    auto const hl = a_hi * b_lo;  
    auto const lh = a_lo * b_hi;  
    auto const hh = a_hi * b_hi;    

     bool const ov((hl | lh) & mask | hh | ((T(ll) + T(hl) + T(lh) + T(hh)) >> half_digits));
     return ov;
}

#ifndef CT_NO_CXX_EXTENSION
#  ifdef __SIZEOF_INT128__
constexpr bool umult_overflow_large(uint64_t a, uint64_t b, uint64_t &result){
#   define CT_HAS_UMULT_OVERFLOW_LARGE  
    auto const p =  __uint128_t(a) * __uint128_t(b);
    result = static_cast<uint64_t>(p);
    return static_cast<bool>(p >> 64);
}
#  elif (defined(__GNUC__) || defined(__clang__))
#   define CT_HAS_UMULT_OVERFLOW_INTRINSIC    
constexpr bool umult_overflow_intrinsic(uint64_t a, uint64_t b, uint64_t &result){
    return __builtin_umulll_overflow(a, b, &result);
}
#  endif

#endif

constexpr bool umult_overflow(uintmax_t a, uintmax_t b, uintmax_t &result)
{
    static_assert(std::is_same<uintmax_t, uint64_t>::value);
#ifdef CT_HAS_UMULT_OVERFLOW_LARGE
    return umult_overflow_large(a,b,result);
#elif defined(CT_HAS_UMULT_OVERFLOW_INTRINSIC)
    return umult_overflow_intrinsic(a, b, result);
#else    
    return umult_overflow_longdiv(a, b, result);
#endif
}

}