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

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.1",
        "12.7.2021",
        __FILE__,
        "Noah Rubin, Curtis Bright, Oliver Kullmann, and Oleg Zaikin",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/2mols.cpp",
        "GPL v3"};

  namespace GC = Gecode;
  namespace LA = Lookahead;

  typedef LA::BrTypeO BrTpO;
  typedef LA::BrSourceO BrSrcO;
  typedef LA::BrMeasureO BrMsrO;
  typedef LA::BrSolutionO BrSltnO;
  typedef LA::option_t option_t;

  constexpr int N_default = 10;

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

int copyCount = 0;
bool sym_breaking = false;
bool fix_entries = false;
int* A_init;
int* B_init;

class TWO_MOLS : public GC::Space {

protected:
  int n;
  GC::IntVarArray x, y, z, V;

  inline LA::size_t x_index(const LA::size_t i) const noexcept { return i; }
  inline LA::size_t y_index(const LA::size_t i) const noexcept { return i + LA::tr(x.size()); }
  inline LA::size_t z_index(const LA::size_t i) const noexcept {
    return i + LA::tr(x.size()) + LA::tr(y.size());
  }

public:
  TWO_MOLS(int DIMENSION) :
    x(*this, (int)std::pow(DIMENSION, 2), 0, DIMENSION - 1),
    y(*this, (int)std::pow(DIMENSION, 2), 0, DIMENSION - 1),
    z(*this, (int)std::pow(DIMENSION, 2), 0, DIMENSION - 1),
    V(*this, x.size() + y.size() + z.size(), 0, DIMENSION - 1) {
    n = DIMENSION;

    // Use an umbrella variable array for all variables:
    for (LA::size_t i = 0; i < LA::tr(x.size()); ++i) V[x_index(i)] = x[i];
    for (LA::size_t i = 0; i < LA::tr(y.size()); ++i) V[y_index(i)] = y[i];
    for (LA::size_t i = 0; i < LA::tr(z.size()); ++i) V[z_index(i)] = z[i];

    if (sym_breaking) {
      // Declare domains for lexicographic ordering
      std::vector<int> domain_lex(n);
      for (int i = 0; i < n; ++i) domain_lex[i] = i;

      // Declare domains for Dominance Detection
      std::vector<std::vector<int>> domain_constrained(n);

      // Fix entry (0,1) to 2 in L2
      domain_constrained[1].push_back(2);

      // Fix remaining constrained domains for column 1 in L2
      for (int i = 2; i < n; ++i) {
        std::vector<int> domainTemp;
        if (i == (n - 2)) {
          domainTemp.push_back((n - 1));
          domain_constrained[i] = domainTemp;
          continue;
        }
        if (i == (n - 1)) {
          domainTemp.push_back(1);
          for (int j = 3; j <= (n - 2); ++j) {
            domainTemp.push_back(j);
          }
          domain_constrained[i] = domainTemp;
          break;
        }
        for (int j = 1; j <= i + 1; ++j)
          if (j != i && j != 2) domainTemp.push_back(j);
        domain_constrained[i] = domainTemp;
      }

      // Apply domain constraints to the first rows and columns
      for (int i = 0; i < n; ++i) {
        GC::dom(*this, x[i], domain_lex[i]);
        GC::dom(*this, y[i], domain_lex[i]);
        GC::dom(*this, x[i * n], domain_lex[i]);
        if (i > 0) {
          GC::dom(*this, y[i * n], GC::IntSet(GC::IntArgs(domain_constrained[i])));
        }
      }
    }

    if (fix_entries) {
      for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
          if (A_init[i*n+j] >= 0)
            GC::dom(*this, x[i*n+j], A_init[i*n+j], A_init[i*n+j]);
          if (B_init[i*n+j] >= 0)
            GC::dom(*this, y[i*n+j], B_init[i*n+j], B_init[i*n+j]);
        }
      }
    }

    // Latin property in rows of X
    for (int i = 0; i < n; ++i) {
      std::vector<GC::IntVar> rows_x;
      for (int j = 0; j < n; ++j)
        rows_x.push_back(x[i * n + j]);
      GC::distinct(*this, rows_x);
    }

    // Latin property in cols of X
    for (int i = 0; i < n; ++i) {
      std::vector<GC::IntVar> cols_x;
      for (int j = 0; j < n; ++j)
        cols_x.push_back(x[j * n + i]);
      GC::distinct(*this, cols_x);
    }

    // Latin property in rows of Y
    for (int i = 0; i < n; ++i) {
      std::vector<GC::IntVar> rows_y;
      for (int j = 0; j < n; ++j)
        rows_y.push_back(y[i * n + j]);
      GC::distinct(*this, rows_y);
    }

    // Latin property in cols of Y
    for (int i = 0; i < n; ++i) {
      std::vector<GC::IntVar> cols_y;
      for (int j = 0; j < n; ++j)
        cols_y.push_back(y[j * n + i]);
      GC::distinct(*this, cols_y);
    }

    // Row uniqueness of Z
    for (int i = 0; i < n; ++i) {
      std::vector<GC::IntVar> rows_z;
      for (int j = 0; j < n; ++j)
        rows_z.push_back(z[i * n + j]);
      GC::distinct(*this, rows_z);
    }

    // Column uniqueness of Z
    for (int i = 0; i < n; ++i) {
      std::vector<GC::IntVar> cols_z;
      for (int j = 0; j < n; ++j)
        cols_z.push_back(z[j * n + i]);
      GC::distinct(*this, cols_z);
    }

    // Enforce element constraints on Z, X, Y
    for (int i = 0; i < n; ++i) {
      std::vector<GC::IntVar> Zvec_i;
      for (int j = 0; j < n; ++j)
        Zvec_i.push_back(z[i * n + j]);
      for (int j = 0; j < n; ++j)
        GC::element(*this, GC::IntVarArgs(Zvec_i), x[i * n + j], y[i * n + j]);
    }

    if (not this->failed()) {
      const option_t options = {BrTpO::mind, BrSrcO::v, BrMsrO::mu1, BrSltnO::all};
      LA::post_branching<TWO_MOLS>(*this, V, options);
    }

  }


  TWO_MOLS(TWO_MOLS& T) : GC::Space(T) {
    x.update(*this, T.x);
    y.update(*this, T.y);
    z.update(*this, T.z);
    V.update(*this, T.V);
    this->n = T.n;
  }


  virtual GC::Space* copy() {
    copyCount++;
    return new TWO_MOLS(*this);
  }

  inline bool valid () const noexcept {return valid(V);}
  inline bool valid (const GC::IntVarArray V) const noexcept {
    return V.size() == x.size() + y.size() + z.size();
  }
  inline bool valid (const LA::size_t i) const noexcept {return i<LA::tr(V.size());}

  inline GC::IntVar at(const LA::size_t i) const noexcept {
    assert(valid()); assert(valid(i));
    return V[i];
  }
  inline GC::IntVarArray at() const noexcept { assert(valid()); return V; }

  void Print() {
    assert(n > 0);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        std::cout << x[i * n + j];
        if (j < n-1) std::cout << " ";
      }
      std::cout << "\n";
    }

    std::cout << "\n";

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        std::cout << y[i * n + j];
        if (j < n-1) std::cout << " ";
      }
      std::cout << "\n";
    }

  }

};

// Driver
int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  int n = 0;
  if (argc == 2) {
    n = atoi(argv[1]);
    sym_breaking = true;
  }
  else {
    fix_entries = true;
    std::string str;
    std::cin >> n;
    getline(std::cin, str);
    getline(std::cin, str);
    assert(str.empty());

    A_init = new int[n*n];
    B_init = new int[n*n];

    for(int i = 0; i < n; ++i) {
      for(int j = 0; j < n; ++j) {
        std::cin >> str;
        A_init[i*n+j] = (str == "*") ? -1 : stoi(str);
      }
    }
    getline(std::cin, str);
    getline(std::cin, str);
    assert(str.empty());

    for(int i = 0; i < n; ++i) {
      for(int j = 0; j < n; ++j) {
        std::cin >> str;
        B_init[i*n+j] = (str == "*") ? -1 : stoi(str);
      }
    }

  }

  TWO_MOLS* const T = new TWO_MOLS(n);

  GC::Search::Options *O = new GC::Search::Options();
  O->threads = 1;

  GC::DFS<TWO_MOLS> e(T, *O);

  delete T;

  if (TWO_MOLS * S = e.next())
    S->Print();
  else
    std::cout << "No solutions found\n";

  if(fix_entries) {
    delete [] A_init;
    delete [] B_init;
  }

  return 0;
}
