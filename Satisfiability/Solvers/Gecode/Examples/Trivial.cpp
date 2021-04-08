// Oleg Zaikin, 7.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A program for using the Trivial class (derived from Gecode::Space).

  TODOS:

  1. DONE A concept of ownership is needed.
    - Naked applications of "delete" must be avoided.

*/

#include <cassert>

#include "Trivial.hpp"

int main() {
  Trivial::IntArr m(3, 0, 2);
  Trivial::IntArr m2(m);
  assert(m.size() == 3);
  assert(m2.size() == 3);
  m.print();
  m2.print();
}
