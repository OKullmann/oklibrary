/*
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

> ./Euler [N=6] [k=2] [algorithmic-options]
[N] [K]
[LS1]
[LS2]

BUGS:

1. > LSRG 6,2 "-co" "1*0,0,36;1*0,0,0" 0 | ./Euler
takes a long time (say one minutes).

    NEEDS UPDATE XXX

    - Perhaps due to propagations too weak?
    - Currently (feb6a83b0271f7647c34e5290a6008728e2502dc) the completion-mode
      is disabled (needs to be re-abled soon).
    - Needs to be investigated historically, and then needs to become an
      application test.
    - The LSRG-command on the current version (0.4.8) is executed fast (about
      1 second). However, at the time of bug report (version 0.2.10), another
      branching strategy was used by default. At that time the default one was
      la,v,one (lookahead, values-branching, find the first solution), while now
      it is mind,eq,one (minimal-domain-size variable, equality-branching, find
      the first solution). On the current version, the bug can be
      reproduced by
      > LSRG 6,2 "-co" "1*0,0,36;1*0,0,0" 0 | ./Euler 0 0 la,v,one
      XXX
      On a laptop this run takes 22 seconds.
      So the bug is confirmed and needs further investigation.

      > LSRG 6,2 "-co" "1*0,0,36;1*0,0,0" 0 | ./Euler 0 2 la
      takes longer XXX

      > time LSRG 6,2 "-co" "1*0,0,36;1*0,0,0" 0 | ./Euler 0 2 la,mu1
105239     95519      9720         0         1         0         1         0

real	0m59.366s
user	0m59.341s
sys	0m0.008s

2. FIXED (1. Old 2mols-like symmetry breaking was used together with partially filled Latin
   squares, now the former is removed. 2. Gecode-constraints for partially filled Latin
   squares were not applied, it was fixed.)
   for (( i=0; i<10000; ++i )); do res="$(LSRG 5,2 "-co" "1*0,0,25;1*0,0,0" t | ./Euler | tr -s " " | cut -d" " -f 4)"; echo -n $res; done
yields only unsatisfiable instances:
    - For small N the probabilities should be all known.
    - This seems to be a bug.

*/

/* TODOS:

-4. Introduce Posix-runtime measurent:
    - As done in other parts of the OKlibrary.

-3. Symmetry-breaking options:
    - To start with: no symmetry breaking.
    - This goes together with the other algorithmic-options.

-2. Reading of input:
    - The format is, from standard input

N K
[N lines of numbers and *, ignore spaces for LS1]
[N lines of numbers and *, ignore spaces for LS2]

    EOL-symbols between blocks are ignored.
    - N=0 means the above standard-input processing, for N>=1 however
      nothing is read from standard input, and both given partial ls's
      are now assumed to be empty.
    - The statistics-output just contains the number of given cells, per
      ls.

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

2. DONE (namespace-abbreviations and proper types are used, version and help provided)
   Update coding standard (OZ)
    - DONE Use namespace-abbreviations.
    - DONE Use proper types.
    - DONE Provide version and help.

3. Handle the options for propagation-levels: (OZ)
    - Perhaps command-line options, which are translated into
      enumerated-values, for which one defines switch-statements.
    - Handling all distinct- and all element-constraints for now
      the same.
    - Finding, if possible, the best combination for now.
    - The model is
        LSRG N,2 "-co" "" "1*0,0,m;1*0,0,0" seeds .
    - Collecting some easy statistics.

4. Make the model explicit. (OK, OZ)

5. Use symmetry-breaking. (OZ, OK)
    - Is an option useful for that?

6. It seems best to unify the two problem-types?

7. Design for general k
    - Based on the "constraint" LS(a,b,c) for a,b,c in {1, ..., k+2}.
    - Just posting all binom(N+2,3) such LS-constraints, plus the equalities
      (element-constraints) between these LS's.

*/

#include <iostream>
#include <string>
#include <vector>

#include <cmath>

#include "gecode/driver.hh"
#include "gecode/int.hh"
#include "gecode/search.hh"

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "../Lookahead.hpp"

#include "LatinSquares.hpp"
#include "LSRG.hpp"
#include "Numbers.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.8",
        "17.7.2021",
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

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " N k [algorithmic-options]\n" <<
    "[N2] [k2]\n" <<
    "[LS1]\n" <<
    "[LS2]\n\n" <<
    " N                   : default = " << N_default << "\n" <<
    " k                   : default = " << k_default << "\n" <<
    " algorithmic-options : " << Environment::WRP<LA::BrTypeO>{} << "\n" <<
    "                     : " << Environment::WRP<LA::BrSourceO>{} << "\n" <<
    "                     : " << Environment::WRP<LA::BrMeasureO>{} << "\n" <<
    "                     : " << Environment::WRP<LA::BrSolutionO>{} << "\n" <<
    " N2, k2              : values of N and k from the LSRG output. \n" <<
    " LS1, LS2            : partialy filled Latin squares in the LSRG" <<
    " output format:\n a square is represented by N lines, each contains N " <<
    " space-separated symbols (* or integer 0..N-1).\n\n" <<
    "For a given N, k, and k partially filled Latin squares, solves the " <<
    "Euler square completion problem(s).\n" <<
    "If N > 0, then all k LS are considered unfilled and next lines of the " <<
    "standard input are ignored.\nIf N == 0, then values of N and k are " <<
    "taken from N2 and k2, and LS are parsed from the standard input.\n\n";
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

  LS::ls_dim_t read_k(const std::string& s, const std::string& error) noexcept {
    if (s.empty()) return k_default;
    const LS::ls_dim_t k = FloatingPoint::touint(s);
    if (not LS::valid(k) and k != 0) {
      std::cerr << error << "k must be a nonnegative integer in [0,"
                << LS::max_dim-1 << "]" << ", but k=" << k << ".\n";
      std::exit(int(RG::Error::domain));
    }
    return k;
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
      ls_s.push_back(s);
    } while (ls_s.size() != size);
    for (LS::ls_dim_t i=0; i < size; ++i) {
      assert(i < partial_ls.size() and i < ls_s.size());
      partial_ls[i] = (ls_s[i] == "*") ? -1 : std::stoi(ls_s[i]);
    }
    return partial_ls;
  }

}

class TWO_MOLS : public GC::Space {
  const LS::ls_dim_t N;
  const LA::option_t options;
  GC::IntVarArray x, y, z, V;

  inline LA::size_t x_index(const LA::size_t i) const noexcept { return i; }
  inline LA::size_t y_index(const LA::size_t i) const noexcept { return i + LA::tr(x.size()); }
  inline LA::size_t z_index(const LA::size_t i) const noexcept {
    return i + LA::tr(x.size()) + LA::tr(y.size());
  }

public:
  TWO_MOLS(const LS::ls_dim_t N, const LA::option_t options,
           const gecode_intvec_t ls1_partial = {}, const gecode_intvec_t ls2_partial = {}) :
    N(N), options(options),
    x(*this, N*N, 0, N - 1),
    y(*this, N*N, 0, N - 1),
    z(*this, N*N, 0, N - 1),
    V(*this, x.size() + y.size() + z.size(), 0, N - 1) {

    assert(valid());

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
            dom(*this, x[i*N + j], ls1_partial[i*N + j], ls1_partial[i*N + j]);
          }
          assert(i*N + j < ls2_partial.size());
          if (ls2_partial[i*N + j] >= 0) {
            dom(*this, y[i*N + j], ls2_partial[i*N + j], ls2_partial[i*N + j]);
          }
        }
      }
    }

    // Latin property in rows of X:
    for (LS::ls_dim_t i = 0; i < N; ++i) {
      gecode_intvarvec_t rows_x;
      for (LS::ls_dim_t j = 0; j < N; ++j) rows_x.push_back(x[i*N + j]);
      GC::distinct(*this, rows_x);
    }

    // Latin property in cols of X:
    for (LS::ls_dim_t i = 0; i < N; ++i) {
      gecode_intvarvec_t cols_x;
      for (LS::ls_dim_t j = 0; j < N; ++j) cols_x.push_back(x[j*N + i]);
      GC::distinct(*this, cols_x);
    }

    // Latin property in rows of Y:
    for (LS::ls_dim_t i = 0; i < N; ++i) {
      gecode_intvarvec_t rows_y;
      for (LS::ls_dim_t j = 0; j < N; ++j) rows_y.push_back(y[i*N + j]);
      GC::distinct(*this, rows_y);
    }

    // Latin property in cols of Y:
    for (LS::ls_dim_t i = 0; i < N; ++i) {
      gecode_intvarvec_t cols_y;
      for (LS::ls_dim_t j = 0; j < N; ++j) cols_y.push_back(y[j*N + i]);
      GC::distinct(*this, cols_y);
    }

    // Row uniqueness of Z:
    for (LS::ls_dim_t i = 0; i < N; ++i) {
      gecode_intvarvec_t rows_z;
      for (LS::ls_dim_t j = 0; j < N; ++j) rows_z.push_back(z[i*N + j]);
      GC::distinct(*this, rows_z);
    }

    // Column uniqueness of Z:
    for (LS::ls_dim_t i = 0; i < N; ++i) {
      gecode_intvarvec_t cols_z;
      for (LS::ls_dim_t j = 0; j < N; ++j) cols_z.push_back(z[j*N + i]);
      GC::distinct(*this, cols_z);
    }

    // Enforce element constraints on Z, X, Y:
    for (LS::ls_dim_t i = 0; i < N; ++i) {
      gecode_intvarvec_t Zvec_i;
      for (LS::ls_dim_t j = 0; j < N; ++j) Zvec_i.push_back(z[i*N + j]);
      for (LS::ls_dim_t j = 0; j < N; ++j) {
        GC::element(*this, GC::IntVarArgs(Zvec_i), x[i*N + j], y[i*N + j]);
      }
    }

    if (not this->failed()) {
      LA::post_branching<TWO_MOLS>(*this, V, options);
    }

  }

  TWO_MOLS(TWO_MOLS& T) : GC::Space(T), N(T.N), options(T.options) {
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

  LA::option_t branching_options() const noexcept { assert(valid()); return options; }

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

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  Environment::Index index;
  LS::ls_dim_t N = argc <= index ?
    N_default : read_N(argv[index++], error);
  LS::ls_dim_t k = argc <= index ?
    k_default : read_k(argv[index++], error);
  const LA::option_t options = argc <= index ? LA::option_t{} :
    Environment::translate<LA::option_t>()(argv[index++], LA::sep);
  index++;
  index.deactivate();

  gecode_intvec_t ls1_partial, ls2_partial;

  if (N == 0) {
    std::string s;
    std::cin >> s;
    N = read_N(s, error);
    std::cin >> s;
    k = read_k(s, error);

    ls1_partial = read_partial_ls(N);
    ls2_partial = read_partial_ls(N);
    assert(not ls1_partial.empty());
    assert(not ls2_partial.empty());
  }

  if (k != 2) {
    std::cerr << error << "k > 2 is not implemented yet" << std::endl;
    std::exit(int(RG::Error::domain));
  }

  assert(N > 0 and k > 0);
  const std::shared_ptr<TWO_MOLS> m(new TWO_MOLS(N, options, ls1_partial, ls2_partial));
  assert(m->valid());
  LA::solve<TWO_MOLS>(m, true);

  return 0;
}
