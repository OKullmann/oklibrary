// Oliver Kullmann, 1.7.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Data structures representing extended rows for the 2-sweep algorithm


TODOS :

BASIC

1. It seems better to distinguish between extensions for diagonals and for
   antidiagonals, since then only 2N-1 bits are needed.

EXTENSIONS

*/

#ifndef EXTROWS_Q9FdphioR5
#define EXTROWS_Q9FdphioR5

#include <bitset>
#include <ostream>

#include <cstdint>

#include "Dimensions.hpp"

namespace ExtRows {

  namespace D = Dimensions;


  template <class R>
  class ExtRow {
    typedef std::bitset<3*D::N-2> extrow_t;
    extrow_t b;
  public :
    static constexpr bool valid =
      2*D::N <= std::numeric_limits<unsigned long long>::digits;
    ExtRow(const R& r) noexcept : b(r.to_ullong() << (D::N-1)) {}
    operator R() const noexcept {
      return ((b << (D::N-1)) >> 2*(D::N-1)).to_ullong();
    }
    void add(const R& r) noexcept { b |= ExtRow(r).b; }
    void left() noexcept { b <<= 1; }
    void right() noexcept { b >>= 1; }
  };


  template <class R>
  class ExtRow_uint {
    typedef std::uint64_t extrow_t;
    static_assert(std::is_integral_v<extrow_t> and std::is_unsigned_v<extrow_t>);
    extrow_t b;
  public :
    static constexpr bool valid =
      (3 * D::N - 2 <= std::numeric_limits<extrow_t>::digits) and
      (D::N <= std::numeric_limits<unsigned long long>::digits);
    ExtRow_uint(const R& r) noexcept : b(r.to_ullong() << (D::N-1)) {}
    operator R() const noexcept {
      return (unsigned long long)((b << (D::N-1)) >> 2*(D::N-1));
    }
    void add(const R& r) noexcept { b |= ExtRow_uint(r).b; }
    void left() noexcept { b <<= 1; }
    void right() noexcept { b >>= 1; }
  };


  template <D::ERtypes, class> struct ChoiceERT_;
  template <class X> struct ChoiceERT_<D::ERtypes::bitset, X> {
    using type = ExtRow<X>;
  };
  template <class X> struct ChoiceERT_<D::ERtypes::uint, X> {
    using type = ExtRow_uint<X>;
  };
  template <D::ERtypes ert, class X>
  using ChoiceERT = typename ChoiceERT_<ert,X>::type;



  // Diagonal/antidiagonal-type:
  enum class DT { diagonal=0, antidiagonal=1 };
  template <DT, class> class DARow;
  template <DT, class> class DARow_uint;

  template <class R> class DARow<DT::diagonal, R> {
    typedef std::bitset<2*D::N-1> extrow_t;
    extrow_t b;
    static constexpr unsigned long long mask = (1ull << D::N) - 1;
  public :
    static constexpr bool valid =
      2*D::N <= std::numeric_limits<unsigned long long>::digits;
    constexpr DARow(const R& r) noexcept : b(r.to_ullong()) {}
    constexpr operator R() const noexcept { return b.to_ullong() & mask; }
    void add(const R& r) noexcept { b |= ExtRow(r).b; }
    void up() noexcept { b <<= 1; }
    void down() noexcept { b >>= 1; }
  };
  template <class R> class DARow<DT::antidiagonal, R> {
    typedef std::bitset<2*D::N-1> extrow_t;
    extrow_t b;
    static constexpr extrow_t set_high_N() noexcept {
      extrow_t res;
      for (D::size_t i = 0; i < D::N; ++i) res.set(2*D::N-2-i);
      return res;
    }
    static constexpr extrow_t mask = set_high_N();
  public :
    static constexpr bool valid =
      2*D::N <= std::numeric_limits<unsigned long long>::digits;
    constexpr DARow(const R& r) noexcept : b(r.to_ullong() << (D::N-1)) {}
    constexpr operator R() const noexcept { return (b & mask).to_ullong(); }
    void add(const R& r) noexcept { b |= ExtRow(r).b; }
    void up() noexcept { b >>= 1; }
    void down() noexcept { b <<= 1; }
  };

  template <class R> class DARow_uint<DT::diagonal, R> {
    typedef std::uint64_t extrow_t;
    static_assert(std::is_integral_v<extrow_t> and std::is_unsigned_v<extrow_t>);
    extrow_t b;
    static constexpr extrow_t mask = (1ull << D::N) - 1;
  public :
    static constexpr bool valid =
      (2 * D::N - 1 <= std::numeric_limits<extrow_t>::digits) and
      (D::N <= std::numeric_limits<unsigned long long>::digits);
    constexpr DARow_uint(const R& r) noexcept : b(r.to_ullong()) {}
    constexpr operator R() const noexcept {
      return (unsigned long long)(b & mask);
    }
    void add(const R& r) noexcept { b |= DARow_uint(r).b; }
    void up() noexcept { b <<= 1; }
    void down() noexcept { b >>= 1; }
  };
  template <class R> class DARow_uint<DT::antidiagonal, R> {
    typedef std::uint64_t extrow_t;
    static_assert(std::is_integral_v<extrow_t> and std::is_unsigned_v<extrow_t>);
    extrow_t b;
    static constexpr extrow_t mask = ((1ull << D::N) - 1) << (std::numeric_limits<extrow_t>::digits - D::N);
  public :
    static constexpr bool valid =
      (2 * D::N - 1 <= std::numeric_limits<extrow_t>::digits) and
      (D::N <= std::numeric_limits<unsigned long long>::digits);
    constexpr DARow_uint(const R& r) noexcept : b(r.to_ullong() << (D::N-1)) {}
    constexpr operator R() const noexcept {
      return (unsigned long long)(b & mask);
    }
    void add(const R& r) noexcept { b |= DARow_uint(r).b; }
    void up() noexcept { b >>= 1; }
    void down() noexcept { b <<= 1; }
  };

}

#endif
