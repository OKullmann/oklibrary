// Oliver Kullmann, 22.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Translating biclique-related problems to SAT

   - var_t
   - MaxN

   - class Param, valid(Param)

   - class VarEncoding

   - scoped enums SB, DC, DP CS
   - const char sep, alg_options_t, format_options_t
   - default_sb_rounds
   - scoped enum Error

   - class BC2SAT


   - General helper functions
    - erase_if_byswap : faster than erase_if due to not keeping the order

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
  constexpr bool valid(const Param p) noexcept {
    if (p.V > Param::MaxV) return false;
    if (p.B > p.E) return false;
    if (p.E > (p.V * (p.V - 1)) / 2) return false;
    if (p.B == 0) return true;
    const var_t bound = MaxN / 2 / p.B;
    if (p.V > bound / 2) return false; // see numvarbic below
    if (p.E > bound) return false; // see numvaredg below
    return true;
  }


  struct VarEncoding {
    typedef Graphs::AdjVecUInt graph_t;
    const graph_t& G;

    const var_t V, E; // vertices 0 <= v < V, edges are pairs of vertices,

    typedef DimacsTools::Lit Lit;
    const DimacsTools::Lit_filter lf;

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
      assert(newB < B_);
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
    Bcc_frame post_process(Bcc_frame& res) const noexcept {
      assert(valid(res,G));
      assert(disjoint(res));
      assert(is_bcc(res,G));
      return res;
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
    Bcc_frame core_extraction(const std::vector<Lit>& pa) const {
      return core_rextraction(pa);
    }
    template <class RAN>
    Bcc_frame rextract_bcc(const RAN& pa) const {
      Bcc_frame res = core_rextraction(pa);
      return post_process(res);
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
    default : return out << "SB::UNKNOWN";}
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
    unknown = 0,
    init_unsat_sb = 1,
    init_unsat = 2,
    init_timeout = 3,
    final_timeout = 4,
    aborted = 5,
    exact = 6,
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
    typedef graph_t::vecedges_t vecedges_t;
    const vecedges_t edges;

    typedef VarEncoding enc_t;
    typedef VarEncoding::id_t id_t;

    explicit BC2SAT(const graph_t& G, const id_t B) noexcept :
      G(G), edges(G.alledges()), enc_(G,B) {}


    const enc_t& enc() const noexcept { return enc_; }
    void update_B(const var_t newB) noexcept {
      assert(newB < enc_.B());
      enc_.update_B(newB);
    }


    // Compute a random maximal bc-incompatible sequences of edges
    // (every pair is incompatible), given by their indices:
    typedef std::vector<id_t> vei_t; // vector of edge-indices
    vei_t max_bcincomp(RandGen::RandGen_t& g) const {
      vei_t avail; avail.reserve(enc_.E);
      for (id_t i = 0; i < enc_.E; ++i) avail.push_back(i);
      RandGen::shuffle(avail.begin(), avail.end(), g);
      vei_t res;
      while (not avail.empty()) {
        const id_t e = avail.back(); avail.pop_back();
        res.push_back(e);
        const auto ee = edges[e];
        erase_if_byswap(avail,
          [&ee,this](const id_t x){
                     return Bicliques::bccomp(ee,edges[x], G);});
      }
      return res;
    }
    typedef GenStats::BasicStats<id_t, FloatingPoint::float80> stats_t;
    std::pair<vei_t, stats_t>
        max_bcincomp(const id_t rounds, RandGen::RandGen_t& g) const {
      if (rounds == 0) return {};
      vei_t res = max_bcincomp(g);
      stats_t S; S += res.size();
      for (id_t i = 0; i < rounds-1; ++i) {
        vei_t nres = max_bcincomp(g); const auto s = nres.size();
        S += s; if (s > res.size()) res = std::move(nres);
      }
      return {res, S};
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

    // For edge {v,w} forbid to have both vertices in biclique b:
    ClauseList edge_in_bc(const id_t v, const id_t w, const id_t b) const noexcept {
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
    id_t all_edges_in_bc(std::ostream& out) const {
      id_t count = 0;
      const auto nedges = G.allnonedges(true);
      for (id_t b = 0; b < enc_.B(); ++b)
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
      assert(e < enc_.E and b < enc_.B());
      const auto [v,w] = edges[e];
      ClauseList F; F.reserve(6);
      const Lit evw{enc_.edge(e,b),1},
        lv{enc_.left(v,b),1}, lw{enc_.left(w,b),1},
        rv{enc_.right(v,b),1}, rw{enc_.right(w,b),1};
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
        const alg_options_t ao, const format_options_t fo,
        const id_t sb_rounds,
        const RandGen::vec_eseed_t& seeds = {RandGen::to_eseed("t")}) const {
      const SB sb = std::get<SB>(ao);
      const DC dc = std::get<DC>(fo);
      const DP dp = std::get<DP>(fo);
      const CS cs = std::get<CS>(fo);

      const auto [sbv, sbs] = sb == SB::none ?
        std::make_pair(vei_t{}, stats_t{}) :
        [&sb_rounds, &seeds, this]{
          RandGen::RandGen_t g(seeds);
          return max_bcincomp(sb_rounds, g);}();
      if (sbv.size() > enc_.B()) throw Unsatisfiable(sbv, enc_.B());
      const RandGen::dimacs_pars res{enc_.n(), num_cl(sbv)};

      if (dc == DC::with) {
        using Environment::DWW; using Environment::DHW;
        out <<
          DHW{"Parameters"} <<
          DWW{"V"} << enc_.V << "\n" <<
          DWW{"E"} << enc_.E << "\n" <<
          DWW{"B"} << enc_.B() << "\n" <<
          DWW{"sb-option"} << sb << "\n" <<
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


    struct result_t {
      Bicliques::Bcc_frame bcc;
      id_t B;
      ResultType rt;
      const id_t init_B;
      explicit result_t(const id_t B) noexcept
        : B(B), rt(ResultType::unknown), init_B(B) {}

      void output(std::ostream& out, const Graphs::AdjVecUInt& G) const {
        assert(int(rt) >= 1 and int(rt) <= 6);
        out << "bcc";
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

    result_t operator()(std::ostream* const log,
        const alg_options_t ao, const id_t sb_rounds,
        const FloatingPoint::uint_t sec,
        const RandGen::vec_eseed_t& seeds = {RandGen::to_eseed("t")}) {
      result_t res(enc_.B());
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
        const RandGen::dimacs_pars dp{enc_.n(), num_cl(sbv)};
        const std::string inp = filename_head + std::to_string(enc_.B())
          + ".dimacs";
        {std::ofstream file(inp);
         if (not file)
           throw std::runtime_error(
             "Bicliques2SAT::operator(...): can not open output-file \"" +
             inp + "\"");
         file << dp;
         all_clauses(sbv, file);
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
