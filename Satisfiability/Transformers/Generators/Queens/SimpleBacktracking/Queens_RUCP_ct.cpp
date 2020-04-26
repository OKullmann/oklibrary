// Oliver Kullmann 25.4.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

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
      "0.5.4",
      "26.4.2020",
      __FILE__,
      "Oliver Kullmann",
      "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/Queens_RUCP_ct.cpp",
      "GPL v3"};

typedef std::size_t size_t;
#ifndef NN
# error "NN must be defined."
#endif
constexpr size_t N=NN;
static_assert(N >= 1);

bool show_usage(const int argc, const char* const argv[]) {
  assert(argc >= 1);
  if (not Environment::help_header(std::cout, argc, argv, proginfo))
    return false;
  std::cout <<
    "> " << proginfo.prg << "\n"
    " runs the program for built-in N = " << N << ", outputting\n"
    "  N solution_count node_count\n"
    "\n"
    "> ./Call_QueensRUCPct N\n"
    " compiles optimised and debugging forms of this program, and runs\n"
    " the optimised version, with added user-time (s) and max-memory (kb).\n"
;
  return true;
}


enum class RS { empty=0, unit=1, other=2 }; // "row-state"
class Board;
class Row {
  typedef std::bitset<N> row_t; // "true" means forbidden or occupied
  row_t r;
  Row(row_t r) noexcept : r(r) {}
  void set(size_t i) noexcept { r.set(i); }
  friend Board initial(size_t) noexcept;

  // Iterating through the positions i with not x[i], dereferencing to
  // the bitset with exactly one 1, at position i:
  class IteratorRow {
    row_t x;
    size_t i;
  public :
    IteratorRow() noexcept : i(N) {}
    IteratorRow(const row_t x) noexcept : x(x), i() {
      if (not x[0]) i = 0;
      else for (i=1; i < N and x[i]; ++i);
    }
    IteratorRow& operator ++() noexcept {
      assert(i < N and not x[i]);
      while (++i < N and x[i]);
      return *this;
    }
    Row operator *() const noexcept {
      assert(i < N);
      return 1ull << i;
    }
    bool operator !=(const IteratorRow rhs) { return i != rhs.i; }
  };

public :
  Row() = default;
  Row(const unsigned long long u) : r(u) {}
  unsigned long long to_ullong() const noexcept { return r.to_ullong(); }

  bool none() const noexcept { return r.none(); }
  RS rs() const noexcept {
    if (r.all()) return RS::empty;
    else if (r.count() == N-1) return RS::unit;
    else return RS::other;
  }

  void reset() noexcept { r.reset(); }
  void operator |= (const Row& rhs) noexcept { r |= rhs.r; }

  friend Row operator | (const Row& lhs, const Row& rhs) noexcept {
    return lhs.r | rhs.r;
  }
  friend Row operator ~ (const Row& r) noexcept {
    return ~ r.r;
  }

  IteratorRow begin() const noexcept { return r; }
  IteratorRow end() const noexcept { return {}; }
};


struct Board {
  typedef std::array<Row,N> board_t;
  board_t b;
  size_t i; // current bottom-row, i <= N
  bool falsified_;
  // If not falsified, then the board is amo+alo-consistent, assuming that
  // all-0-rows mean rows with placed queen.

  bool falsified() const noexcept { return falsified_; }
  bool satisfied() const noexcept { return not falsified_ and i >= N-1; }
};


template <class R>
class ExtRow {
  typedef std::bitset<3*N-2> extrow_t;
  static_assert(N <= 32); // so that to_ullong suffices
  extrow_t b;
public :
  ExtRow(const R& r) noexcept : b(r.to_ullong() << (N-1)) {}
  operator R() const noexcept {  return ((b << (N-1)) >> 2*(N-1)).to_ullong(); }
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
  operator R() const noexcept {  return (b << (N-1)) >> 2*(N-1); }
  void add(const R& r) noexcept { b |= ExtRow_uint(r).b; }
  void left() noexcept { b <<= 1; }
  void right() noexcept { b >>= 1; }
};

// Propagate the single queen which is set in the current bottom-row:
template <class R = Row, class ER = ExtRow_uint<R>>
inline void ucp(Board& B) noexcept {
  if (N <= 1) return;
  assert(not B.falsified());
  assert(not B.satisfied());

  Row units = B.b[B.i];
  ++B.i;
  ER diag(units), antidiag = diag;
  bool found;
  do {
    // Up-sweep:
    found = false;
    for (size_t j = B.i; j != N; ++j) {
      diag.left(); antidiag.right();
      if (B.b[j].none()) continue;
      assert(B.b[j].rs() != RS::empty);
      const Row new_row = B.b[j] | units | Row(diag) | Row(antidiag);
      const RS rs = new_row.rs();
      if (rs == RS::empty) { B.falsified_ = true; return; }
      else if (rs == RS::unit) {
        const Row new_unit = ~ new_row;
        units |= new_unit; diag.add(new_unit); antidiag.add(new_unit);
        B.b[j].reset();
        found = true;
      }
      else B.b[j] = new_row;
    }
    if (not found) break;

    // Down-sweep:
    found = false;
    for (size_t j0 = N-1; j0 != B.i; --j0) {
      const size_t j = j0-1;
      diag.right(); antidiag.left();
      if (B.b[j].none()) continue;
      assert(B.b[j].rs() != RS::empty);
      const Row new_row = B.b[j] | units | Row(diag) | Row(antidiag);
      const RS rs = new_row.rs();
      if (rs == RS::empty) { B.falsified_ = true; return; }
      else if (rs == RS::unit) {
        const Row new_unit = ~ new_row;
        units |= new_unit; diag.add(new_unit); antidiag.add(new_unit);
        B.b[j].reset();
        found = true;
      }
      else B.b[j] = new_row;
    }
    diag.right(); antidiag.left();
  } while (found);

  while (B.i < N and B.b[B.i].none()) ++B.i;
  if (B.i == N) return;
  assert(B.i < N-1);
}


Board initial(const size_t i) noexcept {
  assert(i < N);
  Board res{};
  for (size_t j = 0; j < N; ++j) res.b[j].set(i);
  ucp(res);
  return res;
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

result_t count(const Board& B) {
  result_t res{0,1};
  for (const Row new_row : B.b[B.i]) {
    Board Bj(B);
    Bj.b[B.i] = new_row;
    ucp(Bj);
    if (Bj.satisfied()) ++res.first;
    else if (not Bj.falsified()) res += count(Bj);
  }
  return res;
}

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  std::vector<std::future<result_t>> results;

  for (size_t i = 0; i < (N+1)/2; ++i) {
    const Board B = initial(i);
    if (B.satisfied())
      results.push_back(std::async(std::launch::deferred, [](){return result_t{1,1};}));
    else if (B.falsified())
      results.push_back(std::async(std::launch::deferred, [](){return result_t{0,1};}));
    else
      results.push_back(std::async(std::launch::async, count, B));
  }
  result_t res{};
  for (size_t i = 0; i < N/2; ++i) res += results[i].get();
  res.first *= 2;
  if (N % 2 == 1) res += results.back().get();
  std::cout << N << " " << res << "\n";
}
