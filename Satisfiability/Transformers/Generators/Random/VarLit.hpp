// Oliver Kullmann, 13.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Components for variables and literals

*/

#ifndef VARLIT_PXJ0EaHqJd
#define VARLIT_PXJ0EaHqJd

#include <cstdint>

namespace RandGen {

  typedef std::uint64_t var_t;

  struct Var {
    var_t v;
    friend constexpr bool operator ==(Var, Var) noexcept = default;
    friend constexpr auto operator <=>(Var, Var) noexcept = default;
  };
  static_assert(Var{0} < Var{1});


  struct Lit {
    Var v;
    signed char sign;
    friend constexpr bool operator ==(Lit, Lit) noexcept = default;
    friend constexpr auto operator <=>(Lit, Lit) noexcept = default;
  };
  static_assert(Lit{0,-2} < Lit{0,-1});
  static_assert(Lit{1,1} < Lit{2,-1});

  inline constexpr bool valid(const Var v) noexcept { return v.v >= 1; }
  inline constexpr bool valid(const Lit x) noexcept {
    return valid(x.v) and (x.sign == -1 or x.sign == +1);
  }
  static_assert(not valid(Var{0}));
  static_assert(valid(Var{1}));
  static_assert(not valid(Lit{1,0}));
  static_assert(valid(Lit{1,-1}));
  static_assert(valid(Lit{1,1}));

  std::ostream& operator <<(std::ostream& out, const Lit x) {
    if (x.sign == -1) out << "-";
    return out << x.v.v;
  }

  inline constexpr Lit operator -(const Lit x) noexcept {
    return {x.v, x.sign==1 ? (signed char)-1 : (signed char)1};
  }
  static_assert(-Lit{0,-1} == Lit{0,1});
  static_assert(-Lit{0,1} == Lit{0,-1});
  static_assert(-Lit{0,2} == Lit{0,1});

}

#endif
