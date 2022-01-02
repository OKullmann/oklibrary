// Oliver Kullmann, 20.12.2021 (Swansea)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  USAGE:

  EXAMPLES:

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 0 1000 1000 1 /dev/stdin "" 0
(0,0),14.203125
real	0m0.180s
user	0m0.176s
sys	0m0.004s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 0 1000 1000 1 /dev/stdin "0" 0
(2.9999999999999999993,0.49999999999999999984),5.8774717541114375398e-38
real	0m0.378s
user	0m0.373s
sys	0m0.005s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "" 0
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m2.437s
user	0m2.438s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "0" 0
(2.9999999944298000466,0.49999999845846909008),5.558866614159696022e-18

real	0m3.391s
user	0m3.392s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "" 0
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m2.445s
user	0m2.446s
sys	0m0.001s

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "0" 0
(-3.750572657504313355,1.2175990261172870535),0.81728202994738042176
real	0m3.399s
user	0m3.399s
sys	0m0.000s
Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "1" 0
(2.999999995370998804,0.49999999877908325797),3.5528835712481896098e-18
real	0m3.398s
user	0m3.398s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 0
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m3.498s
user	0m3.980s
sys	0m0.388s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
(2.9999999944298000466,0.49999999845846909008),5.558866614159696022e-18
real	0m4.699s
user	0m5.250s
sys	0m0.217s

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 0
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m3.442s
user	0m3.997s
sys	0m0.352s

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
(-3.7365384226717407658,1.2182842932992600933),0.81850338032673788621
real	0m3.430s
user	0m3.430s
sys	0m0.000s
Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "1" 0
(2.9999999951756807154,0.49999999867330022986),4.1175798878053400249e-18
real	0m3.404s
user	0m3.405s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "" 0
(0.023263908576965331871,-4.5),9.3929483088704549413
real	0m2.488s
user	0m2.486s
sys	0m0.004s

Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "0" 0
(0.023263909013964370279,-4.4999999999999997723),9.3929483088704531658
real	0m3.474s
user	0m3.475s
sys	0m0.000s
Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "20" 0
(0.02326390900919802842,-4.4999999999999999219),9.3929483088704531241
real	0m3.389s
user	0m3.389s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 0
(0.023263908576965331871,-4.5),9.3929483088704549413
real	0m3.497s
user	0m3.960s
sys	0m0.397s

Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
(0.023263909013964370279,-4.4999999999999997723),9.3929483088704531658
real	0m4.698s
user	0m5.184s
sys	0m0.283s

Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "20" 0
(0.02326390900919802842,-4.4999999999999999219),9.3929483088704531241
real	0m4.686s
user	0m5.210s
sys	0m0.306s

Numerics> time echo -e "-4.5 -4.5 1 4.5 4.5\n-4.5 -4.5 1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 0
(-2.5019914169311523438,1.3027376276992034913),0.97098619904576047182
real	0m4.448s
user	0m17.202s
sys	0m1.685s

Numerics> time echo -e "-4.5 -4.5 1 4.5 4.5\n-4.5 -4.5 1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
(-4.4999999999999977253,1.1864290570530033295),0.76206965089231612005
real	0m3.735s
user	0m14.885s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 2 4.5 4.5\n-4.5 -4.5 2 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 0
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m6.779s
user	0m47.483s
sys	0m4.981s

Numerics> time echo -e "-4.5 -4.5 2 4.5 4.5\n-4.5 -4.5 2 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
(-3.480072761827384739,1.2316301025976710268),0.8423486744273232522
real	0m5.271s
user	0m45.609s
sys	0m0.000s
Numerics> time echo -e "-4.5 -4.5 2 4.5 4.5\n-4.5 -4.5 2 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "1" 0
(2.9999999959857859223,0.49999999907559742763),2.6954407021551495249e-18
real	0m5.109s
user	0m43.990s
sys	0m0.000s


Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "" 1
(1.7999999553680419921,0.19999997005462646464),84.000000000000300725
real	0m2.546s
user	0m2.546s
sys	0m0.001s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "0" 1
(1.7999999737464820132,0.1999999828748160792),84.000000000000102654
real	0m3.460s
user	0m3.460s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 1
(1.7999999553680419921,0.19999997005462646464),84.000000000000300725
real	0m3.542s
user	0m4.075s
sys	0m0.437s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 1
(1.7999999737464820132,0.1999999828748160792),84.000000000000102654
real	0m4.785s
user	0m5.457s
sys	0m0.248s

Numerics> time echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "" 1
(-1.3100751963206599058e-19,-0.99999999961853027375),3.0000000000000000503
real	0m2.544s
user	0m2.544s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "0" 1
(-1.2356096963899761264e-10,-1.0000000000763654306),2.999999999999999966
real	0m3.477s
user	0m3.477s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 1
(-1.3100751963206599058e-19,-0.99999999961853027375),3.0000000000000000503
real	0m3.546s
user	0m4.083s
sys	0m0.429s

Numerics> time echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 1
(-1.2356096963899761264e-10,-1.0000000000763654306),2.999999999999999966
real	0m4.729s
user	0m5.377s
sys	0m0.328s

Numerics> time echo -e "-4.5 -4.5 1 4.5 4.5\n-4.5 -4.5 1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 1
(-1.8070024064261784141e-19,-0.99999999961853027353),3.0000000000000000659
real	0m2.753s
user	0m10.944s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 1 4.5 4.5\n-4.5 -4.5 1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 1
(2.3779029580190141691,0.57860198575223903303),183.83119303446519176
real	0m3.829s
user	0m15.221s
sys	0m0.000s
Numerics> time echo -e "-4.5 -4.5 1 4.5 4.5\n-4.5 -4.5 1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "2" 1
(-3.0719426545660699433,-3.0450466614003354083),17055.473852189535046
real	0m3.779s
user	0m15.093s
sys	0m0.000s

Numerics> time echo -e "-10 -10 9 10 10\n-10 -10 9 10 10" | ./BBOpt 10000 30 20 12 /dev/stdin "" 1
(-5.1499516920708510749e-19,-1.0000000000000000007),2.9999999999999999844
real	0m39.121s
user	6m24.722s
sys	0m0.012s

Numerics> time echo -e "-10 -10 9 10 10\n-10 -10 9 10 10" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 1
(1.8000000488356253292,0.20000003377648644698),84.000000000000382451
real	0m57.233s
user	9m5.433s
sys	0m0.020s
Numerics> time echo -e "-10 -10 9 10 10\n-10 -10 9 10 10" | ./BBOpt 10000 30 20 12 /dev/stdin "1" 1
(1.8000000535460977032,0.20000003559951966956),84.0000000000004294
real	0m57.493s
user	9m7.023s
sys	0m0.020s
Numerics> time echo -e "-10 -10 9 10 10\n-10 -10 9 10 10" | ./BBOpt 100 100 20 12 /dev/stdin "0" 1
(3.7558884747816771266e-09,-0.99999999728495941062),3.0000000000000045276
real	0m1.651s
user	0m15.150s
sys	0m0.000s
Numerics> time echo -e "-10 -10 9 10 10\n-10 -10 9 10 10" | ./BBOpt 100 100 20 12 /dev/stdin "1" 1
(1.8000011131121099098,0.20000075674461296895),84.000000000190756036
real	0m1.662s
user	0m14.890s
sys	0m0.001s


BUGS:

*/

#include <iostream>
#include <string>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Optimisation.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.7.1",
        "2.1.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/BBOpt.cpp",
        "GPL v3"};

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> "<<proginfo.prg<< " M R S T optFILE seeds [parameters for opt]\n\n"
      " - M = partition-size >= 0\n"
      " - R = number of rounds (running through all coordinates once) >= 1\n"
      " - S = number of shrinkings (on top of the rounds) >= 1\n"
      " - T = number of threads >= 1\n"
      " - optFILE : contains a line with 5 numbers per coordinate (allows formatting)\n"
      " - seeds   : either empty string or seed-sequence\n"
      " - all following arguments are passed to the functor to be optimised.\n"
 ;
    return true;
  }

  // Prototype:
  struct Func_triv {
    void init([[maybe_unused]] const int, const char* const []) noexcept {}
    Optimisation::y_t func(const Optimisation::vec_t&) noexcept {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      return 0;
    }
  };

  struct Func {
    typedef Optimisation::x_t x_t;
    typedef Optimisation::y_t y_t;
    typedef Optimisation::vec_t vec_t;
    typedef std::function<y_t(const vec_t&)> f_t;

    // Beale function https://www.sfu.ca/~ssurjano/beale.html ,
    // global minimum at (3,0.5) -> 0 :
    const f_t f0 = [](const vec_t& v) {
      assert(v.size() == 2);
      using namespace FloatingPoint;
      const float80 x = v[0], y = v[1];
      return sq(1.5L-x+x*y) + sq(2.25L-x+x*sq(y)) + sq(2.625L-x+x*cb(y));
    };
    // Goldstein-Price function https://www.sfu.ca/~ssurjano/goldpr.html ,
    // global minimum at (0,-1) -> 3 :
     const f_t f1 = [](const vec_t& v) {
      assert(v.size() == 2);
      using namespace FloatingPoint;
      const float80 x = v[0], y = v[1];
      return
        (1 + sq(x+y+1) * (19-14*x+3*sq(x)-14*y+6*x*y+3*sq(y))) *
        (30 + sq(2*x-3*y) * (18-32*x+12*sq(x)+48*y-36*x*y+27*sq(y)));
     };

    f_t f;
    void init([[maybe_unused]] const int argc,
              const char* const argv[]) noexcept {
      assert(argc == 1);
      const std::string arg = argv[0];
      if (arg == "0") f = f0; else f = f1;
    }
    Optimisation::y_t func(const vec_t& v, y_t) noexcept {
      return f(v);
    }
  };

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  FloatingPoint::fullprec_float80(std::cout);
  const auto result = Optimisation::bbopt_rounds_app(argc, argv, Func());
  std::cout << result << "\n";
}
