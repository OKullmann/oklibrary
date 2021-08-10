/*
 *  Authors:
 *    Noah Rubin, Curtis Bright, Oliver Kullmann, Oleg Zaikin
 *
 *  Copyright:
 *    Oliver Kullmann, 2021
 *
  Imported 7.5.2021 by Oliver Kullmann.
  Original notes:

  Noah Rubin,  modifications by Curtis Bright


For a given N, k, and k partially filled Latin squares, solves the
Euler square completion problem(s).

USAGE:

> ./Euler -v | --version

for information on the program, the version, and the environment.

> ./Euler -h | --help

for basic help-information.

> ./Euler [N=0] [k=2] [algorithmic-options] [output-options]


BUGS:

0. FIXED (with a proper propagation level on this example the runtime is reasonable.
     On a campus desktop, Euler version 0.6.1:
   default (weak) propagation with equality+values branchings:
   > LSRG 6,2 "-co" "1*0,0,36;1*0,0,0" 0 | ./Euler 0 2 la
     N k m1 m2 brt brsrc brm brsol prp t sat nds inds lvs ulvs sol chcs taus sbps chctime tautime sbptime prptime ptime prog vers
     6 2 36 0 la eqval mu0 one def 31.6225 0 106777 98137 8640 8640 0 106777 3281850 5262096 31.4615 9.8995 12.3636 0.0000 0.0005 Euler 0.6.1
   domain (strong) propagation with equality+values branchings:
   > LSRG 6,2 "-co" "1*0,0,36;1*0,0,0" 0 | ./Euler 0 2 la "" dom
     N k m1 m2 brt brsrc brm brsol prp t sat nds inds lvs ulvs sol chcs taus sbps chctime tautime sbptime prptime ptime prog vers
     6 2 36 0 la eqval mu0 one dom 15.6697 0 25183 22303 2880 2880 0 25183 758364 1260096 15.5077 2.4583 3.0023 0.0000 0.0000 Euler 0.6.1
   domain propagation with values-branchings:
   > LSRG 6,2 "-co" "1*0,0,36;1*0,0,0" 0 | ./Euler 0 2 la,val "" dom
     N k m1 m2 brt brsrc brm brsol prp t sat nds inds lvs ulvs sol chcs taus sbps chctime tautime sbptime prptime ptime prog vers
     6 2 36 0 la val mu0 one dom 6.5497 0 5557 2677 2880 2880 0 5557 94692 403572 6.4601 0.3175 1.0832 0.0000 0.0001 Euler 0.6.1
   domain propagation with equality-branchings:
   > LSRG 6,2 "-co" "1*0,0,36;1*0,0,0" 0 | ./Euler 0 2 la,eq "" dom
     N k m1 m2 brt brsrc brm brsol prp t sat nds inds lvs ulvs sol chcs taus sbps chctime tautime sbptime prptime ptime prog vers
     6 2 36 0 la eq mu0 one dom 4.1538 0 2833 2483 350 350 0 2833 151312 307592 4.1360 0.5001 0.8338 0.0000 0.0000 Euler 0.6.1
    )
   > LSRG 6,2 "-co" "1*0,0,36;1*0,0,0" 0 | ./Euler 0 2 la
takes a long time (interrupted after 10 minutes).
    - In this case, lookahead with equality-branchings and mu0 function is used.
    - If mu0 is replaced by mu1, it takes 1 minute:
      > time LSRG 6,2 "-co" "1*0,0,36;1*0,0,0" 0 | ./Euler 0 2 la,mu1
105239     95519      9720         0         1         0         1         0

real	0m59.366s
user	0m59.341s
sys	0m0.008s

*/

/* TODOS:

-2. Use symmetry-breaking. (OZ, OK)
    - Symmetry-breaking options:
      a) To start with: no symmetry breaking.
      b) This goes together with the other algorithmic-options.

-1. Extend makefile, so that all variations are automatically created.
    - Possibly using a prefix

0. Read N and k by the LSRG function read_N().
    - LSRG::read_N() needs an input string in the format "N,k", but Euler
      deals with the format "N k".
    - Possible solutions: a) Extend LSRG::read_N() to deal with any given
      delimiter between N and k; b) Replace space by ',' in a string and
      give it to LSRG::read_N() without changing the function.
    - One more problem: in LSRG::read_N(), N == 0 is invalid case, while
      in Euler in one mode (reading partial Latin squares) N should be 0.

1. Determine the precise meaning of the branchers:
    - Do different branchers create a common pool?
    - It seems sensible here to use *one* basic vector of variables, which
      contains *all* variables.
    - Helper functions perform all the index-computations (so that the code is easy
      to read).

2. Handle the options for propagation-levels: (OZ)
    - DONE (a new option is added)
      Perhaps command-line options, which are translated into
      enumerated-values, for which one defines switch-statements.
    - DONE (by default in Gecode the propagation level is IPL_DEF,
      it means the default propagation level for each constraint.
      Now if e.g. dom is specified in the option, all constaints
      get the full domain propagation.)
      Handling all distinct- and all element-constraints for now
      the same.
    - Finding, if possible, the best combination for now.
    - The model is
        LSRG N,2 "-co" "" "1*0,0,m;1*0,0,0" seeds .
    - Collecting some easy statistics.

3. Make the model explicit. (OK, OZ)

4. It seems best to unify the two problem-types?

5. Design for general k
    - Based on the "constraint" LS(a,b,c) for a,b,c in {1, ..., k+2}.
    - Just posting all binom(N+2,3) such LS-constraints, plus the equalities
      (element-constraints) between these LS's.

*/

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <iomanip>

#include "gecode/driver.hh"
#include "gecode/int.hh"
#include "gecode/search.hh"

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>
#include <SystemSpecifics/Timing.hpp>
#include <Transformers/Generators/Random/LatinSquares.hpp>
#include <Transformers/Generators/Random/LSRG.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/ClauseSets.hpp>

#include "../Lookahead.hpp"

namespace Euler{

  const Environment::ProgramInfo proginfo{
        "0.6.1",
        "10.8.2021",
        __FILE__,
        "Noah Rubin, Curtis Bright, Oliver Kullmann, and Oleg Zaikin",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/2mols.cpp",
        "GPL v3"};

  namespace GC = Gecode;
  namespace LA = Lookahead;
  namespace LS = LatinSquares;
  namespace RG = RandGen;

  typedef std::vector<int> gecode_intvec_t;
  typedef std::vector<GC::IntVar> gecode_intvarvec_t;
  typedef std::vector<std::string> partial_ls_t;

  constexpr LS::ls_dim_t N_default = 0;
  constexpr LS::ls_dim_t k_default = 2;

  // Pgopagation level for Gecode constraints.
  // def: default propagation (can be different for different
  //      constraints).
  // val: value propagation (the fastest and the simplest one).
  // bnd: bound propagation (average speed and performance).
  // dom: domain propagation (the slowest and the most effective).
  enum class PropO {def=0, val=1, bnd=2, dom=3};
  constexpr int PropOsize = 4;

  enum class HeO {show=0, noshow=1};
  constexpr int HeOsize = 2;
  enum class StatO {show=0, noshow=1};
  constexpr int StatOsize = 2;
  enum class SolO {show=0, noshow=1};
  constexpr int SolOsize = 2;
}
namespace Environment {
  template <> struct RegistrationPolicies<Euler::PropO> {
    static constexpr int size = Euler::PropOsize;
    static constexpr std::array<const char*, size> string {"def", "val", "bnd", "dom"};
  };
  template <> struct RegistrationPolicies<Euler::HeO> {
    static constexpr int size = Euler::HeOsize;
    static constexpr std::array<const char*, size> string {"+head", "-head"};
  };
  template <> struct RegistrationPolicies<Euler::StatO> {
    static constexpr int size = Euler::StatOsize;
    static constexpr std::array<const char*, size> string {"+stat", "-stat"};
  };
  template <> struct RegistrationPolicies<Euler::SolO> {
    static constexpr int size = Euler::SolOsize;
    static constexpr std::array<const char*, size> string {"+sol", "-sol"};
  };
}
namespace Euler {
  constexpr char sep = ',';

  typedef std::tuple<PropO> gecode_option_t;
  std::ostream& operator <<(std::ostream& out, const PropO m) {
    switch (m) {
    case PropO::val : return out << "values-prop";
    case PropO::bnd : return out << "bounds-prop";
    case PropO::dom : return out << "domain-prop";
    default : return out << "default-prop";}
  }

  typedef std::tuple<HeO, StatO, SolO> output_option_t;
  std::ostream& operator <<(std::ostream& out, const HeO m) {
    switch (m) {
    case HeO::show : return out << "show-header";
    default : return out << "noshow-header";}
  }
  std::ostream& operator <<(std::ostream& out, const StatO m) {
    switch (m) {
    case StatO::show : return out << "show-statistics";
    default : return out << "noshow-statistics";}
  }
  std::ostream& operator <<(std::ostream& out, const SolO m) {
    switch (m) {
    case SolO::show : return out << "show-solutions";
    default : return out << "noshow-solutions";}
  }

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [N] [k] [algorithmic-options]\n" <<
    " N                   : default = " << N_default << "\n" <<
    " k                   : default = " << k_default << "\n" <<
    " algorithmic-options : " << Environment::WRP<LA::BrTypeO>{} << "\n" <<
    "                     : " << Environment::WRP<LA::BrSourceO>{} << "\n" <<
    "                     : " << Environment::WRP<LA::BrMeasureO>{} << "\n" <<
    "                     : " << Environment::WRP<LA::BrSolutionO>{} << "\n" <<
    " output-options      : " << Environment::WRP<HeO>{} << "\n" <<
    "                     : " << Environment::WRP<StatO>{} << "\n" <<
    "                     : " << Environment::WRP<SolO>{} << "\n" <<
    " propagation-level   : " << Environment::WRP<PropO>{} << "\n\n" <<
    "If N>0, then all k Latin squares are considered unfilled and\n" <<
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

  LS::ls_dim_t read_N(const std::string& s, const std::string& error) noexcept {
    if (s.empty()) return N_default;
    const LS::ls_dim_t N = FloatingPoint::touint(s);
    if (not LS::valid(N) and N != 0) {
      std::cerr << error << "N must be a nonnegative integer in [0,"
                << LS::max_dim-1 << "]" << ", but N=" << N << ".\n";
      std::exit(int(RG::Error::domain));
    }
    return N;
  }
  LS::ls_dim_t read_N(const std::string& error) noexcept {
    std::string s;
    std::cin >> s;
    return read_N(s, error);
  }

  LS::ls_dim_t read_k(const std::string& s,
                      const std::string& error) noexcept {
    if (s.empty()) return k_default;
    const LS::ls_dim_t k = FloatingPoint::touint(s);
    if (not LS::valid(k) and k != 0) {
      std::cerr << error << "k must be a nonnegative integer in [0,"
                << LS::max_dim-1 << "]" << ", but k=" << k << ".\n";
      std::exit(int(RG::Error::domain));
    }
    return k;
  }
  LS::ls_dim_t read_k(const std::string& error) noexcept {
    std::string s;
    std::cin >> s;
    return read_k(s, error);
  }

  gecode_intvec_t read_partial_ls(const LS::ls_dim_t N) noexcept {
    assert(N > 0);
    const LS::ls_dim_t size = N*N;
    gecode_intvec_t partial_ls(size);
    std::string s;
    partial_ls_t ls_s;
    do {
      std::cin >> s;
      if (s.empty()) continue;
      if (N > 10) ls_s.push_back(s);
      else for (auto c : s) ls_s.push_back(std::string(1,c));
      assert(ls_s.size() <= size);
    } while (ls_s.size() != size);
    for (LS::ls_dim_t i=0; i < size; ++i) {
      assert(i < partial_ls.size() and i < ls_s.size());
      partial_ls[i] = (ls_s[i] == "*") ? -1 : std::stoi(ls_s[i]);
    }
    return partial_ls;
  }

  LS::ls_dim_t given_cells(const gecode_intvec_t ls_partial) {
    LS::ls_dim_t res = 0;
    for (auto x : ls_partial) res += x==-1 ? 0 : 1;
    return res;
  }

  void print_header() {
    std::cout << "N k m1 m2 brt brsrc brm brsol prp t sat nds inds lvs "
              << "ulvs sol chcs taus sbps chctime tautime sbptime "
              << "prptime ptime prog vers\n";
  }

  void print_stat(const LS::ls_dim_t N, const LS::ls_dim_t k,
                  const LS::ls_dim_t m1, const LS::ls_dim_t m2,
                  const double reading_time, const double solving_time,
                  const LA::SearchStat stat, const gecode_option_t gc_options) {
    const auto sat = stat.solutions==0 ? 0 : 1;
    const auto lvs = stat.unsat_leaves + stat.solutions;

    const auto alg_options= stat.br_options;
    const LA::BrTypeO brt = std::get<LA::BrTypeO>(alg_options);
    const LA::BrSourceO brsrc = std::get<LA::BrSourceO>(alg_options);
    const LA::BrMeasureO brm = std::get<LA::BrMeasureO>(alg_options);
    const LA::BrSolutionO brsol = std::get<LA::BrSolutionO>(alg_options);
    Environment::RegistrationPolicies<LA::BrTypeO> rp_brt;
    Environment::RegistrationPolicies<LA::BrSourceO> rp_brsrc;
    Environment::RegistrationPolicies<LA::BrMeasureO> rp_brm;
    Environment::RegistrationPolicies<LA::BrSolutionO> rp_brsol;
    const std::string sbrt = rp_brt.string[int(brt)];
    const std::string sbrsrc = rp_brsrc.string[int(brsrc)];
    const std::string sbrm = sbrt == "la" ? rp_brm.string[int(brm)] : "\"\"";
    const std::string sbrsol = rp_brsol.string[int(brsol)];

    const Euler::PropO prop = std::get<Euler::PropO>(gc_options);
    Environment::RegistrationPolicies<Euler::PropO> rp_prop;
    const std::string sprop = rp_prop.string[int(prop)];

    const unsigned prec_time = 4;
    const auto fi = std::fixed;
    std::cout << std::setprecision(prec_time) << fi << N << " " << k
              << " " << m1 << " " << m2 << " " << sbrt << " " << sbrsrc
              << " " << sbrm << " " << sbrsol << " " << sprop << " "
              << solving_time << " " << sat << " " << stat.nodes << " "
              << stat.inner_nodes << " " << lvs << " " << stat.unsat_leaves
              << " " << stat.solutions << " " << stat.choice_calls << " "
              << stat.tau_calls << " " << stat.subproblem_calls << " "
              << stat.choice_time << " " << stat.tau_time << " "
              << stat.subproblem_time << " " << stat.propag_time << " "
              << reading_time << " " << proginfo.prg << " "
              << proginfo.vrs << "\n";
  }

  class TWO_MOLS : public GC::Space {
    const LS::ls_dim_t N;
    const LA::option_t alg_options;
    const gecode_option_t gecode_options;
    GC::IntVarArray x, y, z, V;
    inline LA::size_t x_index(const LA::size_t i) const noexcept { return i; }
    inline LA::size_t y_index(const LA::size_t i) const noexcept { return i + LA::tr(x.size()); }
    inline LA::size_t z_index(const LA::size_t i) const noexcept {
      return i + LA::tr(x.size()) + LA::tr(y.size());
    }

    inline GC::IntPropLevel prop_level(const gecode_option_t gc_options) const noexcept {
      GC::IntPropLevel ipl = GC::IPL_DEF;
      const auto gc_option = std::get<PropO>(gc_options);
      switch( gc_option ) {
      case PropO::val:
          ipl = GC::IPL_VAL;
          break;
      case PropO::bnd:
          ipl = GC::IPL_BND;
          break;
      case PropO::dom:
          ipl = GC::IPL_DOM;
          break;
      case PropO::def:
          ipl = GC::IPL_DEF;
          break;
      default:
          ipl = GC::IPL_DEF;
          break;
      }
      return ipl;
    }
  public:
    TWO_MOLS(const LS::ls_dim_t N, const LA::option_t alg_options,
             const gecode_option_t gecode_options,
             const gecode_intvec_t ls1_partial = {},
             const gecode_intvec_t ls2_partial = {}) :
      N(N), alg_options(alg_options), gecode_options(gecode_options),
      x(*this, N*N, 0, N - 1),
      y(*this, N*N, 0, N - 1),
      z(*this, N*N, 0, N - 1),
      V(*this, x.size() + y.size() + z.size(), 0, N - 1) {
      assert(valid());
      // Determine propagation level:
      GC::IntPropLevel prp_lvl = prop_level(gecode_options);
      // Use an umbrella variable array for all variables:
      for (LA::size_t i = 0; i < LA::tr(x.size()); ++i) V[x_index(i)] = x[i];
      for (LA::size_t i = 0; i < LA::tr(y.size()); ++i) V[y_index(i)] = y[i];
      for (LA::size_t i = 0; i < LA::tr(z.size()); ++i) V[z_index(i)] = z[i];
      // Known cells of partially filled Latin squares:
      if (not ls1_partial.empty() and not ls2_partial.empty()) {
        assert(ls1_partial.size() == N*N and ls2_partial.size() == N*N);
        for(LS::ls_dim_t i = 0; i < N; ++i) {
          for(LS::ls_dim_t j = 0; j < N; ++j) {
            assert(i*N + j < ls1_partial.size());
            if (ls1_partial[i*N + j] >= 0) {
              dom(*this, x[i*N + j], ls1_partial[i*N + j], ls1_partial[i*N + j],
                  prp_lvl);
            }
            assert(i*N + j < ls2_partial.size());
            if (ls2_partial[i*N + j] >= 0) {
              dom(*this, y[i*N + j], ls2_partial[i*N + j], ls2_partial[i*N + j],
                  prp_lvl);
            }
          }
        }
      }

      // Latin property in rows of X:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t rows_x;
        for (LS::ls_dim_t j = 0; j < N; ++j) rows_x.push_back(x[i*N + j]);
        GC::distinct(*this, rows_x, prp_lvl);
      }
      // Latin property in cols of X:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t cols_x;
        for (LS::ls_dim_t j = 0; j < N; ++j) cols_x.push_back(x[j*N + i]);
        GC::distinct(*this, cols_x, prp_lvl);
      }
      // Latin property in rows of Y:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t rows_y;
        for (LS::ls_dim_t j = 0; j < N; ++j) rows_y.push_back(y[i*N + j]);
        GC::distinct(*this, rows_y, prp_lvl);
      }
      // Latin property in cols of Y:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t cols_y;
        for (LS::ls_dim_t j = 0; j < N; ++j) cols_y.push_back(y[j*N + i]);
        GC::distinct(*this, cols_y, prp_lvl);
      }
      // Row uniqueness of Z:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t rows_z;
        for (LS::ls_dim_t j = 0; j < N; ++j) rows_z.push_back(z[i*N + j]);
        GC::distinct(*this, rows_z, prp_lvl);
      }
      // Column uniqueness of Z:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t cols_z;
        for (LS::ls_dim_t j = 0; j < N; ++j) cols_z.push_back(z[j*N + i]);
        GC::distinct(*this, cols_z, prp_lvl);
      }
      // Enforce element constraints on Z, X, Y:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t Zvec_i;
        for (LS::ls_dim_t j = 0; j < N; ++j) Zvec_i.push_back(z[i*N + j]);
        for (LS::ls_dim_t j = 0; j < N; ++j) {
          GC::element(*this, GC::IntVarArgs(Zvec_i), x[i*N + j],
                      y[i*N + j], prp_lvl);
        }
      }

      if (not this->failed()) {
        LA::post_branching<TWO_MOLS>(*this, V, alg_options);
      }

    }

    TWO_MOLS(TWO_MOLS& T) : GC::Space(T), N(T.N), alg_options(T.alg_options),
             gecode_options(T.gecode_options) {
      assert(T.valid());
      x.update(*this, T.x);
      y.update(*this, T.y);
      z.update(*this, T.z);
      V.update(*this, T.V);
      assert(valid(V));
    }
    virtual GC::Space* copy() {
      return new TWO_MOLS(*this);
    }

    inline bool valid () const noexcept {return N > 0 and valid(V);}
    inline bool valid (const GC::IntVarArray V) const noexcept {
      return x.size() > 0 and V.size() == x.size() + y.size() + z.size();
    }
    inline bool valid (const LA::size_t i) const noexcept {return i<LA::tr(V.size());}

    inline GC::IntVar at(const LA::size_t i) const noexcept {
      assert(valid()); assert(valid(i));
      return V[i];
    }
    inline GC::IntVarArray at() const noexcept { assert(valid()); return V; }

    LA::option_t branching_options() const noexcept { assert(valid()); return alg_options; }

    void print() {
      assert(valid());
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        for (LS::ls_dim_t j = 0; j < N; ++j) {
          std::cout << x[i*N + j];
          if (j < N-1) std::cout << " ";
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        for (LS::ls_dim_t j = 0; j < N; ++j) {
          std::cout << y[i*N + j];
          if (j < N-1) std::cout << " ";
        }
        std::cout << std::endl;
      }
    }

  };

}

int main(const int argc, const char* const argv[]) {

  using namespace Euler;
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const Timing::UserTime timing;
  const Timing::Time_point t0 = timing(); // start of computation

  Environment::Index index;
  LS::ls_dim_t N = argc <= index ?
    N_default : read_N(argv[index++], error);
  LS::ls_dim_t k = argc <= index ?
    k_default : read_k(argv[index++], error);
  const LA::option_t alg_options = argc <= index ? LA::option_t{} :
    Environment::translate<LA::option_t>()(argv[index++], LA::sep);
  const output_option_t output_options = argc <= index ?
    output_option_t{HeO::show, StatO::show, SolO::noshow} :
    Environment::translate<output_option_t>()(argv[index++], sep);
  const gecode_option_t gecode_options = argc <= index ?
    gecode_option_t{PropO::def} :
    Environment::translate<gecode_option_t>()(argv[index], sep);
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

  LS::ls_dim_t m1 = given_cells(ls1_partial);
  LS::ls_dim_t m2 = given_cells(ls2_partial);
  assert(m1 == 0 or m1 == N*N);
  assert(m2 == 0 or m2 == N*N);

  if (k != 2) {
    std::cerr << error << "k > 2 is not implemented yet" << std::endl;
    std::exit(int(RG::Error::domain));
  }

  assert(N > 0 and k > 0);
  const std::shared_ptr<TWO_MOLS> p(new TWO_MOLS(N, alg_options,
                        gecode_options, ls1_partial, ls2_partial));
  assert(p->valid());
  const Timing::Time_point t1 = timing(); // after reading and set up
  const double reading_time = t1 - t0;

  bool prsol = std::get<SolO>(output_options) == SolO::show ? true : false;
  LA::SearchStat stat = LA::solve<TWO_MOLS>(p, prsol);
  const Timing::Time_point t2 = timing(); // after solving
  const double solving_time = t2 - t1;

  if (std::get<HeO>(output_options) == HeO::show) print_header();
  if (std::get<StatO>(output_options) == StatO::show) {
    print_stat(N, k, m1, m2, reading_time, solving_time, stat, gecode_options);
  }

  return 0;
}
