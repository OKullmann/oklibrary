// Oliver Kullmann, 6.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Ode.hpp"

/*
TODOS:

*/


namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "7.2.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/DifferenetialEquations/Test.cpp",
        "GPL v3"};

  using namespace FloatingPoint;
  using namespace Ode;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {Euler1d<float80> E(0,0,[](float80, float80){return 0;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.y() == 0);
  }
  {Euler1d<float80> E(0,0,[](float80, float80){return 1;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 1);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == -1);
   E.steps(2);
   assert(E.x() == 1);
   assert(FP::accuracy(1,E.y()) <= 0.3e4);
  }
  {Euler1d<float80> E(0,0,[](float80, float80){return 2;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 2);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == -2);
   E.steps(2);
   assert(E.x() == 1);
   assert(FP::accuracy(2,E.y()) <= 0.3e4);
  }
  {Euler1d<float80> E(0,0,[](float80 x, float80){return x;});
   const auto f = [](float80 x){return x*x/2;};
   E.steps(1);
   assert(E.x() == 1);
   assert(FP::accuracy(f(1),E.y()) <= 0.2e15);
   E.steps(1);
   assert(E.x() == 2);
   assert(FP::accuracy(f(2),E.y()) <= 1e14);
  }
  {Euler1d<float80> E(0,1,[](float80, float80 y){return y;});
   const auto f = [](float80 x){return FP::exp(x);};
   E.steps(1);
   assert(E.x() == 1);
   assert(FP::accuracy(f(1),E.y()) <= 0.7e14);
   E.steps(1);
   assert(E.x() == 2);
   assert(FP::accuracy(f(2),E.y()) <= 0.2e15);
   E.steps(-3);
   assert(E.x() == -1);
   assert(FP::accuracy(f(-1),E.y()) <= 0.8e15);
  }
  {Euler1d<float80> E(0,1,[](float80, float80 y){return y;});
   const auto f = [](float80 x){return FP::exp(x);};
   E.steps(1,1e6);
   assert(E.x() == 1);
   assert(FP::accuracy(f(1),E.y()) <= 7e12);
  }
}
