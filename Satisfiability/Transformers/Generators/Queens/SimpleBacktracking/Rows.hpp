// Oliver Kullmann, 29.6.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Data structures representing rows for the 2-sweep algorithm


TODOS:

BASIC

EXTENSIONS

1. Consolidate functions for bit-operations with integers:
    - We have functions in
      - this file Queens_RUCP_Ct.cpp
      - NQueens.hpp
      - Numerics/FloatingPoint.hpp
      - Generators/Random/Numbers.hpp.
    - Perhaps it is time to unify these services.
    - With C++20 there is the new library <bit>, which provides basic
      functionality implemented in the above files.
    - For now we need to employ compile-time switches to distinguish between
      C++17 and C++20. And using new targets "...ct20..." in Makefile.
    - Function firstzero(x): alternative is
#if __cplusplus > 201703L
   assert(x != UINT(-1));
   return UINT(1) << std::countr_one(x);
#else
  const UINT y = x+1; return (y ^ x) & y;
#endif
      (implemented now).
      On csltok this yields a speed-up.
      The semantics is different (undefined for x=all-bits-set.
    - For function amo_zero(x) the alternative would be
#if __cplusplus > 201703L
   assert(x != UINT(-1));
   return std::has_single_bit(UINT(~x));
#else
  return ((x+1) | x) == UINT(-1);
#endif
      which leads to a slowdown on csltok; due to the negation?
      Or is this different on other machines.
      Again, the semantics changed.


23  Currently compiling with C++20 in debug-mode yields strange
    compilation-errors related to std::bitset and operator ==. In class Row
    we have
friend bool operator ==(const Row& lhs, const Row& rhs) noexcept {
  return lhs.r == rhs.r;
}
      and here gcc in debug-mode has problems with == :

In file included from /usr/local/lib64/gcc/x86_64-pc-linux-gnu/10.1.0/include/c++/bitset:1595,
                 from Queens_RUCP_ct.cpp:100:
/usr/local/lib64/gcc/x86_64-pc-linux-gnu/10.1.0/include/c++/debug/bitset: In instantiation of ‘bool std::__debug::bitset<_Nb>::operator==(const std::__debug::bitset<_Nb>&) const [with long unsigned int _Nb = 16]’:
Queens_RUCP_ct.cpp:222:25:   required from here
/usr/local/lib64/gcc/x86_64-pc-linux-gnu/10.1.0/include/c++/debug/bitset:356:26: error: ambiguous overload for ‘operator==’ (operand types are ‘const _Base’ {aka ‘const std::__cxx1998::bitset<16>’} and ‘const std::__debug::bitset<16>’)
  356 |       { return _M_base() == __rhs; }
      |                ~~~~~~~~~~^~~~~~~~
/usr/local/lib64/gcc/x86_64-pc-linux-gnu/10.1.0/include/c++/debug/bitset:355:7: note: candidate: ‘bool std::__debug::bitset<_Nb>::operator==(const std::__debug::bitset<_Nb>&) const [with long unsigned int _Nb = 16]’ (reversed)
  355 |       operator==(const bitset<_Nb>& __rhs) const _GLIBCXX_NOEXCEPT
      |       ^~~~~~~~
In file included from Queens_RUCP_ct.cpp:100:
/usr/local/lib64/gcc/x86_64-pc-linux-gnu/10.1.0/include/c++/bitset:1306:7: note: candidate: ‘bool std::__cxx1998::bitset<_Nb>::operator==(const std::__cxx1998::bitset<_Nb>&) const [with long unsigned int _Nb = 16]’
 1306 |       operator==(const bitset<_Nb>& __rhs) const _GLIBCXX_NOEXCEPT

      Non-debug mode works.
      One would assume that this is a compiler-bug, where we hope that with
      gcc 10.2 the problem disappears.
      Until then the Makefile only creates the non-debug version.

*/

#ifndef ROWS_xfC4q3a6kv
#define ROWS_xfC4q3a6kv

#include <bitset>
#if __cplusplus > 201703L
# include <bit>
#endif

#include <ostream>
#include <type_traits>

#include <cassert>
#include <cstdint>

#include "Dimensions.hpp"

namespace Rows {

  namespace D = Dimensions;

  enum class RS { empty=0, unit=1, other=2 }; // "row-state"

  // Returns 2^i, with i the position of the first 0 of x (0 iff there is
  // no 0):
  template <typename UINT>
  inline constexpr UINT firstzero(const UINT x) noexcept {
#if __cplusplus > 201703L
     assert(x != UINT(-1));
     return UINT(1) << std::countr_one(x);
#else
    const UINT y = x+1; return (y ^ x) & y;
#endif
  }
  static_assert(firstzero(0ull) == 1);
  static_assert(firstzero(1ull) == 2);
  static_assert(firstzero(2u) == 1);
  static_assert(firstzero(3ul) == 4);
  static_assert(firstzero(4u) == 1);
  static_assert(firstzero(0xFull) == 0x10ull);
#if __cplusplus <= 201703L
  static_assert(firstzero((unsigned long long) -1) == 0);
#endif

  // At-most-one zero:
  template <typename UINT>
  inline constexpr bool amo_zero(const UINT x) noexcept {
    return ((x+1) | x) == UINT(-1);
  }
  static_assert(not amo_zero(0ull));
  static_assert(amo_zero(std::uint8_t(0xFEu)));
  static_assert(amo_zero(std::uint8_t(0xFD)));
  static_assert(not amo_zero(std::uint8_t(0xFC)));
  static_assert(amo_zero((unsigned long long)(-1) - 1ull));
#if __cplusplus <= 201703L
  static_assert(amo_zero((unsigned long long) -1));
#endif

  template <typename UINT>
  inline constexpr UINT invalid_bits(const D::sizet i) {
    return ~((UINT(1) << i) - UINT(1));
  }
  static_assert(invalid_bits<std::uint8_t>(0) == 0xFF);
  static_assert(invalid_bits<std::uint8_t>(2) == 0xFC);
  static_assert(invalid_bits<std::uint8_t>(8) == 0);


  class Row {
    typedef Dimensions::row_t row_t;
    static const row_t all_set = row_t(-1);
    // the other bits set to 1 (an invariant):
    static constexpr row_t mask = invalid_bits<row_t>(D::N);
    row_t r;

    class Iterator {
      row_t rem;
      row_t val;
    public :
      Iterator() noexcept : val(0) {}
      Iterator(const row_t x) noexcept : rem(x), val(firstzero(x)) {
        assert((rem & mask) == mask);
      }
      void operator ++() noexcept {
        rem |= val; val = firstzero(rem);
      }
      Row operator *() const noexcept { return val; }
      bool operator !=(const Iterator& rhs) { return val != rhs.val; }
    };

  public :
    static constexpr bool valid = D::N <= std::numeric_limits<row_t>::digits;
    static_assert(valid);

    Row() = default;
    constexpr Row(const row_t u) : r(u | mask) {}
    constexpr Row(const D::sizet i, bool) noexcept : r((row_t(1) << i) | mask) {}

    constexpr row_t extract() const noexcept { return r & ~mask; }

    constexpr bool none() const noexcept { return r == mask; }
    constexpr bool any() const noexcept { return r != mask; }
    constexpr bool all() const noexcept { return r == all_set; }
    constexpr RS rs() const noexcept {
      if (r == all_set) return RS::empty;
      else if (amo_zero(r)) return RS::unit;
      else return RS::other;
    }
#ifndef NDEBUG
    unsigned long long count() const noexcept {
      return std::bitset<D::N>(r&~mask).count();
    }
#endif
    void reset() noexcept { r = mask; }
    void set() noexcept { r = all_set; }
    void operator |= (const Row rhs) noexcept { r |= rhs.r; }
    void operator &= (const Row rhs) noexcept { r &= rhs.r; }

    friend Row operator | (const Row lhs, const Row rhs) noexcept {
      return lhs.r | rhs.r;
    }
    friend Row operator & (const Row lhs, const Row rhs) noexcept {
      return lhs.r & rhs.r;
    }
    friend Row operator ~(const Row r) noexcept {return ~r.r;}

    typedef Iterator iterator;
    Iterator begin() const noexcept { return r; }
    Iterator end() const noexcept { return {}; }

    friend bool operator ==(const Row lhs, const Row rhs) noexcept {
      return lhs.r == rhs.r;
    }
    friend bool operator !=(const Row lhs, const Row rhs) noexcept {
      return lhs.r != rhs.r;
    }

    friend std::ostream& operator <<(std::ostream& out, const Row r) {
      const auto b = std::bitset<D::N>(r.r);
      for (D::sizet i = 0; i < D::N; ++i) out << b[i];
      return out;
    }

  };

  static_assert(std::is_trivial_v<Row>);

}

#endif
