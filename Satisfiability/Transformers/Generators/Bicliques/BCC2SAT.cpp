// Oliver Kullmann, 25.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Translating biclique-covering problems into SAT-problems
  (see BCCbySAT for the complete packaging of repeated SAT-solving
   for computing bcc(G) of the input-graph G)


EXAMPLES:

Just obtaining statistics:

Bicliques> ./GraphGen grid 20 20 | ./BCC2SAT 200 "" "-cs"
c ** Parameters **
c V                                     400
c E                                     760
c B                                     200
c sb-option                             basic-sb
c ** Formatting **
c comments-option                       with-comments
c dimacs-parameter-option               with-parameters
c clauses-option                        without-cs
c ** Symmetry Breaking **
c planted-edges                         159
c sb-stats                              100 : 141 149.29 159; 3.60498
c num_e-seeds                           1
c  e-seeds                              1646648668327395707
p cnf 312000 32609237

One sees that symmetry-breaking with 100 attempts obtained a maximum of
159 planted edges.


TODOS:

1. Read seeds

*/


#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Graphs.hpp"
#include "Bicliques2SAT.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.5.0",
        "8.3.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/BCC2SAT.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;
  using namespace BCC2SAT;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " B=" << default_B <<
      " algo-options format-options [sb-rounds=" <<
      default_sb_rounds << "]\n\n"
    " algo-options   : " << Environment::WRP<SB>{} << "\n"
    " format-options : " << Environment::WRP<DC>{} << "\n"
    "                : " << Environment::WRP<DP>{} << "\n"
    "                : " << Environment::WRP<CS>{} << "\n\n"
    " reads a graph from standard input, and prints the SAT-translation to standard output:\n\n"
    "  - Arguments \"\" (the empty string) yield also the default-values.\n"
    "  - Default-values for the options are the first possibilities given.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc < 4) {
    std::cerr << error << "Exactly three arguments (B, algo-opt, form-opt)"
      " needed, but only " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const var_t B = read_var_t(argv[1], default_B);
  const alg_options_t algopt =
    Environment::translate<alg_options_t>()(argv[2], sep);
  const format_options_t formopt =
    Environment::translate<format_options_t>()(argv[3], sep);
  const var_t sb_rounds = argc >= 5 ?
    read_var_t(argv[4], default_sb_rounds) : default_sb_rounds;

  if (std::get<SB>(algopt) != SB::none and sb_rounds == 0) {
    std::cerr << error <<
      "Symmetry-breaking on, but number of rounds is zero.\n";
    return int(Error::bad_sb);
  }
  if (std::get<SB>(algopt) == SB::extended) {
    std::cerr << error <<
      "Extended symmetry-breaking not implemented yet.\n";
    return int(Error::bad_sb);
  }

  const auto G = Graphs::make_AdjVecUInt(std::cin, Graphs::GT::und);
  BC2SAT trans(G, B);
  try { trans(std::cout, algopt, formopt, sb_rounds); }
  catch (const BC2SAT::Unsatisfiable& e) {
    std::cerr << "UNSAT\nB >= " << e.incomp.size() << "\n";
    return int(Error::found_unsat);
  }

}
