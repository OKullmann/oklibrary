// Oliver Kullmann, 14.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Pseudo-random generation of graphs


DESIGN RATIONAL:

1. Using a high-quality pseudo-random engine, the 64-bit Mersenne Twister
   (from the C++ standard library).

2. Allows arbitrarily long sequences of user-seeds (unsigned 64 bits, which
   can be given, or created as timestamps or truly random values). Not just
   the usual 32-bit or maximally 64-bit seeds.

3. Seed-management makes automatically sure that every change of essential
   parameters results in a different random sequence (guaranteeing diversity).
   This is done by incorporating these parameters into the initial library-
   given part of the seed-sequence.
   With every other generator all over the world again and again the same
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

7. Supports different output-formats.

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


The above seem the only libraries explicitly providing these two random graphs.

https://www.boost.org/doc/libs/1_85_0/libs/graph/doc/random.html
has some tools, and generate_random_graph provides G(n,m)-graphs, but no
details given.

graph-tool has many network-models, but does not seem to provide
the two basic models (G(n,p), G(n,m)).


TODOS:

1. Connect to LSRG.cpp and BRG.cpp.
  - LSRG.cpp is newer (together with LSRG.hpp); we also do here GRRG.hpp.

2.
   - Arguments:
         type n m/p seeds format [output]
    - DONE type: undirected/directed, without/with loops;
      that is: allowing "+" in front of Graphs::GT (for "with-loops").
    - DONE n: number of vertices
    - DONE m/p: either m (number of edges) or p (as quotient, with ","), the
      probability of an edge.
    - format: 2 enumerations: graph-output, +-com and com-only
    - DONE output: "-cout" (standard output) or ""[-]"" (default filename) or
      "FILENAME"
   - Seed-management shall be synchronised with BRG/LSRG; so the parameters
     become automatically part of the seed.
   - For our applications we are especially interested in connected graphs.
    - We could just consider general random graphs, and split them into
      components, handling each component (and adding up the effort).
    - Seems good enough here; perhaps the generator should then create
      already the components?
    - BCCbySAT should handle components (see below), however it would be
      handy if the generator would already create the components (in a
      directory) as an option.

---

*/



#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include "GRRG.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.6",
        "15.5.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/GRGG.cpp",
        "GPL v3"};

  using namespace RandomGraphs;

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
    " n          : number of vertices (<= 4294967295 for G(n,m)) \n"
    " m|p        : number m of clauses or probability p = \"num,denom\"\n"
    " seeds      : "; RG::explanation_seeds(std::cout, 12);
    std::cout <<
    " format     : " << Environment::WRP<GR::GrFo>{} << " XXX \n"
    " output    : \"-cout\" (standard output) or \"\"[-]\"\""
    " (default filename) or \"FILENAME\"\n\n"
    " prints a pseudo-random graph G(n,m) resp. G(n,p) to standard output:\n\n"
    " - Arguments \"\" (the empty string) yield the default-values,\n"
    "    except for the output, where it yields the default output-filename.\n"
    " - Default-values for the options are the first possibilities given.\n"
    " - The optional \"-\" for the default-filename means \"don't print"
    " filename\" (done otherwise).\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

}
