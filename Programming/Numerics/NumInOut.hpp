// Oliver Kullmann, 19.12.2021 (Swansea)
/* Copyright 2021, 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for numerical input and output

  Input:

    - stold(string s) (returns float80; wrapper for standard-library function)
    - stod(s) (same for float64)

    - to_float80(string s) converts s to float80 (improved stold regarding
      error-checking and -messages)
    - to_float64(string s) same for float 64

    - to_UInt(string s) converts s to UInt_t (similarly improved stoull)
      (see also toUInt below, which is more liberal, since starting from
       float80)
    - to_unsigned<UInt>(s) generalises this to unsigned integral UInt
      (recognising "-1" as maximum)
    - to_Int(s)

    - to_F80ai(string s) also considers ".", "+" and "e0"

    - to_vec_unsigned<UInt>(string s, char sep) returns a vector of UInt
    - to_vec_float80(string s, char sep) returns a vector of float80
    - to_vec_float80ai(string, char, UInt_t i) returns a pair of
      vector and F80ai (for index i)

    - typedef table_t = std::vector<std::vector<float80>>
    - read_table(tokens_t), read_table(istream&), read_table(filesystem::path)
        all -> table_t

    - typedef table_wai_t =
      std::vector<std::pair<std::vector<float80>, F80ai>>
    - read_table_ai(tokens_t, UInt_t i)
      read_table_ai(istream&, UInt_t i)
      read_table_ai(filesystem::path, UIint_t i)
        all -> table_wai_t
      (representing the lines minus component i, and component i)

    - struct Pfloat80<POL> wraps variant_t<POL> by providing string-
      input (either POL- or float80-input) and output
    - to_vec_pfloat80(string)

    - toUInt(string s) converts every string, which is convertible
      to float80, to UInt_t
    - touint(std::string s) converts every string convertible to float80
      to uint_t

    Sequences of unsigned integers:
    - sequence(vector<UInt>): sequences "a,b,c" (from a to b step c)
    - sequences(string s): such sequences, and their concatenation by ";".

  Output:

    - fullprec_float80(ostream&) sets the maximum precision
    - similarly fullprec_float64, fullprec_float32, fullprec_floatg<FLOAT>
    - Wrap, Wrap64, Wrap32 for output-streaming with full precision:
      Wrap(x) just wraps x, and output-streaming of such an object happens
      with maximal precision according to type (float80/64/32)
    - setting output-style and precision, returning old state:
       - fixed_width(ostream&, streamsize)
       - engineering_width(ostream&, streamsize)
       - default_widthengineering_width(ostream&, streamsize)
    - undo(ostream&, strstate_t) resets to the old state
    - out_fixed_width(ostream&, streamsize, float80) packages the above
      two functions (i.e., output with fixed width, without changing the
      stream-state)
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
#include <type_traits>
#include <limits>
#include <ios>
#include <typeinfo>

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
    try { x = FloatingPoint::stold(s, &converted); }
    catch (const std::invalid_argument& e) {
      std::ostringstream ss;
      ss << "FloatingPoint::to_float80(string): invalid for s=\"" << s << "\","
        " due to\n  " << e.what();
      throw std::invalid_argument(ss.str());
    }
    catch (const std::out_of_range& e) {
      std::ostringstream ss;
      ss << "FloatingPoint::to_float80(string): out-of-range for s=\"" << s
         << "\", due to\n  " << e.what();
      throw std::out_of_range(ss.str());
    }
    if (converted != s.size())
      throw std::invalid_argument
        ("FloatingPoint::to_float80(string), trailing: \""
         + s.substr(converted) + "\" in \"" + s + "\"");
    return x;
  }
  inline F80ai to_F80ai(const std::string& s) {
    return {to_float80(s),
        s.find('.') == std::string::npos,
        s.find('+') != std::string::npos,
        s.find("e0") != std::string::npos};
  }

  inline float64 stod(const std::string& s, std::size_t* const pos = 0) {
    return std::stod(s, pos);
  }

  inline float64 to_float64(const std::string& s) {
    std::size_t converted;
    double x;
    try { x = FloatingPoint::stod(s, &converted); }
    catch (const std::invalid_argument& e) {
      throw std::invalid_argument("FloatingPoint::to_float64(string), invalid"
                                  " for \"" + s + "\"");
    }
    catch (const std::out_of_range& e) {
      throw std::out_of_range("FloatingPoint::to_float64(string), \""
                              + s + "\"");
    }
    if (converted != s.size())
      throw std::invalid_argument
        ("FloatingPoint::to_float64(string), trailing: \""
         + s.substr(converted) + "\" in \"" + s + "\"");
    return x;
  }


  // Similarly, improving std::stoull:
  inline UInt_t to_UInt(const std::string& s) {
    std::size_t converted;
    unsigned long long x;
    try { x = std::stoull(s, &converted); }
    catch (const std::invalid_argument& e) {
      std::ostringstream ss;
      ss << "FloatingPoint::to_UInt(string): invalid for s=\"" << s << "\","
        " due to\n  " << e.what();
      throw std::invalid_argument(ss.str());
    }
    catch (const std::out_of_range& e) {
      std::ostringstream ss;
      ss << "FloatingPoint::to_UInt(string): out-of-range for s=\"" << s
         << "\", due to\n  " << e.what();
      throw std::out_of_range(ss.str());
    }
    if (converted != s.size())
      throw std::invalid_argument
        ("FloatingPoint::to_UInt(string), trailing: \""
         + s.substr(converted) + "\" in \"" + s + "\"");
    if (x > P264m1)
      throw std::domain_error("FloatingPoint::to_UInt(string), x=" +
                              std::to_string(x) + " > " +
                              std::to_string(P264m1));
    return x;
  }
  template <typename UInt>
  UInt to_unsigned(std::string s) {
    static_assert(std::is_integral_v<UInt>);
    static_assert(std::is_unsigned_v<UInt>);
    constexpr UInt max = std::numeric_limits<UInt>::max();
    static_assert(max <= P264m1);
    Environment::mremove_leading_spaces(s);
    if (s == "-1") return max;
    const UInt_t x = to_UInt(s);
    if (x > max)
      throw std::out_of_range("FloatingPoint::to_unsigned(string), x=" +
                              std::to_string(x) + " > " +
                              std::to_string(max));
    return x;
  }
  inline Int_t to_Int(const std::string& s) {
    std::size_t converted;
    long long x;
    try { x = std::stoll(s, &converted); }
    catch (const std::invalid_argument& e) {
      std::ostringstream ss;
      ss << "FloatingPoint::to_Int(string): invalid for s=\"" << s << "\","
        " due to\n  " << e.what();
      throw std::invalid_argument(ss.str());
    }
    catch (const std::out_of_range& e) {
      std::ostringstream ss;
      ss << "FloatingPoint::to_Int(string): out-of-range for s=\"" << s
         << "\", due to\n  " << e.what();
      throw std::out_of_range(ss.str());
    }
    if (converted != s.size())
      throw std::invalid_argument
        ("FloatingPoint::to_Int(string), trailing: \""
         + s.substr(converted) + "\" in \"" + s + "\"");
    if (x > P263m1)
      throw std::domain_error("FloatingPoint::to_Int(string), x=" +
                              std::to_string(x) + " > " +
                              std::to_string(P263m1));
    if (x < mP263)
      throw std::domain_error("FloatingPoint::to_Int(string), x=" +
                              std::to_string(x) + " < " +
                              std::to_string(mP263));
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


  template <typename UInt>
  std::vector<UInt> sequence(const std::vector<UInt> v) {
    static_assert(std::is_integral_v<UInt>);
    static_assert(std::is_unsigned_v<UInt>);
    static_assert(std::numeric_limits<UInt>::max() <= P264m1);
    typedef std::vector<UInt> v_t;
    const auto size = v.size();
    if (size <= 1 or size >= 4) return v;
    const UInt a = v[0], b = v[1], s = size == 2 ? 1 : v[2];
    if (b < a) return {};
    v_t res(1,a);
    if (a == b or s == 0) return res;
    const UInt n = (UInt_t(b) - UInt_t(a)) / UInt_t(s);
    const UInt_t np1 = UInt_t(n) + 1;
    if (np1 == 0) return {};
    res.reserve(np1);
    for (UInt_t i = 0, x = a+s; i < n; ++i, x += s) res.push_back(x);
    assert(res.size() == np1);
    return res;
  }
  template <typename UInt>
  std::vector<UInt> sequences(const std::string& s) {
    std::vector<UInt> res;
    auto split = Environment::split2(s, ';', ',');
    for (const auto& seq0 : split) {
      std::vector<UInt> seq1; seq1.reserve(seq0.size());
      for (const auto& s : seq0) seq1.push_back(to_unsigned<UInt>(s));
      const auto seq2 = sequence(std::move(seq1));
      for (UInt x : seq2) res.push_back(x);
    }
    return res;
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
  template <typename UInt>
  std::vector<UInt> to_vec_unsigned(const std::string& s, const char sep) {
    const auto elements = Environment::isspace(sep) ?
      Environment::split(Environment::transform_spaces(s,' '), ' ') :
      Environment::split(Environment::remove_spaces(s), sep);
    std::vector<UInt> res; res.reserve(elements.size());
    for (const auto& x : elements) res.push_back(to_unsigned<UInt>(x));
    return res;
  }
  // Now index i is treated special, returned as F80ai, where the criterion for
  // being "integral" is the non-existence of "." :
  std::pair<std::vector<float80>, F80ai>
  to_vec_float80ai(const std::string& s, const char sep, const UInt_t i) {
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

  typedef std::vector<std::vector<float80>> table_t;
  table_t read_table(const Environment::tokens_t& lines) {
    table_t res; res.reserve(lines.size());
    for (const auto& l : lines) {
      if (l.empty() or l.front() == '#') continue;
      std::vector<float80> v;
      try { v = to_vec_float80(l, ' '); }
      catch (const std::exception& e) {
        std::ostringstream ss;
        ss << "FloatingPoint::read_table(lines,i): failed due to\n  "
           << typeid(e).name() << "\n  " << e.what();
        throw std::runtime_error(ss.str());
      }
      res.push_back(std::move(v));
    }
    return res;
  }
  table_t read_table(const std::istream& in) {
    Environment::tokens_t lines;
    try { lines = Environment::get_lines(in); }
    catch (const std::exception& e) {
      std::ostringstream ss;
      ss << "FloatingPoint::read_table(in): failed due to\n  "
         << typeid(e).name() << "\n  " << e.what();
      throw std::runtime_error(ss.str());
    }
    return read_table(std::move(lines));
  }
  table_t read_table(const std::filesystem::path& p) {
    Environment::tokens_t lines;
    try { lines = Environment::get_lines(p); }
    catch (const std::exception& e) {
      std::ostringstream ss;
      ss << "FloatingPoint::read_table(p): failed due to\n  "
         << typeid(e).name() << "\n  " << e.what();
      throw std::runtime_error(ss.str());
    }
    return read_table(std::move(lines));
  }

  typedef std::vector<std::pair<std::vector<float80>, F80ai>> table_wai_t;
  table_wai_t read_table_ai(const Environment::tokens_t& lines,
                            const UInt_t i) {
    table_wai_t res; res.reserve(lines.size());
    for (const auto& l : lines) {
      if (l.empty() or l.front() == '#') continue;
      std::pair<std::vector<float80>, F80ai> v;
      try { v = to_vec_float80ai(l, ' ', i); }
      catch (const std::exception& e) {
        std::ostringstream ss;
        ss << "FloatingPoint::read_table_ai(lines,i): failed due to\n  "
           << typeid(e).name() << "\n  " << e.what();
        throw std::runtime_error(ss.str());
      }
      res.push_back(std::move(v));
    }
    return res;
  }
  table_wai_t read_table_ai(const std::istream& in, const UInt_t i) {
    Environment::tokens_t lines;
    try { lines = Environment::get_lines(in); }
    catch (const std::exception& e) {
      std::ostringstream ss;
      ss << "FloatingPoint::read_table_ai(in,i): failed due to\n  "
         << typeid(e).name() << "\n  " << e.what();
      throw std::runtime_error(ss.str());
    }
    return read_table_ai(std::move(lines), i);
  }
  table_wai_t read_table_ai(const std::filesystem::path& p, const UInt_t i) {
    Environment::tokens_t lines;
    try { lines = Environment::get_lines(p); }
    catch (const std::exception& e) {
      std::ostringstream ss;
      ss << "FloatingPoint::read_table_ai(p,i): failed due to\n  "
         << typeid(e).name() << "\n  " << e.what();
      throw std::runtime_error(ss.str());
    }
    return read_table_ai(std::move(lines), i);
  }


  template <typename POL>
  struct Pfloat80 {
    typedef POL policy_t;
    typedef FloatingPoint::variant_t<policy_t> variant_t;
    variant_t v;

    constexpr Pfloat80() noexcept = default;
    constexpr Pfloat80(const variant_t v) noexcept : v(v) {}
    Pfloat80(const std::string& s) : v(convert(s)) {}

    static variant_t convert(const std::string& s) noexcept {
      const auto p = read(policy_t{}, s); // ADL
      if (p) return p.value();
      else return to_float80(s);
    }

    bool operator ==(const Pfloat80&) const noexcept = default;
    auto operator <=>(const Pfloat80&) const noexcept = default;

    friend std::ostream& operator <<(std::ostream& out, const Pfloat80 pf) {
      if (pf.v.index() == 0) return out << std::get<0>(pf.v);
      else return out << std::get<1>(pf.v);
    }

  };

  template <typename POL>
  std::vector<Pfloat80<POL>> to_vec_pfloat80(const std::string& s, const char sep) {
    const auto elements = Environment::isspace(sep) ?
      Environment::split(Environment::transform_spaces(s,' '), ' ') :
      Environment::split(Environment::remove_spaces(s), sep);
    std::vector<Pfloat80<POL>> res; res.reserve(elements.size());
    for (const auto& x : elements) res.emplace_back(x);
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

  // Changing formatting and precision:
  typedef std::pair<std::ios_base::fmtflags, std::streamsize> strstate_t;
  // Fixed precision prec after the decimal point, returning old state:
  strstate_t fixed_width(std::ostream& out, const std::streamsize prec) {
    strstate_t res;
    res.first = out.flags();
    out.setf(std::ios_base::fixed, std::ios_base::floatfield);
    res.second = out.precision(prec);
    return res;
  }
  // Fixed precision with engineering style:
  strstate_t engineering_width(std::ostream& out, const std::streamsize prec) {
    strstate_t res;
    res.first = out.flags();
    out.setf(std::ios_base::scientific, std::ios_base::floatfield);
    res.second = out.precision(prec);
    return res;
  }
  // Flexible output-style:
  strstate_t default_width(std::ostream& out, const std::streamsize prec) {
    strstate_t res;
    res.first = out.flags();
    out << std::defaultfloat;
    res.second = out.precision(prec);
    return res;
  }
  // Resetting the stream to the old state:
  void undo(std::ostream& out, const strstate_t info) {
    out.precision(info.second);
    out.flags(info.first);
  }
  // Slow output:
  void out_fixed_width(std::ostream& out, const std::streamsize prec,
                       const float80 x) {
    const strstate_t old = fixed_width(out, prec);
    out << x;
    undo(out, old);
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
