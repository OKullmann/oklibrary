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

std::future is used for parallel computation, using N/2 parallel threads.

  Version 0.7.1, 20.7.2019.

  Usage:

> ./qcount_p N

Output of the solution count; e.g.

> ./qcount_p 8
92

That is, 92 solutions (nonattacking placements of 8 queens on the 8x8 board).


For timing-output, the gnu-tool /usr/bin/time is recommended.
For example, output to file Out, first current date and time, then the output
of this program, then the command itself, then the time measurements,
first user-time (most important), then total-time, then system-time,
always appending to Out:
> date >> Out; /usr/bin/time -ao Out -f "%C\n%U %e %S" ./qcount_p 16 >>Out
> cat Out
Tue 26 Jun 11:11:23 BST 2018
14772512
./qcount 16
3.66 3.66 0.00

A simple experiment in Bash then, running N=0,...,20, and collecting the data
in file NQ_out:

> OUT="NQ_out"; for ((N=0; N<=20; ++N)); do echo "N=$N" | tee -a $OUT;
   date >> $OUT; /usr/bin/time -ao $OUT -f "%C\n%U %e %S" ./qcount_p $N >> $OUT;
   done

*/


#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <future>

#include <cstdint>
#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "NQueens.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.7.2",
        "23.7.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/NQueenspar.cpp",
        "GPL v3"};

using namespace Queens;

// The recursive counting-function;
// using bit-positions 0, ..., N-1 for the columns 1, ..., N:

queen_t all_columns; // the first N bits 1, the rest 0
input_t N;

// Idea: size-many rows (from bottom) have been processed, now consider the
// next row, and try to place the next queen in some column.
inline count_t backtracking(queen_t avail,
  const queen_t columns, const queen_t fdiag, const queen_t fantid,
  const input_t size) {
  // avail: columns available (set to 1) for this invocation (only)
  // columns: the current placement of queens
  // fdiag: forbidden columns due to diagonal constraints
  // fantid: forbidden columns due to antidiagonal constraints
  assert(size == 0 or avail == (~(columns|fdiag|fantid) & all_columns));
  //assert(std::bitset<maxN>(columns).count() == size);
  assert(avail);
  count_t count = 0;
  const queen_t sdiag = fdiag>>1, santid = fantid<<1;
  const queen_t newavail0 = ~(columns | sdiag | santid) & all_columns;
  if (not newavail0) return count;
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
      if (newavail)
	count += backtracking(newavail,newcolumns,newdiag,newantid,sp1);
    } while (next = keeprightmostbit(avail^=next));
  return count;
}

  bool show_usage(const int argc, const char* const argv[]) {
    assert(argc >= 1);
    if (argc != 2 or not Environment::is_help_string(argv[1])) return false;
    const std::string& program = proginfo.prg;
    std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version information and exits.\n"
    "> " << program << " [-h | --help]\n"
    " shows help information and exits.\n"
    "> " << program << " N\n"
    " computes the solution- and node-count for the board of dimension N.\n"
;
    return true;
  }


}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  count_t count = 0;
  std::vector<std::future<count_t>> futures;
  const unsigned long arg1 = std::stoul(argv[1]);
  if (arg1 <= 1) { std::cout << 1 << "\n"; return 0; }
  if (arg1 > maxN) { std::cerr << " N <= " << int(maxN) << " required.\n"; return 1; }
  N = arg1;
  all_columns = setrightmostbits(N);
  // Using mirror-symmetry around vertical axis:
  if (N % 2 == 0) {
    for (int i = 0; i < N/2; ++i) futures.push_back(std::async(std::launch::async, backtracking, one(i), 0, 0, 0, 0));
    for (auto& e : futures) count += e.get();
    std::cout << 2*count << "\n";
  } else {
    for(int i = 0; i < N/2; ++i) futures.push_back(std::async(std::launch::async, backtracking, one(i), 0, 0, 0, 0));
    for (auto& e : futures) count += e.get();
    const count_t half = count;
    count = backtracking(one(N/2), 0, 0, 0, 0);
    std::cout << 2*half + count << "\n";
  }
}
