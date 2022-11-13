#pragma once

#include <primordial/unit_system.hpp>
#include <primordial/powermap.hpp>
#include <primordial/utility.hpp>
#include <primordial/supersub.hpp>
#include <iostream>
#include <iterator>

///\todo register a facet to allow utf8 output

namespace primordial
{
    struct default_unit_streamer
    {
        explicit default_unit_streamer(unsigned dim_no) : sym(char('a' + dim_no)){}
        char sym;
        friend std::ostream &operator<<(std::ostream &os, default_unit_streamer s)
        {
                return os << s.sym;
        }
    };

    template <unit_system_type system, NQ q> 
    std::ostream &operator<<(std::ostream &os, unit<system, q> const &)
    {
        auto const & m = power_map<q>;

        auto const num = q.num();
        auto const den = q.den();

        int k = 0;

        if (num == 1) os << '1';
        else for (auto u : power_map<q>)
        {
            if (u > 0)
            {
                os << system::base_unit(k)();
                if (u != 1) os << superscript(u);       
            }
            ++k;
        }

        os << '/';

        k = 0;
        if (den != 1) for (auto u : power_map<q>)
        {
            if (u < 0)
            {
                os << system::base_unit(k)();
                if (u != -1) os << superscript(-u);       
            }
            ++k;            
        }

        return os; 
    }    
}


