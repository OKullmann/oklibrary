// Oliver Kullmann, 1.7.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for analysing qdimacs

TODOS:

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
#include <vector>
#include <tuple>

#include <cstdlib>

#include <ProgramOptions/Strings.hpp>
#include <Numerics/NumTypes.hpp>
#include <Numerics/NumInOut.hpp>

#include "DimacsTools.hpp"
#include "Algorithms.hpp"

namespace QDimacsSyntax {

  typedef Environment::tokens_t tokens_t;
  typedef FloatingPoint::uint_t level_t;
  typedef FloatingPoint::UInt_t count_t;
  typedef FloatingPoint::float80 float_t;

  enum class Error {
    missing_parameters = 1,
    input_file_error = 2,
    replacement_file_error = 3,
    replacement_output_error = 4,
  };

  const std::string grepstr = "GREP";

  const std::string is_incorrect = "ERROR";
  constexpr std::streamsize width_code = 3;
  void syntax_error(const int code) noexcept {
    std::cout << is_incorrect << std::setfill('0') <<
      std::setw(width_code) << code << std::endl;
    std::exit(0);
  }

  std::pair<std::pair<Environment::tokens_t, bool>, count_t>
  get_lines(std::ifstream& input) {
    assert(input.good());
    std::pair<std::pair<Environment::tokens_t, bool>, count_t>
      res{Environment::get_lines_check(input), 0};
    if (not res.first.second) return res;
    for (auto it = res.first.first.rbegin();
         it != res.first.first.rend(); ++it)
      if (it->empty()) ++res.second;
      else break;
    res.first.first.resize(res.first.first.size() - res.second);
    return res;
  }

  // Line with first non-c-line, returns true iff c-lines have syntax-error:
  std::pair<count_t, bool> analyse_comments(const tokens_t& F,
                                            const level_t tolerance) noexcept {
    count_t first_nonc = 0;
    for (; first_nonc < F.size(); ++first_nonc) {
      const auto& L = F[first_nonc];
      if (not L.starts_with("c")) break;
      if (tolerance == 0 and (L.size() == 1 or L[1] != ' '))
        return {first_nonc, true};
    }
    return {first_nonc, false};
  }

  // Allows s=="0":
  bool is_strict_natnum(const std::string_view s) noexcept {
    if (s.empty()) return false;
    if (s[0] == '0') return s == "0";
    return std::ranges::all_of(s, [](const char c)noexcept{
                                  return '0' <= c and c <= '9';});
  }

  // True iff an error occurred:
  std::pair<DimacsTools::dimacs_pars, bool>
  analyse_parline(const std::string& L, const level_t tolerance) noexcept {
    const auto S = Environment::split(L, ' ');
    if (tolerance == 0) {
      if (S.size() != 4 or S[0] != "p" or S[1] != "cnf" or
          not is_strict_natnum(S[2]) or not is_strict_natnum(S[3])
          or L.back() == ' ') return {{}, true};
      count_t n, c;
      try {
        n = FloatingPoint::to_UInt(S[2]);
        c = FloatingPoint::to_UInt(S[3]);
      }
      catch(const std::exception&) { return {{}, true}; }
      if (n == RandGen::max_var) return {{}, true};
      return {{n,c}, false};
    }
    else {
      if (S.size() < 4 or S[0] != "p" or S[1] != "cnf" or
          not is_strict_natnum(S[2])) return {{}, true};
      count_t cindex = 3;
      while (cindex < S.size() and S[cindex].empty()) ++cindex;
      if (cindex == S.size() or not is_strict_natnum(S[cindex]))
        return {{}, true};
      count_t n, c;
      try {
        n = FloatingPoint::to_UInt(S[2]);
        c = FloatingPoint::to_UInt(S[cindex]);
      }
      catch(const std::exception&) { return {{}, true}; }
      if (n == RandGen::max_var) return {{}, true};
      ++cindex; while (cindex < S.size() and S[cindex].empty()) ++cindex;
      if (cindex != S.size()) return {{}, true};
      return {{n,c}, false};
    }
  }

  bool begins_ae(const std::string& s) noexcept {
    return s.starts_with("a ") or s.starts_with("e ");
  }
  count_t first_nonae(const tokens_t& F, count_t i) noexcept {
    while (i < F.size() and begins_ae(F[i])) ++i;
    return i;
  }
  // Returns empty set iff error was found, otherwise the variables in
  // ae-line s:
  std::set<count_t> analyse_numbers_ae(const std::string& s,
                                       const count_t n,
                                       const level_t verbosity,
                                       count_t& spaces) noexcept {
    assert(s.starts_with("a ") or s.starts_with("e "));
    assert(s.ends_with(" 0"));
    const auto size = s.size();
    if (size <= 4) {
      if (verbosity >= 1)
        std::cout << "\nempty a/e-line\n";
      return {};
    }
    if (s[size-3] == ' ') ++spaces;
    const auto split =
      Environment::split(std::string_view(s).substr(2, size-4), ' ');
    std::set<count_t> res;
    for (count_t i = 0; i < split.size(); ++i) {
      const std::string& entry = split[i];
      if (entry.empty()) {++spaces; continue;}
      if (not is_strict_natnum(entry)) {
        if (verbosity >= 1)
          std::cout << "\nwrong entry \"" << entry << "\"\n";
        return {};
      }
      const count_t x = FloatingPoint::to_UInt(entry);
      if (x == 0) {
        if (verbosity >= 1)
          std::cout << "\nwrong entry 0\n";
        return {};
      }
      if (x > n) {
        if (verbosity >= 1)
          std::cout << "\nwrong entry " << x << " > max-n = " << n << "\n";
        return {};
      }
      if (res.contains(x)) {
        if (verbosity >= 1)
          std::cout << "\nrepeated entry " << x << "\n";
        return {};
      }
      res.insert(x);
    }
    if (res.empty()) {
      if (verbosity >= 1) {
        std::cout << "\na/e-line only contains spaces\n";
      }
      return {};
    }
    return res;
  }

  bool empty_ae_line(const std::string& s) noexcept {
    assert(s.size() >= 2);
    const std::string_view v(s.begin()+1, s.end()-1);
    return v.find_first_not_of(' ') == std::string::npos;
  }

  // The variables of the a-e-lines (second coordinate != end iff error found):
  std::tuple<std::vector<std::set<count_t>>, count_t, count_t>
  readae(const tokens_t& F,
         const count_t n,
         const count_t begin0, const count_t end,
         const level_t verbosity,
         const level_t tolerance) {
    assert(begin0 < end and end < F.size());
    const count_t begin =
      tolerance >= 3 and empty_ae_line(F[begin0]) ? begin0+1 : begin0;
    std::tuple<std::vector<std::set<count_t>>, count_t, count_t>
      res(end - begin, begin, begin - begin0);
    auto& [vars, index, increment] = res; // increment is to be ignored
    count_t additional_spaces = 0;
    for (; index < end; ++index) {
      const count_t i0 = index - begin;
      vars[i0] = analyse_numbers_ae(F[index], n, verbosity, additional_spaces);
      if (vars[i0].empty()) break;
      for (count_t j0 = 0; j0 < i0; ++j0) {
        if (not Algorithms::empty_intersection(vars[i0], vars[j0])) {
          if (verbosity >= 1) {
            const auto intersection =
              Algorithms::intersection(vars[i0], vars[j0]);
            assert(not intersection.empty());
            std::cout << "\na/e-line " << j0 << " intersects with line "
                      << i0 << " in " << intersection.size() << " elements";
            if (verbosity >= 2) {
              std::cout << ":\n";
              Environment::out_line(std::cout, intersection);
            }
            else
              std::cout << "; first: " << intersection.front();
            std::cout <<  "\n";
          }
          return res;
        }
      }
    }
    if (verbosity >= 2)
      std::cout << "add-spaces-ae " << additional_spaces << "\n";
    return res;
  }

  count_t max_ae_index(const std::vector<std::set<count_t>>& vars) noexcept {
    count_t res = 0;
    for (const auto& S : vars) {
      assert(not S.empty());
      res = std::max(res, *S.crbegin());
    }
    return res;
  }
  // Whether block 0 <= i < vars.size() is universal:
  std::vector<bool>
  is_universal_block(const std::vector<std::set<count_t>>& vars,
                     const bool first_a) {
    const auto size = vars.size();
    assert(size != 0);
    std::vector<bool> res(size);
    res[0] = first_a;
    for (count_t i = 1; i < size; ++i) res[i] = not res[i-1];
    return res;
  }
  // Whether variable v is in the ae-blocks:
  std::vector<bool>
  is_ae_var(const std::vector<std::set<count_t>>& vars,
                   const count_t n) {
    std::vector<bool> res(n+1);
    for (const auto& S : vars)
      for (const auto v : S) res[v] = true;
    return res;
  }
  // Whether variable v is universal:
  std::vector<bool>
  is_universal_var(const std::vector<std::set<count_t>>& vars,
                   const std::vector<bool>& unib,
                   const count_t n) {
    assert(vars.size() == unib.size());
    std::vector<bool> res(n+1);
    for (count_t i = 0; i < vars.size(); ++i) {
      const bool is_uni = unib[i];
      for (const auto v : vars[i]) res[v] = is_uni;
    }
    return res;
  }

  // Input is the clause-string s without final " 0", extracting literals
  // as they appear, second component false iff syntax-error with
  // the literals:
  std::pair<DimacsTools::Clause, bool>
  extract_literals(const std::string_view s,
                   const count_t n,
                   const level_t verbosity,
                   count_t& spaces) {
    const auto size = s.size();
    if (size == 0) {
      if (verbosity >= 1)
        std::cout << "\nempty clause\n";
      return {};
    }
    if (s[size-1] == ' ') ++spaces;
    const auto split = Environment::split(s, ' ');
    using Var = DimacsTools::Var;
    std::pair<DimacsTools::Clause, bool> res{};
    for (count_t i = 0; i < split.size(); ++i) {
      const std::string_view entry(split[i]);
      if (entry.empty()) {++spaces; continue;}
      const count_t start = entry[0] == '-';
      const std::string_view num = entry.substr(start);
      if (not is_strict_natnum(num)) {
        if (verbosity >= 1)
          std::cout << "\nwrong variable \"" << num << "\"\n";
        return {};
      }
      const Var v(FloatingPoint::to_UInt(std::string(num)));
      if (v.v == 0) {
        if (verbosity >= 1)
          std::cout << "\nwrong variable 0\n";
        return {};
      }
      if (v.v > n) {
        if (verbosity >= 1)
          std::cout << "\nwrong variable " << v << " > max-n = " << n << "\n";
        return {};
      }
      res.first.emplace_back(start==0, v);
    }
    res.second = true;
    return res;
  }

  // The string_view-data is nullptr iff no " 0" was found:
  std::pair<std::string_view, std::string::size_type>
  literal_part(const std::string& s) noexcept {
    const auto end = s.rfind(" 0");
    if (end == std::string::npos) return {};
    if (s.find_first_not_of(' ', end+2) != std::string::npos) return {};
    return {{s.begin(), s.begin() + end}, s.size() - end - 2};
  }
  // Returns size of clause, and false iff error:
  typedef std::vector<count_t> degvec_t;
  std::pair<count_t, bool>
  analyse_clause(const std::string& s0, degvec_t& pos, degvec_t& neg,
                 const count_t n, const level_t verbosity,
                 const std::vector<bool>& aev,
                 const std::vector<bool>& univ,
                 count_t& spaces,
                 const level_t tolerance,
                 count_t& repetitions,
                 const bool replace,
                 DimacsTools::ClauseList& F) {
    const auto [s, trailing_spaces] = literal_part(s0);
    if (s.data() == nullptr) {
      if (verbosity >= 1)
        std::cout << "\nclause not containing final \" 0\"\n";
      return {};
    }
    spaces += trailing_spaces;
    if (tolerance == 0 and trailing_spaces != 0) {
      if (verbosity >= 1)
        std::cout << "\nclause containing trailing spaces\n";
      return {};
    }

    auto [C, correct_literals] = extract_literals(s, n, verbosity, spaces);
    if (not correct_literals) return {};
    const auto size = C.size();
    if (size == 0) {
      if (verbosity >= 1)
        std::cout << "\nempty clause\n";
      return {};
    }
    using DimacsTools::Lit; using DimacsTools::Var;
    for (const Lit x : C) {
      const Var v = x.v;
      if (not aev[v.v]) {
        if (verbosity >= 1)
          std::cout << "\nnon-ae-variable " << v << "\n";
        return {};
      }
    }
    std::ranges::sort(C);
    {const auto complementary = std::ranges::adjacent_find(C,
       [](const Lit x, const Lit y) noexcept { return x == -y; });
     if (complementary != C.end()) {
       if (tolerance <= 1) {
         if (verbosity >= 1)
           std::cout << "\ncomplementary literal " << *complementary << "\n";
         return {};
       }
       else return {{}, true};
     }
    }
    C.erase(std::unique(C.begin(), C.end()), C.end());
    const auto nsize = C.size();
    if (nsize != size) {
      if (tolerance <= 1) {
        if (verbosity >= 1)
          std::cout << "\n" << size - nsize << " repeated literals\n";
        return {};
      }
      else repetitions += size - nsize;
    }
    if (std::ranges::all_of(C, [&univ](const Lit x) noexcept {
                              return univ[x.v.v];})) {
      if (verbosity >= 1) {
        std::cout << "\nclause only contains universal variables\n";
      }
      return {};
    }
    if (nsize == 0) return {0, true};
    for (const Lit x : C) if (x.s) ++pos[x.v.v]; else ++neg[x.v.v];
    if (replace) F.push_back(std::move(C));
    return {nsize, true};
  }

  // Formal and pure (non-formal) global variables, and global variables to
  // be removed:
  std::tuple<count_t, count_t, std::set<count_t>>
  num_pure_global_vars(const bool first_a,
                       const std::set<count_t>& G,
                       const degvec_t& pos, const degvec_t& neg) {
    std::tuple<count_t, count_t, std::set<count_t>> res{};
    if (not first_a) return res;
    for (const count_t v : G) {
      assert(v < pos.size()); assert(v < neg.size());
      const bool zpos = pos[v] == 0, zneg = neg[v] == 0;
      if (zpos or zneg) {
        std::get<2>(res).insert(v);
        if (zpos) { std::get<0>(res) += zneg; std::get<1>(res) += not zneg; }
        else ++std::get<1>(res);
      }
    }
    return res;
  }

  count_t update_max_ae(const size_t old_max_ae,
                        const std::set<count_t>& rem_g,
                        const std::vector<bool>& is_ae) noexcept {
    assert(old_max_ae != 0);
    assert(old_max_ae < is_ae.size() and is_ae[old_max_ae]);
    count_t res = old_max_ae;
    if (not rem_g.contains(res)) return res;
    --res;
    while (rem_g.contains(res) or not is_ae[res]) --res;
    assert(res != 0);
    return res;
  }

}

#endif
