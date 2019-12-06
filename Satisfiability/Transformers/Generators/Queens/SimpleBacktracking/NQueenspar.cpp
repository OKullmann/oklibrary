// Irfansha Shaik 26.6.2018 (Swansea)
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

std::future is used for parallel computation, using N/2 parallel threads.

  Usage:

> ./pqcount N

Output of N and the solution count; e.g.

> ./pqcount 8
8 92

*/

#include <iostream>
#include <string>
#include <vector>
#include <future>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "NQueens.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.7.4",
        "6.12.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/SimpleBacktracking/NQueenspar.cpp",
        "GPL v3"};

using namespace Queens;

constexpr input_t N_default = 15;
static_assert(N_default <= maxN);

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
    const std::string& program = "./pqcount";
    std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version information and exits.\n"
    "> " << program << " [-h | --help]\n"
    " shows help information and exits.\n"
    "> " << program << " N\n"
    " computes the solution-count for the board of dimension N.\n"
    "The default-value of N is " << (unsigned long) N_default << ".\n"
;
    return true;
  }


}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const unsigned long arg1 = argc < 2 ? N_default : std::stoul(argv[1]);
  if (arg1 <= 1) { std::cout << 1 << "\n"; return 0; }
  if (arg1 > maxN) {
    std::cerr << " N <= " << (unsigned long) maxN << " required.\n"; return 1;
  }
  N = arg1;
  std::cout << (unsigned long) N << " "; std::cout.flush();

  typedef std::future<count_t> future_t;
  all_columns = setrightmostbits(N);
  // Using mirror-symmetry around vertical axis:
  if (N % 2 == 0) {
    std::vector<future_t> futures;
    for (input_t i = 0; i < N/2; ++i)
      futures.push_back(std::async(std::launch::async, backtracking, one(i), 0, 0, 0, 0));
    count_t count = 0;
    for (auto& e : futures) count += e.get();
    count *= 2;
    std::cout << count << "\n";
  } else {
    std::vector<future_t> futures;
    for (input_t i = 0; i < N/2; ++i)
      futures.push_back(std::async(std::launch::async, backtracking, one(i), 0, 0, 0, 0));
    future_t mid = std::async(std::launch::async, backtracking, one(N/2), 0, 0, 0, 0);
    count_t count = 0;
    for (auto& e : futures) count += e.get();
    count *= 2;
    count += mid.get();
    std::cout << count << "\n";
  }
}
