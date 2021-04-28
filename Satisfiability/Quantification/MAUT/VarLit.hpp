// Oliver Kullmann, 28.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Variables and literals

*/


#ifndef VARLIT_JzFwAuM8fn
#define VARLIT_JzFwAuM8fn

#include <type_traits>
#include <limits>

#include <cstdint>
#include <cassert>

namespace MAUT {

  typedef std::uint64_t VAR;
  constexpr VAR max_var = std::numeric_limits<VAR>::max() / 2 - 1;

  constexpr bool valid(const VAR v) noexcept { return v <= max_var; }
  static_assert(valid(VAR(0)));
  static_assert(valid(max_var));
  static_assert(not valid(max_var+1));

  constexpr bool singular(const VAR v) noexcept { return v == 0; }
  static_assert(singular(VAR(0)));
  static_assert(not singular(VAR(1)));

  typedef std::int64_t LIT;
  static_assert(std::is_same_v<LIT, std::make_signed_t<VAR>>);

  constexpr bool valid(const LIT x) noexcept {
    if (x >= 0) return valid(VAR(x));
    else return valid(VAR(-x));
  }
  static_assert(valid(LIT(0)));
  static_assert(valid(LIT(max_var)));
  static_assert(valid(-LIT(max_var)));
  static_assert(not valid(LIT(max_var)+1));
  static_assert(not valid(-LIT(max_var) -1));

  constexpr bool singular(const LIT x) noexcept { return x == 0; }
  static_assert(singular(LIT(0)));
  static_assert(not singular(LIT(1)));
  static_assert(not singular(LIT(-1)));

  constexpr VAR var(const LIT x) noexcept { return x >= 0 ? x : -x; }
  static_assert(var(0) == 0);
  static_assert(var(-1) == VAR(1));
  static_assert(var(1) == VAR(1));

  constexpr LIT lit(const VAR v, const LIT sign) noexcept {
    assert(valid(v));
    if (sign > 0) return v;
    else if (sign == 0) return 0;
    else return -LIT(v);
  }
  static_assert(lit(1,0) == 0);
  static_assert(lit(1,1) == 1);
  static_assert(lit(1,-1) == -1);

  inline constexpr bool comp(const LIT& x, const LIT& y) noexcept {
    assert(valid(x) and valid(y));
    const VAR v = var(x), w = var(y);
    return v < w or (v == w and x < y);
  }
  static_assert(comp(0, -1));
  static_assert(not comp(-1, 0));
  static_assert(comp(-1, 1));
  static_assert(not comp(1, -1));

}

#endif
