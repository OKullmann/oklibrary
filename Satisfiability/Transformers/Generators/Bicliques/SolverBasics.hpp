// Oliver Kullmann, 8.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  General components for handling external solvers

*/

#ifndef SOLVERBASICS_RcDCyjCz1d
#define SOLVERBASICS_RcDCyjCz1d

#include <ostream>
#include <istream>

#include <cassert>

#include <Numerics/NumTypes.hpp>
#include <Numerics/NumInOut.hpp>

namespace SolverBasics {

  typedef FloatingPoint::float80 numeric_t;
  struct Value {
    numeric_t x;

    constexpr Value() noexcept : x(na) {}
    explicit constexpr Value(const numeric_t x) noexcept : x(x) {}
    Value(const std::string& s) : x(s == Value::err_str ? err :
                                    s == Value::na_str ? na :
                                    FloatingPoint::to_float80(s)) {}

    static constexpr numeric_t err = FloatingPoint::NaN;
    static constexpr std::string err_str = "ERR";
    static constexpr numeric_t na = FloatingPoint::minfinity;
    static constexpr std::string na_str = "NA";

    constexpr bool error() const noexcept { return FloatingPoint::isnan(x); }
    constexpr bool NA() const noexcept { return x == na; }
    constexpr bool avail() const noexcept { return not error() and not NA(); }

    explicit constexpr operator numeric_t() const noexcept { return x; }
    // Based on FloatingPoint::to_string (with maximal precision):
    operator std::string() const noexcept {
      if (error()) return err_str;
      else if (NA()) return na_str;
      else return FloatingPoint::to_string(x);
    }

    constexpr bool operator ==(const Value& rhs) const noexcept {
      if (error() or rhs.error()) return error() and rhs.error();
      else return x == rhs.x;
    }
    constexpr std::partial_ordering operator <=>(const Value& rhs) const noexcept {
      if (error() or rhs.error())
        if (error() and rhs.error()) return std::partial_ordering::equivalent;
        else return std::partial_ordering::unordered;
      else return x <=> rhs.x;
    }
  };
  static_assert(Value{}.NA()); static_assert(not Value{}.error());
  constexpr Value Verr = Value{Value::err}; static_assert(Verr == Verr);
  static_assert(Verr.error()); static_assert(not Verr.NA());
  static_assert(Value{0}.x == 0); static_assert(Value{0}.avail());
  static_assert(Value{} < Value{0});

  // Using floating-point output-streaming (with the given precision of the
  // streaml note that this is different from the above string-conversion):
  std::ostream& operator <<(std::ostream& out, const Value& v) noexcept {
    if (v.error()) return out << Value::err_str;
    else if (v.NA()) return out << Value::na_str;
    else return out << v.x;
  }
  // Using standard string input:
  std::istream& operator >>(std::istream& in, Value& v) {
    std::string s; in >> s; Value v0(s); v = v0;
    return in;
  }

}

#endif
