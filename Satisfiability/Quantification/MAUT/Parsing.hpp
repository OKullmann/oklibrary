// Oliver Kullmann, 29.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Parsing of Dimacs-input

*/

#ifndef PARSING_nJaQMv88is
#define PARSING_nJaQMv88is

#include <exception>
#include <string>
#include <istream>
#include <sstream>
#include <regex>

#include <cassert>

#include "VarLit.hpp"
#include "ClauseSets.hpp"

#include <ProgramOptions/Environment.hpp>

namespace MAUT {

  inline const std::string error_prefix = "[MAUT::";
  inline const std::string syntax_prefix = error_prefix + "Syntax] ";
  struct Syntax : std::runtime_error {
    Syntax(std::string m) : std::runtime_error(syntax_prefix + m) {}
  };
  inline const std::string number_prefix = error_prefix + "Number] ";
  struct Number : std::runtime_error {
    Number(std::string m) : std::runtime_error(number_prefix + m) {}
  };
  inline const std::string logic_prefix = error_prefix + "Logic] ";
  struct Logic : std::runtime_error {
    Logic(std::string m) : std::runtime_error(logic_prefix + m) {}
  };


  inline const std::basic_regex comment_rx("c (.*)");
  std::string comment(const std::string& l) {
    std::smatch m;
    if (not std::regex_match(l, m, comment_rx))
      throw Syntax("comment=" + l);
    assert(m.size() == 2);
    return m[1];
  }

  inline const std::string size_rx0 = "0|(?:[1-9][0-9]*)";
  inline const std::basic_regex pline_rx("p cnf (" + size_rx0 + ") (" +
                                         size_rx0 + ")");
  DimPar pline(const std::string& l) {
    std::smatch m;
    if (not std::regex_match(l, m, pline_rx)) throw Syntax("pline=" + l);
    assert(m.size() == 3);
    DimPar res;
    {const std::string n(m[1]);
     try { res.n = std::stoull(n); }
     catch (const std::out_of_range&) { throw Number("n=" + n); }
     if (not valid(res.n)) throw Number("n=" + n);
    }
    {const std::string c(m[2]);
     try { res.c = std::stoull(c); }
     catch (const std::out_of_range&) { throw Number("c=" + c); }}
    return res;
  }

  inline const std::string literal_rx0 = "-?[1-9][0-9]*";
  inline const std::basic_regex clause_rx("(?:" + literal_rx0 + " )*0");
  CL clause(const std::string& l) {
    if (l == "0") return {};
    if (not std::regex_match(l, clause_rx)) throw Syntax("clause=" + l);
    CL C;
    {std::stringstream s(l.substr(0, l.size()-2));
     std::string xs;
     while (s >> xs) {
       LIT x;
       try { x = std::stoll(xs); }
       catch (const std::out_of_range&) { throw Number("x=" + xs); }
       if (not valid(x)) throw Number("x=" + xs);
       C.push_back(x);
     }
    }
    std::sort(C.begin(), C.end(), comp);
    if (not valid(C)) throw Logic("clause=" + l);
    return C;
  }


  DimPar pline_old(const std::string& s) {
    std::stringstream in; in << s;
    {if (not in) {
       throw std::string("pline");
     }
     if (in.peek() != 'p') {
       throw std::string("pline");
     }
     std::string s; in >> s; in >> s;
     if (s != "cnf") {
       throw std::string("pline");
     }
    }
    VAR n; in >> n;
    if (not in) {
      throw std::string("pline");
    }
    if (not valid(n)) {
      throw std::domain_error("pline");
    }
    size_t c; in >> c;
    if (not in) {
      throw std::string("pline");
    }
std::cerr << n << " " << c << "\n";
    return {n,c};
  }

  CL read_cl(const std::string& s) {
std::cerr << "read_cl: " << s << "\n";
    auto lits = Environment::split(s, ' ');
    if (lits.empty())
      throw std::domain_error("read_cl");
    if (lits.back() != "0")
      throw std::domain_error("read_cl");
    lits.pop_back();
std::cerr << "In read_cl, nach pop: " << lits.size() << "\n";
    CL C;
    for (const auto& ls : lits) {
      const LIT x = std::stoll(ls);
      C.push_back(x);
    }
std::cerr << "In read_cl, vor sort: " << C.size() << "\n";
for (const LIT x : C) std::cerr << x << " ";
std::cerr << "\n";
    std::sort(C.begin(), C.end(), comp);
std::cerr << "In read_cl\n";
    if (not valid(C)) {
      throw std::domain_error("read_cl");
    }
std::cerr << "In read_cl, nach valid.\n";
    return C;
  }

  ClauseSet read(std::istream& in) {
    const auto lines = Environment::split(in, '\n');
    const auto num_lines = lines.size();
    if (in.bad()) {
      throw std::string("read");
    }
    for (const std::string l : lines)
      if (l.empty())
        throw std::string("read");
std::cerr << num_lines << "\n";
    size_t i = 0;
    while (i < num_lines and lines[i].front() == 'c') ++i;
    if (i == num_lines)
      throw std::string("read");
    const auto [n,c] = pline_old(lines[i]);
    ClauseSet res({n,c});
    ++i;
    if (num_lines - i != c)
      throw std::string("read");
std::cerr << "ZZZ\n";
    for (; i < num_lines; ++i) {
std::cerr << i << "\n";
      res.F.push_back(read_cl(lines[i]));
    }
std::cerr << "YYY\n";
   res.s = count(res.F);
    if (res.s.n != n) {
      throw std::string("read");
    }
    if (res.s.c != c) {
      throw std::string("read");
    }
std::cerr << "XXX\n";
    res.occ.enter(res.F);
    return res;
  }

}

#endif
