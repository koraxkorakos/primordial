#pragma once

#include <primordial/concepts.hpp>

namespace primordial
{

#if (__cplusplus <= 202002L)
    constexpr auto to_underlying(enum_type e)    { return std::underlying_t<decltype(e)>(e); }
#else
    using std::to_underlying;
#endif

}