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

#include <utility>
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

  std::pair<VAR, size_t> pline(const std::string& s) {
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
    const auto [n,c] = pline(lines[i]);
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
