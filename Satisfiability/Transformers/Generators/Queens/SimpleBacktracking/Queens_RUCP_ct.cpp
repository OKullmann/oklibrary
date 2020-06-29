// Oliver Kullmann 25.4.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

USAGE:

> ./Queens_RUCP_ct

uses the default N=16; use "make SETN=-DNUMQUEENS=X" for compilation to have
N=X. Or, more conveniently, use

> ./Call_QueensRUCPct N

for performing first the compilation, and then running the program.

(N+1)/2 parallel threads are used.

Via "-h" help-information is obtained, via "-v" version-information.

Classes Row, Row_uint implement one row of the board, while ExtRow,
ExtRow_uint implement the extensions for the diagonals. "uint" means
that the implementation uses unsigned integers, while otherwise std::bitset
is used.

Setting the typedefs R, ER (just before the main-function) regulates which
of the 2*2=4 possibilities is activated.

The core function is ucp(Board&), which performs complete row-unit-propagation,
while for columns exactly the inconsistencies are detected (i.e., empty
columns).

The recursion is handled by function count(Board).


TODOS:

-2. Replace std::is_pod_v by
      std::is_standard_layout_v and std::is_trivial_v
    since it's deprecated with C++20.
    - The question is where this should be placed.
      DONE: for now placing the macro is_pod(X) into central places.

-1. Currently compiling with C++20 in debug-mode yields strange
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


0. Write tests (in the usual dedicated testfile).
    - The statistics need to be precisely determined for small cases (at least
      up to N=5) by hand.
    - Also the behaviour of UCP needs to be exactly specified and tested.
    - Start test-program. DONE

1. Improved output:
    - The version-information should contain N and information on which of
      the 2*2 configurations is used.
    - So the Environment-function for showing version-information needs
      to become customisable; be a function-object, which prints additional
      information.
    - The configuration should also show up in the output.
    - There should be enumerated constants, as global variables,
      and according to their values, the aliases R, ER are defined
      (via partial specialisation).
    - These constants are placed after definition of N.

2. Eliminate the recursion in function count(Board<R>& B):
    - One big loop, with a two-dimensional movement, "vertically" (recursive
      call or backtracking), and "horizontally" (within the current row).
    - An array of size N contains the state of the current board. which is
      a triple (start-board, current state of row, board after ucp) concerning
      the board.
    - The current statistics is the fourth data-member of those stack-
      elements.

3. Can the two sweeps of ucp be unified (nicely)?

4. Move general definitions to header-files:
    - Perhaps Rows.hpp, Board.hpp, Backtracking.hpp.
    - Which namespace?

5. Document the various concepts (rows, extended rows, boards).

6. After 1-5, version 1.0 is reached.

7. Consolidate functions for bit-operations with integers:
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

*/

#include <bitset>
#include <iostream>
#include <string>
#include <array>
#include <future>
#include <vector>
#if __cplusplus > 201703L
# include <bit>
#endif


#include <ProgramOptions/Environment.hpp>

#include "../GenericSAT/Recursion.hpp"

#include <cstdlib>
#include <cassert>
#include <cstdint>

namespace {

const Environment::ProgramInfo proginfo{
      "0.9.0",
      "13.6.2020",
      __FILE__,
      "Oliver Kullmann",
      "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/Queens_RUCP_ct.cpp",
      "GPL v3"};

typedef std::size_t size_t;
#ifndef NUMQUEENS
constexpr size_t N=16;
#else
constexpr size_t N=NUMQUEENS;
#endif
static_assert(N >= 1);


bool show_usage(const int argc, const char* const argv[]) {
  assert(argc >= 1);
  if (not Environment::help_header(std::cout, argc, argv, proginfo))
    return false;
  std::cout <<
    "> " << proginfo.prg << "\n"
    " runs the program for built-in N = " << N << ", outputting the line\n"
    "   N solution_count node_count\n"
    "\n"
    "> ./Call_QueensRUCPct N\n"
    " compiles optimised and debugging forms of this program, and runs\n"
    " the optimised version, with added user-time (s) and max-memory (kb).\n"
;
  return true;
}


enum class RS { empty=0, unit=1, other=2 }; // "row-state"

template <class> class Board;

class Row {
  typedef std::bitset<N> row_t; // "true" means forbidden or occupied
  row_t r;
  Row(row_t r) noexcept : r(r) {}
  // Assumes r = 0:
  void set(size_t i) noexcept { r.set(i); }
  friend class Board<Row>;

  // Iterating through the positions i with not x[i], dereferencing to
  // the bitset with exactly one 1, at position i:
  class IteratorRow {
    row_t x;
    size_t i;
  public :
    IteratorRow() noexcept : i(N) {}
    IteratorRow(const row_t x) noexcept : x(x) {
      if (not x[0]) i = 0;
      else for (i=1; i < N and x[i]; ++i);
    }
    void operator ++() noexcept {
      assert(i < N and not x[i]);
      while (++i < N and x[i]);
    }
    Row operator *() const noexcept {
      assert(i < N);
      return 1ull << i;
    }
    bool operator !=(const IteratorRow& rhs) { return i != rhs.i; }
  };

public :
  Row() = default;
  Row(const unsigned long long u) : r(u) {}
  unsigned long long to_ullong() const noexcept { return r.to_ullong(); }

  bool none() const noexcept { return r.none(); }
  bool any() const noexcept { return r.any(); }
  bool all() const noexcept { return r.all(); }
  RS rs() const noexcept {
    if (r.all()) return RS::empty;
    else if (r.count() == N-1) return RS::unit;
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
    for (size_t i = 0; i < N; ++i) out << r.r[i];
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
inline constexpr UINT invalid_bits(const size_t i) {
  return ~((UINT(1) << i) - UINT(1));
}
static_assert(invalid_bits<std::uint8_t>(0) == 0xFF);
static_assert(invalid_bits<std::uint8_t>(2) == 0xFC);
static_assert(invalid_bits<std::uint8_t>(8) == 0);

class Row_uint {
  typedef std::uint64_t row_t; // using the first N bits
  static const row_t all_set = row_t(-1);
  // the other bits set to 1 (an invariant):
  static constexpr row_t mask = invalid_bits<row_t>(N);
  row_t r;
  Row_uint(row_t r) noexcept : r(r | mask) {}
  void set(size_t i) noexcept { r = (row_t(1) << i) | mask; }
  friend class Board<Row_uint>;

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
    return std::bitset<N>(r&~mask).count();
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
    const auto b = std::bitset<N>(r.r);
    for (size_t i = 0; i < N; ++i) out << b[i];
    return out;
  }

};


template <class R>
class ExtRow {
  typedef std::bitset<3*N-2> extrow_t;
  static_assert(N <= 32); // so that to_ullong suffices
  extrow_t b;
public :
  ExtRow(const R& r) noexcept : b(r.to_ullong() << (N-1)) {}
  operator R() const noexcept {
    return ((b << (N-1)) >> 2*(N-1)).to_ullong();
  }
  void add(const R& r) noexcept { b |= ExtRow(r).b; }
  void left() noexcept { b <<= 1; }
  void right() noexcept { b >>= 1; }
};
template <class R>
class ExtRow_uint {
  typedef std::uint64_t extrow_t;
  static_assert(N <= 22); // so that 3N-2 <= 64
  extrow_t b;
public :
  ExtRow_uint(const R& r) noexcept : b(r.to_ullong() << (N-1)) {}
  operator R() const noexcept {
    return (unsigned long long)((b << (N-1)) >> 2*(N-1));
  }
  void add(const R& r) noexcept { b |= ExtRow_uint(r).b; }
  void left() noexcept { b <<= 1; }
  void right() noexcept { b >>= 1; }
};


typedef std::uint_fast64_t count_t;
class Statistics {
  count_t sols, // solutions
          nds,  // nodes
          ucs,  // unit-clauses
          r2s,  // r2-satisfiability
          r2u,  // r2-unsatisfiability
          cu,   // column-unsatisfiability
          duplications; // how often solutions are multiplied
public :
  constexpr Statistics(const bool root = false) noexcept : sols(0), nds(root), ucs(0), r2s(0), r2u(0), cu(0), duplications(1) {}
  count_t num_sols() const noexcept { return sols; }
  void found_uc() noexcept { ++ucs; }
  void found_r2s() noexcept { ++sols; ++r2s; }
  void found_r2u() noexcept { ++r2u; }
  void found_cu() noexcept { ++cu; }
  void add_duplication() noexcept { ++duplications; }
  Statistics& operator +=(const Statistics& s) noexcept {
    sols += s.duplications * s.sols;
    nds+=s.nds; ucs+=s.ucs; r2s+=s.r2s; r2u+=s.r2u; cu+=s.cu;
    return *this;
  }
  friend std::ostream& operator <<(std::ostream& out, const Statistics& s) {
    return out << s.duplications * s.sols << " " << s.nds << " " << s.ucs
      << " " << s.r2s << " " << s.r2u << " " << s.cu;
  }
};


template <class R>
struct Board {
private :
  typedef std::array<R,N> board_t;
  board_t b;
  size_t i; // current bottom-row, i <= N
  size_t cbi() const noexcept { return i; }
  void inc() noexcept { ++i; }
  R closed_columns;
  bool falsified_;
  // If not falsified, then the board is amo+alo-consistent, assuming that
  // all-0-rows mean rows with placed queen.
  void set_falsified() noexcept { falsified_ = true; }

public :

  Board(const size_t i) noexcept : i(0), falsified_(false) {
    for (size_t j = 0; j < N; ++j) b[j].set(i);
    closed_columns.set(i);
  }

  const R& cbr() const noexcept { return b[i]; }
  void set_cbr(R r) noexcept { b[i] = r; closed_columns |= r; }

  bool falsified() const noexcept { return falsified_; }
  bool satisfied() const noexcept { return not falsified_ and i >= N-1; }

  friend std::ostream& operator <<(std::ostream& out, const Board& B) {
    for (size_t i = N; i != 0; --i) out << B.b[i-1] << "\n";
    out << "i=" << B.i << ", falsified=" << B.falsified() << "\n";
    return out << "closed_columns=" << B.closed_columns << "\n";
  }

  // Propagate the single queen which is set in the current bottom-row:
  template <template <class> class ExtR>
  void ucp(Statistics& s) noexcept {
    if (N == 1) {s.found_r2s(); return;}
    typedef ExtR<R> ER;
    assert(not falsified());
    assert(not satisfied());
    assert(closed_columns.count() >= cbi());
    R units = cbr(), old_units;
    inc();
    ER diag(units), antidiag = diag;
    R open_columns;
    do {
      // Up-sweep:
      old_units = units;
      open_columns.set();
      for (size_t j = cbi(); j != N; ++j) {
        diag.left(); antidiag.right();
        R& curr(b[j]);
        if (curr.none()) continue;
        assert(curr.rs() != RS::empty);
        curr |= units | diag | antidiag;
        switch (curr.rs()) {
        case RS::empty : s.found_r2u(); falsified_ = true; return;
        case RS::unit : { s.found_uc();
          const R new_unit = ~curr; curr.reset();
          units |= new_unit; diag.add(new_unit); antidiag.add(new_unit);
          break; }
        default : open_columns &= curr; }
      }
      closed_columns |= units;
      if ((~closed_columns & open_columns).any()) {
        s.found_cu(); falsified_ = true; return;
      }
      if (units == old_units) break;

      // Down-sweep:
      old_units = units;
      if (b[N-1].none()) open_columns.set();
      else open_columns = b[N-1];
      for (size_t j = N-2; j != cbi()-1; --j) {
        diag.right(); antidiag.left();
        R& curr(b[j]);
        if (curr.none()) continue;
        assert(curr.rs() != RS::empty);
        curr |= units | diag | antidiag;
        switch (curr.rs()) {
        case RS::empty : s.found_r2u(); falsified_ = true; return;
        case RS::unit : { s.found_uc();
          const R new_unit = ~curr; curr.reset();
          units |= new_unit; diag.add(new_unit); antidiag.add(new_unit);
          break; }
        default : open_columns &= curr; }
      }
      closed_columns |= units;
      if ((~closed_columns & open_columns).any()) {
        s.found_cu(); falsified_ = true; return;
      }
      diag.right(); antidiag.left();
    } while (units != old_units);

    while (cbi() < N and cbr().none()) inc();
    if (cbi() == N) {s.found_r2s(); return;}
    assert(cbi() < N-1);
  }

};


template <class R, template <class> class ER>
Statistics count(const Board<R>& B) noexcept {
  Statistics res(true);
  for (const R new_row : B.cbr()) {
    Board<R> Bj(B);
    Bj.set_cbr(new_row);
    Bj.template ucp<ER>(res);
    if (not Bj.satisfied() and not Bj.falsified()) res += count<R,ER>(Bj);
  }
  return res;
}


  typedef Row_uint R;
  template<class X> using ER = ExtRow_uint<X>;

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  std::vector<std::future<Statistics>> jobs;
  std::vector<Statistics> results;
  Statistics res(true);

  for (size_t i = 0; i < N; ++i) {
    Board<R> B(i);
    B.ucp<ER>(res);
    if (not B.satisfied() and not B.falsified()) {
      if (i < (N+1)/2) {
        jobs.push_back(std::async(std::launch::async, count<R, ER>, B));
        results.push_back({});
      }
      else
        results[i - (N+1)/2].add_duplication();
    }
  }
  assert(jobs.size() == results.size());
  for (size_t i = 0; i < jobs.size(); ++i) results[i] += jobs[i].get();
  for (const auto& r : results) res += r;

  if (res.num_sols() != Recursion::exact_value(N)) {
    std::cerr << "\nERROR[" << proginfo.prg << "]: The statistics are\n"
              << N << " " << res << "\nbut the correct count for N=" << N
              << " is " << Recursion::exact_value(N) << ".\n\n";
    return 1;
  }
  std::cout << N << " " << res << "\n";
}
