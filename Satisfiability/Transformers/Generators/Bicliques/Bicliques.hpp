// Oliver Kullmann, 28.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Bicliques on their own, and coverings and partitions with them

*/

#ifndef BICLIQUES_zlDSHv4COU
#define BICLIQUES_zlDSHv4COU

#include <algorithm>
#include <vector>

#include <cassert>

#include "Graphs.hpp"
#include "DimacsTools.hpp"

namespace Bicliques {

  namespace DT = DimacsTools;

  typedef Graphs::AdjVecUInt AdjVecUInt;
  typedef AdjVecUInt::id_t id_t;
  typedef AdjVecUInt::list_t list_t;

  struct bc_frame {
    list_t l, r;
    bool operator ==(const bc_frame& rhs) const noexcept = default;
    friend std::ostream& operator <<(std::ostream& out, const bc_frame& bc) {
      if (bc.l.empty()) out << " |";
      else {
        for (const id_t v : bc.l) out << v << " ";
        out << "|";
      }
      if (bc.r.empty()) out << " ";
      else for (const id_t v : bc.r) out << " " << v;
      return out;
    }
  };
  inline bool valid(const list_t& L, const AdjVecUInt& G) noexcept {
    const auto n = G.n();
    const auto test = [&n](const id_t v){return v < n;};
    return std::all_of(L.begin(), L.end(), test) and
      std::ranges::is_sorted(L);
  }
  bool valid(const bc_frame& b, const AdjVecUInt& G) noexcept {
    return valid(b.l, G) and valid(b.r, G);
  }

  inline bool is_star(const id_t v, const list_t& L, const AdjVecUInt& G) noexcept {
    assert(v < G.n());
    assert(valid(L, G));
    return std::ranges::includes(G.neighbours(v), L);
  }
  inline bool is_bc(const bc_frame& b, const AdjVecUInt& G) noexcept {
    assert(valid(b, G));
    assert(G.type() == Graphs::GT::und);
    if (b.l.size() <= b.r.size()) {
      const auto test = [&b, &G](const id_t v){return is_star(v, b.r, G);};
      return std::all_of(b.l.begin(), b.l.end(), test);
    }
    else {
      const auto test = [&b, &G](const id_t v){return is_star(v, b.l, G);};
      return std::all_of(b.r.begin(), b.r.end(), test);
    }
  }
  inline bool covers(const bc_frame& b, const id_t v, const id_t w) noexcept {
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
    explicit Bcc_frame(const id_t n) : L(n) {}
    Bcc_frame(v_t L) noexcept : L(L) {}
    bool operator ==(const Bcc_frame& rhs) const noexcept = default;
    friend std::ostream& operator <<(std::ostream& out, const Bcc_frame& bcc) {
      for (const auto& bc : bcc.L) out << bc << "\n";
      return out;
    }
  };
  inline bool valid(const Bcc_frame& B, const AdjVecUInt& G) noexcept {
    return std::all_of(B.L.begin(), B.L.end(),
                       [&G](const bc_frame& b){return valid(b, G);});
  }
  inline bool is_bc(const Bcc_frame& B, const AdjVecUInt& G) noexcept {
    assert(valid(B, G));
    return std::all_of(B.L.begin(), B.L.end(),
                       [&G](const bc_frame& b){return is_bc(b, G);});
  }
  inline bool covers(const Bcc_frame& B, const id_t v, const id_t w) noexcept {
    return std::any_of(B.L.begin(), B.L.end(),
                       [v,w](const bc_frame& b){return covers(b,v,w);});
  }
  inline bool is_cover(const Bcc_frame& B, const AdjVecUInt& G) noexcept {
    assert(valid(B, G));
    const auto E = G.alledges();
    return std::all_of(E.begin(), E.end(),
                       [&B](const auto e){return covers(B,e.first,e.second);});
  }
  inline bool is_bcc(const Bcc_frame& B, const AdjVecUInt& G) noexcept {
    return is_bc(B, G) and is_cover(B, G);
  }


  // The maximal-vertex + 1 in B:
  id_t numcl(const Bcc_frame& B) noexcept {
    id_t res = 0;
    for (const bc_frame& b : B.L) {
      for (const id_t v : b.l) res = std::max(v+1, res);
      for (const id_t v : b.r) res = std::max(v+1, res);
    }
    return res;
  }
  DT::DimacsClauseList bcc2CNF(const Bcc_frame& B) {
    const DT::dimacs_pars dp{B.L.size(), numcl(B)};
    DT::ClauseList F(dp.c);
    for (DT::var_t v = 0; v < dp.n; ++v) {
      for (const id_t i : B.L[v].l) F[i].push_back(DT::Lit{v+1,1});
      for (const id_t i : B.L[v].r) F[i].push_back(DT::Lit{v+1,-1});
    }
    return {dp, F};
  }
  Bcc_frame CNF2bcc(const DT::DimacsClauseList& F) {
    const auto n = F.first.n;
    assert(F.first.c == F.second.size());
    Bcc_frame B(n);
    for (id_t i = 0; i < F.first.c; ++i)
      for (const DT::Lit x : F.second[i]) {
        const auto v = x.v.v;
        assert(1 <= v and v <= n);
        if (x.sign == 1) B.L[v-1].l.push_back(i);
        else B.L[v-1].r.push_back(i);
      }
    return B;
  }

}

#endif
