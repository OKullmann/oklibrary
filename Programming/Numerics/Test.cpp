// Oliver Kullmann, 3.3.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <type_traits>
#include <string_view>
#include <string>
#include <sstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "FloatingPoint.hpp"
#include "Tau.hpp"
#include "Tau_mpfr.hpp"

/*
TODOS:

*/


namespace {

  const Environment::ProgramInfo proginfo{
        "0.6.0",
        "25.12.2020",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/Test.cpp",
        "GPL v3"};

  using namespace FloatingPoint;
  using namespace Tau;
  using namespace Tau_mpfr;

  // wtau-values:
  constexpr float80 wtau_3 = 1.14673525752010692398807549755L;
  constexpr float80 wtau_1e1 = 1.80228896667058602861853333625575652823878884333896L;
  constexpr float80 wtau_1e2 = 3.3987174914955975436252852989942230342502652566959L;
  constexpr float80 wtau_1e3 = 5.25180769366060098878243543492707526968999657498425224L;
  constexpr float80 wtau_1e4 = 7.2321637000221608257527465621653540281705636296L;
  constexpr float80 wtau_1e10 = 20.0286854142587627562898888952734927L;
  constexpr float80 wtau_1e20 = 42.306755091738393851977808536069859687L;
  constexpr float80 wtau_1e100 = 224.84310644511850153937313433795567541L;
  constexpr float80 wtau_1e200 = 454.398045033714016125652064599033334L;
  constexpr float80 wtau_1e1000 = 2294.846671683506869652792785993616789973L;
  constexpr float80 wtau_max = 11347.18668117145943454728636602558601464420171172L;

  constexpr float64 wtau_max_64 = 703.2270331047701868756785650981770616;
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;
  {assert(isnan(stold("NaN")));
   assert(stold("inf") == pinfinity);
   assert(stold("-inf") == minfinity);
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

  {assert(accuracy(FP::log(4) / (1+1/tau_meaneqLW), FP::lambertW0_lb(tau_meaneqLW)) == 1);

   assert(accuracy(wtau_max, wtau_ge1(max_value)) == 0);
   assert(accuracy(wtau_max, wtau_ge1_ub(max_value)) == 0);
   assert(accuracy(wtau_3, wtau_ge1_ub(3)) == 1);

   assert(accuracy(wtau_3, wtau(3)) == 1);
   assert(accuracy(wtau_1e1, wtau(1e1)) == 0);
   assert(accuracy(wtau_1e2, wtau(1e2)) <= 1);
   assert(accuracy(wtau_1e3, wtau(1e3)) == 0);
   assert(accuracy(wtau_1e4, wtau(1e4)) == 0);
   assert(accuracy(wtau_1e10, wtau(1e10)) == 0);
   assert(accuracy(wtau_1e20, wtau(1e20)) == 0);
   assert(accuracy(wtau_1e100, wtau(1e100)) <= 1);
   assert(accuracy(wtau_1e200, wtau(1e200)) <= 1);
   assert(accuracy(wtau_1e1000, wtau(1e1000L)) == 0);
   assert(accuracy(wtau_max, wtau(max_value)) == 0);

   STATIC_ASSERT((wtau_c(3) == WithCounting{wtau(3), 4}));
   STATIC_ASSERT((wtau_c(1e1) == WithCounting{wtau(1e1), 4}));
   STATIC_ASSERT((wtau_c(1e2) == WithCounting{wtau(1e2), 4}));
   STATIC_ASSERT((wtau_c(1e3) == WithCounting{wtau(1e3), 3}));
   STATIC_ASSERT((wtau_c(1e4) == WithCounting{wtau(1e4), 3}));
   STATIC_ASSERT((wtau_c(1e10) == WithCounting{wtau(1e10), 3}));
   STATIC_ASSERT((wtau_c(1e20) == WithCounting{wtau(1e20), 3}));
   STATIC_ASSERT((wtau_c(1e100) == WithCounting{wtau(1e100), 2}));
   STATIC_ASSERT((wtau_c(1e200) == WithCounting{wtau(1e200), 2}));
   STATIC_ASSERT((wtau_c(1e1000L) == WithCounting{wtau(1e1000L), 1}));
   STATIC_ASSERT((wtau_c(max_value) == WithCounting{wtau(max_value), 1}));
  }

  {assert(accuracy_64(std::log(4) / (1+1/tau_meaneqLW_64), FP::lambertW0_lb_64(tau_meaneqLW_64)) <= 1);
   assert(accuracy_64(wtau_3, wtau_64(3)) == 0);
   assert(accuracy_64(wtau_1e1, wtau_64(1e1)) <= 1);
   assert(accuracy_64(wtau_1e2, wtau_64(1e2)) <= 1);
   assert(accuracy_64(wtau_1e3, wtau_64(1e3)) == 0);
   assert(accuracy_64(wtau_1e4, wtau_64(1e4)) == 0);
   assert(accuracy_64(wtau_1e10, wtau_64(1e10)) == 0);
   assert(accuracy_64(wtau_1e20, wtau_64(1e20)) == 0);
   assert(accuracy_64(wtau_1e100, wtau_64(1e100)) <= 1);
   assert(accuracy_64(wtau_1e200, wtau_64(1e200)) <= 1);
   assert(accuracy_64(wtau_max_64, wtau_64(max_value64)) == 0);
  }

  {mpfr_t x; dinit(x); mpfr_set_ui(x, 227, defrnd);
   lambertW0_lb(x);
   assert(to_string(x,20) ==  "0.39971353923393024631e1");
   assert(to_float80(x) == 3.997135392339302463L);
   mpfr_clear(x);
  }

  {mpfr_t x, y;
    dinit(x); dinit(y);

   mpfr_set_ui(x, 1, defrnd);
   elem_lb(x);
   mpfr_const_log2(y, defrnd);
   assert(mpfr_equal_p(x,y));

   for (unsigned i = 1; i <= 1000; ++i) {
     const float80 fx = i;
     const float80 frx = 1 / fx;

     mpfr_set_ld(x, frx, defrnd);
     elem_lb(x);
     assert(accuracy(to_float80(x), wtau_elem_lb(frx)) <= 1);
     assert(accuracy_64(to_float64(x), wtau_elem_lb_64(frx)) <= 1);

     if (i != 1) {
       mpfr_set_ld(x, fx, defrnd);
       lambertW0_lb(x);
       if (i != 227)
         assert(accuracy(to_float80(x), lambertW0_lb(fx)) <= 1);
       else {
         const float80 res1 = to_float80(x), res2 = lambertW0_lb(fx);
         assert(accuracy(res1, res2) <= 2);
         const std::string expected = "3.997135392339302463";
         std::stringstream out;
         out << Wrap(res1);
         assert(out.str() == expected);
       }
       assert(accuracy_64(to_float64(x), lambertW0_lb_64(fx)) <= 1);
     }

     mpfr_set_ld(x, fx, defrnd);
     wtau(x);
     assert(accuracy(to_float80(x), wtau(fx)) <= 1);
     assert(accuracy_64(to_float64(x), wtau_64(fx)) <= 1);
   }
   mpfr_clear(x); mpfr_clear(y);
  }

  {assert(wtau(NaN,0) == "NaN");
   assert(wtau(minfinity,0) == "NaN");
   assert(wtau(-1,0) == "NaN");
   assert(wtau(0,0) == "0");
   assert(wtau(pinfinity,0) == "inf");
   assert(wtau(1, MPFR_PREC_MAX/4+1) == "ERROR:prec");

   assert(wtau(1, 30) ==     "0.693147180559945309417232121458e0");
   assert(wtau(2, 30) ==     "0.962423650119206894995517826849e0");
   assert(wtau(P264m1,30) == "0.406562665724989266359973595092e2");
   assert(wtau(10, 40)  == "0.1802288966670586028618533336255756528239e1");
   assert(wtau(1e4, 40) == "0.7232163700022160825752746562165354028171e1");

   assert(wtau("NaN",0) == "NaN");
   assert(wtau("-inf",0) == "NaN");
   assert(wtau("-1",0) == "NaN");
   assert(wtau("0",0) == "0");
   assert(wtau("+inf",0) == "inf");
   assert(wtau("1", MPFR_PREC_MAX/4+1) == "ERROR:prec");

   assert(wtau("1", 30) == "0.693147180559945309417232121458e0");
   assert(wtau("2", 30) == "0.962423650119206894995517826849e0");
   assert(wtau("18446744073709551615",30) ==
                           "0.406562665724989266359973595092e2");
   assert(wtau("1e100", 30) ==
                           "0.224843106445118501539373134338e3");
   assert(wtau("1e1000", 30) ==
                           "0.229484667168350686965279278599e4");

   constexpr unsigned dec_prec = 10000;
   assert(wtau(0.5L, dec_prec) == const_func(ltau12, dec_prec));
   assert(wtau(0.25L, dec_prec) == const_func(ltau14, dec_prec));
   mpfr_t a;
   constexpr auto prec = dec2bin_prec(dec_prec);
   mpfr_init2(a,prec);
   mpfr_set_ui(a,1,defrnd);
   mpfr_div_ui(a,a,3,defrnd);
   assert(wtau(a, dec_prec) == const_func(ltau13, dec_prec));
   mpfr_init2(a,prec);
   mpfr_set_ui(a,1,defrnd);
   mpfr_div_ui(a,a,5,defrnd);
   assert(wtau(a, dec_prec) == const_func(ltau15, dec_prec));
  }
}
