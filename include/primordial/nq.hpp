#pragma once

#include <primordial/details/mult128.hpp>
#include <numeric>

namespace primordial {

///\brief group of reduced fraction of positive integers
///
///\par Algebraic Structure
///
/// \f[ \left< \mathbb{Q}^\ast_+, \cdot, 1 \right> \f]
///
/// \f[ Q^\ast_+ = \left\lbrace (a,b) | a,b \in \mathbb{N}, \gcd(a,b) = 1 \right\rbrace \f] the set of reduced fractions of natural numbers.
///
///\par Group Operations
///
/// Multiplication \f$ \cdot \f$ is done  by memberwise application and reduction by the division by the greatest common divisor of
/// numerator and denomiator of the multiplication result, as is the usual way for fractions.
/// 
/// The neutral element is \f$ \mathbf{1} = \fract{1}{1} \f$ .
///
/// Te inverse element of  is computed by swapping numerator and denominator; \f$ \fract{n}{d} \fract{d}{n} = \mathbf{1} \f$ .
/// 
///\par Algebraic Properties
///
/// The \f$ \left< \mathbb{Q}^\ast_+, \cdot, 1 \right> \f$ is a monoid. It is:
///
/// - closed under the group operation:
/// - associative
/// - has aneutral element
/// - for each element an inverse according to the group operatio is defined.
///
/// Furthermore, \f$ \left< \mathbb{Q}^\ast_+, \cdot, 1 \right> \f$ is commutative.
///
///\par C++ Implmentation and Design Decisions
///    
/// In the C++ implementation we use then binray `+` and unary `-` operators for \f$ \cdot \f$ and \f$ a^{-1} \f$ , because no multiplicative 
/// inversion operator (reciprocal) exists in C++.  Furthermore, thisn emphasesized the fact that the group is commutative and closed under
/// inversion, which `operator/` would less well not suggest.
///
/// The implementation is meant be be in its totality usable for compiletime computations (`constexpr`). Due to this some usability restrivtion had to be made.
/// At compile time dynamic memory allocation is not possible, such tha the maximal representable  the numbers for numerator and denominator have to be defined ahed.
/// This for the representing natural numbers \f$ \mathbb{N} \f$ builtin 64 bit unsigned integers have been choosen. Thesese model the 
/// ring of integers modulo 64  \f$ \mathbb{Z}/2^{64}\mathbb{Z} \f$ . The modulo addition differs from the addition of our  multiplicative group of 
/// natural fraction overflows if an overflow occurs. The implementation avoids overflows, where possible. In only overflows, when the final result is not 
/// representable. Internally multiplication uses 128 bit arithmetic or compiler support to detect overflow.
///
/// This is implemented in `uint64_mult_overflow` the utility.hpp header. At the moment this header has two versions:
///
/// - using GCC/Clang intrinsics
/// - using 128 bit integer support from details int128.hpp
///
/// At the moment there is no fallback for pure Stanndard C++. It will follow in a later step, but likely be rather slow,
///
///\par Error Reporting 
///
/// The only operation, which could overflow is the multiplication, since the implementation is closed under inversion. Since the class is `constexpr`
/// me must always return a value on a function, neither can we throw an exception, nor terminate the application in a regular way (divide by zero causingf 
/// would be possible). We there for choose a special representation `NQ::defective`, which is \f$ \fract{0}{0} \f$ to indicate an overflow,
/// Any operation except assignment, constrion and comparison envolfing a `defective` will also return `degenrate`. This mirrors the behaviour 
/// of floating-point _quiet_ NaN (see IEEE754 Standard). However the are some difference, mnainly for performance and usability reasons:
///
/// - `defective == defgenerate`, but `NAN != NAN`
///
/// Formally speaking a return  of `defective` corresponds to not retuernimg anything since it does not belong to \f$ \mathbb{Q}^\ast_+ \f$
///
///\par Invariants
///
/// The invrainats are as follows
///
/// - `(num() == 0) == (den() == 0)` if the numeratoe is 0 then alse is the denominator and vice versa.
/// - `gcd(num(), den()) == not (num() && den())` , i.e. zero for `degenmerate` and `1` otherwise.
///
///\par Conversions
///
/// The class converts to `bool` in a bollean context. Similar to C++ streams a value of `false` corresppnds to the error condition, 
/// i.e. is is a way to test, whether the instance is `defective`. If the instance is not defective it converts to `true`.
///
/// In the current design the attributes are not individually mutable, but the whole instance is assignable. This helpps the mantain the
///  class invariants and this improces.
///
///\par Concepts
///
/// The class models the follwin concepts, some of which are modelled by C++ concepts.
///
/// - `std::regular`
/// - `constexpr`
/// - monolithic_mutable, i.e. attributes cannot be changed _separately_    
///
///\par Restrictions and Dependencies
///
/// - C++20 capable compiler and Standard Library
/// - `constexpr bool ct::uint64_mult_overflow(uint64_t a, uint64_t b, uint64_t &result)`
///
///\par Overflow
///  - methods functioms and operators tagged as (S) for safe will not return `defective()`, 0 or `false`.
///  - methods functioms and operators tagged as (P) for partial will not create overflow if the instance and the
///    opeartaions arguments or not `defective()`, 0 or `false`.
///  - methods functioms and operators tagged as (U) for unsafe will always return `defective()` on overflow
///  - methods functioms and operators tagged as (D) for defective will always return `defective()`, 0 or `false`.
class NQ{
    enum normalized_tag { is_normalized };     ///< as tag in internal constructors to skip normalization
public:
    using value_type = std::uint64_t;          ///< type used for numerator and denominator  
   
    enum class kind {
        defective,
        zero,
        valid
    };

    constexpr NQ();                            ///< create neutral element (zero) (D) 
    explicit constexpr NQ(value_type n);       ///< create element with 1 in the denominator \post no overflow (S) 
    constexpr NQ(value_type n, value_type d);  ///< constructor with ormalization (P) 
    
    constexpr value_type num() const;          ///< query numerator   (P) 
    constexpr value_type den() const;          ///< query denominator (P)

    static constexpr NQ zero();                ///< constant for neutral element (S)
    static constexpr NQ defective();           ///< constant for error element   (D)

    constexpr bool valid() const;              ///< query `*this != degemerate()` but faster
    constexpr explicit operator bool() const;  ///< check for `zero()` (P)

    constexpr NQ operator-() const;            ///< inverse element (P)
    constexpr NQ &operator+=(NQ const &rhs);    ///< ads rhs (O)
    constexpr NQ &operator-=(NQ const &rhs);   ///< add additive inverse of rhs (O)

    constexpr kind get_kind() const;           ///< query kind  of instance
    
private:
    constexpr NQ(value_type n, 
                 value_type d, 
                 normalized_tag);              ///< internal constructor skipping ormalization (P)

    constexpr void normalize();                ///< reduces fraction (P)
public: ///\note current Standard *requires* that constexpr classes used as non type template arguments must be 
        /// public. This is unfortunate for us, as it prohibts us to keep a guarantee on the normalization of
        /// attributes and IMHO a strange design decision in C++20 as private methods are allowed. Hopefully
        // this restriction will be lifted in a Standard bugfix or in C++23.
    uintmax_t num_; ///< numerator
    uintmax_t den_; ///< denominator

    friend constexpr NQ operator+(NQ lhs, NQ rhs){ return NQ(lhs) += rhs; } /// (O)
    friend constexpr NQ operator-(NQ lhs, NQ rhs){ return NQ(lhs) += rhs; } /// (O)

    friend constexpr bool operator==(NQ lhs, NQ rhs){ return lhs.num_ == rhs.num_ && lhs.den_ == rhs.den_; }
    friend constexpr bool operator!=(NQ lhs, NQ rhs){ return !(lhs == rhs); }
};

/// inline, constexpr and template code ////////////////////////////////////////////////////////////////////

constexpr NQ::NQ() : num_{1}, den_{1}{} 

constexpr NQ::NQ(value_type n) : NQ{n, 1, is_normalized} {} 

constexpr NQ::NQ(value_type n, value_type d) : num_{n}, den_{d}
{ 
    using std::gcd;

     // normalize
    if  (num_ && den_) 
    {
        auto const f = gcd(num_, den_);
        num_ /= f;
        den_ /= f;
    }
    else 
        *this = defective(); 
}

constexpr NQ::NQ(value_type n, 
                 value_type d, 
                 normalized_tag) : num_{n}, den_{d}
{ 
}

constexpr void NQ::normalize()
{
    using std::gcd;

     // normalize
    if  (num_ && den_) 
    {
        auto const f = gcd(num_, den_);
        num_ /= f;
        den_ /= f;
    }
    else 
        *this = defective(); 
}

constexpr NQ::value_type NQ::num() const{ 
    return num_;
}

constexpr NQ::value_type NQ::den() const
{
    return den_;
}

constexpr NQ::operator bool () const
{
    return *this != zero();
}

///\return instance
constexpr NQ &NQ::operator+=(NQ const &rhs){

    using std::gcd;

    // special cases (avod divion by zero)
    if (!(den_ && rhs.den_)) return *this = defective();

    auto const a = gcd(num_, rhs.den_);
    auto const b = gcd(den_, rhs.num_);

    num_ /= a;
    den_ /= b;
    auto const rn = rhs.num_ /  b;
    auto const rd = rhs.den_ /  a;

    ///\note `gcd` has many divisions, so not worth opimizing overflow detection
    bool const has_overflowed = umult_overflow(num_, rn, num_)
                             || umult_overflow(den_, rd, den_);
        
    if (has_overflowed) *this = defective();

    return *this;
}

/// subtracs negative of `rhs`: 
///\return instance
constexpr NQ &NQ::operator-=(NQ const &rhs){
    return *this += -rhs;
}

///\note neutral element \f$ \mathbf{1}\f$ in \f$ \mathbb_{Q}_\mathbb_^\ast_+ \f$, not neutral with respect to to \f$ \fract{0}{0} \f$ (`defective()`).
///\return neutral element `NQ{1,1}`
constexpr NQ NQ::zero()
{
    return NQ{};
} 

///\return explicitely create defective (i.e. error) element `NQ{0,0}`.
///\note This only other way to generate a defective element is by overflow in operator `+` or `-`
constexpr NQ NQ::defective() 
{
    return NQ{0, 0, is_normalized };
} 

///\invariant `a + -a = zero()`
///\return return inverse element
constexpr NQ NQ::operator-() const
{ 
    return NQ{den_, num_, is_normalized };
}

constexpr bool NQ::valid() const
{
    return den_;
}

constexpr NQ::kind NQ::get_kind() const {
    return !den_ ? kind::defective 
        : *this == zero()
          ? kind::zero 
          : kind::valid;
};


} // namespace ct