// Oliver Kullmann 25.5.2018 (Swansea)
/*
  Copyright Alessandro Nicolosi 2016, 2017
  Copyright Oliver Kullmann 2018

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

  Started as copy from
  https://github.com/alenic/nqueens/commits/master
  ID f50264291093fe8529d143995ae9784f097d7ebf.

The basic idea is the same as explained at

  https://helloacm.com/n-queen-problem-in-back-tracing-bit-logics/

  Usage:

> ./qcount N

Output of the solution count and the number of nodes; e.g.

> ./qcount 8
92 983

That is, 92 solutions (nonattacking placements of 8 queens on the 8x8 board),
using 983 nodes in the backtracking tree.

For compilation the Makefile in the same directory as this file can be used.
It produces also the debugging-version "qcount_debug".

*/

#include <cstdint>
#include <iostream>
#include <string>
#include <cassert>

namespace {

typedef std::uint8_t input_t;
typedef std::uint_fast64_t count_t;
typedef std::uint_fast64_t queen_t; // for 64 < N <= 128, use queen_t = std::uint_fast128_t (and appropriate count_t)

// N 1's from the right, the rest 0:
constexpr queen_t setrightmostbits(const input_t N) noexcept {
  // assert(N<=64);
  return (queen_t(1) << N) - 1;
}
// 1 at position+1 (from right), the rest 0:
constexpr queen_t one(const input_t position) noexcept {
  return queen_t(1) << position;
}
// Set all bits in x to 0 except of rightmost one (if exists):
inline constexpr queen_t keeprightmostbit(const queen_t x) noexcept {
  return -x & x;
}

count_t count = 0, nodes = 0;
queen_t all_columns;
input_t N;
inline void backtracking(queen_t avail,
  const queen_t columns, const queen_t fdiag, const queen_t fantid,
  input_t size) noexcept {
  // avail: columns available for this invocation (only), via their column positions (bit-positions 0, ..., N-1)
  // columns: the current placement of queens
  // fdiag: forbidden columns due to diagonal constraints
  // fantid: forbidden columns due to antidiagonal constraints
  // Idea: size-many rows (from bottom) have been processed, now consider the
  // next row, and try to place the next queen in some column.
  assert(size == 0 or avail == (~(columns|fdiag|fantid) & all_columns));
  ++nodes;
  assert(avail);
  ++size; // due to the placement of next
  assert(size < N);
  queen_t next = keeprightmostbit(avail); // could be any bit, but that seems fastest
  assert(next);
  do {const queen_t newcolumns = columns|next,
        newdiag = (fdiag|next) >> 1, newantid = (fantid|next) << 1,
        newavail = ~(newcolumns|newdiag|newantid) & all_columns;
    if (newavail)
      if (size+1 == N) ++count; else
      backtracking(newavail,newcolumns,newdiag,newantid,size);
  } while (next = keeprightmostbit(avail^=next));
}
}

int main(const int argc, const char* const argv[]) {
  if (argc != 2) { std::cout << "Usage[qcount]: N\n"; return 0; }
  const unsigned long arg1 = std::stoul(argv[1]);
  if (arg1 <= 1) { std::cout << 1 << " " << nodes << "\n"; return 0; }
  if (arg1 > 64) { std::cerr << " N <= 64 required.\n"; return 1; }
  N = arg1;
  all_columns = setrightmostbits(N);
  // Using rotation-symmetry around vertical axis:
  if (N % 2 == 0) {
    backtracking(setrightmostbits(N/2), 0, 0, 0, 0);
    std::cout << 2*count << " " << nodes << "\n";
  } else {
    backtracking(setrightmostbits(N/2), 0, 0, 0, 0);
    const count_t half = count; count = 0;
    backtracking(one(N/2), 0, 0, 0, 0);
    std::cout << 2*half + count << " " << nodes << "\n";
  }
}
