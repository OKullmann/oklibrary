/*********************************************************************
tawSolver -- Copyright (c) 2007-2013, Tanbir Ahmed
http://users.encs.concordia.ca/~ta_ahmed/

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**********************************************************************/

/*
  Compile with

> g++ --std=c++11 -Wall -Ofast -funroll-loops -fwhole-program -DNDEBUG -o tawSolver tawSolver.cpp

(or with "g++ --std=c++11 -Wall -g -o tawSolver tawSolver.cpp"
for debugging).

  Usage:

> tawSolver [argument]

   - without argument shows usage
   - with argument= "-v" or "--version" shows information
   - with argument=filename runs the SAT solver.

  When sending SIGINT to the program (for example via CTRL-C from the calling
  terminal), then the current state of statistics is output, and computation
  is aborted. When sending SIGUSR1, then the statistics is output, and the
  computation is continued.

  There are three macros to control compilation:
   - LIT_TYPE (default int)
   - BASIS_WEIGHT1 (default 7.5) and BASIS_WEIGHT2 (default 2.0):
     clauses of length 2,3,4,5,... have weight
     BASIS_WEIGHT1, 1, BASIS_WEIGHT2^-1, BASIS_WEIGHT2^-2, ... .

  To provide further versioning-information, there are two macros, which are
  only relevant if they are defined:
   - GIT_ID (for the identity hash-value of the Git repository)
   - OKLIB (with the url (without prefix "http://") for the OKlibrary.
*/

#include <limits>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <type_traits>
#include <stack>
#include <iomanip>

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <csignal>

namespace {

const std::string version = "1.7.0";
const std::string date = "6.7.2013";

const std::string program = "tawSolver";
const std::string err = "ERROR[" + program + "]: ";

enum Error_codes {
  file_reading_error=1,
  num_vars_error=2,
  variable_value_error=3,
  number_clauses_error=4,
  empty_clause_error=5,
  unit_clause_error=6
};

enum Result_value { unsat=20, sat=10, unknown=0 };
Result_value interprete_run(const bool result) { return result ? sat : unsat; }

#ifndef LIT_TYPE
# define LIT_TYPE int
#endif
typedef LIT_TYPE Lit;
static_assert(std::is_signed<Lit>::value, "Type \"Lit\" must be signed integral.");
static_assert(sizeof(Lit) != 1, "LIT_TYPE = char (or int8_t) doesn't work with reading (since not numbers are read, but characters).");

typedef std::make_unsigned<Lit>::type Var;

struct Clause {
  Lit* literals; // the array of literals in the clause
  Lit* end;
  int length; // the current length
  bool status; // true iff currently not satisfied
};
// Members "literals", "end" and "index" are fixed after reading the input.
typedef Clause* ClauseP;
typedef const Clause* ClausePc;

std::vector<Clause> clauses;

struct literal_occurrences {
  ClauseP* occur; // array with clause-pointers
  unsigned int n_occur;
};
std::vector<std::array<literal_occurrences,2>> lits;
/* lits[v][pos/neg] for a variable v represents the list of occurrences; this
   data is fixed with the input. */

// The stack of touched clauses:
typedef std::vector<ClauseP> Change_v;
typedef Change_v::size_type change_index_t;
Change_v changes(1); // acts as a global stack
change_index_t changes_index = 0; // Invariant: changes_index < changes.size().

unsigned int max_clause_length = 0;

// the clause-weights:
#ifdef BASIS_WEIGHT1
  constexpr double basis_w_1 = BASIS_WEIGHT1;
#else
  constexpr double basis_w_1 = 7.5;
#endif
#ifdef BASIS_WEIGHT2
  constexpr double basis_w_2 = BASIS_WEIGHT2;
#else
  constexpr double basis_w_2 = 2.0;
#endif
std::vector<double> weights {0,0, basis_w_1, 1, 1/basis_w_2};
constexpr int first_open_weight = 5;
void initialise_weights() {
  for (int i = first_open_weight; unsigned(i) <= max_clause_length; ++i)
    weights[i] = std::pow(basis_w_2,-i+3);
}
// Remark: weights[i] == 0 iff i >= 1078 (for basis_w_2 == 2).

unsigned int n_header_clauses, n_clauses, r_clauses; // "r" = "remaining"
Var n_vars;

std::vector<Lit> pass; /* the current assignment: pass[v] is 0 iff variable
 v is unassigned, otherwise it is v in case v->true and else -v. */

unsigned long long int n_nodes = 0;
unsigned long long int n_units = 0;
unsigned long long int n_backtracks = 0;


inline Var var(const Lit x) { return std::abs(x); }
enum Polarity { pos=0, neg=1 };
inline Polarity sign(const Lit x) { return (x > 0) ? pos : neg; }
inline Polarity inv_polarity(const Polarity p) { return (p == pos) ? neg:pos; }

// to handle the branching-assignment plus the derived assignments:
std::vector<Lit> gucl_stack;
int n_gucl = 0; // the index of the next free element of the stack
bool contradictory_unit_clauses = false;

// --- Input and initialisation ---

void read_formula_header(std::ifstream& f) {
  std::string line;
  while (true) {
    std::getline(f, line);
    if (not f) {
      std::cerr << err << "Reading error.\n";
      std::exit(file_reading_error);
    }
    if (line[0] == 'p') break;
  }
  std::stringstream s(line);
  {std::string inp; s >> inp;
   if (inp != "p") {
     std::cerr << err << "Syntax error in parameter line (no \"p \").\n";
     std::exit(file_reading_error);
   }
   s >> inp;
   if (inp != "cnf") {
     std::cerr << err << "Syntax error in parameter line (no \"cnf\").\n";
     std::exit(file_reading_error);
   }
  }
  s >> n_vars;
  if (n_vars > Var(std::numeric_limits<Lit>::max())) {
    std::cerr << err << "Parameter n=" << n_vars << " is too big for numeric_limits<Lit>::max=" << std::numeric_limits<Lit>::max() << "\n";
    std::exit(num_vars_error);
  }
  s >> n_header_clauses;
  if (not s) {
    std::cerr << err << "Reading error with parameters.\n";
    std::exit(file_reading_error);
  }
  lits.resize(n_vars+1);
  pass.resize(n_vars+1);
  gucl_stack.resize(n_vars);
  clauses.resize(n_header_clauses);
}

std::vector<Lit> current_working_clause;

bool read_a_clause_from_file(std::ifstream& f) {
  static std::vector<int> literal_table;
  current_working_clause.clear();
  literal_table.assign(n_vars+1,0);
  bool tautology = false;
  Lit x;
  f >> x;
  if (f.eof()) return false;
  while (true) {
    if (not f) {
      std::cerr << err << "Invalid literal-read.\n";
      std::exit(file_reading_error);
    }
    if (x == 0) break;
    const Var v = var(x);
    if (v > n_vars) {
      std::cerr << err << "Literal " << x << " contradicts n=" << n_vars << ".\n";
      std::exit(variable_value_error);
    }
    if (literal_table[v] == 0) {
      current_working_clause.push_back(x);
      literal_table[v] = x;
    }
    else if (literal_table[v] == -x) tautology = true;
    f >> x;
  }
  if (tautology) {
    current_working_clause.clear();
    return true;
  }
  if (current_working_clause.empty()) {
    std::cerr << err << "Found empty clause in input.\n";
    std::exit(empty_clause_error);
  }
  if (current_working_clause.size() == 1) {
    std::cerr << err << "Found unit-clause in input.\n";
    std::exit(unit_clause_error);
  }
  return true;
}

void add_a_clause_to_formula() {
  const auto n = current_working_clause.size();
  if (n == 0) return; // means tautology here
  if (n_clauses >= n_header_clauses) {
    std::cerr << err << "More than " << n_header_clauses << " clauses, contradicting cnf-header.\n";
    std::exit(number_clauses_error);
  }
  auto& C = clauses[n_clauses];
  C.length = n;
  C.status = true;
  C.literals = new Lit[n];
  C.end = C.literals + n;
  if (n>max_clause_length) max_clause_length = n;
  for (int i=0; i<(int)n; ++i) {
    const Lit x = current_working_clause[i];
    C.literals[i] = x;
    const Var v = var(x);
    const Polarity p = sign(x);
    auto& L = lits[v][p];
    L.occur = (ClauseP*) std::realloc(L.occur, (L.n_occur+1)*sizeof(ClauseP));
    L.occur[L.n_occur] = &(clauses[n_clauses]);
    ++L.n_occur;
  }
  ++n_clauses;
}

void read_formula(const std::string& filename) {
  std::ifstream f(filename);
  if (not f) {
    std::cerr << err << "Invalid file name.\n";
    std::exit(file_reading_error);
  }
  read_formula_header(f);
  n_clauses = 0;
  while (read_a_clause_from_file(f)) add_a_clause_to_formula();
  r_clauses = n_clauses;
  weights.resize(max_clause_length+1);
  initialise_weights();
}

// --- SAT solving ---

void assign(const Lit x) {
/* set x to true, enter found unit-literals onto the global stack, and create
   change information */
  assert(x);
  const Var v = var(x);
  assert(v <= n_vars);
  pass[v] = x;
  const Polarity p = sign(x);

  {const auto L = lits[v][p];
   const auto occur_true = L.n_occur;
   const auto max_size = changes_index + occur_true + 1;
   if (max_size >= changes.size()) changes.resize(max_size);
   changes[changes_index++] = nullptr;
   for (unsigned int i=0; i < occur_true; ++i) {
     const auto C = L.occur[i];
     if (not C->status) continue;
     assert(C->length >= 1);
     C->status = false;
     assert(r_clauses >= 1);
     --r_clauses;
     changes[changes_index++] = C;
   }
  }
  {const Polarity np = inv_polarity(p);
   const auto L = lits[v][np];
   const auto occur_false = L.n_occur;
   const auto max_size = changes_index + occur_false + 1;
   if (max_size > changes.size()) changes.resize(max_size);
   changes[changes_index++] = nullptr;
   for (unsigned int i=0; i < occur_false; ++i) {
     const auto C = L.occur[i];
     if (not C->status) continue;
     changes[changes_index++] = C;
     assert(C->length >= 2);
     --C->length;
     if (C->length == 1) {
       const Lit* cend = C->end;
       for (const Lit* lp = C->literals; lp != cend; ++lp) {
         const Lit ucl = *lp;
         const Var ucv = var(ucl);
         Lit& val = pass[ucv];
         if (val == 0) {
           gucl_stack[n_gucl++] = ucl;
           val = ucl;
           goto occ_loop;
         }
         else if (val == ucl) goto occ_loop;
       }
       contradictory_unit_clauses = true;
       goto end;
     }
   occ_loop:;}
  }
  end:;
}

void unassign(const Lit x) {
  assert(x);
  pass[var(x)] = 0;
  while (const ClauseP C = changes[--changes_index]) {
    assert(C->status);
    ++C->length;
  }
  while (const ClauseP C = changes[--changes_index]) {
    assert(not C->status);
    C->status = true;
    ++r_clauses;
  }
}

// performance-critical computation:
inline void accumulate(const bool stat, const unsigned length, double& sum) {
  sum += stat * weights[length];
}
inline Lit branching_literal() {
  Lit x = 0;
  double max = 0, max2 = 0;
  const auto nvar = n_vars;
  for (Var v = 1; v <= nvar; ++v) {
    if (pass[v] == 0) {
      double ps = 0;
      {const auto vpos = lits[v][pos]; const auto pos_occur = vpos.n_occur;
       for (unsigned int k=0; k < pos_occur; ++k) {
         const ClausePc C = vpos.occur[k];
         accumulate(C->status, C->length, ps);
       }}
      double ns = 0;
      {const auto vneg = lits[v][neg]; const auto neg_occur = vneg.n_occur;
       for (unsigned int k=0; k < neg_occur; ++k) {
         const ClausePc C = vneg.occur[k];
         accumulate(C->status, C->length, ns);
       }}
      const double prod = ps * ns, sum = ps + ns;
      if (prod > max) { max = prod; max2 = sum; x = (ps>=ns)?v:-Lit(v); }
      // handles also the case that only pure literals are left:
      else if (prod==max and sum>max2) { max2 = sum; x = (ps>=ns)?v:-Lit(v); }
    }
  }
  if (not x) {
    /* All remaining clauses have length >=1078, and thus the instance is
       satisfiable (since we can't have 2^1078 clauses); choose a literal
       occurring most often. */
    unsigned int max = 0;
    for (Var v = 1; v <= nvar; ++v)
      if (pass[v] == 0) {
        unsigned int count = 0;
        {const auto vpos = lits[v][pos]; const auto pos_o = vpos.n_occur;
         for (unsigned int k=0; k<pos_o; ++k) count += vpos.occur[k]->status;}
        if (count > max) {max = count; x = v;}
        count = 0;
        {const auto vneg = lits[v][neg]; const auto neg_o = vneg.n_occur;
         for (unsigned int k=0; k<neg_o; ++k) count += vneg.occur[k]->status;}
        if (count > max) {max = count; x = -Lit(v);}
      }
  }
  return x;
}

bool dpll() {
  ++n_nodes;
  std::stack<Lit> lucl_stack; // local unit-clause literals
  while (true) { // unit-clause propagation
    if (contradictory_unit_clauses) {
      while (not lucl_stack.empty()) {
        assert(pass[var(lucl_stack.top())]);
        unassign(lucl_stack.top());
        lucl_stack.pop();
      }
      contradictory_unit_clauses = false;
      while (n_gucl) pass[var(gucl_stack[--n_gucl])] = 0;
      return false;
    }
    else if (n_gucl) {
      const Lit implied_literal = gucl_stack[--n_gucl];
      lucl_stack.push(implied_literal);
      assign(implied_literal);
      ++n_units;
    }
    else break;
  }
  if (not r_clauses) return true;
  assert(n_gucl == 0);
  const Lit x = branching_literal();
  assert(x); assert(pass[var(x)] == 0);
  assign(x);
  if (dpll()) return true;
  assert(pass[var(x)] == x);
  unassign(x);
  ++n_backtracks;

  const Lit nx = -x;
  assert(pass[var(x)] == 0);
  assign(nx);
  if (dpll()) return true;
  assert(pass[var(x)] == nx);
  unassign(nx);

  while (not lucl_stack.empty()) {
    unassign(lucl_stack.top());
    lucl_stack.pop();
  }
  return false;
}

// --- Output ---

void output(const std::string& file, const Result_value result, const double elapsed) {
  std::cout << "s ";
  switch (result) {
    case unknown : std::cout << "UNKNOWN\n"; break;
    case unsat : std::cout << "UN";
    case sat : std::cout << "SATISFIABLE\n";
  }
  std::cout <<
         "c number_of_variables                   " << n_vars << "\n" <<
         "c number_of_clauses                     " << n_clauses << "\n" <<
         "c maximal_clause_length                 " << max_clause_length << "\n" <<
         "c running_time(sec)                     " << std::setprecision(2) << std::fixed << elapsed << "\n" <<
         "c number_of_nodes                       " << n_nodes << "\n" <<
         "c number_of_binary_nodes                " << n_backtracks << "\n" <<
         "c number_of_1-reductions                " << n_units << "\n" <<
         "c max_number_changes                    " << changes.size() << "\n" <<
         "c file_name                             " << file << std::endl;
  if (result == sat) {
    std::cout << "v ";
    for (Var i=1; i <= n_vars; ++i) if (pass[i]) std::cout << pass[i] << " ";
    std::cout << "0" << std::endl;
  }
}

#define S(x) #x
#define STR(x) S(x)
void version_information() {
  std::cout << program << ":\n"
   " author: Tanbir Ahmed\n"
   " url: http://sourceforge.net/projects/tawsolver/\n"
   " Changes by Oliver Kullmann\n"
   " Version: " << version << "\n"
   " Last change date: " << date << "\n"
   " Clause-weight parameters: " << basis_w_1 << ", " << basis_w_2 << " (resulting weights for clause-lengths 2,3,4:  " << weights[2] << ", " << weights[3] << ", " << weights[4] << ")\n" 
   " Macro settings:\n"
   "  LIT_TYPE = " STR(LIT_TYPE) " (with " << std::numeric_limits<Lit>::digits << " binary digits)\n"
#ifdef NDEBUG
   " Compiled with NDEBUG\n"
#else
   " Compiled without NDEBUG\n"
#endif
#ifdef __OPTIMIZE__
   " Compiled with optimisation options\n"
#else
   " Compiled without optimisation options\n"
#endif
   " Compilation date: " __DATE__ " " __TIME__ "\n"
#ifdef __GNUC__
   " Compiler: g++, version " __VERSION__ "\n"
#else
   " Compiler not gcc\n"
#endif
#ifdef OKLIB
   " Provided in the OKlibrary " "http://" STR(OKLIB) "\n"
#endif
#ifdef GIT_ID
   " Git ID = " STR(GIT_ID) "\n"
#endif
  ;
}

}

std::string filename;
#include <sys/resource.h>
rusage timing;
rusage* const ptiming = &timing;
double current_time() {
  getrusage(RUSAGE_SELF, ptiming);
  return timing.ru_utime.tv_sec + timing.ru_utime.tv_usec / 1000000.0;
}
double t1; // start of SAT solving
void abortion(const int sig) {
  std::signal(SIGINT, abortion);
  output(filename, unknown, current_time() - t1);
  std::exit(unknown);
}
void show_statistics(const int sig) {
  signal(SIGUSR1, show_statistics);
  output(filename, unknown, current_time() - t1);
}

int main(const int argc, const char* const argv[]) {
  if (argc == 1) {
    std::cout << "Usage:\n"
      "> " << program << " argument\n"
      " where argument is one of \"-v\", \"--version\", or a filename.\n";
    return 0;
  }
  filename = argv[1];
  if (filename == "-v" or filename == "--version") {
    version_information();
    return 0;
  }
  read_formula(filename);
  std::signal(SIGINT, abortion);
  std::signal(SIGUSR1, show_statistics);
  t1 = current_time();
  const auto result = dpll();
  const auto t2 = current_time();
  const auto ires = interprete_run(result);
  output(filename, ires, t2-t1);
  return ires;
}
