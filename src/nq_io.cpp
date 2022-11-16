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

std::ostream &operator<<(std::ostream &os, NQ::kind k)
{
    switch(k)
    {
        case NQ::kind::defective: return os << "defective";
        case NQ::kind::zero: return os << "zero";
        case NQ::kind::valid: return os << "valid";
        default: os << "illegal_value(" << std::underlying_type_t<NQ::kind>(k) << ')';
    }
    return os;
}

} // namespace ct