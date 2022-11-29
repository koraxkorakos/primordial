#pragma once

#include <iostream>
#include <primordial/quantity.hpp>
#include <primordial/unit_system_io.hpp>

namespace primordial
{
    template <unit_type auto U, arithmetic_type S, quantity_kind kind>
    std::ostream &operator<<(std::ostream &os, quantity<U,S,kind> const &q)
    {
        return os << q.cofactor << ' ' << U;
    }
}