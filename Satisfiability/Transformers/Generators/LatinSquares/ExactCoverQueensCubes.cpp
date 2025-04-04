// Oliver Kullmann, 3.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Same input as ExpandQueensCubes.cpp, but now creating a program
  to be compiled with the system
    Ben Lynn <blynn@cs.stanford.edu> https://github.com/blynn/dlx
  for running the DLX-algorithms to compute all solutions.

USAGE:

LatinSquares> ExactCoverQueensCubes

reads from standard input, determines N (dimension) and m (number of
cubes), and creates the file

  EC_QC_N_m.c

To be compiled by

LatinSquares> gcc -O3 -Wall -o EC_QC_N_m EC_QC_N_m.c dlx.c

The whole chain of action is conveniently put into
ExactCoverQueensCubes.bash.


EXAMPLES:

1. N=5

First showing the 2 modular Queens-solutions for N=5 (the various programs
used need to be on the path, for example by putting a symbolic link to them
into ~/bin):
LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1
 0 3 1 4 2
 0 2 4 1 3
Now piping these two solutions into ExactCoverQueensCubes (one could also
input them directly on standard input), creating EC_QC_5_2.c; the output
shows how to compile it together with dlx.c (in this directory):
LatinSquares> N=5; CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1 | ./ExactCoverQueensCubes
Compile with:
gcc -O3 -Wall -o EC_QC_5_2 EC_QC_5_2.c

Showing the main block of QC_5_2.c:
dlx_set(d,0,0);dlx_set(d,0,8);dlx_set(d,0,11);dlx_set(d,0,19);dlx_set(d,0,22);
dlx_set(d,1,0);dlx_set(d,1,7);dlx_set(d,1,14);dlx_set(d,1,16);dlx_set(d,1,23);
dlx_set(d,2,1);dlx_set(d,2,9);dlx_set(d,2,12);dlx_set(d,2,15);dlx_set(d,2,23);
dlx_set(d,3,1);dlx_set(d,3,8);dlx_set(d,3,10);dlx_set(d,3,17);dlx_set(d,3,24);
dlx_set(d,4,2);dlx_set(d,4,5);dlx_set(d,4,13);dlx_set(d,4,16);dlx_set(d,4,24);
dlx_set(d,5,2);dlx_set(d,5,9);dlx_set(d,5,11);dlx_set(d,5,18);dlx_set(d,5,20);
dlx_set(d,6,3);dlx_set(d,6,6);dlx_set(d,6,14);dlx_set(d,6,17);dlx_set(d,6,20);
dlx_set(d,7,3);dlx_set(d,7,5);dlx_set(d,7,12);dlx_set(d,7,19);dlx_set(d,7,21);
dlx_set(d,8,4);dlx_set(d,8,7);dlx_set(d,8,10);dlx_set(d,8,18);dlx_set(d,8,21);
dlx_set(d,9,4);dlx_set(d,9,6);dlx_set(d,9,13);dlx_set(d,9,15);dlx_set(d,9,22);

LatinSquares> ./EC_QC_5_2
 0 0 0 0 0
 1 1 1 1 1

Interpreting the first solution "0 0 0 0 0": for digit 0 use cube 0, for
digit 1 ... ; thus we get
0 1 2 3 4
2 3 4 0 1
4 0 1 2 3
1 2 3 4 0
3 4 0 1 2

Remark: The above standard call of ExactCoverQueensCubes, with the order
of input-cubes as used in the OKlibrary (so that one can consistently refer
to them via indices), plus the compilation, executation, and deletion of
the two created files (c-file, executable), is put into a script:
LatinSquares> ./ExactCoverQueensCubes.bash 5
 0 0 0 0 0
 1 1 1 1 1


LatinSquares> ./ExactCoverQueensCubes.bash 7
 0 0 0 0 0 0 0
 1 1 1 1 1 1 1
 2 2 2 2 2 2 2
 3 3 3 3 3 3 3

LatinSquares> ./ExactCoverQueensCubes.bash 11
 0 0 0 0 0 0 0 0 0 0 0
 1 1 1 1 1 1 1 1 1 1 1
 2 2 2 2 2 2 2 2 2 2 2
 3 3 3 3 3 3 3 3 3 3 3
 4 4 4 4 4 4 4 4 4 4 4
 5 5 5 5 5 5 5 5 5 5 5
 6 6 6 6 6 6 6 6 6 6 6
 7 7 7 7 7 7 7 7 7 7 7

N=13 is the first harder computation:

LatinSquares> N=13; CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1 | ./ExactCoverQueensCubes
LatinSquares> time gcc -O3 -Wall -o EC_QC_13_348 EC_QC_13_348.c dlx.c
real	0m8.209s
user	0m7.966s
sys	0m0.243s
LatinSquares> time ./EC_QC_13_348 | Sort > OUT13
real	7m22.819s
user	7m23.075s
sys	0m0.416s
LatinSquares> wc -l OUT13
12386 OUT13
LatinSquares> head -2 OUT13
  0   0   0   0   0   0   0   0   0   0   0   0   0
  0   4  10  26  28   2   1  10   2  26  28   1  20

We see that the first solution found is cyclic, while the second isn't.

Reminder: The same output into file OUT13 can be achieved by
LatinSquares> time ./ExactCoverQueensCubes.bash 13 > OUT13


OLD output-format (just the raw row-indices):
On a machien with larger memory (~ 27GB needed):
LatinSquares> N=17; CPandiagonal $N "" | clasp 0 | CP_clasp_first_columns.awk -v N=$N | ./ExactCoverQueensCubes
/usr/bin/ld: warning: /tmp/ccrF7UBT.o: requires executable stack (because the .note.GNU-stack section is executable)
real    8m25.847s
user    8m5.052s
sys     0m20.759s
LatinSquares> ulimit -s unlimited
LatinSquares> ls -l EC_QC_17_8276*
-rwxr-xr-x 1 oliver users 43045072 Apr  4 05:35 EC_QC_17_8276
-rw-r--r-- 1 oliver users 49960512 Apr  4 05:26 EC_QC_17_8276.c
LatinSquares> time ./EC_QC_17_8276 > OUT17
 0 16583 33049 74484 41706 33188 136756 124171 62100 14541 53996 68659 82844 95383 107533 111756 122129
 0 16583 42760 33336 82764 100019 26208 111084 66515 65585 49687 14454 77980 91343 116571 130318 132648
 0 16583 57963 43538 36748 132421 127335 69852 25934 74489 8276 49660 84918 91120 99396 108694 120223
 0 16583 65455 90283 41401 29182 132723 131793 76935 8297 35262 51814 70562 93487 106965 107588 120211
 0 16583 66229 33131 77394 123387 91063 64197 124145 24832 11920 47645 49656 85670 99343 115111 136060
 0 16583 66915 36014 128553 99312 66153 136775 25154 8307 42486 54015 78831 83086 99257 108295 120277
 0 16583 70557 115869 33104 82791 28472 102956 51036 45793 12125 61781 78830 91040 111934 128489 136829
 0 16583 85955 45726 74211 29174 77679 124085 12623 33430 53152 57932 97214 105490 107619 132143 140637
aborted:
real    29398m14.145s
user    29396m48.504s
sys     0m0.644s
LatinSquares> awk -v m=8276 'BEGIN{PROCINFO["sorted_in"]="@ind_num_asc"}{delete A;for (i=1;i<=NF;++i) A[$i]="";for (i in A) {d=int(i/m);r=i%m;printf " %d,%d",d,r}printf "\n"}' OUT17
 0,0 1,6265 2,31 3,8221 4,84 5,326 6,4340 7,4168 8,2451 9,0 10,84 11,4347 12,8221 13,4168 14,6265 15,31 16,4340
 0,0 1,6178 2,31 3,1380 4,232 5,1380 6,31 7,7653 8,307 9,3496 10,4 11,307 12,707 13,3496 14,707 15,6178 16,232
 0,0 1,0 2,31 3,1106 4,3644 5,2158 6,4 7,31 8,3644 9,5 10,2158 11,84 12,84 13,1106 14,4359 15,3195 16,5
 0,0 1,21 2,31 3,4354 4,2158 5,21 6,2158 7,7523 8,4354 9,2451 10,7523 11,2451 12,7653 13,0 14,4347 15,7653 16,307
 0,0 1,3644 2,31 3,4 4,27 5,6265 6,0 7,6265 8,21 9,2910 10,2910 11,27 12,31 13,7523 14,7523 15,5 16,3644
 0,0 1,31 2,31 3,326 4,2910 5,1106 6,4359 7,8221 8,707 9,4347 10,326 11,8221 12,0 13,707 14,4413 15,4413 16,4359
 0,0 1,3849 2,31 3,3644 4,0 5,4413 6,1380 7,3849 8,4349 9,4346 10,31 11,4 12,3644 13,4346 14,5 15,4349 16,4413
 0,0 1,4347 2,31 3,4346 4,326 5,4346 6,3496 7,0 8,8003 9,3195 10,3195 11,6178 12,6178 13,31 14,8221 15,8003 16,8221
We see that none of these solutions are row-cyclic.

TODOS:

0. DONE Provide for translating the output:
  - Another program could do the translation (given the same input).
  - DONE (not provided)
    Or the created C-program prints the squares.
   - Perhaps this is indicated by passing N, M as the two parameters of
     the program.
  - DONE
    Likely the following output-format "d,r" (division, remainder) is always
    better: it shows for the "digits" 0,...,N-1 the choice of queens-solutions
    (appropriately shifted); the current output (the raw indices of the "rows")
    is translated as follows, using m=M:
awk -v m=M 'BEGIN{PROCINFO["sorted_in"]="@ind_num_asc"}{delete A;for (i=1;i<=NF;++i) A[$i]="";for (i in A) {d=int(i/m);r=i%m;printf " %d,%d",d,r}printf "\n"}' OUTPUT

1. DONE
   Interpretation of the queens in the example:
  - Since CP_clasp_first_columns.awk uses the default mode=0,
    the original output is the first column of the cyclic pandiagonal square.
  - This then gets re-interpreted as the positions of the queens.
  - So using mode=1 would be more appropriate.

2. Replace dlx.h|c by a native program:
  - Only needing the same input as ExactCoverQueensCubes.
  - One thread for each of the m possibilities for the first "queen" (digit).

*/

#include <iostream>
#include <fstream>
#include <sstream>

#include <ProgramOptions/Environment.hpp>

#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "25.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/ExactCoverQueensCubes.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 0;

  const std::string prefix = "EC_QC_", suffix = ".c";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> " << proginfo.prg <<
      "\n\n"
      "reads from standard input, establishes N, m, and creates file\n"
      "  " << prefix << "N_m" << suffix << " .\n\n"
 ;
    return true;
  }

  const std::string init_part1 = R"(
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
)";
  const std::string init_part2 = R"(
struct dlx_s;
typedef struct dlx_s *dlx_t;
dlx_t dlx_new();
void dlx_clear(dlx_t dlx);
int dlx_rows(dlx_t dlx);
int dlx_cols(dlx_t dlx);
void dlx_set(dlx_t dlx, int row, int col);
void dlx_mark_optional(dlx_t dlx, int col);
int dlx_remove_row(dlx_t p, int row);
int dlx_pick_row(dlx_t dlx, int row);
void dlx_forall_cover(dlx_t dlx, void (*cb)(int rows[], int n));
void dlx_solve(dlx_t dlx,
               void (*cover_cb)(int col, int s, int row),
               void (*uncover_cb)(),
               void (*found_cb)(),
               void (*stuck_cb)(int col));
)";

  const std::string init_part3 = R"(
#define F(i,n) for(int i = 0; i < n; i++)
#define C(i,n,dir) for(cell_ptr i = (n)->dir; i != n; i = i->dir)
struct cell_s;
typedef struct cell_s *cell_ptr;
struct cell_s {
  cell_ptr U, D, L, R;
  int n;
  union {
    cell_ptr c;
    int s;
  };
};
static cell_ptr LR_self(cell_ptr c) { return c->L = c->R = c; }
static cell_ptr UD_self(cell_ptr c) { return c->U = c->D = c; }
static cell_ptr LR_delete(cell_ptr c) {
  return c->L->R = c->R, c->R->L = c->L, c;
}
static cell_ptr UD_delete(cell_ptr c) {
  return c->U->D = c->D, c->D->U = c->U, c;
}
static cell_ptr UD_restore(cell_ptr c) { return c->U->D = c->D->U = c; }
static cell_ptr LR_restore(cell_ptr c) { return c->L->R = c->R->L = c; }
static cell_ptr LR_insert(cell_ptr j, cell_ptr k) {
  return j->L = k->L, j->R = k, k->L = k->L->R = j;
}
static cell_ptr UD_insert (cell_ptr j, cell_ptr k) {
  return j->U = k->U, j->D = k, k->U = k->U->D = j;
}
cell_ptr col_new() {
  cell_ptr c = malloc(sizeof(*c));
  UD_self(c)->s = 0;
  return c;
}
struct dlx_s {
  int ctabn, rtabn, ctab_alloc, rtab_alloc;
  cell_ptr *ctab, *rtab;
  cell_ptr root;
};
typedef struct dlx_s *dlx_t;
dlx_t dlx_new() {
  dlx_t p = malloc(sizeof(*p));
  p->ctabn = p->rtabn = 0;
  p->ctab_alloc = p->rtab_alloc = 8;
  p->ctab = malloc(sizeof(cell_ptr) * p->ctab_alloc);
  p->rtab = malloc(sizeof(cell_ptr) * p->rtab_alloc);
  p->root = LR_self(col_new());
  return p;
}
void free_row(cell_ptr r) {
  cell_ptr next;
  for(cell_ptr j = r->R; j != r; j = next) {
    next = j->R;
    free(j);
  }
  free(r);
}
void dlx_clear(dlx_t p) {
  F(i, p->rtabn) {
    cell_ptr r = p->rtab[i];
    if (r) free_row(r);
  }
  F(i, p->ctabn) free(p->ctab[i]);
  free(p->rtab);
  free(p->ctab);
  free(p->root);
  free(p);
}
int dlx_rows(dlx_t dlx) { return dlx->rtabn; }
int dlx_cols(dlx_t dlx) { return dlx->ctabn; }
void dlx_add_col(dlx_t p) {
  cell_ptr c = col_new();
  LR_insert(c, p->root);
  c->n = p->ctabn++;
  if (p->ctabn == p->ctab_alloc) {
    p->ctab = realloc(p->ctab, sizeof(cell_ptr) * (p->ctab_alloc *= 2));
  }
  p->ctab[c->n] = c;
}
void dlx_add_row(dlx_t p) {
  if (p->rtabn == p->rtab_alloc) {
    p->rtab = realloc(p->rtab, sizeof(cell_ptr) * (p->rtab_alloc *= 2));
  }
  p->rtab[p->rtabn++] = 0;
}
static void alloc_col(dlx_t p, int n) { while(p->ctabn <= n) dlx_add_col(p); }
static void alloc_row(dlx_t p, int n) { while(p->rtabn <= n) dlx_add_row(p); }
void dlx_mark_optional(dlx_t p, int col) {
  alloc_col(p, col);
  cell_ptr c = p->ctab[col];
  // Prevent undeletion by self-linking.
  LR_self(LR_delete(c));
}
void dlx_set(dlx_t p, int row, int col) {
  alloc_row(p, row);
  alloc_col(p, col);
  cell_ptr c = p->ctab[col];
  cell_ptr new1() {
    cell_ptr n = malloc(sizeof(*n));
    n->n = row;
    n->c = c;
    c->s++;
    UD_insert(n, c);
    return n;
  }
  cell_ptr *rp = p->rtab + row;
  if (!*rp) {
    *rp = LR_self(new1());
    return;
  }
  if ((*rp)->c->n == col) return;
  C(r, *rp, R) if (r->c->n == col) return;
  LR_insert(new1(), *rp);
}
static void cover_col(cell_ptr c) {
  LR_delete(c);
  C(i, c, D) C(j, i, R) UD_delete(j)->c->s--;
}
static void uncover_col(cell_ptr c) {
  C(i, c, U) C(j, i, L) UD_restore(j)->c->s++;
  LR_restore(c);
}
int dlx_pick_row(dlx_t p, int i) {
  if (i < 0 || i >= p->rtabn) return -1;
  cell_ptr r = p->rtab[i];
  if (!r) return 0;  // Empty row.
  cover_col(r->c);
  C(j, r, R) cover_col(j->c);
  return 0;
}
int dlx_remove_row(dlx_t p, int i) {
  if (i < 0 || i >= p->rtabn) return -1;
  cell_ptr r = p->rtab[i];
  if (!r) return 0;  // Empty row.
  UD_delete(r)->c->s--;
  C(j, r, R){
    UD_delete(j)->c->s--;
  }
  p->rtab[i] = 0;
  free_row(r);
  return 0;
}
void dlx_solve(dlx_t p,
               void (*try_cb)(int, int, int),
               void (*undo_cb)(void),
               void (*found_cb)(),
               void (*stuck_cb)()) {
  void recurse() {
    cell_ptr c = p->root->R;
    if (c == p->root) {
      if (found_cb) found_cb();
      return;
    }
    int s = INT_MAX;
    C(i, p->root, R) if (i->s < s) s = (c = i)->s;
    if (!s) {
      if (stuck_cb) stuck_cb(c->n);
      return;
    }
    cover_col(c);
    C(r, c, D) {
      if (try_cb) try_cb(c->n, s, r->n);
      C(j, r, R) cover_col(j->c);
      recurse();
      if (undo_cb) undo_cb();
      C(j, r, L) uncover_col(j->c);
    }
    uncover_col(c);
  }
  recurse();
}
void dlx_forall_cover(dlx_t p, void (*cb)(int[], int)) {
  int sol[p->rtabn], soln = 0;
  void cover(int c, int s, int r) { sol[soln++] = r; }
  void uncover() { soln--; }
  void found() { cb(sol, soln); }
  dlx_solve(p, cover, uncover, found, NULL);
}
)";

  const std::string init_part4 = R"(
int main() {
  setbuf(stdout, NULL);
  dlx_t d = dlx_new();

)";
  const std::string init_part = init_part1 + init_part2 +
    init_part3 + init_part4;

  using UInt_t = Algorithms::UInt_t;

  std::string final_part1(const UInt_t N, const UInt_t m) noexcept {
    std::ostringstream s;
    s << "\n  const int M = " << m << ";\n  int sorting[" << N << "];\n";
    return s.str();
  }
  const std::string final_part2 = R"(
  int comp(const void* const a, const void* const b) {
    const int arg1 = *(const int*)a, arg2 = *(const int*)b;
    return (arg1 > arg2) - (arg1 < arg2);
  }
  void f(int row[], const int n) {
    memcpy(sorting, row, n * sizeof(int));
    qsort(sorting, n, sizeof(int), comp);
    for (int i = 0; i < n; ++i) {
      const int v = sorting[i], r = v%M;
      printf(" %d", r);
    }
    printf("\n");
  }
  dlx_forall_cover(d, f);
  dlx_clear(d);
  return 0;
}
)";

  constexpr UInt_t cell_index(const UInt_t i, const UInt_t j,
                              const UInt_t N) noexcept {
    assert(i < N); assert(j < N);
    return i * N + j;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed (k, of), but the real number is " << argc-1 << ".\n";
    return 1;
  }

  const auto init_cubes = Algorithms::read_queens_cubing(std::cin);
  if (init_cubes.m == 0) {
    std::cout << "Empty input.\n";
    return 0;
  }

  const std::string basefilename = prefix + std::to_string(init_cubes.N) +
    "_" + std::to_string(init_cubes.m);
  const std::string filename = basefilename + suffix;
  std::ofstream file(filename);
  if (not file) {
    std::cerr << error << "Can not open file \"" << filename << "\" for"
      " writing.\n";
    return 1;
  }
  std::cout << "Compile with:\ngcc -O3 -Wall -o " << basefilename <<
    " " << filename << "\n";

  file << init_part;
  for (UInt_t co = 0, dlx_row=0; co < init_cubes.N; ++co)
    for (UInt_t cu = 0; cu < init_cubes.m; ++cu, ++dlx_row) {
      const auto Q = init_cubes.queens({co,cu});
      for (UInt_t i = 0; i < init_cubes.N; ++i) {
        const UInt_t j = Q[i];
        const UInt_t index = cell_index(i, j, init_cubes.N);
        file << "dlx_set(d," << dlx_row << "," << index << ");";
      }
      file << std::endl;
    }
  file << final_part1(init_cubes.N, init_cubes.m) << final_part2;
}
