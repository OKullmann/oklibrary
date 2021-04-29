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
  inline const std::string file_prefix = error_prefix + "File] ";
  struct File : std::runtime_error {
    File(std::string m) : std::runtime_error(file_prefix + m) {}
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

  ClauseSet read(std::istream& in) {
    size_t i = 0;
    std::string line;
    for (; in.peek() == 'c'; std::getline(in, line), ++i);
    if (in.bad()) throw File("bad reading after comments");
    std::getline(in, line), ++i;
    if (in.bad()) throw File("bad reading after p-line");
    ClauseSet res(pline(line));
    while (std::getline(in, line)) res.F.push_back(clause(line));
    if (in.bad()) throw File("bad reading after clauses");
    res.update();
    return res;
  }

}

#endif
