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

// Compile with
// g++ --std=c++11 -Wall -Ofast -DNDEBUG -o tawSolver tawSolver.cpp

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
static_assert(max_clause_length==32 or max_clause_length==64,"Currently only MAX_CLAUSE_LENGTH=32,64 is possible.");
#if MAX_CLAUSE_LENGTH == 32
 typedef uint32_t Clause_content;
#else
 typedef uint64_t Clause_content;
#endif
static_assert(std::numeric_limits<Clause_content>::digits==max_clause_length,"Error with choice of type \"Clause_content\".");

enum Error_codes {
  missing_file_error=1, file_reading_error=2, clause_length_error=3,
  variable_value_error=4, number_clauses_error=5, empty_clause_error=6 };

enum Polarity { NEG = 0, POS = 1 };

typedef int Lit; // alternative: short
static_assert(std::numeric_limits<Lit>::digits <= 32, "Not prepared for literals with more than 32 bits.");
static_assert(std::is_signed<Lit>::value, "Type \"Lit\" must be signed integral.");
constexpr Lit forbidden_lit = std::numeric_limits<Lit>::min();

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
  int* clause_occ; // array with clause-numbers
  int* clause_index; // array with array-indices (for the clauses)
  unsigned int n_occur;
  bool status;
};
typedef std::array<lit_info,2> lit_info_pair;
std::vector<lit_info_pair> lits;

struct change_info {
  int clause_number;
  int literal_index;
};

typedef std::vector<change_info> Change_v;
typedef Change_v::size_type change_index_t;
Change_v changes(1);
change_index_t changes_index = 0; // Invariant: changes_index < changes.size().

typedef std::array<int,2> int_pair;
std::vector<int_pair> n_changes;

unsigned int n_clauses, n_header_clauses, r_clauses;
unsigned int n_vars, depth = 0;
unsigned int act_max_clause_length = 0;

std::vector<Lit> gucl_stack;
int n_gucl = 0;
int contradictory_unit_clauses = false;
std::vector<Lit> checker;

std::vector<Lit> out;

unsigned long long int n_branches = 0, n_units = 0;
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
    if (x == forbidden_lit) {
      std::cerr << "Literal " << x << " can not be negated.\n";
      std::exit(variable_value_error);
    }
    const Lit v = std::abs(x);
    if ((unsigned) v > n_vars) {
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
  clauses[n_clauses].number = n_clauses;
  clauses[n_clauses].status = true;
  clauses[n_clauses].length = n;
  clauses[n_clauses].value = (Clause_content(1) << n) - 1;
  clauses[n_clauses].c_ucl = 0;
  clauses[n_clauses].literals = new int[n];

  if (n>act_max_clause_length) act_max_clause_length = n;

  for (int i=0; i<(int)n; ++i) {
    const Lit p = std::abs(A[i]);
    const Polarity q = A[i]>0 ? POS : NEG;
    lits[p][q].clause_occ =
      (int*) std::realloc(lits[p][q].clause_occ,
                          (lits[p][q].n_occur+1) * sizeof(int));
    lits[p][q].clause_index =
      (int*) std::realloc(lits[p][q].clause_index,
                          (lits[p][q].n_occur+1) * sizeof(int));
    lits[p][q].clause_occ[lits[p][q].n_occur] = n_clauses;
    lits[p][q].clause_index[lits[p][q].n_occur] = i;
    ++lits[p][q].n_occur;
    lits[p][q].status = true;
    clauses[n_clauses].literals[i] = A[i];
  }
  ++n_clauses;
}

void read_formula(const char* const filename) {
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
static_assert(N==5 or N==6, "Unexpected size of type \"Clause_content\".");
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

void reduce(const Lit v) {
  const Lit p = std::abs(v);
  {
   const Polarity q = (v>0) ? POS : NEG;
   const auto occur_true = lits[p][q].n_occur;
   const auto max_size = changes_index + occur_true;
   if (max_size >= changes.size()) changes.resize(max_size);
   for (unsigned int i=0; i<occur_true; ++i) {
     const int m = lits[p][q].clause_occ[i];
     if (not clauses[m].status) continue;
     clauses[m].status = false;
     assert(r_clauses >= 1);
     --r_clauses;
     changes[changes_index++].clause_number = m;
     ++n_changes[depth][POS];
   }
  }
  {
   const Polarity q = (v>0) ? NEG : POS;
   const auto occur_false = lits[p][q].n_occur;
   const auto max_size = changes_index + occur_false;
   if (max_size > changes.size()) changes.resize(max_size);
   for (unsigned int i=0; i<occur_false; ++i) {
     const int m = lits[p][q].clause_occ[i];
     if (!clauses[m].status) continue;
     assert(clauses[m].length >= 2);
     --clauses[m].length;
     const int n = lits[p][q].clause_index[i];
     clauses[m].value -= Clause_content(1) << n;

     changes[changes_index++] = {m,n};
     ++n_changes[depth][NEG];

     if (clauses[m].length == 1) {
       const Lit ucl = clauses[m].literals[log2s(clauses[m].value)];
       const Lit aucl = std::abs(ucl);
       if (checker[aucl] == 0) {
         gucl_stack[n_gucl++] = ucl;
         checker[aucl] = ucl;
         clauses[m].c_ucl = ucl;
       }
       if (checker[aucl]+ucl == 0) {
         contradictory_unit_clauses = true;
         checker[aucl] = 0;
       }
     }
   }
  }
  ++depth;
  lits[p][POS].status = false;
  lits[p][NEG].status = false;
}

void reverse(const Lit v) {
  const Lit p = std::abs(v);
  assert(depth >= 1);
  --depth;
  while (n_changes[depth][NEG]) {
    --n_changes[depth][NEG];
    const int m = changes[--changes_index].clause_number;
    const int n = changes[changes_index].literal_index;
    ++clauses[m].length;
    if (clauses[m].length == 2) {
      checker[std::abs(clauses[m].c_ucl)] = 0;
      clauses[m].c_ucl = 0;
    }
    clauses[m].value += Clause_content(1) << n;
  }

  while (n_changes[depth][POS]) {
    --n_changes[depth][POS];
    const int m = changes[--changes_index].clause_number;
    clauses[m].status = true;
    ++r_clauses;
  }
  lits[p][POS].status = true;
  lits[p][NEG].status = true;
}

inline Lit get_variable_2sjw() {
  double max = 0;
  Lit v = 0;
  const auto mlen = act_max_clause_length;
  const auto nvar = n_vars;
  for (unsigned int i=1; i<=nvar; ++i) {
    const auto& vpos = lits[i][POS];
    const auto& vneg = lits[i][NEG];
    if (vpos.status or vneg.status) {
      double pz = 0;
      {const auto pos_occur = vpos.n_occur;
       for(unsigned int k=0; k<pos_occur; ++k) {
         const unsigned int ell = vpos.clause_occ[k];
         pz += Clause_content(clauses[ell].status) << (mlen - clauses[ell].length);
       }}
      double nz = 0;
      {const auto neg_occur = vneg.n_occur;
       for (unsigned int k=0; k<neg_occur; ++k) {
         const unsigned int ell = vneg.clause_occ[k];
         nz += Clause_content(clauses[ell].status) << (mlen - clauses[ell].length);
       }}
      const double s = pz + nz;
      if (s > max) { max = s; v = (pz >= nz) ? i : -i; }
    }
  }
  return v;
}

bool dpll() {
  ++n_branches;
  std::stack<Lit> lucl_stack;
  while (true) {
    if (contradictory_unit_clauses) {
      while (not lucl_stack.empty()) {
        reverse(lucl_stack.top());
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
      reduce(implied_literal);
      ++n_units;
    }
    else break;
  }
  if (!r_clauses) return true;
  const Lit v = get_variable_2sjw();
  assert(v);
  assert(depth < n_vars);
  out[depth] = v;
  reduce(v);
  if (dpll()) return true;
  reverse(v);
  ++n_backtracks;

  const Lit nv = -v;
  out[depth] = nv;
  reduce(nv);
  if (dpll()) return true;
  reverse(nv);
  out[depth] = 0;

  while (not lucl_stack.empty()) {
    reverse(lucl_stack.top());
    lucl_stack.pop();
  }
  contradictory_unit_clauses = false;
  return false;
}

void output(const char* const file, const bool result, const double elapsed) {
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
    for (unsigned int i=0; i<n_vars; i++) {
      const auto val = out[i];
      const auto index = std::abs(val)-1;
      if (val > 0) order[index] = 1;
      else if (val < 0) order[index] = -1;
    }
    std::cout << "v ";
    for (int i=0; i< (int)n_vars; ++i)
      if (order[i]) std::cout << order[i]*(i+1) << " ";
    std::cout << "0" << std::endl;
  }
}


int main(const int argc, const char* const argv[]) {
  if (argc < 2) {
    std::cerr << "Missing file name.\n";
    return missing_file_error;
  }
  read_formula(argv[1]);
  struct rusage runtime;
  getrusage(RUSAGE_SELF, &runtime);
  const double t1 = runtime.ru_utime.tv_sec+runtime.ru_utime.tv_usec/1000000.0;
  const bool result = dpll();
  getrusage(RUSAGE_SELF, &runtime);
  const double t2 = runtime.ru_utime.tv_sec+runtime.ru_utime.tv_usec/1000000.0;
  output(argv[1], result, t2-t1);
  return (result) ? 10 : 20;
}
