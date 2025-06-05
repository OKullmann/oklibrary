// Oliver Kullmann, 22.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Components for generalised clause-sets

*/

#ifndef GENCLAUSES_iVsas4Zamq
#define GENCLAUSES_iVsas4Zamq

#include <vector>
#include <algorithm>
#include <istream>
#include <ostream>
#include <ranges>

#include "GenLit.hpp"

namespace GenClauses {

  namespace GL = GenLit;

  typedef std::vector<GL::VarVal> gclause_t;
  struct GClause {
    gclause_t C;

    typedef GL::VarVal value_type;
    typedef GL::VarVal& reference;
    typedef const GL::VarVal& const_reference;
    typedef gclause_t::iterator iterator;
    typedef gclause_t::const_iterator const_iterator;
    typedef gclause_t::difference_type difference_type;
    typedef gclause_t::size_type size_type;
    typedef gclause_t::pointer pointer;
    typedef gclause_t::const_pointer const_pointer;

    constexpr GClause() noexcept = default;
    explicit GClause(const size_type count) : C(count) {}
    GClause(const size_type count, const GL::VarVal& x)
      : C(count,x) {}
    GClause(gclause_t C) noexcept : C(std::move(C)) {}
    template <class RAN>
    // GCC 14.3 ERROR: constructor using std::from_range not implemented:
    // GClause(std::from_range_t, RAN&& r) : C(std::from_range, r) {}
    GClause(std::from_range_t, RAN&& r) :
      C(std::ranges::to<gclause_t>(std::forward<RAN>(r))) {}
    GClause(const std::initializer_list<GL::VarVal> init)
      : C(init) {}

    iterator begin() noexcept { return C.begin(); }
    const_iterator begin() const noexcept { return C.begin(); }
    const_iterator cbegin() const noexcept { return C.cbegin(); }
    iterator end() noexcept { return C.end(); }
    const_iterator end() const noexcept { return C.end(); }
    const_iterator cend() const noexcept { return C.cend(); }

    constexpr bool empty() const noexcept { return C.empty(); }
    constexpr size_type size() const noexcept { return C.size(); }

    reference front() noexcept { return C.front(); }
    const_reference front() const noexcept { return C.front(); }
    reference back() noexcept { return C.back(); }
    const_reference back() const noexcept { return C.back(); }
    reference operator [](const size_type i) noexcept { return C[i]; }
    const_reference operator [](const size_type i) const noexcept {
      return C[i];
    }

    void clear() noexcept { C.clear(); }
    void push_back(const GL::VarVal& x) { C.push_back(x); }
    void pop_back() { C.pop_back(); }

    constexpr bool is_sorted() const noexcept {
      return std::ranges::is_sorted(C);
    }
    void sort() noexcept { std::ranges::sort(C); }

    constexpr bool has_consecutive_duplicates() const noexcept {
      return std::ranges::adjacent_find(C) != C.end();
    }
    // Returns the number of eliminated literals (the difference in size):
    size_type remove_consecutive_duplicates() noexcept {
      const auto left_over = std::ranges::unique(C);
      const size_type res = left_over.size();
      C.erase(left_over.begin(), left_over.end());
      return res;
    }

    // Returns the smallest value of a literal in C with variable v,
    // or singval is there is no such literal:
    GL::val_t val(const GL::var_t v) const noexcept {
      assert(is_sorted());
      const auto it = std::ranges::lower_bound(C, v, {}, GL::var);
      if (it == C.end()) return GL::singval;
      const auto x = *it;
      if (GL::var(x) == v) return GL::val(x);
      else return GL::singval;
    }

     iterator search(const GL::VarVal& x) noexcept {
       assert(is_sorted());
       const auto it = std::ranges::lower_bound(C, x);
       if (it == C.end() or *it != x) return C.end();
       else return it;
    }
    const_iterator search(const GL::VarVal& x) const noexcept {
       assert(is_sorted());
       const auto it = std::ranges::lower_bound(C, x);
       if (it == C.end() or *it != x) return C.end();
       else return it;
    }
    GL::var_t isearch(const GL::VarVal& x) const noexcept {
      assert(is_sorted());
      const GL::var_t dif = search(x) - C.begin();
      if (dif == C.size()) return GL::singvar;
      else return dif;
    }

    // The following three find-variations don't asume C being sorted, but
    // use linear search (finding the first occurrence of x if present):
    const_iterator find(const GL::VarVal& x) const noexcept {
      return std::ranges::find(C, x);
    }
    iterator find(const GL::VarVal& x) noexcept {
      return std::ranges::find(C, x);
    }
    GL::var_t ifind(const GL::VarVal& x) const noexcept {
      const GL::var_t dif = find(x) - C.begin();
      if (dif == C.size()) return GL::singvar;
      else return dif;
    }

    constexpr auto operator <=>(const GClause& D) const noexcept {
      return std::lexicographical_compare_three_way(
        C.rbegin(), C.rend(), D.C.rbegin(), D.C.rend());
    }
    constexpr bool operator ==(const GClause& D) const noexcept = default;

    friend std::ostream& operator <<(std::ostream& out, const GClause& C) {
      for (const auto& x : C) out << x << " ";
      return out << "0\n";
    }
    friend std::istream& operator >>(std::istream& in, GClause& C) {
      for (GL::optVarVal ov; in >> ov, ov; C.push_back(ov.value()));
      return in;
    }

  };

  static_assert(std::ranges::random_access_range<GClause>);
  static_assert(std::ranges::sized_range<GClause>);
  static_assert(std::ranges::contiguous_range<GClause>);


  bool totsing(const GClause& C) noexcept {
    return C == GClause{{GL::totsingvv}};
  }
  GClause totsingcl() noexcept { return GClause{{GL::totsingvv}}; }


  bool nonsingular(const GClause& C) noexcept {
    return not std::ranges::any_of(C, GL::sing);
  }
  bool valid(const GClause& C, const GL::var_pars& P) noexcept {
    return std::ranges::all_of(C, [&P](const auto& x) noexcept {
                                   return valid(x, P);});
  }
  // Remark: If valid(C, P) for any P, then nonsingular(C).

  // Checks if adjacent literals are clashing:
  bool has_consecutive_clashes(const GClause& C) noexcept {
    return std::ranges::adjacent_find(C, GL::clash) != C.end();
  }
  bool tautological(const GClause& C) {
    auto D(C); D.sort();
    return has_consecutive_clashes(D);
  }

  bool is_standardised(const GClause& C) noexcept {
    return C.is_sorted() and not has_consecutive_clashes(C) and not tautological(C);
  }


  bool varequal(const GClause& C, const GClause& D) noexcept {
    return std::ranges::equal(C, D, {}, GL::var, GL::var);
  }
  bool varequal_valleq(const GClause& C, const GClause& D) noexcept {
    return std::ranges::equal(C, D, [](const GL::VarVal& x, const GL::VarVal& y)noexcept{
                                return x.v==y.v and x.e <= y.e;});
  }

}

#endif
