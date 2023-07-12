// Oliver Kullmann, 1.7.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for analysing qdimacs

TODOSL

1. Function snn2UInt_t (strict natnum to UInt_t)
    - Assume is_strict_natnum.
    - So the only problems can be that the number is too big; just return a
      pair of UInt_t and bool.

*/

#ifndef QDIMACSSYNTAX_s4ZN5nxQ7J
#define QDIMACSSYNTAX_s4ZN5nxQ7J

#include <utility>
#include <algorithm>
#include <exception>

#include <cstdlib>

#include <ProgramOptions/Strings.hpp>
#include <Numerics/NumTypes.hpp>
#include <Numerics/NumInOut.hpp>

#include "DimacsTools.hpp"

namespace QDimacsSyntax {

  typedef Environment::tokens_t tokens_t;
  typedef FloatingPoint::uint_t level_t;
  typedef FloatingPoint::UInt_t count_t;
  typedef FloatingPoint::float80 float_t;

  enum class Error {
    missing_parameters = 1,
    input_file_error = 2,
  };

  const std::string is_incorrect = "ERROR";
  void syntax_error() noexcept {
    std::cout << is_incorrect << std::endl;
    std::exit(0);
  }

  // Line with first non-c-line, returns true iff c-lines have syntax-error:
  std::pair<count_t, bool> analyse_comments(const tokens_t& F) noexcept {
    count_t first_nonc = 0;
    for (; first_nonc < F.size(); ++first_nonc) {
      const auto& L = F[first_nonc];
      if (not L.starts_with("c")) break;
      if (not L.starts_with("c ")) return {first_nonc, true};
    }
  return {first_nonc, false};
  }

  bool is_strict_natnum(const std::string& s) noexcept {
    if (s.empty()) return false;
    if (s[0] == '0') return s == "0";
    return std::ranges::all_of(s, [](const char c)noexcept{
                                  return '0' <= c and c <= '9';});
  }

  // True iff an error occurred:
  std::pair<DimacsTools::dimacs_pars, bool>
  analyse_parline(const std::string& L) noexcept {
    const auto S = Environment::split(L, ' ');
    if (S.size() != 4 or S[0] != "p" or S[1] != "cnf" or
        not is_strict_natnum(S[2]) or not is_strict_natnum(S[3])
        or L.back() == ' ') return {{}, true};
    count_t n, c;
    try {
      n = FloatingPoint::to_UInt(S[2]);
      c = FloatingPoint::to_UInt(S[3]);
    }
    catch(const std::exception&) { return {{}, true}; }
    return {{n,c}, false};
  }

  bool begins_ae(const std::string& s) noexcept {
    return s.starts_with("a ") or s.starts_with("e ");
  }
  count_t first_nonae(const tokens_t& F, count_t i) noexcept {
    while (i < F.size() and begins_ae(F[i])) ++i;
    return i;
  }
  // Returns empty set iff error was found:
  std::set<count_t> analyse_numbers_ae(const std::string& s,
                                       const count_t n,
                                       const count_t level,
                                       count_t& spaces) noexcept {
    assert(s.starts_with("a ") or s.starts_with("e "));
    assert(s.ends_with(" 0"));
    const auto size = s.size();
    if (size <= 4) {
      if (level >= 1)
        std::cout << "\nempty a/e-line\n";
      return {};
    }
    if (s[size-3] == ' ') ++spaces;
    const auto split = Environment::split(s.substr(2, size-4), ' ');
    std::set<count_t> res;
    for (count_t i = 0; i < split.size(); ++i) {
      const std::string& entry = split[i];
      if (entry.empty()) {++spaces; continue;}
      if (not is_strict_natnum(entry)) {
        if (level >= 1)
          std::cout << "\nwrong entry \"" << entry << "\"\n";
        return {};
      }
      const count_t x = FloatingPoint::to_UInt(entry);
      if (x == 0) {
        if (level >= 1)
          std::cout << "\nwrong entry 0\n";
        return {};
      }
      if (x > n) {
        if (level >= 1)
          std::cout << "\nwrong entry " << x << " > max-n = " << n << "\n";
        return {};
      }
      if (res.contains(x)) {
        if (level >= 1)
          std::cout << "\nrepeated entry " << x << "\n";
        return {};
      }
      res.insert(x);
    }
    if (res.empty()) {
      if (level >= 1) {
        std::cout << "\na/e-line only contains spaces\n";
      }
      return {};
    }
    return res;
  }

  // The variables of the a-e-lines (second coordinate != end iff error found):
  std::pair<std::vector<std::set<count_t>>, count_t>
  readae(const tokens_t& F,
         const count_t n,
         const count_t begin, const count_t end,
         const count_t level) {
    assert(begin < end and end < F.size());
    std::vector<std::set<count_t>> res(end - begin);
    count_t i = begin;
    count_t additional_spaces = 0;
    for (; i < end; ++i) {
      const count_t i0 = i - begin;
      res[i0] = analyse_numbers_ae(F[i], n, level, additional_spaces);
      if (res[i0].empty()) break;
      // check for disjointness XXX
    }
    if (level >= 2)
      std::cout << "add-spaces-ae " << additional_spaces << "\n";
    return {res, i};
  }

}

#endif
