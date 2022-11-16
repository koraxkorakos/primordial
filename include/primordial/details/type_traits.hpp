#pragma once

#include <type_traits>
#include <cstdint>

namespace primordial {

#if defined(__SIZEOF_INT128__) && !defined(CT_NO_CXX_EXTENSION)

    ///file We cannot specilize the integer traits dor 128 bit type due to `std::maxint_t` and `std::maxint_t` etc. do not include 128 bit integers.

    template <typename T> struct is_integral: std::is_integral<T>{};
    template <typename T> struct is_signed : std::is_signed<T>{};
    template <typename T> struct is_unsigned : std::is_unsigned<T>{};
    template <typename T> struct make_signed : std::make_signed<T>{};
    template <typename T> struct make_unsigned : std::make_unsigned<T>{};
    
    template <typename T> constexpr auto is_integral_v = is_integral<T>::value;
    template <typename T> constexpr auto is_signed_v = is_signed<T>::value;
    template <typename T> constexpr auto is_unsigned_v = is_signed<T>::value;

    template <typename T> using make_signed_t = typename make_signed<T>::type;
    template <typename T> using make_unsigned_t = typename make_unsigned<T>::type;
#else
   using std::make_signed;
   using std::make_unsigned;
   using std::is_signed;
   using std::is_unsigned;
   using std::is_integral;
#endif 

    template <int bits> struct int_type;
    template <> struct int_type<8> : std::type_identity<int8_t>{};
    template <> struct int_type<16> : std::type_identity<int16_t>{};
    template <> struct int_type<32> : std::type_identity<int32_t>{};
    template <> struct int_type<64> : std::type_identity<int64_t>{};

    template <int bits> using int_type_t = typename int_type<bits>::type;

    template <int bits> struct uint_type;
    template <> struct uint_type<8> : std::type_identity<uint8_t>{};
    template <> struct uint_type<16> : std::type_identity<uint16_t>{};
    template <> struct uint_type<32> : std::type_identity<uint32_t>{};
    template <> struct uint_type<64> : std::type_identity<uint64_t>{};
    
#ifndef CT_NO_CXX_EXTENSION

#  ifdef __SIZEOF_INT128__
      template <> struct uint_type<128> : std::type_identity<__uint128_t>{};
      template <> struct int_type<128> : std::type_identity<__int128_t>{};
#  endif
#endif


    template <int bits> using uint_type_t = typename uint_type<bits>::type;


}

