// Irfansha Shaik 26.6.2018 (Swansea)
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
  https://github.com/OKullmann/oklibrary/commits/master/Satisfiability
  ID 3d8d9e84ec1154cf6773dfc71b1c3cec5a1f0be4.

A "white queen" is a queen placed on a white square (chessboard) or a white
vertex (Queens graph).
A square/vertex (x,y), where x is the row-index and y is the column-index, is
considered white iff x+y is odd; x,y are either both 0-based or both 1-based.

  Version 0.5, 26.6.2018.
  Usage:
> ./wqcount N

Output of the solution count; e.g.

> ./wqcount 9
4 304
6 48

That is, 304 solutions with 4 white queens and 48 solutions with 6 white
queens in each solution,
(nonattacking placements of 9 queens on the 8x8 board).

For compilation use,

>g++ --std=c++11 -pedantic -Wall -Wno-parentheses -Ofast -DNDEBUG   NQueens_w.cpp -o wqcount

For timing-output, the gnu-tool /usr/bin/time is recommended.
For example, output to file Out, first current date and time, then the output
of this program, then the command itself, then the time measurements,
first user-time (most important), then total-time, then system-time,
always appending to Out:
> date >> Out; /usr/bin/time -ao Out -f "%C\n%U %e %S" ./wqcount 16 >>Out
> cat Out
Tue 26 Jun 11:42:57 BST 2018
4 76
6 1143066
8 12486228
10 1143066
12 76
./wqcount 16
5.34 5.34 0.00
A simple experiment in Bash then, running N=0,...,20, and collecting the data
in file NQ_out:
> OUT="NQ_out"; for ((N=0; N<=20; ++N)); do echo "N=$N" | tee -a $OUT;
   date >> $OUT; /usr/bin/time -ao $OUT -f "%C\n%U %e %S" ./wqcount $N >> $OUT;
   done
*/

#include <cstdint>
#include <iostream>
#include <string>
#include <cassert>
#include <limits>
#include <vector>
#include <iomanip>

namespace {

typedef std::uint8_t input_t; // type of N
typedef std::uint_fast64_t count_t; // counting solutions

// Definition of queen_t, the bits representing the N columns:
#ifndef NMAX
# define NMAX 64
#endif
constexpr input_t maxN = NMAX;
static_assert(maxN == 32 or maxN == 64, "Max value for N: 32 or 64.");
#if maxN == 32
  typedef std::uint_fast32_t queen_t;
#else
 typedef std::uint_fast64_t queen_t;
#endif
static_assert(std::numeric_limits<queen_t>::digits >= maxN, "Problem with queen_t.");
// For 64 < N <= 128, use queen_t = std::uint_fast128_t (and appropriate count_t).

// Five helper functions for bit-operations:

// N 1's from the right, the rest 0:
constexpr queen_t setrightmostbits(const input_t N) noexcept {
  return (N>=maxN) ? queen_t(-1) : (queen_t(1) << N) - 1;
}
queen_t all_columns; // set by setrightmostbits

// 1 at position+1 (from right), the rest 0:
constexpr queen_t one(const input_t position) noexcept {
  return queen_t(1) << position;
}

// Set all bits in x to 0 except of rightmost one (if exists):
inline constexpr queen_t keeprightmostbit(const queen_t x) noexcept {
  return -x & x;
}

// Repeating the bit-pattern 1010:
#if maxN == 32
  constexpr queen_t wbref = 0xAAAAAAAA;
#else
  constexpr queen_t wbref = 0xAAAAAAAAAAAAAAAA;
#endif
/* Variable bit is a single bit at position j >= 0 -- return true if the
   parity of j is as given by parameter even:
*/
  inline constexpr bool parity_pos(const queen_t bit, const bool even) noexcept {
    return unsigned(even) ^ unsigned(bool(wbref & bit));
}

// The recursive counting-function;
// using bit-positions 0, ..., N-1 for the columns 1, ..., N:
input_t N;
std::vector<count_t> wcount; // wcount[i] is the number of solutions with precisely i+1 white queens

// Idea: size-many rows (from bottom) have been processed, now consider the
// next row, and try to place the next queen in some column.
inline void backtracking(queen_t avail,
  const queen_t columns, const queen_t fdiag, const queen_t fantid,
  const input_t size, const input_t numw) noexcept {
  // avail: columns available (set to 1) for this invocation (only)
  // columns: the current placement of queens
  // fdiag: forbidden columns due to diagonal constraints
  // fantid: forbidden columns due to antidiagonal constraints
  // numw: number of white queens
  const bool odd_row = size % 2 == 1;
  assert(size == 0 or avail == (~(columns|fdiag|fantid) & all_columns));
  //assert(std::bitset<maxN>(columns).count() == size);
  assert(avail);
  const queen_t sdiag = fdiag>>1, santid = fantid<<1;
  const queen_t newavail0 = ~(columns | sdiag | santid) & all_columns;
  if (not newavail0) return;
  queen_t next = keeprightmostbit(avail); // could be any bit, but that seems fastest
  const input_t sp1 = size+1; // due to the placement of next
  assert(sp1 < N);
  if (sp1+1 == N) do {
    const bool slr = parity_pos(next,odd_row); // true iff second last row has a white queen
    const queen_t lravail = newavail0 & ~(next | next>>1 | next<<1); // lravail is available position in last row
    const bool lr = parity_pos(lravail, !odd_row); // true iff last row has a white queen
    if (bool(lravail)) ++wcount[numw+slr+lr-1];
    } while (next = keeprightmostbit(avail^=next));
  else
    do {const queen_t newcolumns = columns|next,
          nextrs = next>>1, nextls = next<<1,
          newdiag = sdiag | nextrs, newantid = santid | nextls,
          newavail = newavail0 & ~(next | nextrs | nextls);
        if (newavail) backtracking(newavail,newcolumns,newdiag,newantid,sp1,(parity_pos(next,odd_row)) ? numw+1 : numw);
    } while (next = keeprightmostbit(avail^=next));
}
}

int main(const int argc, const char* const argv[]) {
  if (argc != 2) { std::cout << "Usage[qcount]: N\n"; return 0; }
  const unsigned long arg1 = std::stoul(argv[1]);
  if (arg1 <= 3) { std::cout << 0 << " " << 0 << "\n"; return 0; }
  if (arg1 > maxN) { std::cerr << " N <= " << int(maxN) << " required.\n"; return 1; }
  N = arg1;
  wcount.resize(N); // should be N+1 XXX
  all_columns = setrightmostbits(N);
  // Using mirror-symmetry around vertical axis:
  if (N % 2 == 0) {
    backtracking(setrightmostbits(N/2), 0, 0, 0, 0, 0);
    // Due to vertical flip number of white queens count become black queen count for mirror solutions:
    for (input_t i = 0; i < N/2; ++i)
      wcount[i] = wcount[N-i-2] = wcount[i] + wcount[N-i-2];
  } else {
    backtracking(setrightmostbits(N/2), 0, 0, 0, 0, 0);
    for(count_t& c : wcount) c *= 2;
    backtracking(one(N/2), 0, 0, 0, 0, 0);
  }

  for (input_t i = 0; i != wcount.size(); ++i)
    if (wcount[i] != 0) std::cout << std::setw(2) << i+1 << " " << wcount[i] << "\n";
}
