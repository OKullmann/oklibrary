/**************************************************************************************************
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
**************************************************************************************************/

/*
 Compile with
   g++ --std=c++11 -Wall -Ofast -DNDEBUG -o tawSolver tawSolver.cpp
 There are two macros to control compilation:
   - MAX_CLAUSE_LENGTH (default 32)
   - LIT_TYPE (default int).
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

#include <cstdint>
#include <cstdlib>
#include <cassert>

#include <sys/resource.h>

#ifndef MAX_CLAUSE_LENGTH
# define MAX_CLAUSE_LENGTH 32
#endif
constexpr int max_clause_length = MAX_CLAUSE_LENGTH;
static_assert(max_clause_length==8 or max_clause_length==16 or max_clause_length==32 or max_clause_length==64,"Currently only MAX_CLAUSE_LENGTH=8,16,32,64 is possible.");
#if MAX_CLAUSE_LENGTH == 8
 typedef uint8_t Clause_content;
#elif MAX_CLAUSE_LENGTH == 16
 typedef uint16_t Clause_content;
#elif MAX_CLAUSE_LENGTH == 32
 typedef uint32_t Clause_content;
#else
 typedef uint64_t Clause_content;
#endif
static_assert(std::numeric_limits<Clause_content>::digits==max_clause_length,"Error with choice of type \"Clause_content\".");

enum Error_codes {
  missing_file_error=1,
  file_reading_error=2,
  num_vars_error=3,
  clause_length_error=4,
  variable_value_error=5,
  number_clauses_error=6,
  empty_clause_error=7
};

enum Exit_codes { sat=10, unsat=20 };

enum Polarity { neg=0, pos=1 };

#ifndef LIT_TYPE
# define LIT_TYPE int
#endif
typedef LIT_TYPE Lit;
static_assert(std::is_signed<Lit>::value, "Type \"Lit\" must be signed integral.");
static_assert(sizeof(Lit) != 1, "LIT_TYPE = char (or int8_t) doesn't work with reading (since not numbers are read, but characters).");

typedef std::make_unsigned<Lit>::type Var;

struct clause_info {
  Lit* literals;
  Clause_content value;
  int number;
  int length;
  Lit c_ucl;
  bool status;
};

std::vector<clause_info> clauses;

struct lit_info {
  unsigned int* clause_occ; // array with clause-numbers
  unsigned int* clause_index; // array with array-indices (for the clauses)
  unsigned int n_occur;
  bool status;
};
typedef std::array<lit_info,2> lit_info_pair;
std::vector<lit_info_pair> lits;

struct change_info {
  unsigned int clause_number;
  unsigned int literal_index;
};

typedef std::vector<change_info> Change_v;
typedef Change_v::size_type change_index_t;
Change_v changes(1);
change_index_t changes_index = 0; // Invariant: changes_index < changes.size().

typedef std::array<int,2> int_pair;
std::vector<int_pair> n_changes;

unsigned int n_clauses, n_header_clauses, r_clauses;
Var n_vars;
unsigned int depth = 0;
unsigned int act_max_clause_length = 0;

std::vector<Lit> gucl_stack;
int n_gucl = 0;
bool contradictory_unit_clauses = false;
std::vector<Lit> checker;

std::vector<Lit> out;

unsigned long long int n_branches = 0;
unsigned long long int n_units = 0;
unsigned long long int n_backtracks = 0;


void read_formula_header(std::ifstream& f) {
  std::string line;
  while (true) {
    std::getline(f, line);
    if (not f) {
      std::cerr << "Reading error.\n";
      std::exit(file_reading_error);
    }
    if (line[0] == 'p') break;
  }
  std::stringstream s(line);
  {std::string inp; s >> inp;
   if (inp != "p") {
     std::cerr << "Syntax error in parameter line (no \"p \").\n";
     std::exit(file_reading_error);
   }
   s >> inp;
   if (inp != "cnf") {
     std::cerr << "Syntax error in parameter line (no \"cnf\").\n";
     std::exit(file_reading_error);
   }
  }
  s >> n_vars;
  if (n_vars > Var(std::numeric_limits<Lit>::max())) {
    std::cerr << "Parameter n=" << n_vars << " is too big for numeric_limits<Lit>::max=" << std::numeric_limits<Lit>::max() << "\n";
    std::exit(num_vars_error);
  }
  s >> n_header_clauses;
  if (not s) {
    std::cerr << "Reading error with parameters.\n";
    std::exit(file_reading_error);
  }
  lits.resize(n_vars+1);
  n_changes.resize(n_vars);
  checker.resize(n_vars+1);
  out.resize(n_vars);
  gucl_stack.resize(n_vars);
  clauses.resize(n_header_clauses+1);
}

Lit current_working_clause[max_clause_length];
int cwc_length;

bool read_a_clause_from_file(std::ifstream& f) {
  cwc_length = 0;
  bool tautology = false;
  Lit x;
  f >> x;
  if (f.eof()) return false;
  std::vector<int> checker_cl(n_vars+1);
  while (true) {
    if (not f) {
      std::cerr << "Invalid literal-read.\n";
      std::exit(file_reading_error);
    }
    if (x == 0) break;
    const Var v = std::abs(x);
    if (v > n_vars) {
      std::cerr << "Literal " << x << " contradicts n=" << n_vars << ".\n";
      std::exit(variable_value_error);
    }
    if (checker_cl[v] == 0) {
      if (cwc_length >= max_clause_length) {
        std::cerr << "Clauses can have at most " << max_clause_length << " elements.\n";
        std::exit(clause_length_error);
      }
      current_working_clause[cwc_length++] = x;
      checker_cl[v] = x;
    }
    else if (checker_cl[v] == -x) tautology = true;
    f >> x;
  }
  if (tautology) {
    cwc_length = 0;
    return true;
  }
  if (cwc_length == 0) {
    std::cerr << "Found empty clause.\n";
    std::exit(empty_clause_error);
  }
  return true;
}

void add_a_clause_to_formula(const Lit A[], const unsigned n) {
  if (n == 0) return;
  if (n_clauses >= n_header_clauses) {
    std::cerr << "More than " << n_header_clauses << " clauses, contradicting cnf-header.\n";
    std::exit(number_clauses_error);
  }
  auto& C = clauses[n_clauses];
  C.number = n_clauses;
  C.status = true;
  C.length = n;
  C.value = (Clause_content(1) << n) - 1;
  C.c_ucl = 0;
  C.literals = new Lit[n];

  if (n>act_max_clause_length) act_max_clause_length = n;

  for (int i=0; i<(int)n; ++i) {
    const Lit x = A[i];
    const Var v = std::abs(x);
    const Polarity p = x > 0 ? pos : neg;
    auto& L = lits[v][p];
    L.clause_occ = (unsigned int*) std::realloc(L.clause_occ, (L.n_occur+1)*sizeof(unsigned int));
    L.clause_index = (unsigned int*) std::realloc(L.clause_index, (L.n_occur+1)*sizeof(unsigned int));
    L.clause_occ[L.n_occur] = n_clauses;
    L.clause_index[L.n_occur] = i;
    ++L.n_occur;
    L.status = true;
    C.literals[i] = x;
  }
  ++n_clauses;
}

void read_formula(const std::string& filename) {
  std::ifstream f(filename);
  if (not f) {
    std::cerr << "Invalid file name.\n";
    std::exit(file_reading_error);
  }
  read_formula_header(f);
  n_clauses = 0;
  while (read_a_clause_from_file(f))
    add_a_clause_to_formula(current_working_clause, cwc_length);
  r_clauses = n_clauses;
}


// Defining "special logarithm" log2s(v)=k, where v=2^k, k natural number:
constexpr Clause_content pow2(const unsigned e) {return (e==0)?1:2*pow2(e-1);}
constexpr int log2(const Clause_content n) {return (n <= 1)?0:1+log2(n/2);}
constexpr int N = log2(max_clause_length);
static_assert(pow2(N) == (unsigned) max_clause_length, "Number of bits in \"Clause_content\" not a power of 2.");
static_assert(N==3 or N==4 or N==5 or N==6, "Unexpected size of type \"Clause_content\".");
constexpr Clause_content pow22(const unsigned e) {return pow2(pow2(e));}
constexpr Clause_content B(const unsigned N, const unsigned i) {
  return (i>=N) ? 0 : (i<N-1) ? B(N-1,i)*(1+pow22(N-1)) : pow22(N)-pow22(N-1);
}
constexpr Clause_content b[6] {B(N,0),B(N,1),B(N,2),B(N,3),B(N,4),B(N,5)};
inline int log2s(const Clause_content v) {
  assert(pow2(log2(v)) == v);
  Clause_content r = bool(v & b[0]);
  for (int i = 1; i < N; ++i) r |= bool(v & b[i]) << i;
  return r;
}

void assign(const Lit x) {
  const Var v = std::abs(x);
  assert(v <= n_vars);
  const Polarity p = (x>0) ? pos : neg;
  {
   const auto L = lits[v][p];
   const auto occur_true = L.n_occur;
   const auto max_size = changes_index + occur_true;
   if (max_size >= changes.size()) changes.resize(max_size);
   for (unsigned int i=0; i<occur_true; ++i) {
     const auto m = L.clause_occ[i];
     auto& C = clauses[m];
     if (not C.status) continue;
     C.status = false;
     assert(r_clauses >= 1);
     --r_clauses;
     changes[changes_index++].clause_number = m;
     ++n_changes[depth][pos];
   }
  }
  {
   const Polarity np = (x>0) ? neg : pos;
   const auto L = lits[v][np];
   const auto occur_false = L.n_occur;
   const auto max_size = changes_index + occur_false;
   if (max_size > changes.size()) changes.resize(max_size);
   for (unsigned int i=0; i<occur_false; ++i) {
     const auto m = L.clause_occ[i];
     auto& C = clauses[m];
     if (not C.status) continue;
     assert(C.length >= 2);
     --C.length;
     const auto n = L.clause_index[i];
     C.value -= Clause_content(1) << n;

     changes[changes_index++] = {m,n};
     ++n_changes[depth][neg];

     if (C.length == 1) {
       const Lit ucl = C.literals[log2s(C.value)];
       const Var aucl = std::abs(ucl);
       if (checker[aucl] == 0) {
         gucl_stack[n_gucl++] = ucl;
         checker[aucl] = ucl;
         C.c_ucl = ucl;
       }
       if (checker[aucl]+ucl == 0) {
         contradictory_unit_clauses = true;
         checker[aucl] = 0;
       }
     }
   }
  }
  ++depth;
  lits[v][pos].status = false;
  lits[v][neg].status = false;
}

void unassign(const Lit x) {
  const Var v = std::abs(x);
  assert(depth >= 1);
  --depth;
  while (n_changes[depth][neg]) {
    --n_changes[depth][neg];
    const auto m = changes[--changes_index].clause_number;
    const auto n = changes[changes_index].literal_index;
    auto& C = clauses[m];
    ++C.length;
    if (C.length == 2) {
      checker[std::abs(C.c_ucl)] = 0;
      C.c_ucl = 0;
    }
    C.value += Clause_content(1) << n;
  }

  while (n_changes[depth][pos]) {
    --n_changes[depth][pos];
    const auto m = changes[--changes_index].clause_number;
    clauses[m].status = true;
    ++r_clauses;
  }
  lits[v][pos].status = true;
  lits[v][neg].status = true;
}

inline Lit branching_literal_2sjw() {
  double max = 0;
  Lit x = 0;
  const auto mlen = act_max_clause_length;
  const auto nvar = n_vars;
  for (Lit i=1; (unsigned)i <= nvar; ++i) {
    const auto& vpos = lits[i][pos];
    const auto& vneg = lits[i][neg];
    if (vpos.status or vneg.status) {
      double pz = 0;
      {const auto pos_occur = vpos.n_occur;
       for (unsigned int k=0; k<pos_occur; ++k) {
         const auto ci = vpos.clause_occ[k];
         assert(ci < clauses.size());
         pz += Clause_content(clauses[ci].status) << (mlen - clauses[ci].length);
       }}
      double nz = 0;
      {const auto neg_occur = vneg.n_occur;
       for (unsigned int k=0; k<neg_occur; ++k) {
         const auto ci = vneg.clause_occ[k];
         assert(ci < clauses.size());
         nz += Clause_content(clauses[ci].status) << (mlen - clauses[ci].length);
       }}
      const auto s = pz + nz;
      if (s > max) { max = s; x = (pz >= nz) ? i : -i; }
    }
  }
  return x;
}

bool dpll() {
  ++n_branches;
  std::stack<Lit> lucl_stack;
  while (true) {
    if (contradictory_unit_clauses) {
      while (not lucl_stack.empty()) {
        unassign(lucl_stack.top());
        lucl_stack.pop();
        out[depth] = 0;
      }
      contradictory_unit_clauses = false;
      n_gucl = 0;
      return false;
    }
    else if (n_gucl) {
      const Lit implied_literal = gucl_stack[--n_gucl];
      lucl_stack.push(implied_literal);
      out[depth] = implied_literal;
      assign(implied_literal);
      ++n_units;
    }
    else break;
  }
  if (!r_clauses) return true;
  const Lit x = branching_literal_2sjw();
  assert(x);
  assert(depth < n_vars);
  out[depth] = x;
  assign(x);
  if (dpll()) return true;
  unassign(x);
  ++n_backtracks;

  const Lit nx = -x;
  out[depth] = nx;
  assign(nx);
  if (dpll()) return true;
  unassign(nx);
  out[depth] = 0;

  while (not lucl_stack.empty()) {
    unassign(lucl_stack.top());
    lucl_stack.pop();
  }
  contradictory_unit_clauses = false;
  return false;
}

void output(const std::string& file, const bool result, const double elapsed) {
  std::cout << "s ";
  if (not result) std::cout << "UN";
  std::cout << "SATISFIABLE\n";
  std::cout <<
         "c number_of_variables                   " << n_vars << "\n" <<
         "c number_of_clauses                     " << n_clauses << "\n" <<
         "c running_time(sec)                     " << std::setprecision(2) << std::fixed << elapsed << "\n" <<
         "c number_of_nodes                       " << n_branches << "\n" <<
         "c number_of_binary_nodes                " << n_backtracks << "\n" <<
         "c number_of_1-reductions                " << n_units << "\n" <<
         "c max_number_changes                    " << changes.size() << "\n" <<
         "c file_name                             " << file << std::endl;
  if (result) {
    std::vector<int> order(n_vars);
    for (Var i=0; i<n_vars; i++) {
      const auto val = out[i];
      const auto index = std::abs(val)-1;
      if (val > 0) order[index] = 1;
      else if (val < 0) order[index] = -1;
    }
    std::cout << "v ";
    for (Lit i=0; i< (Lit)n_vars; ++i)
      if (order[i]) std::cout << order[i]*(i+1) << " ";
    std::cout << "0" << std::endl;
  }
}


int main(const int argc, const char* const argv[]) {
  if (argc < 2) {
    std::cerr << "Missing file name.\n";
    return missing_file_error;
  }
  const std::string filename(argv[1]);
  read_formula(filename);
  rusage runtime;
  getrusage(RUSAGE_SELF, &runtime);
  const double t1 = runtime.ru_utime.tv_sec+runtime.ru_utime.tv_usec/1000000.0;
  const bool result = dpll();
  getrusage(RUSAGE_SELF, &runtime);
  const double t2 = runtime.ru_utime.tv_sec+runtime.ru_utime.tv_usec/1000000.0;
  output(filename, result, t2-t1);
  return (result) ? sat : unsat;
}
