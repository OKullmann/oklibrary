// Oliver Kullmann, 28.2.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Bicliques on their own, and coverings and partitions with them

   - DT = DimacsTools

   - from Graphs:
    - AdjVecUInt (the graph type)
    - fromAdjVec:
     - idv_t = id_t
     - list_t, edge_t


   Single bicliques:
   - struct bc_frame (an aggregate containing lists l, r for the two sides):
       member-functions no_edge, no_vertex check for triviality;
       function output(std::ostream&, AdjVecUInt G) for output of the two sides
       with their names as given by G;
       plus ==, <<.
   Various validation functions:
    - valid0/01(list_t, AdjVecUInt)
    - valid1/2(list_t)
    - valid(list_t, AdjVecUInt)
    - valid1/2(bc_frame), valid(bc_frame)

   - disjoint(bc_frame) (whether the two sides are disjoint)
   - is_star(idv_t, list_t, AdjVecUInt)
   - is_bc(bc_frame, AdjVecUInt)
   - covers(bc_frame, idv_t, idv_t)


   Lists of bicliques:
   - Bcc_frame (a concrete data type containing list L of bc_frame's)
       constructors from size and L;
       function output(std::ostream&, AdjVecUInt) for named output;
       plus ==, <<.
   - valid2(Bcc_frame), valid(Bcc_frame, AdjVecUInt)
   - disjoint(Bcc_frame) (whether all bicliques are disjoint)
   - is_bc(Bcc_frame, AdjVecUInt)
   - covers(Bcc_frame, idv_t, idv_t)
   - is_cover(Bcc_frame, AdjVecUInt)
   - is_partition(Bcc_frame, AdjVecUInt)
   - is_bcc(Bcc_frame, AdjVecUInt)
   - is_bcp(Bcc_frame, AdjVecUInt)


   Translating biclique-covers to CNFs:

   - numocc(Bcc_frame)
   - numcl(Bcc_frame)

   - bcc2CNF(Bcc_frame) -> DimacsClauseList (elements sorted)
   - CNF2bcc(DT::DimacsClauseList) -> Bcc_frame (sides sorted)

   Processing bicliques:

   - sort(bc_frame&)
   - sort(Bcc_frame&)

   - triv_trim(Bcc_frame&) (removing bicliques with one empty side)
   - trim(Bcc_frame&) (removing superfluous edges)

   - bccomp(edge_t, edge_t, AdjVecUInt) (whether two edges can be in the same
     biclique)
   - bccomp_graph_bydef(AdjVecUInt G, vecedges_t E) -> AdjVecUInt
     bccomp_graph_bydef(AdjVecUInt G) -> AdjVecUInt


TODOS:

1. Once edges-index-access for a derived form of AdjVecUInt is provided,
   try to remove the inefficient calls to alledges.

*/

#ifndef BICLIQUES_zlDSHv4COU
#define BICLIQUES_zlDSHv4COU

#include <algorithm>
#include <vector>
#include <map>
#include <iterator>
#include <utility>
#include <type_traits>
#include <string>

#include <cassert>

#include "Graphs.hpp"
#include "DimacsTools.hpp"
#include "ConflictGraphs.hpp"
#include "Algorithms.hpp"

namespace Bicliques {

  namespace DT = DimacsTools;

  typedef Graphs::AdjVecUInt AdjVecUInt;
  typedef AdjVecUInt::id_t idv_t;
  typedef AdjVecUInt::list_t list_t;
  typedef AdjVecUInt::edge_t edge_t;
  typedef AdjVecUInt::vecedges_t vecedges_t;


  struct bc_frame {
    list_t l, r;

    bool no_edge() const noexcept {return l.empty() or r.empty();}
    bool no_vertex() const noexcept {return l.empty() and r.empty();}

    bool operator ==(const bc_frame& rhs) const noexcept = default;
    friend std::ostream& operator <<(std::ostream& out, const bc_frame& bc) {
      if (bc.l.empty()) out << " |";
      else {
        for (const idv_t v : bc.l) out << v << " ";
        out << "|";
      }
      if (bc.r.empty()) out << " ";
      else for (const idv_t v : bc.r) out << " " << v;
      return out;
    }
    void output(std::ostream& out, const AdjVecUInt& G) const {
      if (l.empty()) out << " |";
      else {
        for (const idv_t v : l) out << G.name(v) << " ";
        out << "|";
      }
      if (r.empty()) out << " ";
      else for (const idv_t v : r) out << " " << G.name(v);
    }
  };
  static_assert(std::is_aggregate_v<bc_frame>);


  // Valid vertices:
  inline bool valid0(const list_t& L, const AdjVecUInt& G) noexcept {
    const auto n = G.n();
    return std::all_of(L.begin(), L.end(), [&n](const idv_t v){return v < n;});
  }
  // Sorted:
  inline bool valid1(const list_t& L) noexcept {
    return std::ranges::is_sorted(L);
  }
  // Valid sorted vertices:
  inline bool valid01(const list_t& L, const AdjVecUInt& G) noexcept {
    return valid0(L, G) and valid1(L);
  }
  // Vertices all different (given that L is sorted):
  inline bool valid2(const list_t& L) noexcept {
    assert(valid1(L));
    return std::adjacent_find(L.begin(), L.end()) == L.end();
  }
  // Valid sorted distinct vertices:
  inline bool valid(const list_t& L, const AdjVecUInt& G) noexcept {
    return valid01(L, G) and valid2(L);
  }
  // Both sides are sorted:
  bool valid1(const bc_frame& b) noexcept {
    return valid1(b.l) and valid1(b.r);
  }
  // Both sides are all-different:
  bool valid2(const bc_frame& b) noexcept {
    return valid2(b.l) and valid2(b.r);
  }
  // Both sides are valid sorted distinct vertices:
  bool valid(const bc_frame& b, const AdjVecUInt& G) noexcept {
    return valid(b.l, G) and valid(b.r, G);
  }

  // Whether the two sides are disjoint(assuming sorted vertices):
  inline bool disjoint(const bc_frame& b) noexcept {
    assert(valid1(b));
    return Algorithms::empty_intersection(b.l, b.r);
  }

  inline bool is_star(const idv_t v, const list_t& L, const AdjVecUInt& G) noexcept {
    assert(v < G.n());
    assert(valid(L, G));
    return std::ranges::includes(G.neighbours(v), L);
  }
  inline bool is_bc(const bc_frame& b, const AdjVecUInt& G) noexcept {
    assert(valid(b, G));
    assert(G.type() == Graphs::GT::und);
    if (b.l.size() <= b.r.size()) {
      const auto test = [&b, &G](const idv_t v){return is_star(v, b.r, G);};
      return std::all_of(b.l.begin(), b.l.end(), test);
    }
    else {
      const auto test = [&b, &G](const idv_t v){return is_star(v, b.l, G);};
      return std::all_of(b.r.begin(), b.r.end(), test);
    }
  }
  // Is {v,w} covered (v left or v right):
  inline bool covers(const bc_frame& b, const idv_t v, const idv_t w) noexcept {
    const auto lb = b.l.begin(); const auto le = b.l.end();
    const auto rb = b.r.begin(); const auto re = b.r.end();
    return
      (std::binary_search(lb,le,v) and std::binary_search(rb,re,w)) or
      (std::binary_search(lb,le,w) and std::binary_search(rb,re,v));
  }


  struct Bcc_frame {
    typedef std::vector<bc_frame> v_t;
    v_t L;

    Bcc_frame() noexcept = default;
    explicit Bcc_frame(const idv_t n) : L(n) {}
    Bcc_frame(v_t L) noexcept : L(L) {}

    bool empty() const noexcept { return L.empty(); }

    bool operator ==(const Bcc_frame& rhs) const noexcept = default;
    friend std::ostream& operator <<(std::ostream& out, const Bcc_frame& bcc) {
      for (const auto& bc : bcc.L) out << bc << "\n";
      return out;
    }
    void output(std::ostream& out, const AdjVecUInt& G) const {
      for (const auto& bc : L) {
        bc.output(out, G); out << "\n";
      }
    }
  };
  inline bool valid2(const Bcc_frame& B) noexcept {
    return std::all_of(B.L.begin(), B.L.end(),
                       [](const bc_frame& b){return valid2(b);});
  }
  inline bool valid(const Bcc_frame& B, const AdjVecUInt& G) noexcept {
    return std::all_of(B.L.begin(), B.L.end(),
                       [&G](const bc_frame& b){return valid(b, G);});
  }

  inline bool is_bc(const Bcc_frame& B, const AdjVecUInt& G) noexcept {
    assert(valid(B, G));
    return std::all_of(B.L.begin(), B.L.end(),
                       [&G](const bc_frame& b){return is_bc(b, G);});
  }
  inline bool covers(const Bcc_frame& B, const idv_t v, const idv_t w) noexcept {
    return std::any_of(B.L.begin(), B.L.end(),
                       [v,w](const bc_frame& b){return covers(b,v,w);});
  }
  inline bool is_cover(const Bcc_frame& B, const AdjVecUInt& G) noexcept {
    assert(valid(B, G));
    const auto E = G.alledges();
    return std::all_of(E.begin(), E.end(),
                       [&B](const auto e){return covers(B,e.first,e.second);});
  }
  inline bool is_partition(const Bcc_frame& B, const AdjVecUInt& G) noexcept {
    assert(valid(B, G));
    const auto E = G.alledges();
    for (const auto [v,w] : E) {
      idv_t count = 0;
      for (const auto& b : B.L)
        if (covers(b, v, w)) {
          ++count;
          if (count == 2) return false;
        }
      if (count != 1) return false;
    }
    return true;
  }
  inline bool is_bcc(const Bcc_frame& B, const AdjVecUInt& G) noexcept {
    return is_bc(B, G) and is_cover(B, G);
  }
  inline bool is_bcp(const Bcc_frame& B, const AdjVecUInt& G) noexcept {
    return is_bc(B, G) and is_partition(B, G);
  }

  inline bool disjoint(const Bcc_frame& B) noexcept {
    return std::all_of(B.L.begin(), B.L.end(),
                       [](const bc_frame& b){return Bicliques::disjoint(b);});
    }


  // The number of literal occurrences:
  idv_t numocc(const Bcc_frame& B) noexcept {
    idv_t res = 0;
    for (const bc_frame& b : B.L) { res += b.l.size(); res += b.r.size(); }
    return res;
  }

  // The number of clauses, i.e., the maximal-vertex + 1 in B:
  idv_t numcl(const Bcc_frame& B) noexcept {
    idv_t res = 0;
    for (const bc_frame& b : B.L) {
      for (const idv_t v : b.l) res = std::max(v+1, res);
      for (const idv_t v : b.r) res = std::max(v+1, res);
    }
    return res;
  }


  // c is the minimum number of clauses to be used (clauses not mentioned
  // in B are then empty):
  DT::DimacsClauseList bcc2CNF(const Bcc_frame& B, const idv_t c) {
    assert(c >= numcl(B));
    const DT::dimacs_pars dp{B.L.size(), c};
    DT::ClauseList F(dp.c);
    for (DT::var_t v = 0; v < dp.n; ++v) {
      for (const idv_t i : B.L[v].l) F[i].push_back(DT::Lit{v+1,1});
      for (const idv_t i : B.L[v].r) F[i].push_back(DT::Lit{v+1,-1});
    }
    return {dp, F};
  }
  Bcc_frame CNF2bcc(const DT::DimacsClauseList& F) {
    const auto n = F.first.n;
    assert(F.first.c == F.second.size());
    Bcc_frame B(n);
    for (idv_t i = 0; i < F.first.c; ++i)
      for (const DT::Lit x : F.second[i]) {
        const auto v = x.v.v;
        assert(1 <= v and v <= n);
        if (x.s) B.L[v-1].l.push_back(i);
        else B.L[v-1].r.push_back(i);
      }
    return B;
  }


  void sort(bc_frame& b) noexcept {
    std::ranges::sort(b.l); std::ranges::sort(b.r);
  }
  void sort(Bcc_frame& B) noexcept {
    for (bc_frame& b : B.L) sort(b);
  }

  // Returns the number of eliminated bicliques (since no edge in them):
  idv_t triv_trim(Bcc_frame& B) noexcept {
    const auto old_size = B.L.size();
    std::erase_if(B.L, [](const auto& b){return b.no_edge();});
    return old_size - B.L.size();
  }


  /*
    Trimming: removing vertices from bicliques, when all their edges
    are covered elsewhere.

    When going through the vertices of a single biclique, the order
    does not matter -- however the order of the bicliques matter, so
    running through various permutations could yield a better result.

    Returns the number of reduced literal-occurrences.
  */
  idv_t trim(Bcc_frame& B) {
    assert(valid2(B));
    const idv_t orig = numocc(B);
    triv_trim(B);
    using edge_t = Graphs::AdjVecUInt::edge_t;
    const auto e = [](idv_t v, idv_t w){
      if (v<=w) return edge_t{v,w}; else return edge_t{w,v};
    };
    std::map<edge_t, idv_t> M;
    for (const auto& b : B.L)
      for (const auto v : b.l)
        for (const auto w : b.r)
          ++M[e(v,w)];

    for (bc_frame& b : B.L) {
      {std::vector<idv_t> reml;
       for (const idv_t v : b.l) {
         const auto t = [&e,&M,&v](idv_t w){return M[e(v,w)] >= 2;};
         if (std::all_of(b.r.begin(), b.r.end(), t)) {
           reml.push_back(v);
           for (const auto w : b.r) --M[e(v,w)];
         }
       }
       assert(reml.size() <= b.l.size());
       std::vector<idv_t> diff; diff.reserve(b.l.size() - reml.size());
       std::ranges::set_difference(b.l, reml, std::back_inserter(diff));
       b.l = std::move(diff);
      }
      {std::vector<idv_t> remr;
       for (const idv_t w : b.r) {
         const auto t = [&e,&M,&w](idv_t v){return M[e(v,w)] >= 2;};
         if (std::all_of(b.l.begin(), b.l.end(), t)) {
           remr.push_back(w);
           for (const auto v : b.l) --M[e(v,w)];
         }
       }
       assert(remr.size() <= b.r.size());
       std::vector<idv_t> diff; diff.reserve(b.r.size() - remr.size());
       std::ranges::set_difference(b.r, remr, std::back_inserter(diff));
       b.r = std::move(diff);
      }
    }

    triv_trim(B);
    return orig - numocc(B);
  }


  // Whether edges e1, e2 can be in the same biclique
  // ("biclique-compatibility"):
  inline bool bccomp(const edge_t e1, const edge_t e2,
                     const AdjVecUInt& G) noexcept {
    const auto [a,b] = e1;
    const auto [c,d] = e2;
    if (c==a or c==b or d==a or d==b) return true;
    return
      (G.adjacent(c,a) and G.adjacent(d,b)) or
      (G.adjacent(c,b) and G.adjacent(d,a));
  }

  // The vertices are the edges of G in E (via their indices), with an
  // edge between them iff bccomp holds:
  AdjVecUInt bccomp_graph_bydef(const AdjVecUInt& G, const vecedges_t& E,
                                const std::string& sep) {
    assert(not has_loops(G));
    const idv_t n = E.size();
    AdjVecUInt res(Graphs::GT::und, n);
    if (n <= 1) return res;;
    AdjVecUInt::adjlist_t A(n);
    for (idv_t i = 0; i < n-1; ++i) {
      auto& row = A[i];
      const edge_t& e1 = E[i];
      for (idv_t j = i+1; j < n; ++j) {
        const edge_t& e2 = E[j];
        if (bccomp(e1, e2, G)) {
          row.push_back(j);
          A[j].push_back(i);
        }
      }
    }
    res.set(A);
    if (not sep.empty()) {
      res.set_names();
      for (idv_t i = 0; i < E.size(); ++i) {
        const auto& [v, w] = E[i];
        res.set_name(i, G.with_names() ? G.name(v) + sep + G.name(w) :
                        std::to_string(v) + sep + std::to_string(w));
      }
    }
    return res;
  }
  AdjVecUInt bccomp_graph_bydef(const AdjVecUInt& G,
                                const std::string& sep = "") {
    return bccomp_graph_bydef(G, G.alledges(), sep);
  }

}

#endif
