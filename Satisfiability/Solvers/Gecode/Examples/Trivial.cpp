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

#include <cassert>

#include "Trivial.hpp"

int main() {
  const Trivial::Sum m(3, 0, 2);
  assert(m.size() == 3);
  m.print();
  Trivial::Sum m2(m); // calling the real copy-constructor, since m is const
  assert(m == m2);

  Trivial::Sum* const m3 = new Trivial::Sum(3, 0, 1);
  m3->branch_min_var_size();
  Gecode::DFS<Trivial::Sum> e(m3);
  delete m3;
  while (Trivial::Sum* const s = e.next()) {
    s->print();
    delete s;
  }

}
