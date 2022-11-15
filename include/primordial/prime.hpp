#pragma once

#include <algorithm>
#include <bit>
#include <concepts>
#include <limits>
#include <type_traits>
#include <cstdint>

#include <primordial/concepts.hpp>

namespace primordial
{
    namespace details{
        ///\retval -1 if `!v` to indicate failure
        constexpr int log2(std::unsigned_integral auto v)
        {
            using std::countl_zero;            
            using std::numeric_limits;
            using T = decltype(v);
            return v 
                ? numeric_limits<T>::digits - std::countl_zero(v) - 1 // std::countl_zero
                : -1;
        }
    }

    ///\par complexity
    /// - o(1) for low primes (up to 19) and non prime powers of 2
    /// - o(n) otherwise
    constexpr bool is_prime(std::uintmax_t k)
    {        
        if (k <= 1) return false;
        if (k % 2 == 0) return k == 2;
        if (k % 3 == 0) return k == 3;
        if (k % 5 == 0) return k == 5;
        else{
            auto const log2k = details::log2(k);
            decltype(k) const sqrt_bound = (1ULL << (log2k / 2 + 1)) - 1;            
            for(auto j = 5; j <= sqrt_bound;)            
            {   
                j += 2;                
                if (k % j == 0) return false;
                j += 4;
                if (k % j == 0) return false;
            }
            return true;
        }
    }

    ///\retval false if not representable or not prime
    ///\pre required twos complemment, ones complement, sign and magnitude representation or the like, where negative range is not more than 1 larger than positive.
    /// `numeric_limits<T>::min() + numeric_limits<T>::max() + T(1) >= T()? 
    constexpr bool is_prime(std::signed_integral auto k)
    {
        using T = decltype(k);
        using std::numeric_limits;

        static_assert(-(numeric_limits<T>::min() + numeric_limits<T>::max()) <= T(1)) ;

        return  k < -numeric_limits<T>::max() //not representable positively
                ? false
                : is_prime(std::make_signed_t<decltype(k)>(-k));
    }

    ///\retval 0 on overflow
    /*constexpr*/ uintmax_t next_prime(uintmax_t n)
    {
        switch (n)
        {
            case 0: 
            case 1: return 2U;
            case 2: return 3U;
            case 3: 
            case 4: return 5U;
            case 5: 
            case 6: return 7U;
            case 7: 
            case 8: 
            case 9: 
            case 10: return 11U;            
            case 11: 
            case 12: return 13U; 
            case 13: 
            case 14: 
            case 15: 
            case 16: return 17U;
            case 17:  
            case 18: return 19U;
            case 19:
            case 20:
            case 21:                          
            case 22: return 23U;                         
            case 23: 
            case 24: 
            case 25:             
            case 26:                         
            case 27:                                     
            case 28: return 29U;
            case 29: 
            case 30: return 31U;                        
            case 31: 
            case 32: 
            case 33: return 37U;
            default: { 
                auto n_old = n;
                switch (n % 6)
                {
                case 0: n += 1; break; 
                case 1: n += 4; break; 
                case 2: n += 3; break; 
                case 3: n += 2; break; 
                case 4: n += 1; break; 
                case 5: n += 2; break;                                                                                 
                }               
                for(;;)
                {
                    if (is_prime(n)) return n;
                    n_old = n;
                    n += 4;
                    n = n_old;                   
                    if (n < n_old) return 0;

                    if (is_prime(n)) return n;
                    n_old = n;
                    n += 1; 
                    if (n < n_old) return 0;
                }
            }        
        }

        return 0;
    }

    ///\note we rate 1 as the zeroth prime
    /*constexpr*/ uintmax_t nth_prime(unsigned n)
    {
        switch (n)
        {
            case 0: return 1U; 
            case 1: return 2U;
            case 2: return 3U;
            case 3: return 5U;
            case 4: return 7U;
            case 5: return 11U;
            case 6: return 13U;
            case 7: return 17U;
            case 8: return 19U;
            case 9: return 23U;
            case 10: return 29U;            
            default: {
                n -= 10U;
                uintmax_t p = 30U;
                for(; n; --n) p = next_prime(p); 
                return p;
            }        
        }
    } 

    ///\brief compute how many times the number `q` can be divided by `p` without rest
    constexpr int get_power_in(uintmax_t a, uintmax_t b)
    {
        int power = 0;        
        if (!(a && b)) return 0;
        for (int power = 0;  !(b %= a);  b /= a, ++power);
        return power;
    }

}