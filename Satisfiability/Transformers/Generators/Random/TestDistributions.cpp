// Oliver Kullmann, 18.4.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cmath>

#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"

namespace {

  const Environment::ProgramInfo pi{
        "0.2.17",
        "20.4.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/Distributions.cpp",
        "GPL v3"};

  using namespace RandGen;

  // For the uniform distribution in [0,2^50):
  constexpr gen_uint_t valempty_2p50_10001 = valempty_10001 / iexp2(64-50);
  static_assert(valempty_2p50_10001 == 792872142654181ULL);
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, pi, argc, argv))
  return 0;

  {bernoulliS b;
   assert(b());
   b.g.discard(9999);
   assert(not b());
   b.g.discard(9999);
   assert(not b());
   b.g.discard(9999);
   assert(b());
  }

  {randgen_t g; g.discard(10000);
   assert(bernoulli_high(g));
  }
  {RandGen_t g;
   assert(bernoulli_high(g));
   g.discard(9999);
   assert(bernoulli_high(g));
   g.discard(9999);
   assert(bernoulli_high(g));
   g.discard(9999);
   assert(not bernoulli_high(g));
  }

  {bernoulli_low b;
   assert(b == bernoulli_low());
   typedef std::bitset<64> bv_t;
   bv_t bv;
   for (unsigned i = 0; i < 64; ++i) bv[i] = b();
   assert(bv == valempty_1);
   b.discard(64*9999);
   for (unsigned i = 0; i < 64; ++i) bv[i] = b();
   assert(bv == valempty_10001);
   b.discard(64*9999);
   for (unsigned i = 0; i < 64; ++i) bv[i] = b();
   assert(bv == valempty_20001);
   assert(b != bernoulli_low());
  }

  {bernoulli_low b;
    // value of b.bv (starting with 0), and first and last access:
   // 0100010100011111011101010000010110011111110011010110100111010000
   // F          L
   b.discard(1);
   assert(b());
   b.discard(3);
   assert(b());
   assert(not b());
   b.discard(4);
   assert(b());
   b.discard(64*10000);
   // 1100100010100010100111010100100101011001110111001110001000101101
   //             F  L
   assert(not b());
   b.discard(2);
   assert(not b());
   b.discard(64*10000);
   // 0110011011100000110101000101000000111001000011101100000011110111
   //                 F           L
   assert(b());
   b.discard(5);
   assert(not b());
   assert(not b());
   b.discard(3);
   assert(b());
   assert(not b());
   b.discard(64*10000);
   // 1010000000000101101110100000110101101000111111111010000111001100
   //                              F     L
   assert(b()); // 30
   b.discard(5); // 35
   assert(not b()); // 36
  }

  {bernoulli_low b;
   b.discard(60);
   for (unsigned i = 0; i < 6; ++i) assert(not b());
   assert(b() and not b() and b() and not b() and b() and b());
   b.discard(51);
   assert(b() and not b() and b() and not b() and not b());
   b.discard(1);
   for (unsigned i = 0; i < 4; ++i) assert(not b());
   assert(b());
   b.discard(19);
   for (unsigned i = 0; i < 7; ++i) assert(not b());
   b.discard(23);
   for (unsigned i = 0; i < 7; ++i) assert(not b());
   assert(b());
   assert(not b());
   // 010001010001111101110101000001011001111111001101011010011101 0000
   // 00 101011 100110101100100111110111101100110001101010100010001 10100
   // 1 0000 11110100010000111101 0000000 11110110011000011000101 0000000 10
  }

  {RandGen_t g;
   Bernoulli b(g,{1,3});
   assert(b.last() == 0);
   assert(b.rejected() == 0);
   for (unsigned i = 0; i < 10001; ++i) b();
   assert(b.last() == valempty_10001);
   assert(b.rejected() == 0);
  }

  {RandGen_t g;
   Bernoulli b(g,{1,9223372036854775809ULL});
   for (int i = 0; i < 10000; ++i) b();
   assert(b.rejected() == 10157);
   RandGen_t g2;
   g2.discard(10000+b.rejected());
   assert(g == g2);
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
   assert(u() == valempty_2p50_10001);
  }
  {UniformRangeS u(iexp2(50), {});
   for (int i = 0; i < 10000; ++i) u();
   assert(u() == valempty_2p50_10001);
  }

  {RandGen_t g;
   randgen_t g1(g.extract());
   UniformRange u(g1, iexp2(50));
   g1.discard(10000);
   assert(u() == valempty_2p50_10001);
  }
  {RandGen_t g(transform("", EP::one));
   assert(g() == valempty_1);
   g.discard(9999);
   {randgen_t g1(g.extract());
    UniformRange u(g1, iexp2(50));
    assert(u() == valempty_2p50_10001);}
   {randgen_t g1(g.extract());
    UniformRange u(g1, iexp2(50));
    assert(u() == valempty_2p50_10001);}
  }

  {randgen_t g;
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
   }
  }

}
