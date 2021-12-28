// Oliver Kullmann, 20.12.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  USAGE:

  EXAMPLES:

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin 0
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m2.437s
user	0m2.438s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin 0
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m2.445s
user	0m2.446s
sys	0m0.001s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin 0
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m3.498s
user	0m3.980s
sys	0m0.388s

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin 0
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m3.442s
user	0m3.997s
sys	0m0.352s

Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin 0
(0.023263908576965331871,-4.5),9.3929483088704549413
real	0m2.488s
user	0m2.486s
sys	0m0.004s

Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin 0
(0.023263908576965331871,-4.5),9.3929483088704549413
real	0m3.497s
user	0m3.960s
sys	0m0.397s

Numerics> time echo -e "-4.5 -4.5 1 4.5 4.5\n-4.5 -4.5 1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin 0
(-2.5019914169311523438,1.3027376276992034913),0.97098619904576047182
real	0m4.448s
user	0m17.202s
sys	0m1.685s

Numerics> time echo -e "-4.5 -4.5 2 4.5 4.5\n-4.5 -4.5 2 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin 0
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m6.779s
user	0m47.483s
sys	0m4.981s


Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin 1
(1.7999999553680419921,0.19999997005462646464),84.000000000000300725
real	0m2.546s
user	0m2.546s
sys	0m0.001s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin 1
(1.7999999553680419921,0.19999997005462646464),84.000000000000300725
real	0m3.542s
user	0m4.075s
sys	0m0.437s

Numerics> time echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin 1
(-1.3100751963206599058e-19,-0.99999999961853027375),3.0000000000000000503
real	0m2.544s
user	0m2.544s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin 1
(-1.3100751963206599058e-19,-0.99999999961853027375),3.0000000000000000503
real	0m3.546s
user	0m4.083s
sys	0m0.429s

Numerics> time echo -e "-4.5 -4.5 1 4.5 4.5\n-4.5 -4.5 1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin 1
(-2.3491034926689306311e-19,-0.99999999961853027353),3.0000000000000000659
real	0m4.486s
user	0m18.039s
sys	0m1.528s

Numerics> time echo -e "-10 -10 9 10 10\n-10 -10 9 10 10" | ./BBOpt 10000 30 20 12 /dev/stdin 1
(-5.1499516920708510749e-19,-1.0000000000000000007),2.9999999999999999844
real	1m19.356s
user	11m13.812s
sys	0m55.439s


BUGS:

*/

#include <iostream>
#include <string>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Optimisation.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.5.1",
        "28.12.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/BBOpt.cpp",
        "GPL v3"};

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> "<<proginfo.prg<< " M R S T optFILE [parameters for opt]\n\n"
      " - M = partition-size\n"
      " - R = number of rounds (running through all coordinates once)\n"
      " - S = number of shrinkings (on top of the rounds)\n"
      " - T = number of threads\n"
      " - optFILE : contains a line with 5 numbers per coordinate (allows formatting)\n"
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
