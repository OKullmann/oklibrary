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
// g++ -Wall -Ofast tawSolver.cpp -o tawSolver

#include <limits>
#include <vector>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <sys/resource.h>

#ifndef MAX_VARS
# define MAX_VARS 10000
#endif
constexpr int max_vars = MAX_VARS;
static_assert(max_vars > 0, "MAX_VARS must be positive.");

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
  missing_file_error=1, file_reading_error=2, clause_length_error=3 };


constexpr int POS = 1;
constexpr int NEG = 0;

struct clause_info {
  int* literals;
  Clause_content value;
  int number;
  int length;
  int c_ucl;
  bool status;
};

clause_info* clauses;

// Defining "special logarithm" log2s(v)=k, where v=2^k, k natural number:
constexpr Clause_content pow2(const unsigned e) {return (e==0)?1:2*pow2(e-1);}
constexpr int log2(const Clause_content n) {return (n <= 1)?0:1+log2(n/2);}
constexpr int N = log2(max_clause_length);
static_assert(pow2(N) == (unsigned) max_clause_length, "Number of bits in \"Clause_content\" not a power of 2.");
static_assert(N==5 or N==6, "Unexpected size of type \"Clause_content\".");

struct var_info {
  int* var_in_clauses;
  int* var_in_clause_locs;
  unsigned int n_occur;
  int is_ucl;
  bool status;
};

var_info vars[max_vars+1][2];

struct change_info {
  int clause_number;
  int literal_index;
};

typedef std::vector<change_info> Change_v;
typedef Change_v::size_type change_index_t;
Change_v changes(1);
change_index_t changes_index = 0; // Invariant: changes_index < changes.size().

int n_changes[max_vars][2];

unsigned int n_clauses, r_clauses, n_init_clauses, n_vars, depth = 0;
unsigned int act_max_clause_length = 0;

int current_working_clause[max_clause_length], cwc_length;

int gucl_stack[max_vars], n_gucl = 0;
int contradictory_unit_clauses = false;

unsigned long long int n_branches = 0, n_units = 0;
unsigned long long int n_backtracks = 0;


FILE* open_formula_file(const char* const file_name) {
  FILE* const f = fopen(file_name, "r");
  if(!f) {
    printf("Invalid file name.\n");
    exit(file_reading_error);
  }
  else return f;
}

void read_formula_header(FILE* const f) {
  char str[256], p[2], cnf[4];
  while(1) {
    fgets(str, 256, f);
    if(str[0] == 'p') break;
  }
  sscanf(str, "%s %s %u %u", p, cnf, &n_vars, &n_clauses);
  clauses = (clause_info*) realloc(clauses, (n_clauses+1)*sizeof(clause_info));
  n_clauses = r_clauses = 0;
}

void close_formula_file(FILE* const f) { if (f) fclose(f); }

bool read_a_clause_from_file(FILE* const f) {
  bool trivial_clause = false;
  cwc_length = 0;
  int* const checker = (int*) calloc((n_vars+1), sizeof(int));
  while (true) {
    int x;
    if (fscanf(f, "%d", &x) == EOF) return false;
    if (x == 0) break;
    if (checker[std::abs(x)]==0) {
      if (cwc_length >= max_clause_length) {
        printf("Clauses can have at most %u elements.\n", max_clause_length);
        exit(clause_length_error);
      }
      current_working_clause[cwc_length++] = x;
      checker[std::abs(x)] = x;
    }
    else if (checker[std::abs(x)] + x == 0) trivial_clause = true;
  }
  if (trivial_clause) {
    cwc_length = 0;
    return true;
  }
  free(checker);
  if(cwc_length == 0) return false;
  return true;
}

void add_a_clause_to_formula(const int A[], const unsigned n) {
  if (n == 0) return;
  clauses[n_clauses].number = n_clauses;
  clauses[n_clauses].status = true;
  clauses[n_clauses].length = n;
  clauses[n_clauses].value = ((Clause_content) 1 << n) - 1;
  clauses[n_clauses].c_ucl = 0;
  clauses[n_clauses].literals = (int*) malloc((n + 1) * sizeof(int));

  if (n>act_max_clause_length) act_max_clause_length = n;

  for (int i=0; i<(int)n; ++i) {
    const int p = std::abs(A[i]), q = A[i]>0 ? POS : NEG;
    vars[p][q].var_in_clauses = (int*) realloc(vars[p][q].var_in_clauses,
                                (vars[p][q].n_occur+1) * sizeof(int));
    vars[p][q].var_in_clause_locs = (int*) realloc(vars[p][q].var_in_clause_locs,
                                (vars[p][q].n_occur+1) * sizeof(int));
    vars[p][q].var_in_clauses[vars[p][q].n_occur] = n_clauses;
    vars[p][q].var_in_clause_locs[vars[p][q].n_occur] = i;
    ++vars[p][q].n_occur;
    vars[p][q].status = true;
    vars[p][q].is_ucl = false;
    clauses[n_clauses].literals[i] = A[i];
  }
  ++n_clauses;
  ++r_clauses;
}

void read_formula(const char* const filename) {
  FILE* const f = open_formula_file(filename);
  read_formula_header(f);
  n_init_clauses = 0;
  while(read_a_clause_from_file(f)) {
    add_a_clause_to_formula(current_working_clause, cwc_length);
    ++n_init_clauses;
  }
  close_formula_file(f);
}


int checker[max_vars+1];

constexpr Clause_content pow22(const unsigned e) {return pow2(pow2(e));}
inline constexpr Clause_content bp(const unsigned N, const unsigned i) {
  return (i < N-1) ? bp(N-1,i) * (1 + pow22(N-1)) : pow22(N) - pow22(N-1);
}
const Clause_content b[6] {bp(N,0),bp(N,1),bp(N,2),bp(N,3),bp(N,4), (N==6)?bp(N,5):0}; // Unfortunately there is no reasonable way in C++ to just define b[N].
inline int log2s(const Clause_content v) {
  assert(pow2(log2(v)) == v);
  Clause_content r = (v & b[0]) != 0;
  for (int i = 1; i < N; ++i) r |= ((v & b[i]) != 0) << i;
  return r;
}

void reduce(const int v) {
  const int p = std::abs(v);
  {
   const int q = (v>0) ? POS : NEG;
   const auto occur_true = vars[p][q].n_occur;
   const auto max_size = changes_index + occur_true;
   if (max_size >= changes.size()) changes.resize(max_size);
   for (unsigned int i=0; i<occur_true; ++i) {
     const int m = vars[p][q].var_in_clauses[i];
     if(not clauses[m].status) continue;
     clauses[m].status = false;
     assert(r_clauses >= 1);
     --r_clauses;
     changes[changes_index++].clause_number = m;
     ++n_changes[depth][POS];
   }
  }
  {
   const int q = (v>0) ? NEG : POS;
   const auto occur_false = vars[p][q].n_occur;
   const auto max_size = changes_index + occur_false;
   if (max_size > changes.size()) changes.resize(max_size);
   for (unsigned int i=0; i<occur_false; ++i) {
     const int m = vars[p][q].var_in_clauses[i];
     if (!clauses[m].status) continue;
     assert(clauses[m].length >= 2);
     --clauses[m].length;
     const int n = vars[p][q].var_in_clause_locs[i];
     clauses[m].value -= (Clause_content) 1 << n;

     changes[changes_index++] = {m,n};
     ++n_changes[depth][NEG];

     if (clauses[m].length == 1) {
       const int ucl = clauses[m].literals[log2s(clauses[m].value)];
       const int aucl = std::abs(ucl);
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
  vars[p][POS].status = false;
  vars[p][NEG].status = false;
}

void reverse(const int v) {
  const int p = std::abs(v);
  assert(depth >= 1);
  --depth;
  while (n_changes[depth][NEG]) {
    --n_changes[depth][NEG];
    const int m = changes[--changes_index].clause_number;
    const int n = changes[changes_index].literal_index;
    ++clauses[m].length;
    if(clauses[m].length == 2) {
      checker[std::abs(clauses[m].c_ucl)] = 0;
      clauses[m].c_ucl = 0;
    }
    clauses[m].value += (Clause_content) 1 << n;
  }

  while (n_changes[depth][POS]) {
    --n_changes[depth][POS];
    const int m = changes[--changes_index].clause_number;
    clauses[m].status = true;
    ++r_clauses;
  }
  vars[p][POS].status = true;
  vars[p][NEG].status = true;
}

inline int get_variable_2sjw() {
  const auto mlen = act_max_clause_length;
  const auto nvar = n_vars;
  double max = 0;
  int v = 0;

  for (unsigned int i=1; i<=nvar; ++i) {
    const auto& vpos = vars[i][POS];
    const auto& vneg = vars[i][NEG];
    if (vpos.status or vneg.status) {
      double pz = 0;
      {const auto pos_occur = vpos.n_occur;
       for(unsigned int k=0; k<pos_occur; ++k) {
         const unsigned int ell = vpos.var_in_clauses[k];
         pz += (Clause_content) clauses[ell].status << (mlen - clauses[ell].length);
       }
      }
      double nz = 0;
      {const auto neg_occur = vneg.n_occur;
       for (unsigned int k=0; k<neg_occur; ++k) {
         const unsigned int ell = vneg.var_in_clauses[k];
         nz += (Clause_content) clauses[ell].status << (mlen - clauses[ell].length);
       }
      }
      const double s = pz + nz;
      if (s > max) { max = s; v = (pz >= nz) ? i : -i; }
    }
  }
  return v;
}

int out[max_vars];

bool dpll() {
  ++n_branches;
  unsigned int n_lucl = 0;
  int* lucl_stack = nullptr;
  while (true) {
    if (contradictory_unit_clauses) {
      while(n_lucl) {
        reverse(lucl_stack[--n_lucl]);
        out[depth] = 0;
      }
      contradictory_unit_clauses = false;
      free(lucl_stack);
      n_gucl = 0;
      return false;
    }
    else if (n_gucl) {
      lucl_stack = (int*) realloc(lucl_stack, (n_lucl + 1) * sizeof(int));
      const int implied_literal = gucl_stack[--n_gucl];
      out[depth] = lucl_stack[n_lucl++] = implied_literal;
      reduce(implied_literal);
      ++n_units;
    }
    else break;
  }
  if (!r_clauses) return true;
  const int v = get_variable_2sjw();
  assert(v);
  assert(depth < n_vars);
  out[depth] = v;
  reduce(v);
  if (dpll()) return true;
  reverse(v);
  ++n_backtracks;

  const int nv = -v;
  out[depth] = nv;
  reduce(nv);
  if (dpll()) return true;
  reverse(nv);
  out[depth] = 0;

  while (n_lucl) {
    reverse(lucl_stack[--n_lucl]);
    out[depth] = 0;
  }
  free(lucl_stack);
  contradictory_unit_clauses = false;
  return false;
}

void output(const char* const file, const bool result, const double elapsed) {
  printf("s ");
  if (result) printf("SATISFIABLE\n"); else printf("UNSATISFIABLE\n");
  printf("c number_of_variables                   %u\n"
         "c number_of_clauses                     %u\n"
         "c running_time(sec)                     %1.2f\n"
         "c number_of_nodes                       %llu\n"
         "c number_of_binary_nodes                %llu\n"
         "c number_of_1-reductions                %llu\n"
         "c max_number_changes                    %lu\n"
         "c file_name                             %s\n",
       n_vars, n_init_clauses, elapsed, n_branches, n_backtracks, n_units, changes.size(), file);
  if (result) {
    int order[max_vars];
    for (unsigned int i=0; i<n_vars; i++) {
      const auto val = out[i];
      const auto index = std::abs(val)-1;
      if (val > 0) order[index] = 1;
      else if (val < 0) order[index] = -1;
      else order[index] = 0;
    }
    printf("v ");
    for (unsigned int i=0; i<n_vars; ++i)
     if (order[i]) printf("%d ", order[i]*(i+1));
    printf("0\n");
  }
}


int main(const int argc, const char* const argv[]) {
  if (argc < 2) {
    printf("Missing file name.\n");
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
