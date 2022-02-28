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

namespace Bicliques {

  typedef Graphs::AdjVecUInt AdjVecUInt;
  typedef AdjVecUInt::id_t id_t;
  typedef AdjVecUInt::list_t list_t;

  struct bc_frame {
    list_t l, r;
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
    std::vector<bc_frame> L;
    Bcc_frame() noexcept = default;
    explicit Bcc_frame(const id_t n) : L(n) {}
    explicit Bcc_frame(std::vector<bc_frame> L) noexcept : L(L) {}
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

}

#endif
