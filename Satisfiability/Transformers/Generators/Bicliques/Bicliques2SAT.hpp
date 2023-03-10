// Oliver Kullmann, 22.2.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Translating biclique-related problems to SAT

   - var_t
   - MaxN

   - class Param:
       aggregate with variables V, E, B (vertices, edges, bicliques)
       plus constant MaxV
   - valid(Param)

   - class VarEncoding:

    - constructor VarEncoding(graph_t G, B) (undirected graphs without loops)
    - typedefs graph_t, Lit, Bcc_frame
    - public constants G, V, E, lf (Lit_filter for biclique-variables)
    - access-function to constants:
     - B()
     - n() (total number of variables; n() = nb() + ne())
     - nb() (number of variables for bicliques)
     - ne() (number of variables for edges)
    - update_B(newB)

    - encoding-functions (yielding variables):
     - left(v, b) -> var_t
     - right(v, b) -> var_t
     - edge(e, b) -> var_t
    - the first block of variables is given by left and right, the second by
      edge;

    - decoding (obtaining the resulting biclique-cover):
     - aggregate struct elem, specifying an element of a biclique,
       with elements b (index of biclique), boolean left, v (variable);
     - inv(v) -> elem (for variables in the first block)
       (the auxiliary variables don't need to be decoded)
     - for creating the biclique-cover:
      - add_literal(Lit x, Bcc_frame& bcc) adds the literal x (with a variable
        from the first block, and positive x) to the given biclique-frame
      - extract_bcc(std::istream& in) -> Bcc_frame
        call first core_extraction(in), and then post_process on the result
       - core_extraction(std::istream&) -> Bcc_frame
         reads a Dimacs-assignment, populates bicliques according to the
         positive biclique-literals, and performs trivial trimming and sorting
       - post_process(Bcc_frame&) -> Bcc_frame checks the frame and returns it
      - the overload extract_bcc(vector<LIT>) takes the vector of the literals
        (only the relevant ones, the positive biclique-literals)
        using the overloaded core_extraction(vector<LIT>)
      - the more general range-versions rextract_bcc and core_rextraction
        have a template-parameter for the range (generalising the vector)

   SAT translation:

    - scoped enums (all registered with RegistrationPolicies):
     - SB (symmetry-breaking forms)
     - DC (Dimacs-comments on/off)
     - DP (Dimacs-parameters on/off)
     - CS (clause-set-output on/off)

    - input and output control:
     - const char sep
     - typedefs alg_options_t, format_options_t
     - constant default_sb_rounds

    - scoped enum ResultType

    - class BC2SAT:

     - typedefs
      - graph_t
      - enc_t = VarEncoding
      - id_t (64-bit unsigned int, used for vertices, edges, bicliques)
      - vei_t (vector of id_t -- vector of edge-indices)
        function output(vei_t, std::ostream) outputs the list of edges (with
        their names in G) as a list of items "(v,w)" (without separation)
      - stats_t (basic statistics for a sequence of float80)
      - symmbreak_res_t (pair of vei_t and stats_t)
      - imported Var, Lit, Clause, ClauseList from RandGen
     - public constants G, edges (all edges of G)
     - access-functions to data:
      - enc() for the enc_t-object (const-reference)
      - update_b(newB) (must be smaller than previous B)
     - constructor BC2SAT(graph_t G, id_t B)
       stores G, saves all-edges in edges, creates enc_t-object from G, B

     - symmetry-breaking:
      - max_bcincomp(RandGen) -> vei_t : compute greedily a maximal sequence
        of bc-incompatible edges (using a random generator)
      - max_bcincomp(id_t rounds, RandGen) -> symmbreak_res_t
        (repeat rounds-often, and return best result with statistics)

     - clause-translations:

      - nonedge_for_bc(v,w,b) -> ClauseList (edge {v,w} must not be covered)
      - all_nonedges_for_bcs(std::ostream& out) -> id_t : all non-edges
        must not be covered, output to out (returned the number of clauses)
      - num_cl_bcedges() is the number of clauses here

      - edge_def(e,b) -> ClauseList
      - all_edges_def(std::ostream& out) -> id_t
      - num_cl_defedges()

      - edge_cov(e) -> ClauseList
      - all_edges_cov(std::ostream& out) -> id_t
      - num_cl_covedges()

      - all_basic_clauses(std::ostream& out) -> id_t : the above three blocks
      - num_basic_cl()

      - place_edge(e,b) -> ClauseList
      - all_sbedges(vei_t sb, std::ostream& out) -> id_t : places all
        edges from sb (symmetry-breaking)
      - num_cl_sb(vei_t)

      - all_clauses(vei_t sb, std::ostream& out) -> id_t : combines
        all_basic_clauses with all_sbedges
      - num_cl(vei_t)

      - the complete translation (with complete output):
       - struct Unsatisfiable (thrown when B was too low to start with
         (from above))
       - operator (ostream&, alg_options, format_options, rounds, seeds)
         -> dimacs_pars (throws Unsatisfiable)

     - the complete SAT-solving (from above), computing an optimum
       bcc:
      - struct result_t (Bcc_frame, B, ResultType, init_B)
      - operator (ostream*, alg_options, rounds, sec, seeds)
        stores the Dimacs-files on disc, using the minisat_call, decrementing
        B until unsatisfiability reached



   - General helper functions
    - erase_if_byswap(vec, pred) : faster than std::erase_if due to
      not keeping the order.


TODOS:

0. Upwards-search

1. UCP for BC2SAT

(a) The class gets bloated, but for a start it seems easiest to develop
    the functionality in this (richer) context.

(b) Given and initial edge-placement via sb of type vei_t and an initial
    consistent bcc-frame F (either empty or obtained by complete previous
    processing), place the edges of sb in F, and perform complete UCP for F.

(c) A bcc-frame F contains for each 0 <= i < B:
  - the assigned vertices L, R;
    perhaps as sorted vectors; both are empty or both are non-empty;
  - the (other) possible vertices PL, PR;
    as long as L,R are empty, also PL, PR are empty, and then with the first
    edge PL, PR are assigned the other neighbours;
    with further edges only the common neighbours remain;
    this data only shrinks (after the initial setting), so perhaps also a
    sorted vector is best;
  - the edges watching this bc, as pairs (i,e), with 0 <= i < E and e in {0,1},
    for the direction (forward/backward).

(d) For each edge-index 0 <= i < E:
     - indication whether assigned to which biclique (with which polarity)
     - if unassigned, then two watched bicliques (with polarity; which
       can cover the edge)
    The two watched bicliques in "ladder- order" (updating advances
    in this order).

2. Implement partition1:
  - This linear form is available under LatinSquares.
  - At this time then the structure of BC2SAT needs to be improved.

*/

#ifndef BICLIQUES2SAT_MI3iJYZoB5
#define BICLIQUES2SAT_MI3iJYZoB5

#include <limits>
#include <exception>
#include <string>
#include <vector>
#include <ostream>
#include <utility>
#include <tuple>
#include <istream>
#include <functional>
#include <fstream>
#include <filesystem>
#include <type_traits>

#include <cstdint>
#include <cassert>
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumTypes.hpp>
#include <Numerics/Statistics.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/Algorithms.hpp>
#include <Transformers/Generators/Random/ClauseSets.hpp>

#include "Graphs.hpp"
#include "Bicliques.hpp"
#include "DimacsTools.hpp"

namespace Bicliques2SAT {

  typedef std::uint64_t var_t;
  constexpr var_t MaxN = std::numeric_limits<var_t>::max() - 1;


  struct Param {
    var_t V; // number of vertices
    var_t E; // number of edges
    var_t B; // number of bicliques

    static constexpr var_t MaxV =
      std::numeric_limits<std::uint32_t>::max() - 1;
  };
  /*
    A parameter-tripe (V,E,B) is valid iff
     - V <= MaxV
     - E <= maximal number of edges for V
     - B allows for valid var-encoding.
  */
  constexpr bool valid(const Param p) noexcept {
    if (p.V > Param::MaxV) return false;
    const var_t maxE = (p.V * (p.V - 1)) / 2;
    if (p.E > maxE) return false;
    if (p.B == 0) return true;
    const var_t bound = MaxN / 2 / p.B;
    if (p.V > bound / 2) return false; // see numvarbic below
    if (p.E > bound) return false; // see numvaredg below
    return true;
  }
  static_assert(valid(Param{}));


  /*
    Encoding and decoding (the meaning of the variables)

    The parameters for encoding are
      V, E, B,
    while the graph G plays a role only for the enhanced decoding of a
    solution (with basic simplifications/reductions).

    The first block of variables is
     1, ..., nb() = 2*V*B,
    which encodes the B bicliques, with each biclique given by the left and
    the right subset of the set of vertices.

    The second block of variables is
      nb() + 1, ..., nb()+ne(),
      ne() = E*B,
     which encodes for every edge the bicliques to which it belongs.

     Encoding (into variables) happens by
       left(v, b), right(v, b), edge(e, b).

     Decoding (into a Bcc_frame) only needs the positive literals from the
     first block, and happens by
      - extract_bcc(std::istream) (filtering out the relevant literals)
      - extract_bcc(vector<LIT>) (the partial assignments contains only the
                                  relevant literals)
      - more general for a range: rextract_bcc(RAN).
  */
  struct VarEncoding {
    typedef Graphs::AdjVecUInt graph_t;
    const graph_t& G;

    const var_t V, E; // vertices are numbered 0 <= v < V,
      // edge-indices are < E (edges are pairs of vertices)

    typedef DimacsTools::Lit Lit;
    const DimacsTools::Lit_filter lf; // true iff the underlying variable is
      // a biclique-variable and the literal is positive

    explicit VarEncoding(const graph_t& G, const var_t B)
      : G(G), V(G.n()), E(G.m()),
        lf([this](const Lit x){return x.s and x.v.v <= nb_;}),
        B_(B), nb_(numvarbic(V,B_)), ne_(numvaredg(E,B_)), n_(nb_+ne_) {
      if (G.type() != Graphs::GT::und)
        throw std::domain_error("ERROR[VarEncoding]: only undirected graphs");
      if (not valid(Param{V,E,B_}))
        throw std::domain_error("ERROR[VarEncoding]: parameters"
                                " V=" + std::to_string(V) +
                                " E=" + std::to_string(E) +
                                " B=" + std::to_string(B_));
      assert(not has_loops(G));
      assert(nb_ <= MaxN and ne_ <= MaxN and n_ <= MaxN);
    }

    void update_B(const var_t newB) noexcept {
      assert(B_ == 0 or newB < B_);
      B_ = newB;
      nb_ = numvarbic(V,B_); ne_ = numvaredg(E,B_); n_ = nb_ + ne_;
    }

    var_t B() const noexcept { return B_; }
    var_t n() const noexcept { return n_; }
    var_t nb() const noexcept { return nb_; }
    var_t ne() const noexcept { return ne_; }

    static constexpr var_t numvarbic(const var_t V, const var_t B) noexcept {
      return 2*V*B;
    }
    static constexpr var_t numvaredg(const var_t E, const var_t B) noexcept {
      return E*B;
    }

    typedef graph_t::id_t id_t;
    var_t left(const id_t v, const id_t b) const noexcept {
      assert(v < V);
      assert(b < B_);
      return 1 + b*2*V + v;
    }
    var_t right(const id_t v, const id_t b) const noexcept {
      assert(v < V);
      assert(b < B_);
      return 1 + b*2*V + V + v;
    }
    var_t edge(const id_t e, const id_t b) const noexcept {
      assert(e < E);
      assert(b < B_);
      return 1 + nb_ + b * E + e;
    }

    struct elem {
      const id_t b;
      const bool left;
      const id_t v;
      constexpr bool operator ==(const elem& rhs) const noexcept = default;
    };
    static_assert(std::is_aggregate_v<elem>);
    constexpr elem inv(const var_t v) const noexcept {
      assert(1 <= v and v <= nb_);
      const auto [b,r] = std::lldiv(v-1, 2*V);
      assert(r >= 0 and b >= 0);
      if (id_t(r) < V) return {id_t(b),true,id_t(r)};
      else return {id_t(b),false,id_t(r)-V};
    }
    typedef Bicliques::Bcc_frame Bcc_frame;
    void add_literal(const Lit x, Bcc_frame& bcc) const noexcept {
      assert(lf(x));
      const elem el = inv(x.v.v);
      if (el.left) bcc.L[el.b].l.push_back(el.v);
      else bcc.L[el.b].r.push_back(el.v);
    }
    Bcc_frame core_extraction(std::istream& in) const {
      assert(in);
      Bcc_frame res(B_);
      for (Lit x; (x=DimacsTools::read_strict_literal(in)).v.v != 0;)
        if (lf(x)) add_literal(x, res);
      assert(in);
      triv_trim(res);
      sort(res);
      return res;
    }
    Bcc_frame post_process(Bcc_frame& res) const noexcept {
      assert(valid(res,G));
      assert(disjoint(res));
      assert(is_bcc(res,G));
      return res;
    }
    Bcc_frame extract_bcc(std::istream& in) const {
      Bcc_frame res = core_extraction(in);
      return post_process(res);
    }
    template <class RAN>
    Bcc_frame core_rextraction(const RAN& pa) const {
      Bcc_frame res(B_);
      for (const Lit x : pa) { assert(lf(x)); add_literal(x, res); }
      triv_trim(res);
      sort(res);
      return res;
    }
    template <class RAN>
    Bcc_frame rextract_bcc(const RAN& pa) const {
      Bcc_frame res = core_rextraction(pa);
      return post_process(res);
    }
    Bcc_frame core_extraction(const std::vector<Lit>& pa) const {
      return core_rextraction(pa);
    }
    Bcc_frame extract_bcc(const std::vector<Lit>& pa) const {
      return rextract_bcc(pa);
    }

  private :

    var_t B_; // bicliques 0 <= b < B
    var_t nb_, ne_; // number of variables for bicliques and edges
    var_t n_;

  };


  enum class SB { basic=0, extended=1, none=2 }; // symmetry-breaking
  enum class PT { cover=0, partition1=1, partition2=2 }; // problem type
  enum class DC { with=0, without=1 }; // Dimacs-comments
  enum class DP { with=0, without=1 }; // Dimacs-parameters
  enum class CS { with=0, without=1 }; // clause-set
  enum class DI { upwards=0, downwards=1 }; // search direction
  enum class UB { check=0, trust=1 }; // upper-bound: check or trust

  constexpr char sep = ',';
  typedef std::tuple<SB,PT> alg_options_t;
  typedef std::tuple<SB,PT,DI,UB> alg2_options_t;
  typedef std::tuple<DC,DP,CS> format_options_t;

  constexpr id_t default_sb_rounds = 100;
}
namespace Environment {
  template <>
  struct RegistrationPolicies<Bicliques2SAT::SB> {
    static constexpr int size = int(Bicliques2SAT::SB::none)+1;
    static constexpr std::array<const char*, size> string
    {"+sb", "++sb", "-sb"};
  };
  template <>
  struct RegistrationPolicies<Bicliques2SAT::PT> {
    static constexpr int size = int(Bicliques2SAT::PT::partition2)+1;
    static constexpr std::array<const char*, size> string
    {"cover", "partition1", "partition2"};
  };
  template <>
  struct RegistrationPolicies<Bicliques2SAT::DC> {
    static constexpr int size = int(Bicliques2SAT::DC::without)+1;
    static constexpr std::array<const char*, size> string
    {"+dc", "-dc"};
  };
  template <>
  struct RegistrationPolicies<Bicliques2SAT::DP> {
    static constexpr int size = int(Bicliques2SAT::DP::without)+1;
    static constexpr std::array<const char*, size> string
    {"+dp", "-dp"};
  };
  template <>
  struct RegistrationPolicies<Bicliques2SAT::CS> {
    static constexpr int size = int(Bicliques2SAT::CS::without)+1;
    static constexpr std::array<const char*, size> string
    {"+cs", "-cs"};
  };
  template <>
  struct RegistrationPolicies<Bicliques2SAT::DI> {
    static constexpr int size = int(Bicliques2SAT::DI::downwards)+1;
    static constexpr std::array<const char*, size> string
    {"up", "down"};
  };
  template <>
  struct RegistrationPolicies<Bicliques2SAT::UB> {
    static constexpr int size = int(Bicliques2SAT::UB::trust)+1;
    static constexpr std::array<const char*, size> string
    {"check", "trust"};
  };
}
namespace Bicliques2SAT {
  std::ostream& operator <<(std::ostream& out, const SB s) {
    switch (s) {
    case SB::basic : return out << "basic-sb";
    case SB::extended : return out << "extended-sb";
    case SB::none : return out << "no-sb";
    default : return out << "SB::UNKNOWN";}
  }
  std::ostream& operator <<(std::ostream& out, const PT s) {
    switch (s) {
    case PT::cover : return out << "cover";
    case PT::partition1 : return out << "partition-linear";
    case PT::partition2 : return out << "partition-quadratic";
    default : return out << "PT::UNKNOWN";}
  }
  std::ostream& operator <<(std::ostream& out, const DC s) {
    switch (s) {
    case DC::with : return out << "with-comments";
    case DC::without : return out << "without-comments";
    default : return out << "DC::UNKNOWN";}
  }
  std::ostream& operator <<(std::ostream& out, const DP s) {
    switch (s) {
    case DP::with : return out << "with-parameters";
    case DP::without : return out << "without-parameters";
    default : return out << "DP::UNKNOWN";}
  }
  std::ostream& operator <<(std::ostream& out, const CS s) {
    switch (s) {
    case CS::with : return out << "with-cs";
    case CS::without : return out << "without-cs";
    default : return out << "CS::UNKNOWN";}
  }
  std::ostream& operator <<(std::ostream& out, const DI d) {
    switch (d) {
    case DI::upwards : return out << "upwards";
    case DI::downwards : return out << "downwards";
    default : return out << "DI::UNKNOWN";}
  }
  std::ostream& operator <<(std::ostream& out, const UB u) {
    switch (u) {
    case UB::check : return out << "check-ub";
    case UB::trust : return out << "trust-ub";
    default : return out << "UB::UNKNOWN";}
  }


  template <class T, class Alloc, class Pred>
  constexpr typename std::vector<T,Alloc>::size_type erase_if_byswap
      (std::vector<T,Alloc>& v, const Pred pred) noexcept {
    const auto end = v.end();
    const auto it = std::partition(v.begin(), end, std::not_fn(pred));
    const auto r = std::distance(it, end);
    v.erase(it, end);
    return r;
  }


  enum class ResultType {
    unknown = 0,       // default-value
    init_unsat_sb = 1, // symmetry-breaking showed unsatisfiability
    init_unsat = 2,    // initial value of B is unsatisfiable
    init_timeout = 3,  // initial value of B yielded solver-timeout
    final_timeout = 4, // non-initial value of B yielded solver-timeout
    aborted = 5,       // solver aborted computation
    exact = 6,         // exact result obtained
  };
  std::ostream& operator <<(std::ostream& out, const ResultType r) {
    switch(r) {
    case ResultType::unknown : return out << "initialy-unknown";
    case ResultType::init_unsat_sb : return out << "immediately-unsat-by-sb";
    case ResultType::init_unsat : return out << "immediately-unsat";
    case ResultType::init_timeout : return out << "immediately-timeout";
    case ResultType::final_timeout : return out << "timeout";
    case ResultType::aborted : return out << "aborted";
    case ResultType::exact : return out << "exact";
    default : return out << "ResultType::UNKNOWN";}
  }


  struct BC2SAT {
    typedef Graphs::AdjVecUInt graph_t;
    const graph_t& G;
    const graph_t::vecedges_t edges; // vector of pairs of id_t's

    typedef VarEncoding enc_t;
    typedef VarEncoding::id_t id_t;
    static_assert(std::is_same_v<id_t, graph_t::id_t>);

    explicit BC2SAT(const graph_t& G, const id_t B) noexcept :
      G(G), edges(G.alledges()), enc_(G,B) {}


    const enc_t& enc() const noexcept { return enc_; }
    void update_B(const var_t newB) noexcept {
      assert(newB < enc_.B());
      enc_.update_B(newB);
    }


    // Compute a (single) random maximal bc-incompatible sequence of edges
    // (every pair is incompatible), given by their indices, by repeatedly
    // chosing a random edge, and then removing all bc-compatible edges,
    // until no edge is left:
    typedef std::vector<id_t> vei_t; // vector of edge-indices
    vei_t max_bcincomp(RandGen::RandGen_t& g) const {
      vei_t avail; avail.reserve(enc_.E);
      for (id_t i = 0; i < enc_.E; ++i) avail.push_back(i);
      RandGen::shuffle(avail.begin(), avail.end(), g);
      vei_t res;
      while (not avail.empty()) {
        const id_t e = avail.back(); avail.pop_back();
        res.push_back(e);
        erase_if_byswap(avail,
          [e,this](const id_t x){return
                                 Bicliques::bccomp(edges[e],edges[x], G);});
      }
      return res;
    }
    typedef GenStats::BasicStats<id_t, FloatingPoint::float80> stats_t;
    typedef std::pair<vei_t, stats_t> symmbreak_res_t;
    // Now repeat rounds-often, and return the first best, with statistics:
    symmbreak_res_t max_bcincomp(const id_t rounds,
                                 RandGen::RandGen_t& g) const {
      if (rounds == 0) return {};
      symmbreak_res_t res;
      res.first = max_bcincomp(g);
      res.second += res.first.size();
      for (id_t i = 0; i < rounds-1; ++i) {
        vei_t nres = max_bcincomp(g); const auto s = nres.size();
        res.second += s; if (s > res.first.size()) res.first = std::move(nres);
      }
      return res;
    }
    void output(const vei_t& v, std::ostream& out) const {
      for (const id_t x : v) {
        assert(x < enc_.E);
        const auto [a,b] = edges[x];
        out << "(" << G.name(a) << "," << G.name(b) << ")";
      }
    }


    typedef RandGen::Var Var;
    typedef RandGen::Lit Lit;
    typedef RandGen::Clause Clause;
    typedef RandGen::ClauseList ClauseList;

    // Forbid the edge {v,w} to be covered by biclique b
    // (including the case v, w):
    ClauseList nonedge_for_bc(const id_t v, const id_t w, const id_t b)
      const noexcept {
      assert(v < enc_.V and w < enc_.V and v <= w and b < enc_.B());
      if (v == w) {
        return {{Lit{enc_.left(v,b),-1}, Lit{enc_.right(v,b),-1}}};
      }
      else {
        ClauseList F; F.reserve(2);
        F.push_back({Lit{enc_.left(v,b),-1}, Lit{enc_.right(w,b),-1}});
        F.push_back({Lit{enc_.left(w,b),-1}, Lit{enc_.right(v,b),-1}});
        assert(F.size() == 2);
        assert(RandGen::valid(F));
        return F;
      }
    }
    id_t num_cl_bcedges() const noexcept {
      const id_t other_edges = (enc_.V * (enc_.V + 1)) / 2 - enc_.E;
      return enc_.B() * (2 * other_edges - enc_.V);
    }
    id_t num_lit_bcedges() const noexcept {
      return 2 * num_cl_bcedges();
    }
    id_t all_nonedges_for_bcs(std::ostream& out) const {
      id_t count = 0;
      const auto nedges = G.allnonedges(true); // including loops
      for (id_t b = 0; b < enc_.B(); ++b)
        for (const auto [v,w] : nedges) {
          const auto F = nonedge_for_bc(v,w,b);
          out << F;
          count += F.size();
        }
      assert(count == num_cl_bcedges());
      return count;
    }

    // Define the auxiliary variable edge(e,b) as equivalent to
    // having the vertices from left to right or reverse:
    ClauseList edge_def(const id_t e, const id_t b) const noexcept {
      assert(e < enc_.E and b < enc_.B());
      const auto [v,w] = edges[e];
      ClauseList F; F.reserve(6);
      const Lit evw{enc_.edge(e,b),1},
        lv{enc_.left(v,b),1}, lw{enc_.left(w,b),1},
        rv{enc_.right(v,b),1}, rw{enc_.right(w,b),1};
      /*
        evw <-> (lv and rw) or (lw and rv) =
        evw  -> (lv or lw) and (lv and rv) and (rw and lw) and (rw and rv) AND
        lv and rw -> evw  AND  lw and rv -> evw
      */
      F.push_back({-evw, lv, lw}); F.push_back({-evw, lv, rv});
      F.push_back({-evw, rw, lw}); F.push_back({-evw, rw, rv});
      F.push_back({-lv, -rw, evw}); F.push_back({-lw, -rv, evw});
      assert(F.size() == 6);
      assert(RandGen::valid(F));
      return F;
    }
    id_t num_cl_defedges() const noexcept {
      return enc_.B() * 6 * enc_.E;
    }
    id_t num_lit_defedges() const noexcept {
      return 3 * num_cl_defedges();
    }
    id_t all_edges_def(std::ostream& out) const {
      id_t count = 0;
      for (id_t b = 0; b < enc_.B(); ++b)
        for (id_t e = 0; e < enc_.E; ++e) {
          const auto F = edge_def(e,b);
          out << F;
          count += F.size();
        }
      assert(count == num_cl_defedges());
      return count;
    }

    // Require that edge e is covered by some biclique:
    ClauseList edge_cov(const id_t e) const noexcept {
      assert(e < enc_.E);
      Clause C; C.reserve(enc_.B());
      for (id_t b = 0; b < enc_.B(); ++b)
        C.push_back(Var(enc_.edge(e,b)));
      assert(C.size() == enc_.B());
      assert(RandGen::valid(C));
      return {C};
    }
    id_t num_cl_covedges() const noexcept {
      return enc_.E;
    }
    id_t num_lit_covedges() const noexcept {
      return enc_.B() * num_cl_covedges();
    }
    id_t all_edges_cov(std::ostream& out) const {
      id_t count = 0;
      for (id_t e = 0; e < enc_.E; ++e) {
        const auto F = edge_cov(e);
        out << F;
        count += F.size();
      }
      assert(count == num_cl_covedges());
      return count;
    }

    id_t num_basic_cl() const noexcept {
      return num_cl_bcedges() + num_cl_defedges() + num_cl_covedges();
    }
    id_t num_basic_lit() const noexcept {
      return num_lit_bcedges() + num_lit_defedges() + num_lit_covedges();
    }
    id_t all_basic_clauses(std::ostream& out) const {
      id_t sum = 0;
      sum += all_nonedges_for_bcs(out);
      sum += all_edges_def(out);
      sum += all_edges_cov(out);
      assert(sum == num_basic_cl());
      return sum;
    }

    // For edge e the unit-clauses putting it into biclique b:
    ClauseList place_edge(const id_t e, const id_t b) const noexcept {
      assert(e < enc_.E and b < enc_.B());
      ClauseList F; F.reserve(3);
      const auto [v,w] = edges[e];
      F.push_back({Lit{enc_.left(v,b),1}});
      F.push_back({Lit{enc_.right(w,b),1}});
      F.push_back({Lit{enc_.edge(e,b),1}});
      assert(F.size() == 3);
      assert(RandGen::valid(F));
      return F;
    }
    id_t num_cl_sb(const vei_t& sb) const noexcept {
      return 3 * sb.size();
    }
    id_t num_lit_sb(const vei_t& sb) const noexcept {
      return num_cl_sb(sb);
    }
    id_t all_sbedges(const vei_t& sb, std::ostream& out) const {
      assert(sb.size() <= enc_.B());
      id_t count = 0;
      for (id_t b = 0; const id_t e : sb) {
        const auto F = place_edge(e, b);
        out << F;
        count += F.size();
        ++b;
      }
      assert(count == num_cl_sb(sb));
      return count;
    }

    id_t num_cl(const vei_t& sb) const noexcept {
      return num_basic_cl() + num_cl_sb(sb);
    }
    id_t num_lit(const vei_t& sb) const noexcept {
      return num_basic_lit() + num_lit_sb(sb);
    }
    id_t all_clauses(const vei_t& sb, std::ostream& out) const {
      id_t sum = 0;
      sum += all_basic_clauses(out);
      sum += all_sbedges(sb, out);
      assert(sum == num_cl(sb));
      return sum;
    }

    // Require that edge e is covered by at most one biclique, via all primes:
    ClauseList edge_part2(const id_t e) const noexcept {
      assert(e < enc_.E);
      const id_t B = enc_.B();
      if (B <= 1) return {};
      const id_t count = (B * (B-1)) / 2;
      ClauseList F; F.reserve(count);
      for (id_t b1 = 0; b1 < B-1; ++b1)
        for (id_t b2 = b1+1; b2 < B; ++b2)
          F.push_back({-Lit(enc_.edge(e,b1)),-Lit(enc_.edge(e,b2))});
      assert(F.size() == count);
      assert(RandGen::valid(F));
      return F;
    }
    id_t num_cl_edgepart2() const noexcept {
      return (enc_.B() * (enc_.B()-1)) / 2 * enc_.E;
    }
    id_t num_lit_edgepart2() const noexcept {
      return 2 * num_cl_edgepart2();
    }
    id_t all_edgepart2(std::ostream& out) const noexcept {
      id_t count = 0;
      for (id_t e = 0; e < enc_.E; ++e) {
        const auto F = edge_part2(e);
        out << F;
        count += F.size();
      }
      assert(count == num_cl_edgepart2());
      return count;
    }

    id_t num_basicpart2_cl() const noexcept {
      return num_basic_cl() + num_cl_edgepart2();
    }
    id_t num_basicpart2_lit() const noexcept {
      return num_basic_lit() + num_lit_edgepart2();
    }
    id_t all_basicpart2_clauses(std::ostream& out) const {
      id_t sum = 0;
      sum += all_basic_clauses(out);
      sum += all_edgepart2(out);
      assert(sum == num_basicpart2_cl());
      return sum;
    }

    id_t num_part2_cl(const vei_t& sb) const noexcept {
      return num_basicpart2_cl() + num_cl_sb(sb);
    }
    id_t num_part2_lit(const vei_t& sb) const noexcept {
      return num_basicpart2_lit() + num_lit_sb(sb);
    }
    id_t all_part2_clauses(const vei_t& sb, std::ostream& out) const {
      id_t sum = 0;
      sum += all_basicpart2_clauses(out);
      sum += all_sbedges(sb, out);
      assert(sum == num_part2_cl(sb));
      return sum;
    }


    // Signalling unsatisfiability (necessarily here due to symmetry-breaking
    // contradicting the given value of B):
    struct Unsatisfiable {
      const vei_t incomp;
      const id_t B;
      Unsatisfiable(const vei_t ip, const id_t B) : incomp(ip), B(B) {
        assert(ip.size() > B);
      }
    };
    // Output a (single) SAT-translation (updating enc.B if it is zero, in
    // case of symmetry-breaking):
    RandGen::dimacs_pars operator()(std::ostream& out,
        const alg_options_t ao, const format_options_t fo,
        const id_t sb_rounds,
        const RandGen::vec_eseed_t& seeds = {RandGen::to_eseed("t")}) {
      const SB sb = std::get<SB>(ao);
      const PT pt = std::get<PT>(ao);
      if (pt == PT::partition1) throw "partition1 not implemented yet.\n";
      const DC dc = std::get<DC>(fo);
      const DP dp = std::get<DP>(fo);
      const CS cs = std::get<CS>(fo);

      const auto [sbv, sbs] = sb == SB::none ?
        std::make_pair(vei_t{}, stats_t{}) :
        [&sb_rounds, &seeds, this]{
          RandGen::RandGen_t g(seeds);
          return max_bcincomp(sb_rounds, g);}();
      if (enc_.B() == 0) enc_.update_B(sbv.size());
      else if (sbv.size() > enc_.B()) throw Unsatisfiable(sbv, enc_.B());
      const RandGen::dimacs_pars res{enc_.n(),
          pt == PT::cover ? num_cl(sbv) : num_part2_cl(sbv)};

      if (dc == DC::with) {
        using Environment::DWW; using Environment::DHW;
        out <<
          DHW{"Parameters"} <<
          DWW{"V"} << enc_.V << "\n" <<
          DWW{"E"} << enc_.E << "\n" <<
          DWW{"B"} << enc_.B() << "\n" <<
          DWW{"sb-option"} << sb << "\n" <<
          DWW{"pt-option"} << pt << "\n" <<

          DHW{"Statistics"} <<
          DWW{" bc-variables"} << enc_.nb() << "\n" <<
          DWW{" edge-variables"} << enc_.ne() << "\n" <<
          DWW{"total-variables"} << enc_.n() << "\n" <<
          DWW{" bc-clauses"} << num_cl_bcedges() << "\n" <<
          DWW{"  bc-lit-occurrences"} << num_lit_bcedges() << "\n" <<
          DWW{" edge-clauses"} << num_cl_defedges() << "\n" <<
          DWW{"  edge-lit-occurrences"} << num_lit_defedges() << "\n" <<
          DWW{" cover-clauses"} << num_cl_covedges() << "\n" <<
          DWW{"  cover-lit-occurrences"} << num_lit_covedges() << "\n";
        if (pt == PT::partition2)
          out <<
            DWW{" partition-clauses"} << num_cl_edgepart2() << "\n" <<
            DWW{"  partition-lit-occurrences"} << num_lit_edgepart2() << "\n";
        out <<
          DWW{" unit-clauses"} << num_cl_sb(sbv) << "\n" <<
          DWW{"total-clauses"} << res.c << "\n" <<
          DWW{"total-lit-occurrences"} <<
            (pt==PT::cover ? num_lit(sbv) : num_part2_lit(sbv)) << "\n" <<

          DHW{"Formatting"} <<
          DWW{"comments-option"} << dc << "\n" <<
          DWW{"dimacs-parameter-option"} << dp << "\n" <<
          DWW{"clauses-option"} << cs << "\n";

        if (sb != SB::none) {
          out <<
            DHW{"Symmetry Breaking"} <<
            DWW{"planted-edges"} << sbv.size() << "\n" <<
            DWW{"sb-stats"} << sbs << "\n" <<
            DWW{"num_e-seeds"} << seeds.size() << "\n" <<
            DWW{" e-seeds"} << RandGen::ESW{seeds} << "\n";
        }
      }

      if (dp == DP::with) out << res;
      if (cs == CS::with) pt==PT::cover ? all_clauses(sbv, out)
        : all_part2_clauses(sbv, out);
      return res;
    }


    struct result_t {
      Bicliques::Bcc_frame bcc;
      id_t B;
      ResultType rt;
      const id_t init_B;
      const PT pt;
      explicit result_t(const id_t B, const PT pt) noexcept
        : B(B), rt(ResultType::unknown), init_B(B), pt(pt) {}

      void output(std::ostream& out, const Graphs::AdjVecUInt& G) const {
        assert(int(rt) >= 1 and int(rt) <= 6);
        if (pt == PT::cover) out << "bcc";
        else out << "bcp";
        if (rt == ResultType::init_timeout or
            rt == ResultType::aborted)
          out << " ?";
        else if (rt == ResultType::exact)
          out << "=" << B;
        else if (rt == ResultType::init_unsat_sb or
                 rt == ResultType::init_unsat)
          out << ">" << B;
        else {
          assert(rt == ResultType::final_timeout);
          out << "<=" << B + 1;
        }
        out << "\n" << rt << " " << B << " " << init_B << "\n";
        bcc.output(out, G);
      }
    };


    // Perform a complete optimisation, with SAT-solving, downwards:
    result_t operator()(std::ostream* const log,
        const alg2_options_t ao,
        const id_t sb_rounds,
        const FloatingPoint::uint_t sec,
        const RandGen::vec_eseed_t& seeds) {
      const PT pt = std::get<PT>(ao);
      result_t res(enc_.B(), pt);
      if (enc_.E == 0) {
        res.B = 0; res.rt = ResultType::exact;
        assert(is_bcc(res.bcc, G));
        return res;
      }

      const SB sb = std::get<SB>(ao);
      const auto [sbv, sbs] = sb == SB::none ?
        std::make_pair(vei_t{}, stats_t{}) :
        [&sb_rounds, &seeds, this]{
        RandGen::RandGen_t g(seeds);
        return max_bcincomp(sb_rounds, g);}();
      if (log) {
        *log << "Symmetry-breaking: " << sbs << "\n";
      }
      if (sbv.size() > enc_.B()) {
        res.rt = ResultType::init_unsat_sb;
        return res;
      }

      const std::string filename_head = SystemCalls::system_filename(
        "Bicliques2SAT_" + std::to_string(Environment::CurrentTime::timestamp()))
        + "_";
      const std::string solver_options = "-cpu-lim=" + std::to_string(sec);
      for (bool found_bcc = false; ;) {
        const RandGen::dimacs_pars dp{enc_.n(),
            pt==PT::cover ? num_cl(sbv) : num_part2_cl(sbv)};
        const std::string inp = filename_head + std::to_string(enc_.B())
          + ".dimacs";
        {std::ofstream file(inp);
         if (not file)
           throw std::runtime_error(
             "Bicliques2SAT::operator(...): can not open output-file \"" +
             inp + "\"");
         file << dp;
         if (pt == PT::cover) all_clauses(sbv, file);
         else all_part2_clauses(sbv, file);
        }
        const auto call_res = DimacsTools::minisat_call
          (inp, enc_.lf, solver_options);
        if (log) {
          *log << "Minisat-call for B=" << res.B << ": " << call_res.stats;
        }
        if (not std::filesystem::remove(std::filesystem::path(inp)))
          throw std::runtime_error(
            "Bicliques2SAT::operator(...): can not remove output-file \"" +
            inp + "\"");

        if (call_res.stats.sr == DimacsTools::SolverR::aborted) {
          res.rt = ResultType::aborted;
          return res;
        }
        else if (call_res.stats.sr == DimacsTools::SolverR::unknown) {
          res.rt = found_bcc ?
            ResultType::final_timeout : ResultType::init_timeout;
          return res;
        }
        else if (call_res.stats.sr == DimacsTools::SolverR::unsat) {
          if (not found_bcc) res.rt = ResultType::init_unsat;
          else {
            ++res.B; res.rt = ResultType::exact;
            assert(is_bcc(res.bcc, G));
          }
          return res;
        }
        else {
          assert(call_res.stats.sr == DimacsTools::SolverR::sat);
          found_bcc = true;
          res.bcc = enc_.extract_bcc(call_res.pa);
          const auto red = trim(res.bcc);
          if (log) {
            *log << "  Literal-Reduction by trimming: " << red << "\n"
              "  Size obtained: " << res.bcc.L.size() << "\n";
          }
          assert(res.bcc.L.size() <= res.B);
          res.B = std::min(res.B, res.bcc.L.size());
          assert(res.B > 0);
          assert(res.B >= sbv.size());
          if (res.B == sbv.size()) {
            res.rt = ResultType::exact;
            assert(is_bcc(res.bcc, G));
            return res;
          }
          --res.B; update_B(res.B);
        }
      }
    }

  private :

    enc_t enc_;

  };

}

#endif
