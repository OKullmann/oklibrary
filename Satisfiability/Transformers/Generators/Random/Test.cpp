// Oliver Kullmann, 2.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <vector>

#include <cassert>
#include <cmath>

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

  {
   {UniformRange u(g,1);
    assert(u.n==1 and u.s==0 and u.trivial and u.p2 and u.size_region==0 and
      u.last_regions==randgen_max);
    for (unsigned int i = 0; i < 5; ++i) assert(u() == 0);}
   randgen_t g0(g);
   assert(g0 == g);
   randgen_t gsave;
   {UniformRange u(g,2);
    assert(u.n==2 and u.s==0 and not u.trivial and u.p2 and
      u.size_region==std::exp2(gen_uint_t(63)) and
      u.last_regions==randgen_max);
    assert(u() == 0);
    assert(u() == 1);
    gsave = g;
    assert(u() == 0);
    assert(u() == 1);
    assert(u() == 1);
   }
   {UniformRange u(g0,2,77);
    assert(u.n==2 and u.s==77 and not u.trivial and u.p2 and
      u.size_region==std::exp2(gen_uint_t(63)) and
      u.last_regions==randgen_max);
    assert(u() == 77);
    assert(u() == 78);
    assert(u() == 77);
    assert(u() == 78);
    assert(u() == 78);
   }
   g = gsave;
  }

  {randgen_t g1, g2, g3;
   UniformRange u(g1,2);
   const Bernoulli b(g2,1,1);
   assert(b.s == Bernoulli::S::nc);
   assert(b.threshold == iexp2(63));
   for (unsigned i = 0; i < 10'000'000; ++i) {
     const bool v = u();
     assert(v == not b());
     assert(v == not bernoulli(g3));
   }
  }

  {randgen_t g1, g2;
   for (gen_uint_t e = 0; e <= 5; ++e) {
     const gen_uint_t max = iexp2(e);
     UniformRange u(g1, max);
     assert(u.p2);
     for (gen_uint_t x = 0; x <= max; ++x) {
       Bernoulli b(g2, x, e);
       assert(u.trivial or b.threshold == x * u.size_region);
       for (unsigned i = 0; i < 100'000; ++i) {
         assert(b() == (u() < x));
         if (not u.trivial and b.s != Bernoulli::S::nc) g2.discard(1);
       }
     }
   }
  }

  const std::vector v0{1,2,3,4,5,6,7};
  std::vector v(v0);
  randgen_t gcopy(g);
  shuffle(v.begin(), v.end(), randgen_t(g)); // second form
  const std::vector vs{5,2,1,7,6,4,3};
  assert(v == vs);
  assert(g == gcopy);
  v = v0;
  shuffle(v.begin(), v.end(), g); // first form
  assert(v == vs);
  assert(g != gcopy);
  v = v0;
  shuffle(v.begin(), v.end(), gcopy); // first form
  assert(v == vs);
  assert(g == gcopy);
  shuffle(v.begin(), v.end(), g); // first form
  const std::vector vs2{6,4,7,5,2,1,3};
  assert(v == vs2);
  assert(g != gcopy);
  gcopy.discard(6);
  assert(g == gcopy); // Remark: not guaranteed in general, due to possible discarded calls of g() in UniformRange.
  shuffle(v.begin(), v.end(), std::move(g)); // second form
  const std::vector vs3{3,6,4,5,1,2,7};
  assert(v == vs3);
  assert(g != gcopy);
  v = vs2;
  shuffle(v.begin(), v.end(), std::move(gcopy)); // second form
  assert(v == vs3);
  g.seed(); gcopy.seed();
  assert(g == gcopy);
  assert(g == randgen_t(5489));
}
