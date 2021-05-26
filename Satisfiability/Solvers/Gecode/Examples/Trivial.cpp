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

#include "../Lookahead.hpp"

#include "Trivial.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.7",
        "26.5.2021",
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

  Environment::Index index;
  const std::string s = argc <= index ? "" : argv[index++];

  typedef std::shared_ptr<Trivial::Sum> node_ptr;
  const node_ptr m(new Trivial::Sum(3, 0, 2, LA::branching_type(s)));
  assert(m->valid());
  m->print();

  // Find and print all solutions:
  LA::SearchStat stat = LA::find_all_solutions<Trivial::Sum>(m, true);
  stat.print();

  // Visualise via Gist:
  const std::string v = argc <= index ? "" : argv[index++];
  if (v == "-gist") LA::visualise<Trivial::Sum>(m);

}
