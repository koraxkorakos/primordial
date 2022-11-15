#include "demangle.hpp"

#include <stdexcept>
#include <iostream>
#include <memory>
#ifdef __GNUC__        
#  include <cxxabi.h>
#endif

namespace ct
{
    namespace testing
    {    
        Demangler::Demangler(char const *p) : name(p)
        {
            if (!name) throw std::invalid_argument("null pointer");
        }

        std::string demangle(char const name[])
        {
            if (!name) throw std::invalid_argument("nullptr");
    #ifdef __GNUC__        
            auto const deleter = [](void *p){ if (p) free(p); };
            int status;
            auto real_name_ptr = std::unique_ptr<char,decltype(deleter)>(abi::__cxa_demangle(name, nullptr, nullptr, &status));
            return std::string(real_name_ptr.get() && status == 0 ? real_name_ptr.get() : name);
    #else
            return std::string(name);
    #endif        
        }

        std::ostream &operator<<(std::ostream &os, Demangler d)
        {
            auto const deleter = [](void *p){ if (p) free(p); };
            int status;
            auto real_name_ptr = std::unique_ptr<char,decltype(deleter)>(abi::__cxa_demangle(d.name, nullptr, nullptr, &status));

            return os << (real_name_ptr.get() && status == 0 ? real_name_ptr.get() : d.name);
        }    
    }
}