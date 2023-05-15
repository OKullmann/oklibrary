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

For a biclique with two equal sides, the maximum independent sets are
exactly the two sides, and the simple greedy approach finds them:
Bicliques> ./GraphGen biclique 6 6 | ./MaxIndependentGreedy_debug "" 0 ""
# "./MaxIndependentGreedy_debug" "" "0"
# comments-result 1 1
# num_e-seeds 1
# e-seeds 0
# T 1
#  V 12
#  E 36
#  result-size 6
l5 l6 l3 l1 l4 l2
Bicliques> ./GraphGen biclique 6 6 | ./MaxIndependentGreedy_debug -com 4 ""
r4 r5 r2 r1 r6 r3

For the biclique-compatability-graphs of grids, it seems this simple
lower bound for bcc is easily exact when just using a relatively
small number of runs:

Bicliques> time ./GraphGen grid 100 100 | ./Graph2BCcompGraph | ./MaxIndependentGreedy -sol 18 ""
# "./MaxIndependentGreedy" "-sol" "18"
# comments-result 1 0
# num_e-seeds 1
# e-seeds 18
# T 1
#  V 19800
#  E 78406
#  result-size 4999
real	0m2.920s
user	0m2.934s
sys	0m0.021s
Bicliques> time ./GraphGen grid 100 100 | ./Graph2BCcompGraph | ./MaxIndependentGreedy -sol "" 1000
# "./MaxIndependentGreedy" "-sol" "" "1000"
# comments-result 1 0
# num_e-seeds 0
# T 1000
#  V 19800
#  E 78406
#  result-sizes 1000 : 4950 4977.59 4999; 14.6813
#  seed 18
real	0m9.795s
user	0m9.811s
sys	0m0.020s

Bicliques> time ./GraphGen grid 200 200 | ./Graph2BCcompGraph | ./MaxIndependentGreedy -sol 18 ""
# "./MaxIndependentGreedy" "-sol" "18"
# comments-result 1 0
# num_e-seeds 1
# e-seeds 18
# T 1
#  V 79600
#  E 316806
#  result-size 19999
real	0m31.440s
user	0m31.551s
sys	0m0.041s
Bicliques> time ./GraphGen grid 200 200 | ./Graph2BCcompGraph | ./MaxIndependentGreedy -sol 18 1000
# "./MaxIndependentGreedy" "-sol" "18" "1000"
# comments-result 1 0
# num_e-seeds 1
# e-seeds 18
# T 1000
#  V 79600
#  E 316806
#  result-sizes 1000 : 19900 19956.3 19999; 29.832
#  seed 4
real	1m3.254s
user	1m3.359s
sys	0m0.036s

Bicliques> time ./GraphGen grid 300 300 | ./Graph2BCcompGraph | ./MaxIndependentGreedy -sol "" 1000
# "./MaxIndependentGreedy" "-sol" "" "1000"
# comments-result 1 0
# num_e-seeds 0
# T 1000
#  V 179400
#  E 715206
#  result-sizes 1000 : 44851 44929.1 44999; 47.382
#  seed 18
real	3m57.414s
user	3m57.635s
sys	0m0.121s
Just measuring the time for the construction of the graph:
Bicliques> time ./GraphGen grid 300 300 | ./Graph2BCcompGraph | ./MaxIndependentGreedy -sol "" 0
real	2m23.794s
user	2m24.037s
sys	0m0.118s
(very slow currently).


Remark:

As one can see above, the seed 18 worked the same under very different
circumstances: if for a series of experiments one wants to make (practically)
sure that different pseudo-random sequences are created, then easiest to
create one fixed timestamp T, store it (for reproducyibility), and use
key-sequences "T,i" for this series.

*/

#include <iostream>
#include <tuple>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>
#include <Numerics/NumInOut.hpp>
#include <Numerics/Statistics.hpp>

#include "Graphs.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.1",
        "14.5.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/MaxIndependentGreedy.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;
  using namespace BCC2SAT;
  using namespace Graphs;
  typedef AdjVecUInt::id_t idv_t;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  const std::string comment = "# ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " format-options seeds T \n\n"
    " format-options : " << Environment::WRP<DC>{} << "\n"
    "                : " << Environment::WRP<BC>{} << "\n"
    " seeds          : " << "sequence, can contain \"t\" or \"r\"\n"
    " T              : " << "number of trials, natural number >= 0, default 1"
                         << "\n\n"
    " reads a graph from standard input, and prints a maximal vertex-set to standard output:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n"
    "  - T can be an arbitrary 80-bit floating-point (rounding is applied).\n\n"
;
    return true;
  }

  typedef GenStats::BasicStats<idv_t, FloatingPoint::float80> stats_t;
  std::tuple<AdjVecUInt::list_t, stats_t, idv_t>
  perform_trials(const AdjVecUInt& G,
                 RandGen::vec_eseed_t seeds,
                 const idv_t T) {
    assert(T != 0);
    if (T == 1)
      return {maximal_independent_greedy_simplest(G, seeds), {}, {}};
    else {
      AdjVecUInt::list_t res;
      stats_t S;
      idv_t opti = 0;
      seeds.push_back(opti);
      for (idv_t i = 0; i < T; ++i) {
        seeds.back() = i;
        AdjVecUInt::list_t I = maximal_independent_greedy_simplest(G, seeds);
        S += I.size();
        if (I.size() > res.size()) {
          res = std::move(I);
          opti = i;
        }
      }
      return {res,S,opti};
    }
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 4) {
    std::cerr << error <<
      "Exactly two arguments (form-opt, seeds, T)"
      " needed, but " << argc-1 << " provided.\n";
    return int(argc > 4 ? Error::too_many_parameters :
                          Error::missing_parameters);
  }

  const format2_options_t formopt =
    Environment::translate<format2_options_t>()(argv[1], sep);
  const RandGen::vec_eseed_t seeds = RandGen::extract_seeds(argv[2]);
  const std::string Tstring = argv[3];
  const idv_t T = Tstring.empty() ? 1 : FloatingPoint::toUInt(argv[3]);

  const bool with_comments = std::get<DC>(formopt) == DC::with;
  const bool with_result = std::get<BC>(formopt) == BC::with;
  if (with_comments) {
    commandline_output(std::make_tuple(DC::with), comment, std::cout,
                       argc, argv);
    std::cout << comment << "comments-result " << with_comments
              << " " << with_result << "\n"
              << comment << "num_e-seeds " << seeds.size() << "\n";
    if (not seeds.empty())
      std::cout << comment << "e-seeds " << RandGen::ESW{seeds} << "\n";
    std::cout << comment << "T " << T << std::endl;
  }

  const auto G = make_AdjVecUInt(std::cin, GT::und);

  if (with_comments) {
    std::cout << comment << " V " << G.n() << "\n"
              << comment << " E " << G.m() << std::endl;
  }

  const auto [I,S,opti] = perform_trials(G, seeds, T);
  if (T == 0) return 0;

  if (with_comments) {
    if (T == 1)
      std::cout << comment << " result-size " << I.size() << "\n";
    else
      std::cout << comment << " result-sizes " << S << "\n"
                << comment << " seed " << opti << "\n";
  }
  std::cout.flush();

  if (with_result) {
    G.output(std::cout, I);
    std::cout << "\n";
  }

}
