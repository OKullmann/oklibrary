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

#include <cassert>

#include "VarLit.hpp"
#include "ClauseSets.hpp"
#include "PartialAssignments.hpp"
#include "Counting.hpp"

namespace MAUT {

  struct BasicStatsTrees {
    typedef MAUT::size_t size_t;
    size_t nds, lvs;

    BasicStatsTrees() noexcept : nds(0), lvs(0) {}
    BasicStatsTrees(const size_t n, const size_t l)
      noexcept : nds(n), lvs(l) {}

    static constexpr const char* header_ = "nds lvs";
    static std::string header() noexcept { return header_; }
  };
  bool operator ==(const BasicStatsTrees& lhs, const BasicStatsTrees& rhs) noexcept {
    return lhs.nds == rhs.nds and lhs.lvs == rhs.lvs;
  }
  std::ostream& operator <<(std::ostream& out, const BasicStatsTrees& S) {
    return out << S.nds << " " << S.lvs;
  }

  template <class COUNT>
  struct RetBack {
    typedef COUNT count_t;
    BasicStatsTrees t;
    count_t ca{}; // the number of maximal falsifying assignments
    size_t cn{}; // the number of leaves where such were found

    RetBack() {}

    static std::string header() noexcept {
      return BasicStatsTrees::header() + " " + count_header;
    }
    friend bool operator ==(const RetBack& lhs, const RetBack& rhs) noexcept {
      return lhs.t == rhs.t and lhs.ca == rhs.ca and lhs.cn == rhs.cn;
    }
    friend std::ostream& operator <<(std::ostream& out, const RetBack& S) {
      return out << S.t << " " << S.ca << " " << S.cn;
    }

  };


  template <class COUNT>
  struct FirstOpen {
    typedef COUNT count_t;
    typedef RetBack<count_t> ret_t;
    ret_t result;

    const ClauseSet& F;

    FirstOpen(const ClauseSet& F) noexcept : F(F) { assert(valid(F)); }
    FirstOpen(const FirstOpen& F) = delete;

    void solve() {
      Pass pa(F.dp.n);
      solve(pa, F.dp.n);
    }

    static VAR firstopen(const Pass& pa) noexcept {
      assert(pa.n >= 1);
      VAR v;
      for (v = 1; pa[v] != PA::o; ++v);
      return v;
    }

    void solve(Pass pa, size_t n) {
      result.t.nds += 1;
      if (n == 0) {
        result.t.lvs += 1;
        if (maximal_falsified(pa, F.F)) {
          result.ca.add(countb(pa, F)); ++result.cn;
        }
        return;
      }
      n -= add_pure(pa, F);
      if (n == 0) {
        result.t.lvs += 1;
        if (maximal_falsified(pa, F.F)) {
          result.ca.add(countb(pa, F)); ++result.cn;
        }
        return;
      }
      const VAR open = firstopen(pa); --n;
      pa[open] = PA::t; solve(pa, n);
      pa[open] = PA::f; solve(pa, n);
    }

  };

}

#endif
