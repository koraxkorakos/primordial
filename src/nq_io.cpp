#include <primordial/nq_io.hpp>
#include <ostream>

namespace primordial
{

///\brief print in the form `99999/999999` 
std::ostream &operator<<(std::ostream &os, NQ const & v)
{
    return os << '^' << v.num() << '_' << v.den(); ///\todo better format later
}


} // namespace ct