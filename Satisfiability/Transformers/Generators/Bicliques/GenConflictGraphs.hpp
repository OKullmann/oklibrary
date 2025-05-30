// Oliver Kullmann, 25.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Conflict graphs of nonbolean clause-sets etc.
  (generalising ConflictGraphs.hpp)

  Tested in TestConflictGraphs.cpp.

*/

#ifndef GENCONFLICTGRAPHS_BZP7YhQAWS
#define GENCONFLICTGRAPHS_BZP7YhQAWS

#include <vector>
#include <utility>
#include <array>

#include <cassert>

#include <Transformers/Generators/Random/GenClauseSets.hpp>

namespace GenConflictGraphs {

  namespace GL = GenLit;
  namespace GC = GenClauses;
  namespace GCS = GenClauseSets;


  // Generalising ConflictGraphs::OccVar :
  struct GOccVar {
    typedef GL::var_t var_t;
    typedef GL::val_t val_t;
    typedef GCS::count_t count_t;

    typedef std::vector<count_t> lit_occ_t; // clause-index
    typedef std::vector<lit_occ_t> var_occ_t; // value -> clause-list

    var_t v = GL::singvar;
    var_occ_t O;

    // If a clause contains several literals with variable v, then only the
    // first occurrence is considered (by C.val(v)):
    static var_occ_t transfer(const GCS::GClauseList& F, const var_t v) {
      const var_t n = F.n();
      if (v >= n) return {};
      const val_t D = F.dom[v];
      var_occ_t res(D);
      for (count_t i = 0; i < F.c(); ++i) {
        const auto& C = F.F[i];
        const val_t val = C.val(v);
        if (val == GL::singval) continue;
        assert(val < D);
        res[val].push_back(i);
      }
      return res;
    }

    constexpr GOccVar() noexcept = default;
    constexpr GOccVar(const var_t v) noexcept : v(v), O{} {}
    explicit GOccVar(var_occ_t O) noexcept : O(std::move(O)) {}
    explicit GOccVar(const var_t v, var_occ_t O) noexcept :
      v(v), O(std::move(O)) {}
    explicit GOccVar(const GCS::GClauseList& F, const var_t v) :
      v(v), O(transfer(F,v)) {}

    constexpr var_t var() const noexcept { return v; }
    constexpr val_t dom() const noexcept { return O.size(); }

    lit_occ_t& operator[](const val_t e) noexcept { return O[e]; }
    const lit_occ_t& operator[](const val_t e) const noexcept { return O[e]; }

    lit_occ_t all_occurrences() const { return Algorithms::rmerge(O); }

    // Frequencies of domain-sizes 0, 1, >= 2:
    typedef std::array<val_t, 3> frequencies_t;
    constexpr static frequencies_t freqs(const var_occ_t& o) noexcept {
      frequencies_t res{};
      for (val_t i = 0; i < o.size(); ++i) {
        const count_t s = o[i].size();
        if (s <= 1) ++ res[s];
        else ++res[2];
      }
      return res;
    }

    // freqs == {0,0,0} (or, equivalently, deg() == 0) :
    constexpr bool formal() const noexcept {
      return O.empty();
    }
    // freqs == {>=1, 0, 0} :
    constexpr bool trivial() const noexcept {
      return not formal() and
        std::ranges::all_of(O, [](const auto& L)noexcept{return L.empty();});
    }
    // freqs[0] >= 1 and freqs[1]+freqs[2] >= 1 :
    constexpr bool pure() const noexcept {
      return not trivial() and
        std::ranges::any_of(O, [](const auto& L)noexcept{return L.empty();});
    }
    // freqs[0] + freqs[2] == 0 and freqs[1] >= 1 :
    constexpr bool osingular() const noexcept {
      return not formal() and
        std::ranges::all_of(O, [](const auto& L)noexcept{return L.size()==1;});
    }
    // Remark: osingular() implies not pure().
    constexpr bool singular() const noexcept {
      if (formal()) return false;
      const frequencies_t f = freqs(O);
      return f[0] == 0 and f[2] <= 1;
    }
    // Remark: osingular() implies singular().
    constexpr bool nonosingular() const noexcept {
      return singular() and not osingular();
    }
    constexpr bool nonsingular() const noexcept {
      return not formal() and not trivial() and not pure() and not singular();
    }

    constexpr count_t deg() const noexcept {
      return Algorithms::sum_sizes(O);
    }

    constexpr auto operator <=>(const GOccVar&) const noexcept = default;

  };
  static_assert(GOccVar{}.var() == GL::singvar);
  static_assert(GOccVar{}.dom() == 0);
  static_assert(GOccVar{}.formal());
  static_assert(GOccVar{} == GOccVar());
  static_assert(GOccVar(GL::singvar) == GOccVar());
  static_assert(GOccVar(0) < GOccVar(1));
  static_assert(GOccVar(1) < GOccVar());
  static_assert(GOccVar{1} == GOccVar(1));
  static_assert(GOccVar(1).var() == 1);
  static_assert(GOccVar(1).dom() == 0);
  static_assert(GOccVar{1}.formal());
}

#endif
