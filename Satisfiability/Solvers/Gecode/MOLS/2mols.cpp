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

*/

/* TODOS:

0. DONE Import standard makefile (OZ)

1. DONE Write application-tests (OZ)

2. Update coding standard (OZ)
    - DONE Use namespace-abbreviations.

3. Likely best for now to remove all verbosity. (OZ)
    - Verbosity in the main output has been removed.
    - Verbosity in the customised Gecode Space (via Tracer) should also be removed.

4. Handle the options for propagation-levels: (OZ)
    - Perhaps command-line options, which are translated into
      enumerated-values, for which one defines switch-statements.
    - Handling all distinct- and all element-constraints for now
      the same.
    - Finding, if possible, the best combination for now.
    - The model is
        LSRG N,2 "-co" "" "1*0,0,m;1*0,0,0" seeds .
    - Collecting some easy statistics.

5. Make the model explicit. (OK, OZ)

6. Fix branching for default (without macros). (OZ)

7. Use symmetry-breaking. (OZ,OK)

*/

#include <iostream>
#include <vector>

#include <math.h>

#include "gecode/driver.hh"
#include "gecode/int.hh"
#include "gecode/search.hh"

#include <ProgramOptions/Environment.hpp>

#define BRANCH_XY 1

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
        "20.5.2021",
        __FILE__,
        "Noah Rubin, Curtis Bright, Oliver Kullmann, and Oleg Zaikin",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/2mols.cpp",
        "GPL v3"};

  namespace GC = Gecode;

}

int copyCount = 0;
bool sym_breaking = false;
bool fix_entries = false;
int * A_init;
int * B_init;

#if VERBOSE == 1
// Override the default standard Tracer for integer variables
// We want to tell the user which variable is being pruned (stored as a char)
class StdIntTracer_VariableIDs : public StdIntTracer {
protected:
  char Type;
public:
  StdIntTracer_VariableIDs() {
    Type = 'N';
  }
  StdIntTracer_VariableIDs(char S) : Type(S) {}
  void prune(const GC::Space& home, const IntTraceRecorder& trace, const ViewTraceInfo& vti, int i, IntTraceDelta& delta) {
    os << "trace<Int>::prune(id:" << trace.id();
    if (trace.group().in())
      os << ",g:" << trace.group().id();

    // Output variable current value (after pruning)
    os << "): " << Type << "[" << i << "] = " << trace[i] << " - {";

    // Output minimum prune value
    os << delta.min();

    // Output maximum prune value
    if (delta.width() > 1)
      os << ".." << delta.max();

    // Iterate over pruned values (delta) and output them
    ++delta;
    while (delta()) {
      os << ',' << delta.min();
      if (delta.width() > 1)
        os << ".." << delta.max();
      ++delta;
    }
    os << "} by " << vti << std::endl;
  }

  // Declare static tracers for X, Y, Z variables
  static StdIntTracer_VariableIDs Sx;
  static StdIntTracer_VariableIDs Sy;
  static StdIntTracer_VariableIDs Sz;
};

// Provide variable names (to be output in the prune() call, helps us figure out what is happening)
StdIntTracer_VariableIDs StdIntTracer_VariableIDs::Sx('X');
StdIntTracer_VariableIDs StdIntTracer_VariableIDs::Sz('Z');
#endif

// We define the model as a sub-space
class TWO_MOLS : public GC::Space {

  // Store dimension and variables
protected:
  int n;
  GC::IntVarArray x, y, z;

  // Provide constructor
public:
  TWO_MOLS(int DIMENSION) : x(*this, (int)pow(DIMENSION, 2), 0, DIMENSION - 1), y(*this, (int)pow(DIMENSION, 2), 0, DIMENSION - 1), z(*this, (int)pow(DIMENSION, 2), 0, DIMENSION - 1) {
    n = DIMENSION;

    if(sym_breaking) {
      // Declare domains for lexocographic ordering
      std::vector<int> domain_lex(n);
      for (int i = 0; i < n; i++) domain_lex[i] = i;

      // Declare domains for Dominance Detection
      std::vector<std::vector<int>> domain_constrained(n);

      // Fix entry (0,1) to 2 in L2
      domain_constrained[1].push_back(2);

      // Fix remaining constrained domains for column 1 in L2
      for (int i = 2; i < n; i++) {
        std::vector<int> domainTemp;
        if (i == (n - 2)) {
          domainTemp.push_back((n - 1));
          domain_constrained[i] = domainTemp;
          continue;
        }
        if (i == (n - 1)) {
          domainTemp.push_back(1);
          for (int j = 3; j <= (n - 2); j++) {
            domainTemp.push_back(j);
          }
          domain_constrained[i] = domainTemp;
          break;
        }
        for (int j = 1; j <= i + 1; j++)
          if (j != i && j != 2) domainTemp.push_back(j);
        domain_constrained[i] = domainTemp;
      }

      // Apply domain constraints to the first rows and columns
      for (int i = 0; i < n; i++) {
        dom(*this, x[i], domain_lex[i]);
        dom(*this, y[i], domain_lex[i]);
        dom(*this, x[i * n], domain_lex[i]);
        if (i > 0) {
          dom(*this, y[i * n], GC::IntSet(GC::IntArgs(domain_constrained[i])));
        }
      }
    }

    if (fix_entries) {
      // Set initialized entries
      for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
          if (A_init[i*n+j] >= 0)
            dom(*this, x[i*n+j], A_init[i*n+j], A_init[i*n+j]);
          if (B_init[i*n+j] >= 0)
            dom(*this, y[i*n+j], B_init[i*n+j], B_init[i*n+j]);
        }
      }
    }

    // Latin property in rows of X
    for (int i = 0; i < n; i++) {
      std::vector<GC::IntVar> rows_x;
      for (int j = 0; j < n; j++) {
        rows_x.push_back(x[i * n + j]);
      }
      distinct(*this, rows_x);
    }

    // Latin property in cols of X
    for (int i = 0; i < n; i++) {
      std::vector<GC::IntVar> cols_x;
      for (int j = 0; j < n; j++) {
        cols_x.push_back(x[j * n + i]);
      }
      GC::distinct(*this, cols_x);
    }

    // Latin property in rows of Y
    for (int i = 0; i < n; i++) {
      std::vector<GC::IntVar> rows_y;
      for (int j = 0; j < n; j++) {
        rows_y.push_back(y[i * n + j]);
      }
      GC::distinct(*this, rows_y);
    }

    // Latin property in cols of Y
    for (int i = 0; i < n; i++) {
      std::vector<GC::IntVar> cols_y;
      for (int j = 0; j < n; j++) {
        cols_y.push_back(y[j * n + i]);
      }
      GC::distinct(*this, cols_y);
    }

    // Row uniqueness of Z
    for (int i = 0; i < n; i++) {
      std::vector<GC::IntVar> rows_z;
      for (int j = 0; j < n; j++) {
        rows_z.push_back(z[i * n + j]);
      }
      GC::distinct(*this, rows_z);
    }

    // Column uniqueness of Z
    for (int i = 0; i < n; i++) {
      std::vector<GC::IntVar> cols_z;
      for (int j = 0; j < n; j++) {
        cols_z.push_back(z[j * n + i]);
      }
      GC::distinct(*this, cols_z);
    }

    // Enforce element constraints on Z, X, Y
    for (int i = 0; i < n; i++)
      {
        std::vector<GC::IntVar> Zvec_i;
        for (int j = 0; j < n; j++) {
          Zvec_i.push_back(z[i * n + j]);

        }
        for (int j = 0; j < n; j++) {
          element(*this, GC::IntVarArgs(Zvec_i), x[i * n + j], y[i * n + j]);
        }
      }


    // Branch first on Xs, then Ys, then Zs

    // Branch strategy: select variable w/ smallest domain size --> select its minimum value:
#if BRANCH_XY == 0
    GC::branch(*this, x, GC::INT_VAR_NONE(), GC::INT_VAL_MIN());
    GC::branch(*this, y, GC::INT_VAR_NONE(), GC::INT_VAL_MIN());
#elif BRANCH_XY == 1
    for (int i = 0; i < x.size(); i++)
      {
        GC::branch(*this, x[i], GC::INT_VAL_MIN());
        GC::branch(*this, y[i], GC::INT_VAL_MIN());
      }
#endif
    GC::branch(*this, z, GC::INT_VAR_NONE(), GC::INT_VAL_MIN());

    // Apply tracers to search to print out variable domain reductions
#if VERBOSE == 1
    GC::trace(*this, x, TE_COMMIT | TE_INIT | TE_DONE | TE_FAIL | TE_POST | TE_PROPAGATE | TE_PRUNE, StdIntTracer_VariableIDs::Sx);
    GC::trace(*this, y, TE_COMMIT | TE_INIT | TE_DONE | TE_FAIL | TE_POST | TE_PROPAGATE | TE_PRUNE, StdIntTracer_VariableIDs::Sy);
    GC::trace(*this, z, TE_COMMIT | TE_INIT | TE_DONE | TE_FAIL | TE_POST | TE_PROPAGATE | TE_PRUNE, StdIntTracer_VariableIDs::Sz);
#endif

  }


  // Provide copy constructor for committing
  TWO_MOLS(TWO_MOLS& T) : GC::Space(T){
    x.update(*this, T.x);
    y.update(*this, T.y);
    this->n = T.n;
  }


  // Provide instruction for copy within search
  virtual GC::Space* copy(void) {
    copyCount++;
#if VERBOSE == 1
    if (this->stable()) {
      const Choice* ch = this->choice();
      //this->print(*ch, 0, std::cout);
      //cout << "\n";
      //delete ch;
    }
#endif
    return new TWO_MOLS(*this);
  }

  // Provide function to print solutions
  void Print() {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        std::cout << x[i * n + j] << " ";
      }
      std::cout << "\n";
    }

    std::cout << "\n";

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        std::cout << y[i * n + j] << " ";
      }
      std::cout << "\n";
    }

  }

};

// Driver
int main(int argc, char *argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;

  // Parse dimension
  int n = 0;
  if (argc == 2) {
    n = atoi(argv[1]);
    sym_breaking = true;
  }
  else {
    fix_entries = true;
    std::string str;
    std::cout << "Format: Enter order; newline; entries of first square; newline; entries of second square" << std::endl;
    std::cout << "Use asterisks for undefined entries" << std::endl;
    std::cin >> n;
    getline(std::cin, str);
    getline(std::cin, str);
    assert(str.empty());

    A_init = new int[n*n];
    B_init = new int[n*n];

    for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
        std::cin >> str;
        A_init[i*n+j] = (str == "*") ? -1 : stoi(str);
      }
    }
    getline(std::cin, str);
    getline(std::cin, str);
    assert(str.empty());

    for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
        std::cin >> str;
        B_init[i*n+j] = (str == "*") ? -1 : stoi(str);
      }
    }

  }

  // Declare new search space
  TWO_MOLS *T = new TWO_MOLS(n);

  // Declare options - the Tracer will tell us when the search branches and on what value
  GC::Search::Options *O = new GC::Search::Options();
  O->threads = 1;
#if VERBOSE == 1
  O->tracer = &StdSearchTracer::def;
#endif

  // Execute Depth-First Search on the space
  GC::DFS<TWO_MOLS> e(T, *O);

  // Free up old space
  delete T;

  // Grab first solution from DFS or else we have no solutions
  if (TWO_MOLS * S = e.next()) {
    S->Print();
  }
  else {
    std::cout << "No solutions found\n";
  }

  if(fix_entries) {
    delete [] A_init;
    delete [] B_init;
  }

  return 0;
}
