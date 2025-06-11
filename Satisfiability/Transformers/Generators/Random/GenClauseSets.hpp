// Oliver Kullmann, 23.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Components for generalised clause-sets


TODOS:

0. Write (above) a short summary of the components (also in the
   other files).

1. Naming of the file format
  - NOBOCONF ("non-boolean conjunctive normal forms") seems reasonable.

2. Document the basics of NOBOCONF

3. Document the details of NOBOCONF
  - Where are "spaces" allowed (and what are these?)?
  - What is allowed for the "numbers" ?

*/

#ifndef GENCLAUSESETS_w1SoFOhS1G
#define GENCLAUSESETS_w1SoFOhS1G

#include <vector>
#include <ostream>
#include <utility>
#include <exception>
#include <array>
#include <algorithm>

#include <ProgramOptions/Strings.hpp>

// Guaranteed to be included:
#include "GenLit.hpp"
#include "GenClauses.hpp"

namespace GenClauseSets {

  namespace GL = GenLit;
  namespace GC = GenClauses;

  // Three typedefs for std::uint64_t :
  using var_t = GL::var_t;
  using val_t = GL::val_t;
  using count_t = GL::var_t;

  using VarVal = GL::VarVal;


  constexpr char comchar = 'C';
  constexpr char numvarchar = 'n';
  constexpr char numclchar = 'c';


  typedef std::vector<GC::GClause> gclauseset_t;
  typedef std::vector<GL::val_t> domsizes_t;

  // The maximal variable-index + 1 (and checking that variables and values
  // are not singular):
  var_t num_var(const gclauseset_t& F) {
    const std::string error = "GenClauseSets::num_var: ";
    var_t res = 0;
    for (const auto& C : F)
      for (const auto& x : C)
        if (x.v == GL::singvar)
          throw GL::LiteralReadError(error + "singular variable in " +
                                     to_string(x));
        else if (x.e == GL::singval)
          throw GL::LiteralReadError(error + "singular value in " +
                                     to_string(x));
        else if (x.v >= res) res = x.v+1;
    return res;
  }
  // Sets the domain-sizes as found in F:
  domsizes_t dom_sizes(const gclauseset_t& F) {
    domsizes_t res(num_var(F));
    for (const auto& C : F)
      for (const auto& x : C) {
        val_t& dom = res[x.v];
        if (x.e >= dom) dom = x.e+1;
      }
    return res;
  }


  struct ClauseListReadError : std::runtime_error {
    static std::string add(const std::string& m) noexcept {
      return std::string("GenClauseSets::ClauseListReadError: ") + m;
    }
    ClauseListReadError(const std::string& m) noexcept :
    std::runtime_error(add(m)) {}
  };


  struct GClauseList {
    gclauseset_t F;
    domsizes_t dom;

    constexpr GClauseList() noexcept = default;
    explicit GClauseList(gclauseset_t F0)
      : F(std::move(F0)), dom(dom_sizes(F)) {}
    GClauseList(gclauseset_t F0, domsizes_t dom0) noexcept
      : F(std::move(F0)), dom(std::move(dom0)) {}
    template <class RAN>
    // GCC 14.3 ERROR: constructor using std::from_range not implemented:
    // GClauseList(std::from_range_t, RAN&& r) : F(std::from_range, r), dom(dom_sizes(F)) {}
    GClauseList(std::from_range_t, RAN&& r)
      : F(std::ranges::to<gclauseset_t>(std::forward<RAN>(r))),
        dom(dom_sizes(F)) {}
    GClauseList(const std::initializer_list<GC::GClause> init)
      : F(init), dom(dom_sizes(F)) {}

    constexpr var_t n() const noexcept { return dom.size(); }
    constexpr count_t c() const noexcept { return F.size(); }

    constexpr bool valid() const noexcept {
      for (const auto& C : F)
        for (const auto& x : C)
          if (x.v >= dom.size() or x.e >= dom[x.v]) return false;
      return true;
    }


    constexpr bool clauses_sorted() const noexcept {
      return std::ranges::all_of(F, [](const GC::GClause& C)noexcept{
                                   return C.is_sorted();});
    }
    void sort_clauses() noexcept { for (auto& C : F) C.sort(); }

    constexpr bool has_consecutive_duplicates() const noexcept {
      return std::ranges::any_of(F, [](const GC::GClause& C)noexcept{
                                     return C.has_consecutive_duplicates();});
    }
    // Returns total number of eliminated literal-occurrences:
    count_t remove_consecutive_duplicates() noexcept {
      count_t res = 0;
      for (auto& C : F) res += C.remove_consecutive_duplicates();
      return res;
    }

    bool no_consecutive_clashes() const noexcept {
      return not std::ranges::any_of(F, GC::has_consecutive_clashes);
    }
    // Returns number of eliminated clauses; assumes all clauses are sorted,
    // more precisely, finds only consecutive clashing literals:
    count_t remove_with_consecutive_clashes() noexcept {
      const auto to_be_removed =
        std::ranges::remove_if(F, GC::has_consecutive_clashes);
      const count_t res = to_be_removed.size();
      F.erase(to_be_removed.begin(), to_be_removed.end());
      return res;
    }

    constexpr bool clauselist_sorted() const noexcept {
      return std::ranges::is_sorted(F);
    }
    void sort_clauselist() noexcept { std::ranges::sort(F); }

    constexpr bool has_consecutive_duplicated_clauses() const noexcept {
      return std::ranges::adjacent_find(F) != F.end();
    }
    count_t remove_consecutive_duplicated_clauses() noexcept {
      const auto left_over = std::ranges::unique(F);
      const count_t res = left_over.size();
      F.erase(left_over.begin(), left_over.end());
      return res;
    }

    bool is_standardised() const noexcept {
      if (not clauses_sorted()) return false;
      if (has_consecutive_duplicates()) return false;
      if (not no_consecutive_clashes()) return false;
      return true;
    }
    // The number of eliminated clauses and eliminated duplicated literal-
    // occurrences:
    typedef std::array<count_t, 2> elim_cl_t;
    elim_cl_t standardise() noexcept {
      sort_clauses();
      const count_t ed = remove_consecutive_duplicates();
      const count_t et = remove_with_consecutive_clashes();
      return {et, ed};
    }

    bool is_fully_standardised() const noexcept {
      if (not is_standardised()) return false;
      if (not clauselist_sorted()) return false;
      return true;
    }
    elim_cl_t fully_standardise() noexcept {
      const elim_cl_t res = standardise();
      sort_clauselist();
      return res;
    }

    bool is_clauseset() const noexcept {
      if (not is_fully_standardised()) return false;
      if (has_consecutive_duplicated_clauses()) return false;
      return true;
    }
    // Number of eliminated tautological clauses, duplicated clauses,
    // duplicated literal-occurrences;
    typedef std::array<count_t, 3> elim_ccl_t;
    elim_ccl_t make_clauseset() noexcept {
      const elim_cl_t cl = fully_standardise();
      const count_t d = remove_consecutive_duplicated_clauses();
      return {cl[0], d, cl[1]};
    }


    void spiking(const var_t offset = 0) {
      const count_t numcl = F.size();
      for (var_t i = 0; i < numcl; ++i)
        F[i].push_back(GL::spiking_lit(offset+i));
    }
    count_t unspiking() {
      count_t sum = 0;
      for (GC::GClause& C : F) {
        const auto left_over = std::ranges::remove_if(C, GL::is_spiking_lit);
        sum += left_over.size();
        C.C.erase(left_over.begin(), left_over.end());
      }
      sort_clauselist();
      return sum;
    }

    auto operator <=>(const GClauseList&) const noexcept = default;


    friend std::ostream& operator <<(std::ostream& out, const GClauseList& F) {
      out << numvarchar << " " << F.n() << "\n"
          << numclchar << " " << F.c() << "\n";
      for (const auto& C : F.F) out << C;
      return out;
    }
    friend std::istream& operator >>(std::istream& in, GClauseList& F) {
      const var_t start_n = F.n();
      var_t maxn = start_n, current_n;
      const count_t start_c = F.c();
      count_t last_c = start_c, current_c;
      bool n_set = false, c_set = false;

      count_t line_counter = 0;
      const std::string
        error("In operator >>(std::istream, GClauseList), line=");
      using std::to_string;
      const auto mes = [&line_counter, &error]
        {return error + to_string(line_counter) + " : ";};

      for (std::string line; std::getline(in, line);) {
        ++line_counter;
        if (line.empty()) continue;
        else if (line.front() == comchar) continue;
        std::istringstream sline(line);

        if (line.front() == numvarchar) {
          std::string item; sline >> item;
          const std::string expected(1,numvarchar);
          if (item != expected)
            throw ClauseListReadError(mes() + "expected \"" + expected + "\", "
                                      "but found \"" + item + "\"");
          item.clear(); sline >> item;
          if (item.empty()) n_set = false;
          else {
            n_set = true;
            try { current_n = FloatingPoint::to_UInt(item); }
            catch(const std::exception& e) {
              throw ClauseListReadError(mes() + "reading n-value yields:\n" +
                                        e.what());
            }
            maxn = std::max(maxn, current_n);
          }
        }

        else if (line.front() == numclchar) {
          if (c_set) {
            const count_t clauses_read = F.c() - last_c;
            if (current_c != clauses_read)
              throw ClauseListReadError(mes() + "read " +
                to_string(clauses_read) + " # c=" + to_string(current_c));
          }
          std::string item; sline >> item;
          const std::string expected(1,numclchar);
          if (item != expected)
            throw ClauseListReadError(mes() + "expected \"" + expected + "\", "
                                      "but found \"" + item + "\"");
          item.clear(); sline >> item;
          if (item.empty()) c_set = false;
          else {
            c_set = true;
            last_c = F.c();
            try { current_c = FloatingPoint::to_UInt(item); }
            catch(const std::exception& e) {
              throw ClauseListReadError(mes() + "reading c-value yields:\n" +
                                        e.what());
            }
          }
        }

        else {
          GC::GClause C;
          try { sline >> C; }
          catch(const std::exception& e) {
            throw ClauseListReadError(mes() + "reading clause yields:\n" +
                                      e.what());
          }
          for (const auto& x : C) {
            if (GL::sing(x))
              throw GL::LiteralReadError(mes() + "singular literal " +
                                         to_string(x));
            if (n_set) {
              if (x.v >= current_n)
                throw ClauseListReadError(mes() + "variable=" +
                  to_string(x.v) + " >= n=" + to_string(current_n));
            }
            else maxn = std::max(maxn, x.v + 1);
          }
          F.F.push_back(std::move(C));
        }
      }
      const count_t final_c = F.c();
      if (c_set) {
        const count_t clauses_read = final_c - last_c;
        if (current_c != clauses_read)
          throw ClauseListReadError(mes() + "read " +
            to_string(clauses_read) + " # c=" + to_string(current_c));
      }
      F.dom.resize(maxn);
      for (count_t i = start_c; i < final_c; ++i) {
        const auto& C = F.F[i];
        for (const auto& x : C) {
          val_t& dom = F.dom[x.v];
          if (x.e >= dom) dom = x.e+1;
        }
      }
      return in;
    }

  };


  // Excluding final-end-of-line:
  template <class RANGE>
  void out_datacomment(std::ostream& out, const std::string& name,
                       const count_t width, const RANGE& r,
                       const std::string& sep = " ",
                       const std::streamsize w = 0) {
    const std::string header = std::string(1,comchar) + name;
    const count_t nwidth = std::max(width, header.size());
    const auto flags = out.flags();
    out.width(nwidth); out << std::left;
    out << header; Environment::out_line(std::cout, r, sep, w);
    out.flags(flags);
  }

}

#endif
