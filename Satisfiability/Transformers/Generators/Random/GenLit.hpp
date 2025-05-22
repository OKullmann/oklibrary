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
#include <istream>
#include <string>
#include <optional>
#include <exception>

#include <Numerics/NumInOut.hpp>

#include "VarLit.hpp"

namespace GenLit {

  /*
    VarVal and singularity, validity
  */

  using var_t = RandGen::var_t;
  using val_t = var_t;

  constexpr var_t singvar = -1;
  constexpr val_t singval = -1;

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

  constexpr VarVal totsingvv{singvar, singval};

  constexpr bool varsing(const VarVal& v) noexcept { return v.v == singvar; }
  static_assert(varsing(VarVal{singvar}));
  constexpr bool valsing(const VarVal& v) noexcept { return v.e == singval; }
  static_assert(valsing(VarVal{0,singval}));
  constexpr bool totsing(const VarVal& v) noexcept {
    return varsing(v) and valsing(v);
  }
  static_assert(totsing(totsingvv));
  constexpr VarVal var2sing(const var_t v) noexcept {
    return {v,singval};
  }
  static_assert(valsing(var2sing(0)));
  constexpr VarVal val2sing(const val_t e) noexcept {
    return {singvar,e};
  }
  static_assert(varsing(val2sing(0)));

  constexpr bool valid_for_n(const VarVal& v, const var_t n) noexcept {
    return v.v < n;
  }
  static_assert(valid_for_n(var2sing(0), 1));
  static_assert(not valid_for_n(VarVal{}, 0));
  constexpr bool valid_for_D(const VarVal& v, const val_t D) noexcept {
    return v.e < D;
  }
  static_assert(valid_for_D(val2sing(0), 1));
  static_assert(not valid_for_D(VarVal{}, 0));

  struct var_pars {
    val_t D; var_t n;
  };
  constexpr bool valid(const VarVal& v, const var_pars& P) noexcept {
    return valid_for_D(v, P.D) and valid_for_n(v, P.n);
  }
  static_assert(valid({}, {1,1}));


  /*
    Extractions
  */

  constexpr var_t var(const VarVal v) noexcept { return v.v; }
  constexpr val_t val(const VarVal v) noexcept { return v.e; }
  static_assert(var(VarVal{5,7}) == 5);
  static_assert(val(VarVal{5,7}) == 7);


  /*
    Conversions
  */

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


  /*
    Properties
  */

  constexpr bool clash(const VarVal& x, const VarVal& y) noexcept {
    return x.v == y.v and x.e != y.e;
  }
  static_assert(clash(VarVal{0,0}, VarVal{0,1}));
  static_assert(not clash(VarVal{}, VarVal{}));
  static_assert(not clash(VarVal{2,0}, VarVal{3,1}));


  /*
    Reading
  */

  struct LiteralReadError : std::runtime_error {
    static std::string add(const std::string& m) noexcept {
      return std::string("GenLit::LiteralReadError: ") + m;
    }
    LiteralReadError(const std::string& m) noexcept :
    std::runtime_error(add(m)) {}
  };

  VarVal to_varval(const std::string& L) {
    const auto seppos = L.find_first_of(valsep);
    if (seppos ==  std::string::npos)
      throw LiteralReadError(std::string(std::string("Missing separator \"")
                                         + valsep + "\""));
    using FloatingPoint::to_UInt;
    VarVal res;
    try {
      res = {to_UInt(L.substr(0,seppos)), to_UInt(L.substr(seppos+1))};
    }
    catch(const std::exception& e) {
      throw LiteralReadError(e.what());
    }
    return res;
  }

  std::istream& operator >>(std::istream& in, VarVal& v) {
    std::string s; in >> s; v = to_varval(s);
    return in;
  }
  typedef std::optional<VarVal> optVarVal;
  // Returns empty optional-object iff reading "0":
  std::istream& operator >>(std::istream& in, optVarVal& ov) {
    std::string s; in >> s;
    if (s == "0") ov = {};
    else ov = to_varval(s);
    return in;
  }

}

#endif
