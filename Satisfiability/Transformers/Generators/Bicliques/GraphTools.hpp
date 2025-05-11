// Oliver Kullmann, 10.4.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Tools for using external graph-tools

    Constants:
     - default_redumis_seed : int
     - default_redumis_timeout : double


    Functor-class Redumis_call for computing large independent sets
    of a graph G (deployed in MaxIndependentRedumis.cpp):

      Data-members:
       - const reference G to graph_type = AdjVecUInt
       - int seeed, double timeout
       - string path_use_redumis (path to the wrapper for redumis)
       - string additional_options

      Via member-function
        command() -> string
      one obtains the string for the system-(like-)command.

      Constructors:
       - Redumis_call(G)
       - Redumis_call(G,seed,timeout,path,options)
       - Redumis_call(G,seed,timeout,path).
      If the path-argument is empty, then
         default_path_use_redumis = "use_redumis"
      is used.

      Operator ()() -> vertex_list
        calls use_redumis (via a pipe to standard input), and returns
        an independent set/list (empty in case of any error).

      With
        check_independence(vertex_list) -> bool
      one can check the result.


    Functor-class BC_incomp_by_redumis computes a large biclique-incompatible
    set of edge-indices for a graph G and its edge-list E:

     - Constructed with G, E, a Redumis_call-object RC is created, holding
       the biclique-compatibility-graph (stored as BG).
     - The function-call-operator then simply returns RC().

TODOS:

-1. New general facility to employ the new standardised facilities
  - Currently called
      (vc|mi)std_(fastvc|redumis).bash
  - Perhaps the "std" should be at the front, for easier searching?
    Perhaps "callgraphsolver" ?
  - Perhaps "vc" resp. "mi" should be an argument?
    Should this have a default? "vc" for fastvc, "mi" for redumis?
  - There needs to be some mechanism to associate the graph-format with
    the solver.

0. For redumis and fastvc always supply the 4 statistics:

   UPDATE: we have now two additional numbers, "n" and "c", say --
   number of vertices/variables, number of edges/clauses.
   And the basic statistics are 5 numbers: result, 3xtimes, memory.
   Perhaps all numbers are float80, with NaN indicating error,
   and -inf indicating NA ? String-input/output would be ERR and NA.
   For SAT-solvers the ordinary result has 0,1,2 (not 3 anymore) --
   2 says computation was not succesful (different from -inf, which means
   no computation took place).
   This is the minimal statistics.
   An extension then provides solver-information (as below).
   All such types to new module SolverBasics.hpp.

  - Providing a standard structure Usrbintimestats.
    UPDATE: perhaps MinSolverResult ?
  - The return-value is then a pair of vertex_list and Usrbintimestats.
  - To activate for the underlying use-tools, add "statistics=1".
    UPDATE: the standardised underlying tools always provide minimal
    statistics.
  - With redumis, it is the first line of output, with fastvc the second.
    DONE with the new tools.
  - Perhaps adding two fields, "result-status" and "option", both unsigned
    integers, which for SAT have values {0,1,2,3} for the status, while for
    these graph-solvers we could have
     - 0 : ordinary partial computation (bound)
     - 1 : exact value
     - 2 : error.
    UPDATE: Currently for an incomplete solver actually to output
    a precise result (like "this vertex cover is optimal") can't be
    expressed with the six values.
    But this information should be part of the solver-information.
  - Perhaps then also data about the type of solver, so that better
    output can be produced?
  - For such a complete (minimal) result-object, also the version would
    be good to have; likely best as string.
  - The solver-description would have four entries:
   - an enumerated value for the solver-type (SAT, MAXSAT, MVC, MIS)
   - an integer-code for the solver (w.r.t. the underlying enumeration)
   - an unsigned integer for the option
   - a string for the version.
    UPDATE: perhaps also some entry about the exactness-mode.
  - For the optimising solvers, getting the result (unsigned integer) of
    the optimisation possibly in this object would also be good?
    UPDATE: yes, the result is part of the minimal statistics.
  - Or does this better belong to a larger object, containing the minimal
    data plus the additional results (satisfying assignments, optimal value
    etc.)?
  - The value found could be used instead of the status, which then should
    be unsigned 64-bits?
    UPDATE: float80 seems better.

1. Functor class BC_incomp_by_redumis:
  - Having the biclique-compatability-graph stored in BG can be useful for
    inspection, but we also need a version, which does not store this graph,
    but pipes it directly to standard-input of use_redumis.

*/

#ifndef GRAPHTOOLS_UOmMwSdFsy
#define GRAPHTOOLS_UOmMwSdFsy

#include <string>
#include <sstream>
#include <utility>
#include <algorithm>

#include <cassert>
#include <cstdio> // for std::FILE, std::fputs
#include <cinttypes> // for PRIu64

#include <SystemSpecifics/SystemCalls.hpp>
#include <Numerics/NumInOut.hpp>

#include "Graphs.hpp"
#include "Bicliques.hpp"
#include "SolverBasics.hpp"

namespace GraphTools {

  /*
    Specialised tools for certain external solvers
  */

  constexpr int default_redumis_seed = 0;
  constexpr double default_redumis_timeout = 10;
  const std::string default_path_use_redumis = "use_redumis.bash";


  struct Redumis_call {
    typedef Graphs::AdjVecUInt graph_type;
    typedef Graphs::AdjVecUInt::list_t vertex_list;
    typedef graph_type::id_t id_t;

    const graph_type& G;

    int seed = default_redumis_seed;
    double timeout = default_redumis_timeout;
    std::string path_use_redumis = default_path_use_redumis;
    std::string additional_options = "--disable_checks";

    std::string command() const {
      std::stringstream out;
      out << "timelimit=" << timeout << " " << path_use_redumis
          << " --seed=" << seed << " " << additional_options;
      return out.str();
    }

    Redumis_call(const graph_type& G) noexcept : G(G) {}
    Redumis_call(const graph_type& G,
                 const int seed, const double timeout,
                 std::string path, std::string options) noexcept :
    G(G), seed(seed), timeout(timeout),
    path_use_redumis(path.empty()?default_path_use_redumis:std::move(path)),
    additional_options(std::move(options)) {}
    Redumis_call(const graph_type& G,
                 const int seed, const double timeout,
                 std::string path) noexcept :
    G(G), seed(seed), timeout(timeout),
    path_use_redumis(path.empty()?default_path_use_redumis:std::move(path)) {}

    bool check_independence(const vertex_list& V) const noexcept {
      return std::ranges::is_sorted(V) and Graphs::is_independent(V, G);
    }

    // Returning the empty list in case of error:
    vertex_list operator()() const {
      const std::string com = command();
      SystemCalls::Popen po(com);
      struct handle_format { // set and reset the graph-format
        const Graphs::AdjVecUInt& G;
        const Graphs::GrFo old;
        handle_format(const Graphs::AdjVecUInt& G) noexcept
        : G(G), old(G.format()) { G.format(Graphs::GrFo::metis); }
        ~handle_format() noexcept { G.format(old); }
      };
      const handle_format H(G);
      const auto res = po.etransfer(Graphs::AdjVecUIntref_put(G));
      if (res.rv.error() or not res.err.empty()) return {};
      vertex_list ind = FloatingPoint::to_vec_unsigned<id_t>(res.out, ' ');
      for (id_t& v : ind)
        if (v == 0 or v > G.n()) return {};
        else --v;
      return ind;
    }

  };

  struct BC_incomp_by_redumis {
    using graph_type = Redumis_call::graph_type;
    using vertex_list = Redumis_call::vertex_list;
    typedef Graphs::AdjVecUInt::vecedges_t vecedges_t;

    const graph_type BG;
    const Redumis_call RC;

    static graph_type produce_BG(const graph_type& G, const vecedges_t& E) {
      return Bicliques::bccomp_graph<Bicliques::best_version_bccomp>(G,E,"");
    }

    BC_incomp_by_redumis(const graph_type& G, const vecedges_t& E) :
      BG(produce_BG(G,E)), RC(BG) {}
    BC_incomp_by_redumis(const graph_type& G, const vecedges_t& E,
                         const int seed, const double timeout,
                         std::string path, std::string options) :
      BG(produce_BG(G,E)), RC(BG,seed,timeout,path,options) {}
    BC_incomp_by_redumis(const graph_type& G, const vecedges_t& E,
                         const int seed, const double timeout,
                         std::string path) :
      BG(produce_BG(G,E)), RC(BG,seed,timeout,path) {}

    vertex_list operator()() const { return RC(); }
  };


  constexpr unsigned default_fastvc_seed = 0;
  constexpr double default_fastvc_timeout = 10;
  const std::string default_path_use_fastvc = "use_fastvc.bash";


  struct FastVC_call {
    typedef Graphs::AdjVecUInt graph_type;
    typedef Graphs::AdjVecUInt::list_t vertex_list;
    typedef graph_type::id_t id_t;

    const graph_type& G;

    unsigned seed = default_fastvc_seed;
    double timeout = default_fastvc_timeout;
    std::string path_use_fastvc = default_path_use_fastvc;

    std::string command() const {
      std::stringstream out;
      out << "timelimit=" << timeout << " " << "seed=" << seed
          << " " << path_use_fastvc;
      return out.str();
    }

    FastVC_call(const graph_type& G) noexcept : G(G) {}
    FastVC_call(const graph_type& G,
                const unsigned seed, const double timeout,
                std::string path) noexcept :
    G(G), seed(seed), timeout(timeout),
    path_use_fastvc(path.empty()?default_path_use_fastvc:std::move(path)) {}

    bool check_vertexcover(const vertex_list& V) const noexcept {
      return std::ranges::is_sorted(V) and Graphs::is_vertexcover(V, G);
    }

    // Returning the empty list in case of error:
    vertex_list operator()() const {
      const std::string com = command();
      SystemCalls::Popen po(com);
      struct handle_format { // set and reset the graph-format
        const Graphs::AdjVecUInt& G;
        const Graphs::GrFo old;
        handle_format(const Graphs::AdjVecUInt& G) noexcept
        : G(G), old(G.format()) { G.format(Graphs::GrFo::dimacs); }
        ~handle_format() noexcept { G.format(old); }
      };
      const handle_format H(G);
      const auto res = po.etransfer(Graphs::AdjVecUIntref_put(G));
      if (res.rv.error() or not res.err.empty()) return {};
      vertex_list vc = FloatingPoint::to_vec_unsigned<id_t>(res.out, ' ');
      for (id_t& v : vc)
        if (v == 0 or v > G.n()) return {};
        else --v;
      return vc;
    }
  };

}

#endif
