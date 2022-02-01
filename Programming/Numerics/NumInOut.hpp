// Oliver Kullmann, 19.12.2021 (Swansea)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for numerical input and output

  Input:

    - stold(string s) (returns float80; wrapper for standard-library function)

    - to_float80(string s) converts s to float80 (improved stold regarding
      error-checking and -messages)
    - to_F80ai(string s) also considers "." and "+"
    - to_vec_float80(string s, char sep) returns a vector of float80
    - to_vec_float80ai(string, char, UInt_t i) returns a pair of
      vector and F80ai (for index i)

    - read_table(filesystem::path) returns a vector of vector of float80
    - read_table_ai(filesystem::path, UIint_t i) returns a vector of pairs
      of vector and F80ai (for index i)

    - toUInt(string s) converts every string, which is convertible
      to float80, to UInt_t
    - touint(std::string s) converts every string convertible to float80
      to uint_t.

  Output:

    - fullprec_float80(ostream&) sets the maximum precision
    - similarly fullprec_float64, fullprec_float32, fullprec_floatg<FLOAT>
    - Wrap, Wrap64, Wrap32 for output-streaming with full precision:
      Wrap(x) just wraps x, and output-streaming of such an object happens
      with maximal precision according to type (float80/64/32)
    - WrapE<Float>(x) is output-streamed with current precision in scientific
      notation without trailing zeros.

*/

#ifndef NUMINOUT_HTjNkdTM7M
#define NUMINOUT_HTjNkdTM7M

#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <filesystem>
#include <utility>
#include <stdexcept>

#include <cstddef>

#include <ProgramOptions/Strings.hpp>

#include "NumTypes.hpp"
#include "Conversions.hpp"

namespace FloatingPoint {

  /* Input */


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
  inline F80ai to_F80ai(const std::string& s) {
    return {to_float80(s),
        s.find('.') == std::string::npos,
        s.find('+') != std::string::npos,
        s.find("e0") != std::string::npos};
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


  // Splitting a string into a vector, where in case sep is any space-
  // character, all such characters are just treated as ' ' :
  std::vector<float80> to_vec_float80(const std::string& s, const char sep) {
    const auto elements = Environment::isspace(sep) ?
      Environment::split(Environment::transform_spaces(s,' '), ' ') :
      Environment::split(Environment::remove_spaces(s), sep);
    std::vector<float80> res; res.reserve(elements.size());
    for (const auto& x : elements) res.push_back(to_float80(x));
    return res;
  }
  // Now index i is treated special, returned as F80ai, where the criterion for
  // being "integral" is the non-existence of "." :
  std::pair<std::vector<float80>, F80ai> to_vec_float80ai(const std::string& s, const char sep, const UInt_t i) {
    const auto elements = Environment::isspace(sep) ?
      Environment::split(Environment::transform_spaces(s,' '), ' ') :
      Environment::split(Environment::remove_spaces(s), sep);
    const auto N = elements.size();
    if (i >= N)
       throw std::out_of_range(
         "FloatingPoint::to_vec_float80ai(string,char,UInt_t): i="
         + std::to_string(i) + " >= size=" + std::to_string(N));
    std::vector<float80> res1; res1.reserve(N-1);
    for (UInt_t j = 0; j < N; ++j)
      if (j != i) res1.push_back(to_float80(elements[j]));
    return {res1, to_F80ai(elements[i])};
  }

  std::vector<std::vector<float80>> read_table(const std::filesystem::path& p) {
    const auto lines = Environment::get_lines(p);
    std::vector<std::vector<float80>> res; res.reserve(lines.size());
    for (const auto l : lines) {
      if (l.empty() or l.front() == '#') continue;
      res.push_back(to_vec_float80(l, ' '));
    }
    return res;
  }

  std::vector<std::pair<std::vector<float80>, F80ai>>
  read_table_ai(const std::filesystem::path& p, const UInt_t i) {
    const auto lines = Environment::get_lines(p);
    std::vector<std::pair<std::vector<float80>, F80ai>> res;
    res.reserve(lines.size());
    for (const auto l : lines) {
      if (l.empty() or l.front() == '#') continue;
      res.push_back(to_vec_float80ai(l, ' ', i));
    }
    return res;
  }


  /* Output */


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
