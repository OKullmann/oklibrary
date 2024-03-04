// Oliver Kullmann, 3.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  SAT translation of cyclic pandiagonal squares (refining
  Pandiagonal.cpp)

  "Cyclic" here means simple that for every every cell with value x
  the cyclically next cell ("wrapping around") has value x+1 mod N.
  This is called "horizontally semicyclic" in https://oeis.org/A071607 .

  The problem is the same as finding a modular ("toroidal") N-Queens
  solution (placing N queens) with one queen placed top-left.

TODOS:

1. Due to the symmetry, only one diagonal and one antidiagonal constraint
   is needed here.


EXAMPLES:

Viewing the four (cyclic) pandiagonal Sudokus for N=11:

LatinSquares> N=11; ./CPandiagonal +$N "" | clasp 0 | ./CP_clasp_first_columns.awk -v N=$N | ./CP_clasp_expand.awk -v N=$N
  0  1  2  3  4  5  6  7  8  9 10
  4  5  6  7  8  9 10  0  1  2  3
  8  9 10  0  1  2  3  4  5  6  7
  1  2  3  4  5  6  7  8  9 10  0
  5  6  7  8  9 10  0  1  2  3  4
  9 10  0  1  2  3  4  5  6  7  8
  2  3  4  5  6  7  8  9 10  0  1
  6  7  8  9 10  0  1  2  3  4  5
 10  0  1  2  3  4  5  6  7  8  9
  3  4  5  6  7  8  9 10  0  1  2
  7  8  9 10  0  1  2  3  4  5  6

  0  1  2  3  4  5  6  7  8  9 10
  7  8  9 10  0  1  2  3  4  5  6
  3  4  5  6  7  8  9 10  0  1  2
 10  0  1  2  3  4  5  6  7  8  9
  6  7  8  9 10  0  1  2  3  4  5
  2  3  4  5  6  7  8  9 10  0  1
  9 10  0  1  2  3  4  5  6  7  8
  5  6  7  8  9 10  0  1  2  3  4
  1  2  3  4  5  6  7  8  9 10  0
  8  9 10  0  1  2  3  4  5  6  7
  4  5  6  7  8  9 10  0  1  2  3

  0  1  2  3  4  5  6  7  8  9 10
  8  9 10  0  1  2  3  4  5  6  7
  5  6  7  8  9 10  0  1  2  3  4
  2  3  4  5  6  7  8  9 10  0  1
 10  0  1  2  3  4  5  6  7  8  9
  7  8  9 10  0  1  2  3  4  5  6
  4  5  6  7  8  9 10  0  1  2  3
  1  2  3  4  5  6  7  8  9 10  0
  9 10  0  1  2  3  4  5  6  7  8
  6  7  8  9 10  0  1  2  3  4  5
  3  4  5  6  7  8  9 10  0  1  2

  0  1  2  3  4  5  6  7  8  9 10
  3  4  5  6  7  8  9 10  0  1  2
  6  7  8  9 10  0  1  2  3  4  5
  9 10  0  1  2  3  4  5  6  7  8
  1  2  3  4  5  6  7  8  9 10  0
  4  5  6  7  8  9 10  0  1  2  3
  7  8  9 10  0  1  2  3  4  5  6
 10  0  1  2  3  4  5  6  7  8  9
  2  3  4  5  6  7  8  9 10  0  1
  5  6  7  8  9 10  0  1  2  3  4
  8  9 10  0  1  2  3  4  5  6  7


One pandiagonal Sudoku for N=25:
LatinSquares> N=25; ./CPandiagonal +$N "" | clasp -t 1 | ./CP_clasp_first_columns.awk -v N=$N | ./CP_clasp_expand.awk -v N=$N
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
 15 16 17 18 19 20 21 22 23 24  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  0  1  2  3  4  5  6  7  8  9
  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  0  1  2  3  4
 20 21 22 23 24  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
 14 15 16 17 18 19 20 21 22 23 24  0  1  2  3  4  5  6  7  8  9 10 11 12 13
  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  0  1  2  3  4  5  6  7  8
 24  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
 19 20 21 22 23 24  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  0  1  2  3
 22 23 24  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
 17 18 19 20 21 22 23 24  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  0  1
  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  0  1  2  3  4  5  6
 12 13 14 15 16 17 18 19 20 21 22 23 24  0  1  2  3  4  5  6  7  8  9 10 11
  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  0  1  2  3  4  5  6  7
 13 14 15 16 17 18 19 20 21 22 23 24  0  1  2  3  4  5  6  7  8  9 10 11 12
 18 19 20 21 22 23 24  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  0  1  2
 23 24  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
 16 17 18 19 20 21 22 23 24  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  0
 21 22 23 24  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
 11 12 13 14 15 16 17 18 19 20 21 22 23 24  0  1  2  3  4  5  6  7  8  9 10
  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  0  1  2  3  4  5

Remark: the same chain of scripts works also for cadical (then only one
solution is extracted; also for OKsolver), e.g.
LatinSquares> N=29; ./CPandiagonal +$N "" | cadical | ./CP_clasp_first_columns.awk -v N=$N | ./CP_clasp_expand.awk -v N=$N
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28
 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10
  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4
 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7
 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1
 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6
 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0
 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11
 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9
 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3
 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2
 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13
 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26
  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8
 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12
 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28  0  1  2  3  4  5

Indeed seems OKsolver more effective than Cadical; for example in rather short time:
LatinSquares> N=37; ./CPandiagonal $N "" > TEMP$N; OKsolver2002 -O TEMP$N | ./CP_clasp_first_columns.awk -v N=$N | ./CP_clasp_expand.awk -v N=$N; rm TEMP$N
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36
  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1
 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3
 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9
 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0
 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13
 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28
 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35
 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34
 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8
 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11
 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10
 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5
 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26
 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12
 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29
 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4
 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7
 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2
  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6

A pandiagonal Sudoku for N=37 is harder to find; clasp with 12 threads found a solution rather quickly:
LatinSquares> N=37; ./CPandiagonal +$N "" > TEMP$N; clasp -t 12 TEMP$N | ./CP_clasp_first_columns.awk -v N=$N | ./CP_clasp_expand.awk -v N=$N; rm TEMP$N
 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5
 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12
 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26
 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7
  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1
  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6
 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0
 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13
 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8
  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2
 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11
 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35
 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29
  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4
 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9
 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34
 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28
 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22
  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3
 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36  0  1  2  3  4  5  6  7  8  9 10

Remark: a quick success is not guaranteed (which and how the threads are executed is not determined);
a manual restart might help.


Only counting:

The following sequence is
  https://oeis.org/A071607
  Number of strong complete mappings of the cyclic group Z_{2n+1}
for N=1,3,5,7,9,11, ... :
  1, 0, 2, 4, 0, 8, 348, 0, 8276, 43184, 0, 5602176, 78309000, 0, 20893691564, 432417667152, 0
LatinSquares> for (( i=11; i <=20; ++i)); do echo $i; ./CPandiagonal $i "" | clasp 0 -q -t 6 | awk '/^c Models/;/^c CPU/'; done
11
c Models         : 8
c CPU Time       : 0.006s
12
c Models         : 0
c CPU Time       : 0.012s
13
c Models         : 348
c CPU Time       : 0.025s
14
c Models         : 0
c CPU Time       : 0.148s
15
c Models         : 0
c CPU Time       : 0.731s
16
c Models         : 0
c CPU Time       : 0.977s
17
c Models         : 8276
c CPU Time       : 6.762s
18
c Models         : 0
c CPU Time       : 15.801s
19
c Models         : 43184
c CPU Time       : 195.162s
20
c Models         : 0
c CPU Time       : 188.548s


LatinSquares> for (( i=11; i <=20; ++i)); do echo $i; ./CPandiagonal +$i "" | clasp 0 -q | awk '/^c Models/;/^c CPU/'; done
11
c Models         : 4
c CPU Time       : 0.002s
12
c Models         : 0
c CPU Time       : 0.004s
13
c Models         : 112
c CPU Time       : 0.007s
14
c Models         : 0
c CPU Time       : 0.026s
15
c Models         : 0
c CPU Time       : 0.095s
16
c Models         : 0
c CPU Time       : 0.012s
17
c Models         : 28
c CPU Time       : 0.060s
18
c Models         : 0
c CPU Time       : 0.348s
19
c Models         : 30
c CPU Time       : 1.010s
20
c Models         : 0
c CPU Time       : 4.151s


The list of sizes for 1 <= N <= 25 (with debugging-version):

LatinSquares> for (( i=1; i <= 25; ++i )); do ./CPandiagonal_debug $i prime | awk '/^p cnf/'; done
p cnf 1 5
p cnf 4 17
p cnf 9 49
p cnf 16 113
p cnf 25 221
p cnf 36 385
p cnf 49 617
p cnf 64 929
p cnf 81 1333
p cnf 100 1841
p cnf 121 2465
p cnf 144 3217
p cnf 169 4109
p cnf 196 5153
p cnf 225 6361
p cnf 256 7745
p cnf 289 9317
p cnf 324 11089
p cnf 361 13073
p cnf 400 15281
p cnf 441 17725
p cnf 484 20417
p cnf 529 23369
p cnf 576 26593
p cnf 625 30101

LatinSquares> for (( i=1; i <= 25; ++i )); do ./CPandiagonal_debug $i seco | awk '/^p cnf/'; done
p cnf 1 5
p cnf 4 17
p cnf 9 49
p cnf 16 113
p cnf 45 201
p cnf 60 313
p cnf 105 449
p cnf 128 609
p cnf 189 793
p cnf 220 1001
p cnf 297 1233
p cnf 336 1489
p cnf 429 1769
p cnf 476 2073
p cnf 585 2401
p cnf 640 2753
p cnf 765 3129
p cnf 828 3529
p cnf 969 3953
p cnf 1040 4401
p cnf 1197 4873
p cnf 1276 5369
p cnf 1449 5889
p cnf 1536 6433
p cnf 1725 7001

LatinSquares> for (( i=1; i <= 25; ++i )); do ./CPandiagonal_debug $i secouep | awk '/^p cnf/'; done
p cnf 1 5
p cnf 4 17
p cnf 9 49
p cnf 16 113
p cnf 45 221
p cnf 60 337
p cnf 105 505
p cnf 128 673
p cnf 189 901
p cnf 220 1121
p cnf 297 1409
p cnf 336 1681
p cnf 429 2029
p cnf 476 2353
p cnf 585 2761
p cnf 640 3137
p cnf 765 3605
p cnf 828 4033
p cnf 969 4561
p cnf 1040 5041
p cnf 1197 5629
p cnf 1276 6161
p cnf 1449 6809
p cnf 1536 7393
p cnf 1725 8101

LatinSquares> for (( i=1; i <= 25; ++i )); do ./CPandiagonal_debug +$i prime | awk '/^p cnf/'; done
p cnf 1 5
p cnf 4 17
p cnf 9 49
p cnf 16 113
p cnf 25 221
p cnf 36 385
p cnf 49 617
p cnf 64 929
p cnf 81 4330
p cnf 100 5081
p cnf 121 7019
p cnf 144 10129
p cnf 169 11597
p cnf 196 14897
p cnf 225 19861
p cnf 256 38721
p cnf 289 41957
p cnf 324 49681
p cnf 361 60269
p cnf 400 75281
p cnf 441 80725
p cnf 484 92577
p cnf 529 108377
p cnf 576 130273
p cnf 625 218226


*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "PQOptions.hpp"
#include "PQEncoding.hpp"
#include "Commandline.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "4.3.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/CPandiagonal.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 2;

  using namespace Commandline;
  using namespace PQOptions;
  using namespace PQEncoding;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " [+]N constraint-type\n\n"
      " - N               : unsigned integer\n"
      " - constraint-type : " << Environment::WRPO<CT>{} << "\n\n" <<
      "Here\n"
      "  - \"+\" adds the (generalised) Sudoku-conditions\n"
      "  - for options the first possibility is the default, "
        "triggered by the empty string.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed, but the real number is " << argc-1 << ".\n";
    return 1;
  }

  const auto [N, sudoku] = read_dim(argv[1], error);
  const auto ct0 = Environment::read<CT>(argv[2]);
  if (not ct0) {
    std::cerr << error << "The constraint-type could not be read from"
        " string \"" << argv[2] << "\".\n";
    return 1;
  }
  const CT ct = ct0.value();

  const CEncoding enc(N, ct, sudoku);

  std::cout << Environment::Wrap(proginfo, Environment::OP::dimacs);
  using Environment::DHW;
  using Environment::DWW;
  std::cout << DHW{"Parameters"}
            << DWW{"command-line"};
  Environment::args_output(std::cout, argc, argv);
  std::cout << "\n"
            << DWW{"N"} << N << "\n"
            << DWW{"Constraint_type"} << ct << "\n"
            << DWW{"box-constraint"} << sudoku << "\n";
  if (sudoku) {
    std::cout << DWW{"  b,q,r"} << enc.b << " " << enc.q << " "
              << enc.r << "\n"
              << DWW{"  main,sides,corner"};
    PEncoding::output(std::cout, enc.boxes);
    std::cout << std::endl;
  }

  cpandiagonal(std::cout, enc, sudoku);
}
