// Oliver Kullmann, 28.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General definitions

*/

#ifndef RPLTREES_luQ83eIcqO
#define RPLTREES_luQ83eIcqO

#include <limits>
#include <type_traits>
#include <vector>
#include <algorithm>

#include <cassert>
#include <cstdint>

namespace RPL_trees {

  typedef std::uint64_t VAR;
  constexpr VAR max_var = std::numeric_limits<VAR>::max() / 2 - 1;
  constexpr bool valid(const VAR v) noexcept { return v <= max_var; }
  constexpr bool singular(const VAR v) noexcept { return v == 0; }

  typedef std::int64_t LIT;
  static_assert(std::is_same_v<LIT, std::make_signed_t<VAR>>);
  constexpr bool valid(const LIT x) noexcept {
    if (x >= 0) return valid(x);
    else return valid(-x);
  }
  constexpr bool singular(const LIT x) noexcept { return x == 0; }
  constexpr VAR var(const LIT x) noexcept { return x >= 0 ? x : -x; }
  constexpr LIT lit(const VAR v, const int sign) noexcept {
    assert(valid(v));
    if (sign > 0) return v;
    else if (sign == 0) return 0;
    else return -LIT(v);
  }

  typedef std::vector<LIT> CL;
  bool valid(const CL& C) noexcept {
    const auto b = C.begin(), e = C.end();
    return std::is_sorted(b, e) and
      std::adjacent_find(b, e) == e and
      std::find(b, e, 0) == e and
      std::all_of(b, e, [](const LIT x){return valid(x);});
  }

  typedef std::vector<CL> CLS;
  bool valid(const CLS& F) noexcept {
    return std::all_of(F.begin(), F.end(), [](const CL& C){return valid(C);});
  }

  typedef CLS::size_type size_t;

  typedef CLS::const_pointer LitOcc;
  typedef std::vector<LitOcc> OccList;

  struct BasicStats {
    VAR n;
    size_t c, l;
  };
  BasicStats count(const CLS& F) noexcept {
    assert(valid(F));
    BasicStats res{0,F.size(),0};
    for (const CL& C : F) {
      const auto s = C.size();
      if (s != 0) {
        res.n = std::max(var(C.back()), res.n);
        res.l += s;
      }
    }
    return res;
  }

  struct Occurrences {
    const VAR n;

    Occurrences(const VAR n) : n(n), occ(2*n+1) { assert(valid(n)); }

    const OccList& operator [](const LIT x) noexcept {
      return occ[tr(x)];
    }

    void enter(const CLS& F) {
      for (const CL& C : F) {
        const LitOcc p = &C;
        for (const LIT x : C) add(x, p);
      }
    }

  private :
    size_t tr(const LIT x) const noexcept {
      assert(valid(x) and var(x) <= n);
      if (x >= 0) return x;
      else return var(x) + n;
    }
    void add(const LIT x, const LitOcc p) {
      occ[tr(x)].push_back(p);
    }
    std::vector<OccList> occ;
  };

}

#endif
