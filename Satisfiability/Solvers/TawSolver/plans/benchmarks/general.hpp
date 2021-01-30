// Oliver Kullmann, 30.1.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/benchmarks/general.hpp
  \brief General overview on benchmarks for the TawSolver


  \todo Run benchmarks
  <ul>
   <li> An overview is needed how the tawSolver (in various configurations)
   performs. </li>
  </ul>


  \todo Timing
  <ul>
   <li> Test instances created by
   \verbatim
> VanderWaerdenCNF-O3-DNDEBUG 3 12 134
> VanderWaerdenCNF-O3-DNDEBUG 3 12 135
   \endverbatim
   (now in app_tests/test_cases/full).
   </li>
   <li> On cswsok (3GHz):
   \verbatim
# ID 237cbfc4d9b772a29e125928959af14cb4495d3e
# Version 2.6.6

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     8.82
c number_of_nodes                       808309
c number_of_binary_nodes                404147
c number_of_1-reductions                9437027
c reading-and-set-up_time(sec)          0.002
c file_name                             VanDerWaerden_2-3-12_134.cnf
c options                               ""

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     10.78
c number_of_nodes                       961949
c number_of_binary_nodes                480974
c number_of_1-reductions                11312180
c reading-and-set-up_time(sec)          0.003
c file_name                             VanDerWaerden_2-3-12_135.cnf
c options                               ""

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     18.72
c number_of_nodes                       936151
c number_of_binary_nodes                468068
c number_of_1-reductions                11031117
c number_of_pure_literals               1492
c reading-and-set-up_time(sec)          0.002
c file_name                             VanDerWaerden_2-3-12_134.cnf
c options                               "PT5"

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     19.37
c number_of_nodes                       953179
c number_of_binary_nodes                476589
c number_of_1-reductions                11285634
c number_of_pure_literals               1317
c reading-and-set-up_time(sec)          0.001
c file_name                             VanDerWaerden_2-3-12_135.cnf
c options                               "PT5"

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     10.61
c number_of_nodes                       968509
c number_of_binary_nodes                484254
c number_of_1-reductions                11308431
c number_of_solutions                   1
c reading-and-set-up_time(sec)          0.001
c file_name                             VanDerWaerden_2-3-12_134.cnf
c options                               "A19"

c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     10.70
c number_of_nodes                       961949
c number_of_binary_nodes                480974
c number_of_1-reductions                11312180
c number_of_solutions                   0
c reading-and-set-up_time(sec)          0.001
c file_name                             VanDerWaerden_2-3-12_135.cnf
c options                               "A19"

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     19.20
c number_of_nodes                       969297
c number_of_binary_nodes                484648
c number_of_1-reductions                11427083
c number_of_solutions                   1
c reading-and-set-up_time(sec)          0.002
c file_name                             VanDerWaerden_2-3-12_134.cnf
c options                               "T5A19"

c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     19.23
c number_of_nodes                       953175
c number_of_binary_nodes                476587
c number_of_1-reductions                11286127
c number_of_solutions                   0
c reading-and-set-up_time(sec)          0.001
c file_name                             VanDerWaerden_2-3-12_135.cnf
c options                               "T5A19"

> oklib timing
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
8.81 8.88 8.81 8.87 8.93
tawSolver VanDerWaerden_2-3-12_135.cnf
10.78 10.65 10.70 10.65 10.73
ttawSolver VanDerWaerden_2-3-12_134.cnf
18.72 18.68 18.63 18.74 18.72
ttawSolver VanDerWaerden_2-3-12_135.cnf
19.36 19.35 19.39 19.45 19.23
ctawSolver VanDerWaerden_2-3-12_134.cnf
10.61 10.45 10.68 10.54 10.48
ctawSolver VanDerWaerden_2-3-12_135.cnf
10.70 10.67 10.64 10.64 10.63
cttawSolver VanDerWaerden_2-3-12_134.cnf
19.20 19.16 19.17 19.26 19.14
cttawSolver VanDerWaerden_2-3-12_135.cnf
19.23 19.16 19.17 19.15 19.23


# ID 4a74a73e720d378ba92dac09e95d9b76b46878db
# Version 2.6.7
> make timing
# (system-gcc 4.7.1) versus
> oklib timing (one directory up, with oklib-gcc 4.7.3).
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
8.71 8.67 8.70 8.75 8.76
8.87 8.77 8.81 8.89 8.91
tawSolver VanDerWaerden_2-3-12_135.cnf
10.64 10.56 10.60 10.65 10.57
10.90 10.74 10.76 10.70 10.81
ttawSolver VanDerWaerden_2-3-12_134.cnf
18.74 18.65 18.66 18.67 18.65
18.86 18.81 18.82 18.74 18.82
ttawSolver VanDerWaerden_2-3-12_135.cnf
19.23 19.25 19.22 19.25 19.25
19.45 19.41 19.35 19.38 19.45
ctawSolver VanDerWaerden_2-3-12_134.cnf
10.76 10.76 10.65 10.71 10.71
10.63 10.65 10.53 10.54 10.56
ctawSolver VanDerWaerden_2-3-12_135.cnf
10.84 10.92 10.98 10.84 10.85
10.72 10.72 10.69 10.71 10.69
cttawSolver VanDerWaerden_2-3-12_134.cnf
19.53 19.57 19.53 19.55 19.53
19.20 19.26 19.39 19.41 19.29
cttawSolver VanDerWaerden_2-3-12_135.cnf
19.47 19.54 19.47 19.43 19.47
19.20 19.18 19.30 19.25 19.26

# ID 4c72e7630862d70972c9c74636a4a328a45c39b7
# Version 2.7.9
> make timing
# (system-gcc 4.7.1)
# cswsok (one other process running (low memory usage))

tawSolver VanDerWaerden_2-3-12_134.cnf
8.98 8.94 9.04 8.98 9.02
tawSolver VanDerWaerden_2-3-12_135.cnf
10.97 10.96 10.96 10.99 10.93
ttawSolver VanDerWaerden_2-3-12_134.cnf
19.26 19.13 19.11 19.08 19.13
ttawSolver VanDerWaerden_2-3-12_135.cnf
19.74 19.72 19.72 19.66 19.73
ctawSolver VanDerWaerden_2-3-12_134.cnf
11.16 11.24 11.19 11.12 11.22
ctawSolver VanDerWaerden_2-3-12_135.cnf
11.31 11.26 11.34 11.25 11.34
cttawSolver VanDerWaerden_2-3-12_134.cnf
20.00 19.91 20.04 19.94 19.97
cttawSolver VanDerWaerden_2-3-12_135.cnf
19.98 20.05 20.01 19.90 19.96

Consistently slower than before; need to re-run without other process.
   \endverbatim
   </li>
  </ul>

*/
