#pragma once

#include <primordial/unit_system.hpp>
#include <primordial/powermap.hpp>
#include <primordial/utility.hpp>
#include <primordial/supersub.hpp>
#include <iostream>
#include <iterator>
#include <span>

///\todo register a facet to allow utf8 output

///\file the code here is more complicated than needed to reduce code genaeration due to templates
///
/// To customize your base unit names in streaming, do the following:
///
/// - Create a default constructible class that realizes `unit_streamer_interface` and implements the abstract method
///   to write the name of the kth base unit to the stream passsed as outout parameter.
/// - Register your class with a unit system by speciualizing `unit_streamer_trait`
/// - instantiate your unit streamer
/// Example
///\code 
/// struct MyUnitStreamer{
///    virtual void streamput(std::ostream &os, unsigned k) const override{
///        switch(k){
///          case 0 : os "m"; break;
///          case 1 : os "kg"; break;
///          case 2 : os "s"; break;
///     }
/// }; 
///
/// template <> struct unit_streamer_trait<MKS> : std::type_identity<>{}; // if MKS is your unit system
///\endcode

namespace primordial
{
    struct default_unit_streamer;

    /// specialize of needed for other unit names
    template <unit_system_type sys>
    struct unit_streamer_trait : std::type_identity<default_unit_streamer>{};

    template <unit_system_type sys>
    using  unit_streamer_trait_t = typename unit_streamer_trait<sys>::type;


    struct unit_streamer_interface
    {
        virtual void streamput(std::ostream &os, unsigned k) const = 0;
    protected:
        unit_streamer_interface() = default;    
        unit_streamer_interface(unit_streamer_interface const &) = default;
        unit_streamer_interface &operator=(unit_streamer_interface const &) = default;
        ~unit_streamer_interface() = default;
    };

    struct default_unit_streamer : unit_streamer_interface
    {
        virtual void streamput(std::ostream &os, unsigned k) const override;
    };

    namespace details
    {
        struct unit_stream_writer
        {
            template <unit_system_type sys, NQ q> 
            explicit unit_stream_writer(unit<sys,q>) 
                : streamer{unit_streamer_trait_t<sys>{}}
                , powers{power_map<q>} {}

            unit_streamer_interface const &streamer;
            std::span<int const> powers;
        };

        ///\brief uses minuscle letters starting from 'a' in increasing order for the unit names to stream
        ///\return `os`
        std::ostream &operator<<(std::ostream &os, unit_stream_writer const &s);

    }


    ///\brief uses minuscle letters starting from 'a' in increasing order for the unit names to stream
    ///\return `os`
    template <unit_system_type sys, NQ q>
    std::ostream &operator<<(std::ostream &os, unit<sys,q> const &u)
    {
        return os << details::unit_stream_writer{u};
    }

}


