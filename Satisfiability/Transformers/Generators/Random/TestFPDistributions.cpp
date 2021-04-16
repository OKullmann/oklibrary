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
        "0.0.2",
        "16.4.2021",
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
   Uniform80Range U(g);
   assert(U() == 0.045268295711760839673L);
   assert(U() == 0.17293314960222741267L);
   assert(U() == 0.25124397478200063803L);
   assert(U() == 0.33163953516761894596L);
   assert(U() == 0.76861149576035981334L);
   assert(U() == 0.91953646838336549246L);
   assert(U() == 0.52517478649793348074L);

   Uniform80Range U2(g,-10,10);
   assert(U2() == 8.5607938361487779565L);
   assert(U2() == 0.77021488482888793444L);
   assert(U2() == 6.8623090125500191792L);
   assert(U2() == 5.6866645687915447953L);
   assert(U2() == 4.0761928110532688499L);
   assert(U2() == 1.6594030707104697267L);
   assert(U2() == -9.6670287649430283131L);
  }
}
