// Oliver Kullmann 25.4.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

USAGE:

> ./Queens_RUCP_ct

uses the default N=16; use "make SETN=-DNN=X" for compilation to have N=X.
Or, more conveniently, use

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

*/

#include <bitset>
#include <iostream>
#include <string>
#include <array>
#include <future>
#include <vector>

#include <ProgramOptions/Environment.hpp>

#include <cstdlib>
#include <cassert>
#include <cstdint>

namespace {

const Environment::ProgramInfo proginfo{
      "0.7.0",
      "27.4.2020",
      __FILE__,
      "Oliver Kullmann",
      "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/Queens_RUCP_ct.cpp",
      "GPL v3"};

typedef std::size_t size_t;
#ifndef NN
constexpr size_t N=16;
#else
constexpr size_t N=NN;
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

  friend std::ostream& operator <<(std::ostream& out, const Row& r) {
    for (size_t i = 0; i < N; ++i) out << r.r[i];
    return out;
  }
};

// Returns 2^i, with i the position of the first 0 of x (0 iff there is
// no 0):
template <typename UINT>
inline constexpr UINT firstzero(const UINT x) noexcept {
  const UINT y = x+1; return (y ^ x) & y;
}
static_assert(firstzero(0ull) == 1);
static_assert(firstzero(1ull) == 2);
static_assert(firstzero(2u) == 1);
static_assert(firstzero(3ul) == 4);
static_assert(firstzero(4u) == 1);
static_assert(firstzero(0xFull) == 0x10ull);
static_assert(firstzero((unsigned long long) -1) == 0);

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

  friend std::ostream& operator <<(std::ostream& out, const Row_uint& r) {
    const auto b = std::bitset<N>(r.r);
    for (size_t i = 0; i < N; ++i) out << b[i];
    return out;
  }

};


template <class R>
struct Board {
private :
  bool falsified_;
  typedef std::array<R,N> board_t;
public :
  board_t b;
  size_t i; // current bottom-row, i <= N
  R closed_columns;
  // If not falsified, then the board is amo+alo-consistent, assuming that
  // all-0-rows mean rows with placed queen.

  Board(const size_t i) noexcept : falsified_(false), i(0) {
    for (size_t j = 0; j < N; ++j) b[j].set(i);
    closed_columns.set(i);
  }

  void set_falsified() noexcept { falsified_ = true; }
  bool falsified() const noexcept { return falsified_; }
  bool satisfied() const noexcept { return not falsified_ and i >= N-1; }

  friend std::ostream& operator <<(std::ostream& out, const Board& B) {
    for (size_t i = N; i != 0; --i) out << B.b[i-1] << "\n";
    out << "i=" << B.i << ", falsified=" << B.falsified() << "\n";
    return out << "closed_columns=" << B.closed_columns << "\n";
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


// Propagate the single queen which is set in the current bottom-row:
template <class R, template <class> class ExtR>
inline void ucp(Board<R>& B) noexcept {
  if (N == 1) return;
  typedef ExtR<R> ER;
  assert(not B.falsified());
  assert(not B.satisfied());
  assert(B.closed_columns.count() >= B.i);
  R units = B.b[B.i];
  ++B.i;
  ER diag(units), antidiag = diag;
  bool found;
  R open_columns;
  do {
    // Up-sweep:
    found = false;
    open_columns.set();
    for (size_t j = B.i; j != N; ++j) {
      diag.left(); antidiag.right();
      if (B.b[j].none()) continue;
      assert(B.b[j].rs() != RS::empty);
      const R new_row = B.b[j] | units | R(diag) | R(antidiag);
      switch (new_row.rs()) {
      case RS::empty : B.set_falsified(); return;
      case RS::unit : {const R new_unit = ~ new_row;
        units |= new_unit; B.closed_columns |= new_unit;
        diag.add(new_unit); antidiag.add(new_unit);
        B.b[j].reset(); found = true; break;}
      default : B.b[j] = new_row; open_columns &= new_row; }
    }
    if ((~B.closed_columns & open_columns).any()) {
      B.set_falsified(); return;
    }
    if (not found) break;

    // Down-sweep:
    found = false;
    if (B.b[N-1].none()) open_columns.set();
    else open_columns = B.b[N-1];
    for (size_t j0 = N-1; j0 != B.i; --j0) {
      const size_t j = j0-1;
      diag.right(); antidiag.left();
      if (B.b[j].none()) continue;
      assert(B.b[j].rs() != RS::empty);
      const R new_row = B.b[j] | units | R(diag) | R(antidiag);
      switch (new_row.rs()) {
      case RS::empty : B.set_falsified(); return;
      case RS::unit : {const R new_unit = ~ new_row;
        units |= new_unit; B.closed_columns |= new_unit;
        diag.add(new_unit); antidiag.add(new_unit);
        B.b[j].reset(); found = true; break;}
      default : B.b[j] = new_row; open_columns &= new_row; }
    }
    if ((~B.closed_columns & open_columns).any()) {
      B.set_falsified(); return;
    }
    diag.right(); antidiag.left();
  } while (found);

  while (B.i < N and B.b[B.i].none()) ++B.i;
  if (B.i == N) return;
  assert(B.i < N-1);
}


typedef std::uint_fast64_t count_t;
typedef std::pair<count_t,count_t> result_t; // count, nodes

void operator +=(result_t& r, const result_t other) {
  r.first += other.first;
  r.second += other.second;
}
std::ostream& operator <<(std::ostream& out, const result_t& r) {
  return out << r.first << " " << r.second;
}

template <class R, template <class> class ER>
result_t count(const Board<R>& B) {
  result_t res{0,1};
  for (const R new_row : B.b[B.i]) {
    Board<R> Bj(B);
    Bj.b[B.i] = new_row; Bj.closed_columns |= new_row;
    ucp<R,ER>(Bj);
    if (Bj.satisfied()) ++res.first;
    else if (not Bj.falsified()) res += count<R,ER>(Bj);
  }
  return res;
}


  typedef Row_uint R;
  template<class X> using ER = ExtRow_uint<X>;

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  std::vector<std::future<result_t>> results;

  for (size_t i = 0; i < (N+1)/2; ++i) {
    auto B = Board<R>(i); ucp<R, ER>(B);
    if (B.satisfied())
      results.push_back(std::async(std::launch::deferred, [](){return result_t{1,1};}));
    else if (B.falsified())
      results.push_back(std::async(std::launch::deferred, [](){return result_t{0,1};}));
    else
      results.push_back(std::async(std::launch::async, count<R, ER>, B));
  }
  result_t res{};
  for (size_t i = 0; i < N/2; ++i) res += results[i].get();
  res.first *= 2;
  if (N % 2 == 1) res += results.back().get();
  std::cout << N << " " << res << "\n";
}
