// Oliver Kullmann, 22.4.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing a "small" vertex cover of a graph by program FastVC
  (via "use_fastvc").

TODO:

*/

#include <iostream>
#include <tuple>
#include <limits>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include "Bicliques2SAT.hpp"
#include "BCC2SAT.hpp"
#include "Graphs.hpp"
#include "GraphTools.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.6",
        "22.4.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/MinVertexCoverFastVC.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  const std::string comment = "# ";

  const std::string default_use_fastvc = "use_fastvc";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " format-options seed t path \n\n"
    " format-options : " << Environment::WRP<Bicliques2SAT::DC>{} << "\n"
    " seed           : " << "unsigned integer (unsigned), or \"r\"; default="
                         << GraphTools::default_fastvc_seed << "\n"
    " t              : " << "timeout (seconds), of type double; default="
                         << GraphTools::default_fastvc_timeout << "\n"
    " path           : " << "the use_fastvc tool; default=\""
                         << default_use_fastvc << "\"\n\n"
    " reads a graph from standard input, and prints the size of a "
    " vertex cover to standard output:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n\n"
;
    return true;
  }

  unsigned read_seed(const std::string& arg) {
    if (arg.empty()) return GraphTools::default_fastvc_seed;
    if (arg == "r") return RandGen::device_to_eseed();
    const long unsigned seed0 = std::stoul(arg);
    if (seed0 > std::numeric_limits<unsigned>::max()) {
      std::cerr << error << "seed=" << seed0
                << " too large for unsigned with max="
                << std::numeric_limits<unsigned>::max() << ".\n";
      std::exit(int(BCC2SAT::Error::faulty_parameters));
    }
    return seed0;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 5) {
    std::cerr << error <<
      "Exactly four arguments (form-opt, seeds, t, path)"
      " needed, but " << argc-1 << " provided.\n";
    return int(argc > 4 ? BCC2SAT::Error::too_many_parameters :
                          BCC2SAT::Error::missing_parameters);
  }

  const Bicliques2SAT::format2_options_t formopt =
    Environment::translate<Bicliques2SAT::format2_options_t>()(argv[1], Bicliques2SAT::sep);
  const unsigned seed = read_seed(argv[2]);
  const std::string tstring = argv[3];
  const double timeout = tstring.empty() ?
    GraphTools::default_fastvc_timeout : std::stod(tstring);
  const std::string pstring = argv[4];
  const std::string path_use_fastvc = pstring.empty() ? default_use_fastvc :
    pstring;

  const bool with_comments =
    std::get<Bicliques2SAT::DC>(formopt) == Bicliques2SAT::DC::with;
  if (with_comments) {
    BCC2SAT::commandline_output(std::make_tuple(Bicliques2SAT::DC::with),
                                comment, std::cout, argc, argv);
    std::cout << comment << "comments " << with_comments << "\n";
    std::cout << comment << "seed " << seed << "\n";
    std::cout << comment << "timeout " << timeout << std::endl;
  }

  const auto G = Graphs::make_AdjVecUInt(std::cin, Graphs::GT::und);

  if (G.m() == 0) {
    if (with_comments)
      std::cout << comment << " ZERO EDGES.\n";
    std::cout << "0\n";
    return 0;
  }
  if (has_loops(G)) {
    std::cerr << "ERROR: LOOPS not allowed.\n";
    return int(BCC2SAT::Error::bad_graph);
  }

  const GraphTools::FastVC_call F(G,seed,timeout,path_use_fastvc);
  if (with_comments) {
    std::cout << comment << "command: " << F.command() << std::endl;
  }

  if (with_comments) {
    std::cout << comment << " V " << G.n() << "\n"
              << comment << " E " << G.m() << std::endl;
  }

  const auto result = F();
  if (result.empty()) {
    std::cerr << "ERROR: EMPTY vertex cover.\n";
    return int(BCC2SAT::Error::bad_external_call);
  }
  if (not F.check_vertexcover(result)) {
    std::cerr << "ERROR: VERTEX COVER CHECK failed.\n";
    return int(BCC2SAT::Error::faulty_external_tool);
  }
  std::cout << result.size() << std::endl;
}
