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

> g++ --std=c++11 -Wall -Ofast -funroll-loops -funsafe-loop-optimizations -fno-math-errno -funsafe-math-optimizations -fwhole-program -DNDEBUG -o tawSolver tawSolver.cpp

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
   - LIT_TYPE (default std::int32_t)
   - WEIGHT_2, WEIGHT_4, WEIGHT_5, WEIGHT_6 and WEIGHT_BASIS_OPEN:
     the weight for clause-length k=3 is standardised to 1, the weights for
     k = 2, 4, 5, 6 are given by the first four macros, and the
     non-predetermined weights (w.r.t. the initialisation of vector "weights")
     are obtained by repeatedly dividing the last predetermined weight by
     WEIGHT_BASIS_OPEN.
   - UCP_STRATEGY (default 1): 0 means BFS-processing, 1 means DFS-processing
     of unit-clauses.
   - PURE_LITERALS: if defined (default is undefined), then pure literals are
     eliminated (as they are encountered, not iteratedly).
   - TAU_ITERATION: if defined (default is undefined), then this natural number
     is the number of iterations for computing the in principle more accurate,
     but also more costly tau-function as projection (instead of the product);
     if TAU_ITERATION, then a reasonable default seems the value 5.
     TAU_ITERATION implies PURE_LITERALS.

  To provide further versioning-information, there are two macros, which are
  only relevant if they are defined:
   - GIT_ID (for the identity hash-value of the Git repository)
   - OKLIB (with the url (without prefix "http://") for the OKlibrary.

  Remarks on the statistics output:
   - "running_time" is only solver-time
   - "running_time + reading-and-set-up_time is total time.

*/

#include <limits>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <type_traits>
#include <iomanip>
#include <exception>

#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <cassert>
#include <csignal>

namespace {

const std::string version = "2.4.0";
const std::string date = "2.8.2013";

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


// --- Data structures for literals and clauses ---

enum Result_value { unsat=20, sat=10, unknown=0 };
Result_value interprete_run(const bool result) { return result ? sat : unsat; }

#ifndef LIT_TYPE
# define LIT_TYPE std::int32_t
#endif
typedef LIT_TYPE Lit_int;
static_assert(std::is_signed<Lit_int>::value, "Type \"Lit_int\" must be signed integral.");
static_assert(sizeof(Lit_int) != 1, "Lit_int = char (or int8_t) doesn't work with reading (since not numbers are read, but characters).");

typedef std::make_unsigned<Lit_int>::type Var;
enum Polarity { pos=0, neg=1 };
inline Polarity operator -(const Polarity p) { return (p == pos) ? neg:pos; }

class Lit {
  Lit_int x;
public :
  Lit() = default;
  constexpr explicit Lit(const Lit_int x) : x(x) {}
  constexpr explicit operator bool() const { return x; }
  constexpr Lit operator -() const { return Lit(-x); }
  constexpr bool operator ==(const Lit y) const { return x == y.x; }
  friend constexpr Var var(const Lit x) { return std::abs(x.x); }
  friend constexpr Polarity sign(const Lit x) {return (x.x >= 0) ? pos : neg;}
  friend std::ostream& operator <<(std::ostream& out, const Lit x) {
    return out << x.x;
  }
  friend std::istream& operator >>(std::istream& in, Lit& x) {
    return in >> x.x;
  }
};
static_assert(std::is_pod<Lit>::value, "Lit is not POD.");

typedef std::vector<Lit> Lit_vec;

typedef double Weight_t; // weights and their sums
static_assert(std::is_pod<Weight_t>::value, "Weight_t is not POD.");
typedef std::vector<Weight_t> Weight_vector;
typedef Var Clause_index;
static_assert(std::numeric_limits<Clause_index>::max() <= std::numeric_limits<Weight_vector::size_type>::max(), "Type Clause_index too large for weight vector (conversions cost too much time here).");

typedef std::uint_fast64_t Count_clauses;
typedef std::vector<std::array<Count_clauses,2>> Count_vec;

Count_clauses r_clauses; // "r" = "remaining"

class Clause {
  const Lit* b; // the array of literals in the clause (as in the input)
  const Lit* e; // one past-the-end
  Clause_index length_; // the current length, or 0 iff clause is satisfied
  Clause_index old_length;
  void increment() { assert(length_ >= 1); ++length_; }
  void decrement() { assert(length_ >= 2); --length_; }
  void deactivate() {
    assert(length_ >= 1);
    old_length = length_;
    length_ = 0;
    assert(r_clauses >= 1);
    --r_clauses;
  }
  void activate() {
    assert(length_ == 0);
    length_ = old_length;
    ++r_clauses;
  }
  friend void add_a_clause_to_formula(const Lit_vec&, Count_vec&);
  friend class ChangeManagement;
  friend void assign_0(Lit);
  friend void assign_1(Lit);
public :
  const Lit* begin() const { return b; }
  const Lit* end() const { return e; }
  Clause_index length() const { return length_; }
  explicit operator bool() const { return length_; }
};
static_assert(std::is_pod<Clause>::value, "Clause is not POD.");

typedef Clause* ClauseP;

typedef std::vector<Clause> Clause_vec;
typedef std::vector<ClauseP> ClauseP_vec;

class Literal_occurrences {
  const ClauseP* b; // array with clause-pointers
  const ClauseP* e; // one past-the-end
public :
   const ClauseP* begin() const { return b; }
   const ClauseP* end() const { return e; }
   Count_clauses size() const { return e-b; }
   friend void set_literal_occurrences(Count_vec&);
};
static_assert(std::is_pod<Literal_occurrences>::value, "Literal_occurrences is not POD.");

typedef std::uint_fast64_t Count_statistics;


// --- Basic global variables ---

Clause_vec clauses;

std::vector<std::array<Literal_occurrences,2>> lits;
// lits[v][pos/neg] for a variable v represents the list of occurrences.
ClauseP_vec all_lit_occurrences; // the underlying raw storage

Count_clauses n_header_clauses, n_clauses; // r_clauses see above
Count_clauses n_lit_occurrences;
Var n_vars;
Clause_index max_clause_length;

Count_statistics n_nodes;
Count_statistics n_backtracks;
Count_statistics n_units;
#ifdef PURE_LITERALS
Count_statistics n_pure_literals;
#endif


// --- Input and initialisation ---

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
  if (n_vars > Var(std::numeric_limits<Lit_int>::max())) {
    std::cerr << err << "Parameter maximal-variable-index n=" << n_vars <<
      " is too big for numeric_limits<Lit_int>::max=" <<
      std::numeric_limits<Lit_int>::max() << ".\n";
    std::exit(num_vars_error);
  }
  s >> n_header_clauses;
  if (not s) {
    std::cerr << err << "Reading error with parameter number-of-clauses "
      "(too big or not-a-number).\n";
    std::exit(file_pline_error);
  }
  try { lits.resize(n_vars+1); }
  catch (const std::bad_alloc&) {
    std::cerr << err << "Allocation error for lits-vector of size " <<
      n_vars << " (the maximal-variable-index).\n";
    std::exit(allocation_error);
  }
  try { clauses.resize(n_header_clauses); }
  catch (const std::bad_alloc&) {
    std::cerr << err << "Allocation error for clauses-vector of size " <<
      n_header_clauses << " (the number-of-clauses).\n";
    std::exit(allocation_error);
  }
}

bool read_a_clause_from_file(std::ifstream& f, Lit_vec& C) {
  static Lit_vec literal_table;
  C.clear();
  literal_table.assign(n_vars+1,Lit());
  bool tautology = false;
  Lit x;
  f >> x;
  if (f.eof()) return false;
  while (true) {
    if (not f) {
      std::cerr << err << "Invalid literal-read.\n";
      std::exit(literal_read_error);
    }
    if (not x) break;
    const Var v = var(x);
    if (v > n_vars) {
      std::cerr << err << "Literal " << x << " contradicts n=" << n_vars << ".\n";
      std::exit(variable_value_error);
    }
    if (not literal_table[v]) {
      C.push_back(x);
      literal_table[v] = x;
    }
    else if (literal_table[v] == -x) tautology = true;
    f >> x;
  }
  if (tautology) {
    C.clear();
    return true;
  }
  if (C.empty()) {
    std::cerr << err << "Found empty clause in input.\n";
    std::exit(empty_clause_error);
  }
  if (C.size() == 1) {
    std::cerr << err << "Found unit-clause in input.\n";
    std::exit(unit_clause_error);
  }
  return true;
}

void add_a_clause_to_formula(const Lit_vec& D, Count_vec& count) {
  const auto n = D.size();
  if (n == 0) return; // means tautology here
  if (n_clauses >= n_header_clauses) {
    std::cerr << err << "More than " << n_header_clauses << " clauses, contradicting cnf-header.\n";
    std::exit(number_clauses_error);
  }
  auto& C = clauses[n_clauses];
  C.length_ = n;
  C.b = new Lit[n];
  C.e = C.b + n;
  if (n > max_clause_length) max_clause_length = n;
  for (Clause_index i = 0; i < n; ++i) {
    const Lit x = D[i];
    const_cast<Lit*>(C.b)[i] = x;
    ++count[var(x)][sign(x)];
  }
  ++n_clauses;
  n_lit_occurrences += n;
}

void set_literal_occurrences(Count_vec& count) {
  if (all_lit_occurrences.empty()) return;
  const ClauseP* pointer = &all_lit_occurrences[0];
  for (Var v = 1; v <= n_vars; ++v)
    for (int p = 0; p <= 1; ++p) {
      auto& L = lits[v][p];
      L.b = pointer;
      pointer += count[v][p];
      L.e = pointer;
    }
  assert(pointer == &all_lit_occurrences[0] + n_lit_occurrences);
  {const auto clend = clauses.cend();
   for (auto i = clauses.cbegin(); i != clend; ++i) {
     for (const Lit x : *i) {
       const Var v = var(x); const Polarity p = sign(x);
       *const_cast<ClauseP*>(lits[v][p].end() - count[v][p]--) =
         const_cast<ClauseP>(&*i);
     }
   }
  }
}

void read_formula(const std::string& filename) {
  std::ios_base::sync_with_stdio(false);
  std::ifstream f(filename);
  if (not f) {
    std::cerr << err << "Invalid file name.\n";
    std::exit(file_reading_error);
  }
  read_formula_header(f);
  Count_vec count;
  try { count.resize(n_vars+1); }
  catch (const std::bad_alloc&) {
    std::cerr << err << "Allocation error for counting-vector of size " <<
      n_vars << " (the maximal-variable-index).\n";
    std::exit(allocation_error);
  }
  n_clauses = 0;
  {Lit_vec C;
   while (read_a_clause_from_file(f,C)) add_a_clause_to_formula(C,count);
  }
  if (not (r_clauses = n_clauses)) return;
  try { all_lit_occurrences.resize(n_lit_occurrences); }
  catch (const std::bad_alloc&) {
    std::cerr << err << "Allocation error for ClauseP-vector of size " <<
       n_lit_occurrences << " (the number of literal occurrences).\n";
    std::exit(allocation_error);
  }
  set_literal_occurrences(count);
}


// --- SAT solving data structures ---

Lit_vec pass; /* the current assignment: pass[v] is 0 iff variable
 v is unassigned, otherwise it is v in case v->true and else -v. */

class ChangeManagement {
  ClauseP_vec changes;
  const ClauseP* begin;
  ClauseP* next;
  void init() {
    const auto s = n_lit_occurrences + 3 * n_vars;
    assert(s >= 1);
    changes.resize(s);
    begin = next = &*changes.begin();
  }
  friend void initialisation();
public :
  void start_new() { *(next++) = nullptr; }
  void push(const ClauseP C) { *(next++) = C; }
  void reactivate_0() { while (const ClauseP C = *(--next)) C->increment(); }
  void reactivate_1() { while (const ClauseP C = *(--next)) C->activate(); }
};
ChangeManagement changes;

bool delete_assignments = true;

#ifndef UCP_STRATEGY
# define UCP_STRATEGY 1
#endif
#if UCP_STRATEGY == 0 // BFS
class Unit_stack {
  typedef Lit_vec stack_t;
  static stack_t stack;
  static Lit* end_;
  static Lit* open_;
  const Lit* const begin_;
  static void init() {
    stack.resize(n_vars);
    assert(n_vars);
    end_ = &stack[0];
  }
  friend void initialisation();
public :
  static void push(const Lit x) {
    assert(end_ - &stack[0] < n_vars);
    *(end_++) = x;
  }
  static Lit pop() {
    assert(open_ != end_);
    return *(open_++);
  }
  Unit_stack() : begin_(end_) { open_ = end_; }
  ~Unit_stack() {
    if (delete_assignments) {
      const auto end = end_;
      for (auto p = begin_; p != end; ++p) pass[var(*p)] = Lit();
    }
    end_ = const_cast<Lit*>(begin_);
  }
  explicit operator bool() const { return end_ != open_; }
  const Lit* begin() const { return begin_; }
  static const Lit* end() { return end_; }
};
Unit_stack::stack_t Unit_stack::stack;
Lit* Unit_stack::end_;
Lit* Unit_stack::open_;

#else // DFS

class Unit_stack {
  typedef Lit_vec stack_t;
  static stack_t main_stack;
  static stack_t input_stack;
  static Lit* begin_input;
  static Lit* end_input;
  static Lit* end_main;
  const Lit* const begin_main;
  static Lit push_main(const Lit x) {
    assert(end_main - &main_stack[0] < n_vars);
    return *(end_main++) = x;
  }
  static Lit pop_input() {
    assert(end_input != begin_input);
    return *(--end_input);
  }
  static void init() {
    main_stack.resize(n_vars);
    input_stack.resize(n_vars);
    assert(n_vars);
    end_main = &main_stack[0];
    begin_input = end_input = &input_stack[0];
  }
  friend void initialisation();
public :
  static void push(const Lit x) {
    assert(end_input - begin_input < n_vars);
    *(end_input++) = x;
  }
  static Lit pop() { return push_main(pop_input()); }
  Unit_stack() : begin_main(end_main) { end_input = begin_input; }
  ~Unit_stack() {
    if (delete_assignments) {
      const auto mend = end_main;
      for (auto p = begin_main; p != mend; ++p) pass[var(*p)] = Lit();
      const auto iend = end_input;
      for (auto p = begin_input; p != iend; ++p) pass[var(*p)] = Lit();
    }
    end_main = const_cast<Lit*>(begin_main);
  }
  explicit operator bool() const { return end_input != begin_input; }
  const Lit* begin() const { return begin_main; }
  static const Lit* end() { return end_main; }
};
Unit_stack::stack_t Unit_stack::main_stack;
Unit_stack::stack_t Unit_stack::input_stack;
Lit* Unit_stack::begin_input;
Lit* Unit_stack::end_input;
Lit* Unit_stack::end_main;
#endif

class Push_unit_clause {
  bool contradiction_ = false;
public :
  // returns false iff contradiction found:
  bool operator() (const Clause& C) {
    assert(C.length() == 1);
    assert(not contradiction_);
    for (const Lit y : C) {
      Lit& val = pass[var(y)];
      if (not val) {
        Unit_stack::push(y);
        val = y;
        return true;
      }
      else if (val == y) return true;
    }
    contradiction_ = true;
    return false;
  }
  bool contradiction() {
    if (contradiction_) {contradiction_ = false; return true;}
    else return false;
  }
};
Push_unit_clause push_unit_clause;

#ifdef PURE_LITERALS
class Pure_stack {
  typedef Lit_vec stack_t;
  static stack_t stack;
  static const Lit* new_begin;
  static Lit* end_;
  const Lit* const begin_;
  const Lit* begin() const { return begin_; }
  static const Lit* end() { return end_; }
  static void init() {
    stack.resize(n_vars);
    assert(n_vars);
    end_ = &stack[0];
  }
  friend void initialisation();
public :
  static void clear() { new_begin = end_; }
  static void push(const Lit x) {
    assert(end_ - &stack[0] < n_vars);
    *(end_++) = x;
  }
  Pure_stack() : begin_(new_begin) {}
  ~Pure_stack() {
    if (delete_assignments) for (const Lit x : *this) pass[var(x)] = Lit();
    end_ = const_cast<Lit*>(begin_);
  }
};
Pure_stack::stack_t Pure_stack::stack;
const Lit* Pure_stack::new_begin;
Lit* Pure_stack::end_;
#endif


// --- Weight handling ---

#ifdef WEIGHT_2
  constexpr Weight_t weight_2 = WEIGHT_2;
#else
# ifdef TAU_ITERATION
  constexpr Weight_t weight_2 = 5.0;
# else
  constexpr Weight_t weight_2 = 4.85;
# endif
#endif
#ifdef WEIGHT_4
  constexpr Weight_t weight_4 = WEIGHT_4;
#else
# ifdef TAU_ITERATION
  constexpr Weight_t weight_4 = 0.295;
# else
  constexpr Weight_t weight_4 = 0.354;
# endif
#endif
#ifdef WEIGHT_5
  constexpr Weight_t weight_5 = WEIGHT_5;
#else
# ifdef TAU_ITERATION
  constexpr Weight_t weight_5 = 0.122;
# else
  constexpr Weight_t weight_5 = 0.11;
# endif
#endif
#ifdef WEIGHT_6
  constexpr Weight_t weight_6 = WEIGHT_6;
#else
# ifdef TAU_ITERATION
  constexpr Weight_t weight_6 = 0.0756;
# else
  constexpr Weight_t weight_6 = 0.0694;
# endif
#endif

#ifdef WEIGHT_BASIS_OPEN
  constexpr Weight_t basis_open = WEIGHT_BASIS_OPEN;
#else
# ifdef TAU_ITERATION
  constexpr Weight_t basis_open = 1.60;
# else
  constexpr Weight_t basis_open = 1.46;
# endif
#endif

/* The current parameter values have been obtained mainly via optimisation on
   VanDerWaerden_2-3-12_135.cnf (van der Waerden problem with two colours,
   first colour arithmetic progressions of size 3, second colour
   arithmetic progressions of size 12, with n = 135, the first n such that
   every partition of {1,...,n} must contain an arithmetic progression of
   size 3 in the first part or of size 12 in the second part).
*/

constexpr Clause_index first_open_weight = 7;
static_assert(first_open_weight >= 4, "Wrong value of first_open_weight.");
constexpr std::array<Weight_t, first_open_weight> predetermined_weights {{0,0, weight_2, 1, weight_4, weight_5, weight_6}};
static_assert(predetermined_weights[0] == 0, "weights[0] != 0.");
static_assert(predetermined_weights[2] != 0, "Zero weight.");
static_assert(predetermined_weights[3] == 1, "weights[3] != 1.");
static_assert(predetermined_weights[first_open_weight-1] != 0, "Zero weight.");
/* Remarks:
    - predetermined_weights[1] is arbitrary (since not used).
    - If special weights for clause-lengths k = 4,5,... are to be used, then
      these weights are written into predetermined_weights, and
      first_open_weight is to be adapted accordingly.
*/

#ifdef TAU_ITERATION
static_assert(std::numeric_limits<Weight_t>::has_infinity, "Tau-computation needs +inf.");
constexpr Weight_t inf_weight = std::numeric_limits<Weight_t>::infinity();
#endif

// weight[k] is the weight for clause-length k >= 2:
class Weights {
  constexpr static Weight_t min_weight = std::numeric_limits<Weight_t>::min();
  static_assert(min_weight != 0, "Error with min_weight.");
  constexpr static Weight_t set_min(const Weight_t w) {
    return (w == 0)? min_weight : w;
  }
  // the weights for clause of length >= first_open_weight:
  constexpr static Weight_t wopen(const Clause_index clause_length) {
    return set_min(predetermined_weights[first_open_weight-1] *
      std::pow(basis_open,-Weight_t(clause_length)+first_open_weight-1));
  }

  Weight_vector weights;
  void init() {
    assert(weights.size() == first_open_weight);
    try { weights.resize(max_clause_length+1); }
    catch (const std::bad_alloc&) {
      std::cerr << err << "Allocation error for double-vector of size " <<
         max_clause_length << "+1 (the maximal clause-length).\n";
      std::exit(allocation_error);
    }
    for (Clause_index i = first_open_weight; i <= max_clause_length; ++i)
      weights[i] = wopen(i);
  }
  friend void initialisation();
public :
  Weights() : weights(predetermined_weights.begin(), predetermined_weights.end()) {}
  Weight_t operator[] (const Clause_index i) const { return weights[i]; }
};
constexpr Weight_t Weights::min_weight;
Weights weight;


// --- SAT solving algorithms ---

void initialisation() {
  pass.resize(n_vars+1);
  changes.init();
  Unit_stack::init();
  weight.init();
#ifdef PURE_LITERALS
  Pure_stack::init();
#endif
}

inline void assign_0(const Lit x) {
  assert(x);
  const Var v = var(x);
  assert(v <= n_vars);
  assert(not(pass[v] == -x));
  pass[v] = x;
  for (auto C : lits[v][-sign(x)]) {
    if (not *C) continue;
    changes.push(C);
    C->decrement();
    if (C->length() == 1 and not push_unit_clause(*C)) return;
  }
}
inline void assign_1(const Lit x) {
  assert(x);
  const Var v = var(x);
  assert(v <= n_vars);
  assert(pass[v] == x);
  for (auto C : lits[v][sign(x)]) {
    if (not *C) continue;
    C->deactivate();
    changes.push(C);
  }
}

inline Lit first_branch(const Weight_t pd, const Weight_t nd, const Var v) {
  return (pd>=nd) ? Lit(v) : -Lit(v);
}
#ifdef TAU_ITERATION
# ifndef PURE_LITERALS
#  error "TAU_ITERATION requires PURE_LITERALS"
# endif
class Branching_tau {
  Lit x;
  Weight_t min1, max2;
  static Weight_t tau(const Weight_t a, const Weight_t b) {
    constexpr int iterations = TAU_ITERATION;
    Weight_t x = std::pow(4,1/(a+b));
    for (int i = 0; i < iterations; ++i) {
      const Weight_t pa = std::pow(x,-a), pb = std::pow(x,-b);
      x *= 1 + (pa + pb - 1) / (a*pa + b*pb);
    }
    return x;
  }
public :
  Branching_tau() : x{}, min1(inf_weight), max2(0) {}
  operator Lit() const { return x; }
  void operator()(const Weight_t pd, const Weight_t nd, const Var v) {
    assert(pd > 0); assert(nd > 0);
    const Weight_t chi = std::pow(min1,-pd) + std::pow(min1,-nd);
    if (chi>1) return;
    const Weight_t sum = pd + nd;
    if (chi==0) { if ((min1=tau(pd,nd))==inf_weight and sum<=max2) return; }
    else if (chi==1) { if (sum<=max2) return; }
    else if (chi<1) min1=tau(pd,nd);
    max2=sum;
    x = first_branch(pd,nd,v);
  }
};
typedef Branching_tau Best_branching;
#else
class Branching_product {
  Lit x;
  Weight_t max1, max2;
public :
  Branching_product() : x{}, max1(0), max2(0) {}
  operator Lit() const { return x; }
  void operator()(const Weight_t pd, const Weight_t nd, const Var v) {
    const Weight_t prod = pd * nd;
    if (prod < max1) return;
    const Weight_t sum = pd + nd;
    if (prod > max1) max1=prod;
    else if (sum <= max2) return;
    max2 = sum;
    x = first_branch(pd,nd,v);
  }
};
typedef Branching_product Best_branching;
#endif

inline Lit branching_literal() {
  Best_branching br;
#ifdef PURE_LITERALS
  Pure_stack::clear(); changes.start_new();
#endif
  const auto nvar = n_vars;
  for (Var v = 1; v <= nvar; ++v) {
    if (not pass[v]) {
      const auto Occ = lits[v];
      Weight_t ps = 0;
      for (const auto C : Occ[pos]) ps += weight[C->length()];
#ifdef PURE_LITERALS
      if (ps == 0) {
        const Lit pl = -Lit(v);
        pass[v] = pl;
        Pure_stack::push(pl);
        assign_1(pl);
        ++n_pure_literals;
        if (not r_clauses) return Lit(); else continue;
      }
#endif
      Weight_t ns = 0;
      for (const auto C : Occ[neg]) ns += weight[C->length()];
#ifdef PURE_LITERALS
      if (ns == 0) {
        const Lit pl = Lit(v);
        pass[v] = pl;
        Pure_stack::push(pl);
        assign_1(pl);
        ++n_pure_literals;
       if (not r_clauses) return Lit(); else continue;
      }
#endif
      br(ps, ns, v);
    }
  }
  return br;
}

bool dll(const Lit x) {
  ++n_nodes;
  assert(x);
  changes.start_new();
  const Unit_stack unit_stack;
  Unit_stack::push(x);
  assign_0(Unit_stack::pop());
  while (unit_stack) { // unit-clause propagation
    ++n_units;
    assign_0(Unit_stack::pop());
    if (push_unit_clause.contradiction()) goto only_units;
  }

  changes.start_new();
  for (const Lit y : unit_stack) assign_1(y);
  if (not r_clauses) {delete_assignments = false; return true;}

  {const Lit y = branching_literal();
#ifdef PURE_LITERALS
   const Pure_stack pure_stack;
   if (not r_clauses) {delete_assignments = false; return true;}
#endif
   if (dll(y)) return true;
   ++n_backtracks;
   if (dll(-y)) return true;
#ifdef PURE_LITERALS
   changes.reactivate_1();
#endif
  }

  changes.reactivate_1();
only_units :
  changes.reactivate_0();
  return false;
}

bool dll0() { // without unit-clauses
  ++n_nodes;
  if (not n_clauses) return true;
  const Lit x = branching_literal();
#ifdef PURE_LITERALS
  if (not r_clauses) {delete_assignments = false; return true;}
#endif
  return dll(x) or (++n_backtracks, dll(-x));
#ifdef PURE_LITERALS
  // pure_stack and changes.reactivate_1() superfluous here, since no backtrack
#endif
}


// --- Output ---

typedef double Time_point;

#include <sys/resource.h>
class UserTime {
  rusage timing;
  rusage* const ptiming;
public :
  UserTime() : ptiming(&timing) {}
  Time_point operator()() {
    getrusage(RUSAGE_SELF, ptiming);
    return timing.ru_utime.tv_sec + timing.ru_utime.tv_usec / 1000000.0;
  }
};
UserTime timing;

Time_point t0; // start of computation
Time_point t1; // start of SAT solving

void output(const std::string& file, const Result_value result) {
  const Time_point elapsed = timing() - t1;
  std::cout << "s ";
  switch (result) {
    case unknown : std::cout << "UNKNOWN\n"; break;
    case unsat : std::cout << "UN";
    case sat : std::cout << "SATISFIABLE\n";
  }
  std::cout <<
         "c number_of_variables                   " << n_vars << "\n"
         "c number_of_clauses                     " << n_clauses << "\n"
         "c maximal_clause_length                 " << max_clause_length << "\n"
         "c number_of_literal_occurrences         " << n_lit_occurrences << "\n"
         "c running_time(sec)                     " << std::setprecision(2) << std::fixed << elapsed << "\n"
         "c number_of_nodes                       " << n_nodes << "\n"
         "c number_of_binary_nodes                " << n_backtracks << "\n"
         "c number_of_1-reductions                " << n_units << "\n"
#ifdef PURE_LITERALS
         "c number_of_pure_literals               " << n_pure_literals << "\n"
#endif
         "c reading-and-set-up_time(sec)          " << std::setprecision(3) << t1 - t0 << "\n"
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
   " authors: Tanbir Ahmed and Oliver Kullmann\n"
   " url's:\n"
   "  http://sourceforge.net/projects/tawsolver/\n"
   "  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/TawSolver/tawSolver.cpp\n"
   " Version: " << version << "\n"
   " Last change date: " << date << "\n"
   " Mapping k -> weight, for clause-lengths k specified at compile-time:\n ";
   std::cout.precision(4);
   for (Clause_index k = 2; k < first_open_weight; ++k)
     std::cout << "  " << k << "->" << weight[k];
   std::cout << "\n"
   " Divisor for open weights: " << basis_open << "\n"
   " Macro settings:\n"
   "  LIT_TYPE = " STR(LIT_TYPE) " (with " << std::numeric_limits<Lit_int>::digits << " binary digits)\n"
   "  UCP_STRATEGY = " << UCP_STRATEGY << "\n"
#ifdef TAU_ITERATION
   "  TAU_ITERATION = " << TAU_ITERATION << "\n"
#else
   " Compiled without TAU_ITERATION\n"
#endif
#ifdef PURE_LITERALS
   " Compiled with PURE_LITERALS\n"
#else
   " Compiled without PURE_LITERALS\n"
#endif
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

std::string filename;

void abortion(const int sig) {
  std::signal(SIGINT, abortion);
  output(filename, unknown);
  std::exit(unknown);
}
void show_statistics(const int sig) {
  signal(SIGUSR1, show_statistics);
  output(filename, unknown);
}

} // anonymous namespace

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
  t0 = timing();
  read_formula(filename);
  if (n_clauses) {
    try { initialisation(); }
    catch (const std::bad_alloc&) {
      std::cerr << err << "Allocation error with initialisation of algorithmic"
       " data structures.\n";
      std::exit(allocation_error);
    }
  }
  std::signal(SIGINT, abortion);
  std::signal(SIGUSR1, show_statistics);
  t1 = timing();
  const auto result = dll0();
  const auto ires = interprete_run(result);
  output(filename, ires);
  return ires;
}
