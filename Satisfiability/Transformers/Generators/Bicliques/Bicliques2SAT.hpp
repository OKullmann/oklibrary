// Oliver Kullmann, 22.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Translating biclique-related problems to SAT

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

#include <cstdint>
#include <cassert>
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

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
  constexpr bool valid(const Param p) noexcept {
    if (p.V > Param::MaxV) return false;
    if (p.B > p.E) return false;
    if (p.E > (p.V * (p.V - 1)) / 2) return false;
    const var_t bound = MaxN / 2 / p.B;
    if (p.V > bound / 2) return false; // see numvarbic below
    if (p.E > bound) return false; // see numvaredg below
    return true;
  }


  struct VarEncoding {
    typedef Graphs::AdjVecUInt graph_t;
    const graph_t& G;

    const var_t V, E, B; // vertices 0 <= v < V, edges are pairs of vertices,
                         // bicliques 0 <= b < B
    const var_t nb, ne; // number of variables for bicliques and edges
    const var_t n;

    explicit VarEncoding(const graph_t& G, const var_t B)
      : G(G), V(G.n()), E(G.m()), B(B),
        nb(numvarbic(V,B)), ne(numvaredg(E,B)), n(nb+ne) {
      if (G.type() != Graphs::GT::und)
        throw std::domain_error("ERROR[VarEncoding]: only undirected graphs");
      if (not valid(Param{V,E,B}))
        throw std::domain_error("ERROR[VarEncoding]: parameters"
                                " V=" + std::to_string(V) +
                                " E=" + std::to_string(E) +
                                " B=" + std::to_string(B));
      assert(not has_loops(G));
      assert(nb <= MaxN and ne <= MaxN and n <= MaxN);
    }

    static constexpr var_t numvarbic(const var_t V, const var_t B) noexcept {
      return 2*V*B;
    }
    static constexpr var_t numvaredg(const var_t E, const var_t B) noexcept {
      return E*B;
    }

    typedef graph_t::id_t id_t;
    var_t left(const id_t v, const id_t b) const noexcept {
      assert(v < V);
      assert(b < B);
      return 1 + b*2*V + v;
    }
    var_t right(const id_t v, const id_t b) const noexcept {
      assert(v < V);
      assert(b < B);
      return 1 + b*2*V + V + v;
    }
    var_t edge(const id_t e, const id_t b) const noexcept {
      assert(e < E);
      assert(b < B);
      return 1 + nb + b * E + e;
    }

    struct elem {
      const id_t b;
      const bool left;
      const id_t v;
      constexpr bool operator ==(const elem& rhs) const noexcept = default;
    };
    constexpr elem inv(const var_t v) const noexcept {
      assert(1 <= v and v <= nb);
      const auto [b,r] = std::lldiv(v-1, 2*V);
      assert(r >= 0 and b >= 0);
      if (id_t(r) < V) return {id_t(b),true,id_t(r)};
      else return {id_t(b),false,id_t(r)-V};
    }

    Bicliques::Bcc_frame extract_bcc(std::istream& in) const noexcept {
      assert(in);
      using DimacsTools::Lit;
      Bicliques::Bcc_frame res(B);
      for (Lit x; (x=DimacsTools::read_strict_literal(in)).v.v != 0; ) {
        const id_t v = x.v.v;
        if (v > nb or x.sign == -1) continue;
        const elem el = inv(v);
        if (el.left) res.L[el.b].l.push_back(el.v);
        else res.L[el.b].r.push_back(el.v);
      }
      assert(in);
      return res;
    }

  };


  enum class SB { basic=0, extended=1, none=2 }; // symmetry-breaking
  enum class DC { with=0, without=1 }; // Dimacs-comments
  enum class DP { with=0, without=1 }; // Dimacs-parameters
  enum class CS { with=0, without=1 }; // clause-set

  constexpr char sep = ',';
  typedef std::tuple<SB> alg_options_t;
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
}
namespace Bicliques2SAT {
  std::ostream& operator <<(std::ostream& out, const SB s) {
    switch (s) {
    case SB::basic : return out << "basic-sb";
    case SB::extended : return out << "extended-sb";
    case SB::none : return out << "no-sb";
    default : return out << "SB::UNKONW";}
  }
  std::ostream& operator <<(std::ostream& out, const DC s) {
    switch (s) {
    case DC::with : return out << "with-comments";
    case DC::without : return out << "without-comments";
    default : return out << "DC::UNKONW";}
  }
  std::ostream& operator <<(std::ostream& out, const DP s) {
    switch (s) {
    case DP::with : return out << "with-parameters";
    case DP::without : return out << "without-parameters";
    default : return out << "DP::UNKONW";}
  }
  std::ostream& operator <<(std::ostream& out, const CS s) {
    switch (s) {
    case CS::with : return out << "with-cs";
    case CS::without : return out << "without-cs";
    default : return out << "CS::UNKONW";}
  }


  struct BC2SAT {
    typedef Graphs::AdjVecUInt graph_t;
    const graph_t& G;
    typedef graph_t::vecedges_t vecedges_t;
    const vecedges_t edges;

    typedef VarEncoding enc_t;
    const enc_t enc;

    explicit BC2SAT(const graph_t& G, const var_t B) noexcept :
      G(G), edges(G.alledges()), enc(G,B) {}

    typedef VarEncoding::id_t id_t;

    // Whether edges e1, e2 can be in the same biclique
    // ("biclique-compatibility"):
    bool bccomp(const id_t e1, const id_t e2) const noexcept {
      assert(e1 < enc.E and e2 < enc.E);
      const auto [a,b] = edges[e1];
      const auto [c,d] = edges[e2];
      if (c==a or c==b or d==a or d==b) return true;
      return
        (G.adjacent(c,a) and G.adjacent(d,b)) or
        (G.adjacent(c,b) and G.adjacent(d,a));
    }

    // Compute a random maximal bc-incompatible sequences of edges
    // (every pair is incompatible), given by their indices:
    typedef std::vector<id_t> vei_t; // vector of edge-indices
    vei_t max_bcincomp(RandGen::RandGen_t& g) const {
      vei_t avail; avail.reserve(enc.E);
      for (id_t i = 0; i < enc.E; ++i) avail.push_back(i);
      RandGen::shuffle(avail.begin(), avail.end(), g);
      vei_t res;
      while (not avail.empty()) {
        const id_t e = avail.back();
        avail.pop_back();
        res.push_back(e);
        std::erase_if(avail, [&e,this](const id_t x){return bccomp(e,x);});
      }
      return res;
    }
    vei_t max_bcincomp(const id_t rounds, RandGen::RandGen_t& g) const {
      if (rounds == 0) return {};
      vei_t res = max_bcincomp(g);
      for (id_t i = 0; i < rounds-1; ++i) {
        vei_t nres = max_bcincomp(g);
        if (nres.size() > res.size()) res = std::move(nres);
      }
      return res;
    }
    void output(const vei_t& v, std::ostream& out) const {
      for (const id_t x : v) {
        assert(x < enc.E);
        const auto [a,b] = edges[x];
        out << "(" << G.name(a) << "," << G.name(b) << ")";
      }
    }

    typedef RandGen::Var Var;
    typedef RandGen::Lit Lit;
    typedef RandGen::Clause Clause;
    typedef RandGen::ClauseList ClauseList;

    // For edge {v,w} forbid to have both vertices in biclique b:
    ClauseList edge_in_bc(const id_t v, const id_t w, const id_t b) const noexcept {
      assert(v < enc.V and w < enc.V and v <= w and b < enc.B);
      ClauseList F; F.reserve(2);
      F.push_back({Lit{enc.left(v,b),-1}, Lit{enc.right(w,b),-1}});
      F.push_back({Lit{enc.left(w,b),-1}, Lit{enc.right(v,b),-1}});
      assert(F.size() == 2);
      assert(RandGen::valid(F));
      return F;
    }
    id_t num_cl_bcedges() const noexcept {
      const id_t other_edges = (enc.V * (enc.V + 1)) / 2 - enc.E;
      return enc.B * 2 * other_edges;
    }
    id_t all_edges_in_bc(std::ostream& out) const {
      id_t count = 0;
      const auto nedges = G.allnonedges(true);
      for (id_t b = 0; b < enc.B; ++b)
        for (const auto [v,w] : nedges) {
          const auto F = edge_in_bc(v,w,b);
          out << F;
          count += F.size();
        }
      assert(count == num_cl_bcedges());
      return count;
    }

    // Define the auxiliary variable edge(e,b) as equivalent to
    // having the vertices from left to right or reverse:
    ClauseList edge_def(const id_t e, const id_t b) const noexcept {
      assert(e < enc.E and b < enc.B);
      const auto [v,w] = edges[e];
      ClauseList F; F.reserve(6);
      const Lit evw{enc.edge(e,b),1},
        lv{enc.left(v,b),1}, lw{enc.left(w,b),1},
        rv{enc.right(v,b),1}, rw{enc.right(w,b),1};
      F.push_back({-evw, lv, lw}); F.push_back({-evw, lv, rv});
      F.push_back({-evw, rw, lw}); F.push_back({-evw, rw, rv});
      F.push_back({-lv, -rw, evw}); F.push_back({-lw, -rv, evw});
      assert(F.size() == 6);
      assert(RandGen::valid(F));
      return F;
    }
    id_t num_cl_defedges() const noexcept {
      return enc.B * 6 * enc.E;
    }
    id_t all_edges_def(std::ostream& out) const {
      id_t count = 0;
      for (id_t b = 0; b < enc.B; ++b)
        for (id_t e = 0; e < enc.E; ++e) {
          const auto F = edge_def(e,b);
          out << F;
          count += F.size();
        }
      assert(count == num_cl_defedges());
      return count;
    }

    // Require that edge e is covered by some biclique:
    ClauseList edge_cov(const id_t e) const noexcept {
      assert(e < enc.E);
      Clause C; C.reserve(enc.B);
      for (id_t b = 0; b < enc.B; ++b)
        C.emplace_back(Var(enc.edge(e,b)),1);
      assert(C.size() == enc.B);
      assert(RandGen::valid(C));
      return {C};
    }
    id_t num_cl_covedges() const noexcept {
      return enc.E;
    }
    id_t all_edges_cov(std::ostream& out) const {
      id_t count = 0;
      for (id_t e = 0; e < enc.E; ++e) {
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
    id_t all_basic_clauses(std::ostream& out) const {
      id_t sum = 0;
      sum += all_edges_in_bc(out);
      sum += all_edges_def(out);
      sum += all_edges_cov(out);
      assert(sum == num_basic_cl());
      return sum;
    }

    // For edge e the unit-clauses putting it into biclique b:
    ClauseList place_edge(const id_t e, const id_t b) const noexcept {
      assert(e < enc.E and b < enc.B);
      ClauseList F; F.reserve(3);
      const auto [v,w] = edges[e];
      F.push_back({Lit{enc.left(v,b),1}});
      F.push_back({Lit{enc.right(w,b),1}});
      F.push_back({Lit{enc.edge(e,b),1}});
      assert(F.size() == 3);
      assert(RandGen::valid(F));
      return F;
    }
    id_t num_cl_sb(const vei_t& sb) const noexcept {
      return 3 * sb.size();
    }
    id_t all_sbedges(const vei_t& sb, std::ostream& out) const {
      assert(sb.size() <= enc.B);
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
    id_t all_clauses(const vei_t& sb, std::ostream& out) const {
      id_t sum = 0;
      sum += all_basic_clauses(out);
      sum += all_sbedges(sb, out);
      assert(sum == num_cl(sb));
      return sum;
    }


    struct Unsatisfiable {
      const vei_t incomp;
      const id_t B;
      Unsatisfiable(const vei_t ip, const id_t B) : incomp(ip), B(B) {
        assert(ip.size() > B);
      }
    };
    RandGen::dimacs_pars operator()(std::ostream& out,
        const SB sb, const DC dc, const DP dp, const CS cs,
        const id_t sb_rounds = default_sb_rounds,
        const RandGen::vec_eseed_t& seeds = {RandGen::to_eseed("t")}) const {

      const vei_t sbv = sb == SB::none ? vei_t{} :
      [&sb_rounds, &seeds, this]{
        RandGen::RandGen_t g(seeds);
        return max_bcincomp(sb_rounds, g);}();
      if (sbv.size() > enc.B) throw Unsatisfiable(sbv, enc.B);
      const RandGen::dimacs_pars res{enc.n, num_basic_cl() + num_cl_sb(sbv)};

      if (dc == DC::with) {
        using Environment::DWW; using Environment::DHW;
        out <<
          DHW{"Parameters"} <<
          DWW{"V"} << enc.V << "\n" <<
          DWW{"E"} << enc.E << "\n" <<
          DWW{"B"} << enc.B << "\n" <<
          DWW{"sb-option"} << sb << "\n" <<
          DHW{"Formatting"} <<
          DWW{"comments-option"} << dc << "\n" <<
          DWW{"parameter-option"} << dp << "\n" <<
          DWW{"clauses-option"} << cs << "\n";
        if (sb != SB::none) {
          out <<
            DHW{"Symmetry Breaking"} <<
            DWW{"planted-edges"} << sbv.size() << "\n" <<
            DWW{"num_e-seeds"} << seeds.size() << "\n" <<
            DWW{" e-seeds"} << RandGen::ESW{seeds} << "\n";
        }

        // XXX
      }

      if (dp == DP::with) {
        out << res;
      }

      if (cs == CS::with) {
        all_clauses(sbv, out);
      }

      return res;
    }

  };


  enum class Error {
    missing_parameters = 1,
    found_unsat = 20,
  };

}

#endif
