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
     - SB (primary symmetry-breaking forms)
     - SS (secondary symmetry-breaking)
     - PT (problem-types)
     - DC (Dimacs-comments on/off)
     - DP (Dimacs-parameters on/off)
     - CS (clause-set-output on/off)
     - DI (search direction)
     - SO (solver options)

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

      - all_cover_clauses(vei_t sb, std::ostream& out) -> id_t : combines
        all_basic_clauses with all_sbedges
      - num_cl(vei_t)

      - the complete translation (with complete output):
       - struct Unsatisfiable (thrown when B was too low to start with
         (from above))
       - sat_translate(ostream&, alg_options, format_options, rounds, seeds)
         -> dimacs_pars (throws Unsatisfiable)

     - the complete SAT-solving (from above), computing an optimum
       bcc:
      - struct result_t (Bcc_frame, B, ResultType, init_B)
      - operator (ostream*, alg_options, rounds, sec, seeds)
        stores the Dimacs-files on disc, using the minisat_call, decrementing
        B until unsatisfiability reached.


See plans/general.txt.

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
#include <algorithm>
#include <map>
#include <ios>

#include <cstdint>
#include <cassert>
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumTypes.hpp>
#include <Numerics/Statistics.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/Algorithms.hpp>
#include <Transformers/Generators/Random/ClauseSets.hpp>

#include "Algorithms.hpp"
#include "Graphs.hpp"
#include "Bicliques.hpp"
#include "DimacsTools.hpp"
#include "GraphTraversal.hpp"
#include "ConflictGraphs.hpp"

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


  enum class SB { basic=0,
                  sorteda=1, sortedm=2, sortedi=3,
                  none=4 }; // symmetry-breaking by edge-placement
  enum class SS { with=0, without=1 }; // symmetry-breaking by edge-restriction
  enum class PT { cover=0, partition1=1, partition2=2 }; // problem type
  enum class DI { downwards=0, upwards=1, binary_search=2,
                  none=3 }; // search direction
  enum class SO { none=0, nopre=1 }; // solver options
  enum class DC { with=0, without=1 }; // Dimacs-comments (or other comments)
  enum class DP { with=0, without=1 }; // Dimacs-parameters
  enum class CS { with=0, without=1 }; // clause-set
  enum class BC { with=0, without=1 }; // biclique-list (or other results)

  std::string solver_option(const SO so) {
    if (so == SO::nopre) return " -no-pre";
    else return "";
  }

  constexpr char sep = ',';
  typedef std::tuple<SB,SS,PT> alg_options_t;
  typedef std::tuple<SB,SS,PT,DI,SO> alg2_options_t;
  typedef std::tuple<DC,DP,CS> format_options_t;
  typedef std::tuple<DC,BC> format2_options_t;

  constexpr id_t default_sb_rounds = 100;
}
namespace Environment {
  template <>
  struct RegistrationPolicies<Bicliques2SAT::SB> {
    static constexpr int size = int(Bicliques2SAT::SB::none)+1;
    static constexpr std::array<const char*, size> string
    {"+sb", "+sba", "+sbm", "+sbi", "-sb"};
  };
  template <>
  struct RegistrationPolicies<Bicliques2SAT::SS> {
    static constexpr int size = int(Bicliques2SAT::SS::without)+1;
    static constexpr std::array<const char*, size> string
    {"+ssb", "-ssb"};
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
    {"+com", "-com"};
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
  struct RegistrationPolicies<Bicliques2SAT::BC> {
    static constexpr int size = int(Bicliques2SAT::BC::without)+1;
    static constexpr std::array<const char*, size> string
    {"+sol", "-sol"};
  };
  template <>
  struct RegistrationPolicies<Bicliques2SAT::DI> {
    static constexpr int size = int(Bicliques2SAT::DI::none)+1;
    static constexpr std::array<const char*, size> string
    {"down", "up", "binsearch", "stationary"};
  };
  template <>
  struct RegistrationPolicies<Bicliques2SAT::SO> {
    static constexpr int size = int(Bicliques2SAT::SO::nopre)+1;
    static constexpr std::array<const char*, size> string
    {"defsolve", "nopre"};
  };
}
namespace Bicliques2SAT {
  std::ostream& operator <<(std::ostream& out, const SB s) {
    switch (s) {
    case SB::basic : return out << "basic-sb";
    case SB::sorteda : return out << "sorted-sb-addition";
    case SB::sortedm : return out << "sorted-sb-multiplication";
    case SB::sortedi : return out << "greedy-sb";
    case SB::none : return out << "no-sb";
    default : return out << "SB::UNKNOWN";}
  }
  std::ostream& operator <<(std::ostream& out, const SS s) {
    switch (s) {
    case SS::with : return out << "with-ssb";
    case SS::without : return out << "without-ssb";
    default : return out << "SS::UNKNOWN";}
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
  std::ostream& operator <<(std::ostream& out, const BC b) {
    switch (b) {
    case BC::with : return out << "with-solution";
    case BC::without : return out << "without-solution";
    default : return out << "BC::UNKNOWN";}
  }
  std::ostream& operator <<(std::ostream& out, const DI d) {
    switch (d) {
    case DI::downwards : return out << "downwards";
    case DI::upwards : return out << "upwards";
    case DI::binary_search : return out << "binsearch";
    case DI::none : return out << "stationary";
    default : return out << "DI::UNKNOWN";}
  }
  std::ostream& operator <<(std::ostream& out, const SO s) {
    switch (s) {
    case SO::none : return out << "default-solver";
    case SO::nopre : return out << "-no-pre";
    default : return out << "SO::UNKNOWN";}
  }


  enum class ResultType {
    unknown = 0,       // default-value

    // contradictions to given upper-bound:
    upper_unsat_sb = 1, // symmetry-breaking showed unsatisfiability
    // timeouts:
    other_timeout = 2, // non-upper-bound-value of B yielded solver-timeout

    aborted = 3,       // solver aborted computation
    exact = 4,         // exact result obtained
  };
  std::ostream& operator <<(std::ostream& out, const ResultType r) {
    switch(r) {
    case ResultType::unknown : return out << "initialy-unknown";
    case ResultType::upper_unsat_sb : return out << "upper-bound-unsat-by-sb";
    case ResultType::other_timeout : return out << "timeout";
    case ResultType::aborted : return out << "aborted";
    case ResultType::exact : return out << "exact";
    default : return out << "ResultType::UNKNOWN";}
  }

  /* Class Bounds

    lower bound l : all B < l are unsatisfiable
    upper bound u : all B >= u are satisfiable.
  */
  typedef std::pair<var_t, bool> value_or_increment_t; // true means inc

  struct Bounds {
    typedef Graphs::AdjVecUInt graph_t;
    typedef VarEncoding::id_t size_t;

    const DI di;
    const bool incl = false, incu = false;
    const size_t generic_ub = trivial_upper_bound();
  private :
    size_t l = trivial_lower_bound(), u = trivial_upper_bound();
    bool init;
  public :

    Bounds() = delete;

    struct choose_l {};
    constexpr Bounds(DI d, choose_l, const value_or_increment_t B) noexcept :
    di(d), incl(B.second), l(B.first), init(not incl) {
      assert(di == DI::upwards or di == DI::none);
    }
    struct choose_u {};
    constexpr Bounds(DI d, choose_u, const value_or_increment_t B) noexcept :
    di(d), incu(B.second), u(B.first), init(not incu) {
      assert(di == DI::downwards or di == DI::binary_search);
    }

    constexpr Bounds(DI d, const value_or_increment_t lower,
                     const value_or_increment_t upper) noexcept :
    di(d), incl(lower.second), incu(upper.second),
    l(lower.first), u(upper.first), init(not incl and not incu) {}

    Bounds(const Bounds& other, const graph_t& G) noexcept :
    di(other.di), incl(other.incl), incu(other.incu),
    generic_ub(std::min(other.generic_ub, simple_upper_bound(G))),
    l(other.l), u(incu ? other.u : std::min(other.u, generic_ub)),
    init(other.init) {
      if (not incl) l = std::min(l, generic_ub);
    }

    size_t lb() const noexcept { return l; }
    size_t ub() const noexcept { return u; }

    size_t next() const noexcept {
      if (not init) return 0;
      if (di == DI::downwards)
        return u==0 ? 0 : u-1;
      else if (di == DI::binary_search) {
        assert(l <= u);
        return l + (u-l)/2;
      }
      else return l;
    }

    constexpr bool initialised() const noexcept { return init; }
    constexpr bool open() const noexcept { return l < u; }
    constexpr bool closed() const noexcept { return l == u; }
    constexpr bool inconsistent() const noexcept { return l > u; }

    // If the increment-flags are set, then the current value of l resp. u
    // means an increment on the symmetry-breaking-value:
    void update_by_sb(const size_t sb) noexcept {
      init = true;
      if (incl) {
        assert(sb + l > l);
        l = std::min(sb + l, generic_ub);
      }
      else
        l = std::max(l, sb);
      if (incu) {
        assert(sb + u > u);
        u = std::min(sb + u, generic_ub);
      }
    }
    void update_if_sat(const size_t newu) noexcept {
      assert(init);
      assert(newu <= next());
      u = newu;
    }
    // if B = next() was found unsatisfiable:
    void update_if_unsat() noexcept {
      assert(init);
      const size_t B = next();
      assert(B < B+1);
      l = B + 1;
    }

    static constexpr size_t trivial_lower_bound() noexcept {return 0;}
    static constexpr size_t trivial_upper_bound() noexcept {return -1;}

    static size_t simple_lower_bound(const graph_t& G) noexcept {
      if (G.m() == 0) return 0; else return 1;
      // for general G: the number of connected components with at least
      // one edge
    }
    static size_t simple_upper_bound(const graph_t& G) noexcept {
      if (G.m() == 0) return 0;
      assert(G.n() >= 1);
      return std::min(G.n() - 1, G.m());
      // for general G with k connected components: G.n() - k
    }

    friend std::ostream& operator <<(std::ostream& out, const Bounds& b) {
      if (b.di == DI::none) {
        assert(not b.incu);
        if (b.init) return out << b.l;
        assert(b.incl);
        return out << "+" << b.l;
      }
      else {
        out << b.di << " ";
        if (b.init) return out << b.l << " " << b.u;
        if (b.incl) out << "+";
        out << b.l << " ";
        if (b.incu) out << "+";
        return out << b.u;
      }
    }
  };


  struct BC2SAT {
    typedef Graphs::AdjVecUInt graph_t;
    const graph_t& G;
    const graph_t::vecedges_t edges; // vector of pairs of id_t's
    Bounds bounds;

    typedef VarEncoding enc_t;
    typedef VarEncoding::id_t id_t;
    static_assert(std::is_same_v<id_t, graph_t::id_t>);
    static_assert(std::is_same_v<id_t, std::uint64_t>);

    explicit BC2SAT(const graph_t& G, Bounds b) noexcept :
      G(G), edges(G.alledges()), bounds(b,G), enc_(G,bounds.next()) {}


    const enc_t& enc() const noexcept { return enc_; }
    void update_B(const var_t newB) noexcept {
      enc_.update_B(newB);
    }


    // Compute a maximal bc-incompatible sequence of edges (every pair is
    // incompatible), given by their indices, by repeatedly chosing the first
    // edge available, and then removing all bc-compatible edges, until no
    // edge is left:
    typedef std::vector<id_t> vei_t; // vector of edge-indices
    bool bccomp(const id_t i, const id_t j) const noexcept {
      return Bicliques::bccomp(edges[i],edges[j], G);
    }
    bool is_bcincomp(const vei_t& v) const noexcept {
      return Algorithms::is_independent(v, [this](id_t i, id_t j)
                                        {return bccomp(i,j);});
    }
    vei_t max_bcincomp_unstable(vei_t avail) const {
      return Algorithms::greedy_max_independent_unstable(std::move(avail),
             [this](id_t i, id_t j) {return bccomp(i,j);});
    }
    vei_t max_bcincomp_stable(vei_t avail) const {
      return Algorithms::greedy_max_independent(std::move(avail),
             [this](id_t i, id_t j) {return bccomp(i,j);});
    }

    typedef GenStats::BasicStats<id_t, FloatingPoint::float80> stats_t;
    struct symmbreak_res_t {
      vei_t v; // vector for primary symmetry-breaking (placing edges)
      vei_t sv; // secondary symmetry-breaking: the candidate edges to restrict
      stats_t s;
      id_t i = 0;
      vei_t restrict_sv(const id_t B) const {
        const id_t optsbs = v.size();
        if (B <= optsbs) return {};
        const id_t choose = B - optsbs;
        assert(choose >= 1);
        const id_t size = sv.size();
        if (choose >= size) return sv;
        return vei_t(sv.begin(), sv.begin()+choose);
      }
    };
    id_t max_B(const id_t optsbs) const noexcept {
      Bounds b(bounds);
      b.update_by_sb(optsbs);
      if (b.di == DI::none or b.di == DI::downwards) return b.next();
      else {
        const id_t u = b.ub();
        return u==0 ? u : u-1;
      }
    }
    void add_random_secondary_edges(symmbreak_res_t& sbres,
                                    const RandGen::vec_eseed_t& seeds) const {
      const auto optsbs = sbres.v.size();
      const id_t B = max_B(optsbs);
      if (B <= optsbs) return;
      const auto choose = B - optsbs;
      assert(choose >= 1);
      auto copy = sbres.v; std::ranges::sort(copy);
      const auto complement = Algorithms::complement_uint(copy, enc_.E);
      assert(choose <= complement.size());
      const auto choice_indices = RandGen::choose_kn(choose,
                              complement.size(), RandGen::RandGen_t(seeds));
      sbres.sv.reserve(choice_indices.size());
      for (const auto i : choice_indices) sbres.sv.push_back(complement[i]);
    }
    void add_sorted_secondary_edges(symmbreak_res_t& sbres,
                                    vei_t avail) const {
      const auto optsbs = sbres.v.size();
      const id_t B = max_B(optsbs);
      if (B <= optsbs) return;
      const auto choose = B - optsbs;
      assert(choose >= 1);
      std::ranges::reverse(avail);
      sbres.sv = Algorithms::complement_subsequence(sbres.v, avail);
      assert(choose <= sbres.sv.size());
      sbres.sv.resize(choose);
    }

    // Now repeat rounds-often, and return the first best, with statistics:
    symmbreak_res_t max_bcincomp_unstable(const id_t rounds,
                                          RandGen::vec_eseed_t seeds,
                                          const SS ssb) const {
      assert(rounds < id_t(-1));
      if (rounds == 0 or enc_.E == 0) return {};
      if (rounds == 1) {
        symmbreak_res_t res;
        res.v = max_bcincomp_unstable(
          RandGen::random_permutation<vei_t>(enc_.E,seeds));
        res.s += res.v.size();
        if (ssb == SS::with) add_random_secondary_edges(res, seeds);
        return res;
      }
      seeds.push_back(0);
      symmbreak_res_t res;
      for (id_t i = 1; i <= rounds; ++i) {
        seeds.back() = i;
        vei_t nres = max_bcincomp_unstable(
          RandGen::random_permutation<vei_t>(enc_.E,seeds));
        const auto s = nres.size();
        assert(s >= 1);
        res.s += s;
        if (s > res.v.size()) {res.i = i; res.v = std::move(nres);}
      }
      if (ssb == SS::with) {
        seeds.back() = res.i;
        add_random_secondary_edges(res, seeds);
      }
      return res;
    }

    id_t adegree(const id_t e) const noexcept {
      assert(e < enc_.E);
      const auto [v,w] = edges[e];
      return G.degree(v) + G.degree(w);
    }
    id_t mdegree(const id_t e) const noexcept {
      assert(e < enc_.E);
      const auto [v,w] = edges[e];
      return G.degree(v) * G.degree(w);
    }
    vei_t sorted_order(const RandGen::vec_eseed_t& seeds, const SB sb) const {
      assert(sb == SB::sorteda or sb == SB::sortedm);
      const auto sorta =
        [this](const id_t e1, const id_t e2) noexcept {
        return adegree(e1) > adegree(e2);
      };
      const auto sortm =
        [this](const id_t e1, const id_t e2) noexcept {
        return mdegree(e1) > mdegree(e2);
      };
      vei_t res = RandGen::random_permutation<vei_t>(enc_. E,seeds);
      if (sb == SB::sorteda) std::ranges::stable_sort(res, sorta);
      else std::ranges::stable_sort(res, sortm);
      return res;
    }
    symmbreak_res_t max_bcincomp_sort(const id_t rounds,
                                      RandGen::vec_eseed_t seeds,
                                      const SS ssb,
                                      const SB sb) const {
      assert(rounds < id_t(-1));
      assert(sb == SB::sorteda or sb == SB::sortedm);
      if (rounds == 0 or enc_.E == 0) return {};
      if (rounds == 1) {
        symmbreak_res_t res;
        const vei_t order = sorted_order(seeds, sb);
        res.v = max_bcincomp_stable(order);
        res.s += res.v.size();
        if (ssb == SS::with) add_sorted_secondary_edges(res, order);
        return res;
      }
      seeds.push_back(0);
      symmbreak_res_t res;
      for (id_t i = 1; i <= rounds; ++i) {
        seeds.back() = i;
        const vei_t order = sorted_order(seeds, sb);
        vei_t nres = max_bcincomp_stable(order);
        const auto s = nres.size();
        assert(s >= 1);
        res.s += s;
        if (s > res.v.size()) {
          res.i = i;
          res.v = std::move(nres);
          if (ssb == SS::with) add_sorted_secondary_edges(res, order);
        }
      }
      return res;
    }

    symmbreak_res_t max_bcincomp(const id_t rounds,
                                 const RandGen::vec_eseed_t& seeds,
                                 const SB sb,
                                 const SS ss) const {
      switch (sb) {
      case SB::none : return {};
      case SB::basic : return max_bcincomp_unstable(rounds, seeds, ss);
      case SB::sorteda : [[fallthrough]];
      case SB::sortedm : return max_bcincomp_sort(rounds, seeds, ss, sb);
      case SB::sortedi : return {};
      default : assert(false); return {};}
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
    template <class STREAM>
    id_t all_nonedges_for_bcs(STREAM out) const {
      id_t count = 0;
      const auto nedges = G.allnonedges(true); // including loops
      for (id_t b = 0; b < enc_.B(); ++b)
        for (const auto [v,w] : nedges) {
          const auto F = nonedge_for_bc(v,w,b);
          using DimacsTools:: operator <<;
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
    template <class STREAM>
    id_t all_edges_def(STREAM out) const {
      id_t count = 0;
      for (id_t b = 0; b < enc_.B(); ++b)
        for (id_t e = 0; e < enc_.E; ++e) {
          const auto F = edge_def(e,b);
          using DimacsTools:: operator <<;
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
    template <class STREAM>
    id_t all_edges_cov(STREAM out) const {
      id_t count = 0;
      for (id_t e = 0; e < enc_.E; ++e) {
        const auto F = edge_cov(e);
        using DimacsTools:: operator <<;
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
    template <class STREAM>
    id_t all_basic_clauses(STREAM out) const {
      id_t sum = 0;
      sum += all_nonedges_for_bcs<STREAM>(out);
      sum += all_edges_def<STREAM>(out);
      sum += all_edges_cov<STREAM>(out);
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
    template <class STREAM>
    id_t all_sbedges(const vei_t& sb, STREAM out) const {
      assert(sb.size() <= enc_.B());
      id_t count = 0;
      for (id_t b = 0; const id_t e : sb) {
        const auto F = place_edge(e, b);
        using DimacsTools:: operator <<;
        out << F;
        count += F.size();
        ++b;
      }
      assert(count == num_cl_sb(sb));
      return count;
    }

    // For edge e forbidding the negative direction for biclique b,
    // and forbidding it at all in bicliques > b:
    ClauseList forbid_edge(const id_t e, const id_t b) const noexcept {
      const auto B = enc_.B();
      assert(e < enc_.E); assert(b < B);
      const auto numcl = B - b;
      ClauseList F; F.reserve(numcl);
      const auto [v,w] = edges[e];
      F.push_back({-Lit(enc_.left(w,b)), -Lit(enc_.right(v,b))});
      for (id_t b2 = b+1; b2 < B; ++b2)
        F.push_back({-Lit(enc_.edge(e,b2))});
      assert(F.size() == numcl);
      assert(RandGen::valid(F));
      return F;
    }
    id_t num_cl_ssb(const vei_t& ssb) const noexcept {
      const auto M = ssb.size();
      return (M*(M+1))/2;
    }
    id_t num_lit_ssb(const vei_t& ssb) const noexcept {
      return num_cl_ssb(ssb) + ssb.size();
    }
    template <class STREAM>
    id_t all_ssbedges(const id_t sbs, const vei_t& ssb, STREAM out) const {
      assert(sbs + ssb.size() <= enc_.B());
      id_t count = 0;
      for (id_t b = sbs; const id_t e : ssb) {
        const auto F = forbid_edge(e, b);
        using DimacsTools:: operator <<;
        out << F;
        count += F.size();
        ++b;
      }
      assert(count == num_cl_ssb(ssb));
      return count;
    }

    id_t num_cover_cl(const vei_t& sb, const vei_t& ssb) const noexcept {
      return num_basic_cl() + num_cl_sb(sb) + num_cl_ssb(ssb);
    }
    id_t num_cover_lit(const vei_t& sb, const vei_t& ssb) const noexcept {
      return num_basic_lit() + num_lit_sb(sb) + num_lit_ssb(ssb);
    }
    template <class STREAM>
    id_t all_cover_clauses(const vei_t& sb, const vei_t& ssb,
                           STREAM out) const {
      id_t sum = 0;
      sum += all_basic_clauses<STREAM>(out);
      sum += all_sbedges<STREAM>(sb, out);
      sum += all_ssbedges<STREAM>(sb.size(), ssb, out);
      assert(sum == num_cover_cl(sb, ssb));
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
    template <class STREAM>
    id_t all_edgepart2(STREAM out) const noexcept {
      id_t count = 0;
      for (id_t e = 0; e < enc_.E; ++e) {
        const auto F = edge_part2(e);
        using DimacsTools:: operator <<;
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
    template <class STREAM>
    id_t all_basicpart2_clauses(STREAM out) const {
      id_t sum = 0;
      sum += all_basic_clauses<STREAM>(out);
      sum += all_edgepart2<STREAM>(out);
      assert(sum == num_basicpart2_cl());
      return sum;
    }

    id_t num_part2_cl(const vei_t& sb, const vei_t& ssb) const noexcept {
      return num_basicpart2_cl() + num_cl_sb(sb) + num_cl_ssb(ssb);
    }
    id_t num_part2_lit(const vei_t& sb, const vei_t& ssb) const noexcept {
      return num_basicpart2_lit() + num_lit_sb(sb) + num_lit_ssb(ssb);
    }
    template <class STREAM>
    id_t all_part2_clauses(const vei_t& sb, const vei_t& ssb,
                           STREAM out) const {
      id_t sum = 0;
      sum += all_basicpart2_clauses<STREAM>(out);
      sum += all_sbedges<STREAM>(sb, out);
      sum += all_ssbedges<STREAM>(sb.size(), ssb, out);
      assert(sum == num_part2_cl(sb, ssb));
      return sum;
    }

    RandGen::dimacs_pars all_dimacs(const vei_t& sb, const vei_t& ssb,
                                    const PT pt) const noexcept {
      assert(pt == PT::cover or pt == PT::partition2);
      return {enc_.n(),
          pt == PT::cover ? num_cover_cl(sb, ssb) : num_part2_cl(sb, ssb)};
    }
    template <class STREAM>
    void all_clauses(const vei_t& sb, const vei_t& ssb, const PT pt,
                     STREAM out) {
      assert(pt == PT::cover or pt == PT::partition2);
      if (pt == PT::cover) all_cover_clauses<STREAM>(sb, ssb, out);
      else all_part2_clauses<STREAM>(sb, ssb, out);
    }


    // Output a (single) SAT-translation (not using bounds, but
    // updating enc.B if it is zero, in case of symmetry-breaking;
    // this function is not called by sat_solve):
    RandGen::dimacs_pars sat_translate(std::ostream& out,
        const alg_options_t ao, const format_options_t fo,
        const id_t sb_rounds,
        const RandGen::vec_eseed_t& seeds = {RandGen::to_eseed("t")}) {
      assert(bounds.di == DI::none);
      const SB sb = std::get<SB>(ao);
      const SS ss = std::get<SS>(ao);
      const PT pt = std::get<PT>(ao);
      if (pt == PT::partition1) throw "partition1 not implemented yet.\n";
      const DC dc = std::get<DC>(fo);
      const DP dp = std::get<DP>(fo);
      const CS cs = std::get<CS>(fo);

      using Environment::DWW; using Environment::DHW;
      if (dc == DC::with) {
        out <<
          DHW{"Parameters"} <<
          DWW{"B"} << bounds << "\n" <<
          DWW{"sb-options"} << sb << " " << ss << "\n" <<
          DWW{"pt-option"} << pt << "\n" <<
          DWW{"comments-option"} << dc << "\n" <<
          DWW{"dimacs-parameter-option"} << dp << "\n" <<
          DWW{"clauses-option"} << cs << "\n";
        if (sb != SB::none) {
          out <<
            DWW{"sb-rounds"} << sb_rounds << "\n" <<
            DWW{"num_e-seeds"} << seeds.size() << "\n";
          if (not seeds.empty())
            out <<
              DWW{" e-seeds"} << RandGen::ESW{seeds} << "\n";
        }
        out.flush();
      }

      const symmbreak_res_t sbr = max_bcincomp(sb_rounds, seeds, sb, ss);
      const auto optsbs = sbr.v.size();
      bounds.update_by_sb(optsbs);
      assert(not bounds.inconsistent());
      const auto B = bounds.next();
      update_B(B);
      if (dc == DC::with and sb != SB::none) {
        out <<
          DHW{"Symmetry Breaking"} <<
          DWW{"planted-edges"} << optsbs << "\n" <<
          DWW{"sb-stats"} << sbr.s << "\n" <<
          DWW{"sb-seed"} << sbr.i << "\n";
        if (ss == SS::with)
          out <<
            DWW{"restricted-edges"} << sbr.sv.size() << "\n";
      }

      const RandGen::dimacs_pars res = all_dimacs(sbr.v, sbr.sv, pt);
      if (dc == DC::with) {
        out <<
          DHW{"Statistics"} <<
          DWW{"V"} << enc_.V << "\n" <<
          DWW{"E"} << enc_.E << "\n" <<
          DWW{"B"} << B << "\n" <<
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
          DWW{" positive unit-clauses"} << num_cl_sb(sbr.v) << "\n" <<
          DWW{" negative unit-clauses"} <<
            num_cl_ssb(sbr.sv)-sbr.sv.size() << "\n" <<
          DWW{" negative binary-clauses"} << sbr.sv.size() << "\n" <<
          DWW{"total-clauses"} << res.c << "\n" <<
          DWW{"total-lit-occurrences"} << (pt==PT::cover ?
            num_cover_lit(sbr.v, sbr.sv) :
            num_part2_lit(sbr.v, sbr.sv)) << "\n";
      }

      if (dp == DP::with) out << res;
      if (cs == CS::with) all_clauses<std::ostream&>(sbr.v, sbr.sv, pt, out);
      return res;
    }


    struct result_t {
      Bicliques::Bcc_frame bcc;
      bool solution = false;
      id_t B;
      ResultType rt;

      const id_t init_B;
      const PT pt;
      const stats_t sbs; // symmetry-breaking statistics

      typedef DimacsTools::TableMinisatStats::float_t float_t;
      DimacsTools::TableMinisatStats minisat_stats;

      result_t(const id_t B, const PT pt, const stats_t& sbs) noexcept
        : B(B), rt(ResultType::unknown), init_B(B), pt(pt), sbs(sbs),
          minisat_stats({"B"}) {}

      void output(std::ostream* const out,
                  const BC bc,
                  const Graphs::AdjVecUInt& G,
                  std::ofstream* const stats) const {
        assert(int(rt) >= 1 and int(rt) <= 6);
        if (out) {
          using Environment::DWW;
          *out << DWW{"sb-stats"} << sbs << "\n"
               << DWW{"result-type"} << rt << "\n";
          *out << DWW{pt == PT::cover ? "bcc" : "bcp"};
          if (rt == ResultType::aborted)
            *out << "?";
          else if (rt == ResultType::exact)
            *out << "= " << B;
          else if (rt == ResultType::upper_unsat_sb)
            *out << "> " << B;
          else {
            assert(rt == ResultType::other_timeout);
            *out << "<= " << B + 1;
          }
          *out << "\n";
        }
        if (bc == BC::with) bcc.output(*out, G);
        if (stats) *stats << minisat_stats;
      }
    };


    // Perform a complete optimisation, with SAT-solving:
    result_t sat_solve(std::ostream* const log,
        const alg2_options_t ao,
        const id_t sb_rounds,
        const FloatingPoint::uint_t sec,
        const RandGen::vec_eseed_t& seeds) {
      const PT pt = std::get<PT>(ao);
      if (enc_.E == 0) {
        result_t res(0, pt, {});
        res.rt = ResultType::exact;
        assert(is_bcc(res.bcc, G));
        return res;
      }

      const SB sb = std::get<SB>(ao);
      const SS ss = std::get<SS>(ao);
      const symmbreak_res_t sbr =  max_bcincomp(sb_rounds, seeds, sb, ss);
      const auto optsbs = sbr.v.size();
      if (log) {
        *log << "Symmetry-breaking: index=" << sbr.i << ", stats= " << sbr.s
             << "; r-edges= " << sbr.sv.size() << std::endl;
      }

      bounds.update_by_sb(optsbs);
      result_t res(bounds.ub(), pt, sbr.s);
      if (bounds.inconsistent()) {
        res.rt = ResultType::upper_unsat_sb;
        return res;
      }
      if (bounds.closed()) {
        res.rt = ResultType::exact;
        res.B = bounds.lb();
        return res;
      }

      const std::string filename_head = SystemCalls::system_filename(
        "Bicliques2SAT_" + Environment::CurrentTime::timestamp_str())
        + "_";
      const std::string solver_options = "-cpu-lim=" + std::to_string(sec)
        + solver_option(std::get<SO>(ao));

      while (true) { // main solver-loop
        assert(bounds.open());
        {const auto nc = bounds.next();
         update_B(nc);
         res.B = nc; // so that in case of error the last B-value is available
        }

        const auto inp = [this, &sbr, pt](std::FILE* const fp){
          using DimacsTools:: operator <<;
          const auto sbsv = sbr.restrict_sv(enc_.B());
          fp << all_dimacs(sbr.v, sbsv, pt);
          all_clauses(sbr.v, sbsv, pt, fp);
        };
        const auto call_res = DimacsTools::minisat_call
          (inp, enc_.lf, solver_options);
        if (log) {
          *log << "Minisat-call for B=" << res.B << ": " << call_res.stats
               << call_res.rv.out << std::endl;
        }

        if (call_res.stats.sr == DimacsTools::SolverR::aborted) {
          res.rt = ResultType::aborted;
          return res;
        }
        else if (call_res.stats.sr == DimacsTools::SolverR::unknown) {
          res.rt = ResultType::other_timeout;
          return res;
        }
        res.minisat_stats.add(call_res, {result_t::float_t(res.B)});

        if (call_res.stats.sr == DimacsTools::SolverR::unsat)
          bounds.update_if_unsat();
        else {
          assert(call_res.stats.sr == DimacsTools::SolverR::sat);
          res.solution = true;
          res.bcc = enc_.extract_bcc(call_res.pa);
          const auto red = trim(res.bcc);
          if (log) {
            *log << "  Literal-Reduction by trimming: " << red << "\n"
              "  Size obtained: " << res.bcc.L.size() << std::endl;
          }
          bounds.update_if_sat(res.bcc.L.size());
        }

        assert(not bounds.inconsistent());
        if (bounds.closed()) {
          res.rt = ResultType::exact;
          res.B = bounds.lb();
          assert(not res.solution or is_bcc(res.bcc, G));
          return res;
        }
      }
    }

  private :

    enc_t enc_;

  };


  /*
    For a DT::GslicedCNF, compute an optimal replacement of the global
    slice.

     - Keeping the order of the clauses.
     - Reusing the given variable numbers, for each component using the
       initial part.

  */
  struct GlobRepl {
    typedef GraphTraversal::size_t size_t;
    typedef DimacsTools::GslicedCNF GslicedCNF;
    const GslicedCNF& F;
    typedef ConflictGraphs::AllOcc AllOcc;
    const AllOcc occ;
    typedef GraphTraversal::CCbyIndices CCbyIndices;
    const CCbyIndices CC;
    typedef CCbyIndices::sizes_t sizes_t;
    const sizes_t sizes; // for a cc its size (at index cc-1)
    const size_t numntcc; // number of non-trivial connected components
    typedef CCbyIndices::components_t components_t;
    const components_t ccvec; // for a cc the (sorted) clause-indices (at cc-1)
    typedef CCbyIndices::indvec_t indvec_t;
    const indvec_t ntcc; // the non-trivial components (creating a
                         // 0-based numbering 0, ..., numntcc-1)
    typedef DimacsTools::varlist_t varlist_t;
    typedef std::vector<varlist_t> ntvar_t;
    const ntvar_t ntvar; // ntvar[i] are the (sorted) variables of cc ntcc[i]
    typedef RandGen::dimacs_pars dimacs_pars;
    typedef std::map<dimacs_pars, indvec_t> ntcc_map_t;
    const ntcc_map_t ntcc_map; // (n,c) -> list of nt-cc's
    typedef ntcc_map_t::const_iterator map_it;


    explicit GlobRepl(const GslicedCNF& F) noexcept
    : F(F), occ(ConflictGraphs::allocc(F.G())),
      CC(ConflictGraphs::cc_by_dfs(F.G(), occ)), sizes(CC.sizes()),
      numntcc(count_ntcc()), ccvec(CC.components(sizes)),
      ntcc(create_ntcc()), ntvar(create_ntvar()),
      ntcc_map(create_ntcc_map()) {}

    bool nontrivial(const size_t c) const noexcept {
      assert(CC.valid_cc(c));
      assert(sizes[c-1] >= 1);
      return sizes[c-1] >= 2;
    }
  private :
    size_t count_ntcc() const noexcept {
      size_t res = 0;
      for (size_t c = 1; c <= CC.numcc; ++c) res += nontrivial(c);
      return res;
    }
    indvec_t create_ntcc() const {
      indvec_t res; res.reserve(numntcc);
      for (size_t c = 1; c <= CC.numcc; ++c)
        if (nontrivial(c)) res.push_back(c);
      return res;
    }
    ntvar_t create_ntvar() const {
      ntvar_t res(numntcc);
      for (size_t i = 0; i < numntcc; ++i) {
        const size_t c = ntcc[i];
        const auto& clis = ccvec[c-1];
        DimacsTools::VarSet V;
        for (const auto cli : clis) {
          const DimacsTools::VarSet Vi = DimacsTools::var(F.G().second[cli]);
          V.insert(Vi.begin(), Vi.end());
        }
        res[i].assign(V.begin(), V.end());
      }
      return res;
    }
    ntcc_map_t create_ntcc_map() const {
      ntcc_map_t res;
      for (size_t i = 0; i < numntcc; ++i) {
        const size_t n = ntvar[i].size();
        const size_t c = sizes[ntcc[i]-1];
        res[{n,c}].push_back(i);
      }
      return res;
    }
  public :

    // ignoring pure universal literals:
    bool is_pure_other(const size_t i) const noexcept {
      [[maybe_unused]]const size_t c = F.O().first.c;
      assert(i < c);
      return sizes[CC.cv[i]-1] == 1;
    }
    // The pure "other" clauses:
    void E0(std::ostream& out) const {
      const size_t n = F.O().first.n;
      const size_t c = CC.numcc - numntcc;
      out << dimacs_pars{n,c};
      size_t count = 0;
      for (size_t i = 0; i < F.O().first.c; ++i)
        if (is_pure_other(i)) {
          out << F.O().second[i];
          ++count;
        }
      assert(count == c);
    }
    // The other-part at it, index i:
    void E(std::ostream& out, const map_it& it, const size_t i) const {
      const auto [P, L] = *it;
      assert(i < L.size());
      const size_t n = F.O().first.n, c = P.c;
      assert(c >= 2);
      out << dimacs_pars{n,c};
      const size_t cci = L[i];
      const size_t cc = ntcc[cci];
      const auto& clause_indices = ccvec[cc-1];
      assert(clause_indices.size() == c);
      for (const size_t i : clause_indices)
        out << F.O().second[i];
    }
    // The global-part at it, index i, variables renamed to 1, ... :
    void A(std::ostream& out, const map_it& it, const size_t i) const {
      const auto [P, L] = *it;
      assert(i < L.size());
      const auto [n,c] = P;
      assert(n >= 1); assert(c >= 2);
      out << dimacs_pars{n,c};
      const size_t cci = L[i];
      const size_t cc = ntcc[cci];
      const auto& clause_indices = ccvec[cc-1];
      assert(clause_indices.size() == c);
      const auto& V = ntvar[cci];
      const auto m = DimacsTools::list2map(V);
      for (const size_t i : clause_indices)
        out << DimacsTools::rename(F.G().second[i], m);
    }

    typedef Graphs::AdjVecUInt graph_t;
    graph_t conflictgraph(const size_t i) const {
      assert(i < numntcc);
      const auto cc = ntcc[i];
      assert(nontrivial(cc));
      return ConflictGraphs::conflictgraph(ccvec[cc-1], ntvar[i], occ);
    }

    // The clause of the result have the same order as the clauses in the
    // component:
    typedef DimacsTools::DimacsClauseList DimacsClauseList;
    DimacsClauseList solve_ntcc(const size_t i,
                                std::ostream* const log,
                                const alg2_options_t ao,
                                const size_t sb_rounds,
                                const FloatingPoint::uint_t sec,
                                const RandGen::vec_eseed_t& seeds) const {
      assert(i < numntcc);
      const graph_t G = conflictgraph(i);
      assert(G.n() == ccvec[ntcc[i]-1].size());
      const size_t upper_B = std::min(ntvar[i].size(), G.n()-1);
      const Bounds B{DI::downwards, Bounds::choose_u{}, {upper_B, false}};
      BC2SAT solver(G, B);
      const auto res = solver.sat_solve(log, ao, sb_rounds, sec, seeds);
      if (res.rt != ResultType::exact) {
        if (log)
          *log << "FAILURE to solve non-trivial component " << i << ": "
               << res.rt << std::endl;
        std::stringstream ss;
        ss << "GlobRepl::solve_ntcc: can not solve nt-component " << i
           << ", return-code \"" << res.rt << "\"";
        throw std::runtime_error(ss.str());
      }
      if (not res.solution) return {};
      else return Bicliques::bcc2CNF(res.bcc, G.n());
    }

    varlist_t first_nonpure(const size_t i, const size_t ns) const {
      assert(i < numntcc);
      varlist_t res = ntvar[i];
      const auto pure = [this](const DimacsTools::Var v){
        return occ[v].pure();
      };
      std::erase_if(res, pure);
      res.resize(std::min(ns, res.size()));
      return res;
    }

    typedef DimacsTools::FormalClauseList FormalClauseList;
    FormalClauseList solve(std::ostream* const log,
                           const alg2_options_t ao,
                           const size_t sb_rounds,
                           const FloatingPoint::uint_t sec,
                           RandGen::vec_eseed_t seeds) const {
      FormalClauseList res;
      res.F.first = F.G().first;
      res.F.second.resize(res.F.first.c);
      size_t n = 0;
      seeds.push_back({});
      for (size_t i = 0; i < numntcc; ++i) {
        seeds.back() = i;
        const DimacsClauseList Fi = solve_ntcc(i,log,ao,sb_rounds,sec,seeds);
        if (Fi.first.c == 0) { // use old variables and clauses
          const varlist_t V = first_nonpure(i, -1);
          res.V.insert(V.begin(), V.end());
          n += V.size();
          assert(res.V.size() == n);
          const size_t cc = ntcc[i];
          const auto& clause_indices = ccvec[cc-1];
          const size_t c = clause_indices.size();
          for (size_t j = 0; j < c; ++j) {
            const size_t index = clause_indices[j];
            const auto& C0 = F.G().second[index];
            DimacsTools::Clause C; C.reserve(C0.size());
            for (const auto x : C0)
              if (not occ[x.v].pure()) C.push_back(x);
            res.F.second[index] = std::move(C);
          }
        }
        else {
          const varlist_t V = first_nonpure(i, Fi.first.n);
          {const size_t ni = V.size();
           assert(ni == Fi.first.n);
           res.V.insert(V.begin(), V.end());
           n += ni;
          }
          assert(res.V.size() == n);
          const auto map = DimacsTools::list_as_map(V);
          const size_t cc = ntcc[i];
          const auto& clause_indices = ccvec[cc-1];
          assert(clause_indices.size() == Fi.first.c);
          for (size_t j = 0; j < Fi.first.c; ++j) {
            res.F.second[clause_indices[j]] =
              DimacsTools::rename(Fi.second[j], map);
          }
        }
      }
      if (res.V.empty()) res.F.first.n = 0;
      else {
        auto end = res.V.end(); --end;
        res.F.first.n = end->v;
      }
      return res;
    }

    // bool operator ==(const GlobRepl& rhs) const noexcept = default;
    // GCC 10.3 compiler error CERR

  };


  enum class GCGE {
    eq=0,
    diff_O=101,
    diff_sizes=102,
    diff_comp=103,
    diff_cg=104,
    greater_than_base=105,
    diverging_values=106
  };
  // Does not consider greater_than_base or diverging_values:
  GCGE gcg_equivalence(const GlobRepl& G1, const GlobRepl& G2,
                       std::ostream* const log) noexcept {
    assert(valid(G1.F));
    assert(valid(G2.F));
    if (G1.F.O() != G2.F.O()) {
      if (log) {
        *log << "other-parts different" << std::endl;
        const auto dpars = G1.F.O().first;
        if (dpars != G2.F.O().first)
          *log << "  parameters not equal" << std::endl;
        else {
          for (size_t i = 0; i < dpars.c; ++i) {
            const auto& C1 = G1.F.O().second[i], C2 = G2.F.O().second[i];
            if (C1 != C2) {
              *log << "  clauses with index " << i+1 << " differ:\n" <<C1<<C2
                   << std::flush;
              break;
            }
          }
        }
      }
      return GCGE::diff_O;
    }
    if (G1.sizes != G2.sizes) {
      if (log) *log << "sizes of connected components differ" << std::endl;
      return GCGE::diff_sizes;
    }
    assert(G1.numntcc == G2.numntcc);
    assert(G1.ntcc == G2.ntcc);
    if (G1.ccvec != G2.ccvec) {
      if (log) *log << "components differ" << std::endl;
      return GCGE::diff_comp;
    }
    for (size_t i = 0; i < G1.numntcc; ++i)
      if (G1.conflictgraph(i) != G2.conflictgraph(i)) {
        if (log) *log << "conflict-graphs " << i << " differ" << std::endl;
        return GCGE::diff_cg;
      }
    return GCGE::eq;
  }

}

#endif
