// Oliver Kullmann, 19.1.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Simple measurement of tau-runtimes



TODOS:

0. Spending a few hours with the optimisation options of gcc, trying
to improve the situation.
 - starting with the used options, seeing whether removing them makes
   a difference;
 - then considering https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html ,
   whether there are other options useful here.
It makes sense to have a section in the dissertation on these issues.

Actually it is easy to run a proper exhaustive search:
  1. A C++ program OptGCC.cpp, reads from a file the possible options,
     one per line; let K be their number.
  2. Each option is independently just one of off.
  3. There is a script Compile, which is given the list of additional options,
     performs the compilation, and returns the name of the produced executable.
  4. Another script RunTime, runs the executable and returns the produced
     user time.
  5. OptGCC runs through the list of all combinations of the options;
     simplest to do this via binary counting.
  6. It compiles the program via Compile, and runs it N times (another
     parameter), outputs the results to a file.
  7. The min-time of the N runs is computed, and the total result is the
     sorted list of the first M combinations (together with their times).
  8. For the system calls we use std::system; the return-value is the
     error code. Outputs of the programs are stored in files, say,
     CompileOutput and RunTimeOutput, and read into a string.
  9. The output enumerates the results, from 1 to 2^k. Another optional
     parameter is the start value for the option (default 1), in order
     to simply restart the process if it was interrupted.

The build-environment:
 - The new tools given by Random/Environment.hpp should be employed.
 - An appropriate run-time model needs to be established; the basic
   assumption is linearity in N, given that all other parameters are
   the same.
 - On the basic of that model (for a given computer and fixed compilation),
   a reasonable run-time to extracting the parameters needs to be established.
 - For working with different compiler-options, the debug-version is to be
   used; to give examples (on csltok):

GenericSAT> rm MeasureTau_debug
GenericSAT> make MeasureTau_debug
g++ --std=c++17 -pedantic -fmax-errors=5 -Wall -Wextra -I ../../../Generators -I ../../../../../Programming -g -D_GLIBCXX_DEBUG   MeasureTau.cpp -o MeasureTau_debug
GenericSAT> time ./MeasureTau_debug 80 1e1 1e1 1e7 2e6
15.186108805481841686
real    0m8.390s
user    0m8.366s
sys     0m0.003s
GenericSAT> time ./MeasureTau 80 1e1 1e1 1e7 2e6
15.186108805481841686
real    0m7.596s
user    0m7.557s
sys     0m0.019s

We see that the debug-version is only somewhat slower, which should indicate
some problems. Without any options:
GenericSAT> rm MeasureTau_debug
GenericSAT> make Debug_options="" MeasureTau_debug
g++ --std=c++17 -pedantic -fmax-errors=5 -Wall -Wextra -I ../../../Generators -I ../../../../../Programming    MeasureTau.cpp -o MeasureTau_debug
GenericSAT> time ./MeasureTau_debug 80 1e1 1e1 1e7 2e6
15.186108805481841686
real    0m8.398s
user    0m8.376s
sys     0m0.001s

No difference visible; a good (minimal) set of optimisation-options seems:
GenericSAT> rm MeasureTau_debug
GenericSAT> make Debug_options="-Ofast -DNDEBUG -fno-finite-math-only" MeasureTau_debug
g++ --std=c++17 -pedantic -fmax-errors=5 -Wall -Wextra -I ../../../Generators -I ../../../../../Programming -Ofast -DNDEBUG -fno-finite-math-only   MeasureTau.cpp -o MeasureTau_debug
GenericSAT> time ./MeasureTau_debug 80 1e1 1e1 1e7 2e6
15.186108805481841686
real    0m6.650s
user    0m6.632s
sys     0m0.001s

If we go for all current optimisation-options in the makefile, plus enabling
handling of infinity (for the static-asserts), then the run-time gets worse:
GenericSAT> rm MeasureTau_debug
GenericSAT> make Debug_options="-Ofast -DNDEBUG -fstrict-aliasing -funroll-loops -fvariable-expansion-in-unroller -floop-nest-optimize -fgraphite-identity --param graphite-max-nb-scop-params=0 -march=native -fwhole-program -static -fno-finite-math-only" MeasureTau_debug
g++ --std=c++17 -pedantic -fmax-errors=5 -Wall -Wextra -I ../../../Generators -I ../../../../../Programming -Ofast -DNDEBUG -fstrict-aliasing -funroll-loops -fvariable-expansion-in-unroller -floop-nest-optimize -fgraphite-identity --param graphite-max-nb-scop-params=0 -march=native -fwhole-program -static -fno-finite-math-only   MeasureTau.cpp -o MeasureTau_debug
GenericSAT> time ./MeasureTau_debug 80 1e1 1e1 1e7 2e6
15.186108805481841686
real    0m7.339s
user    0m7.297s
sys     0m0.023s

One needs a full approach, testing all combinations; but a first guess is that
"-fwhole-program" is harmful:

First repeating the above measurement for the current optimised version:
GenericSAT> rm MeasureTau
GenericSAT> make MeasureTau
g++ --std=c++17 -pedantic -fmax-errors=5 -Wall -Wextra -I ../../../Generators -I ../../../../../Programming -Ofast -DNDEBUG -fstrict-aliasing -funroll-loops -fvariable-expansion-in-unroller -floop-nest-optimize -fgraphite-identity --param graphite-max-nb-scop-params=0 -march=native -fwhole-program -static  -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math   MeasureTau.cpp -o MeasureTau
GenericSAT> time ./MeasureTau 80 1e1 1e1 1e7 2e6
15.186108805481841686
real    0m7.581s
user    0m7.539s
sys     0m0.022s

Now removing the whole-program options:
GenericSAT> rm MeasureTau
GenericSAT> make CXXFLAGS="-fno-whole-program" MeasureTau
g++ --std=c++17 -pedantic -fmax-errors=5 -Wall -Wextra -I ../../../Generators -I ../../../../../Programming -Ofast -DNDEBUG -fstrict-aliasing -funroll-loops -fvariable-expansion-in-unroller -floop-nest-optimize -fgraphite-identity --param graphite-max-nb-scop-params=0 -march=native -fwhole-program -static  -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math  -fno-whole-program MeasureTau.cpp -o MeasureTau
GenericSAT> time ./MeasureTau 80 1e1 1e1 1e7 2e6
15.186108805481841686
real    0m6.975s
user    0m6.956s
sys     0m0.001s

One sees a significant improvement; perhaps the further disabling of
floating-point optimisation causes the further slowdown.

The tool for running all combinations should show all of that.


1. Basic measurements using parameters
     64/80 1e1 1e1 1e7 1e10

csltok:
T> time ./MeasureTau 64 1e1 1e1 1e7 1e10
75750.154509474588849
real    170m59.247s
user    170m10.173s
sys     0m18.904s
> time ./MeasureTau 80 1e1 1e1 1e7 1e10
75750.154509474586924
real    582m19.229s
user    580m10.964s
sys     0m34.342s
The above use gcc-7.4.0; now with 8.3.0:
> time ./MeasureTau 64 1e1 1e1 1e7 1e10
time ./MeasureTau 80 1e1 1e1 1e7 1e10
75750.154509474588849
real    166m51.825s
user    166m12.361s
sys     0m13.593s
Seems basically the same.
> time ./MeasureTau 80 1e1 1e1 1e7 1e10
75750.154509474586924
real    642m6.186s
user    627m24.894s
sys     0m27.920s
Worse (as with cs-wsok).

Comparison (gcc-7.4.0):
> time ./ExploreBTs +1e10
real    521m9.398s
user    521m35.421s
sys     1m7.061s
> time ./ExploreBTs 1e10
real    1622m26.857s
user    1615m34.799s
sys     2m14.001s


cs-wsok:
> time ./MeasureTau 64 1e1 1e1 1e7 1e10
75750.154509474588849
real    171m14.834s
user    171m8.581s
sys     0m0.002s
> time ./MeasureTau 80 1e1 1e1 1e7 1e10
75750.154509474586924
real    499m26.466s
user    499m7.987s
sys     0m0.011s
The above use gcc-7.4.0; now with 8.3.0:
> time ./MeasureTau 64 1e1 1e1 1e7 1e10
time ./MeasureTau 80 1e1 1e1 1e7 1e10
75750.154509474588849
real    172m37.786s
user    172m31.409s
sys     0m0.034s
Seems basically the same.
> time ./MeasureTau 80 1e1 1e1 1e7 1e10
75750.154509474586924
real    588m56.140s
user    588m34.282s
sys     0m0.083s
This is now considerably worse (similar to csltok above, but even worse;
could be mis-compilation, or striving for higher precision).

Comparison (gcc-7.4.0):
> time ./ExploreBTs +1e10
real    526m53.909s
user    526m33.472s
sys     0m0.919s
> time ./ExploreBTs 1e10
real    1384m48.796s
user    1383m56.399s
sys     0m0.703s


csverify:
$ time ./MeasureTau 64 1e1 1e1 1e7 1e10
75750.154509474588849
real    32m42.582s
user    32m42.582s
sys     0m0.004s
$ time ./MeasureTau 80 1e1 1e1 1e7 1e10
75750.154509474586924
real    516m20.281s
user    516m20.301s
sys     0m0.000s

Testing whether the parameter-range plays a big role:
$ time ./MeasureTau 64 1e9 1e9 1e16 1e10
0.00010440971844115158531
real    32m31.131s
user    32m31.135s
sys     0m0.000s
$ time ./MeasureTau 80 1e9 1e9 1e16 1e10
0.00010440971844115158293
real    474m48.245s
user    474m48.268s
sys     0m0.000s

Comparison:
$ time ./ExploreBTs +1e10
real    98m7.253s
user    98m7.253s
sys     0m0.000s
$ time ./ExploreBTs 1e10
real    1432m49.335s
user    1432m49.322s
sys     0m0.000s


cs2-irfan536:
$ time ./MeasureTau 64 1e1 1e1 1e7 1e10
75750.154509474588849
real    38m42.073s
user    38m41.103s
sys     0m0.308s
$ time ./MeasureTau 80 1e1 1e1 1e7 1e10
75750.154509474586924
real    534m44.544s
user    534m40.675s
sys     0m0.476s
The above use gcc-7.4.0; now with 8.3.0:
$ time ./MeasureTau 64 1e1 1e1 1e7 1e10
75750.154509474588849
real    39m0.878s
user    39m0.344s
sys     0m0.500s
Seems basically the same.
$ time ./MeasureTau 80 1e1 1e1 1e7 1e10
75750.154509474586924
real    625m9.900s
user    625m7.410s
sys     0m2.476s
Considerably worse similar to cs-wsok above.

Comparison (gcc-7.4.0):
$ time ./ExploreBTs +1e10
real	114m11.254s
user	114m10.984s
sys	0m0.128s
$ time ./ExploreBTs 1e10
real	1490m30.361s
user	1490m15.300s
sys	0m2.912s



Final update:

csltok2 (gcc 10.1.0):
> time ./MeasureTau 64 1e1 1e1 1e7 1e10
75750.154509474588849
real	31m41.864s
user	31m41.865s
sys	0m0.001s

csverify (gcc 9.3.0):
XXX

cs-kullmann-server (gcc 10.2.0):
> time ./MeasureTau 64 1e1 1e1 1e7 1e10
75750.154509474588849
real    41m16.495s
user    41m16.382s
sys     0m0.005s
> time ./MeasureTau 80 1e1 1e1 1e7 1e10
XXX


*/

#include <iostream>
#include <string>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>

#include "BranchingTuples.hpp"

namespace {

  const std::string version = "0.0.2";
  const std::string date = "2.3.2019";
  const std::string program = "MeasureTau"
#ifndef NDEBUG
  "_debug"
#endif
;
  const std::string error = "ERROR[" + program + "]: ";

  namespace FP = FloatingPoint;

  void show_usage() noexcept {
    std::cout << "Arguments:" << " digits a b_begin b_end b_steps\n";
  }

}

int main(const int argc, const char* const argv[]) {
  if (argc == 1) { show_usage(); return 0; }
  if (argc != 6) {
    std::cerr << error << "0 or five arguments needed.\n";
    return 1;
  }
  const std::string form = argv[1];
  if (form != "64" and form != "80") {
    std::cerr << error << "form must be \"64\" or \"80\".\n";
    return 1;
  }
  const bool use_double = form == "64";
  const FP::float80 a = std::stold(argv[2]),
    b_begin = std::stold(argv[3]), b_end = std::stold(argv[4]), b_steps_0 = std::stold(argv[5]);
  const FP::UInt_t b_steps = b_steps_0;
  assert(b_steps < FP::P264m1);
  const FP::float80 delta = (b_end - b_begin) / b_steps;
  if (delta == 0) {
    std::cerr << error << "delta=0.\n";
    return 1;
  }
  FP::float80 sum = 0;
  for (FP::UInt_t i = 0; i <= b_steps; ++i) {
    const FP::float80 b = b_begin + i * delta;
    sum += (use_double) ? BranchingTuples::ltau_d(a,b) : BranchingTuples::ltau(a,b);
  }
  std::cout << FP::Wrap(sum) << "\n";
}
