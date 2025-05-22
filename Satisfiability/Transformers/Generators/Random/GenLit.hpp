// Oliver Kullmann, 21.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Components for generalised literals

*/

#ifndef GENLIT_Um2YuEsJQA
#define GENLIT_Um2YuEsJQA

#include <ostream>

#include "VarLit.hpp"

namespace GenLit {

  using var_t = RandGen::var_t;
  using val_t = var_t;

  constexpr char valsep = ':';

  // Concrete datatype (all values are valid):
  struct VarVal {
    var_t v;
    val_t e;

    constexpr auto operator <=>(const VarVal&) const noexcept = default;
    friend std::ostream& operator <<(std::ostream& out, const VarVal& v) {
      return out << v.v << valsep << v.e;
    }
  };
  static_assert(VarVal{} == VarVal{0,0});
  static_assert(VarVal{} != VarVal{0,1});
  static_assert(VarVal{0,1} < VarVal{0,2});
  static_assert(VarVal{0,2} < VarVal{1,0});

  constexpr var_t var(const VarVal v) noexcept { return v.v; }
  constexpr val_t val(const VarVal v) noexcept { return v.e; }
  static_assert(var(VarVal{5,7}) == 5);
  static_assert(val(VarVal{5,7}) == 7);

  constexpr VarVal to_varval(const RandGen::Var v) noexcept {
    return VarVal{v.v,1};
  }
  static_assert(to_varval(RandGen::Var()) == VarVal{0,1});
  static_assert(to_varval(RandGen::Var(7)) == VarVal{7,1});
  constexpr VarVal to_varval(const RandGen::Lit x) noexcept {
    return VarVal{x.v.v,x.s};
  }
  static_assert(to_varval(RandGen::Lit(-5)) == VarVal{5});
  static_assert(to_varval(RandGen::Lit(5)) == VarVal{5,1});
  static_assert(to_varval(RandGen::Var(7)) == to_varval(RandGen::Lit(7)));

  constexpr bool clash(const VarVal& x, const VarVal& y) noexcept {
    return x.v == y.v and x.e != y.e;
  }
  static_assert(clash(VarVal{0,0}, VarVal{0,1}));
  static_assert(not clash(VarVal{}, VarVal{}));
  static_assert(not clash(VarVal{2,0}, VarVal{3,1}));

}

#endif
