// Oliver Kullmann, 29.6.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Data structures representing rows for the 2-sweep algorithm


TODOS:

1.  Currently compiling with C++20 in debug-mode yields strange
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

#include <cassert>
#include <cstdint>

#include "Dimensions.hpp"

namespace Rows {

  namespace D = Dimensions;

  enum class RS { empty=0, unit=1, other=2 }; // "row-state"


  class Row {
    typedef std::bitset<D::N> row_t; // "true" means forbidden or occupied
    row_t r;
    Row(row_t r) noexcept : r(r) {}

    // Iterating through the positions i with not x[i], dereferencing to
    // the bitset with exactly one 1, at position i:
    class IteratorRow {
      row_t x;
      D::size_t i;
    public :
      IteratorRow() noexcept : i(D::N) {}
      IteratorRow(const row_t x) noexcept : x(x) {
        if (not x[0]) i = 0;
        else for (i=1; i < D::N and x[i]; ++i);
      }
      void operator ++() noexcept {
        assert(i < D::N and not x[i]);
        while (++i < D::N and x[i]);
      }
      Row operator *() const noexcept {
        assert(i < D::N);
        return 1ull << i;
      }
      bool operator !=(const IteratorRow& rhs) { return i != rhs.i; }
    };

  public :
    Row() = default;
    Row(const unsigned long long u) : r(u) {}
    Row(const D::size_t i, bool) { r.set(i); }

    unsigned long long to_ullong() const noexcept { return r.to_ullong(); }

    bool none() const noexcept { return r.none(); }
    bool any() const noexcept { return r.any(); }
    bool all() const noexcept { return r.all(); }
    RS rs() const noexcept {
      if (r.all()) return RS::empty;
      else if (r.count() == D::N-1) return RS::unit;
      else return RS::other;
    }
#ifndef NDEBUG
    unsigned long long count() const noexcept { return r.count(); }
#endif
    void reset() noexcept { r.reset(); }
    void set() noexcept { r.set(); }
    void operator |= (const Row& rhs) noexcept { r |= rhs.r; }
    void operator &= (const Row& rhs) noexcept { r &= rhs.r; }

    friend Row operator | (const Row& lhs, const Row& rhs) noexcept {
      return lhs.r | rhs.r;
    }
    friend Row operator & (const Row& lhs, const Row& rhs) noexcept {
      return lhs.r & rhs.r;
    }
    friend Row operator ~(const Row& r) noexcept { return ~ r.r; }

    IteratorRow begin() const noexcept { return r; }
    IteratorRow end() const noexcept { return {}; }

    friend bool operator ==(const Row& lhs, const Row& rhs) noexcept {
      return lhs.r == rhs.r;
    }
    friend bool operator !=(const Row& lhs, const Row& rhs) noexcept {
      return lhs.r != rhs.r;
    }

    friend std::ostream& operator <<(std::ostream& out, const Row& r) {
      for (D::size_t i = 0; i < D::N; ++i) out << r.r[i];
      return out;
    }
  };


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
  inline constexpr UINT invalid_bits(const D::size_t i) {
    return ~((UINT(1) << i) - UINT(1));
  }
  static_assert(invalid_bits<std::uint8_t>(0) == 0xFF);
  static_assert(invalid_bits<std::uint8_t>(2) == 0xFC);
  static_assert(invalid_bits<std::uint8_t>(8) == 0);


  class Row_uint {
    typedef std::uint64_t row_t; // using the first N bits
    static const row_t all_set = row_t(-1);
    // the other bits set to 1 (an invariant):
    static constexpr row_t mask = invalid_bits<row_t>(D::N);
    row_t r;
    Row_uint(row_t r) noexcept : r(r | mask) {}

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
      Row_uint operator *() const noexcept { return val; }
      bool operator !=(const Iterator& rhs) { return val != rhs.val; }
    };

  public :
    Row_uint() = default;
    Row_uint(const unsigned long long u) : r(u | mask) {}
    Row_uint(const D::size_t i, bool) noexcept : r((row_t(1) << i) | mask) {}

    unsigned long long to_ullong() const noexcept { return r & ~mask; }

    bool none() const noexcept { return r == mask; }
    bool any() const noexcept { return r != mask; }
    bool all() const noexcept { return r == all_set; }
    RS rs() const noexcept {
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
    void operator |= (const Row_uint& rhs) noexcept { r |= rhs.r; }
    void operator &= (const Row_uint& rhs) noexcept { r &= rhs.r; }

    friend Row_uint operator | (const Row_uint& lhs, const Row_uint& rhs) noexcept {
      return lhs.r | rhs.r;
    }
    friend Row_uint operator & (const Row_uint& lhs, const Row_uint& rhs) noexcept {
      return lhs.r & rhs.r;
    }
    friend Row_uint operator ~(const Row_uint& r) noexcept {return ~r.r | mask;}

    Iterator begin() const noexcept { return r; }
    Iterator end() const noexcept { return {}; }

    friend bool operator ==(const Row_uint& lhs, const Row_uint& rhs) noexcept {
      return lhs.r == rhs.r;
    }
    friend bool operator !=(const Row_uint& lhs, const Row_uint& rhs) noexcept {
      return lhs.r != rhs.r;
    }

    friend std::ostream& operator <<(std::ostream& out, const Row_uint& r) {
      const auto b = std::bitset<D::N>(r.r);
      for (D::size_t i = 0; i < D::N; ++i) out << b[i];
      return out;
    }

  };

}

#endif
