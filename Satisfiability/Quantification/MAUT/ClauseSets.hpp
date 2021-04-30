// Oliver Kullmann, 29.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Clauses and (multi-)clause-sets

*/

#ifndef CLAUSESETS_AyFIp7S1Y2
#define CLAUSESETS_AyFIp7S1Y2

#include <vector>
#include <algorithm>
#include <ostream>
#include <string>
#include <set>
#include <exception>

#include "VarLit.hpp"

namespace MAUT {

  typedef std::vector<LIT> CL;
  bool valid(const CL& C) noexcept {
    if (C.empty()) return true;
    const auto b = C.begin(), e = C.end();
    return
      std::all_of(b, e, [](const LIT x){return valid(x);}) and
      std::none_of(b, e, [](const LIT x){return singular(x);}) and
      std::is_sorted(b, e, comp) and
      std::adjacent_find(b, e) == e and
      std::adjacent_find(b, e, [](LIT x, LIT y){return y==-x;}) == e;
  }


  typedef std::vector<CL> CLS;
  bool valid(const CLS& F) noexcept {
    return std::all_of(F.begin(), F.end(), [](const CL& C){return valid(C);});
  }

  typedef CLS::size_type size_t;

  typedef CLS::const_pointer LitOcc;
  typedef std::vector<LitOcc> OccList;


  struct BasicStats {
    VAR n = 0; // max variable-index
    VAR no = 0; // number of occurring variables
    VAR pv = 0; // variables occurring in exactly one sign
    size_t c=0, l=0;

    static constexpr const char* header_ = "maxn n pv c l";
    static std::string header() noexcept { return header_; }

    BasicStats() noexcept {}
    BasicStats(const VAR n, const VAR no, const VAR pv,
               const size_t c, const size_t l)
      noexcept : n(n), no(no), pv(pv), c(c), l(l) {
      assert(no <= n and pv <= no);
    }
  };
  bool operator ==(const BasicStats& lhs, const BasicStats& rhs) noexcept {
    return lhs.n == rhs.n and lhs.no == rhs.no and lhs.pv == rhs.pv
      and lhs.c == rhs.c and lhs.l == rhs.l;
  }
  std::ostream& operator <<(std::ostream& out, const BasicStats& S) {
    return out << S.n << " " << S.no << " " << S.pv << " "
               << S.c << " " << S.l;
  }

  BasicStats count(const CLS& F) noexcept {
    assert(valid(F));
    BasicStats res(0,0,0,F.size(),0);
    std::set<VAR> vars;
    std::set<LIT> lits;
    for (const CL& C : F) {
      const auto s = C.size();
      if (s != 0) {
        res.n = std::max(var(C.back()), res.n);
        res.l += s;
        for (const LIT x : C) { lits.insert(x); vars.insert(var(x)); }
      }
    }
    res.no = vars.size();
    assert(res.no <= res.n and lits.size() <= 2*res.no);
    res.pv = 2*res.no - lits.size();
    return res;
  }


  struct Occurrences {
    const VAR n;

    explicit Occurrences(const VAR n) : n(n), occ(2*n+1) { assert(valid(n)); }
    Occurrences() = delete;

    const OccList& operator [](const LIT x) const noexcept {
      return occ[tr(x)];
    }
    const OccList& at(const LIT x) const {
      return occ[trc(x)];
    }

    void enter(const CLS& F) {
      for (const CL& C : F) {
        const LitOcc p = &C;
        for (const LIT x : C) add(x, p);
      }
    }

    friend bool operator ==(const Occurrences& lhs, const Occurrences& rhs) noexcept {
      return lhs.n == rhs.n and lhs.occ == rhs.occ;
    }

  private :
    size_t tr(const LIT x) const noexcept {
      assert(valid(x) and var(x) <= n);
      if (x >= 0) return x;
      else return var(x) + n;
    }
    size_t trc(const LIT x) const {
      assert(valid(x));
      if (var(x) > n)
        throw std::out_of_range(
          "Occurrences::trc, x=" + std::to_string(x) +
          ", n=" + std::to_string(n));
      if (x >= 0) return x;
      else return var(x) + n;
    }
    void add(const LIT x, const LitOcc p) {
      occ[trc(x)].push_back(p);
    }
    std::vector<OccList> occ;
  };


  struct DimPar {
    VAR n;
    size_t c;
    static constexpr const char* header_ = "pn pc";
    static std::string header() noexcept { return header_; }
  };
  constexpr bool operator ==(const DimPar lhs, const DimPar rhs) noexcept {
    return lhs.n == rhs.n and lhs.c == rhs.c;
  }
  std::ostream& operator <<(std::ostream& out, const DimPar& D) {
    return out << D.n << " " << D.c;
  }
  constexpr bool valid(const DimPar d) noexcept {
    return valid(d.n);
  }

  struct ClauseSet {
    DimPar dp;
    CLS F;
    Occurrences occ;
    BasicStats s;

    explicit ClauseSet(const DimPar& p) : dp(p), occ(dp.n) {}

    void update() { occ.enter(F); s = count(F); }

    static std::string header() noexcept {
      return DimPar::header() + " " + BasicStats::header();
    }
    friend std::ostream& operator <<(std::ostream& out, const ClauseSet& F) {
      return out << F.dp << " " << F.s;
    }
  };
  bool valid(const ClauseSet& F) noexcept {
    if (not valid(F.dp) or not valid(F.F) or count(F.F) != F.s) return false;
    Occurrences O(F.dp.n);
    O.enter(F.F);
    return F.occ == O;
  }

}

#endif
