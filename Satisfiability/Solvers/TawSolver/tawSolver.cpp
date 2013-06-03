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

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include <sys/time.h>
#include <sys/resource.h>


const int POS = 1;
const int NEG = 0;
const int UNSAT = 0;
const int SAT = 1;
const int MAX_CLAUSES = 300000;
const int MAX_VARS = 4096;

enum Error_codes { file_reading_error = 1 };

typedef struct clause_info{
  int number;
  int length;
  unsigned int value;
  int status;
  int c_ucl;
  int * literals;
}clause_info;

clause_info * clauses;

typedef struct var_info{
  int status;
  unsigned int n_occur;
  int * var_in_clauses;
  int * var_in_clause_locs;
  int is_ucl;
}var_info;


typedef struct change_info{
  int clause_number;	
  int literal_index;	
}change_info;

change_info changes[MAX_CLAUSES];
int n_changes[MAX_VARS][2], changes_index = 0;

var_info vars[4096][2];
int checker[4096], out[4096];

struct rusage runtime;
int t1,t2; FILE * of;
char outfile[32];

unsigned int n_clauses, r_clauses, n_init_clauses, n_vars, depth = 0;
int current_working_clause[256], cwc_length;
int n_trivial_clauses, n_duplicate_literals;
int gucl_stack[4096], gant_stack[4096], n_gucl = 0;

int contradictory_unit_clauses = false, conflicting_clause;
unsigned long long int n_branches = 0, n_units = 0;
unsigned int max_clause_len = 0;
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
  clauses = (clause_info *)realloc(clauses, (n_clauses+1)*sizeof(clause_info));
  n_clauses = r_clauses = 0;
}

void close_formula_file(FILE* const f) {
  if(f) fclose(f);
}

bool read_a_clause_from_file(FILE* const f) {
 // Assumption: clauses are of length 32 or less
  bool trivial_clause = false;
  cwc_length = 0;
  int* const checker = (int *) calloc((n_vars+1), sizeof(int));
  while(1) {
      int x;
      if(fscanf(f, "%d", &x) == EOF) return false;
      if(x == 0) break;
      if(checker[abs(x)]==0)
	{
	  current_working_clause[cwc_length++] = x;
	  checker[abs(x)] = x;
	}
      else if(checker[abs(x)] + x == 0)
	trivial_clause = true;
      else if(checker[abs(x)] == x)
	++n_duplicate_literals;
    }
  if(trivial_clause) {
      cwc_length = 0;
      ++n_trivial_clauses;
      return true;
    }
  free(checker);
  if(cwc_length == 0) return false;
  return true;
}

void add_a_clause_to_formula(const int A[], const unsigned int n) {
  if(n == 0) return;
  clauses[n_clauses].number = n_clauses;
  clauses[n_clauses].status = true;
  clauses[n_clauses].length = n;
  clauses[n_clauses].value = ((1<<n)-1);
  clauses[n_clauses].c_ucl = 0;
  clauses[n_clauses].literals = (int *) malloc((n + 1) * sizeof(int));

  if(n>max_clause_len) max_clause_len = n;

  for(unsigned int i=0; i<n; ++i) {
    int p = abs(A[i]), q = A[i]>0 ? POS : NEG;
    vars[p][q].var_in_clauses = (int *)realloc(vars[p][q].var_in_clauses,
				(vars[p][q].n_occur+1) * sizeof(int));
    vars[p][q].var_in_clause_locs = (int *)realloc(vars[p][q].var_in_clause_locs,
				(vars[p][q].n_occur+1) * sizeof(int));
    vars[p][q].var_in_clauses[vars[p][q].n_occur] = n_clauses;
    vars[p][q].var_in_clause_locs[vars[p][q].n_occur] = i;
    vars[p][q].n_occur++;
    vars[p][q].status = true;
    vars[p][q].is_ucl = false;
    clauses[n_clauses].literals[i] = A[i];
  }
  ++n_clauses;
  ++r_clauses;
}

void read_formula(const char* const filename) {
  FILE * f = open_formula_file(filename);
  read_formula_header(f);
  n_init_clauses = 0;
  while(read_a_clause_from_file(f)) {
    add_a_clause_to_formula(current_working_clause, cwc_length);    
    n_init_clauses++;
  }
  close_formula_file(f);
}

void reduce(const int v) {
  const int p = abs(v); int q = (v>0) ? POS : NEG;
  for(unsigned int i=0; i<vars[p][q].n_occur; ++i) {    
    const int m = vars[p][q].var_in_clauses[i];
    if(!clauses[m].status) continue;    
    clauses[m].status = false;
    --r_clauses;
    changes[changes_index++].clause_number = m;
    n_changes[depth][POS]++;
  }
  q = !q;
  for(unsigned int i=0; i<vars[p][q].n_occur; ++i) {
    const int m = vars[p][q].var_in_clauses[i];
    if(!clauses[m].status) continue;
    const int n = vars[p][q].var_in_clause_locs[i];
    --clauses[m].length;
    clauses[m].value -= ((1 << n));

    changes[changes_index].clause_number = m;
    changes[changes_index++].literal_index = n;
    n_changes[depth][NEG]++;

    if(clauses[m].length == 1) {      
      const int ucl = clauses[m].literals[int(log2(clauses[m].value))];     
           
      if(checker[abs(ucl)] == 0) {
	gucl_stack[n_gucl] = ucl;
	checker[abs(ucl)] = ucl;
	clauses[m].c_ucl = ucl;
	++n_gucl;
      }
      if(checker[abs(ucl)]+ucl == 0) {
	contradictory_unit_clauses = true;
	checker[abs(ucl)] = 0;
      }
    }
  }
  ++depth;
  vars[p][POS].status = false;
  vars[p][NEG].status = false;
}

void reverse(const int v) {
  const int p = abs(v);

  --depth;

  while(n_changes[depth][NEG]) {
    --n_changes[depth][NEG];
    const int m = changes[--changes_index].clause_number;
    const int n = changes[changes_index].literal_index;
    ++clauses[m].length;
    if(clauses[m].length == 2) {
      checker[abs(clauses[m].c_ucl)] = 0;
      clauses[m].c_ucl = 0;
    }
    clauses[m].value += ((1 << n));
  }

  while(n_changes[depth][POS]) {    
    --n_changes[depth][POS];
    const int m = changes[--changes_index].clause_number;
    clauses[m].status = true;
    ++r_clauses;
  }  
  vars[p][POS].status = true;
  vars[p][NEG].status = true;
}

inline int get_variable_2sjw() {
  const unsigned int mlen = max_clause_len;
  unsigned int max = 0;
  int v = 0;

  for(unsigned int i=1; i<=n_vars; ++i) {
    if(vars[i][POS].status | vars[i][NEG].status) {
       unsigned int pz = 0, nz = 0;
       for(unsigned int k=0; k<vars[i][POS].n_occur; ++k) {
	  const unsigned int ell = vars[i][POS].var_in_clauses[k];
	  pz += ((1 & clauses[ell].status) << (mlen - clauses[ell].length));
       }
       for(unsigned int k=0; k<vars[i][NEG].n_occur; ++k) {
	  const unsigned int ell = vars[i][NEG].var_in_clauses[k];
	  nz += ((1 & clauses[ell].status) << (mlen - clauses[ell].length));
       }
       const unsigned int s = pz + nz;
       if(s > max) {
	  max = s;
	  if(pz >= nz) v = i;
	  else v = -i;
       }
    }
  }
  return v;
}


int impl_clauses[4096], icl_cnt;
double max_resolved = 0.0;

char input_file[64];

int dpll() {
  n_branches++;

  getrusage(RUSAGE_SELF, &runtime);
  t2=(100*runtime.ru_utime.tv_sec)+(runtime.ru_utime.tv_usec/10000);

  unsigned int n_lucl = 0;
  int * lucl_stack = NULL;
  while(1) {
    if(contradictory_unit_clauses) {	  
      icl_cnt = 0;
      while(n_lucl) {
	reverse(lucl_stack[--n_lucl]);	
	out[depth] = 0;
      }
      contradictory_unit_clauses = false;
      free(lucl_stack);	  
      n_gucl = 0;
      return UNSAT;
    }
    else if (n_gucl) {
       lucl_stack = (int *) realloc(lucl_stack, (n_lucl + 1) * sizeof(int));
       const int implied_literal = gucl_stack[--n_gucl];
       out[depth] = lucl_stack[n_lucl++] = implied_literal;       
       reduce(implied_literal);
       n_units++;
    }
    else break;
  }
  if(!r_clauses) return SAT;
  
  int v = get_variable_2sjw();


  out[depth] = v;

  reduce(v);
  if(dpll()) return SAT;
  reverse(v);

  ++n_backtracks;
  
  v = -v;
  out[depth] = v;

  reduce(v);
  if(dpll()) return SAT;
  reverse(v);
  out[depth] = 0;


  while(n_lucl) {
    reverse(lucl_stack[--n_lucl]);
    out[depth] = 0;
  }

  free(lucl_stack);
  contradictory_unit_clauses = false;
  return UNSAT;
}


int order[4096];

void print_solution(const char* const file, const int result, FILE* const of) {
  if(result == SAT) {
    for(unsigned int i=0; i<n_vars; i++) {
      if(out[i]>0) order[abs(out[i])-1] = 1;
      else if(out[i]<0) order[abs(out[i])-1] = -1;
    }

    for(unsigned int i=0; i<n_vars; ++i) {
      printf("[%3d:", i+1);
      if(order[i]==1) printf("H],");
      if(order[i]==-1) printf("L],");
      if(order[i]==0) printf("*],");
      if((i+1)%16==0) printf("\n");
    }
    printf("\n");
  }

  getrusage(RUSAGE_SELF, &runtime);
  t2=(100*runtime.ru_utime.tv_sec)+(runtime.ru_utime.tv_usec/10000);
  printf("V_VARS: %d, N_CLAUSES: %d\n", n_vars, n_init_clauses);
  printf("N_UNITS: %llu, N_BRANCHES: %llu, N_BACK: %llu\n", n_units, n_branches, n_backtracks);
  printf("Running time: %d.%d%d seconds\n", (t2-t1)/100,
	  ((t2-t1)%100)/10, (((t2-t1)%100)%10));
}


int main(const int argc, const char* const argv[]) {
  read_formula(argv[1]);
  strcpy(input_file, argv[1]);

  getrusage(RUSAGE_SELF, &runtime);
  t1=(100*runtime.ru_utime.tv_sec)+(runtime.ru_utime.tv_usec/10000);

  int result = dpll();

  if(result) printf("%s is SATISFIABLE\n", argv[1]);
  else printf("%s is UNSATISFIABLE\n", argv[1]);
  print_solution(argv[1], result, of);
}
