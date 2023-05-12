// Oliver Kullmann, 12.5.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing a maximal independent set of a graph by the simplest greedy
  algorithm


EXAMPLES:

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include "Graphs.hpp"
#include "Bicliques2SAT.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.5",
        "12.5.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/MaxIndependentGreedy.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;
  using namespace BCC2SAT;
  using namespace Graphs;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  const std::string comment = "# ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " format-options seeds \n\n"
    " format-options : " << Environment::WRP<DC>{} << "\n"
    "                : " << Environment::WRP<BC>{} << "\n"
    " seeds          : " << "sequence, can contain \"t\" or \"r\"" << "\n\n"
    " reads a graph from standard input, and prints a maximal vertex-set to standard output:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 3) {
    std::cerr << error <<
      "Exactly two arguments (form-opt, seeds)"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const format2_options_t formopt =
    Environment::translate<format2_options_t>()(argv[2], sep);
  const RandGen::vec_eseed_t seeds = RandGen::extract_seeds(argv[2]);

  const bool with_comments = std::get<DC>(formopt) == DC::with;
  const bool with_result = std::get<BC>(formopt) == BC::with;
  if (with_comments) {
    commandline_output(std::make_tuple(DC::with), comment, std::cout,
                       argc, argv);
    // XXX
  }

  const auto G = make_AdjVecUInt(std::cin, GT::und);

  if (with_comments) {
    // XXX
  }

  const auto I = maximal_independent_greedy_simplest(G, seeds);

  if (with_comments) {
    // XXX
  }

  if (with_result) {
    // XXX
  }

}
