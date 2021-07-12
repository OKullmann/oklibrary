/*
  Imported 7.5.2021 by Oliver Kullmann.
  Original notes:

  Noah Rubin,  modifications by Curtis Bright

  This program implements the 2MOLS(n) CP model using GECODE.

  Notes:
  - Only provide path to ~/GECODE/lib and ~/GECODE/include (header automatically links)
  - Options are vague and not always honored by the search engine
  - Branching is specified manually, and we have a lot of control over the internals
  - The Space is copied many times during the search
  - Using LDS (Limited Discrepency Search) speeds up the time massively for n = 7


BUGS:

0. FIXED -h does not work.

1. > LSRG 6,2 "-co" "1*0,0,36;1*0,0,0" 0 | ./Euler
takes a long time (say one minutes).

    - Perhaps due to propagations too weak?

2. DONE Makefile: Remove the location of gecode (for the public clone).


*/

/* TODOS:

-1. Extend makefile, so that all variations are automatically created.
    - Possibly using a prefix

0. DONE Rename Euler.cpp.

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

7. Design the input format:
    - One option-argument specifies whether it is a completion-problem or not.
    - -comp, +comp possibly to turn off/on the completion-mode.
    - N is likely best to provide also for the completion-problem, and checked (possibly
      optional for completition-on).

8. Design for general k
    - Based on the "constraint" LS(a,b,c) for a,b,c in {1, ..., k+2}.
    - Just posting all binom(N+2,3) such LS-constraints, plus the equalities
      (element-constraints) between these LS's.

*/

#include <iostream>
#include <vector>

#include <cmath>

#include "gecode/driver.hh"
#include "gecode/int.hh"
#include "gecode/search.hh"

#include <ProgramOptions/Environment.hpp>

#include "../Lookahead.hpp"
#include "LatinSquares.hpp"
#include "LSRG.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.1",
        "12.7.2021",
        __FILE__,
        "Noah Rubin, Curtis Bright, Oliver Kullmann, and Oleg Zaikin",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/2mols.cpp",
        "GPL v3"};

  namespace GC = Gecode;
  namespace LA = Lookahead;
  namespace LS = LatinSquares;

  typedef LA::BrTypeO BrTpO;
  typedef LA::BrSourceO BrSrcO;
  typedef LA::BrMeasureO BrMsrO;
  typedef LA::BrSolutionO BrSltnO;
  typedef LA::option_t option_t;
  typedef LSRG::Dim Dim;
  typedef LS::ls_dim_t ls_dim_t;
  typedef std::vector<int> gecode_vector_t;

  constexpr ls_dim_t N_default = 6;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [N] [branching-options]\n\n"
    " N                 : default = " << N_default << "\n" <<
    " branching-options : " << Environment::WRP<BrTpO>{} << "\n"
    "                   : " << Environment::WRP<BrSrcO>{} << "\n"
    "                   : " << Environment::WRP<BrMsrO>{} << "\n"
    "                   : " << Environment::WRP<BrSltnO>{} << "\n";
    std::cout <<
    "For a given N and a partially filled Latin square, solves the " <<
    "Euler square completion problem.\n\n";
    return true;
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

  using LS::ls_dim_t;
  const Dim D = argc <= index ?
    Dim{} : LSRG::read_N(argv[index++], error);

  const option_t options = argc <= index ? option_t{} :
    Environment::translate<option_t>()(argv[index++], LA::sep);

  index++;
  index.deactivate();

  const std::shared_ptr<TWO_MOLS> m(new TWO_MOLS(D.N, options));
  assert(m->valid());

  LA::solve<TWO_MOLS>(m, true);

  return 0;
}
