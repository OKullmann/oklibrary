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
    return std::all_of(L.begin(), L.end(), test);
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

}

#endif
