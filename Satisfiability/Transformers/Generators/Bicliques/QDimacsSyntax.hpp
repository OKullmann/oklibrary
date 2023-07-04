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

  // Global variables and other-variables:
  std::array<std::set<count_t>, 2> readae(const tokens_t& F,
                                          const count_t begin,
                                          const count_t end) {
    assert(begin < end and end < F.size());
    std::array<std::set<count_t>, 2> res;
    
  }

}

#endif
