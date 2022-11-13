#include <primordial/nq_io.hpp>
#include <ostream>

namespace primordial
{

///\brief print in the form `99999/999999` 
///\odod
std::ostream &operator<<(std::ostream &os, NQ const & v)
{
    return os << v.num() << '/' << v.den(); 
}


} // namespace ct