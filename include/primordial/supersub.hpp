#pragma once

#include <cstdint>
#include <iostream>
#include <ostream>
#include <concepts>

///\file for UTF8 streamout for characters in the 7 bit range, which are supported by UNICODE
///\todo stream input
namespace primordial{

    ///\brief determines the verical position of text within the line
    enum class vertical_text_position { 
        regular = 0, ///< usual stream, output
        super = 1,   ///< render integer digits and sign in superscript
        sub = 2      ///< render integer digits and sign in subscript 
    };    

    ///\brief choose between portable, ugly display style (ascii) and pretty non portable one for integer output in super- and subscript
    enum class encoding { 
        ascii,  ///< use only ascii base (7-bit) set for superscript rendering (LaTeX style  ^{-123} _{-123})
        utf8    ///< use UTF8 super- and subscript code points to render digits and sign.
                ///< Currently only signed and unsigned integer types are supported, since Unicode does not support 
                ///< enough charcaters in super- and subscript for floating point or even string.
    };    

    template <typename T>
    struct text_decorator; ///< will render integers (including sign) in super or subscript
                           ///< you usuall will not construct instances directly, see helper functions below for usage:
                           ///\par example
                           ///\code
                           /// int x;
                           /// os << utf8(superscript(x));
                           ///\endcode
                           ///\note unimplemented, see specializations
    template <std::integral T>
    struct text_decorator<T>; ///< specialization for types currently supported                          
                
    template <typename T>  constexpr text_decorator<T> superscript(T const & t);  ///< create a superscript textdecorator for the argument 
    template <typename T>  constexpr text_decorator<T> subscript(T const & t);    ///< create a subscript textdecorator for the argument     
    template <typename T>  constexpr text_decorator<T> utf8(T const & t);         ///< create an UTF8 textdecorator for the argument 
   
    template <typename T>  constexpr text_decorator<T> regular(T const & t);      ///< return copied decorator, but change vertical text position to regular
    template <typename T>  constexpr text_decorator<T> superscript(T const & t);  ///< return copied decorator, but change vertical text position to regular  
    template <typename T>  constexpr text_decorator<T> subscript(T const & t);    ///< return copied decorator, but change  vertical text position to regular   
    template <typename T>  constexpr text_decorator<T> utf8(T const & t);         ///< return copied decorator, but change  encoding to utf8
    template <typename T>  constexpr text_decorator<T> ascii(T const & t);        ///< return copied decorator, but change  encoding to ascii

    template <typename T,typename Other>  
    constexpr text_decorator<T> rebind(text_decorator<Other> const &source, 
                                       T const &);                               ///< create a text decorator for a new variable with sam attributes as `source`

    std::ostream &operator<<(std::ostream &os, text_decorator<std::intmax_t> const &t);      ///< overload
    std::ostream &operator<<(std::ostream &os, text_decorator<std::uintmax_t> const &t);     ///< overload 
    template <std::signed_integral T>
    std::ostream &operator<<(std::ostream &os, text_decorator<T> t); ///< overload
    template <std::unsigned_integral T>
    std::ostream &operator<<(std::ostream &os, text_decorator<T> t); ///< overload    

// TEMPLATE IMPLEMNTATIONM AND IMPLEMENTATION DETAILS /////////////////////////////////////////////////////////////////

    ///\brief type to store encoding and vertical text position information along with a reference of a variable to stream.
    /// This class template has a separate stream output operator, which allows customization of th text rendering.
    ///\tparam type if wrapper variable to stream out.
    
    template <std::integral T>
    struct text_decorator<T>
    {
        T const &t;
        vertical_text_position pos = vertical_text_position::regular;
        encoding enc = encoding::utf8;
    };

    template <typename T> 
    constexpr text_decorator<T> superscript(T const & t)
    {
        return text_decorator<T>{t, vertical_text_position::super};
    }

    template <typename T> 
    constexpr text_decorator<T> subscript(T const & t)
    {
        return text_decorator<T>{t, vertical_text_position::sub};
    }

    template <typename T> 
    constexpr text_decorator<T> utf8(T const & t)
    {
        return text_decorator<T>{t, vertical_text_position::regular, encoding::utf8};
    }     

    template <typename T> 
    constexpr text_decorator<T> regular(text_decorator<T> t)
    {
        t.pos = vertical_text_position::regular;
        return t;
    }    

    template <typename T> 
    constexpr text_decorator<T> superscript(text_decorator<T> t)
    {
        t.pos = vertical_text_position::super;
        return t;
    }        

    template <typename T> 
    constexpr text_decorator<T> subscript(text_decorator<T> t)
    {
        t.pos = vertical_text_position::sub;
        return t;
    }       

    template <typename T> 
    constexpr text_decorator<T> utf8(text_decorator<T> t)
    {
        t.enc = encoding::utf8;
        return t;
    }             

    template <typename T> 
    constexpr text_decorator<T> ascii(text_decorator<T> t)
    {
        t.enc = encoding::ascii;
        return t;
    }            

    template <typename T,typename Other>  
    constexpr text_decorator<T> rebind(text_decorator<Other> const &source, ///< [in] decorator providing attributes
                                       T const &t                           ///< [in] new variable
                                      )
    {
        return text_decorator<T>{t, source.pos, source.enc};
    }
        
    template <std::signed_integral T>
    inline std::ostream &operator<<(std::ostream &os, text_decorator<T> t)
    {
        intmax_t v{t.t};
        return os << text_decorator<intmax_t>(v);
    }

    template <std::unsigned_integral T>
    inline std::ostream &operator<<(std::ostream &os, text_decorator<T> t) 
    {
        uintmax_t v{t.t};
        return os << text_decorator<uintmax_t>(v);
    }

} // namespace ct