// Oleg Zaikin, 22.12.2021 (Swansea)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  USAGE:

  EXAMPLES:

MOLS> cat data/Weights/testN6 | time ./EulerBBOpt 1 1 1 1 data/Weights/ParaN6 "" val dom
2 3 4 5 upbnd 6 2 12 6 la val one eager prun dom 0.6552 0 253 110 87 14 143 143 0 342 253 4580 46145 0.6513 0.0171 0.1181 0.0000 EulerBBOpt 0.4.2
1 3 4 5 upbnd 6 2 12 6 la val one eager prun dom 0.5507 0 221 94 73 10 127 127 0 326 221 3876 38288 0.5473 0.0143 0.0956 0.0000 EulerBBOpt 0.4.2
100 3 4 5 upbnd 6 2 12 6 la val one eager prun dom 0.4692 0 194 66 34 0 128 128 0 195 194 4338 31676 0.4663 0.0139 0.0779 0.0000 EulerBBOpt 0.4.2
1 1 4 5 upbnd 6 2 12 6 la val one eager prun dom 0.5354 0 205 77 45 6 128 128 0 208 205 4263 35145 0.5319 0.0136 0.0912 0.0000 EulerBBOpt 0.4.2
1 100 4 5 upbnd 6 2 12 6 la val one eager prun dom 0.8953 0 223 100 81 16 123 123 0 327 223 4780 57100 0.8914 0.0258 0.1576 0.0000 EulerBBOpt 0.4.2
1 100 1 5 upbnd 6 2 12 6 la val one eager prun dom 0.9105 0 218 98 80 15 120 120 0 318 218 4792 57181 0.9067 0.0257 0.1609 0.0000 EulerBBOpt 0.4.2
1 100 100 5 upbnd 6 2 12 6 la val one eager prun dom 0.7175 0 204 92 76 13 112 112 0 319 204 3991 44966 0.7140 0.0169 0.1273 0.0000 EulerBBOpt 0.4.2
1 100 100 1 upbnd 6 2 12 6 la val one eager prun dom 0.6777 0 209 96 81 12 113 113 0 305 209 3972 43254 0.6746 0.0164 0.1207 0.0000 EulerBBOpt 0.4.2
1 100 100 100 upbnd 6 2 12 6 la val one eager prun dom 0.5002 0 132 54 37 11 78 78 0 227 132 2230 30844 0.4979 0.0104 0.0845 0.0000 EulerBBOpt 0.4.2
(1.00000000000000000000,100.00000000000000000000,100.00000000000000000000,100.00000000000000000000),78.00000000000000000000
5.91user 0.25system 0:06.17elapsed 99%CPU (0avgtext+0avgdata 6796maxresident)k
0inputs+0outputs (0major+497minor)pagefaults 0swaps

Randomised:

MOLS> cat data/Weights/testN6 | time ./EulerBBOpt 1 1 1 1 data/Weights/ParaN6 "0" val dom
2 3 4 5 upbnd 6 2 12 6 la val one eager prun dom 0.6670 0 253 110 87 14 143 143 0 342 253 4580 46145 0.6629 0.0170 0.1204 0.0000 EulerBBOpt 0.4.2
5.48156 3 4 5 upbnd 6 2 12 6 la val one eager prun dom 0.4989 0 214 70 30 3 144 144 0 201 214 4506 32849 0.4952 0.0130 0.0844 0.0000 EulerBBOpt 0.4.2
18.1204 3 4 5 upbnd 6 2 12 6 la val one eager prun dom 0.5357 0 219 75 39 0 144 144 0 222 219 4918 35839 0.5323 0.0151 0.0908 0.0000 EulerBBOpt 0.4.2
2 25.8732 4 5 upbnd 6 2 12 6 la val one eager prun dom 0.7960 0 241 107 85 18 134 134 0 372 241 4534 54551 0.7923 0.0194 0.1443 0.0000 EulerBBOpt 0.4.2
2 33.8323 4 5 upbnd 6 2 12 6 la val one eager prun dom 0.8184 0 242 107 84 19 135 135 0 373 242 4538 55069 0.8146 0.0201 0.1480 0.0000 EulerBBOpt 0.4.2
2 25.8732 77.0925 5 upbnd 6 2 12 6 la val one eager prun dom 0.5163 0 177 76 61 6 101 101 0 314 177 2882 34123 0.5132 0.0107 0.0923 0.0000 EulerBBOpt 0.4.2
2 25.8732 92.0341 5 upbnd 6 2 12 6 la val one eager prun dom 0.5147 0 175 74 59 4 101 101 0 316 175 2702 33406 0.5116 0.0093 0.0928 0.0000 EulerBBOpt 0.4.2
2 25.8732 77.0925 52.9923 upbnd 6 2 12 6 la val one eager prun dom 0.4740 0 182 80 63 8 102 102 0 226 182 3345 33113 0.4710 0.0118 0.0859 0.0000 EulerBBOpt 0.4.2
2 25.8732 77.0925 92.8759 upbnd 6 2 12 6 la val one eager prun dom 0.4886 0 182 80 63 8 102 102 0 226 182 3345 33113 0.4857 0.0116 0.0888 0.0000 EulerBBOpt 0.4.2
(2.00000000000000000000,25.87315350341806316839,77.09253808027562152733,5.00000000000000000000),101.00000000000000000000
5.31user 0.20system 0:05.51elapsed 100%CPU (0avgtext+0avgdata 6936maxresident)k
0inputs+0outputs (0major+393minor)pagefaults 0swaps

With specified measure:

MOLS$ cat ./data/Weights/testN6 | time ./Euler_BBOpt 1 1 1 1 ./data/Weights/ParaN6_best "" val dom lvs
120 0.1 150 150 upbnd lvs 6 2 12 6 la val one eager prun dom 1.6524 0 759 248 0 113 9 511 511 0 323 759 14841 112404 1.6402 0.0422 0.2818 0.0000 Euler_BBOpt 0.5.0
0.1 0.1 150 150 upbnd lvs 6 2 12 6 la val one eager prun dom 0.6080 0 246 98 0 69 9 148 148 0 229 246 4091 40750 0.6034 0.0162 0.1090 0.0000 Euler_BBOpt 0.5.0
300 0.1 150 150 upbnd lvs 6 2 12 6 la val one eager prun dom 0.5400 0 227 78 0 38 0 149 149 0 55 227 5093 36677 0.5364 0.0157 0.0910 0.0000 Euler_BBOpt 0.5.0
0.1 0.01 150 150 upbnd lvs 6 2 12 6 la val one eager prun dom 0.5784 0 246 98 0 69 9 148 148 0 229 246 4091 40750 0.5744 0.0149 0.1035 0.0000 Euler_BBOpt 0.5.0
0.1 300 150 150 upbnd lvs 6 2 12 6 la val one eager prun dom 0.7343 0 222 97 0 74 19 125 125 0 239 222 4155 50015 0.7309 0.0163 0.1332 0.0000 Euler_BBOpt 0.5.0
0.1 300 0.1 150 upbnd lvs 6 2 12 6 la val one eager prun dom 0.7623 0 227 101 0 78 19 126 126 0 233 227 4332 52935 0.7590 0.0212 0.1370 0.0000 Euler_BBOpt 0.5.0
0.1 300 300 150 upbnd lvs 6 2 12 6 la val one eager prun dom 0.4816 0 131 53 0 35 12 78 78 0 149 131 2202 30686 0.4793 0.0104 0.0827 0.0000 Euler_BBOpt 0.5.0
0.1 300 300 0.1 upbnd lvs 6 2 12 6 la val one eager prun dom 0.5069 0 145 66 0 55 8 79 79 0 132 145 2787 33737 0.5046 0.0114 0.0911 0.0000 Euler_BBOpt 0.5.0
0.1 300 300 300 upbnd lvs 6 2 12 6 la val one eager prun dom 0.4824 0 131 53 0 35 12 78 78 0 149 131 2202 30686 0.4800 0.0100 0.0831 0.0000 Euler_BBOpt 0.5.0
(0.10000000000000000000,300.00000000000000000000,300.00000000000000000000,150.00000000000000000000),78.00000000000000000000
6.34user 0.27system 0:06.62elapsed 99%CPU (0avgtext+0avgdata 6788maxresident)k
0inputs+0outputs (0major+567minor)pagefaults 0swaps

MOLS$ cat ./data/Weights/testN6 | time ./Euler_BBOpt 1 1 1 1 ./data/Weights/ParaN6_best "" val dom laprp
120 0.1 150 150 upbnd laprp 6 2 12 6 la val one eager prun dom 1.6605 0 759 248 0 113 9 511 511 0 323 759 14841 112404 1.6477 0.0425 0.2935 0.0000 Euler_BBOpt 0.5.0
0.1 0.1 150 150 upbnd laprp 6 2 12 6 la val one eager prun dom 0.6305 0 246 98 0 69 9 148 148 0 229 246 4091 40750 0.6258 0.0158 0.1175 0.0000 Euler_BBOpt 0.5.0
300 0.1 150 150 upbnd laprp 6 2 12 6 la val one eager prun dom 2.6386 0 1178 386 0 183 1 792 792 0 381 1178 24829 179101 2.6207 0.0711 0.4619 0.0000 Euler_BBOpt 0.5.0
0.1 0.01 150 150 upbnd laprp 6 2 12 6 la val one eager prun dom 0.5997 0 246 98 0 69 9 148 148 0 229 246 4091 40750 0.5955 0.0160 0.1131 0.0000 Euler_BBOpt 0.5.0
0.1 300 150 150 upbnd laprp 6 2 12 6 la val one eager prun dom 0.7496 0 222 97 0 74 19 125 125 0 239 222 4155 50015 0.7460 0.0176 0.1413 0.0000 Euler_BBOpt 0.5.0
0.1 0.01 0.1 150 upbnd laprp 6 2 12 6 la val one eager prun dom 2.4247 0 1146 354 0 120 31 792 792 0 381 1146 22567 164124 2.4066 0.0548 0.4268 0.0000 Euler_BBOpt 0.5.0
0.1 0.01 300 150 upbnd laprp 6 2 12 6 la val one eager prun dom 0.6204 0 243 97 0 68 10 146 146 0 234 243 4083 40808 0.6159 0.0149 0.1165 0.0000 Euler_BBOpt 0.5.0
0.1 0.01 150 0.1 upbnd laprp 6 2 12 6 la val one eager prun dom 0.4806 0 172 71 0 55 3 101 101 0 223 172 2672 30592 0.4770 0.0112 0.0886 0.0000 Euler_BBOpt 0.5.0
0.1 0.01 150 300 upbnd laprp 6 2 12 6 la val one eager prun dom 0.6084 0 246 98 0 69 9 148 148 0 229 246 4091 40750 0.6038 0.0157 0.1147 0.0000 Euler_BBOpt 0.5.0
(0.10000000000000000000,0.01000000000000000000,150.00000000000000000000,0.10000000000000000000),30592.00000000000000000000
10.42user 0.44system 0:10.96elapsed 99%CPU (0avgtext+0avgdata 6856maxresident)k
0inputs+0outputs (0major+473minor)pagefaults 0swaps


   Remark: In debug-mode the first line is printed two more times, due to
   asserts checking the result.


TODOS:

0. Output of basic data (like version number).

1. Using optionally an alternative measure, like the number of reductions,
   for optimisation OZ (needed for paper):
    - DONE sbps seems to relate closely to the time, and thus should be a
      relevant alternative measure to be optimsed.
    - The total number of propagations seems to be provided by Gecode,
      and could be useful.

2. Implement the four orderings of branchings (given, reverse given,
   descending distance, ascending distance) OZ (needed for paper).

3. There is an enormous code-duplication, which needs to be cleaned-up.

4. Introduce "scaling-factor" sfactor (float80):
  - For depth d >= 0 in the backtracking tree, the weights for domain-sizes
    >= 2 are to be multiplied with sfactor^d.
   - Since to know the current depth is useful, easiest to introduce the
     local variable depth (0 at the root), and then at the beginning of the
     determination of the best branching to compute the current scaling factor
     sfaccurr = pow(sfactor, depth).
   - For sfactor=1 nothing is changed; the expectation is that sfactor < 1
     is useful, making the elimination of variables more important towards
     the leaves.
   - For the optimisation, first we start with the given weights and only
     optimise sfactor; later we make a global optimisation.


BUGS:

1. Non-parallisable

   A local memory-allocator is needed.

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

namespace EulerBBOpt {

  // Type of measure for optimising the weights.
  // lvs   : leaves in the backtracking tree
  // laprp : number of calls to the look-ahead propagation-function
  enum class MeasureO {lvs=0, laprp=1};

  const Environment::ProgramInfo proginfo{
        "0.5.1",
        "4.1.2022",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/EulerBBOpt.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
}
namespace Environment {
  template <>
  struct RegistrationPolicies<EulerBBOpt::MeasureO> {
    static constexpr int size = int(EulerBBOpt::MeasureO::laprp)+1;
    static constexpr std::array<const char*, size> string
    {"lvs", "laprp"};
  };
}
namespace EulerBBOpt {
  std::ostream& operator <<(std::ostream& out, const MeasureO enm) {
    switch (enm) {
    case MeasureO::laprp : return out << "look-ahead-propagation-calls";
    default : return out << "leaves";}
  }

  typedef std::tuple<MeasureO> bbopt_option_t;

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
      " measure-type        : " << Environment::WRP<EulerBBOpt::MeasureO>{} << "\n" <<
      " - .\n"
 ;
    return true;
  }

  struct Func {
    typedef Optimisation::y_t y_t;
    typedef Optimisation::vec_t vec_t;
    typedef std::uint64_t count_t;

    LatinSquares::ls_dim_t N, k, m1, m2;
    Lookahead::option_t alg_options;
    Euler::gecode_option_t gecode_options;
    EulerBBOpt::bbopt_option_t bbopt_options;
    Euler::gecode_intvec_t ls1_partial;
    Euler::gecode_intvec_t ls2_partial;
    const Timing::UserTime timing;

    Lookahead::UpperBoundO ub;
    EulerBBOpt::MeasureO enm;

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

      // Read BBOpt options:
      bbopt_options = argc <= index ?
        EulerBBOpt::bbopt_option_t{EulerBBOpt::MeasureO::lvs} :
        Environment::translate<EulerBBOpt::bbopt_option_t>()(argv[index++],
                                                             Lookahead::sep);
        enm = std::get<EulerBBOpt::MeasureO>(bbopt_options);

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

      Statistics::SearchStat stat;
      const std::shared_ptr<Euler::TWO_MOLS> p(new Euler::TWO_MOLS(N,
        alg_options, gecode_options, ls1_partial, ls2_partial, &v, &stat));
      const Timing::Time_point t1 = timing();

      // Limit the maximal number of leaves if specified in options
      // (if limit is 0, then it is not applied):
      const int maxunsatlvs = with_bound ? b : 0;
      Lookahead::solve<Euler::TWO_MOLS>(p, false, maxunsatlvs, &stat);
      assert(p.use_count() == 1);

      const Timing::Time_point t2 = timing();
      const double solving_time = t2 - t1;
      for (const auto x : v) std::cerr << x << " ";
      const Environment::RegistrationPolicies<Lookahead::UpperBoundO> rp_ub;
      const std::string sub = rp_ub.string[int(ub)];
      std::cerr << sub << " ";
      const Environment::RegistrationPolicies<EulerBBOpt::MeasureO> rp_enm;
      const std::string senm = rp_enm.string[int(enm)];
      std::cerr << senm << " ";
      Euler::print_stat(N, k, m1, m2, 0, solving_time, alg_options,
                        gecode_options, &stat, proginfo);

      count_t msr = 0;
      if (enm == EulerBBOpt::MeasureO::lvs) {
        msr = stat.solutions + stat.unsat_leaves; // leaves
      }
      else if (enm == EulerBBOpt::MeasureO::laprp) {
        msr = stat.subproblem_time.N; // look-ahead propagation calls
      }

      return msr;
    }
  };

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, EulerBBOpt::proginfo, argc, argv)) return 0;
  if (EulerBBOpt::show_usage(argc, argv)) return 0;

  const auto result = Optimisation::bbopt_rounds_app(argc, argv, EulerBBOpt::Func());
  FloatingPoint::fullprec_float80(std::cout);
  std::cout << result << "\n";
}
