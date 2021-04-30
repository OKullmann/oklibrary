// Oliver Kullmann, 30.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Algorithms for creating the MAUT-trees

*/

#ifndef BACKTRACKING_O3vnPCe44r
#define BACKTRACKING_O3vnPCe44r

#include <string>
#include <ostream>

#include "VarLit.hpp"
#include "ClauseSets.hpp"
#include "PartialAssignments.hpp"

namespace MAUT {

  struct BasicStatsTrees {
    typedef MAUT::size_t size_t;
    size_t nds, lvs;

    static constexpr const char* header_ = "nds lvs";
    static std::string header() noexcept { return header_; }
  };
  bool operator ==(const BasicStatsTrees& lhs, const BasicStatsTrees& rhs) noexcept {
    return lhs.nds == rhs.nds and lhs.lvs == rhs.lvs;
  }
  std::ostream& operator <<(std::ostream& out, const BasicStatsTrees& S) {
    return out << S.nds << " " << S.lvs;
  }


  MAUT::VAR firstopen(const MAUT::Pass& pa) noexcept {
    assert(pa.n >= 1);
    VAR v;
    for (v = 1; pa[v] != PA::o; ++v);
    return v;
  }

  // Assuming that the input is reduced and non-trivial:
  BasicStatsTrees first_open(MAUT::Pass pa, const MAUT::ClauseSet& F, size_t n) {
    assert(n == F.dp.n - pa.size());
    assert(n >= 1);
    Pass orig(pa);
    BasicStatsTrees res{1,0};
    using MAUT::VAR; using MAUT::PA;
    const VAR open = firstopen(pa);
    --n;

    pa[open] = PA::t;
    {const size_t reduced_1 = MAUT::add_pure(pa, F);
      if (n == reduced_1) { ++res.nds; ++res.lvs; }
     else {
       const BasicStatsTrees res_1 = first_open(pa, F, n - reduced_1);
       res.nds += res_1.nds; res.lvs += res_1.lvs;
     }
    }

    orig[open] = PA::f;
    {const size_t reduced_2 = MAUT::add_pure(orig, F);
     if (n == reduced_2) { ++res.nds; ++res.lvs; }
     else {
       const BasicStatsTrees res_2 = first_open(orig, F, n - reduced_2);
       res.nds += res_2.nds; res.lvs += res_2.lvs;
     }
    }

    return res;
  }

}

#endif
