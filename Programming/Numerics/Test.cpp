// Oliver Kullmann, 3.3.2019 (Swansea)
/* Copyright 2019, 2020, 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <type_traits>
#include <string_view>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "NumTypes.hpp"
#include "Conversions.hpp"
#include "NumInOut.hpp"
#include "FloatingPoint.hpp"
#include "Tau.hpp"
#include "Tau_mpfr.hpp"
#include "Optimisation.hpp"

/*
TODOS:

*/


namespace {

  const Environment::ProgramInfo proginfo{
        "0.9.17",
        "2.1.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/Test.cpp",
        "GPL v3"};

  using namespace FloatingPoint;
  using namespace Tau;
  using namespace Tau_mpfr;
  using namespace Optimisation;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }


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
/*
  {typedef std::vector<float80> VT;
   VT t;
   for (int i = 1; i < argc; ++i)
     t.push_back(std::stold(argv[i]));
   std::cout << argc-1;
   for (const float80 x : t) std::cout << " " << x;
   const auto end = first_pinf(t, argc-1);
   std::cout << "\n" << end << "\n";
   std::cout << Wrap(stau_ub(t,end)) << " " << Wrap(stau_dlb(t,end)) << " " << Wrap(stau_lb(t,end)) << "\n";
   std::cout << Wrap(stau(t)) << std::endl;
  }
*/
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
   assert(accuracy(0,min_value) == accuracy(0,-min_value));
   assert(accuracy(1,1+epsilon) == 1);
   assert(accuracy(1-epsilon,1) == 2);
   assert(accuracy(1-epsilon/2,1) == 1);
   assert(accuracy(1-epsilon,1+epsilon) == 3);
   assert(accuracy(1,1-epsilon) == 2);
   assert(accuracy(1+epsilon,1-epsilon) == 3);
   assert(accuracy(-1,-1-epsilon) == 1);
   assert(accuracy(1,1+10*epsilon) == 10);
   assert(accuracy(-1,-1-10*epsilon) == 10);
   assert(accuracy(1e1000L, FP::nextafter(FP::nextafter(1e1000L,pinfinity),pinfinity)) == 2);
   assert(accuracy(-1e1000L, FP::nextafter(FP::nextafter(-1e1000L,minfinity),minfinity)) == 2);
   assert(accuracy(0,min_value,PrecZ::min) == 1);
   assert(accuracy(0,-min_value,PrecZ::min) == 1);
   assert(accuracy(0,epsilon,PrecZ::eps) == 1);
   assert(accuracy(0,-epsilon,PrecZ::eps) == 1);
  }
  {assert(accuracyg(0.0L, denorm_min_value) == 1);
   assert(accuracyg(0.0L, -denorm_min_value) == 1);
   assert(accuracyg(0.0L,min_value) > 1e18);
   assert(accuracyg(0.0L,-min_value) > 1e18);
   assert(accuracyg(1.0L,1+epsilon) == 1);
   assert(accuracyg(-1.0L,-1-epsilon) == 1);
   assert(accuracyg(1-epsilon,1.0L) == 2);
   assert(accuracyg(1-epsilon/2,1.0L) == 1);
   assert(accuracyg(1-epsilon,1+epsilon) == 3);
   assert(accuracyg(1.0L,1-epsilon) == 2);
   assert(accuracyg(1+epsilon,1-epsilon) == 3);
   assert(accuracyg(1.0L,1+10*epsilon) == 10);
   assert(accuracyg(-1.0L,-1-10*epsilon) == 10);
   assert(accuracyg(1e1000L, FP::nextafter(FP::nextafter(1e1000L,pinfinity),pinfinity)) == 2);
   assert(accuracyg(-1e1000L, FP::nextafter(FP::nextafter(-1e1000L,minfinity),minfinity)) == 2);
   assert(accuracy(0.0L,min_value,PrecZ::min) == 1);
   assert(accuracy(0.0L,-min_value,PrecZ::min) == 1);
   assert(accuracy(0.0L,epsilon,PrecZ::eps) == 1);
   assert(accuracy(0.0L,-epsilon,PrecZ::eps) == 1);
  }
  {assert(accuracy_64(0, denorm_min_value64) == 1);
   assert(accuracy_64(0, -denorm_min_value64) == 1);
   assert(accuracy_64(0,min_value64) > 4e15);
   assert(accuracy_64(0,-min_value64) > 4e15);
   assert(accuracy_64(0,min_value64) == accuracy_64(0,-min_value64));
   assert(accuracy_64(1,1+epsilon64) == 1);
   assert(accuracy_64(1-epsilon64,1) == 2);
   assert(accuracy_64(1-epsilon64/2,1) == 1);
   assert(accuracy_64(1-epsilon64,1+epsilon64) == 3);
   assert(accuracy_64(1,1-epsilon64) == 2);
   assert(accuracy_64(1+epsilon64,1-epsilon64) == 3);
   assert(accuracy_64(-1,-1-epsilon64) == 1);
   assert(accuracy_64(1,1+10*epsilon64) == 10);
   assert(accuracy_64(-1,-1-10*epsilon64) == 10);
   assert(accuracy_64(1e100, std::nextafter(std::nextafter(1e100,pinfinity64),pinfinity64)) == 2);
   assert(accuracy_64(-1e100, std::nextafter(std::nextafter(-1e100,minfinity64),minfinity64)) == 2);
   assert(accuracy_64(0.0,min_value64,PrecZ::min) == 1);
   assert(accuracy_64(0.0,-min_value64,PrecZ::min) == 1);
   assert(accuracy_64(0.0,epsilon64,PrecZ::eps) == 1);
   assert(accuracy_64(0.0,-epsilon64,PrecZ::eps) == 1);
  }
  {assert(accuracyg(0.0, denorm_min_value64) == 1);
   assert(accuracyg(0.0, -denorm_min_value64) == 1);
   assert(accuracyg(0.0,min_value64) > 4e15);
   assert(accuracyg(0.0,-min_value64) > 4e15);
   assert(accuracyg(0.0,min_value64) == accuracyg(0.0,-min_value64));
   assert(accuracyg(1.0,1+epsilon64) == 1);
   assert(accuracyg(-1.0,-1-epsilon64) == 1);
   assert(accuracyg(1-epsilon64,1.0) == 2);
   assert(accuracyg(1-epsilon64/2,1.0) == 1);
   assert(accuracyg(1-epsilon64,1+epsilon64) == 3);
   assert(accuracyg(1.0,1-epsilon64) == 2);
   assert(accuracyg(1+epsilon64,1-epsilon64) == 3);
   assert(accuracyg(1.0,1+10*epsilon64) == 10);
   assert(accuracyg(-1.0,-1-10*epsilon64) == 10);
   assert(accuracyg(1e100, std::nextafter(std::nextafter(1e100,pinfinity64),pinfinity64)) == 2);
   assert(accuracyg(-1e100, std::nextafter(std::nextafter(-1e100,minfinity64),minfinity64)) == 2);
   assert(accuracyg(0.0,min_value64,PrecZ::min) == 1);
   assert(accuracyg(0.0,-min_value64,PrecZ::min) == 1);
   assert(accuracyg(0.0,epsilon64,PrecZ::eps) == 1);
   assert(accuracyg(0.0,-epsilon64,PrecZ::eps) == 1);
  }
  {typedef std::vector<float80> V1;
   typedef std::vector<float64> V2;
   typedef std::array<float80,3> V3;
   typedef std::array<float64,3> V4;
   assert(eqp(accuracyv(V1{},{}), {}));
   assert(eqp(accuracyv(V1{0},{}), {}));
   assert(eqp(accuracyv(V1{1},{1}), {0}));
   assert(accuracyv(V1{0.5L}, V2{0.5}) == V1{0});
   assert((accuracyv<V1,V4,V2>({1,1-epsilon,1+epsilon}, {1,1,1}) == V2{0,2,1}));
   assert((accuracyv<V3,V4,V2>({1-epsilon,1,1+epsilon}, {1,1,1}) == V2{2,0,1}));   
  }
  {assert(accuracymax<std::vector<float80>>({},{}) == -1);
   assert(accuracymax<std::vector<float80>>({},{0}) == -1);
   assert(accuracymax<std::vector<float80>>({0},{0}) == 0);
   assert((accuracymax<std::vector<float80>>({0},{denorm_min_value,0}) == 1));
   assert((accuracymax<std::vector<float80>>({-denorm_min_value,0,1},{denorm_min_value,0}) == 2));
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

  {const std::vector<float80> pvals{minfinity, -5, -4, -3, -2, -1, 0, 1/3.0L, 0.5L, 1, 2, 3, 4, 5};
   for (const float80 p : pvals) {
     const float80 res = pmean(1,2,p);
     if (p == minfinity) assert(res == 1);
     else if (p == pinfinity) assert(res == 2);
     else if (p == -1) assert(res == 4 / 3.0L);
     else if (p == 0) assert(res == Sqr2);
     else if (p == 1/3.0L) assert(res == cb(1 + FP::cbrt(2)) / 8); // BUG with gcc 10.1
     else if (p == 0.5L) assert(res == sq(1 + FP::sqrt(2)) / 4); // BUG with gcc 10.1
     else if (p == 1) assert(res == 1.5L);
     else if (p == 2) assert(res == sqrt(2.5L));
     else if (p == 3) assert(res == cbrt(4.5L));
     else assert(accuracy(res, FP::pow((1 + FP::pow(2,p)) / 2, 1/p)) <= 1);
   }
   const std::vector<float80> vals{0,0.5L,1,2,3,4,pinfinity};
   for (const float80 p : pvals)
     for (const float80 a : vals)
       for (const float80 b : vals) {
         const float80 res = pmean(a,b,p);
         if (p == minfinity) assert(res == min(a,b));
         else if (p == pinfinity) assert(res == max(a,b));
         else if (p == 0)
           if ((a==0 and b==pinfinity) or (a==pinfinity and b==0))
             assert(isnan(res));
           else
             assert(res == FP::sqrt(a*b));
         else {
           const float80 ares = std::pow(midpoint(pow(a,p), pow(b,p)), 1/p);
           assert(accuracy(res, ares) <= 3);
         }
       }
  }

  {mpfr_t x;
   dinit(x);
   mpfr_set_ui(x,0,defrnd);
   assert(to_string(x,5) == "0.0e0");
   mpfr_set_ui(x,1,defrnd);
   mpfr_div_ui(x,x,3,defrnd);
   assert(to_string(x,3) == "0.333e0");
   mpfr_set_ui(x,1,defrnd);
   mpfr_div_ui(x,x,10,defrnd);
   assert(to_string(x,10) == "0.1e0");
   mpfr_set_nan(x);
   assert(to_string(x,0) == "NaN");
   mpfr_set_inf(x,1);
   assert(to_string(x,0) == "inf");
   mpfr_set_inf(x,0);
   assert(to_string(x,0) == "inf");
   mpfr_set_inf(x,1);
   assert(to_string(x,0) == "inf");
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
         out.str("");
         out << res1;
         assert(out.str() == "3.99714");
         assert(fullprec_float80(out) == 6);
         assert(out.precision() == 20);
         out.str("");
         out << res1;
         assert(out.str() == expected);
         assert(fullprec_float64(out) == 20);
         assert(out.precision() == 17);
         out.str("");
         out << res1;
         assert(out.str() == "3.9971353923393025");
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

  {assert(ltau(NaN,0,0) == "NaN");
   assert(ltau(-1,1,0) == "NaN");
   assert(ltau(0,pinfinity,0) == "NaN");
   assert(ltau(1,0,0) == "inf");
   assert(ltau(FP::pinfinity,1,0) == "0");
   assert(ltau(1,1, MPFR_PREC_MAX/4+1) == "ERROR:prec");
  }

  {mpfr_t x, y; dinit(x); dinit(y);
   for (unsigned i = 1; i <= 100; ++i) {
     const float80 fi = i, fri = 1 / fi;
     for (unsigned j = 1; j <= 100; ++j) {
       const float80 fj = j, frj = 1 / fj;
       mpfr_set_ld(x,fi,defrnd); mpfr_set_ld(y,fj,defrnd);
       ltau(x,y);
       assert(accuracy(to_float80(x), ltau(fi,fj)) <= 2);
       mpfr_set_ld(x,fi,defrnd); mpfr_set_ld(y,frj,defrnd);
       ltau(x,y);
       assert(accuracy(to_float80(x), ltau(fi,frj)) <= 2);
       mpfr_set_ld(x,fri,defrnd); mpfr_set_ld(y,fj,defrnd);
       ltau(x,y);
       assert(accuracy(to_float80(x), ltau(fri,fj)) <= 2);
       mpfr_set_ld(x,fri,defrnd); mpfr_set_ld(y,frj,defrnd);
       ltau(x,y);
       assert(accuracy(to_float80(x), ltau(fri,frj)) <= 2);
     }
   }
  }

  {for (unsigned dec_prec = 20; dec_prec <= 100; ++dec_prec) {
     assert(ltau(1,1,dec_prec) == const_func(ltau11, dec_prec));
     assert(ltau(1,2,dec_prec) == const_func(ltau12, dec_prec));
     assert(ltau(1,3,dec_prec) == const_func(ltau13, dec_prec));
     assert(ltau(1,4,dec_prec) == const_func(ltau14, dec_prec));
     assert(ltau(1,5,dec_prec) == const_func(ltau15, dec_prec));
   }

  }

  {assert(ltau(" NaN","0",0) == "NaN");
   assert(ltau(" -1"," 1",0) == "NaN");
   assert(ltau(" 0"," inf",0) == "NaN");
   assert(ltau(" 1"," 0",0) == "inf");
   assert(ltau(" inf","+1",0) == "0");
   assert(ltau(" +1"," 1", MPFR_PREC_MAX/4+1) == "ERROR:prec");
  }

  {assert(ltau("2.2", "3.4", 40) == "0.2516068722968234282538382045931522266372e0");
   assert(ltau("0.3", "7.9", 40) == "0.3074794115298824465699417106454736002806e0");
   assert(ltau("3.12", "0.99", 40) == "0.3751443889040340235128013093228783316655e0");
   assert(ltau("1e100", "1.23", 40) == "0.2246370093224726609227038528991696611548e-97");
  }

  {mpfr_t x, y; dinit(x); dinit(y);
   for (unsigned i = 1; i <= 100; ++i) {
     const float80 fi = i, fri = 1 / fi;
     for (unsigned j = 1; j <= 100; ++j) {
       const float80 fj = j, frj = 1 / fj;
       mpfr_set_ld(x,fi,defrnd); mpfr_set_ld(y,fj,defrnd);
       mtau(x,y);
       assert(accuracy(to_float80(x), mtau(fi,fj)) <= 3);
       mpfr_set_ld(x,fi,defrnd); mpfr_set_ld(y,frj,defrnd);
       mtau(x,y);
       assert(accuracy(to_float80(x), mtau(fi,frj)) <= 2);
       mpfr_set_ld(x,fri,defrnd); mpfr_set_ld(y,fj,defrnd);
       mtau(x,y);
       assert(accuracy(to_float80(x), mtau(fri,fj)) <= 2);
       mpfr_set_ld(x,fri,defrnd); mpfr_set_ld(y,frj,defrnd);
       mtau(x,y);
       assert(accuracy(to_float80(x), mtau(fri,frj)) <= 2);
     }
   }
  }

  {assert(mtau("0", "0", 20) == "0");
   assert(mtau("1", "1", 20) == "0.1e1");
   assert(mtau("1000", "1000", 20) == "0.1e4");
   assert(mtau("inf", "inf", 20) == "inf");
   assert(mtau("0", "2", 20) == "0");
   assert(mtau("2", "0", 20) == "0");
  }

  {assert(pmean("0", "0", "0", 20) == "0");
   assert(pmean("0", "0", "1", 20) == "0");
   assert(pmean("1", "1", "2", 20) == "0.1e1");
   assert(pmean("inf", "inf", "3", 20) == "inf");
  }

  {assert(diffkptau(1,0, 0,20) == 0);
   
  }

  {assert(tau(1e-1000L, 1e-1000L) == FP::pinfinity);
   assert(tau(FP::max_value,FP::max_value) == 1);
   assert(tau(FP::min_value,FP::min_value) == FP::pinfinity);
   assert(tau(1e1000L,2e1000L) == 1);
   assert(tau(1e-1000L,2e-1000L) == FP::pinfinity);
  }

  {for (unsigned dec_prec = 20; dec_prec <= 100; ++dec_prec) {
     assert(tau(1,1,dec_prec) == const_func(tau11, dec_prec));
     assert(tau(1,2,dec_prec) == const_func(tau12, dec_prec));
     assert(tau(1,3,dec_prec) == const_func(tau13, dec_prec));
     assert(tau(1,4,dec_prec) == const_func(tau14, dec_prec));
     assert(tau(1,5,dec_prec) == const_func(tau15, dec_prec));
   }
  }

  {assert(tau(" NaN","0",0) == "NaN");
   assert(tau(" -1"," 1",0) == "NaN");
   assert(tau(" 0"," inf",0) == "NaN");
   assert(tau(" 1"," 0",0) == "inf");
   assert(tau(" inf","+1",0) == "1");
   assert(tau(" +1"," 1", MPFR_PREC_MAX/4+1) == "ERROR:prec");
  }

  {mpfr_t x, y; dinit(x); dinit(y);
   for (unsigned i = 1; i <= 100; ++i) {
     const float80 fi = i, fri = 1 / fi;
     for (unsigned j = 1; j <= 100; ++j) {
       const float80 fj = j, frj = 1 / fj;
       mpfr_set_ld(x,fi,defrnd); mpfr_set_ld(y,fj,defrnd);
       tau(x,y);
       assert(accuracy(to_float80(x), tau(fi,fj)) <= 3);
       mpfr_set_ld(x,fi,defrnd); mpfr_set_ld(y,frj,defrnd);
       tau(x,y);
       assert(accuracy(to_float80(x), tau(fi,frj)) <= 3);
       mpfr_set_ld(x,fri,defrnd); mpfr_set_ld(y,fj,defrnd);
       tau(x,y);
       assert(accuracy(to_float80(x), tau(fri,fj)) <= 3);
       mpfr_set_ld(x,fri,defrnd); mpfr_set_ld(y,frj,defrnd);
       tau(x,y);
       assert(accuracy(to_float80(x), tau(fri,frj)) <= 1000);
     }
   }
  }

  {assert(tau("0.01", "0.01", 31) == "0.1267650600228229401496703205376e31");
   assert(tau("0.01", "0.01", 40) == "0.1267650600228229401496703205376e31");
   assert(tau("0.001", "0.001", 302) == "0.10715086071862673209484250490600018105614048117055336074437503883703510511249361224931983788156958581275946729175531468251871452856923140435984577574698574803934567774824230985421074605062371141877954182153046474983581941267398767559165543946077062914571196477686542167660429831652624386837205668069376e302");
   assert(tau("2.2", "3.4", 40) == "0.1286090340148076594185691070892477155152e1");
   assert(tau("0.3", "7.9", 40) == "0.1359992808042429993082448241532461033708e1");
   assert(tau("3.12", "0.99", 40) == "0.1455201514401610687691825573455368581226e1");
   assert(tau("1e100", "1.23", 40) == "0.1e1");
   assert(tau("1e100", "1.23", 99) == "0.100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002e1");
  }

  {assert(not is_probdist_basic({}));
   assert(is_probdist_basic({0}));
   assert(is_probdist_basic({0,1,1,0,0.5}));
   assert(not is_probdist_basic({2}));
   assert(not is_probdist_basic({-1}));
   assert(not is_probdist_basic({NaN}));

   assert(not is_probdist_precise({0}));
   assert(is_probdist_precise({0,1,0,0}));
   assert(is_probdist_precise({0.5,0.5,0,0}));

   assert(not is_lprobdist_basic({}));
   assert(is_lprobdist_basic({0}));
   assert(is_lprobdist_basic({0,0,-1,minfinity}));
   assert(not is_lprobdist_basic({1}));
   assert(not is_lprobdist_basic({NaN}));
  }

  {assert(exp(minfinity) == 0);
   assert(FP::exp(0) == 1);

   assert(ptau(0,0) == probdist_t{});
   assert(ptau(0,1) == probdist_t{});
   assert(ptau(1,0) == probdist_t{});
   assert((ptau(0,pinfinity) == probdist_t{1,0}));
   assert((ptau(pinfinity,0) == probdist_t{0,1}));
   assert((ptau(1,pinfinity) == probdist_t{1,0}));
   assert((ptau(pinfinity,1) == probdist_t{0,1}));

   assert((ptau(1,1) == probdist_t{0.5,0.5}));
   assert((ptau(2,2) == probdist_t{0.5,0.5}));

   constexpr float80 goldenm1 = 2 / (sqrt(5.0L)+1);
   constexpr float80 goldenm2 = 4 / sq(sqrt(5.0L)+1);
   const probdist_t goldenp = ptau(1,2);
   assert(goldenm1 == goldenp[0]);
   assert(accuracy(goldenm2, goldenp[1]) <= 1);
  }

  {std::stringstream s;
   {float32 x = 12.5;
    fullprec_floatg<float32>(s);
    s << std::scientific;
    s << WrapE(x);
    assert(s.str() == "1.25e+01");
    x = 12;
    s.str("");
    s << WrapE(x);
    assert(s.str() == "1.2e+01");
    x = 0.001;
    s.str("");
    s << WrapE(x);
    assert(s.str() == "1.00000005e-03");
    x = 0.0;
    s.str("");
    s << WrapE(x);
    assert(s.str() == "0");
   }
  }

  {typedef std::vector<float80> VT;
   assert(first_pinf(VT{0,0,pinfinity,pinfinity,0}, 5u) == 2);
  }
  {typedef std::vector<float80> VT;
   assert(stau_ub(VT{1,1,0}, 2u) == FP::log(3));
   assert(stau_ub(VT{1,1,1,0,-1}, 3u) == FP::log(4));
   assert(stau_ub(VT{1,1,1,1}, 4u) ==  FP::log(5));
   assert(FP::accuracy(FP::log(3) * 5 / 6, stau_ub(VT{1,2}, 2u)) <= 1);
   assert(stau_ub(VT{2,3}, 2u) == FP::log(3) * 11 / 18);
  }
  {typedef std::vector<float80> VT;
   assert(stau_lb(VT{1,1,0}, 2u) == FP::log(3));
   assert(stau_lb(VT{1,1,1,0,-1}, 3u) == FP::log(4));
   assert(stau_lb(VT{1,1,1,1}, 4u) ==  FP::log(5));
   assert(stau_lb(VT{1,2}, 2u) == FP::log(3) * 3 / 4);
   assert(stau_lb(VT{2,3}, 2u) == FP::log(3) / 2);
  }

  {typedef std::vector<float80> VT;
   assert(stau(VT{}) == 0);
   assert(stau(VT{1}) == Log2);
   assert(stau(VT{2}) == log_golden_ratio);
   assert(stau(VT{pinfinity}) == 0);
   assert(stau(VT{pinfinity,pinfinity}) == 0);
   assert(stau(VT{2,pinfinity,pinfinity}) == log_golden_ratio);
   assert(stau(VT{1,1}) == log(3.0L));
   assert(stau(VT{1,1,1}) == log(4.0L));
   assert(stau(VT{1,1,pinfinity}) == log(3.0L));
   assert(stau(VT{1,1,1,pinfinity,pinfinity}) == log(4.0L));
   assert(stau(VT{1}) == const_func(ltau11));
   assert(stau(VT{2}) == const_func(ltau12));
   assert(accuracy(const_func(ltau13), stau(VT{3})) <= 1);
   assert(stau(VT{4}) == const_func(ltau14));
   assert(stau(VT{5}) == const_func(ltau15));

   assert(stau(VT{3,4}) == log_golden_ratio);
   assert(stau(VT{4,4,5}) == log_golden_ratio);
   assert(accuracy(const_func(ltau13), stau(VT{4,6})) <= 1);
   assert(accuracy(const_func(ltau13), stau(VT{4,7,9})) <= 1);
   assert(stau(VT{6,8,9}) == const_func(ltau14));
   assert(stau(VT{6,10}) == const_func(ltau15));
   assert(stau(VT{6,11,15}) == const_func(ltau15));
  }

  {typedef std::vector<float80> VT;
   assert(FP::isnan(ltau(VT{})));
   assert(ltau(VT{0}) == 0);
   assert(ltau(VT{1,2}) == log_golden_ratio);
   assert(FP::isnan(ltau(VT{0,pinfinity})));
   assert(FP::isnan(ltau(VT{0,pinfinity,pinfinity})));
   assert(ltau(VT{0,pinfinity,1}) == pinfinity);
   assert(ltau(VT{1,4,4,5}) == log_golden_ratio);
   assert(ltau(VT{2,8,8,10}) == 0.5 * log_golden_ratio);

   assert(FP::isnan(mtau(VT{})));
   assert(FP::isnan(mtau(VT{1})));
   assert(FP::isnan(mtau(VT{1,pinfinity,pinfinity})));
   assert(FP::isnan(mtau(VT{1,pinfinity})));
   assert(FP::isnan(mtau(VT{0,pinfinity})));
   assert(FP::isnan(mtau(VT{0,pinfinity,pinfinity})));
   assert(mtau(VT{pinfinity,pinfinity}) == pinfinity);
   assert(mtau(VT{0,1}) == 0);
   assert(mtau(VT{0,0,1}) == 0);
   assert(mtau(VT{0,1,pinfinity}) == 0);
   assert(mtau(VT{2,2,2,2}) == 2);
   assert(mtau(VT{1,2}) == FP::Log2 / log_golden_ratio);
   assert(mtau(VT{pinfinity,1,2,pinfinity}) == FP::log(4) / log_golden_ratio);
   assert(mtau(VT{1,4,4,5,pinfinity}) == FP::log(5) / log_golden_ratio);

   assert(FP::isnan(tau(VT{})));
   assert(tau(VT{0}) == 1);
   assert(tau(VT{1,2}) == golden_ratio);
   assert(tau(VT{1,4,4,5}) == golden_ratio);
   assert(FP::isnan(tau(VT{0,pinfinity})));
   assert(FP::isnan(tau(VT{0,pinfinity,pinfinity})));
   assert(tau(VT{0,pinfinity,1}) == pinfinity);
  }

  {assert(not valid(vec_t{}));
   assert(valid(vec_t{0}));
   assert(not valid(vec_t{FP::NaN}));
   assert(valid(vec_t{0,1,0,2}));
   assert(not valid(vec_t{0,0,FP::NaN}));
   assert(valid(point_t{}));
   assert(valid(point_t{0,0}));
   assert(not valid(point_t{FP::NaN,0}));
   assert(not valid(point_t{0,FP::NaN}));
   assert(valid(list_points_t{}));
   assert(valid(list_points_t{{0,0},{1,1},{}}));
   assert(not valid(list_points_t{{0,0},{1,1},{FP::NaN,0}}));
   assert(not valid(fpoint_t{}));
   assert(valid(fpoint_t{{0},0}));
   assert(valid(Interval{}));
   assert(valid(Interval{1,2}));
   assert(valid(Interval{1,1}));
   assert(not valid(Interval{1,0}));
   assert(not valid(Interval{-1,0}));
   assert(valid(list_intervals_t{}));
   assert(valid(list_intervals_t{{0,0},{1,2},{}}));
   assert(not valid(list_intervals_t{{0,0},{1,1},{-1,0}}));
  }

  {const auto f1 = [](vec_t,y_t){return 33;};
   assert(eval(f1, {22}, 11) == 11);
   assert(eval(f1, {22}, 44) == 33);
   const auto f2 = [](const vec_t& x,y_t){return x[0];};
   assert((eval(f2, {77,88}, 1) == 1));
   assert((eval(f2, {77,88}, 100) == 77));
   assert(eval(f1, {{22},11}) == 11);
   assert(eval(f1, {{22}, 44}) == 33);
  }

  {static_assert(eqp(Interval{}, Interval(0,0,0,FP::pinfinity)));
   static_assert(eqp(Interval(1,2), Interval(1,2,0,FP::pinfinity)));
   static_assert(valid(Interval{}));
   static_assert(not valid(Interval(0,-1)));
   static_assert(valid(Interval(0,1)));
   static_assert(not valid(Interval(0,1,1,1)));
   static_assert(not valid(Interval(0,1,0,0)));
   static_assert(valid(Interval(0,1,0,1)));
   assert(valid(list_intervals_t{}));
   assert(not valid(list_intervals_t{{1,0}}));
  }

  {static_assert(element(0, Interval{-1,1}));
   static_assert(not element(-2, Interval{-1,1}));
   static_assert(not element(2, Interval{-1,1}));
   assert(element(vec_t{}, list_intervals_t{}));
   assert(element(vec_t{}, list_intervals_t{{}}));
   assert(element(vec_t{1,2}, list_intervals_t{{1,1},{2,2}}));
   assert(not element(vec_t{1,3}, list_intervals_t{{1,1},{2,2}}));
   static_assert(element(point_t{}, Interval{-1,1}));
   static_assert(not element(point_t{-2,0}, Interval{-1,1}));
   static_assert(not element(point_t{2,0}, Interval{-1,1}));
   assert((element(list_points_t{{}, {1,2},{-2,4}}, list_intervals_t{{},{1,1},{-3,-1}})));
   assert((not element(list_points_t{{}, {1,2},{-2,4},{12,0}}, list_intervals_t{{},{1,1},{-3,-1},{10,11}})));
   assert(element(evec_t{{2,true},{0},{-10,true}}, {{3,4},{},{1,2}}));
  }

  {assert(min_value_points(list_points_t{{}}) == 0);
   assert(min_value_points(list_points_t{{0,5},{1,-7}}) == -7);
   assert(eqp(val_argument_points(list_points_t{{1,2},{2,3},{3,2},{4,0},{5,2}}, 2), {3,2}));
   assert((min_argument_points(list_points_t{{}}) == point_t{0,0}));
   assert((min_argument_points(list_points_t{{0,5},{1,-7}}) == point_t{1,-7}));
   assert((min_argument_points(list_points_t{{-1,3},{0,0},{5,1},{1,0},{7,2},{2,0}}) == point_t{1,0}));
   assert((min_argument_points(list_points_t{{-1,3},{0,0},{5,1},{1,0},{7,2},{2,0},{55,77},{3,0}}) == point_t{1,0}));
   assert((min_argument_points(list_points_t{{-1,3.5},{0,0.5},{5,1},{1,0.5},{7,2},{2,0.5},{55,77},{3,0.5}}) == point_t{1,0.5}));
  }

  {assert(valid_partitionsize(0));
   assert(valid_partitionsize(FP::P264m1-2));
   assert(not valid_partitionsize(FP::P264m1-1));
  }

  {assert(eqp(sampling_points(0,1,1), {0,1}));
   assert(eqp(sampling_points(0,1,2), {0,0.5,1}));
   assert(eqp(sampling_points(0,5,5), {0,1,2,3,4,5}));
   assert(eqp(sampling_points(0,1,0), {0.5}));

   RandGen::RandGen_t g;
   assert(eqp(sampling_points(0,1,0,&g), {0.045268295711760839676L}));
   assert(eqp(sampling_points(-11,11,5,&g),
              {-7.1954707087509969213L,-5.4726325547959859636L,
               -3.703930226312383189L,0.55384530295453657624L,
               5.9094529067279158935L,9.2298023044340408334L}));
  }

  {const function_t f = [](const vec_t& x, const y_t b){
      const y_t res = std::accumulate(x.begin(),x.end(),0.0L);
      if (res > b) return b+1;
      else return res;
    };

   assert((bbopt_index(vec_t{0}, y_t{0}, 0, Interval{0,10}, f, 1) == point_t{0,0}));
   assert((bbopt_index(vec_t{1}, y_t{1}, 0, Interval{1,10}, f, 1) == point_t{1,1}));
   assert((bbopt_index(vec_t{5}, y_t{5}, 0, Interval{2,10}, f, 10) == point_t{2,2}));
   assert((bbopt_index(vec_t{1}, y_t{1}, 0, Interval{1,2}, f, 10) == point_t{1,1}));
   assert((bbopt_index(vec_t{3,1,4}, y_t{8}, 1, Interval{1,2}, f, 1) == point_t{1,8}));
   assert((bbopt_index(vec_t{3,1,4}, y_t{8}, 1, Interval{1,2}, f, 100) == point_t{1,8}));
   assert((bbopt_index(vec_t{3,1,4}, y_t{8}, 1, Interval{0,1000}, f, 100) == point_t{0,7}));

   assert((bbopt_index_parallel(vec_t{0}, y_t{0}, 0, Interval{0,10}, f, 1, 4) == point_t{0,0}));
   assert((bbopt_index_parallel(vec_t{1}, y_t{1}, 0, Interval{1,10}, f, 1, 4) == point_t{1,1}));
   assert((bbopt_index_parallel(vec_t{5}, y_t{5}, 0, Interval{2,10}, f, 10, 4) == point_t{2,2}));
   assert((bbopt_index_parallel(vec_t{1}, y_t{1}, 0, Interval{1,2}, f, 10, 4) == point_t{1,1}));
   assert((bbopt_index_parallel(vec_t{3,1,4}, y_t{8}, 1, Interval{1,2}, f, 1, 4) == point_t{1,8}));
   assert((bbopt_index_parallel(vec_t{3,1,4}, y_t{8}, 1, Interval{1,2}, f, 100, 4) == point_t{1,8}));
   assert((bbopt_index_parallel(vec_t{3,1,4}, y_t{8}, 1, Interval{0,1000}, f, 100, 4) == point_t{0,7}));

   for (index_t T = 1; T <= 20; ++T) {
     assert((bbopt_rounds({vec_t{3,1,4}, y_t{8}}, list_intervals_t{{2,4},{0,1000}, {3,5}}, f, {500,1,1,T}) == fpoint_t{{2,0,3},5}));
     assert((bbopt_rounds({vec_t{3,1,4}, y_t{8}}, list_intervals_t{{2,4},{0,1000}, {3,5}}, f, {500, 3, 1, T}) == fpoint_t{{2,0,3},5}));
     assert((bbopt_rounds({vec_t{3,1,4}, y_t{8}}, list_intervals_t{{2,4},{0,1000}, {3,5}}, f, {500, 0, 1, T}) == fpoint_t{{3,1,4},8}));

     assert((bbopt_rounds({vec_t{3,1,4}, y_t{8}}, list_intervals_t{{2,4,2,4},{0,1000}, {3,10,2,10}}, f, {500, 1, 5, T}) == fpoint_t{{2,0,2},4}));
     assert((bbopt_rounds({vec_t{3,1,4}, y_t{8}}, list_intervals_t{{2,6,1,6},{1,1000,0,1000}, {3,7,2,8}}, f, {500, 3, 2, T}) == fpoint_t{{1,0,2},3}));
     assert((bbopt_rounds({vec_t{3,1,4}, y_t{8}}, list_intervals_t{{2,4},{0,1000}, {3,5}}, f, {500, 0, 1, T}) == fpoint_t{{3,1,4},8}));
   }
  }

  {const function_t f = [](const vec_t& x, y_t){
      y_t sum = 0;
      for (index_t i = 0; i < x.size(); ++i) sum += sq(x[i] - i);
      return sum;};
   assert((bbopt_rounds({vec_t{0,1,2,3}, y_t{0}}, list_intervals_t{{0,10},{0,10}, {0,10},{0,10}}, f, {100, 2, 5}) == fpoint_t{{0,1,2,3},0}));
   assert((bbopt_rounds({vec_t{1,2,3,4}, y_t{4}}, list_intervals_t{{0,10},{0,10}, {0,10},{0,10}}, f, {100, 2, 5}) == fpoint_t{{0,1,2,3},0}));
   assert((bbopt_rounds({vec_t{1,2,3,4}, y_t{4}}, list_intervals_t{{1,3},{2,4}, {3,5},{4,6}}, f, {10, 2, 2}) == fpoint_t{{0.5,1.5,2.5,3.5},1}));
  }

  {list_intervals_t Iv{{0,2}};
   shrink_intervals({1},Iv, 1);
   assert(eqp(Iv, {{0,2}}));
   shrink_intervals({1},Iv, 2);
   assert(eqp(Iv, {{0.5,1.5}}));
   Iv[0].hl = 0.5;
   shrink_intervals({0.5},Iv, 1);
   assert(eqp(Iv, {{0.5,1,0.5,FP::pinfinity}}));
   Iv[0].hr = 1;
   shrink_intervals({1},Iv, 1);
   assert(eqp(Iv, {{0.75,1,0.5,1}}));
  }

  {assert((Parameters("8.5", "-1", "55.5", "1e10") == Parameters(9,0,56,FP::P232m1)));
   assert((Parameters("22.2", "77.7", "100.5", "33.3") == Parameters(22,78,101,33)));
  }

  {assert(eqp(to_vec_float80("", ' '), {}));
   assert(eqp(to_vec_float80(" 1.1 \t 2.3   ", ' '), {1.1L, 2.3L}));
   assert(eqp(to_vec_float80(" 1.1 , \t 2.3 \t\n, 77  ", ','), {1.1L,2.3L,77}));
  }

  {assert(eqp(to_vec_float80ai("1", ' ', 0), {{},{1,true}}));
   assert(eqp(to_vec_float80ai("1.0", ' ', 0), {{},{1}}));
   assert(eqp(to_vec_float80ai("1.0 2.0 3 4.0 5.1", ' ', 2), {{1,2,4,5.1L},{3,true}}));
   assert(eqp(to_vec_float80ai("1.1 2.2 3.3 4.4 5.5", ' ', 1), {{1.1L,3.3L,4.4L,5.5L},{2.2L}}));
  }

  {assert(eqp(fill_possibilities({}, {}), {}));
   assert(eqp(fill_possibilities({{}}, {{-1,1}}), {{0}}));
   assert(eqp(fill_possibilities({{0.5}}, {{-1,1}}), {{0.5}}));
   assert(eqp(fill_possibilities({{1}}, {{-1,1}}), {{1}}));
   assert(eqp(fill_possibilities({{0,true}}, {{-1,1}}), {{0}}));
   assert(eqp(fill_possibilities({{1,true}}, {{-1,1}}), {{-1,1}}));
   assert(eqp(fill_possibilities({{2,true}}, {{-1,1}}), {{-1,0,1}}));
   assert(eqp(fill_possibilities({{4,true}}, {{-1,1}}), {{-1,-0.5,0,0.5,1}}));
   assert(eqp(fill_possibilities({{4,true}}, {{-1,-1}}), {{-1}}));
   assert(eqp(fill_possibilities({{4,true},{2.2L},{4,true}}, {{-1,1},{1,3},{0,4}}), {{-1,-0.5,0,0.5,1},{2.2L},{0,1,2,3,4}}));
  }

  {typedef std::vector<int> itv_t;
   itv_t curr;
   curr = itv_t{0};
   assert(not next_combination(curr, {0}, {1}));
   assert(eqp(curr, {1}));
   assert(next_combination(curr, {0}, {3}));
   assert(eqp(curr, {2}));
   curr = itv_t{0,0};
   assert(next_combination(curr, {0,0}, {2,2}));
   assert(eqp(curr, {1,0}));
   assert(next_combination(curr, {0,0}, {2,2}));
   assert(eqp(curr, {0,1}));
   assert(next_combination(curr, {0,0}, {2,2}));
   assert(eqp(curr, {1,1}));
   assert(not next_combination(curr, {0,0}, {2,2}));
   assert(eqp(curr, {2,2}));
  }

  {assert(bealef({3,0.5}) == 0);
   assert(bealef({0,0}) == 909.0L/64);
   static_assert(909.0L/64 == 14.203125);
   assert(bealef({1,0}) == 285.0L/64);
   assert(bealef({0,1}) == 909.0L/64);
   assert(bealef({1,1}) == 909.0L/64);
   assert(bealef({0,-1}) == 909.0L/64);
   assert(bealef({-1,-1}) == 2477.0L/64);
   assert(bealef({3,0}) == 189.0L/64);

   assert(goldsteinpricef({0,-1}) == 3);
   assert(goldsteinpricef({0,0}) == 600);
   assert(goldsteinpricef({1,0}) == 726);
   assert(goldsteinpricef({0,1}) == 28611);
   assert(goldsteinpricef({1,1}) == 1876);
  }

  {const function_t bf = expand(bealef);
   const function_t gf = expand(goldsteinpricef);

   assert(eqp(bbopt_rounds_scan({{0},{0}}, {{-5,5,-5,5},{-5,5,-5,5}}, bf, {0,1,1,1}, {}, false), {{0,0}, 14.203125}));
   assert(eqp(bbopt_rounds_scan({{-1},{-1}}, {{-5,5,-5,5},{-5,5,-5,5}}, bf, {0,1,1,1}, {}, false), {{0,-1}, 14.203125}));
   assert(eqp(bbopt_rounds_scan({{0},{0}}, {{0,6,0,6},{0,1,0,1}}, bf, {0,1,1,1}, {}, false), {{3,0.5}, 0}));

   assert(eqp(bbopt_rounds_scan({{0},{0}}, {{-5,5,-5,5},{-5,5,-5,5}}, gf, {0,1,1,1}, {}, false), {{0,0}, 600}));
  }

}
