// Oleg Zaikin, 22.12.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  USAGE:

  EXAMPLES:

MOLS> cat data/weights/testN6 | ./Euler_BBOpt_debug 1 1 1 1 data/weights/ParaN6 val dom
  (1,100,100,100),78


More precisely:

MOLS> cat data/weights/testN6 | /usr/bin/time ./Euler_BBOpt_debug 1 1 1 1 data/weights/ParaN6 val dom
(1,100,100,100),78
18.56user 0.54system 0:19.11elapsed 100%CPU (0avgtext+0avgdata 69928maxresident)k
0inputs+0outputs (0major+16693minor)pagefaults 0swaps

With output of SearchStats:

MOLS> cat data/weights/testN6 | ./Euler_BBOpt 1 1 1 1 data/weights/ParaN6 val dom
2 3 4 5 6 2 12 6 la val one eager prun dom 0.6552 0 253 110 87 14 143 143 0 342 253 4580 46145 0.6511 0.0164 0.1196 0.0000 Euler_BBOpt 0.2.7
1 3 4 5 6 2 12 6 la val one eager prun dom 0.5589 0 221 94 73 10 127 127 0 326 221 3876 38288 0.5552 0.0148 0.0991 0.0000 Euler_BBOpt 0.2.7
100 3 4 5 6 2 12 6 la val one eager prun dom 2.6171 0 1182 390 189 1 792 792 0 1176 1182 25374 182800 2.5993 0.0705 0.4409 0.0000 Euler_BBOpt 0.2.7
1 1 4 5 6 2 12 6 la val one eager prun dom 0.8396 0 369 145 100 12 224 224 0 456 369 6736 58371 0.8337 0.0212 0.1503 0.0000 Euler_BBOpt 0.2.7
1 100 4 5 6 2 12 6 la val one eager prun dom 0.8421 0 223 100 81 16 123 123 0 327 223 4780 57100 0.8383 0.0241 0.1523 0.0000 Euler_BBOpt 0.2.7
1 100 1 5 6 2 12 6 la val one eager prun dom 0.8310 0 218 98 80 15 120 120 0 318 218 4792 57181 0.8272 0.0251 0.1501 0.0000 Euler_BBOpt 0.2.7
1 100 100 5 6 2 12 6 la val one eager prun dom 0.6503 0 204 92 76 13 112 112 0 319 204 3991 44966 0.6471 0.0153 0.1182 0.0000 Euler_BBOpt 0.2.7
1 100 100 1 6 2 12 6 la val one eager prun dom 0.6638 0 216 98 82 13 118 118 0 320 216 4134 46050 0.6604 0.0164 0.1207 0.0000 Euler_BBOpt 0.2.7
1 100 100 100 6 2 12 6 la val one eager prun dom 0.4730 0 132 54 37 11 78 78 0 227 132 2230 30844 0.4706 0.0102 0.0823 0.0000 Euler_BBOpt 0.2.7
(1.0000,100.0000,100.0000,100.0000),78.0000

   Remark: In debug-mode the first line is printed two more times, due to
   asserts checking the result.


TODOS:

0. Output of basic data (like version number).

1. Using optionally an alternative measure, like the number of reductions,
   for optimisation:
    - sbps seems to relate closely to the time, and thus should be a
      relevant alternative measure to be optimsed.

2. There is an enormous code-duplication, which needs to be cleaned-up.

BUGS:

1. Non-parallisable

MOLS> cat data/weights/testN6 | ./Euler_BBOpt_debug 1 1 1 2 data/weights/ParaN6 val dom
Segmentation fault (core dumped)

All the global variables need to be removed; see Lookahead.hpp (it seems
there is only "global_stat").

*/


#include <iostream>
#include <string>
#include <memory>
#include <tuple>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <SystemSpecifics/Timing.hpp>
#include <Numerics/NumTypes.hpp>
#include <Numerics/NumInOut.hpp>
#include <Numerics/Optimisation.hpp>
#include <Transformers/Generators/Random/LatinSquares.hpp>

#include <Solvers/Gecode/Lookahead.hpp>
#include <Solvers/Gecode/Statistics.hpp>

#include "Euler.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.10",
        "30.12.2021",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/Euler_BBOpt.cpp",
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
      " algorithmic-options : " << Environment::WRP<Lookahead::BrTypeO>{} << "\n" <<
      "                     : " << Environment::WRP<Lookahead::BrSourceO>{} << "\n" <<
      "                     : " << Environment::WRP<Lookahead::BrSolutionO>{} << "\n" <<
      "                     : " << Environment::WRP<Lookahead::BrPruneO>{} << "\n" <<
      "                     : " << Environment::WRP<Lookahead::UpperBoundO>{} << "\n" <<
      " propagation-level   : " << Environment::WRP<Euler::PropO>{} << "\n" <<
      " - .\n"
 ;
    return true;
  }

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  struct Func {
    typedef Optimisation::y_t y_t;
    typedef Optimisation::vec_t vec_t;

    LatinSquares::ls_dim_t N, k, m1, m2;
    Lookahead::option_t alg_options;
    Euler::gecode_option_t gecode_options;
    Euler::gecode_intvec_t ls1_partial;
    Euler::gecode_intvec_t ls2_partial;
    const Timing::UserTime timing;

    Lookahead::UpperBoundO ub;

    void init(const int argc, const char* const argv[]) {
      Environment::Index index;

      // Read algorithm options:
      alg_options = argc <= index ? Lookahead::option_t{} :
        Environment::translate<Lookahead::option_t>()(argv[index-1],
                                                      Lookahead::sep);
        ub = std::get<Lookahead::UpperBoundO>(alg_options);

      // Read gecode options:
      gecode_options = argc <= index ?
        Euler::gecode_option_t{Euler::PropO::dom} :
        Environment::translate<Euler::gecode_option_t>()(argv[index++],
                                                         Lookahead::sep);
      N = Euler::read_N(error);
      k = Euler::read_k(error);
      assert(N > 0 and k > 0);
      ls1_partial = Euler::read_partial_ls(N);
      ls2_partial = Euler::read_partial_ls(N);
      assert(not ls1_partial.empty() and not ls2_partial.empty());
      m1 = Euler::given_cells(ls1_partial);
      m2 = Euler::given_cells(ls2_partial);
    }

    y_t func(const vec_t& v, const y_t b) const noexcept {
      assert(not v.empty());
      assert(v.size() == N-2);
      assert(b >= 0);

      const bool with_bound = (ub == Lookahead::UpperBoundO::upperbound) and
        not FloatingPoint::isinf(b);
      assert(not with_bound or FloatingPoint::is_int(b));

      const std::shared_ptr<Euler::TWO_MOLS> p(new Euler::TWO_MOLS(N,
        alg_options, gecode_options, ls1_partial, ls2_partial, &v));
      const Timing::Time_point t1 = timing();

      // Limit the maximal number of leaves if specified in options
      // (f limit is 0, then it is not applied):
      const int maxunsatlvs = with_bound ? b : 0;

      const Statistics::SearchStat stat =
        Lookahead::solve<Euler::TWO_MOLS>(p, false, maxunsatlvs);

      const Timing::Time_point t2 = timing();
      const double solving_time = t2 - t1;
      assert(p.use_count() == 1);
      for (const auto x : v) std::cerr << x << " ";
      const Environment::RegistrationPolicies<Lookahead::UpperBoundO> rp_ub;
      const std::string sub = rp_ub.string[int(ub)];
      std::cerr << sub << " ";
      Euler::print_stat(N, k, m1, m2, 0, solving_time, alg_options,
                        gecode_options, stat, proginfo);
      const auto leaves = stat.solutions + stat.unsat_leaves;
      return leaves;
    }
  };

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const auto result = Optimisation::bbopt_rounds_app(argc, argv, Func());
  FloatingPoint::fullprec_float80(std::cout);
  std::cout << result << "\n";
}
