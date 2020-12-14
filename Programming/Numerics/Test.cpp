// Oliver Kullmann, 3.3.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <type_traits>
#include <string_view>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "FloatingPoint.hpp"
#include "Tau.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "14.12.2020",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/Test.cpp",
        "GPL v3"};

  using namespace FloatingPoint;
  using namespace Tau;

  // wtau-values:
  constexpr float80 wtau_1e10 = 20.0286854142587627562898888952734927L;
  constexpr float80 wtau_1e20 = 42.306755091738393851977808536069859687L;
  constexpr float80 wtau_1e100 = 224.84310644511850153937313433795567541L;
  constexpr float80 wtau_1e1000 = 2294.846671683506869652792785993616789973L;
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(isnan(stold("NaN")));
   assert(isnan(0.0L / 0.0L));
   assert(isnan(+0.0L / 0.0L));
   assert(1 / +0.0L == pinfinity);
   assert(1 / -0.0L == minfinity);
   assert(1 / 0.0L == pinfinity);
  }

  {assert(FP::nextafter(0, 1) == denorm_min_value);
   assert(FP::nextafter(denorm_min_value, 0) == 0);
   assert(FP::nextafter(max_value, pinfinity) == pinfinity);
   assert(FP::nextafter(pinfinity,0) == max_value);
   assert(FP::nextafter(1,2) == 1 + epsilon);
   assert(FP::nextafter(1+epsilon,0) == 1);
   assert(FP::nextafter(0, -1) == -denorm_min_value);
   assert(FP::nextafter(-denorm_min_value, 0) == 0);
   assert(FP::nextafter(-max_value, minfinity) == minfinity);
   assert(FP::nextafter(minfinity,0) == -max_value);
   assert(FP::nextafter(-1,-2) == -1 - epsilon);
   assert(FP::nextafter(-1-epsilon,0) == -1);
   assert(isnan(FP::nextafter(NaN, NaN)));
   assert(isnan(FP::nextafter(NaN,0)));
   assert(isnan(FP::nextafter(0,NaN)));
  }

  {assert(accuracy(0, denorm_min_value) == 1);
   assert(accuracy(0, -denorm_min_value) == 1);
   assert(accuracy(0,min_value) > 1e18);
   assert(accuracy(0,-min_value) > 1e18);
   assert(accuracy(1,1+epsilon) == 1);
   assert(accuracy(-1,-1-epsilon) == 1);
   assert(accuracy(1,1+10*epsilon) == 10);
   assert(accuracy(-1,-1-10*epsilon) == 10);
   assert(accuracy(1e1000L, FP::nextafter(FP::nextafter(1e1000L,pinfinity),pinfinity)) == 2);
   assert(accuracy(-1e1000L, FP::nextafter(FP::nextafter(-1e1000L,minfinity),minfinity)) == 2);
  }

  {assert(accuracy(euler-1, eulerm1) <= 1);
   assert(accuracy(2, sq(Sqr2)) <= 1);
   assert(accuracy(Log2/2, FP::log(FP::sqrt(2))) <= 1);
   assert(accuracy(lSfactorial(10), log(Sfactorial(10))) <= 1);
   assert(accuracy(1.00891344545564193334812497256e29L, fbinomial_coeff(100,50)) <= 2);
   assert(accuracy(lbinomial_coeff(60,30), FP::log(binomial_coeff(60,30))) <= 5);
   assert(accuracy(lbinomial_coeff(80,21), FP::log(binomial_coeff(80,21))) <= 2);
   assert(accuracy(lbinomial_coeff(70,27), FP::log(binomial_coeff(70,27))) <= 4);
   assert(accuracy(lbinomial_coeff(100,50), log(fbinomial_coeff(100,50))) <= 2);
   assert(accuracy(euler*euler/(1+euler), lambertW0l_lb(euler)) <= 1);

  }

  {bool thrown = false;
   try { to_float80("x"); }
   catch(const std::invalid_argument& e) {
     assert(e.what() == std::string_view("FloatingPoint::to_float80(string), failed"
        " for \"x\""));
     thrown = true;
   }
   assert(thrown);
  }
  {bool thrown = false;
   try { to_float80("1e5000"); }
   catch(const std::out_of_range& e) {
     assert(e.what() == std::string_view("FloatingPoint::to_float80(string),"
       " \"1e5000\""));
     thrown = true;
   }
   assert(thrown);
  }
  {bool thrown = false;
   try { to_float80("0x"); }
   catch(const std::domain_error& e) {
     assert(e.what() == std::string_view("FloatingPoint::to_float80(string), trailing:"
        " \"x\" in \"0x\""));
     thrown = true;
   }
   assert(thrown);
  }

  {assert(toUInt("1e18") == pow(10.0L,18.0L));
   assert(toUInt("-1e100") == 0);
   assert(toUInt("1.8446744073709551614e19") == P264m1 - 1);
   assert(toUInt("NaN") == 0);
   assert(toUInt("inf") == P264m1);
   assert(toUInt("-inf") == 0);
  }
  {assert(touint("1e9") == pow(10.0L,9.0L));
   assert(touint("-1e100") == 0);
   assert(touint("4.294967294e9") == P232m1 - 1);
   assert(touint("NaN") == 0);
   assert(touint("inf") == P232m1);
   assert(touint("-inf") == 0);
   static_assert(std::is_same_v<decltype(touint("")), uint_t>);
  }

}
