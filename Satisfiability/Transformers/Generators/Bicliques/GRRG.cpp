// Oliver Kullmann, 14.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Pseudo-random generation of graphs


EXAMPLES:

Output in PSALF-format of G(5,6):
Bicliques> GRRG und 5 6 "" ""
# 1609092523835210350 2 0 1746719382361957731 1 3 1 0 3 5 6 0
# 5 6 1
0 2 3 4
1 3
2 0 4
3 0 1 4
4 0 2 3

The first comments-line provides the seeds, the second line
shows n, m, and 1 for "undirected" ("0" would be for directed).
The last three seeds give n,m resp. n,p (see below).

In METIS-format:
Bicliques> GRRG und 5 6 "" metis
% 1609092523835210350 2 0 1746719382361957731 1 3 1 0 3 5 6 0
5 6
3 4 5
4
1 5
1 2 5
1 3 4

In Dimacs-format:
Bicliques> GRRG und 5 6 "" dimacs
c 1609092523835210350 2 0 1746719382361957731 1 3 1 0 3 5 6 0
p edge 5 6
e 1 3
e 1 4
e 1 5
e 2 4
e 3 5
e 4 5

Note that this is always the same graph (the format-argument is not
automatically integrated into the seed-sequence).

The above are uniform random graphs ("Erdoes-Renyi model"), while the
binomial model ("Gilbert model") with probability p = 6 / (5*4/2) = 6/10
(so that the average number of edges is also 6):

Bicliques> GRRG und 5 6/10 "" ""
# 1609092523835210350 2 0 1746719382361957731 0 3 1 0 3 5 3 5
# 5 8 1
0 1 2 3
1 0 2 4
2 0 1 3 4
3 0 2 4
4 1 2 3
Bicliques> GRRG und 5 6/10 "" metis
% 1609092523835210350 2 0 1746719382361957731 0 3 1 0 3 5 3 5
5 8
2 3 4
1 3 5
1 2 4 5
1 3 5
2 3 4
Bicliques> GRRG und 5 6/10 "" dimacs
c 1609092523835210350 2 0 1746719382361957731 0 3 1 0 3 5 3 5
p edge 5 8
e 1 2
e 1 3
e 1 4
e 2 3
e 2 5
e 3 4
e 3 5
e 4 5


Example for a small probability (all numbers can be given as 80-bit
floating-point numbers, using rounding to the nearest unsigned 64-bit
integer (for nominator and denominator separately):
Bicliques> time GRRG und 1e5 2/1e10 "" dimacs
c 1609092523835210350 2 0 1746719382361957731 0 3 1 0 3 100000 1 5000000000
p edge 100000 1
e 19858 26511
real	0m17.649s
user	0m17.647s
sys	0m0.000s
Bicliques> GRRG und 1e5 2.49/1e10 "" dimacs
c 1609092523835210350 2 0 1746719382361957731 0 3 1 0 3 100000 1 5000000000
p edge 100000 1
e 19858 26511


An example for a user-provided seed-sequence, with a first seed US for the
user-site (to be stored and used in the future), the second seed ES for
this experiment (also to be stored and re-used for this experiment),
and third a running index (taken to be 77):
Bicliques> GRRG und 5 6/10 r,t,77 ""
# 1609092523835210350 2 0 1746719382361957731 0 3 1 0 3 5 3 5 13057751223614336272 1747411664800262830 77
# 5 5 1
0 3 4
1 2
2 1 3
3 0 2 4
4 0 3

The values obtained by "r" and "t" are (of course) special for each
invocation.

Thus (here)
  US = 13057751223614336272
  ES = 1747411664800262830
  index = 77

Here US is to differentiate the user-site "from all others", while ES
differentiates the experimentes at the user-site -- "differentiation" here
means that re-using the same underlying pseudo-random-sequence is to be
avoided.

For the next graph in this experiment one would use
Bicliques> GRRG und 5 6/10 13057751223614336272,1747411664800262830,78 ""
# 1609092523835210350 2 0 1746719382361957731 0 3 1 0 3 5 3 5 13057751223614336272 1747411664800262830 78
# 5 5 1
0 2 3 4
1 3
2 0 4
3 0 1
4 0 2

For the starting-graph in another experiment (with index 0) one would keep
US, but change ES:
Bicliques> GRRG und 5 6/10 13057751223614336272,t,0 ""
# 1609092523835210350 2 0 1746719382361957731 0 3 1 0 3 5 3 5 13057751223614336272 1747411888756909473 0
# 5 3 1
0 2
1 3
2 0 3
3 1 2
4

Then the next graph in this sequence (using now the new ES):
Bicliques> GRRG und 5 6/10 13057751223614336272,1747411888756909473,1 ""
# 5 3 1
0 3 4
1 3
2
3 0 1
4 0


DESIGN RATIONAL:

1. Using a high-quality pseudo-random engine, the 64-bit Mersenne Twister
   (from the C++ standard library).

2. Allowing arbitrarily long sequences of user-seeds (unsigned 64 bits, which
   can be given, or created as timestamps or truly random values). Not just
   the usual 32-bit or maximally 64-bit seeds.
   These seeds can be given by the user, or can be demanded as timestamps
   (nanoseconds) or truly random values.
   In these way there can be generated general "master-"seeds for the whole
   user-site, and "experiment-"seeds for one experimental setting, and indices
   for the running formulas.

3. Seed-management makes automatically sure that every change of essential
   parameters results in a different random sequence (guaranteeing diversity).
   This is done by incorporating these parameters into the initial library-
   given part of the seed-sequence.
   With every other generator, all over the world again and again the same
   underlying random sequences are used.

4. Well-defined generation, fully reproducible on any machine and compiler,
   for now and the future. No floating-point operations are used, and every
   aspect of random generation not fully defined by the C++ standard is
   fixed. For the G(n,p) model, the probability p is given
   as a pair of two unsigned 64-bit integers, while n and m (for G(n,m) model)
   are unsigned 64-bit integers (though for G(n,m) n is restricted in fact
   to 32 bits).

5. 64-bit generation possibilities (fully w.r.t. the number of edges).

6. Supports binomial and uniform random graphs.

7. Supports output-formats PSALF (native), Dimacs, MEIIS.


DISCUSSION of existing libraries which allow to generate these graphs:

1. NetworkX

https://networkx.org/documentation/stable/reference/generated/networkx.generators.random_graphs.gnp_random_graph.html
gnp_random_graph

https://networkx.org/documentation/stable/reference/generated/networkx.generators.random_graphs.gnm_random_graph.html#networkx.generators.random_graphs.gnm_random_graph
gnm_random_graph

All just based on int (and unspecified).


2. igraph

https://igraph.org/c/doc/igraph-Random.html
allows different random generators; likely from the C++ standard library,
and thus the usage of these engines is unspecified.

https://igraph.org/c/doc/igraph-Generators.html#igraph_erdos_renyi_game_gnp
igraph_erdos_renyi_game_gnp

https://igraph.org/c/doc/igraph-Generators.html#igraph_erdos_renyi_game_gnm
igraph_erdos_renyi_game_gnm

Numbers depend on the machine.


The above seem the only libraries explicitly providing these two models of
random graphs.

https://www.boost.org/doc/libs/1_85_0/libs/graph/doc/random.html
has some tools, and generate_random_graph provides G(n,m)-graphs, but no
details are given.

graph-tool has many network-models, but does not seem to provide
the two basic models (G(n,p), G(n,m)).


TODOS:

-1. Provide output-stream-handling (as in LSRG.cpp).

0. Provide comment-output-handling.
  - Expand on current comment-output by the usual standard.
  - See TOOOS in GraphConversion.cpp and Graphs.hpp.
  - Possibilities: disabling comments, and comments-only.

1. Provide handling of directed graphs.
  - See TODOS in RandomGraphs.hpp.

2. Connect to LSRG.cpp and BRG.cpp.
  - LSRG.cpp is newer (together with LSRG.hpp); we also do here GRRG.hpp.

3. For our applications we are especially interested in connected graphs.
    - We could just consider general random graphs, and split them into
      components, handling each component (and adding up the effort).
    - Seems good enough here; perhaps the generator should then create
      already the components?
    - BCCbySAT should handle components (see below), however it would be
      handy if the generator would already create the components (in a
      directory) as an option.

*/



#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include "GRRG.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "10.6.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/GRGG.cpp",
        "GPL v3"};

  using namespace RandomGraphs;
  using namespace GRRG;

  namespace RG = RandGen;
  namespace GR = Graphs;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " [+]graph-type n m|p seeds format [output]\n\n"
    " graph-type : " << Environment::WRP<GR::GT>{} << "; \"+\" means"
    " with loops\n"
    " n          : number of vertices (<= 4294967295 for G(n,m))\n"
    " m|p        : number m of edges or probability p = \"num/denom\"\n"
    " seeds      : "; RG::explanation_seeds(std::cout, 12);
    std::cout <<
    " format     : " << Environment::WRP<GR::GrFo>{} << "\n"
    " output    : \"-cout\" (standard output) or \"\"[-]\"\""
    " (default filename) or \"FILENAME\"\n\n"
    " prints a pseudo-random graph G(n,m) resp. G(n,p) to standard output:\n\n"
    " - Arguments \"\" (the empty string) yield the default-values,\n"
    "    except for the output, where it yields the default output-filename.\n"
    " - Default-values for the options are the first possibilities given.\n"
    " - Numbers are unsigned 64-bit integers, however the input allows to\n"
    "   give them as 80-floating-point values (which will be rounded).\n"
    " - The optional \"-\" for the default-filename means \"don't print"
    " filename\" (done otherwise).\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 6 and argc != 7) {
    std::cerr << error <<
      "Five or six arguments (type, n, m|p, seeds, format[, output])"
      " needed, but " << argc-1 << " provided.\n";
    return 1;
  }

  const auto [type, with_loops] = read_type_arg(argv[1], error);
  if (type == GR::GT::dir) {
    std::cerr << error << "Directed graphs not yet handled.\n";
    return 1;
  }
  const UInt_t n{FloatingPoint::toUInt(argv[2])};
  const auto [m, p, uniform_model] = read_mp_arg(argv[3], error);
  const auto [seeds, basic_size] =
    all_seeds(type, with_loops, n, m, p, uniform_model, argv[4]);
  const GR::GrFo format = read_format_arg(argv[5], error);

  output_seeds(std::cout, format, seeds);

  if (uniform_model) {
    if (not GR::Sizes::allops(n)) {
      std::cerr << error << "n=" << n << " > " << GR::Sizes::max_n_allops
                << "\n";
      return 1;
    }
    if (m > GR::Sizes::max_m(n, type, with_loops)) {
      std::cerr << error << "m=" << m << " asks for more edges than"
        " the graph possibly can have.\n";
      return 1;
    }
    RandGen::RandGen_t g(seeds);
    const auto G = uniform_rgr(n, m, g, not with_loops);
    G.format(format);
    std::cout << G;
  }
  else {
    RandGen::RandGen_t g(seeds);
    const auto G = binomial_rgr(n, p, g, not with_loops);
    G.format(format);
    std::cout << G;
  }
}
