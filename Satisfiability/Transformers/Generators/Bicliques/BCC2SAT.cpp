// Oliver Kullmann, 25.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* 

*/


#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Graphs.hpp"
#include "Bicliques2SAT.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.1",
        "6.3.2022",
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

  const auto G = Graphs::make_AdjVecUInt(std::cin, Graphs::GT::und);
  BC2SAT trans(G, B);
  try { trans(std::cout, algopt, formopt, sb_rounds); }
  catch (const BC2SAT::Unsatisfiable& e) {
    std::cerr << "UNSAT\nB >= " << e.incomp.size() << "\n";
    return int(Error::found_unsat);
  }

}
