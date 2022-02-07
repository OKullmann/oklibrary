// Oleg Zaikin, 22.12.2021 (Swansea)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  USAGE:

XXX

TODOS:

0. Output of basic data (like version number).

1. Using optionally an alternative measure, like the number of reductions,
   for optimisation OZ (needed for paper):
    - DONE sbps seems to relate closely to the time, and thus should be a
      relevant alternative measure to be optimsed.
    - The total number of propagations seems to be provided by Gecode,
      and could be useful.
    - The cutoff once the target value is overstepped is crucial:
     - Throwing an exception is a possibility.

2. DONE (Implemented in Lookahead.hpp)
   Implement the four orderings of branchings (given, reverse given,
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

5. Once we are consistently using at least gcc 11.2, use
   std::osyncstream instread of std::cerr for output.
  - Logging facilities need to be improved; see Optimisation.hpp.


BUGS:

*/


#include <iostream>
#include <string>
#include <memory>
#include <tuple>
//#include <syncstream>

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
        "0.5.3",
        "7.2.2022",
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
      "                     : " << Environment::WRP<Lookahead::BrOrderO>{} << "\n" <<
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
      const std::shared_ptr<Euler::TwoMOLS> p(new Euler::TwoMOLS(N,
        alg_options, gecode_options, ls1_partial, ls2_partial, &v, &stat));
      const Timing::Time_point t1 = timing();

      // Limit the maximal number of leaves if specified in options
      // (if limit is 0, then it is not applied):
      const int maxunsatlvs = with_bound ? b : 0;
      Lookahead::solve<Euler::TwoMOLS>(p, false, maxunsatlvs, &stat);
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
