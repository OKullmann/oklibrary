// Oliver Kullmann, 2.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <vector>
#include <utility>
#include <iostream>

#include <cassert>
#include <cmath>

#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo pi{
        "0.2.0",
        "19.3.2019",
        __FILE__};

  using namespace RandGen;

  // The numerical values as specified by the C++ standard:
  constexpr gen_uint_t specval = 9981545732273789042ULL;
  constexpr seed_t specseed = 5489u;

  // The ith generated values using the empty seed-sequence:
  constexpr gen_uint_t valempty_1 = 835052665647855778ULL;
  constexpr gen_uint_t valempty_10000 = 12990417185246102803ULL;
  // For the uniform distribution in [0,2^50):
  constexpr gen_uint_t valempty_2p50_10000 = valempty_10000 / iexp2(64-50);
  static_assert(valempty_2p50_10000 == 792872142654181ULL);
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, pi, argc, argv))
  return 0;

  {assert(is_seed_t({}));
   assert(is_seed_t({0}));
   assert(is_seed_t({iexp2(32)-1}));
   assert(not is_seed_t({iexp2(32)}));
   assert(not is_seed_t({randgen_max}));
   assert((is_seed_t({0,0})));
   assert((is_seed_t({iexp2(32)-1,iexp2(32)-1})));
   assert((not is_seed_t({iexp2(32),iexp2(32)-1})));
   assert((not is_seed_t({iexp2(32)-1,iexp2(32)})));
  }

  {assert(transform({}) == vec_seed_t{});
   assert((transform({0}) == vec_seed_t{0,0}));
   assert((transform({1}) == vec_seed_t{1,0}));
   assert((transform({iexp2(32)}) == vec_seed_t{0,1}));
   assert((transform({gen_uint_t(-1)}) == vec_seed_t{seed_t(-1),seed_t(-1)}));
   assert((transform({0,0}) == vec_seed_t{0,0,0,0}));
   assert((transform({1,1}) == vec_seed_t{1,0,1,0}));
   assert((transform(vec_eseed_t(2,iexp2(32))) == vec_seed_t{0,1,0,1}));

   assert(transform({},SP::trunc) == vec_seed_t{});
   assert((transform({0},SP::trunc) == vec_seed_t{0}));
   assert((transform({1},SP::trunc) == vec_seed_t{1}));
   assert((transform({iexp2(32)},SP::trunc) == vec_seed_t{0}));
   assert((transform({gen_uint_t(-1)},SP::trunc) == vec_seed_t{seed_t(-1)}));
   assert((transform({0,0},SP::trunc) == vec_seed_t{0,0}));
   assert((transform({1,1},SP::trunc) == vec_seed_t{1,1}));
   assert((transform(vec_eseed_t(2,iexp2(32)),SP::trunc) == vec_seed_t{0,0}));

   assert(transform({},SP::check) == vec_seed_t{});
   assert((transform({0},SP::check) == vec_seed_t{0}));
   assert((transform({1},SP::check) == vec_seed_t{1}));
   assert((transform({iexp2(32)},SP::check) == vec_seed_t{0,1}));
   assert(transform({gen_uint_t(-1)},SP::check) == vec_seed_t(2,seed_t(-1)));
   assert((transform({0,0},SP::check) == vec_seed_t{0,0}));
   assert((transform({1,1},SP::check) == vec_seed_t{1,1}));
   assert((transform(vec_eseed_t(2,iexp2(32)),SP::check) == vec_seed_t{0,1,0,1}));
   assert((transform(vec_eseed_t{iexp2(32),1},SP::check) == vec_seed_t{0,1,1,0}));
   assert((transform(vec_eseed_t{1,iexp2(32),1},SP::check) == vec_seed_t{1,0,0,1,1,0}));
  }

  {assert(valid_ascii(""));
   assert((valid_ascii({0,127})));
   assert((not valid_ascii({char(-1)})));
  }

  {assert(transform({}, EP::four) == vec_seed_t{});
   assert(transform({0}, EP::four) == vec_seed_t{0});
   assert(transform({0}, EP::one) == vec_seed_t{0});
   assert((transform({0,0}, EP::four) == vec_seed_t{0}));
   assert((transform({0,0}, EP::one) == vec_seed_t{0,0}));
   assert((transform({0,0,1,127}, EP::one) == vec_seed_t{0,0,1,127}));
   assert((transform({0,0,1,127}, EP::four) == vec_seed_t{0x7F'01'00'00}));
   assert((transform({1}, EP::four) == vec_seed_t{1}));
   assert((transform({1,2}, EP::four) == vec_seed_t{0x2'01}));
   assert((transform({1,2,3}, EP::four) == vec_seed_t{0x3'02'01}));
   assert((transform({1,2,3,4}, EP::four) == vec_seed_t{0x4'03'02'01}));
   assert((transform({1,2,3,4,5}, EP::four) == vec_seed_t{0x4'03'02'01, 5}));
   assert((transform({1,2,3,4,5}, EP::one) == vec_seed_t{1,2,3,4,5}));
   assert((transform("12345", EP::one) == vec_seed_t{49,50,51,52,53}));
   assert((transform("12345", EP::four) == vec_seed_t{0x34'33'32'31, 0x35}));
   assert((transform("xyz{|}~^", EP::four) == vec_seed_t{0x7b'7a'79'78, 0x5e'7e'7d'7c}));
   assert((transform("xyz{|}~^A", EP::four) == vec_seed_t{0x7b'7a'79'78, 0x5e'7e'7d'7c, 0x41}));
  }

  {RandGen_t g0;
   assert(g0 == RandGen_t());
   {RandGen_t g1(g0), g2({0});
    assert(g1 == g0);
    assert(g2 != g1);
    g2 = g1;
    assert(g2 == g1);
   }
   const randgen_t g1 = g0.extract();
   constexpr int basis = 18; // 32, the whole seed_t-range, indeed must work
   for (gen_uint_t i = 0; i < iexp2(basis); ++i) {
     const randgen_t g(i);
     assert(g != g1);
   }
  }

  {RandGen_t g;
   assert(g() == valempty_1);
   g.discard(9999);
   assert(g() == valempty_10000);
  }

  {RandGen_t g1({1,2});
   randgen_t g2(init({1,2}));
   assert(g1.extract() == g2);
  }

  {RandGen_t g1({1,2,3});
   randgen_t g2(g1.extract());
   for (gen_uint_t e = 0; e < 10; ++e) {
     const gen_uint_t pow = iexp2(e);
     for (gen_uint_t x = 0; x <= pow; ++x) {
       Bernoulli b1(g1, {x,pow});
       Bernoulli2 b2(g2, x, e);
       assert(b1.threshold == b2.threshold);
       for (unsigned int i = 0; i < 1000; ++i)
         assert(b1() == b2());
     }
   }
   assert(g1.extract() == g2);
  }

  {randgen_t g1, g2, g3;
   UniformRange u(g1,2);
   const Bernoulli2 b(g2,1,1);
   assert(b.s == Bernoulli2::S::nc);
   assert(b.threshold == iexp2(63));
   for (unsigned i = 0; i < 10'000'000; ++i) {
     const bool v = u();
     assert(v == not b());
     assert(v == not bernoulli(g3));
   }
  }

  {randgen_t g;
   UniformRange u1(g,1007);
   assert(randgen_max - u1.last_regions == 492);
   assert(not u1.p2);
   UniformRange u2(g,10000000019ULL);
   assert(randgen_max - u2.last_regions == 8660737959ULL);
   assert(not u2.p2);
   UniformRange u3(g,iexp2(38));
   assert(u3.last_regions == randgen_max);
   assert(u3.p2);
   assert(u3.size_region == iexp2(64-38));
  }

  {randgen_t g1, g2;
   for (gen_uint_t e = 0; e <= 5; ++e) {
     const gen_uint_t max = iexp2(e);
     UniformRange u(g1, max);
     assert(u.p2);
     for (gen_uint_t x = 0; x <= max; ++x) {
       Bernoulli2 b(g2, x, e);
       assert(u.trivial or b.threshold == x * u.size_region);
       for (unsigned i = 0; i < 100'000; ++i) {
         assert(b() == (u() < x));
         if (not u.trivial and b.s != Bernoulli2::S::nc) g2.discard(1);
       }
     }
   }
  }

  {RandGen_t g1({0,2,4});
   randgen_t g2(g1.extract());
   assert(g1.extract() == g2);
   for (gen_uint_t den = 1; den <= 20; ++den) {
     UniformRange u(g2, den);
     assert(den != 1 or u.trivial);
     for (gen_uint_t nom = 0; nom <= den; ++nom) {
       Bernoulli b(g1, {nom,den});
       assert(nom != 0 or b.s == Bernoulli::S::c0);
       assert(nom != den or b.s == Bernoulli::S::c1);
       assert(nom != 1 or Bernoulli::constant(b.s) or (u.p2 == (b.s == Bernoulli::S::dy)));
       assert(nom != 1 or Bernoulli::constant(b.s) or u.last_regions == b.last_valid);
       assert(nom != 1 or Bernoulli::constant(b.s) or b.threshold == nom * u.size_region);
       for (unsigned i = 0; i < 100'000; ++i) {
         assert(b() == (u() < nom));
         if (not u.trivial and (Bernoulli::constant(b.s))) g1.discard(1);
       }
     }
     assert(g1.extract() == g2); // happens to be the case here that there are no failed generator calls
   }
  }

  {std::seed_seq s{};
   randgen_t g(s);
   assert(g() == valempty_1);
   UniformRange u(g, iexp2(50));
   g.discard(9999);
   assert(u() == valempty_2p50_10000);
  }

  {RandGen_t g;
   randgen_t g1(g.extract());
   UniformRange u(g1, iexp2(50));
   g1.discard(10000);
   assert(u() == valempty_2p50_10000);
  }
  {RandGen_t g(transform("", EP::one));
   assert(g() == valempty_1);
   g.discard(9999);
   {randgen_t g1(g.extract());
    UniformRange u(g1, iexp2(50));
    assert(u() == valempty_2p50_10000);}
   {randgen_t g1(g.extract());
    UniformRange u(g1, iexp2(50));
    assert(u() == valempty_2p50_10000);}
  }

  randgen_t g;

  // According to
  // https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine :
  g.discard(9999);
  assert(g() == specval);

  assert(not bernoulli(g));
  assert(not bernoulli(g));
  assert(bernoulli(g));

  {randgen_t g0(g);
   {UniformRange u(g,1);
    assert(u.n==1 and u.s==0 and u.trivial and u.p2 and u.size_region==0 and
      u.last_regions==randgen_max);
    for (unsigned int i = 0; i < 5; ++i) assert(u() == 0);}
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
  assert(g == randgen_t(specseed));

}
