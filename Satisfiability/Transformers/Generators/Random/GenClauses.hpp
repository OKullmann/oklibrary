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
    void remove_consecutive_duplicates() noexcept {
      const auto left_over = std::ranges::unique(C);
      C.erase(left_over.begin(), left_over.end());
    }

    auto operator <=>(const GClause&) const noexcept = default;
    friend std::ostream& operator <<(std::ostream& out, const GClause& C) {
      for (const auto& x : C) out << x << " ";
      return out << "0\n";
    }
    friend std::istream& operator >>(std::istream& in, GClause& C) {
      for (GL::optVarVal ov; in >> ov, ov; C.push_back(ov.value()));
      return in;
    }

  };

  static_assert(std::ranges::sized_range<GClause>);
  static_assert(std::ranges::contiguous_range<GClause>);

  bool valid(const GClause& C, const GL::var_pars& P) noexcept {
    return std::ranges::all_of(C, [&P](const auto& x) noexcept {
                                   return valid(x, P);});
  }

  // Checks if adjacent literals are clashing:
  bool tautological_sorted(const GClause& C) noexcept {
    return std::ranges::adjacent_find(C, GL::clash) != C.end();
  }
  bool tautological(const GClause& C) {
    auto D(C); D.sort();
    return tautological_sorted(D);
  }


}

#endif
