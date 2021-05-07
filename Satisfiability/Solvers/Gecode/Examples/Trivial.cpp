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
        "0.1.9",
        "7.5.2021",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/Examples/Trivial.cpp",
        "GPL v3"};

  namespace GC = Gecode;
  namespace LA = Lookahead;

  typedef std::shared_ptr<Trivial::Sum> node_ptr;

  LA::SearchStat find_all_solutions(const node_ptr m) noexcept {
    assert(m->valid());
    GC::DFS<Trivial::Sum> e(m.get());
    LA::SearchStat stat;
    while (const node_ptr s{e.next()}) {
      s->print();
      ++stat.solutions;
    }
    stat.engine = e.statistics();
    return stat;
  }
}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (LA::show_usage(proginfo, argc, argv)) return 0;

  const node_ptr m(new Trivial::Sum(3, 0, 1));
  assert(m->valid());
  m->branching_min_var_size();
  m->print();

  LA::SearchStat stat = find_all_solutions(m);

  stat.print();

  // Visualise via Gist:
  Environment::Index index;
  const std::string visual = argc <= index ? "" : argv[index++];
  if (visual == "-gist") {
    GC::Gist::Print<Trivial::Sum> p("Print solution");
    GC::Gist::Options o;
    o.inspect.click(&p);
    GC::Gist::dfs(m.get(),o);
  }

}
