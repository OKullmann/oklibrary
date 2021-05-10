// Oleg Zaikin, 7.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A program for using the Trivial class (derived from GC::Space).

  TODOS:

  1. DONE Add the default structures (from Environment).

*/

#include <iostream>
#include <memory>
#include <iomanip>

#include <cstdint>
#include <cassert>
#include <cstddef>

#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/gist.hh>

#include <ProgramOptions/Environment.hpp>
#include <Lookahead.hpp>

#include "Trivial.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.3",
        "10.5.2021",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/Examples/Trivial.cpp",
        "GPL v3"};

  namespace GC = Gecode;
  namespace LA = Lookahead;

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (LA::show_usage(proginfo, argc, argv)) return 0;

  const std::shared_ptr<Trivial::Sum> m(new Trivial::Sum(3, 0, 1));
  assert(m->valid());
  m->branching_min_var_size();
  m->print();

  // Find and print all solutions:
  LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m, true);
  stat.print();

  Environment::Index index;
  const std::string visual = argc <= index ? "" : argv[index++];
  // Visualise via Gist:
  if (visual == "-gist") LA::visualise<Trivial::Sum>(m);

}
