#pragma once

#include <type_traits>

namespace primordial
{

    namespace meta
    {
        namespace details
        {
            template<bool> struct cond;

            template<>
            struct cond<true>
            {
                template <typename T, typename> using apply = T;
            };

            template<>
            struct cond<false>
            {
                template <typename, typename T> using apply = T;
            };
       

        }

        /// same as `std::constitional_t<bool,typename,typename>`, but better complexity.
        /// \par complexity 
        ///   - O(0)
        template <bool condition, typename Yes, typename No>
        using conditional_t = typename details::cond<condition>:: template apply<Yes,No>;


        ///\brief get position _first_ occurence of `F` in `L...` .
        ///\note partial metafunction 
        ///\pre `F` in `L`
        ///\todo optimize complexity from O(n) to O(1)
        template<typename F, typename... L> struct pos;
        template<typename F, typename T, typename... L> struct pos<F,T,L...> : std::integral_constant<unsigned, 1U + pos<F,L...>::value>{};
        template<typename F, typename... L> struct pos<F,F,L...> : std::integral_constant<unsigned, 0>{};


        template <typename... T> struct type_list {
            using type = type_list;
        };

    }

}