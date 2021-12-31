// Oliver Kullmann, 16.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "FPDistributions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.3",
        "31.12.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/FPDistributions.cpp",
        "GPL v3"};

  using namespace RandGen;
  namespace FP = FloatingPoint;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {RandGen_t g;
   {Uniform80Range U(g);
    assert(U() == 0.045268295711760839673L);
    assert(U() == 0.17293314960222741267L);
    assert(U() == 0.25124397478200063803L);
    assert(U() == 0.33163953516761894596L);
    assert(U() == 0.76861149576035981334L);
    assert(U() == 0.91953646838336549246L);
    assert(U() == 0.52517478649793348074L);
   }
   {Uniform80Range U2(g,-10,10);
    assert(U2() == 8.5607938361487779565L);
    assert(U2() == 0.77021488482888793444L);
    assert(U2() == 6.8623090125500191792L);
    assert(U2() == 5.6866645687915447953L);
    assert(U2() == 4.0761928110532688499L);
    assert(U2() == 1.6594030707104697267L);
    assert(U2() == -9.6670287649430283131L);
   }

   {const FP::float80 e = FP::epsilon;
    Uniform80Range U3(g,1,1+e);
    assert(U3.d == e);
    for (unsigned i = 0; i < 1000; ++i) {
      const FP::float80 r = U3();
      assert(r == 1);
    }
   }
   {const FP::float80 e1 = FP::nextafter(1,2);
    const FP::float80 e2 = FP::nextafter(e1,2);
    std::vector<FP::UInt_t> count(2);
    Uniform80Range U3(g,1,e2);
    for (unsigned i = 0; i < 1000; ++i) {
      const FP::float80 r = U3();
      assert((r == 1) or (r == e1));
      ++count[r != 1];
    }
    assert(count[0] == 490);
    assert(count[1] == 510);
   }
   {const FP::float80 e1 = FP::nextafter(1,2);
    const FP::float80 e2 = FP::nextafter(e1,2);
    const FP::float80 e3 = FP::nextafter(e2,2);
    std::vector<FP::UInt_t> count(3);
    Uniform80Range U3(g,1,e3);
    for (unsigned i = 0; i < 100000; ++i) {
      const FP::float80 r = U3();
      assert((r == 1) or (r == e1) or (r == e2));
      if (r == 1) ++count[0];
      else if (r == e1) ++count[1];
      else ++count[2];
    }
    assert(count[0] == 33351);
    assert(count[1] == 33331);
    assert(count[2] == 33318);
   }

  }

  {RandGen_t g;
   {Uniform80RangeI U(g);
    assert(U() == 0.045268295711760839676L);
    assert(U() == 0.17293314960222741268L);
    assert(U() == 0.25124397478200063806L);
    assert(U() == 0.33163953516761894599L);
    assert(U() == 0.7686114957603598134L);
    assert(U() == 0.91953646838336549252L);
    assert(U() == 0.52517478649793348079L);
   }
   {Uniform80RangeI U2(g,-10,10);
    assert(U2() == 8.5607938361487779565L);
    assert(U2() == 0.77021488482888793444L);
    assert(U2() == 6.8623090125500191792L);
    assert(U2() == 5.6866645687915447953L);
    assert(U2() == 4.0761928110532688499L);
    assert(U2() == 1.6594030707104697267L);
    assert(U2() == -9.6670287649430283131L);
   }

   {const FP::float80 e = FP::epsilon;
    Uniform80RangeI U3(g,1,1+e);
    std::vector<FP::UInt_t> count(2);
    for (unsigned i = 0; i < 10000; ++i) {
      const FP::float80 r = U3();
      assert((r == 1) or (r == 1+e));
      ++count[r != 1];
    }
    assert(count[0] == 4991);
    assert(count[1] == 5009);
   }
   {const FP::float80 e1 = FP::nextafter(1,2);
    const FP::float80 e2 = FP::nextafter(e1,2);
    Uniform80RangeI U3(g,1,e2);
    std::vector<FP::UInt_t> count(3);
    for (unsigned i = 0; i < 10000; ++i) {
      const FP::float80 r = U3();
      assert((r == 1) or (r == e1) or (r == e2));
      if (r == 1) ++count[0];
      else if (r == e1) ++count[1];
      else ++count[2];
    }
    assert(count[0] == 3294);
    assert(count[1] == 3377);
    assert(count[2] == 3329);
   }

  }
}
