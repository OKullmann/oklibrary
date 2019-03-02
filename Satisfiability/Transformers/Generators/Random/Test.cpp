// Oliver Kullmann, 2.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <vector>

#include <cassert>

#include "Numbers.hpp"

int main() {
  using namespace RandGen;

  randgen_t g;

  // According to
  // https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine :
  g.discard(9999);
  assert(g() == 9981545732273789042ULL);

  assert(not bernoulli(g));
  assert(not bernoulli(g));
  assert(bernoulli(g));

  {Uniform u(g,1);
   for (unsigned int i = 0; i < 5; ++i) assert(u() == 1);}
  Uniform u(g,2);
  assert(u() == 2);
  assert(u() == 1);

  const std::vector v0{1,2,3,4,5,6,7};
  std::vector v(v0);
  randgen_t gcopy(g);
  shuffle(v.begin(), v.end(), randgen_t(g)); // first form
  const std::vector vs{4,5,1,2,3,6,7};
  assert(v == vs);
  assert(g == gcopy);
  v = v0;
  shuffle(v.begin(), v.end(), g); // second form
  assert(v == vs);
  assert(g != gcopy);
  v = v0;
  shuffle(v.begin(), v.end(), gcopy); // second form
  assert(v == vs);
  assert(g == gcopy);
  shuffle(v.begin(), v.end(), g); // second form
  const std::vector vs2{7,5,6,3,2,1,4};
  assert(v == vs2);
  assert(g != gcopy);
  gcopy.discard(6);
  assert(g == gcopy); // Remark: not guaranteed in general, due to possible discarded calls of g() in Uniform.
  shuffle(v.begin(), v.end(), std::move(g)); // first form
  const std::vector vs3{6,2,3,7,1,4,5};
  assert(v == vs3);
  assert(g != gcopy);
  v = vs2;
  shuffle(v.begin(), v.end(), std::move(gcopy)); // first form
  assert(v == vs3);
  g.seed(); gcopy.seed();
  assert(g == gcopy);
  assert(g == randgen_t(5489));
}
