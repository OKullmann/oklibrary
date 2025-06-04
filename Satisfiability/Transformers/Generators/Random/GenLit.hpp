// Oliver Kullmann, 21.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Components for generalised literals

  Typedefs for std::uint64_t :
    - var_t, val_t

  Constants:
    - singvar : var_t
    - singval : val_t
    (in both cases the maximum values)
    - valsep : char (separating the value from the variable)

  Main structure:
    - VarVal (concrete data type, aggregate of v : var_t and e : val_t
    - operators <=> and <<
    - to_string(VarVal) -> string (see below for reverse)
    - var(VarVal), val(VarVal) extract v resp. e

  Handling of singularity:
    - varsing(VarVal) -> bool
    - valsing(VarVal) -> bool
    - totsing(VarVal) -> bool (both variable- and value-singular)
    - constant totsingvv
    - sing(VarVar) -> bool (needs only one of them)

    - var2sing(var_t) -> VarVal
    - val2sing(val_t) -> VarVal

  Validity (that is, "normality"):
    - valid_for_n(VarVal, var_t) -> bool
    - valid_for_D(VarVal, val_t) -> bool
    - struct var_pars, wrapper for D : val_t and n : var_t
    - valid(VarVal, var_pars)

  Conversions from boolean variables and literals:
    - to_varval(RandGen::Var) -> VarVal (setting value to 1)
    - to_varval(RandGen::Lit) -> VarVal (taking the boolean as value).

  Properties:
    - clash(VarVal, VarVal) -> bool

  Reading:
    - LiteralReadError derived from std::runtime_error
    - to_varval(string) -> VarVal
    - in-streaming for VarVal and std::optional<VarVal>
      (the latter returns the empty object in case "0" was found, the
      marker for the end-of-clause).

*/

#ifndef GENLIT_Um2YuEsJQA
#define GENLIT_Um2YuEsJQA

#include <ostream>
#include <istream>
#include <string>
#include <optional>
#include <exception>

#include <Numerics/NumBasicFunctions.hpp>
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
  static_assert(singvar == singval);
  static_assert(singvar == FloatingPoint::P264m1);

  constexpr char valsep = ':';


  // Concrete datatype (all values are valid):
  struct VarVal {
    var_t v;
    val_t e;

    VarVal& operator ++() noexcept { ++e; return *this; }

    constexpr auto operator <=>(const VarVal&) const noexcept = default;
  };
  static_assert(VarVal{} == VarVal{0,0});
  static_assert(VarVal{} != VarVal{0,1});
  static_assert(VarVal{0,1} < VarVal{0,2});
  static_assert(VarVal{0,2} < VarVal{1,0});

  std::string to_string(const VarVal& x) noexcept {
    return std::to_string(x.v) + std::string(1,valsep) +
      std::to_string(x.e);
  }

  constexpr VarVal totsingvv{singvar, singval};
  const std::string totsingstr = "SINGULAR";

  std::ostream& operator <<(std::ostream& out, const VarVal& v) {
    if (v == totsingvv) return out << totsingstr;
    else return out << v.v << valsep << v.e;
  }

  constexpr bool varsing(const VarVal& v) noexcept { return v.v == singvar; }
  static_assert(varsing(VarVal{singvar}));
  constexpr bool valsing(const VarVal& v) noexcept { return v.e == singval; }
  static_assert(valsing(VarVal{0,singval}));
  constexpr bool totsing(const VarVal& v) noexcept {
    return varsing(v) and valsing(v);
  }
  static_assert(totsing(totsingvv));
  constexpr bool sing(const VarVal& v) noexcept {
    return varsing(v) or valsing(v);
  }
  static_assert(sing({0,singval}) and sing({singvar,0}));

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
    VarVal is iterator
  */

  struct VarVal_it {
    VarVal current;

    using iterator_category = std::random_access_iterator_tag;
    using value_type = VarVal;
    using difference_type = std::ptrdiff_t;
    using pointer = const VarVal*;
    using reference = VarVal;

    constexpr VarVal_it() noexcept : current(val2sing(0)) {}
    explicit constexpr VarVal_it(const VarVal& x) noexcept : current(x) {}
    constexpr VarVal_it(const VarVal_it& other) noexcept = default;

    VarVal_it& operator ++() noexcept { ++current.e; return *this; }
    VarVal_it& operator --() noexcept { --current.e; return *this; }
    VarVal_it operator ++(int) noexcept {
      const auto t = *this; ++current.e; return t;
    }
    VarVal_it operator --(int) noexcept {
      const auto t = *this; --current.e; return t;
    }
    VarVal_it& operator +=(const difference_type d) noexcept {
      current.e += d; return *this;
    }
    VarVal_it& operator -=(const difference_type d) noexcept {
      current.e -= d; return *this;
    }
    reference operator [](const difference_type d) const noexcept {
      return {current.v, current.e + d};
    }
    reference operator *() const noexcept { return current; }
    bool operator ==(const VarVal_it&) const noexcept = default;
    auto operator <=>(const VarVal_it&) const noexcept = default;

    friend VarVal_it operator +(VarVal_it it,
                                const difference_type d) noexcept {
      return it += d;
    }
    friend VarVal_it operator +(const difference_type d,
                                VarVal_it it) noexcept {
      return it += d;
    }
    friend VarVal_it operator -(VarVal_it it,
                                const difference_type d) noexcept {
      return it -= d;
    }
    friend difference_type operator -(const VarVal_it& lhs,
                                      const VarVal_it& rhs) noexcept {
      return lhs.current.e - rhs.current.e;
    }
  };
  using VarVal_range = std::ranges::subrange<VarVal_it>;
  VarVal_range make_VarVal_range(const var_t v, const val_t a, const val_t b) noexcept {
    // a <= b is not necessary -- unsigned types can wrap around
    return std::ranges::subrange(VarVal_it({v,a}), VarVal_it({v,b}));
  }


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
      throw LiteralReadError(std::string("Missing separator \"")
                             + valsep + "\" in literal \"" + L + "\"");
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


  /*
    Spiking
  */

  constexpr var_t first_spiking = FloatingPoint::exp2(48);
  constexpr var_t max_spiking_index = var_t(-1) - first_spiking;
  constexpr var_t spiking_var_index(const var_t v) noexcept {
    assert(v <= max_spiking_index);
    return first_spiking + v;
  }
  static_assert(spiking_var_index(0) == first_spiking);
  static_assert(spiking_var_index(max_spiking_index) == var_t(-1));
  constexpr VarVal spiking_lit(const var_t v) noexcept {
    return var2sing(spiking_var_index(v));
  }
  static_assert(spiking_lit(max_spiking_index) == totsingvv);
  constexpr bool is_spiking_lit(const VarVal& x) noexcept {
    return var(x) >= first_spiking and valsing(x);
  }
  static_assert(is_spiking_lit(spiking_lit(0)));
  static_assert(not is_spiking_lit(var2sing(first_spiking-1)));

}

#endif
