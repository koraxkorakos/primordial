#pragma once

#include <primordial/concepts.hpp>
#include <type_traits>

namespace primordial
{

#if (__cplusplus <= 202002L)
    constexpr auto to_underlying(enum_type auto e)    { return std::underlying_type_t<decltype(e)>(e); }
#else
    using std::to_underlying;
#endif

}