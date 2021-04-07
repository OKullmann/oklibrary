// Oleg Zaikin, 7.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A program for using the Trivial class (derived from Gecode::Space).

  TODOS:

  1. A concept of ownership is needed.
    - Naked applications of "delete" must be avoided.

*/

#include <cassert>

#include "Trivial.hpp"

int main() {
  const Trivial* const m = new Trivial(3, 0, 2);
  m->print();
  assert(m->size() == 3);
  delete m;
}
