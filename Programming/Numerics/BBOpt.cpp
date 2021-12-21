// Oliver Kullmann, 20.12.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  USAGE:

  EXAMPLES:

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt_debug 10000 30 20 1 /dev/stdin 0
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m8.990s
user	0m8.991s
sys	0m0.001s

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt_debug 10000 30 20 12 /dev/stdin 0
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m16.477s
user	0m18.495s
sys	0m0.716s


Numerics> time echo -e "-4.5 -4.5 -3 4.5 4.5\n-4.5 -4.5 -3 4.5 4.5" | ./BBOpt_debug 10000 30 20 1 /dev/stdin 0
(0.023263908576965331871,-4.5),9.3929483088704549413
real	0m9.064s
user	0m9.066s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 -3 4.5 4.5\n-4.5 -4.5 -3 4.5 4.5" | ./BBOpt_debug 10000 30 20 12 /dev/stdin 0
(0.023263908576965331871,-4.5),9.3929483088704549413
real	0m15.917s
user	0m18.060s
sys	0m0.609s


Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt_debug 10000 30 20 1 /dev/stdin 1
(1.7999999553680419921,0.19999997005462646464),84.000000000000300725
real	0m8.965s
user	0m8.966s
sys	0m0.001s

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt_debug 10000 30 20 12 /dev/stdin 1
(1.7999999553680419921,0.19999997005462646464),84.000000000000300725
real	0m16.127s
user	0m18.457s
sys	0m0.695s

Numerics> time echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt_debug 10000 30 20 1 /dev/stdin 1
(-1.3100751963206599058e-19,-0.99999999961853027375),3.0000000000000000503
real	0m9.137s
user	0m9.120s
sys	0m0.012s

Numerics> time echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt_debug 10000 30 20 12 /dev/stdin 1
(-1.3100751963206599058e-19,-0.99999999961853027375),3.0000000000000000503
real	0m16.135s
user	0m18.490s
sys	0m0.659s


BUGS:

Numerics> echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt_debug 10000 30 20 12 /dev/stdin 1
(-1.3100751963206599058e-19,-0.99999999961853027375),3.0000000000000000503

but

Numerics> echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin 1
Segmentation fault (core dumped)

So for now only running in debug-mode.

*/

#include <iostream>
#include <string>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Optimisation.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "21.12.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/BBOpt.cpp",
        "GPL v3"};

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> "<<proginfo.prg<< " M R S T optFILE [parameters for opt]\n\n"
 ;
    return true;
  }

  struct Func {
    // Beale function https://www.sfu.ca/~ssurjano/beale.html ,
    // global minimum at (3,0.5) -> 0 :
    const Optimisation::function_t f0 = [](const Optimisation::vec_t& v) {
      assert(v.size() == 2);
      using namespace FloatingPoint;
      const float80 x = v[0], y = v[1];
      return sq(1.5L-x+x*y) + sq(2.25L-x+x*sq(y)) + sq(2.625L-x+x*cb(y));
    };
    // Goldstein-Price function https://www.sfu.ca/~ssurjano/goldpr.html ,
    // global minimum at (0,-1) -> 3 :
     const Optimisation::function_t f1 = [](const Optimisation::vec_t& v) {
      assert(v.size() == 2);
      using namespace FloatingPoint;
      const float80 x = v[0], y = v[1];
      return
      (1 + sq(x+y+1) * (19-14*x+3*sq(x)-14*y+6*x*y+3*sq(y))) *
      (30 + sq(2*x-3*y) * (18-32*x+12*sq(x)+48*y-36*x*y+27*sq(y)));
     };


    Optimisation::function_t f;
    void init([[maybe_unused]] const int argc,
              const char* const argv[]) noexcept {
      assert(argc == 1);
      const std::string arg = argv[0];
      if (arg == "0") f = f0; else f = f1;
    }
    Optimisation::y_t func(const Optimisation::vec_t& v) noexcept {
      return f(v);
    }
  };

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  FloatingPoint::fullprec_float80(std::cout);
  std::cout << Optimisation::bbopt_rounds_app(argc, argv, Func()) << "\n";
}
