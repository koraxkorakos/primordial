#pragma once

#include <concepts>
#include <limits>
#include <type_traits>

///\def CT_BASE_UNITS_NUMBER_LIMIT for safety reasons (runtime) limit the number of bases

namespace primordial 
{

    template <typename E> 
    concept enum_type = std::is_enum_v<E>;

    template <typename T> 
    concept arithmetic_type = std::is_arithmetic_v<T> || requires(T a){
        { a } ;
    };


    template <typename T, int digits>
    concept  has_digits = std::bool_constant<std::numeric_limits<T>::digits == digits>::value;

} // namespace 