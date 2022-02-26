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
#include <Numerics/NumInOut.hpp>

#include "Graphs.hpp"
#include "Bicliques2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
        "26.2.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/BCC2SAT.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;

    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc < 4) return 1;

  const id_t B = FloatingPoint::toUInt(argv[1]);

  const alg_options_t algopt =
    Environment::translate<alg_options_t>()(argv[2], sep);
  const SB sb = std::get<SB>(algopt);

  const format_options_t formopt =
    Environment::translate<format_options_t>()(argv[3], sep);
  const DC dc = std::get<DC>(formopt);
  const DP dp = std::get<DP>(formopt);
  const CS cs = std::get<CS>(formopt);

  const auto G = Graphs::make_AdjVecUInt(std::cin, Graphs::GT::und);
  BC2SAT trans(G, B);
  try { trans(std::cout, sb, dc, dp, cs); }
  catch (const BC2SAT::Unsatisfiable& e) {
    std::cout << "UNSAT\nB >= " << e.incomp.size() << "\n";
  }

}
