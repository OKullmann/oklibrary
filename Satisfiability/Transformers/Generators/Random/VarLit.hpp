// Oliver Kullmann, 13.3.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Components for variables and literals

   - typedefs
    - var_t (unsigned 64-bit)
    - lit_t (signed 64-bit)
   - constants
    - max_var = 2^64-1
    - max_lit = 2^63-1
    - min_lit = -2^63

   - classe Var: concrete data type, wrapping v : var_t
    - constructors Var(), Var(var_t)
    - operator <=>
   - singular(Var) (v == 0)
   - valid(Var) (v != 0)
   - operator <<

   - class Lit : concrete data type, wrapping v : Var, s : bool
    - s==true means positive
    - constructors Lit(), Lit(Var), Lit(bool, Var),
        Lit(var_t, lit_t), Lit(lit_t)
    - sign() -> lit_t
    - neg()
    - convertible() -> bool (whether a safe lit_t can be extracted)
    - convert() -> lit_t
    - operator <=>
   - Lit::var(lit_t) -> var_t
   - Lit::sign(lit_t) -> bool (x == 0 yields true)
   - singular(Lit) (true if v == 0)
   - valid(Lit) (true if v >= 1)
   - var(Lit) -> Var
   - operator <<
   - operator -(Lit)


  See also Bicliques/DimacsTools.hpp.

*/

#ifndef VARLIT_PXJ0EaHqJd
#define VARLIT_PXJ0EaHqJd

#include <ostream>

#include <cassert>
#include <cstdint>

namespace RandGen {

  typedef std::uint64_t var_t;
  constexpr var_t max_var = 18446744073709551615ULL; // 2^64 - 1

  /*
    Var is a concrete data type, holding member v of type var_t:

     - The value v = 0 is the "singular" value.
     - Default construction yields the singular value.
     - Construction from var_t is explicit (implicit conversions from
       an integral type would cause too many errors).
     - The comparison operators are the natural ones.

  */
  struct Var {
    var_t v;

    constexpr Var() noexcept : v(0) {}
    explicit constexpr Var(const var_t v) noexcept : v(v) {}

    friend constexpr auto operator <=>(Var, Var) noexcept = default;
  };
  static_assert(Var() == Var(0));
  static_assert(Var{0} < Var{1});
  static_assert(Var(max_var-1) < Var(max_var));

  inline constexpr bool singular(const Var v) noexcept { return v.v == 0; }
  inline constexpr bool valid(const Var v) noexcept { return v.v >= 1; }
  static_assert(singular(Var(0)));
  static_assert(not singular(Var(1)));
  static_assert(not valid(Var{0}));
  static_assert(valid(Var{1}));

  inline std::ostream& operator <<(std::ostream& out, const Var v) {
    return out << v.v;
  }


  typedef std::int64_t lit_t;
  constexpr lit_t max_lit = 9223372036854775807LL; // 2^63 - 1
  constexpr lit_t min_lit = -max_lit - 1;
  static_assert(var_t(max_lit) + 1 == max_var/2 + 1);

  /*
    Lit is a concrete data type, holding members v of type Var, and
    the bool s (for "sign" -- "true" means positive, "false" means negative):

     - The literal is singular iff v is.
     - Default construction yields the positive singular literal.
     - Construction from a variable yields the positive literal.
     - The construction from a boolean and a variable has the boolean first,
       in order to minimise the danger coming from the implicit conversions
       of integral types to bool.
     - The other constructor which allow to specify the sign (explicitly)
       has the sign in the second argument, as an integer s, with "positive"
       meaning "s > 0". So s = 0 means "negative".
     - Construction from a value x of type lit_t produces the positive
       singularity for x = 0.

     Additional access to the sign of a literal is provided by sign(),
     which return -1 or +1 (mainly for legacy reason).
     Negation in place happens via member neg().

     Convertibility to lit_t is checked by convertible(), and performed
     by convert() (a conversion-operator is too error-prone here).

     The order of literals is lexicographical, that is:
       {0,-1} < {0,1} < {1,-1} < {1,1} ...

  */
  struct Lit {
    Var v;
    bool s;

    constexpr Lit() noexcept : v(0), s(true) {};
    constexpr Lit(const Var v) noexcept : v(v), s(true) {}
    constexpr Lit(const bool s, const Var(v)) noexcept : v(v), s(s) {}

    constexpr Lit(const var_t v, lit_t s) noexcept : v(v), s(s>0) {}
    constexpr Lit(const lit_t x) noexcept : v(var(x)), s(sign(x)) {}

    static constexpr var_t var(const lit_t x) noexcept {
      if (x >= 0) return x;
      else if (x == min_lit) return var_t(max_lit) + 1;
      else return -x;
    }
    static constexpr bool sign(const lit_t x) noexcept {
      return x >= 0;
    }

    constexpr lit_t sign() const noexcept { return s ? 1 : -1; }
    constexpr void neg() noexcept { s = not s; }

    constexpr bool convertible() const noexcept {
      return v.v <= max_lit;
    }
    constexpr lit_t convert() const noexcept {
      assert(convertible());
      return s ? v.v : -lit_t(v.v);
    }

    friend constexpr auto operator <=>(Lit, Lit) noexcept = default;
  };
  static_assert(Lit::var(max_lit) == max_lit);
  static_assert(Lit::var(min_lit) == max_var / 2 + 1);
  static_assert(Lit::sign(max_lit));
  static_assert(Lit::sign(0));
  static_assert(not Lit::sign(-1));
  static_assert(not Lit::sign(min_lit));

  static_assert(Lit() == Lit(Var(0)));
  static_assert(Lit() == Lit(true, Var(0)));
  static_assert(Lit() == Lit(0,1));
  static_assert(Lit() == Lit(0));
  static_assert(Lit(0, 1) == Lit(true, Var(0)));
  static_assert(Lit(0, 0) == Lit(false, Var(0)));
  static_assert(Lit(0, -1) == Lit(false, Var(0)));
  static_assert(Lit(Var(0)) == Lit(0,1));
  static_assert(Lit(Var(0)) != Lit(0,-1));
  static_assert(Lit(Var(0)) != Lit(0,-1));
  static_assert(Lit(1) == Lit(true, Var(1)));
  static_assert(Lit(-1) == Lit(false, Var(1)));
  static_assert(Lit{0,-2} == Lit(0,-2));
  static_assert(Lit(0,-2) == Lit(0,0));
  static_assert(Lit(0,-2) == Lit(false, Var(0)));
  static_assert(Lit{1,1}.v == Var(1));
  static_assert(Lit{1,1}.sign() == 1);
  static_assert(Lit{1,-1}.v == Var(1));
  static_assert(Lit{1,-1}.sign() == -1);
  static_assert(Lit{0,-2} == Lit{0,-1});
  static_assert(Lit(0) == Lit(0,1));
  static_assert(Lit(1,Var(1)) == Lit(true, Var(1)));
  static_assert(Lit(-1,Var(1)) == Lit(true, Var(1)));
  static_assert(Lit(0,Var(1)) == Lit(false, Var(1)));
  static_assert(Lit(max_lit).convertible());
  static_assert(not Lit(var_t(max_lit)+1,1).convertible());
  static_assert(not Lit(min_lit).convertible());
  static_assert(Lit(min_lit+1).convertible());

  static_assert(Lit(0,-1) < Lit());
  static_assert(Lit() < Lit(1,-1));
  static_assert(Lit{1,1} < Lit{2,-1});

  inline constexpr bool singular(const Lit x) noexcept {
    return singular(x.v);
  }
  inline constexpr bool valid(const Lit x) noexcept { return valid(x.v); }
  static_assert(singular({0,1}));
  static_assert(singular({0,-1}));
  static_assert(valid(Lit{1,0}));
  static_assert(valid(Lit{1,-1}));
  static_assert(valid(Lit{1,1}));

  inline std::ostream& operator <<(std::ostream& out, const Lit x) {
    if (not x.s) out << "-";
    return out << x.v.v;
  }

  inline constexpr Var var(const Lit x) noexcept {
    return x.v;
  }
  static_assert(var(Lit()) == Var(0));
  static_assert(var(Lit(0, -1)) == Var(0));
  static_assert(var(Lit(1, -1)) == Var(1));
  static_assert(var(Lit(1, 1)) == Var(1));

  inline constexpr Lit operator -(const Lit x) noexcept {
    Lit res(x); res.neg(); return res;
  }
  static_assert(-Lit{0,-1} == Lit{0,1});
  static_assert(-Lit{0,1} == Lit{0,-1});
  static_assert(-Lit{0,2} == Lit{0,-2});
  static_assert(-Lit{1,1} == Lit{1,-1});
  static_assert(-Lit{1,-1} == Lit{1,1});

}

#endif
