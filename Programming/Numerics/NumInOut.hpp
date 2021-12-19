// Oliver Kullmann, 19.12.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for numerical input and output

  Wrap(x) for float80 x just wraps it; output-streaming of such an object
  happens with maximal precision; similar with Wrap64(x).

    - stold (returns float80; wrapper for standard-library function)

    - to_float80(string s) converts s to float80 (improved stold regarding
      error-checking and -messages)
    - toUInt(string s) converts every string, which is convertible
      to float80, to UInt_t
    - touint(std::string s) converts every string convertible to float80
      to uint_t.

  Output-helper-functions:

    - fullprec_float80(ostream&) sets the maximum precision
    - similarly fullprec_float64, fullprec_float32, fullprec_floatg<FLOAT>
    - Wrap, Wrap64, Wrap32 are output-streamed with full precision
    - WrapE<Float>(x) is output-streamed with current precision in scientific
      notation without trailing zeros.

*/

#ifndef NUMINOUT_HTjNkdTM7M
#define NUMINOUT_HTjNkdTM7M

#include <string>
#include <ostream>
#include <sstream>

#include "NumTypes.hpp"
#include "Conversions.hpp"

namespace FloatingPoint {

  inline float80 stold(const std::string& s, std::size_t* const pos = 0) {
    return std::stold(s, pos);
  }

  // Improving stold, by improving error-messages in exceptions, and
  // furthermore throwing std:domain_error in case of trailing characters:
  inline float80 to_float80(const std::string& s) {
    std::size_t converted;
    long double x;
    try { x = FloatingPoint::stold(s,&converted); }
    catch(const std::invalid_argument& e) {
      throw std::invalid_argument("FloatingPoint::to_float80(string), failed"
        " for \"" + s + "\"");
    }
    catch(const std::out_of_range& e) {
      throw std::out_of_range("FloatingPoint::to_float80(string), \""
        + s + "\"");
    }
    if (converted != s.size())
      throw std::domain_error("FloatingPoint::to_float80(string), trailing: \""
        + s.substr(converted) + "\" in \"" + s + "\"");
    return x;
  }

  // Succeeds for every s convertible to float80, interpreting negative x
  // as zero, too big x as the maximal value, and applying rounding otherwise:
  inline UInt_t toUInt(const std::string& s) {
    const float80 x = to_float80(s);
    if (not (x >= 0)) return 0;
    else return toUInt(x);
  }
  inline uint_t touint(const std::string& s) {
    const float80 x = to_float80(s);
    if (not (x >= 0)) return 0;
    else return touint(x);
  }


  auto fullprec_float80(std::ostream& out) noexcept {
    return out.precision(limitfloat::digits10 + 2);
  }
  auto fullprec_float64(std::ostream& out) noexcept {
    return out.precision(limitfloat64::digits10 + 2);
  }
  auto fullprec_float32(std::ostream& out) noexcept {
    return out.precision(limitfloat32::digits10 + 2);
  }
  template <typename FLOAT>
  std::streamsize fullprec_floatg(std::ostream& out) noexcept {
    return out.precision(std::numeric_limits<FLOAT>::digits10 + 2);
  }


  // Slow output:
  std::ostream& operator <<(std::ostream& out, const Wrap x) {
    const auto prec = fullprec_float80(out);
    out << x.x;
    out.precision(prec);
    return out;
  }

  // Slow output:
  std::ostream& operator <<(std::ostream& out, const Wrap64 x) {
    const auto prec = fullprec_float64(out);
    out << x.x;
    out.precision(prec);
    return out;
  }

  // Slow output:
  std::ostream& operator <<(std::ostream& out, const Wrap32 x) {
    const auto prec = fullprec_float32(out);
    out << x.x;
    out.precision(prec);
    return out;
  }


 template <typename FLOAT>
  std::ostream& operator <<(std::ostream& out, const WrapE<FLOAT> x) {
    if (x.deactivated) { return out << x.x; }
    if (x.x == 0) return out << "0";
    std::stringstream s;
    s.precision(out.precision());
    s.setf(std::ios_base::scientific, std::ios_base::floatfield);
    s << x.x;
    assert(s);
    if (s.str().find('.') == std::string::npos) return out << s.str();
    else {
      std::string res = s.str();
      const auto pos_e = res.find('e');
      assert(pos_e != std::string::npos);
      const std::string e = res.substr(pos_e, std::string::npos);
      assert(not e.empty());
      res = res.substr(0, pos_e);
      assert(not res.empty());
      res = res.substr(0, res.find_last_not_of('0')+1);
      assert(not res.empty());
      if (res.back() == '.') res.pop_back();
      assert(not res.empty());
      return out << res + e;
    }
 }

}

#endif
