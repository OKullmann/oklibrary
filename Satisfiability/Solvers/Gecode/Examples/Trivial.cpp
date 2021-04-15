// Oleg Zaikin, 7.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A program for using the Trivial class (derived from Gecode::Space).

  TODOS:

  1. Add the default structures (from Environment).

*/

#include <memory>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Trivial.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.4",
        "15.4.2021",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/Examples/Trivial.cpp",
        "GPL v3"};

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  const std::unique_ptr<Trivial::Sum> m(new Trivial::Sum(3, 0, 1));
  m->branching_min_var_size();
  m->print();
  Gecode::DFS<Trivial::Sum> e(m.get());
  while (Trivial::Sum* const s = e.next()) {
    s->print();
    delete s;
  }

}
