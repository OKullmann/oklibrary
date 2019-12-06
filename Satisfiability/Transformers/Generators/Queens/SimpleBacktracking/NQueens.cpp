// Oliver Kullmann 25.5.2018 (Swansea)
/*
  Copyright Alessandro Nicolosi 2016, 2017
  Copyright Oliver Kullmann 2018, 2019

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
and indeed the same as the program from 2002 by Jeff Somers
  http://users.rcn.com/liusomers/nqueen_demo/nqueens.html

Variable numrows there is our size, lsb is next, bitfield is avail, mask
is all_columns. The array aQueenBitCol contains the values of our variable
column, aQueenBitPosDiag of fdiag, aQueenBitNegDiag of fantid.
An array is used there to simulate the recursion.
The additional aQueenBitRes there just represents the solution for printing.

  Usage:

> ./qcount N

Output of N, the solution count, and the number of nodes; e.g.

> ./qcount 8
8 92 615

That is, 92 solutions (nonattacking placements of 8 queens on the 8x8 board),
using 615 nodes in the backtracking tree.

Without an argument, the default N=13 is used.

For help and version:

> ./qcount -h
> ./qcount -v

For compilation, the Makefile in the same directory as this file can be used.
It produces also the debugging-version "qcount_debug".
To set NMAX (maximal allowed value of N; default 64), use e.g.
> CXXFLAGS="-DNMAX=32" make


TODOS:

1. Update to C++17: DONE
2. Update makefile, and use the new framework (Environment.hpp).

3. Use signals (like the tawSolver), to show current counts.
4. Use futures for parallelisation: DONE (in NQueenspar.cpp)
5. Simulate the recursion by a stack (as done in Jeff Somers's original
   program, whose runtime is very similar to the runtime of this program
   (version 1.0)). OK doesn't expect a speed-up, but one can try.

*/

#include <iostream>
#include <string>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "NQueens.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "1.1.2",
        "6.12.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/NQueens.cpp",
        "GPL v3"};

using namespace Queens;

constexpr input_t N_default = 14;
static_assert(N_default <= maxN);

// The recursive counting-function;
// using bit-positions 0, ..., N-1 for the columns 1, ..., N:

count_t count = 0, nodes = 0;
queen_t all_columns; // the first N bits 1, the rest 0
input_t N;

// Idea: size-many rows (from bottom) have been processed, now consider the
// next row, and try to place the next queen in some column.
inline void backtracking(queen_t avail,
  const queen_t columns, const queen_t fdiag, const queen_t fantid,
  const input_t size) noexcept {
  // avail: columns available (set to 1) for this invocation (only)
  // columns: the current placement of queens
  // fdiag: forbidden columns due to diagonal constraints
  // fantid: forbidden columns due to antidiagonal constraints
  assert(size == 0 or avail == (~(columns|fdiag|fantid) & all_columns));
  //assert(std::bitset<maxN>(columns).count() == size);
  ++nodes;
  assert(avail);
  const queen_t sdiag = fdiag>>1, santid = fantid<<1;
  const queen_t newavail0 = ~(columns | sdiag | santid) & all_columns;
  if (not newavail0) return;
  queen_t next = keeprightmostbit(avail); // could be any bit, but that seems fastest
  const input_t sp1 = size+1; // due to the placement of next
  assert(sp1 < N);
  if (sp1+1 == N) {
    do
      count += bool(newavail0 & ~(next | next>>1 | next<<1));
    while (next = keeprightmostbit(avail^=next));
  }
  else
    do {const queen_t newcolumns = columns|next,
          nextrs = next>>1, nextls = next<<1,
          newdiag = sdiag | nextrs, newantid = santid | nextls,
          newavail = newavail0 & ~(next | nextrs | nextls);
      if (newavail) backtracking(newavail,newcolumns,newdiag,newantid,sp1);
    } while (next = keeprightmostbit(avail^=next));
}

  bool show_usage(const int argc, const char* const argv[]) {
    assert(argc >= 1);
    if (argc != 2 or not Environment::is_help_string(argv[1])) return false;
    const std::string& program = "./qcount";
    std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version information and exits.\n"
    "> " << program << " [-h | --help]\n"
    " shows help information and exits.\n"
    "> " << program << " N\n"
    " computes the solution- and node-count for the board of dimension N.\n"
    "The default-value of N is " << (unsigned long) N_default << ".\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const unsigned long arg1 = argc < 2 ? N_default : std::stoul(argv[1]);
  if (arg1 <= 1) { std::cout << 1 << " " << nodes << "\n"; return 0; }
  if (arg1 > maxN) {
    std::cerr << " N <= " << (unsigned long) maxN << " required.\n"; return 1;
  }
  N = arg1;
  std::cout << (unsigned long) N << " "; std::cout.flush();

  all_columns = setrightmostbits(N);
  // Using mirror-symmetry around vertical axis:
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
