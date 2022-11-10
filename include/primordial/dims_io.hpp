#pragma once

#include <iostream>
#include <utility>
#include <primordial/dims.hpp>

namespace primordial
{
    ///\brief by default unit names are capital letters starting from `'A'`
    /// overload if you want other names 
    template <unit_type U>
    std::ostream &operator<<(std::ostream &os, U)
    {        
        return os << char('a' + to_underlying(U::dim));
    }

    ///\brief by default dimensions names are capital letters starting from `'A'`
    /// overload if you want other names 
    template <dim_enum dims>
    std::ostream &operator<<(std::ostream &os, dims dim)
    {
        return os << char('A' +  to_underlying(dim));
    }

/*
    template <typename T, dim_enum dims>
    std::ostream &operator<<(std::ostream &os, unit_system<T,dims> const &)
    {
        auto const first = 0;
        auto const last = to_underlying(dims::last_dim);

        for (auto k = first;
            k <= last;
            ++k)
        {
            os << '[' << dims(k) << ']' << unit<T>(k); // Oops not in the runtime world
            if (k <last) os << ' ';
        }

        return os << char('a' + to_underlying(u));
    }
*/

}