#pragma once

#include <type_traits>
#include <utility>
#include <primordial/nq.hpp>

#ifndef PRIMORDIAL_MAX_DIMS
#  define PRIMORDIAL_MAX_DIMS 10
#endif

namespace primordial
{
    template <typename E> 
    concept dim_enum = enum_type<E> 
        && std::bool_constant<(E::last_dim > 0 && E::last_dim <= PRIMORDIAL_MAX_DIMS)>::value;

    template <dim_enum E>
    constexpr unsigned size(){ return std::underlying_type_t<E>(E::last_tag) + 1U; }

    /// CRTP
    ///\par usage
    ///\code
    ///  struct MyUnitSystem : public unit_system<D, my_dimension_enum>{};
    ///\endcode
    template <typename D, dim_enum d> 
    struct unit_system
    {
    public:
        using dimensions = d;        
    protected: 
        constexpr unit_system();
        constexpr unit_system(unit_system const &);
    };
 
    template <typename D> 
    concept unit_system_type = std::derived_from<D,unit_system<D,typename D::dimensions>>;

   template <unit_system_type S, typename S::dimensions dim> 
        requires std::derived_from<S,unit_system<S, decltype(dim)>>
   struct unit 
   {
      using system = S;
      using dimensions = S::dimensions;
      static constexpr dimensions dimension = dim;
   };

    template <typename T> 
    concept unit_type = std::same_as<T, unit<typename T::system, T::dim>>;



}
   
