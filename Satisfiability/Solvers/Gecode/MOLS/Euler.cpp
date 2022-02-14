/*
 *  Authors:
 *    Noah Rubin, Curtis Bright, Oliver Kullmann, Oleg Zaikin
 *
 *  Copyright:
 *    Oliver Kullmann, 2021, 2022
 *
  Imported 7.5.2021 by Oliver Kullmann.
  Original notes:

  Noah Rubin,  modifications by Curtis Bright


For a given N, k, and k partially filled Latin squares, solves the
Euler square completion problem(s).

USAGE:

> ./Euler [N=0] [k=2] [algorithmic-options] [output-options] [propagation-level] [distance-weights]


Examples:

MOLS> cat data/weights/testN6 | ./Euler 0 0 val "" dom 1,100,100,100
N k m1 m2 brt brsrc brsol bregr brpr prp t sat nds inds inds2 inds3 lvs ulvs sol 1chld chcs taus sbps chct taut sbpt ptime prog vers
6 2 12 6 la val one eager prun dom 0.4827 0 132 54 37 11 78 78 0 227 132 2230 30844 0.4805 0.0103 0.0830 0.0000 Euler 0.11.7


BUGS:

*/

/* TODOS:

-8. New algorithmic option: Put the given cells either into (x,y) or
    (y,x):
  - Due to the asymmetry in the formulation (the role of Z) this makes
    a difference regarding propagation.

-7. The output must show all parameters OZ
  - The output must show the weights.

-6. Remove the need to supply superfluous weights on the parameter-line.

-5. Add documentation etc. OZ :
 - At https://www.gecode.org/doc-latest/reference/classGecode_1_1Brancher.html
   there is no documentation on a destructor.
 - Apparently GC::Brancher has no base class.
 - Thus apparently GC::Brancher doesn't have a virtual constructor??
 - This then needed to be amended in the source-code! Just adding
      virtual ~Brancher() = default;
 - The gecode-forum needs to be urgently asked about that.
 - Indeed, the provisional destructor of LookaheadEq is never called!
 - However, gcc, with the warning about non-virtual dtor enabled, doesn't
   warn about it?

-4. Change the default values of options (they are misleading).

-3. Update the following.

-2. Use symmetry-breaking. (OZ, OK)
    - Symmetry-breaking options:
      a) To start with: no symmetry breaking.
      b) This goes together with the other algorithmic-options.

-1. Extend makefile, so that all variations are automatically created.
    - Possibly using a prefix

0. Read N and k by the LSRG function read_N().
    - LSRandGen::read_N() needs an input string in the format "N,k", but Euler
      deals with the format "N k".
    - Possible solutions: a) Extend LSRandGen::read_N() to deal with any given
      delimiter between N and k; b) Replace space by ',' in a string and
      give it to LSRandGen::read_N() without changing the function.
    - One more problem: in LSRandGen::read_N(), N == 0 is invalid case, while
      in Euler in one mode (reading partial Latin squares) N should be 0.

1. Determine the precise meaning of the branchers:
    - Do different branchers create a common pool?
    - It seems sensible here to use *one* basic vector of variables, which
      contains *all* variables.
    - Helper functions perform all the index-computations (so that the code is
      easy to read).

2. Make the model explicit. (OK, OZ)

3. It seems best to unify the two problem-types?
    - What are "the two problem-types" ? OZ

4. Design for general k
    - Based on the "constraint" LS(a,b,c) for a,b,c in {1, ..., k+2}.
    - Just posting all binom(N+2,3) such LS-constraints, plus the equalities
      (element-constraints) between these LS's.

*/

#include <iostream>
#include <ostream>
#include <string>
#include <tuple>
#include <iomanip>
#include <memory>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <SystemSpecifics/Timing.hpp>
#include <Numerics/NumInOut.hpp>
#include <Transformers/Generators/Random/LatinSquares.hpp>
#include <Transformers/Generators/Random/LSRG.hpp>

#include "../Lookahead.hpp"
#include "../Statistics.hpp"

#include "Euler.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.15.6",
        "12.2.2022",
        __FILE__,
        "Noah Rubin, Curtis Bright, Oliver Kullmann, and Oleg Zaikin",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/2mols.cpp",
        "GPL v3"};

  using namespace Euler;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) noexcept {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " N k algorithmic-options" <<
    " output-options propagation-level lookahead-weights\n" <<
    " N                   : default = " << N_default << "\n" <<
    " k                   : default = " << k_default << "\n" <<
    " algorithmic-options : " << Environment::WRP<Lookahead::BrTypeO>{} << "\n" <<
    "                     : " << Environment::WRP<Lookahead::BrSourceO>{} << "\n" <<
    "                     : " << Environment::WRP<Lookahead::BrSolutionO>{} << "\n" <<
    "                     : " << Environment::WRP<Lookahead::BrPruneO>{} << "\n" <<
    "                     : " << Environment::WRP<Lookahead::BrOrderO>{} << "\n" <<
    "                     : " << Environment::WRP<Lookahead::LogLvlO>{} << "\n" <<
    " output-options      : " << Environment::WRP<HeO>{} << "\n" <<
    "                     : " << Environment::WRP<StatO>{} << "\n" <<
    "                     : " << Environment::WRP<SolO>{} << "\n" <<
    " propagation-level   : " << Environment::WRP<PropO>{} << "\n" <<
    " lookahead-weights   : comma-separated weigths for calculating" <<
    "                       distances in lookahead." << "\n" <<
#if GIST == 1
    " visualise-options   : " << "+gist:visualise-by-gist" << "\n" <<
#endif
    "\nIf N>0, then all k Latin squares are considered unfilled and\n" <<
    "the standard input is ignored. If N=0, the standard input is read.\n" <<
    "in the following format:\n" <<
    " N K\n" <<
    " [N lines of numbers and *, ignore spaces for LS1]\n" <<
    " [N lines of numbers and *, ignore spaces for LS2]\n" <<
    "EOL-symbols between blocks are ignored.\n\n" <<
    "For given N, k, and k partially filled Latin squares, solves the\n" <<
    "Euler square completion problem.\n\n";
    return true;
  }

  bool show_header(const int argc, const char* const argv[]) noexcept {
    if (argc == 2) {
      const std::string_view s = argv[1];
      if (s == "--header") {
        print_header();
        return true;
      }
    }
    return false;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;
  if (show_header(argc, argv)) return 0;

  const Timing::UserTime timing;
  const Timing::Time_point t0 = timing(); // start of computation

  Environment::Index index;
  LatinSquares::ls_dim_t N = argc <= index ?
    N_default : read_N(argv[index++], error);
  LatinSquares::ls_dim_t k = argc <= index ?
    k_default : read_k(argv[index++], error);
  const Lookahead::option_t alg_options = argc <= index ? Lookahead::option_t{} :
    Environment::translate<Lookahead::option_t>()(argv[index++], Lookahead::sep);
  const output_option_t output_options = argc <= index ?
    output_option_t{HeO::show, StatO::show, SolO::noshow} :
    Environment::translate<output_option_t>()(argv[index++], sep);
  const gecode_option_t gecode_options = argc <= index ?
    gecode_option_t{PropO::dom} :
    Environment::translate<gecode_option_t>()(argv[index++], sep);
  const Lookahead::vec_t wghts = FloatingPoint::to_vec_float80(argv[index++], ',');
#if GIST == 1
  std::string s = argc <= index ? "" : argv[index++];
  bool gist = s=="+gist" ? true : false;
#endif
  index++;
  index.deactivate();

  gecode_intvec_t ls1_partial, ls2_partial;
  if (N == 0) {
    N = read_N(error);
    k = read_k(error);
    assert(N > 0 and k > 0);
    ls1_partial = read_partial_ls(N);
    ls2_partial = read_partial_ls(N);
    assert(not ls1_partial.empty() and not ls2_partial.empty());
  }

  if (wghts.size() != N-2) {
    std::cerr << error << "Weights vector must have size N-2.\n";
    std::exit(int(RandGen::Error::domain));
  }

  const LatinSquares::ls_dim_t m1 = given_cells(ls1_partial);
  const LatinSquares::ls_dim_t m2 = given_cells(ls2_partial);
  assert(m1 <= N*N);
  assert(m2 <= N*N);

  if (k != 2) {
    std::cerr << error << "k > 2 is not implemented yet.\n";
    std::exit(int(RandGen::Error::domain));
  }

  assert(N > 0 and k > 0);
  Statistics::SearchStat stat;
  std::ostream log(NULL);
  const LA::LogLvlO llo = std::get<LA::LogLvlO>(alg_options);
  const std::unique_ptr<TwoMOLS> p(new TwoMOLS(N, alg_options,
                        gecode_options, ls1_partial, ls2_partial,
                        &wghts, &stat, &log, llo));
  assert(p->valid());
  const Timing::Time_point t1 = timing(); // after reading and set up
  const double reading_time = t1 - t0;

  bool prsol = std::get<SolO>(output_options) == SolO::show ? true : false;
  Lookahead::solve<TwoMOLS>(p, prsol, 0, &stat);
  const Timing::Time_point t2 = timing(); // after solving
  const double solving_time = t2 - t1;

  if (std::get<HeO>(output_options) == HeO::show) print_header();
  if (std::get<StatO>(output_options) == StatO::show) {
    print_stat(N, k, m1, m2, reading_time, solving_time,
               alg_options, gecode_options, &stat, proginfo);
  }

#if GIST == 1
  if (gist) {
    // Visualise via Gist:
    GC::Gist::Print<TwoMOLS> vis("Print solution");
    GC::Gist::Options o;
    o.inspect.click(&vis);
    GC::Gist::dfs(p.get(),o);
  }
#endif

}
