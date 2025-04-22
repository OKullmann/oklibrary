// Oliver Kullmann, 10.4.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing a "large" independent set of a graph by program Redumis
  (via "use_redumis").
  A solution is always checked.

EXAMPLES:

Not printing the solution:
Bicliques> time GraphGen grid 20 20 | ./MaxIndependentRedumis -sol "" "" ""
# "./MaxIndependentRedumis" "-sol" "" "" ""
# comments-result 1 0
# seed 0
# timeout 10
# command: timelimit=10 use_redumis --seed=0 --disable_checks
#  V 400
#  E 760
#  result-size 200
real	0m5.672s user	0m6.289s sys	0m0.003s

The last line of the comments contains the size of the independent set found.

The time-limit can be set to 0 (or, equivalently, a negative number),
running Redumis for the minimum time:
Bicliques> time GraphGen grid 20 20 | ./MaxIndependentRedumis -sol "" 0 ""
# "./MaxIndependentRedumis" "-sol" "" "0" ""
# comments-result 1 0
# seed 0
# timeout 0
# command: timelimit=0 use_redumis --seed=0 --disable_checks
#  V 400
#  E 760
#  result-size 200
real	0m0.209s user	0m0.834s sys	0m0.004s

However giving at least 1s time to Redumis seems sensible, since with the
minimum-run quite often the optimum is not reached (but it is with 1s).

TODOS:

1. As with MaxIndependentGreedy, we also need the possibility of
   evaluating many rounds.
    - Then naturally printing statistics in the comments.
    - The solution found is the first smallest one.

2. Provide Redumis with a better version of srand.
    - So that we get platform/compiler-independence.


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
        "0.1.3",
        "22.4.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/MaxIndependentRedumis.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  const std::string comment = "# ";

  const std::string default_use_redumis = "use_redumis";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " format-options seed t path \n\n"
    " format-options : " << Environment::WRP<Bicliques2SAT::DC>{} << "\n"
    "                : " << Environment::WRP<Bicliques2SAT::BC>{} << "\n"
    " seed           : " << "integer (int), or \"r\"; default=" 
                         << GraphTools::default_redumis_seed << "\n"
    " t              : " << "timeout (seconds), of type double; default="
                         << GraphTools::default_redumis_timeout << "\n"
    " path           : " << "the use_redumis tool; default=\""
                         << default_use_redumis << "\"\n\n"
    " reads a graph from standard input, and prints an independent set to standard output:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n\n"
;
    return true;
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
  const std::string sstring = argv[2];
  const int seed = sstring.empty() ? GraphTools::default_redumis_seed :
    sstring=="r" ? RandGen::device_to_eseed() : std::stoi(sstring);
  const std::string tstring = argv[3];
  const double timeout = tstring.empty() ?
    GraphTools::default_redumis_timeout : std::stod(tstring);
  const std::string pstring = argv[4];
  const std::string path_use_redumis = pstring.empty() ? default_use_redumis :
    pstring;

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

  if (G.n() == 0) {
    if (with_comments)
      std::cout << comment << " ZERO VERTICES.\n";
    return 0;
  }
  if (has_loops(G)) {
    std::cerr << "ERROR: LOOPS not allowed.\n";
    return int(BCC2SAT::Error::bad_graph);
  }

  const GraphTools::Redumis_call R(G,seed,timeout,path_use_redumis);
  if (with_comments) {
    std::cout << comment << "command: " << R.command() << std::endl;
  }

  if (with_comments) {
    std::cout << comment << " V " << G.n() << "\n"
              << comment << " E " << G.m() << std::endl;
  }

  const auto result = R();
  if (result.empty()) {
    std::cerr << "ERROR: EMPTY independent set.\n";
    return int(BCC2SAT::Error::bad_external_call);
  }
  if (not R.check_independence(result)) {
    std::cerr << "ERROR: INDEPENDENCY CHECK failed.\n";
    return int(BCC2SAT::Error::faulty_external_tool);
  }
  if (with_comments)
    std::cout << comment << " result-size " << result.size() << "\n";
  if (with_result) {
    G.output_vertices(std::cout, result);
    std::cout << std::endl;
  }
}
