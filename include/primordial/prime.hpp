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
                ? numeric_limits<T>::digits - countl_zero(v) - 1 // std::countl_zero
                : -1;
        }
    }

    ///\par complexity
    /// - o(1) for low primes (up to 19) and non prime powers of 2
    /// - o(n) otheriwise
    constexpr bool is_prime(std::unsigned_integral auto k)
    {
        using std::numeric_limits;

        using T = decltype(k);
        bool const is_odd = k &  T(1);
        if (not is_odd) return k == 2U; // divisible by 2, but not  2
        else
        {
            switch (k)
            {
                    case 1: 
                    case 9:                     
                    case 15: return false;

                    case 3: 
                    case 5: 
                    case 7: 
                    case 11: 
                    case 13:
                    case 17: 
                    case 19: return true;                

                    default: {
                        if (!(k % 3)|| !(k % 5) || !(k % 7) || !(k % 11) || !(k % 13)  || !(k % 17) || !(k % 19))
                            return false; // divisble by 3, 5, 7, 11, 13, 17, 19
                        
                        auto const log2k = details::log2(k);
                        T const sqrt_bound = k >> (log2k / 2);
                        T const tested_bound = (k + 22) / 23; // one factor must be at least 23
                        T const bound = std::min(sqrt_bound, tested_bound); 
                        
                        for (uintmax_t j = 5U; j <= bound; )
                        {
                            if (!(k % j)) return false; // divisble by k
                            j += 2;

                            if (!(k % j)) return false; // divisble by k
                            j += 4;
                        }

                        return true;
                    }
            }
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
                : is_prime(-make_signed_t<decltype(k)>(k));
    }

    ///\retval 0 on overflow
    constexpr uintmax_t next_prime(uintmax_t n)
    {
        using T = decltype(n);
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
            default: {
                switch(n % T(6)) // start value
                {
                case 0: n += 1; break;   
                case 1: n += 4; break;   
                case 2: n += 3; break;
                case 3: n += 2; break;
                case 4: n += 1; break;
                case 5: n += 2; break;               
                }

                // increments
                auto const m = n  % T(6) == 1;
                T const a = m ? 4 : 2;
                T const b = m ? 2 : 4;

                for(;;)
                {
                    if (is_prime(n)) return n; else n += a;
                    if (is_prime(n)) return n; else n += b;
                }           
            }        
        }
    }

    ///\note we rate 1 as the zeroth prime
    constexpr uintmax_t nth_prime(unsigned n)
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
            default: {
                n -= 8U;
                uintmax_t p = 22U;
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