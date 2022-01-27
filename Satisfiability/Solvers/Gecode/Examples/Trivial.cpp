// Oleg Zaikin, 7.4.2021 (Irkutsk)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A program for solving Trivial::Sum problem (derived from GC::Space).

BUGS:

1.
DirMatching ./Trivial tests/app_tests/Trivial
ERROR[DirMatching]: Matching with "/home/kullmann/oklibrary/Satisfiability/Solvers/Gecode/Examples/tests/app_tests/Trivial/Empty.out_lm":
Execution-error with return-code 7.
TESTCASE-ERROR:
  "/home/kullmann/oklibrary/Satisfiability/Solvers/Gecode/Examples/tests/app_tests/Trivial/Empty.cmd"
  /home/kullmann/oklibrary/Satisfiability/Solvers/Gecode/Examples/Trivial
Standard-Error:
  "ERROR[Matching]: File "/home/kullmann/oklibrary/Satisfiability/Solvers/Gecode/Examples/SystemCalls_DirMatching_stdout_20078" has 3 lines, but the pattern-file has 2 lines.
"
Makefile:66: recipe for target 'at_Trivial' failed
make: *** [at_Trivial] Error 24


*/

#include <iostream>
#include <string>
#include <memory>
#include <iomanip>

#include <cstdint>
#include <cassert>
#include <cstddef>

#include <gecode/int.hh>
#include <gecode/search.hh>
#if GIST == 1
#include <gecode/gist.hh>
#endif

#include <ProgramOptions/Environment.hpp>

#include "../Lookahead.hpp"
#include "../Statistics.hpp"

#include "Trivial.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.0",
        "27.1.2021",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/Examples/Trivial.cpp",
        "GPL v3"};

  namespace GC = Gecode;
  namespace LA = Lookahead;

  typedef LA::option_t option_t;

   bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [algorithmic-options]" <<
#if GIST == 1
    " [visualise-options]" <<
#endif
    "\n" <<
    " algorithmic-options : " << Environment::WRP<LA::BrTypeO>{} << "\n" <<
    "                     : " << Environment::WRP<LA::BrSourceO>{} << "\n" <<
    "                     : " << Environment::WRP<LA::BrSolutionO>{} << "\n" <<
#if GIST == 1
    " visualise-options   : " << "+gist:visualise-by-gist" << "\n" <<
#endif
    "\n" <<
    " Solves the equation A+B=C via Gecode.\n";
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  Environment::Index index;
  const LA::option_t options = argc <= index ? option_t{} :
    Environment::translate<option_t>()(argv[index++], LA::sep);
#if GIST == 1
  std::string s = argc <= index ? "" : argv[index++];
  bool gist = s=="+gist" ? true : false;
#endif
  index++;
  index.deactivate();

  Statistics::SearchStat stat;
  typedef std::shared_ptr<Trivial::Sum> node_ptr;
  const node_ptr m(new Trivial::Sum(3, 0, 2, options, &stat));
  assert(m->valid());

  // Find and print solutions:
  LA::solve<Trivial::Sum>(m, true, 0, &stat);
  std::cout << stat << "\n";

#if GIST == 1
  if (gist) {
    // Visualise via Gist:
    GC::Gist::Print<Trivial::Sum> p("Print solution");
    GC::Gist::Options o;
    o.inspect.click(&p);
    GC::Gist::dfs(m.get(),o);
  }
#endif

}

