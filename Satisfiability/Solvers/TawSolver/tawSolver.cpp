/*********************************************************************
tawSolver -- A basic and efficient DLL SAT solver
Copyright (c) 2007-2013 Tanbir Ahmed http://users.encs.concordia.ca/~ta_ahmed/
Copyright 2013 Oliver Kullmann http://www.cs.swan.ac.uk/~csoliver/

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

  There are the following macros to control compilation:
   - LIT_TYPE (default int)
   - WEIGHT_2_CLAUSES, WEIGHT_4_CLAUSES, WEIGHT_5_CLAUSES,
     and WEIGHT_BASIS_OPEN:
     the weight for clause-length k=3 is standardised to 1, the weights for
     k = 2, 4, 5 are given by the first three macros, and for k without
     predetermined weight (in the initialisation of vector weights) the weight
     is decreased from the last set weight by the factor WEIGHT_BASIS_OPEN.

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
#include <exception>

#include <cmath>
#include <cstdint>
#include <cassert>
#include <csignal>

namespace {

const std::string version = "1.9.3";
const std::string date = "12.7.2013";

const std::string program = "tawSolver";
const std::string err = "ERROR[" + program + "]: ";

enum Error_codes {
  file_reading_error=1,
  file_pline_error=2,
  num_vars_error=3,
  allocation_error=4,
  literal_read_error=5,
  variable_value_error=6,
  number_clauses_error=7,
  empty_clause_error=8,
  unit_clause_error=9
};

enum Result_value { unsat=20, sat=10, unknown=0 };
Result_value interprete_run(const bool result) { return result ? sat : unsat; }

#ifndef LIT_TYPE
# define LIT_TYPE int32_t
#endif
typedef LIT_TYPE Lit;
static_assert(std::is_signed<Lit>::value, "Type \"Lit\" must be signed integral.");
static_assert(sizeof(Lit) != 1, "LIT_TYPE = char (or int8_t) doesn't work with reading (since not numbers are read, but characters).");

typedef std::make_unsigned<Lit>::type Var;

inline Var var(const Lit x) { return (x >= 0) ? x : -x; }
enum Polarity { pos=0, neg=1 };
inline Polarity sign(const Lit x) { return (x >= 0) ? pos : neg; }
inline Polarity inv_polarity(const Polarity p) { return (p == pos) ? neg:pos; }

typedef double Weight_t; // weights and their sums
typedef std::vector<Weight_t> Weight_vector;
typedef Var Clause_index;
static_assert(std::numeric_limits<Clause_index>::max() <= std::numeric_limits<Weight_vector::size_type>::max(), "Type Clause_index too large for weight vector.");

class Clause {
  const Lit* b; // the array of literals in the clause (as in the input)
  const Lit* e; // one past-the-end
  Clause_index length_; // the current length, or 0 iff clause is satisfied
  Clause_index old_length;
public :
  const Lit* begin() const { return b; }
  const Lit* end() const {return e; }
  Clause_index length() const { return length_; }
  operator bool() const { return length_; }
  void decrement() { assert(length_ >= 2); --length_; }
  void increment() { assert(length_ >= 1); ++length_; }
  void deactivate() {assert(length_ >= 1); old_length = length_; length_ = 0; }
  void activate() { assert(length_ == 0); length_ = old_length; }
  friend void add_a_clause_to_formula();
};
typedef Clause* ClauseP;

std::vector<Clause> clauses;

class Literal_occurrences {
  const ClauseP* b; // array with clause-pointers
  const ClauseP* e; // one past-the-end
public :
   const ClauseP* begin() const { return b; }
   const ClauseP* end() const {return e; }
   friend void set_literal_occurrences();
};
std::vector<std::array<Literal_occurrences,2>> lits;
// lits[v][pos/neg] for a variable v represents the list of occurrences.

// The stack of touched clauses:
typedef std::vector<ClauseP> Change_vec;
Change_vec changes(1); // acts as a global stack
Change_vec::size_type changes_index = 0; // Invariant: changes_index < changes.size().

std::vector<Lit> pass; /* the current assignment: pass[v] is 0 iff variable
 v is unassigned, otherwise it is v in case v->true and else -v. */

typedef uint_fast64_t Count_clauses;
Count_clauses n_header_clauses, n_clauses, r_clauses; // "r" = "remaining"
Count_clauses n_lit_occurrences = 0;
Var n_vars;

Clause_index max_clause_length = 0;

// The clause-weights:
#ifdef WEIGHT_2_CLAUSES
  constexpr Weight_t weight_2 = WEIGHT_2_CLAUSES;
#else
  constexpr Weight_t weight_2 = 7.0;
#endif
#ifdef WEIGHT_4_CLAUSES
  constexpr Weight_t weight_4 = WEIGHT_4_CLAUSES;
#else
  constexpr Weight_t weight_4 = 0.31;
#endif
#ifdef WEIGHT_5_CLAUSES
  constexpr Weight_t weight_5 = WEIGHT_5_CLAUSES;
#else
  constexpr Weight_t weight_5 = 0.19;
#endif
#ifdef WEIGHT_BASIS_OPEN
  constexpr Weight_t basis_open = WEIGHT_BASIS_OPEN;
#else
  constexpr Weight_t basis_open = 1.70;
#endif
// weights[k] is the weight for clause-length k >= 2:
Weight_vector weights {0,0, weight_2, 1, weight_4, weight_5};
// Remark: weights[1] is arbitrary (since not used).
constexpr Clause_index first_open_weight = 6;
/* If special weights for clause-lengths k = 4,5,... are to be used, then
   these weights are written into the initialisation of weights, and
   first_open_weight is to be adapted accordingly.

   The current parameter values
     weight_2=7.0, weight_4=0.31, weight_5=0.19, basis_open = 1.70
   have been obtained via optimisation on VanDerWaerden_2-3-12_135.cnf,
   yielding a local minimum for the node-count w.r.t. the indicated
   precision (e.g., for weight_4 the values 0.32, 0.30 yield worse node count).
*/
// the weights for clause of length >= first_open_weight:
Weight_t wopen(const Clause_index clause_length) {
  return weights[first_open_weight-1] *
    std::pow(basis_open,-double(clause_length)+first_open_weight-1);
}
void initialise_weights() {
  assert(weights.size() == unsigned(first_open_weight));
  try { weights.resize(max_clause_length+1); }
  catch (const std::bad_alloc&) {
    std::cerr << err << "Allocation error for double-vector of size " <<
       max_clause_length << "+1 (the maximal clause-length).\n";
    std::exit(allocation_error);
  }
  for (Clause_index i = first_open_weight; i <= max_clause_length; ++i)
    weights[i] = wopen(i);
}

typedef uint_fast64_t Count_statistics;
Count_statistics n_nodes = 0;
Count_statistics n_units = 0;
Count_statistics n_backtracks = 0;

// to handle the branching-assignment plus the derived assignments:
typedef std::vector<Lit> Global_assignment_stack;
Global_assignment_stack gucl_stack;
Global_assignment_stack::size_type n_gucl = 0; // the index of the next free element of the stack
typedef std::stack<Lit> Local_assignment_stack;
bool contradictory_unit_clauses = false;


// --- Input and initialisation ---

std::vector<std::array<Count_clauses,2>> lit_occur_count;

void read_formula_header(std::ifstream& f) {
  std::string line;
  while (true) {
    std::getline(f, line);
    if (not f) {
      std::cerr << err << "Reading error (possibly no line starting with \"p\").\n";
      std::exit(file_reading_error);
    }
    if (line[0] == 'p') break;
  }
  std::stringstream s(line);
  {std::string inp; s >> inp;
   if (inp != "p") {
     std::cerr << err << "Syntax error in parameter line (\"p\" not followed by space).\n";
     std::exit(file_pline_error);
   }
   s >> inp;
   if (inp != "cnf") {
     std::cerr << err << "Syntax error in parameter line (no \"cnf\").\n";
     std::exit(file_pline_error);
   }
  }
  s >> n_vars;
  if (not s) {
    std::cerr << err << "Reading error with parameter maximal-variable-index "
      "(too big or not-a-number).\n";
    std::exit(file_pline_error);
  }
  if (n_vars > Var(std::numeric_limits<Lit>::max())) {
    std::cerr << err << "Parameter maximal-variable-index n=" << n_vars <<
      " is too big for numeric_limits<Lit>::max=" <<
      std::numeric_limits<Lit>::max() << ".\n";
    std::exit(num_vars_error);
  }
  s >> n_header_clauses;
  if (not s) {
    std::cerr << err << "Reading error with parameter number-of-clauses "
      "(too big or not-a-number).\n";
    std::exit(file_pline_error);
  }
  try {
    lits.resize(n_vars+1);
    pass.resize(n_vars+1);
    lit_occur_count.resize(n_vars+1);
    gucl_stack.resize(n_vars);
  }
  catch (const std::bad_alloc&) {
    std::cerr << err << "Allocation error for vectors of size " << n_vars <<
      " (the maximal-variable-index).\n";
    std::exit(allocation_error);
  }
  try {
    clauses.resize(n_header_clauses);
  }
  catch (const std::bad_alloc&) {
    std::cerr << err << "Allocation error for vector of size " <<
      n_header_clauses << " (the number-of-clauses).\n";
    std::exit(allocation_error);
  }
}

std::vector<Lit> current_working_clause;

bool read_a_clause_from_file(std::ifstream& f) {
  static std::vector<Lit> literal_table;
  current_working_clause.clear();
  literal_table.assign(n_vars+1,0);
  bool tautology = false;
  Lit x;
  f >> x;
  if (f.eof()) return false;
  while (true) {
    if (not f) {
      std::cerr << err << "Invalid literal-read.\n";
      std::exit(literal_read_error);
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
  C.length_ = n;
  C.b = new Lit[n];
  C.e = C.b + n;
  if (n>max_clause_length) max_clause_length = n;
  for (Clause_index i = 0; i < n; ++i) {
    const Lit x = current_working_clause[i];
    const_cast<Lit*>(C.b)[i] = x;
    ++lit_occur_count[var(x)][sign(x)];
  }
  ++n_clauses;
  n_lit_occurrences += n;
}

std::vector<ClauseP> all_lit_occurrences;

void set_literal_occurrences() {
  if (all_lit_occurrences.empty()) return;
  const ClauseP* pointer = &all_lit_occurrences[0];
  for (Var v = 1; v <= n_vars; ++v)
    for (int p = 0; p <= 1; ++p) {
      lits[v][p].b = pointer;
      pointer += lit_occur_count[v][p];
      lits[v][p].e = pointer;
    }
  assert(pointer == &all_lit_occurrences[0] + n_lit_occurrences);
  {const auto clend = clauses.cend();
   for (auto i = clauses.cbegin(); i != clend; ++i) {
     for (const Lit x : *i) {
       const Var v = var(x); const Polarity p = sign(x);
       *const_cast<ClauseP*>(lits[v][p].end() - lit_occur_count[v][p]--) = const_cast<ClauseP>(&*i);
     }
   }
  }
  lit_occur_count.clear();
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
  initialise_weights();
  try { all_lit_occurrences.resize(n_lit_occurrences); }
  catch (const std::bad_alloc&) {
    std::cerr << err << "Allocation error for ClauseP-vector of size " <<
       n_lit_occurrences << " (the number of literal occurrences).\n";
    std::exit(allocation_error);
  }
  set_literal_occurrences();
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
   const auto obegin = L.begin();
   const auto oend = L.end();
   const auto max_size = changes_index + (oend - obegin) + 1;
   try { if (max_size >= changes.size()) changes.resize(max_size); }
   catch (const std::bad_alloc&) {
    std::cerr << err << "Allocation error when resizing \"changes\" to size "
      << max_size << " (positive occurrences).\n";
    std::exit(allocation_error);
   }
   changes[changes_index++] = nullptr;
   for (auto p = obegin; p != oend; ++p) {
     const auto C = *p;
     if (not *C) continue;
     C->deactivate();
     assert(r_clauses >= 1);
     --r_clauses;
     changes[changes_index++] = C;
   }
  }
  {const Polarity np = inv_polarity(p);
   const auto L = lits[v][np];
   const auto obegin = L.begin();
   const auto oend = L.end();
   const auto max_size = changes_index + (oend - obegin) + 1;
   try { if (max_size > changes.size()) changes.resize(max_size); }
   catch (const std::bad_alloc&) {
    std::cerr << err << "Allocation error when resizing \"changes\" to size "
      << max_size << " (negative occurrences).\n";
    std::exit(allocation_error);
   }
   changes[changes_index++] = nullptr;
   for (auto p = obegin; p != oend; ++p) {
     const auto C = *p;
     if (not *C) continue;
     changes[changes_index++] = C;
     C->decrement();
     if (C->length() == 1) {
       for (const Lit ucl : *C) {
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
  while (const ClauseP C = changes[--changes_index]) C->increment();
  while (const ClauseP C = changes[--changes_index]) {
    C->activate();
    ++r_clauses;
  }
}

inline Lit branching_literal() {
  Lit x = 0;
  Weight_t max = 0, max2 = 0;
  const auto nvar = n_vars;
  for (Var v = 1; v <= nvar; ++v) {
    if (pass[v] == 0) {
      const auto L = lits[v];
      Weight_t ps = 0;
      for (const auto C : L[pos]) ps += weights[C->length()];
      Weight_t ns = 0;
      for (const auto C : L[neg]) ns += weights[C->length()];
      const Weight_t prod = ps * ns, sum = ps + ns;
      if (prod > max) { max = prod; max2 = sum; x = (ps>=ns)?v:-Lit(v); }
      // handles also the case that only pure literals are left:
      else if (prod==max and sum>max2) { max2 = sum; x = (ps>=ns)?v:-Lit(v); }
    }
  }
  if (not x) {
    /* All remaining clauses have length at least 1000 (the first k with
    wopen(k) == 0), and thus the instance is satisfiable (since we can't have
    2^1000 clauses). Now just choosing a literal occurring most often. */
    Count_clauses max = 0;
    for (Var v = 1; v <= nvar; ++v)
      if (pass[v] == 0) {
        const auto L = lits[v];
        Count_clauses count = 0;
        for (const auto C : L[pos]) count += bool(*C);
        if (count > max) {max = count; x = v;}
        count = 0;
        for (const auto C : L[neg]) count += bool(*C);
        if (count > max) {max = count; x = -Lit(v);}
      }
  }
  return x;
}

bool dpll() {
  ++n_nodes;
  Local_assignment_stack lucl_stack; // local unit-clause literals
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

void output(const std::string& file, const Result_value result, const Weight_t elapsed) {
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
         "c number_of_literal_occurrences         " << n_lit_occurrences << "\n" <<
         "c running_time(sec)                     " << std::setprecision(2) << std::fixed << elapsed << "\n" <<
         "c number_of_nodes                       " << n_nodes << "\n" <<
         "c number_of_binary_nodes                " << n_backtracks << "\n" <<
         "c number_of_1-reductions                " << n_units << "\n" <<
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
   " Clause-weight parameters: " << weight_2 << ", " << basis_open << "\n"
   "  Mapping k -> weight for weights specified at compile-time:\n ";
   for (Clause_index k = 2; k < first_open_weight; ++k)
     std::cout << "  " << k << "->" << weights[k];
   std::cout << "\n"
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
