// Oliver Kullmann, 10.4.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing a "large" independent set of a graph by program Redumis
  (via "use_redumis").

TODO:

1. DONE User-interface similar to MaxIndependentGreedy.cpp

*/

#include <iostream>
#include <tuple>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include "Bicliques2SAT.hpp"
#include "BCC2SAT.hpp"
#include "Graphs.hpp"
#include "GraphTools.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.4",
        "10.4.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/MaxIndependentRedumis.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  const std::string comment = "# ";

  const int default_seed = 0;
  const double default_timeout = 10;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " format-options seed t \n\n"
    " format-options : " << Environment::WRP<Bicliques2SAT::DC>{} << "\n"
    "                : " << Environment::WRP<Bicliques2SAT::BC>{} << "\n"
    " seed           : " << "integer (int), or \"r\"; default=" 
                         << default_seed << "\n"
    " t              : " << "timeout (seconds), of type double > 0; default="
                         << default_timeout << "\n\n"
    " reads a graph from standard input, and prints an independent set to standard output:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 4) {
    std::cerr << error <<
      "Exactly three arguments (form-opt, seeds, t)"
      " needed, but " << argc-1 << " provided.\n";
    return int(argc > 4 ? BCC2SAT::Error::too_many_parameters :
                          BCC2SAT::Error::missing_parameters);
  }

  const Bicliques2SAT::format2_options_t formopt =
    Environment::translate<Bicliques2SAT::format2_options_t>()(argv[1], Bicliques2SAT::sep);
  const std::string sstring = argv[2];
  const int seed = sstring.empty() ? default_seed :
    sstring=="r" ? RandGen::device_to_eseed() : std::stoi(sstring);
  const std::string tstring = argv[3];
  const double timeout = tstring.empty() ? default_timeout :
    std::stod(tstring);

  const bool with_comments =
    std::get<Bicliques2SAT::DC>(formopt) == Bicliques2SAT::DC::with;
  const bool with_result =
    std::get<Bicliques2SAT::BC>(formopt) == Bicliques2SAT::BC::with;
  if (with_comments) {
    BCC2SAT::commandline_output(std::make_tuple(Bicliques2SAT::DC::with),
                                comment, std::cout, argc, argv);
    std::cout << comment << "comments-result " << with_comments
              << " " << with_result << "\n";
    std::cout << comment << "seed " << seed << "\n";
    std::cout << comment << "timeout " << timeout << std::endl;
  }

  const auto G = Graphs::make_AdjVecUInt(std::cin, Graphs::GT::und);

  if (with_comments) {
    std::cout << comment << " V " << G.n() << "\n"
              << comment << " E " << G.m() << std::endl;
  }

}
