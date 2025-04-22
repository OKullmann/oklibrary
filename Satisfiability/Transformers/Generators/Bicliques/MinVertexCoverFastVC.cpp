// Oliver Kullmann, 22.4.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing a "small" vertex cover of a graph by program FastVC
  (via "use_fastvc").
  A solution is always checked.

EXAMPLES:

Bicliques> time GraphGen grid 277 101 | ./Graph2BCcompGraph -com "" | ./MinVertexCoverFastVC "" "" "" ""
# "./MinVertexCoverFastVC" "" "" "" ""
# seed 0
# timeout 10
# command: timelimit=10 seed=0 use_fastvc
#  V 55576
#  E 220798
41676
real	0m4.597s user	0m4.647s sys	0m0.070s

Using minimum-time 0 (and no comments):
Bicliques> time GraphGen grid 277 101 | ./Graph2BCcompGraph -com "" | ./MinVertexCoverFastVC -com "" 0 ""
41774
real	0m2.487s user	0m2.561s sys	0m0.046s

The time-limit introduces some non-determinism:
Bicliques> time GraphGen grid 277 101 | ./Graph2BCcompGraph -com "" | ./MinVertexCoverFastVC -com "" 1 ""
41704
real	0m3.471s user	0m3.529s sys	0m0.059s
Bicliques> time GraphGen grid 277 101 | ./Graph2BCcompGraph -com "" | ./MinVertexCoverFastVC -com "" 1 ""
41705
real	0m3.507s user	0m3.545s sys	0m0.077s

Seeds can make a difference:
Bicliques> time GraphGen grid 277 101 | ./Graph2BCcompGraph -com "" | ./MinVertexCoverFastVC -com 2 1 ""
41687
real	0m3.510s user	0m3.610s sys	0m0.045s
Bicliques> time GraphGen grid 277 101 | ./Graph2BCcompGraph -com "" | ./MinVertexCoverFastVC -com 2 1 ""
41686
real	0m3.487s user	0m3.567s sys	0m0.044s

The default time-limit of 10s seems reaching the optimum for smaller graphs.
However e.g.
Bicliques> time GraphGen grid 277 101 | ./Graph2BCcompGraph -com "" | ./MinVertexCoverFastVC -com 3 "" ""
41653
real	0m5.431s user	0m5.439s sys	0m0.108s
Bicliques> time GraphGen grid 277 101 | ./Graph2BCcompGraph -com "" | ./MinVertexCoverFastVC -com 5 "" ""
41627
real	0m7.120s user	0m7.170s sys	0m0.073s

FastVC aborts as soon as progress stalls (no attempt of escaping this
optimum). Thus for larger graphs performing many trials (with different seeds)
can help.


TODOS:

1. As with MaxIndependentGreedy, we also need the possibility of
   evaluating many rounds.
    - Then naturally printing statistics in the comments.
    - While the total result is then the minimum.

2. Provide FastVC with a better version of srand.
    - So that we get platform/compiler-independence.
    - Since FastVC is a small program, which should be straight-forward.

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

  const Bicliques2SAT::format1_options_t formopt =
    Environment::translate<Bicliques2SAT::format1_options_t>()
    (argv[1], Bicliques2SAT::sep);
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
