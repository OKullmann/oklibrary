// Oliver Kullmann, 18.4.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <vector>
#include <utility>

#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo pi{
        "0.2.11",
        "19.4.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/Algorithms.cpp",
        "GPL v3"};

  using namespace RandGen;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, pi, argc, argv))
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
  }
  {RandGen_t g({0});
   assert((choose_kn(5,10,g) == vec_eseed_t{3,1,6,9,8}));
  }
  {RandGen_t g({0});
   assert((choose_kn(6,10,g) == vec_eseed_t{0,2,4,5,7,8}));
  }

}
