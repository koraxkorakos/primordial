#pragma once

#include <iostream>
#include <primordial/quantity.hpp>

namespace primordial
{
    template <unit_type U, NQ nq, arithmetic_type S>
    std::ostream &operator<<(std::ostream &os, quantity<U,nq,S> const &q)
    {
        os << q.get_cofactor();
        for(int k=0; k!=power_map<nq>.size(); ++k)
        {
            auto const p = power_map<nq>[k];
            if (p)
            {
                os << U{};
                if (p != 1) os << U::dim << "^{" << power_map<nq>[k] << '}';
            }
        }
        return os;
    }
}