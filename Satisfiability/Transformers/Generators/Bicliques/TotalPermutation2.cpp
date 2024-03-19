// Oliver Kullmann, 19.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

EXAMPLES:

Bicliques> echo -e "p cnf 5 3\n1 2 0\n2 -5 0\n-2 -3 0\n" | ./TotalPermutation2
c 5 3 9938619543513371241 12113888734880821928
p cnf 5 3
1 3 0
-1 -2 0
-1 -5 0

(same result as with TotalPermutation).

*/

#include <iostream>
#include <vector>
#include <filesystem>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include "DimacsTools.hpp"
#include "TotalPermutation.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.9",
        "19.3.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TotalPermutation2.cpp",
        "GPL v3"};

  using namespace DimacsTools;
  using namespace TotalPermutation;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " [assignment-file]\n\n"
    " reads a strict 2-Dimacs-file from standard input, and prints the"
    " permuted 2-CNF to standard output.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 1 and argc != 2) {
    std::cerr << error <<
      "At most one arguments needed ([assignment-file]), but "
              << argc-1 << " provided.\n";
    return 1;
  }

  const std::string assignmentfile = assignment_file(argc, argv);
  kDimacsClauseList<2> F = read_strict_kDimacs<2>(std::cin);
  const auto s = seeds(F);
  RandGen::RandGen_t g(s);
  RandGen::shuffle(F.second.begin(), F.second.end(), g);

  const varvec_t V = random_variables(g, F.first.n);
  const signvec_t S = random_signs(g, F.first.n);
  output(std::cout, F, s, g, V, S);
  if (not assignmentfile.empty()) {
    if (std::filesystem::exists(assignmentfile)) {
      std::cerr << error <<
        "File \"" << assignmentfile << "\" already exists.\n";
      return 1;
    }
    std::ofstream out(assignmentfile);
    for (RandGen::gen_uint_t i = 0; i < F.first.n; ++i) {
      const RandGen::gen_uint_t oldv = i+1, v = V[i];
      out << oldv << " ";
      assert(S[i] == 1 or S[i] == -1);
      if (S[i] == -1) out << "-";
      out << v << "\n";
    }
    out.close();
    if (not out) {
      std::cerr << error <<
        "Writing problem to file \"" << assignmentfile << "\".\n";
      return 1;
    }
  }
}
