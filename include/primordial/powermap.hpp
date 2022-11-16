#pragma once
#include <array>
#include <primordial/nq.hpp>
#include <primordial/prime.hpp>
//#include <primordial/details/utility.hpp>

///\file utilities to cache prime number mapping and powers of a unit

namespace primordial
{
    constexpr int  get_power_in(unsigned p, NQ q)
    {
        auto const np =  get_power_in(p, q.num());
        return np  > 0 ? np : -get_power_in(p, q.den());
    }

    constexpr int num_dims(NQ q)
    {
        switch (q.get_kind())
        {
            case NQ::kind::defective: return -1;
            case NQ::kind::zero: return 0;                
            default:
            {
                int dims = 0;
                auto n = q.num();
                auto d = q.den();
                for(uintmax_t prime = 2; 
                     n > 1 || d > 1;
                     ++dims, prime = next_prime(prime)) 
                {                    
                    while(n % prime == 0) n /= prime;
                    while(d % prime == 0) d /= prime; 
                }
                return dims;
            }
        }
    }

    template <NQ q>
    constexpr auto make_powermap()
    {        
        std::array<unsigned, num_dims(q)> a;
        uintmax_t p = 0;
        for (unsigned k=0; k != a.size(); ++k)
        {
            p = next_prime(p);
            a[k] = get_power_in(p, q);
        }
        return a;
    }

    template <NQ q> constexpr auto power_map = make_powermap<q>();

}