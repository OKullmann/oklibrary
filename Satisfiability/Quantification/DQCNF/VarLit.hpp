// Oliver Kullmann, 12.2.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Components related to variables and literals */

#ifndef VARLIT_0QMaNdUlYk
#define VARLIT_0QMaNdUlYk

#include <limits>
#include <type_traits>
#include <ostream>

#include <cstdint>

#include "Generics.hpp"

namespace VarLit {

  /*
    The basic classes are Var (variables) and Lit (literals), where
    Lit contains Lit_int, "Literals as integers", which are signed integers,
    while variables are unsigned.

    Polarities pos, neg are expressed via the enumeration-type Pol.

    Var is just a typedef of an unsigned integral type.

    Operations for Lit_int x, Lit y,y', Var v, Pol p:

     - Lit() (the singular literal)
     - copy-construction, assignment for Lit
     - Lit(x) (non-converting)
     - Lit(v, p)
     - bool(y) (explicit; true iff x is not singular)
     - -y, -p, and y.neg() (negation in-place)
     - y == y', y != y'
     - y < y', p < p'
     - var(y) (yields Var)
     - sign(y) (yields Polarity)
     - y.posi(), y.negi() (whether y is positive resp. negative)
     - y.index() (yields Lit_int)
     - for-each loop: for (Pol p : Polarities)
     - ostream << y, istream >> y

     Lit-literals are constructed by n_l for unsigned long-long n.
  */

#ifndef LIT_TYPE
# define LIT_TYPE std::int32_t
#endif

  typedef LIT_TYPE Lit_int;
  static_assert(std::is_signed_v<Lit_int>);
  static_assert(sizeof(Lit_int) != 1, "Lit_int = char (or int8_t) doesn't work with reading (since not numbers are read, but characters).");
  constexpr Lit_int max_lit = std::numeric_limits<Lit_int>::max();
  static_assert(- -max_lit == max_lit);

  typedef std::make_unsigned<Lit_int>::type Var;
  static_assert(Lit_int(Var(max_lit)) == max_lit);
  inline constexpr bool valid(const Var v) noexcept { return v <= Var(max_lit); }

  typedef Var AVar;
  typedef Var EVar;

  enum class Pol { n, p };
  inline constexpr Pol operator -(const Pol p) noexcept {
    return (p==Pol::p) ? Pol::n : Pol::p;
  }
  constexpr std::array<Pol,2> Polarities {{Pol::n, Pol::p}};

  static_assert(std::is_pod_v<Pol>);
  static_assert(static_cast<int>(Pol::n) == 0);
  static_assert(static_cast<int>(Pol::p) == 1);
  static_assert(Pol::n < Pol::p);
  static_assert(-Pol::n == Pol::p and -Pol::p == Pol::n);
  static_assert(Pol(false) == Pol::n and Pol(true) == Pol::p);
  static_assert(Pol(0) == Pol::n and Pol(1) == Pol::p);
  static_assert(Polarities.size() == 2 and Polarities[0] == Pol::n and Polarities[1] == Pol::p);

  class Lit {
    Lit_int x;
  public :
    Lit() = default;
    constexpr explicit Lit(const Lit_int x) noexcept : x(x) {}
    constexpr Lit(const Var v, const Pol p) noexcept : x(p==Pol::p?v:-Lit_int(v)) {}

    constexpr explicit operator bool() const noexcept { return x; }
    constexpr Lit_int index() const noexcept {return x;}
    constexpr bool posi() const noexcept { return x > 0; }
    constexpr bool negi() const noexcept { return x < 0; }

    constexpr Lit operator -() const noexcept { return Lit(-x); }
    void neg() noexcept { x=-x; }

    constexpr bool operator ==(const Lit y) const noexcept { return x == y.x; }
    constexpr bool operator !=(const Lit y) const noexcept { return x != y.x; }

    friend constexpr Var var(const Lit x) noexcept { return std::abs(x.x); }
    friend constexpr Pol sign(const Lit x) noexcept {return Pol(x.x >= 0);}

    friend constexpr bool operator <(const Lit a, const Lit b) noexcept {
      return var(a)<var(b) or (var(a)==var(b) and sign(a)<sign(b));
    }
    friend constexpr bool operator <=(const Lit a, const Lit b) noexcept {
      return a<b or a==b;
    }

    friend std::ostream& operator <<(std::ostream& out, const Lit x) noexcept {
      return out << x.x;
    }
    friend std::istream& operator >>(std::istream& in, Lit& x) noexcept {
      return in >> x.x;
    }
  };
  static_assert(std::is_pod_v<Lit>);

  inline constexpr Lit operator"" _l(const unsigned long long x) noexcept {return Lit(x);}
  static_assert(0_l == Lit());
  // Remark: As usual, as a local variable, the declaration "Lit x;" does not
  // initialise x.
  static_assert(1_l==Lit(1) and -1_l==Lit(-1));
  static_assert(not 0_l);
  static_assert(1_l and -1_l);
  static_assert(1_l != -1_l);
  static_assert(- -1_l == 1_l);
  static_assert(0_l == -0_l);
  static_assert(var(-1_l) == 1);
  static_assert(sign(-1_l) == Pol::n);
  static_assert(1_l == Lit(1,Pol::p) and -1_l == Lit(1,Pol::n));
  static_assert(0_l < 1_l);
  static_assert(-1_l < 1_l);
  static_assert((1_l).index() == 1);
  static_assert((-1_l).index() == -1);
  static_assert((1_l).posi());
  static_assert((-1_l).negi());
  static_assert(Lit(1_l) == 1_l);


  // Boolean function type (nonconstant, false, or true):
  enum class BFt { nc=0, f, t };
  inline constexpr BFt operator -(const BFt t) noexcept {
    return (t==BFt::nc) ? t : ((t==BFt::f) ? BFt::t : BFt::f);
  }
  static_assert(Generics::code(BFt::nc) == 0);
  static_assert(-BFt::nc == BFt::nc);
  static_assert(-BFt::t == BFt::f);
  static_assert(-BFt::f == BFt::t);

  typedef Lit ALit;
  typedef Lit ELit;


  /* Literals plus true/false (the boolean functions with at most one var);
     the linear order is 0,false,true,-1,1,-2,2, ... .
     Constructors (non-converting):
      - Litc() (singular)
      - Litc(x) for Lit x
      - Litc(t) for BFt t
     And bf(b) for bool b yields Litc.
     Exactly one of x.sing(), x.constant(), x.variable() is true for Litc x.
     Operations:
      - explicit conversions to Lit and BFt
      - negation (operator - and in-place member neg())
      - boolean member functions sing(), constant(), variable()
      - ==, !=, <
      - output-streaming.
  */
  class Litc  {
    ALit x;
    BFt t;
    /* Class invariants:
        - assert(t==BFt::nc or not x;);
        - exactly one of sing(), constant() or variable() is true.
    */
    constexpr Litc(const ALit x, const BFt t) noexcept : x(x), t(t) {}
  public :
    Litc() = default;
    constexpr explicit Litc(const ALit x) noexcept : x(x), t(BFt::nc) {}
    constexpr explicit Litc(const BFt t) noexcept : x(0), t(t) {}

    constexpr explicit operator BFt() const noexcept { return t; }
    constexpr explicit operator ALit() const noexcept { return x; }

    constexpr Litc operator -() const noexcept { return Litc(-x,-t); }
    void neg() noexcept { x = -x; t = -t; }

    constexpr bool sing() const noexcept { return not x and t==BFt::nc; }
    constexpr bool constant() const noexcept { return not x and t!=BFt::nc; }
    constexpr bool variable() const noexcept { return bool(x); }

    constexpr bool operator ==(const Litc y) const noexcept {
      return x==y.x and t==y.t;
    }
    constexpr bool operator !=(const Litc y) const noexcept {
      return not (*this == y);
    }
    friend constexpr bool operator<(const Litc x, const Litc y) noexcept {
      return x.x < y.x or (BFt(x) != BFt::nc and BFt(y) == BFt::nc) or
        (BFt(x) != BFt::nc and BFt(y) != BFt::nc and BFt(x) < BFt(y)) or
        (not bool(Lit(x)) and BFt(x) == BFt::nc and BFt(y) != BFt::nc);
    }

    friend std::ostream& operator <<(std::ostream& out, const Litc x) noexcept {
      switch (x.t) {
      case BFt::f : return out << "bf(f)";
      case BFt::t : return out << "bf(t)";
      default : return out << "bf(" << x.x << ")";
      }
    }

  };
  static_assert(std::is_pod_v<Litc>);

  inline constexpr Litc bf(const bool b) noexcept {
    return (b) ? Litc(BFt::t) : Litc(BFt::f);
  }

  static_assert(ALit(Litc()) == 0_l);
  static_assert(BFt(Litc()) == BFt::nc);
  // Remark: As usual, as a local variable, the declaration "Litc x;" does not
  // initialise x.
  static_assert(ALit(Litc(1_l)) == 1_l);
  static_assert(BFt(Litc(1_l)) == BFt::nc);
  static_assert(ALit(bf(false)) == 0_l);
  static_assert(BFt(bf(false)) == BFt::f);
  static_assert(ALit(bf(true)) == 0_l);
  static_assert(BFt(bf(true)) == BFt::t);
  static_assert(Litc() == Litc(0_l));
  static_assert(Litc() != bf(false));
  static_assert(-Litc() == Litc());
  static_assert(-bf(true) == bf(false));
  static_assert(-bf(false) == bf(true));
  static_assert(-Litc(1_l) == Litc(-1_l));
  static_assert(-Litc(-1_l) == Litc(1_l));
  static_assert(Litc(0_l) < bf(false));
  static_assert(bf(false) < bf(true));
  static_assert(bf(true) < Litc(-1_l));
  static_assert(bf(false) < Litc(-1_l));
  static_assert(Litc(-1_l) < Litc(1_l));
  static_assert(Litc().sing());
  static_assert(not Litc(1_l).sing());
  static_assert(not bf(true).sing());
  static_assert(not bf(false).sing());
  static_assert(not Litc().constant());
  static_assert(bf(true).constant());
  static_assert(bf(false).constant());
  static_assert(not Litc(1_l).constant());
  static_assert(not Litc().variable());
  static_assert(not Litc(BFt::f).variable());
  static_assert(not Litc(BFt::t).variable());
  static_assert(Litc(1_l).variable());
  static_assert(Litc(bf(true)) == bf(true));
  static_assert(not (Litc(bf(true)) < Litc(bf(true))));
  static_assert(not (Litc(bf(false)) < Litc(bf(false))));
  static_assert(not (Litc() < Litc()));
  static_assert(not (Litc(Lit(1)) < Litc(Lit(1))));

}

#endif
