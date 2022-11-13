primordial - lightweight C++ compile time units and quantities
==============================================================

(C)opright Christian Kotz 2022

This small library has bee developed as a byproduct of a presenation at ITK CoP C++.
The aim of the presentation had been to show in a easy to understand real world example 
the usefulness of C++ template metaprogramming to improve code saftey by  compile time checks
under the title "Compile-time Physical UNITS. C++ template Meta Programming for saftey."

The library had been used to verify concepts shown at the presentation and been put onb Github 
after requestv by te audience.  It is a minimal functional demo, lacks many non existential 
features larger preexisting unit libraries provide (For a list see earlier efforts). However,
you are welcome to add these by prividing code.

For a full blown implementation of compile time physical units habve a look at the proposed
C++ Standard library addition fpe units (scheduled for C++ 2026, if accepted).

Aims
----

### In scope

- minimal, but useful for typical use caeses 
- fast at compile and runtime (see complexity below)

### Out of scope
- Scaled unit and scale conversions could be added by choosing a suitable scalar type
- instantiations with overflow checking scalars types could be used 
- casts 

Complexity Measurents
---------------------

Template Metaprogramming libraraies are known to be often very demandling on compile time ressources (memory, zime).
The complexity of meta functions etc. is measured in number of template class instantiations, because the symbol table 
needs to be updated, which envolves dynamic mermory atz compiloe time. Template function instatiations are even more
demanding because they produce definitions, not just declarations.

For a discussion and concrete benchmarks see the [Kwasir](https://github.com/kvasir-io/Kvasir). 

We state complexity in amortized number of template class of function instantiations using bi O notationb. 
Note that pure lookups (usings) do not create any template class or function instatiations and are thus rated as 
$$O(0)$$ .

Algorithms
----------

The library distinguishes itself from other C++ compile time unit libraries by an encoding of the unit powers into 
multiplicative group of reduced fractions of natural numbers  $$\left(\mathbb{Q}_{\mathbb{N}},\cdot\right)$$.
Each base unit is mapped to a unique prime number. Positive powers represented by the numerator, negative ones by 
the denomniator.


Earlier Effort
--------------

### Papers

* R. F. Cmelik and N. H. Gehani, “Dimensional Analysis with C++,” IEEE Softw., vol. 5, Art. no. 3, May 1988.
* Z. D. Umrigar, “Fully Static Dimensional Analysis with C++,” SIGPLAN Not., vol. 29, Art. no. 9, Sep. 1994.

### Impementations
* M. C. Schabel, “Boost.Units,” software 1.0, 2003.C++26 Proposal
* I. Shtukarev, “Boost Hana,” Boost C++ Libraries, software 1.71, 2015. (small example)
* J. M. Leon, “Units and measures for C++ 11,” software, May 2017.
* S. Watanabe, “cppnow17-units,” 2019.
* M. Kenniston and M. Moene, “PhysUnits-CT-Cpp11,” software, 2021.
* “The C++ Units Library,” WPILIb, software, Jan. 2022.
* N. Holthaus, “units,” software 2.3.3, 2022.
* M. Pusz and R. Smith, “MP-UNITS,” software, 2022.
* etc. (Ducharm Units, Sende Units , Amour Units, Leon Units,…)

### C++ Standard Proposal (C++26)
* M. Pusz, “A C++ Approach to Physical Units,” ISO/IEC JTC1/SC22/WG21 14882: Programming Language — C++, published proposal P1935R2, Jan. 2020.

Prerequisites
-------------

The library has been written for C++20 Standard. It employs 

* Concepts
* Composite non type template arguments
* `constexpr` functions

Howerver it could be straightforwardly backported to older versions of the standard by sybstituting two `uintmax_t` non type template parameters 
for the `NQ` class and expanding the `constexpr` functions.

License
-------

As the library as well as the presentation had been exclusively developed in private spare time I can make it available for free
under BSD license.

