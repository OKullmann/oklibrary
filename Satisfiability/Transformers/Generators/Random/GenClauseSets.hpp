// Oliver Kullmann, 23.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Components for generalised clause-sets

*/

#ifndef GENCLAUSESETS_w1SoFOhS1G
#define GENCLAUSESETS_w1SoFOhS1G

#include <vector>
#include <ostream>
#include <utility>
#include <exception>

#include <ProgramOptions/Strings.hpp>

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

    GClauseList() noexcept = default;
    GClauseList(gclauseset_t F0) : F(std::move(F0)), dom(dom_sizes(F)) {}
    GClauseList(gclauseset_t F0, domsizes_t dom0) noexcept :
    F(std::move(F0)), dom(std::move(dom0)) {}

    var_t n() const noexcept { return dom.size(); }
    count_t c() const noexcept { return F.size(); }

    bool valid() const noexcept {
      for (const auto& C : F)
        for (const auto& x : C)
          if (x.v >= dom.size() or x.e >= dom[x.v]) return false;
      return true;
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
      const auto mes = [line_counter, &error]
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
          sline >> item;
          if (item.empty()) n_set = false;
          else {
            n_set = true;
            current_n = FloatingPoint::to_UInt(item);
            maxn = std::max(maxn, current_n);
          }
        }
        else if (line.front() == numclchar) {
          if (c_set) {
            const count_t clauses_read = F.c() - last_c;
            if (current_c != clauses_read)
              throw ClauseListReadError(mes() + "found " +
                to_string(clauses_read) + " # c=" + to_string(current_c));
          }
          std::string item; sline >> item;
          const std::string expected(1,numclchar);
          if (item != expected)
            throw ClauseListReadError(mes() + "expected \"" + expected + "\", "
                                      "but found \"" + item + "\"");
          sline >> item;
          if (item.empty()) c_set = false;
          else {
            c_set = true;
            last_c = F.c();
            current_c = FloatingPoint::to_UInt(item);
          }
        }
        else {
          GC::GClause C;
          sline >> C;
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
          throw ClauseListReadError(mes() + "found " +
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

}

#endif
