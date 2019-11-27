// Oliver Kullmann, 22.11.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

USAGE:

> ./SolsQueens [c1|c2|c12]

runs with fixed N, and outputs the statistics for resp. the
1-components, 2-components, or both (the default).

With

> ./RSolsQueens N [c1|c2|c12]

first the version with (compile-time constant) N is compiled, and then
it is run.

OUTPUT:

We have (always) initially the number of solutions, and then 2 lines
per k-components (k=1,2), e.g.

GenericSAT> ./SolsQueens
c Output_time 27.11.2019 11:38:56_+0000 1574854736546410164
c ** Program information **
c program_name                          "SolsQueens"
c version                               "0.4.2"
c date                                  "27.11.2019"
c gid_id                                "aacd7f71503c3f68020133152fbb14062f1a17cc"
c author                                "Oliver Kullmann"
c url                                   "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/GenericSAT/SolsQueens.cpp"
c ** Machine information **
c machine_name                          "csltok.swansea.ac.uk"
c bogomips                              4788.22
c ** Compilation information **
c compiler_version                      "g++ 9.2.0"
c compilation_date                      "Nov_27_2019 11:37:48"
c compilation_options                   "--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -funroll-loops -fno-finite-math-only"
c ** Parameters **
c command-line                          "./SolsQueens"
c N                                     10
c  number_solutions(G)                  7.24e-07
c  size_solution(B)                     10
c  size_solution_vector(GB)             7.24e-06
c  size_vertex(B)                       8
c  size_fixed_vectors(GB)               1.3756e-05
c computation                           both12
724
612: 1,532 2,48 3,32
0.00414365 0.309392
130: 1,80 2,8 3,24 6,8 7,8 226,2
0.312155

We have 612 1-components, of sizes 1,2,3, where the largest components,
of size 3, occur 32 times.
The quotient
  ql = size of largest components / number of vertices
is ~ 4e-3, the average degree of a vertex is ~ 0.309.

And we have 130 2-components, of sizes 1,2,3,6,7,226, where the largest
component occcurs twice.
The quotient ql = 226 / number_solutions is ~ 0.312.

Using the script RSolsQueens, we get in an additional line the runtime in
seconds, and the maximum residual memory in kb:
GenericSAT> ./RSolsQueens 10
...
724
612: 1,532 2,48 3,32
0.00414365 0.309392
130: 1,80 2,8 3,24 6,8 7,8 226,2
0.312155
  0.03 3488

So runtime (user) was 0.03s, and ~ 3.5MB residual memory was used.

The large overhead, compared to the total size of the fixed vectors, is due
to the small problem size; e.g. on cs-kullmann-server:

GenericSAT> ./RSolsQueens 15 c1
g++ --std=c++17 -pedantic -fmax-errors=5 -Wall -Wextra -I ../../../../../Programming -I ../../../../../Satisfiability/Transformers/Generators -Ofast -DNDEBUG -march=native -fwhole-program -static  -funroll-loops  -fno-finite-math-only -DMACHINE="\"cs-kullmann-server.swan.ac.uk\"" -DBOGOMIPS=5400.00 -DGITID="\"aacd7f71503c3f68020133152fbb14062f1a17cc\"" -DOPTIMISATION="\"--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static  -funroll-loops  -fno-finite-math-only -DNQUEENS=15  \"" -DNQUEENS=15  SolsQueens.cpp  -o SolsQueens
c Output_time 27.11.2019 11:54:49_+0000 1574855689855774722
c ** Program information **
c program_name                          "SolsQueens"
c version                               "0.4.2"
c date                                  "27.11.2019"
c gid_id                                "aacd7f71503c3f68020133152fbb14062f1a17cc"
c author                                "Oliver Kullmann"
c url                                   "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/GenericSAT/SolsQueens.cpp"
c ** Machine information **
c machine_name                          "cs-kullmann-server.swan.ac.uk"
c bogomips                              5400
c ** Compilation information **
c compiler_version                      "g++ 8.2.1 20180831 [gcc-8-branch revision 264010]"
c compilation_date                      "Nov_27_2019 11:54:47"
c compilation_options                   "--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -funroll-loops -fno-finite-math-only -DNQUEENS=15"
c ** Parameters **
c command-line                          "./SolsQueens" "c1" "15"
c N                                     15
c  number_solutions(G)                  0.00227918
c  size_solution(B)                     15
c  size_solution_vector(GB)             0.0341878
c  size_vertex(B)                       8
c  size_fixed_vectors(GB)               0.0547004
c computation                           only1
2279184
1224236: 1,843488 2,197568 3,73768 4,36920 5,20448 6,13116 7,8696 8,6180 9,4344 10,3476 11,2816 12,2028 13,1600 14,1436 15,1192 16,808 17,776 18,608 19,504 20,376 21,396 22,356 23,280 24,208 25,236 26,192 27,184 28,160 29,144 30,152 31,108 32,112 33,120 34,112 35,116 36,144 37,96 38,64 39,64 40,64 41,40 42,48 43,16 44,72 45,40 46,32 47,32 48,72 49,8 50,8 51,48 52,16 53,8 54,8 55,8 56,8 57,40 58,24 59,16 61,24 62,8 63,8 65,24 67,32 68,8 69,8 71,8 72,8 74,24 77,8 79,8 80,8 84,8 89,8 90,8 94,8 95,8 152,4 236,8 277,8
0.000121535 0.971618
  16.86 62772



TODOS:

1. We need the same statistics also for the modular solutions.

2. We should also have vertex-degree-statistics:
    - min (this is always 0)
    - max
    - DONE mean

 */

#include <iostream>
#include <string>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "Heuristics.hpp"
#include "Solutions.hpp"
#include "Recursion.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.3",
        "27.11.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/GenericSAT/SolsQueens.cpp",
        "GPL v3"};

  using namespace Solutions;

  constexpr lines_t N =
#ifndef NQUEENS
  10
#else
  NQUEENS
#endif
;
  constexpr auto number_solutions = Recursion::exact_value(N);
  constexpr auto size_solution = sizeof(solution_t<N>);

  using FloatingPoint::float80;

# define UNIT "G"
  constexpr float80 num_unit = std::giga::num;

void print_constants() {
  using Environment::DHW;
  using Environment::DWW;
  std::cout << DWW{"N"} << (unsigned)N << "\n"
            << DWW{" number_solutions(" UNIT ")"} <<
                 float80(number_solutions) / num_unit << "\n"
            << DWW{" size_solution(B)"} << size_solution << "\n"
            << DWW{" size_solution_vector(" UNIT "B)"} <<
                 float80(number_solutions) * size_solution / num_unit << "\n"
            << DWW{" size_vertex(B)"} << sizeof(ChessBoard::Count_t) << "\n"
            << DWW{" size_fixed_vectors(" UNIT "B)"} <<
                 float80(number_solutions) *
                  (size_solution+sizeof(ChessBoard::Count_t)+1) / num_unit
                  << "\n"
;
}

bool show_usage(const int argc, const char* const argv[]) {
  assert(argc >= 1);
  if (argc != 2 or not Environment::is_help_string(argv[1])) return false;
  const std::string& program = proginfo.prg;
  std::cout << "USAGE:\n"
  "> " << program << " (-v | --version)\n"
  " shows version information and exits (including information on memory-usage).\n"
  "> " << program << " (-h | --help)\n"
  " shows help information and exits.\n"
  "> " << program << " [c1|c2|c12]\n"
  " runs the program with the built-in N, computing 1/2-components, or both"
  " (the default).\n"
  "Run\n"
  "> make -B CPPFLAGS=-DNQUEENS=12 " << program << "\n"
  "for compilation with e.g. N=12.\n"
  "Use the script \"RSolsQueens\" for integrated compilation and running.\n"
;
  return true;
}

  template <class ActiveClauseSet>
  struct ListSolutions {
    solution_vector<N> V;
    ListSolutions() { V.reserve(Recursion::exact_value(N)); }
    void sat(const ActiveClauseSet& F) {
      V.push_back(extract<N>(F.board()));
    }
    void unsat(const ActiveClauseSet&) noexcept {}
  };

  // Which of k-neighbourhoods, k=1,2, to compute:
  enum class CK { both12=0, only1=1, only2=2 };
}
namespace Environment {
  template <>
  struct RegistrationPolicies<CK> {
   static constexpr int size = int(CK::only2) + 1;
   static constexpr std::array<const char*, size> string
      {"c12", "c1", "c2"};
  };
}
namespace {
  std::ostream& operator <<(std::ostream& out, const CK k) {
    switch (k) {
    case CK::both12 : return out << "both12";
    case CK::only1 : return out << "only1";
    case CK::only2 : return out << "only2";
    default : return out << "UNKNOWN:" << int(k);}
  }
  typedef std::tuple<CK> options_t;
  constexpr char sep = ',';
}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) {
    std::cout << "\n"; print_constants();
    return 0;
  }
  if (show_usage(argc, argv)) return 0;

  Environment::Index index;
  const options_t choices = (argc <= index) ? options_t{} : Environment::translate<options_t>()(argv[index++], sep);
  const CK level = std::get<CK>(choices);
  index.deactivate();

  std::cout << Environment::Wrap(proginfo, Environment::OP::dimacs);
  using Environment::DHW;
  using Environment::DWW;
  std::cout << DHW{"Parameters"}
            << DWW{"command-line"};
  Environment::args_output(std::cout, argc, argv);
  std::cout << "\n";
  print_constants();
  std::cout << DWW{"computation"} << level << "\n"
; std::cout.flush();

  NQueens::AmoAlo_board Fq(N);
  Backtracking::CountSat<NQueens::AmoAlo_board, Heuristics::TawHeuristics<>, Trees::NoOpTree, Backtracking::EmptyUSAT, Backtracking::Statistics<Backtracking::EmptyUSAT>, ListSolutions<NQueens::AmoAlo_board>> B;
  const auto rFq = B(Fq);
  const auto num_solutions = rFq.solutions;
  assert(num_solutions == number_solutions);
  assert(num_solutions == B.L.V.size());
  std::cout << num_solutions << std::endl;
  std::stable_sort(B.L.V.begin(), B.L.V.end());

  if (level != CK::only2) {
    const auto f1 = frequencies(determine_ccs1<N>(B.L.V));
    std::cout << f1 << "\n";
    assert(not f1.freq.empty());
    std::cout << float80((--f1.freq.cend())->first) / num_solutions << " "
      << float80(f1.s.sum_deg) / num_solutions << std::endl;
  }

  if (level != CK::only1) {
    const auto f2 = frequencies(determine_ccs2<N>(B.L.V));
    std::cout << f2 << "\n";
    assert(not f2.freq.empty());
    std::cout << float80((--f2.freq.cend())->first) / num_solutions << "\n";
  }
}
