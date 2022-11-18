#pragma once

#include <type_traits>

namespace primordial
{
    namespace meta
    {
        template <typename... T> struct type_list {
            using type = type_list;
        };

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

            template <bool found> struct get_; 

            template <> struct get_<true> 
            {
                template <unsigned, typename T, typename... L> using apply = T;
            };

            template <> struct get_<false> 
            {
                template <unsigned k, typename T, typename... L> using apply = get_<!(k-1)>::template apply<k-1, L...>;
            };

        }

        template <unsigned k, typename... L>
        using get = typename details::get_<!k>::template apply<k,L...>;

        namespace details
        {
            template <unsigned k, typename L> struct at_ : at_<k,typename L::type>{};
            template <unsigned k, typename... L> struct at_<k,type_list<L...>>
            {
                using type = get<k,L...>;
            };
        }

        template <unsigned k, typename L> using at = typename details::at_<k, L>;

        /// same as `std::constitional_t<bool,typename,typename>`, but better complexity.
        /// \par complexity 
        ///   - O(0)
        template <bool condition, typename Yes, typename No>
        using conditional_t = typename details::cond<condition>:: template apply<Yes,No>;


        ///\brief get position of _first_ occurence of `F` in `L...` .
        ///\note partial metafunction 
        ///\pre `F` in `L`
        ///\todo optimize complexity from O(n) to O(1)
        template<typename F, typename... L> struct pos;
        template<typename F, typename T, typename... L> struct pos<F,T,L...> : std::integral_constant<unsigned, 1U + pos<F,L...>::value>{};
        template<typename F, typename... L> struct pos<F,F,L...> : std::integral_constant<unsigned, 0>{};

    }

}