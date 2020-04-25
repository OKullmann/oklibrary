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
      "0.5.0",
      "25.4.2020",
      __FILE__,
      "Oliver Kullmann",
      "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/Queens_RUCP_ct.cpp",
      "GPL v3"};

typedef std::size_t size_t;
#ifndef NN
# error "NN must be defined."
#endif
constexpr size_t N=NN;

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


typedef std::bitset<N> row_t; // "true" means forbidden or occupied
typedef std::array<row_t,N> board_t;


typedef std::bitset<3*N> extrow_t;
static_assert(N >= 1 and N <= 32);

inline extrow_t embed(const row_t& r) noexcept {
  return r.to_ullong() << N;
}
inline row_t truncate(const extrow_t& er) noexcept {
  return ((er << N) >> 2*N).to_ullong();
}
inline void add(const row_t& r, extrow_t& er) noexcept {
  er |= embed(r);
}


struct Board {
  board_t b;
  size_t i; // current bottom-row, i <= N
  bool falsified_;
  // If not falsified, then the board is amo+alo-consistent, assuming that
  // all-0-rows mean rows with placed queen.

  bool falsified() const noexcept { return falsified_; }
  bool satisfied() const noexcept { return not falsified_ and i >= N-1; }
};


// Propagate the single queen which is set in the current bottom-row:
inline void ucp(Board& B) noexcept {
  if (N <= 1) return;
  assert(not B.falsified());
  assert(not B.satisfied());
  assert(B.b[B.i].count() == 1);

  row_t units = B.b[B.i];
  ++B.i;
  extrow_t diag = embed(units), antidiag = diag;
  bool found;
  do {
    // Up-sweep:
    found = false;
    for (size_t j = B.i; j != N; ++j) {
      diag >>= 1; antidiag <<= 1;
      if (B.b[j].none()) continue;
      assert(B.b[j].count() < N);
      const row_t new_row = B.b[j] | units | truncate(diag) | truncate(antidiag);
      const auto count = new_row.count();
      if (count == N) { B.falsified_ = true; return; }
      else if (count == N-1) {
        const row_t new_unit = ~ new_row;
        units |= new_unit; add(new_unit, diag); add(new_unit, antidiag);
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
      diag <<= 1; antidiag >>= 1;
      if (B.b[j].none()) continue;
      assert(B.b[j].count() < N);
      const row_t new_row = B.b[j] | units | truncate(diag) | truncate(antidiag);
      const auto count = new_row.count();
      if (count == N) { B.falsified_ = true; return; }
      else if (count == N-1) {
        const row_t new_unit = ~ new_row;
        units |= new_unit; add(new_unit, diag); add(new_unit, antidiag);
        B.b[j].reset();
        found = true;
      }
      else B.b[j] = new_row;
    }
    diag <<= 1; antidiag >>= 1;
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
  for (size_t j = 0; j < N; ++j) {
    if (B.b[B.i][j]) continue;
    Board Bj(B);
    Bj.b[B.i].reset();
    Bj.b[B.i].set(j);
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
