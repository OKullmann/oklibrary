// Oleg Zaikin, 7.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* A program for using the Trivial Gecode class.
*/

#include "trivial.hpp"

int main() {
  Trivial* m = new Trivial(3, 0, 2);
  m->print();
  delete m;
}
