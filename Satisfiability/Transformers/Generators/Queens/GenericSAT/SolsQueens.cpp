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
c Output_time 26.11.2019 04:29:26_+0000 1574742566151975495
c ** Program information **
c program_name                          "SolsQueens"
c version                               "0.3.0"
c date                                  "26.11.2019"
c gid_id                                "963cc5311253ec957c98c5862fd827bf19811b1f"
c author                                "Oliver Kullmann"
c url                                   "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Queens/GenericSAT/SolsQueens.cpp"
c ** Machine information **
c machine_name                          "csltok.swansea.ac.uk"
c bogomips                              4788.22
c ** Compilation information **
c compiler_version                      "g++ 9.2.0"
c compilation_date                      "Nov_26_2019 04:28:11"
c compilation_options                   "--std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -funroll-loops -fno-finite-math-only"
c ** Parameters **
c command-line                          "./SolsQueens"
c N                                     10
c  number_solutions(M)                  0.000724
c  size_solution(B)                     10
c  size_solution_vector(MB)             0.00724
c  size_vertex(B)                       8
c  size_fixed_vectors(MB)               0.013756
c computation                           both12
724
612: 1,532 2,48 3,32
0.00414365
130: 1,80 2,8 3,24 6,8 7,8 226,2
0.312155

We have 612 1-components, of sizes 1,2,3, where the largest components,
of size 3, occur 32 times.
The quotient 3 / number_solutions is ~ 4e-3.

And we have 130 2-components, of sizes 1,2,3,6,7,226, where the largest
component occcurs twice.
The quotient 226 / number_solutions is ~ 0.3.


TODOS:

1. We need the same statistics also for the modular solutions.

2. We should also have vertex-degree-statistics (min, max, mean).

3. The main memory-usage is likely given by the vector of solutions.
    - DONE We know its size in advance, and can output that with the
      parameters.
    - DONE We should be able to predict well the complete memory-usage, and
      also output that under "Parameters".
      - DONE We can sum up the sizes of the fixed vectors (of type
        solution_ccs and std::vector<bool>).
    - std::sort should be replaced by an in-memory algorithm. Does
      std::stable_sort achieve that?
    - The max-memory-usage as reported by RSolsQueens is much higher than
      predicted (including that the stack uses another solution-vector,
      so to speak) -- where does this come from?
 */

#include <iostream>
#include <string>
#include <fstream>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "Heuristics.hpp"
#include "Solutions.hpp"
#include "Recursion.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.0",
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

void print_constants() {
  using Environment::DHW;
  using Environment::DWW;
  std::cout << DWW{"N"} << (unsigned)N << "\n"
            << DWW{" number_solutions(M)"} <<
                 float80(number_solutions) / 1'000'000 << "\n"
            << DWW{" size_solution(B)"} << size_solution << "\n"
            << DWW{" size_solution_vector(MB)"} <<
                 float80(number_solutions) * size_solution / 1'000'000 << "\n"
            << DWW{" size_vertex(B)"} << sizeof(ChessBoard::Count_t) << "\n"
            << DWW{" size_fixed_vectors(MB)"} <<
                 float80(number_solutions) *
                  (size_solution+sizeof(ChessBoard::Count_t)+1) / 1'000'000
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
  std::sort(B.L.V.begin(), B.L.V.end());

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
