#include <primordial/supersub.hpp>
#include <iostream>
#include <concepts>
#include <limits>
#include <cassert>

namespace primordial
{
namespace
{
    constexpr size_t table_size = 12U;

    using substitution_table_type = char const * const [table_size]; 
    
    template <vertical_text_position>
    constexpr substitution_table_type substitution_table; 

    template <> 
    constexpr char const * const substitution_table<vertical_text_position::regular>[] 
        = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "+", "-"};

    ///\note misusing "¯" because most fonts lack  superscript '-' ;
    template <>
    constexpr char const * const substitution_table<vertical_text_position::super>[] 
        = {"⁰", "¹", "²", "³", "⁴", "⁵", "⁶", "⁷", "⁸", "⁹", "⁺", "¯"};  

    template <>
    constexpr char const * const substitution_table<vertical_text_position::sub>[] 
        = {"₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₊", "₋"};

    constexpr auto get_substitution_table(vertical_text_position pos)
    {
        switch(pos)
        {
            case vertical_text_position::regular: return substitution_table<vertical_text_position::regular>;
            case vertical_text_position::super: return substitution_table<vertical_text_position::super>;
            case vertical_text_position::sub: return substitution_table<vertical_text_position::sub>;
        }
        assert(false);
        return substitution_table<vertical_text_position::regular>;
    }

    template <std::integral T>
    void print(std::ostream &os, text_decorator<T> const &t) 
    {
        using riter = std::reverse_iterator<char*>;
        using std::find;
        using std::fill_n;
        using std::stringstream;
        using std::numeric_limits;
        using std::for_each;
        using std::dec;

        ///\todo make both enc the same with respect to manipulators and flags
        switch(t.enc)
        {
            case encoding::ascii: 
            { 
                char const * pre = "";
                char const * post = pre;
                switch(t.pos)
                {
                    case vertical_text_position::super: pre = "^{"; post = "}"; break;
                    case vertical_text_position::sub: pre = "_{"; post = "}"; break;
                }
                auto const flags = os.flags();
                os << pre << std::dec << t.t << post;
                os.setf(flags);                
            }
            case encoding::utf8:             
            {
                auto const tbl = get_substitution_table(t.pos);

                constexpr auto bufsize = numeric_limits<std::uintmax_t>::max_digits10 + 3;
                char buf[bufsize];
                fill_n(buf, bufsize, 0);
                sprintf(buf + 1, "%lld", (long long)t.t);
                bool const is_negative = t.t < 0;
                buf[0] = is_negative ? '-' : '+'; 

                for_each(buf + is_negative, find(buf + 0, buf + bufsize, '\0'), [&os,&tbl](auto c)
                {
                    switch(c)
                    {
                    case '0':    
                    case '1':    
                    case '2':    
                    case '3':                            
                    case '4':    
                    case '5':    
                    case '6':    
                    case '7':    
                    case '8':                            
                    case '9': os << tbl[c - '0']; break;
                    case '+': os << tbl[10]; break;
                    case '-': os << tbl[11]; break;       
                    default:
                        os << c;
                    }
                });
            }
        }
    }
}

std::ostream &operator<<(std::ostream &os, text_decorator<std::intmax_t> const &t)
{
    print(os, t) ;
    return os;
}

std::ostream &operator<<(std::ostream &os, text_decorator<std::uintmax_t> const &t)
{
    print(os, t) ;
    return os;
}
}
