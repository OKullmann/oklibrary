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

The basic idea is the same as explained at
  https://helloacm.com/n-queen-problem-in-back-tracing-bit-logics/
and indeed the same as the program from 2002 by Jeff Somers
  http://users.rcn.com/liusomers/nqueen_demo/nqueens.html

Variable numrows there is our size, lsb is next, bitfield is avail, mask
is all_columns. The array aQueenBitCol contains the values of our variable
column, aQueenBitPosDiag of fdiag, aQueenBitNegDiag of fantid.

A white queen is a queen placed on a white square (chessboard) or a white vertex (Queens graph).
A square/vertex (x,y) where x is row value and y is column value is considered white iff (x+y) is odd.

  Version 0.5, 26.6.2018.
  Usage:
> ./qcount_w N

Output of the solution count; e.g.

> ./qcount_w 9
4 304
6 48

That is, 304 solutions with 4 white queens and 48 solutions with 6 white queens in each solution,
(nonattacking placements of 9 queens on the 8x8 board).

For compilation use,

>g++ --std=c++11 -pedantic -Wall -Wno-parentheses -Ofast -DNDEBUG   NQueens_w.cpp -o qcount_w

For timing-output, the gnu-tool /usr/bin/time is recommended.
For example, output to file Out, first current date and time, then the output
of this program, then the command itself, then the time measurements,
first user-time (most important), then total-time, then system-time,
always appending to Out:
> date >> Out; /usr/bin/time -ao Out -f "%C\n%U %e %S" ./qcount_w 16 >>Out
> cat Out
Tue 26 Jun 11:42:57 BST 2018
4 76
6 1143066
8 12486228
10 1143066
12 76
./qcount_w 16
5.34 5.34 0.00
A simple experiment in Bash then, running N=0,...,20, and collecting the data
in file NQ_out:
> OUT="NQ_out"; for ((N=0; N<=20; ++N)); do echo "N=$N" | tee -a $OUT;
   date >> $OUT; /usr/bin/time -ao $OUT -f "%C\n%U %e %S" ./qcount_w $N >> $OUT;
   done
*/

#include <cstdint>
#include <iostream>
#include <string>
#include <cassert>
#include <limits>
#include <vector>

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
// 1 at position+1 (from right), the rest 0:
constexpr queen_t one(const input_t position) noexcept {
  return queen_t(1) << position;
}

// sets alternate bits to one.
inline queen_t column(const input_t N) noexcept {
  queen_t wbref = 1;
  for (input_t i = 0; i<N-1 ; ++i) { wbref <<= 1; if ((i%2) == 1) wbref += 1;}
  return wbref;
}

// Set all bits in x to 0 except of rightmost one (if exists):
inline constexpr queen_t keeprightmostbit(const queen_t x) noexcept {
  return -x & x;
}

//Checks if the queen is white using wbref.
inline constexpr bool queen_w(bool qr, const queen_t wbref, const queen_t next) noexcept {
  return qr ^ bool(wbref & next);
}

// The recursive counting-function;
// using bit-positions 0, ..., N-1 for the columns 1, ..., N:
queen_t all_columns, wbref; // all_columns has the first N bits 1, the rest 0, wbref has alternate bits 1.
input_t N;
std::vector<count_t> wcount; // global white queens count.
bool slr = 0; // represents if second last row queen is white. 1 is white queen, 0 is not.
bool lr = 0; // represents if last row queen is white. 1 is white queen, 0 is not.

// Idea: size-many rows (from bottom) have been processed, now consider the
// next row, and try to place the next queen in some column.
inline void backtracking(queen_t avail,
  const queen_t columns, const queen_t fdiag, const queen_t fantid,
  const input_t size,count_t lwcount, bool o_r) noexcept {
  // avail: columns available (set to 1) for this invocation (only)
  // columns: the current placement of queens
  // fdiag: forbidden columns due to diagonal constraints
  // fantid: forbidden columns due to antidiagonal constraints
  // lwcount: white queens count for each solution
  // o_r: a single bit representing odd/even row. 1 is odd row and 0 is even row.
  o_r = !o_r;
  assert(size == 0 or avail == (~(columns|fdiag|fantid) & all_columns));
  //assert(std::bitset<maxN>(columns).count() == size);
  assert(avail);
  const queen_t sdiag = fdiag>>1, santid = fantid<<1;
  const queen_t newavail0 = ~(columns | sdiag | santid) & all_columns;
  if (not newavail0) return;
  queen_t next = keeprightmostbit(avail); // could be any bit, but that seems fastest
  const input_t sp1 = size+1; // due to the placement of next
  assert(sp1 < N);
  if (sp1+1 == N) {
    do{slr = queen_w(o_r,wbref,next); // checks and assigns 1 if second last row has a white queen.
      const queen_t lravail = newavail0 & ~(next | next>>1 | next<<1); // lravail is available position in last row
      lr = queen_w(!o_r,wbref,lravail); // checks and assigns 1 if last row has a white queen.
      if(bool(lravail)) ++wcount[lwcount+slr+lr-1]; // increments the count if it is a valid soluion.
    } while (next = keeprightmostbit(avail^=next));
  }
  else
    do {const queen_t newcolumns = columns|next,
          nextrs = next>>1, nextls = next<<1,
          newdiag = sdiag | nextrs, newantid = santid | nextls,
          newavail = newavail0 & ~(next | nextrs | nextls);
        bool flag = 0;
	if (queen_w(o_r,wbref,next)) { lwcount += 1; flag = 1; }
        if (newavail) backtracking(newavail,newcolumns,newdiag,newantid,sp1,lwcount,o_r);
        if (flag) lwcount -= 1;
    } while (next = keeprightmostbit(avail^=next));
}
}

int main(const int argc, const char* const argv[]) {
  if (argc != 2) { std::cout << "Usage[qcount]: N\n"; return 0; }
  const unsigned long arg1 = std::stoul(argv[1]);
  if (arg1 <= 3) { std::cout << 0 << " " << 0 << "\n"; return 0; }
  if (arg1 > maxN) { std::cerr << " N <= " << int(maxN) << " required.\n"; return 1; }
  N = arg1;
  wbref = column(N); // sets alternate bits to 1 as a reference to check if it is a white queen.
  wcount.resize(N);
  all_columns = setrightmostbits(N);
  // Using rotation-symmetry around vertical axis:
  if (N % 2 == 0) {
    backtracking(setrightmostbits(N/2), 0, 0, 0, 0, 0, 0);
    // due to vertical flip number of white queens count become black queen count for mirror solutions.
    for (input_t i = 0; i < wcount.size()/2; ++i) wcount[i] = wcount[N-i-2] = wcount[i] + wcount[N-i-2];
  } else {
    backtracking(setrightmostbits(N/2), 0, 0, 0, 0, 0, 0);
    for(auto& i : wcount) i <<= 1; //doubling white queen counts for symmetry around vertical axis.
    backtracking(one(N/2), 0, 0, 0, 0, 0, 0);
  }

  for (input_t i = 0; i != wcount.size(); ++i) if (wcount[i] != 0) std::cout<<i+1<<" "<<wcount[i]<<std::endl;
}
