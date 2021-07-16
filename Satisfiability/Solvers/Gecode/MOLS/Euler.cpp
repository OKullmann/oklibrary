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

    - Perhaps due to propagations too weak?
    - Currently (feb6a83b0271f7647c34e5290a6008728e2502dc) the completion-mode
      is disabled (needs to be re-abled soon).
    - Needs to be investigated historically, and then needs to become an
      application test.

2. for (( i=0; i<10000; ++i )); do res="$(LSRG 5,2 "-co" "1*0,0,25;1*0,0,0" t | ./Euler | tr -s " " | cut -d" " -f 4)"; echo -n $res; done
yields only unsatisfiable instances:
    - For small N the probabilities should be all known.
    - This seems to be a bug.

*/

/* TODOS:

-2. No trivial typedefs.

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

1. Determine the precise meaning of the branchers:
    - Do different branchers create a common pool?
    - It seems sensible here to use *one* basic vector of variables, which
      contains *all* variables.
    - Helper functions perform all the index-computations (so that the code is easy
      to read).

2. Update coding standard (OZ)
    - DONE Use namespace-abbreviations.
    - Use proper types.
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
        "0.4.4",
        "16.7.2021",
        __FILE__,
        "Noah Rubin, Curtis Bright, Oliver Kullmann, and Oleg Zaikin",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/2mols.cpp",
        "GPL v3"};

  namespace GC = Gecode;
  namespace LA = Lookahead;
  namespace LS = LatinSquares;
  namespace RG = RandGen;

  typedef LA::BrTypeO BrTpO;
  typedef LA::BrSourceO BrSrcO;
  typedef LA::BrMeasureO BrMsrO;
  typedef LA::BrSolutionO BrSltnO;
  typedef LA::option_t option_t;
  typedef LS::ls_dim_t ls_dim_t;
  typedef std::vector<int> gecode_vector_t;

  constexpr ls_dim_t N_default = 6;
  constexpr ls_dim_t k_default = 2;

  bool fixed_entries = false;
  int * A_init;
  int * B_init;

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
    " algorithmic-options : " << Environment::WRP<BrTpO>{} << "\n" <<
    "                     : " << Environment::WRP<BrSrcO>{} << "\n" <<
    "                     : " << Environment::WRP<BrMsrO>{} << "\n" <<
    "                     : " << Environment::WRP<BrSltnO>{} << "\n" <<
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

  ls_dim_t read_N(const std::string s, const std::string& error) noexcept {
    if (s.empty()) return N_default;
    const ls_dim_t N = FloatingPoint::touint(s);
    if (not LS::valid(N) and N != 0) {
      std::cerr << error << "N must be a nonnegative integer in [0,"
                << LS::max_dim-1 << "]" << ", but N=" << N << ".\n";
      std::exit(int(RG::Error::domain));
    }
    return N;
  }

  ls_dim_t read_k(const std::string s, const std::string& error) noexcept {
    if (s.empty()) return k_default;
    const ls_dim_t k = FloatingPoint::touint(s);
    if (not LS::valid(k) and k != 0) {
      std::cerr << error << "k must be a nonnegative integer in [0,"
                << LS::max_dim-1 << "]" << ", but k=" << k << ".\n";
      std::exit(int(RG::Error::domain));
    }
    return k;
  }

}

class TWO_MOLS : public GC::Space {
  const ls_dim_t N;
  const option_t options;
  GC::IntVarArray x, y, z, V;

  inline size_t x_index(const size_t i) const noexcept { return i; }
  inline size_t y_index(const size_t i) const noexcept { return i + LA::tr(x.size()); }
  inline size_t z_index(const size_t i) const noexcept {
    return i + LA::tr(x.size()) + LA::tr(y.size());
  }

public:
  TWO_MOLS(const ls_dim_t N, const option_t options) :
    N(N), options(options),
    x(*this, (ls_dim_t)std::pow(N, 2), 0, N - 1),
    y(*this, (ls_dim_t)std::pow(N, 2), 0, N - 1),
    z(*this, (ls_dim_t)std::pow(N, 2), 0, N - 1),
    V(*this, x.size() + y.size() + z.size(), 0, N - 1) {

    assert(valid());

    // Use an umbrella variable array for all variables:
    for (size_t i = 0; i < LA::tr(x.size()); ++i) V[x_index(i)] = x[i];
    for (size_t i = 0; i < LA::tr(y.size()); ++i) V[y_index(i)] = y[i];
    for (size_t i = 0; i < LA::tr(z.size()); ++i) V[z_index(i)] = z[i];

    // Symmetry breaking if no partially filled square is given:
    // Declare domains for lexicographic ordering
    gecode_vector_t domain_lex(N);
    for (ls_dim_t i = 0; i < N; ++i) domain_lex[i] = i;
    // Declare domains for Dominance Detection
    std::vector<gecode_vector_t> domain_constrained(N);
    // Fix entry (0,1) to 2 in L2
    domain_constrained[1].push_back(2);
    // Fix remaining constrained domains for column 1 in L2
    for (ls_dim_t i = 2; i < N; ++i) {
      gecode_vector_t domainTemp;
      if (i == (N - 2)) {
        domainTemp.push_back((N - 1));
        domain_constrained[i] = domainTemp;
        continue;
      }
      if (i == (N - 1)) {
        domainTemp.push_back(1);
        for (ls_dim_t j = 3; j <= (N - 2); ++j) {
          domainTemp.push_back(j);
        }
        domain_constrained[i] = domainTemp;
        break;
      }
      for (ls_dim_t j = 1; j <= i + 1; ++j)
        if (j != i && j != 2) domainTemp.push_back(j);
      domain_constrained[i] = domainTemp;
    }

    // Apply domain constraints to the first rows and columns
    for (ls_dim_t i = 0; i < N; ++i) {
      GC::dom(*this, x[i], domain_lex[i]);
      GC::dom(*this, y[i], domain_lex[i]);
      GC::dom(*this, x[i * N], domain_lex[i]);
      if (i > 0) {
        GC::dom(*this, y[i * N], GC::IntSet(GC::IntArgs(domain_constrained[i])));
      }
    }

    // ???
    if (fixed_entries) {
			for(ls_dim_t i = 0; i < N; ++i) {
				for(ls_dim_t j = 0; j < N; ++j) {
					if (A_init[i*N+j] >= 0)
						dom(*this, x[i*N+j], A_init[i*N+j], A_init[i*N+j]);
					if (B_init[i*N+j] >= 0)
						dom(*this, y[i*N+j], B_init[i*N+j], B_init[i*N+j]);
				}
			}
		}

    // Latin property in rows of X
    for (ls_dim_t i = 0; i < N; ++i) {
      std::vector<GC::IntVar> rows_x;
      for (ls_dim_t j = 0; j < N; ++j)
        rows_x.push_back(x[i * N + j]);
      GC::distinct(*this, rows_x);
    }

    // Latin property in cols of X
    for (ls_dim_t i = 0; i < N; ++i) {
      std::vector<GC::IntVar> cols_x;
      for (ls_dim_t j = 0; j < N; ++j)
        cols_x.push_back(x[j * N + i]);
      GC::distinct(*this, cols_x);
    }

    // Latin property in rows of Y
    for (ls_dim_t i = 0; i < N; ++i) {
      std::vector<GC::IntVar> rows_y;
      for (ls_dim_t j = 0; j < N; ++j)
        rows_y.push_back(y[i * N + j]);
      GC::distinct(*this, rows_y);
    }

    // Latin property in cols of Y
    for (ls_dim_t i = 0; i < N; ++i) {
      std::vector<GC::IntVar> cols_y;
      for (ls_dim_t j = 0; j < N; ++j)
        cols_y.push_back(y[j * N + i]);
      GC::distinct(*this, cols_y);
    }

    // Row uniqueness of Z
    for (ls_dim_t i = 0; i < N; ++i) {
      std::vector<GC::IntVar> rows_z;
      for (ls_dim_t j = 0; j < N; ++j)
        rows_z.push_back(z[i * N + j]);
      GC::distinct(*this, rows_z);
    }

    // Column uniqueness of Z
    for (ls_dim_t i = 0; i < N; ++i) {
      std::vector<GC::IntVar> cols_z;
      for (ls_dim_t j = 0; j < N; ++j)
        cols_z.push_back(z[j * N + i]);
      GC::distinct(*this, cols_z);
    }

    // Enforce element constraints on Z, X, Y
    for (ls_dim_t i = 0; i < N; ++i) {
      std::vector<GC::IntVar> Zvec_i;
      for (ls_dim_t j = 0; j < N; ++j)
        Zvec_i.push_back(z[i * N + j]);
      for (ls_dim_t j = 0; j < N; ++j)
        GC::element(*this, GC::IntVarArgs(Zvec_i), x[i * N + j], y[i * N + j]);
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
    return x.size() > 0 and V.size() == x.size() * 3;
  }
  inline bool valid (const size_t i) const noexcept {return i<LA::tr(V.size());}

  inline GC::IntVar at(const size_t i) const noexcept {
    assert(valid()); assert(valid(i));
    return V[i];
  }
  inline GC::IntVarArray at() const noexcept { assert(valid()); return V; }

  option_t branching_options() const noexcept { assert(valid()); return options; }

  void print() {
    assert(valid());
    for (ls_dim_t i = 0; i < N; ++i) {
      for (ls_dim_t j = 0; j < N; ++j) {
        std::cout << x[i * N + j];
        if (j < N-1) std::cout << " ";
      }
      std::cout << "\n";
    }
    std::cout << "\n";
    for (ls_dim_t i = 0; i < N; ++i) {
      for (ls_dim_t j = 0; j < N; ++j) {
        std::cout << y[i * N + j];
        if (j < N-1) std::cout << " ";
      }
      std::cout << "\n";
    }
  }

};

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  Environment::Index index;
  ls_dim_t N = argc <= index ?
    N_default : read_N(argv[index++], error);
  ls_dim_t k = argc <= index ?
    k_default : read_k(argv[index++], error);
  const option_t options = argc <= index ? option_t{} :
    Environment::translate<option_t>()(argv[index++], LA::sep);
  index++;
  index.deactivate();

  if (N == 0) {
    std::string s;
    std::cin >> s;
    N = read_N(s, error);
    std::cin >> s;
    k = read_k(s, error);
    getline(std::cin, s);
    assert(s.empty());

    A_init = new int[N*N];
    B_init = new int[N*N];

    for(ls_dim_t i = 0; i < N; ++i) {
      for(ls_dim_t j = 0; j < N; ++j) {
        std::cin >> s;
        A_init[i*N+j] = (s == "*") ? -1 : stoi(s);
      }
    }
    getline(std::cin, s);
    getline(std::cin, s);
    assert(s.empty());

    for(ls_dim_t i = 0; i < N; ++i) {
      for(ls_dim_t j = 0; j < N; ++j) {
        std::cin >> s;
        B_init[i*N+j] = (s == "*") ? -1 : stoi(s);
      }
    }
  }

  if (k != 2) {
    // XXX
    std::cerr << error << "k > 2 is not implemented yet" << std::endl;
    std::exit(int(RG::Error::domain));
  }

  const std::shared_ptr<TWO_MOLS> m(new TWO_MOLS(N, options));
  assert(m->valid());
  LA::solve<TWO_MOLS>(m, true);

  if(fixed_entries) {
		delete [] A_init;
		delete [] B_init;
	}

  return 0;
}
