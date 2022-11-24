#include <primordial/unit_system_io.hpp>
#include <primordial/supersub.hpp>

namespace primordial
{
    unit_streamer_interface const &get_default_unit_streamer() noexcept
    {
        static default_unit_streamer s;
        return s;
    }
    
    void default_unit_streamer::streamput(std::ostream &os, unsigned k) const  
    {
        os << char('a' + k);
    }

    namespace details{

        std::ostream &operator<<(std::ostream &os, details::unit_stream_writer const &s)
        {
            bool const num_pos = any_of(s.powers.begin(), s.powers.end(), [](auto v){ return v > 0; });
            bool const num_neg = any_of(s.powers.begin(), s.powers.end(), [](auto v){ return v < 0; });
      
            if (!(num_pos || num_neg)) return os;

            if (num_pos)
            {
                unsigned k = 0;
                for (auto const p : s.powers)
                {
                    if (p > 0) // do not stream base unit if it has power 0 (not present)
                    {
                        s.streamer.streamput(os, k); // print base unit name

                        if (p > 1) os << '^' << p; //superscript(p);
                    }
                    ++k;
                }
            } else os << '1';

            if (num_neg)
            {
                unsigned k = 0;                
                os << '/';
                for (auto const p : s.powers)
                {
                    if (p < 0) // do not stream base unit if it has power 0 (not present)
                    {
                        s.streamer.streamput(os, k); // print base unit name
                           
                        if (p != -1) os << '^' << -p;//superscript(-p); // print exponent unless it is -1
                    }
                    ++k;
                }            
            }
            return os;
        }           
    }
}

