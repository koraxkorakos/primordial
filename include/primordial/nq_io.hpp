#pragma once

#include <iosfwd>
#include <primordial/nq.hpp>

namespace primordial {

///\brief print in the form `99999/999999` 
std::ostream &operator<<(std::ostream &os, NQ const & v);


} // namespace ct