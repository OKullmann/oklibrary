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

0. Rename Euler.cpp.

1. Determine the precise meaning of the branchers:
    - Do different branchers create a common pool?

2. Update coding standard (OZ)
    - DONE Use namespace-abbreviations.
    - Use proper types.
    - Provide version and help.

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

#include <math.h>

#include "gecode/driver.hh"
#include "gecode/int.hh"
#include "gecode/search.hh"

#include <ProgramOptions/Environment.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "21.5.2021",
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

// We define the model as a sub-space
class TWO_MOLS : public GC::Space {

  // Store dimension and variables
protected:
  int n;
  GC::IntVarArray x, y, z;

  // Provide constructor
public:
  TWO_MOLS(int DIMENSION) : x(*this, (int)pow(DIMENSION, 2), 0, DIMENSION - 1),
                            y(*this, (int)pow(DIMENSION, 2), 0, DIMENSION - 1),
                            z(*this, (int)pow(DIMENSION, 2), 0, DIMENSION - 1) {
    n = DIMENSION;

    if (sym_breaking) {
      // Declare domains for lexicographic ordering
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
        GC::dom(*this, x[i], domain_lex[i]);
        GC::dom(*this, y[i], domain_lex[i]);
        GC::dom(*this, x[i * n], domain_lex[i]);
        if (i > 0) {
          GC::dom(*this, y[i * n], GC::IntSet(GC::IntArgs(domain_constrained[i])));
        }
      }
    }

    if (fix_entries) {
      // Set initialized entries
      for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
          if (A_init[i*n+j] >= 0)
            GC::dom(*this, x[i*n+j], A_init[i*n+j], A_init[i*n+j]);
          if (B_init[i*n+j] >= 0)
            GC::dom(*this, y[i*n+j], B_init[i*n+j], B_init[i*n+j]);
        }
      }
    }

    // Latin property in rows of X
    for (int i = 0; i < n; i++) {
      std::vector<GC::IntVar> rows_x;
      for (int j = 0; j < n; j++) {
        rows_x.push_back(x[i * n + j]);
      }
      GC::distinct(*this, rows_x);
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
    for (int i = 0; i < n; i++) {
      std::vector<GC::IntVar> Zvec_i;
      for (int j = 0; j < n; j++)
        Zvec_i.push_back(z[i * n + j]);
      for (int j = 0; j < n; j++)
        GC::element(*this, GC::IntVarArgs(Zvec_i), x[i * n + j], y[i * n + j]);
    }

    // Branch strategy: select variable w/ smallest domain size --> select its minimum value:
    GC::branch(*this, x, GC::INT_VAR_SIZE_MIN(), GC::INT_VAL_MIN());
    GC::branch(*this, y, GC::INT_VAR_SIZE_MIN(), GC::INT_VAL_MIN());
    GC::branch(*this, z, GC::INT_VAR_SIZE_MIN(), GC::INT_VAL_MIN());

  }


  // Provide copy constructor for committing
  TWO_MOLS(TWO_MOLS& T) : GC::Space(T){
    x.update(*this, T.x);
    y.update(*this, T.y);
    z.update(*this, T.z);
    this->n = T.n;
  }


  // Provide instruction for copy within search
  virtual GC::Space* copy(void) {
    copyCount++;
    return new TWO_MOLS(*this);
  }

  // Provide function to print solutions
  void Print() {
    assert(n > 0);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        std::cout << x[i * n + j];
        if (j < n-1) std::cout << " ";
      }
      std::cout << "\n";
    }

    std::cout << "\n";

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        std::cout << y[i * n + j];
        if (j < n-1) std::cout << " ";
      }
      std::cout << "\n";
    }

  }

};

// Driver
int main(int argc, char *argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;

  //std::cout << "Format: Enter order; newline; entries of first square; newline; entries of second square" << std::endl;
  //std::cout << "Use asterisks for undefined entries" << std::endl;

  // Parse dimension
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

  // Declare options
  GC::Search::Options *O = new GC::Search::Options();
  O->threads = 1;

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
