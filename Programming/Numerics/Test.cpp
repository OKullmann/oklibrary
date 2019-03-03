// Oliver Kullmann, 3.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <cassert>

#include "FloatingPoint.hpp"

int main() {
  using namespace FloatingPoint;

  assert(isnan(stold("NaN")));

  assert(toUInt("1e18") == pow(10.0L,18.0L));
  assert(toUInt("-1e100") == 0);
  assert(toUInt("1.8446744073709551614e19") == P264m1 - 1);
  assert(toUInt("NaN") == 0);
  assert(toUInt("inf") == P264m1);
  assert(toUInt("-inf") == 0);

}
