// Oleg Zaikin, 7.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A program for using the Trivial class (derived from GC::Space).

  TODOS:

  1. Add the default structures (from Environment).

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
        "0.1.6",
        "7.5.2021",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/Examples/Trivial.cpp",
        "GPL v3"};

  namespace GC = Gecode;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  typedef std::uint64_t count_t;
  typedef std::unique_ptr<Trivial::Sum> node_ptr;

  const node_ptr m(new Trivial::Sum(3, 0, 1));
  assert(m->valid());
  m->branching_min_var_size();
  m->print();
  GC::DFS<Trivial::Sum> e(m.get());

  count_t solutions = 0;
  while (const node_ptr s{e.next()}) {
    s->print();
    ++solutions;
  }

  const GC::Search::Statistics stat = e.statistics();

  using std::setw;
  const auto w = setw(10);

  std::cout << stat.node << w << stat.fail << w << solutions << "\n";

}
