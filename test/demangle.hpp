#pragma once

#include <string>
#include <iosfwd>
#include <typeinfo>

namespace primordial
{
    namespace testing
    {
        std::string demangle(char const name[]);

        struct Demangler
        {
            explicit Demangler(char const *p); ///< \pre `p` and string is zero terminated
            char const *name;
        };

        template <typename T>
        struct TypeDemangler : Demangler
        {
            explicit TypeDemangler() : Demangler(typeid(T).name()){}
        };

        std::ostream &operator<<(std::ostream &os, Demangler d);
    }
}