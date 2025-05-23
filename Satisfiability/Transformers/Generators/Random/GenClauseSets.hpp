// Oliver Kullmann, 23.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Components for generalised clause-sets

*/

#ifndef GENCLAUSESETS_w1SoFOhS1G
#define GENCLAUSESETS_w1SoFOhS1G

#include <vector>
#include <ostream>
#include <utility>

#include "GenLit.hpp"
#include "GenClauses.hpp"

namespace GenClauseSets {

  namespace GL = GenLit;
  namespace GC = GenClauses;

  using var_t = GL::var_t;
  using val_t = GL::val_t;
  using VarVal = GL::VarVal;


  constexpr char comchar = 'C';
  constexpr char numvarchar = 'n';
  constexpr char numclchar = 'c';


  typedef std::vector<GC::GClause> gclauseset_t;
  typedef std::vector<GL::val_t> domsizes_t;

  // The maximal variable-index + 1 (and checking that variables and values
  // are not singular):
  var_t num_var(const gclauseset_t& F) {
    var_t res = 0;
    for (const auto& C : F)
      for (const auto& x : C)
        if (x.v == GL::singvar)
          throw GL::LiteralReadError("singular variable");
        else if (x.e == GL::singval)
          throw GL::LiteralReadError("singular value");
        else if (x.v >= res) res = x.v+1;
    return res;
  }
  // Sets the domain-sizes as found in F:
  domsizes_t dom_sizes(const gclauseset_t& F) {
    domsizes_t res(num_var(F));
    for (const auto& C : F)
      for (const auto& x : C) {
        val_t& dom = res[x.v];
        if (x.e >= dom) dom = x.e+1;
      }
    return res;
  }


  struct GClauseList {
    gclauseset_t F;
    domsizes_t dom;

    GClauseList() noexcept = default;
    GClauseList(gclauseset_t F0) : F(std::move(F0)), dom(dom_sizes(F)) {}
    GClauseList(gclauseset_t F0, domsizes_t dom0) noexcept :
    F(std::move(F0)), dom(std::move(dom0)) {}

    var_t n() const noexcept { return dom.size(); }
    var_t c() const noexcept { return F.size(); }

    bool valid() const noexcept {
      for (const auto& C : F)
        for (const auto& x : C)
          if (x.v >= dom.size() or x.e >= dom[x.v]) return false;
      return true;
    }

    auto operator <=>(const GClauseList&) const noexcept = default;

    friend std::ostream& operator <<(std::ostream& out, const GClauseList& F) {
      out << numvarchar << " " << F.n() << "\n"
          << numclchar << " " << F.c() << "\n";
      for (const auto& C : F.F) out << C;
      return out;
    }

  };

}

#endif
