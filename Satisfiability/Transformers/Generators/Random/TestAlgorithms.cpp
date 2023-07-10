// Oliver Kullmann, 18.4.2019 (Swansea)
/* Copyright 2019, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.1",
        "10.7.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/Algorithms.cpp",
        "GPL v3"};

  using namespace RandGen;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {randgen_t g;
   g.discard(9999 + 1 + 3 + 2);

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

  {typedef std::vector<unsigned> v_t;
   assert(random_permutation<v_t>(0, {}) == v_t{});
   assert(random_permutation<v_t>(1, {}) == v_t{0});
   assert((random_permutation<v_t>(10, {777}) == v_t{0,4,7,5,3,1,6,2,9,8}));
  }

  {RandGen_t g;
   const RandGen_t copy(g);
   assert((choose_kn(0,0,g) == vec_eseed_t{}));
   assert((choose_kn(0,1,g) == vec_eseed_t{}));
   assert((choose_kn(1,0,g) == vec_eseed_t{}));
   assert((choose_kn(5,4,g) == vec_eseed_t{}));
   assert((choose_kn(1,1,g) == vec_eseed_t{0}));
   assert((choose_kn(5,5,g) == vec_eseed_t{0,1,2,3,4}));
   assert(g == copy);
  }

  {RandGen_t g({0});
   // Sequence of UniformRange<RandGen_t> for n=10,...,1: 3,1,6,3,1,1,2,1,0,0
   assert((choose_kn_inclusion(10,10,g) == vec_eseed_t{3,1,6,9,8,5,2,4,0,7}));
   typedef std::vector<gen_uint_t> v_t;
   {v_t count(2);
    for (gen_uint_t i = 0; i < 1000; ++i) {
      const auto perm = choose_kn_inclusion(2,2,g);
      if (perm == v_t{0,1}) ++ count[0]; else ++count[1];
    }
    assert((count == v_t{503,497}));
   }
   {v_t count(20); gen_uint_t less = 0;
    for (gen_uint_t i = 0; i < 10000; ++i) {
      const auto ch = choose_kn_inclusion(2,20,g);
      assert(ch.size() == 2);
      assert(ch[0] != ch[1]);
      less += ch[0] < ch[1];
      ++count[ch[0]]; ++count[ch[1]];
    }
    assert(less == 5010);
    assert((count == v_t{1030,1006,996,963,955,1036,1012,986,1047,968,969,1041,994,1001,1008,1020,988,1026,939,1015}));
   }
   {v_t count(19);
    for (gen_uint_t i = 0; i < 10000; ++i) {
      const auto ch = choose_kn_inclusion(1,19,g);
      assert(ch.size() == 1);
      ++count[ch[0]];
    }
    assert((count == v_t{560,531,509,507,570,528,523,544,516,519,492,531,563,534,529,504,499,532,509}));
   }
   {const auto copy(g);
    {const auto result = choose_kn_inclusion(1,1,g);
     assert(result.size() == 1);
     assert(result == v_t{0});
     assert(g == copy);
    }
   }
  }
  {RandGen_t g({0});
   assert((choose_kn(10,10,g) == vec_eseed_t{0,1,2,3,4,5,6,7,8,9}));
   choose_kn_inclusion(10,10,g);
   for (gen_uint_t i = 0; i < 1000; ++i) choose_kn_inclusion(2,2,g);
   typedef std::vector<gen_uint_t> v_t;
   {v_t count(20); gen_uint_t less = 0;
    for (gen_uint_t i = 0; i < 10000; ++i) {
      const auto ch = choose_kn(2,20,g);
      assert(ch.size() == 2);
      assert(ch[0] != ch[1]);
      less += ch[0] < ch[1];
      ++count[ch[0]]; ++count[ch[1]];
    }
    assert(less == 5010);
    assert((count == v_t{1030,1006,996,963,955,1036,1012,986,1047,968,969,1041,994,1001,1008,1020,988,1026,939,1015}));
   }
   {v_t count(19);
    for (gen_uint_t i = 0; i < 10000; ++i) {
      const auto ch = choose_kn(1,19,g);
      assert(ch.size() == 1);
      ++count[ch[0]];
    }
    assert((count == v_t{560,531,509,507,570,528,523,544,516,519,492,531,563,534,529,504,499,532,509}));
   }
   {const auto copy(g);
    {const auto result = choose_kn(1,1,g);
     assert(result.size() == 1);
     assert(result == v_t{0});
     assert(g == copy);
    }
   }
  }
  {RandGen_t g({0});
   assert((choose_kn(5,10,g) == vec_eseed_t{3,1,6,9,8}));
  }
  {RandGen_t g({0});
   assert((choose_kn(5,10,g, true) == vec_eseed_t{1,3,6,8,9}));
  }
  {RandGen_t g({0});
   assert((choose_kn(6,10,g) == vec_eseed_t{0,2,4,5,7,8}));
  }
  {RandGen_t g({0});
   assert((choose_kn(6,10,g, true) == vec_eseed_t{0,2,4,5,7,8}));
  }

  {RandGen_t g1, g2;
   for (gen_uint_t N = 2; N <= 100; ++N) {
     UniformRange U(g1, N);
     for (gen_uint_t i = 0; i < 1000; ++i)
       assert(choose_kn(1, N, g2).front() == U());
   }
  }

}
