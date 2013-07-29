// Oliver Kullmann, 8.6.2013 (Swansea)
/* Copyright 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/general.hpp
  \brief Plans regarding updating and improving the TawSolver


  \todo Create milestones


  \todo Provide information
  <ul>
   <li> On the ExternalSources page the data needs to be entered. </li>
   <li> http://sourceforge.net/projects/tawsolver/ </li>
  </ul>


  \todo Timing
  <ul>
   <li> Test instances created by
   \verbatim
> VanderWaerdenCNF-O3-DNDEBUG 3 12 134
> VanderWaerdenCNF-O3-DNDEBUG 3 12 135
   \endverbatim
   </li>
   <li> On csltok (2.7 GHz) resp. cswsok (3GHz):
   \verbatim
# Id: 588a9cf1d823c5e59090ad20938b1708e6961643

> tawSolver VanDerWaerden_2-3-12_134.cnf
V_VARS: 134, N_CLAUSES: 5172
N_UNITS: 20977611, N_BRANCHES: 1721206, N_BACK: 860593

csltok: 46.44 46.11 46.01 46.25

> tawSolver VanDerWaerden_2-3-12_135.cnf
V_VARS: 135, N_CLAUSES: 5251
N_UNITS: 21978048, N_BRANCHES: 1790733, N_BACK: 895366

csltok: 49.33 49.45 49.24 49.67 48.09


# ID: fb0212392a795e09e3c1ae7d188ad30d0f4e57cf

> tawSolver VanDerWaerden_2-3-12_134.cnf
csltok: 45.85 46.08 45.40 46.04
cswsok: 41.14 41.11 41.14 41.03

> tawSolver VanDerWaerden_2-3-12_135.cnf
csltok: 47.71 47.96 47.75 47.22 47.58
cswsok: 44.10 43.80 43.63 43.80 43.77

# A bit faster: Perhaps due to saving superfluous calls to getrusage.
# It might be, that actually most of the time for these calls is not recorded
# in these times, since it belongs to "sys" (when using "time") ?!


# Id: 0762218ce28bc0a8e9e6e87d092d1a6d86e1b7c6

> tawSolver VanDerWaerden_2-3-12_134.cnf
csltok: 44.99 45.77 45.18 45.37

> tawSolver VanDerWaerden_2-3-12_135.cnf
csltok: 48.36 47.98 47.92 48.41 46.99
# at the same time(!) for the previous version: 49.29 48.88 47.69

# So apparently the introduction of log2s saved 1% run-time.


# Id: 2407136679fc921445acde574e1d5b3cef1f718f
# Now with g++ version 4.7.3.

> tawSolver VanDerWaerden_2-3-12_134.cnf
s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c running_time(sec)                     41.28
c number_of_nodes                       1721206
c number_of_binary_nodes                860593
c number_of_1-reductions                20977611
c file_name                             VanDerWaerden_2-3-12_134.cnf
v 1 2 3 4 5 6 7 8 9 -10 11 12 13 14 15 16 17 18 -19 20 21 22 23 24 25 26 27 28 -29 30 31 -32 33 34 35 -36 37 -38 39 40 41 42 43 44 45 -46 47 48 -49 50 -51 52 53 54 -55 56 57 58 59 60 61 62 63 64 65 66 -67 -68 69 70 71 72 73 74 75 76 77 78 79 -80 81 82 83 -84 85 -86 87 88 -89 90 91 92 93 94 95 96 -97 98 -99 100 101 102 -103 104 105 -106 107 108 109 110 111 112 113 114 115 -116 117 118 119 120 121 122 123 124 -125 126 127 128 129 130 131 132 133 134 0

csltok: 45.61 45.93 45.72 45.45 45.51
cswsok: 41.28 41.31 41.17 41.17 41.25

> tawSolver VanDerWaerden_2-3-12_135.cnf
s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c running_time(sec)                     43.76
c number_of_nodes                       1790733
c number_of_binary_nodes                895366
c number_of_1-reductions                21978048
c file_name                             VanDerWaerden_2-3-12_135.cnf

csltok: 49.22 48.80 49.14 48.68 49.18
cswsok: 43.76 43.83 43.57 43.64 44.00

# ID: a5f506fae59b572b2dee6aeb0330676d65641acc
# Using a std::vector for the changes-array, checking for overflow (this
# should cost the most time), and allowing clauses of length
# up to 64.

> tawSolver VanDerWaerden_2-3-12_134.cnf
s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c running_time(sec)                     41.79
c number_of_nodes                       1721206
c number_of_binary_nodes                860593
c number_of_1-reductions                20977611
c max_number_changes                    9912
c file_name                             VanDerWaerden_2-3-12_134.cnf

cswsok: 41.75 41.79 41.65 41.48 41.84

> tawSolver VanDerWaerden_2-3-12_135.cnf
 UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c running_time(sec)                     44.81
c number_of_nodes                       1790733
c number_of_binary_nodes                895366
c number_of_1-reductions                21978048
c max_number_changes                    10029
c file_name                             VanDerWaerden_2-3-12_135.cnf

cswsok: 44.81 44.25 44.41 44.83 44.58

# Got apparently 1 - 2% slower.

# ID: 583ab3bbb194c94126373765958689d28e847639
# Corrected computations.

> tawSolver VanDerWaerden_2-3-12_134.cnf
cswsok: 46.21 46.25
> tawSolver64 VanDerWaerden_2-3-12_134.cnf
cswsok: 48.38 48.60

> tawSolver VanDerWaerden_2-3-12_135.cnf
cswsok: 48.99 49.14
> tawSolver64 VanDerWaerden_2-3-12_135.cnf
cswsok: 51.65 51.43

# The corrected computations make it more than 10% slower.

# ID: f1157a664c98c72c1a299339ae2690bc25c1fb22
# Input is now handled dynamically.

> tawSolver VanDerWaerden_2-3-12_134.cnf
cswsok: 46.21 46.57 46.10 46.36
> tawSolver64 VanDerWaerden_2-3-12_134.cnf
cswsok: 48.49 48.93 48.84 48.80

> tawSolver VanDerWaerden_2-3-12_135.cnf
cswsok: 48.98 49.84 49.37 49.33
> tawSolver64 VanDerWaerden_2-3-12_135.cnf
cswsok: 52.03 51.85 51.92 52.08

# ID: 0350beb15e9ddcc72f3eac9a7f40c9e92045ea47
# Now using (local) std::stack for storing unit-clauses (saving all
# realloc-calls in dpll()).

> tawSolver VanDerWaerden_2-3-12_134.cnf
cswsok: 45.39 45.50 45.46 44.31 43.51 43.48
# the first three calls had higher cpu-load
> tawSolver64 VanDerWaerden_2-3-12_134.cnf
cswsok: 47.49 47.82 47.46 47.82

> tawSolver VanDerWaerden_2-3-12_135.cnf
cswsok: 46.74 46.49 46.35 46.48
> tawSolver64 VanDerWaerden_2-3-12_135.cnf
cswsok: 50.71 50.45 50.63 50.39

# ID: 01cf7bccbc306bb96b5b7b33a3104a56fcb1452f
# Various type-corrections.

> tawSolver VanDerWaerden_2-3-12_134.cnf
cswsok: 42.42 42.41 42.29 42.36
> tawSolver64 VanDerWaerden_2-3-12_134.cnf
cswsok: 47.09 47.06 47.00 46.98

> tawSolver VanDerWaerden_2-3-12_135.cnf
cswsok: 45.34 45.41 45.04 45.39
> tawSolver64 VanDerWaerden_2-3-12_135.cnf
cswsok: 50.26 50.11 50.15 50.30

# ID: 691fe428fe57f20b22e963697cdd74a3e79f450c
# Explicitly requested loop-unrolling from gcc.
# following times on otherwise idle cswsok:

> tawSolver VanDerWaerden_2-3-12_134.cnf
cswsok: 38.12 38.15 38.31 37.96
> tawSolver64 VanDerWaerden_2-3-12_134.cnf
cswsok: 41.86 41.94 42.01 41.94

> tawSolver VanDerWaerden_2-3-12_135.cnf
cswsok: 40.22 40.18 40.21 40.38
> tawSolver64 VanDerWaerden_2-3-12_135.cnf
cswsok: 44.41 44.55 44.56 44.53

# ID 2da9342c0399452746517de41340ece4720fe495
# perhaps major change: pos now first.

> oklib timing
# cswsok (otherwise idle):

tawSolver VanDerWaerden_2-3-12_134.cnf
36.94 37.17 37.02 36.88 37.07
tawSolver64 VanDerWaerden_2-3-12_134.cnf
42.05 42.23 41.80 42.28 42.01
tawSolver VanDerWaerden_2-3-12_135.cnf
39.20 39.51 39.35 39.59 39.59
tawSolver64 VanDerWaerden_2-3-12_135.cnf
44.63 45.19 44.95 44.79 44.78

# apparently the 32-bit version got faster, while the 64-bit version got
# a bit slower.

# ID bd35d014e905b64e75571ed9199dd9cc8e4a4e38
# some clean-up

> oklib timing
# cswsok (one other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
37.71 37.83 37.71 37.79 37.76
tawSolver64 VanDerWaerden_2-3-12_134.cnf
42.81 42.91 43.08 42.80 42.96
tawSolver VanDerWaerden_2-3-12_135.cnf
40.07 40.28 40.27 40.00 40.12
tawSolver64 VanDerWaerden_2-3-12_135.cnf
45.70 45.74 45.74 45.66 45.67

# higher running time likely due to higher load.

# ID eae286456fc8912331d967e851dba3a4cfbb4898
# clean-up of handling of unit-clause propagation (perhaps most relevant, that
# now an assignment is aborted as soon as a contradiction was found)

> oklib timing
# cswsok (one other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
35.23 35.27 35.30 35.11
tawSolver64 VanDerWaerden_2-3-12_134.cnf
42.16 42.14 42.25 42.10 42.11
tawSolver VanDerWaerden_2-3-12_135.cnf
37.34 37.26 37.39 37.26 37.39
tawSolver64 VanDerWaerden_2-3-12_135.cnf
44.92 44.70 44.78 44.71 44.80

# a speed-up; again, less for 64-bit.

# ID 42ebae49d4ae1c0b10dbaf338b29b204b0c4940d
# Removed the special machinery via (integral) bitsets for determining
# the unit-literal -- the direct computation is faster.
# Now there are no restrictions on clause-size anymore.
# A small speed-penalty is given by the now general handling of weights
# in the heuristics, but now arbitrary weights can be used, which enables
# optimising the heuristics.

> oklib timing
# cswsok (one other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
32.47 32.35 32.50 32.52 32.83
tawSolver VanDerWaerden_2-3-12_135.cnf
34.59 34.65 34.81 34.68 34.56


# ID d72626596917101a573c05cfb90cda9ab25f4214
# Now new basis-weight 3.0.

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c running_time(sec)                     28.43
c number_of_nodes                       1498256
c number_of_binary_nodes                749117
c number_of_1-reductions                18113899
c max_number_changes                    9959
c file_name                             VanDerWaerden_2-3-12_134.cnf
v 1 2 3 4 5 6 7 8 9 -10 11 12 13 14 15 16 17 18 -19 20 21 22 23 24 25 26 27 28 -29 30 31 -32 33 34 35 -36 37 -38 39 40 41 42 43 44 45 -46 47 48 -49 50 -51 52 53 54 -55 56 57 58 59 60 61 62 63 64 65 66 -67 -68 69 70 71 72 73 74 75 76 77 78 79 -80 81 82 83 -84 85 -86 87 88 -89 90 91 92 93 94 95 96 -97 98 -99 100 101 102 -103 104 105 -106 107 108 109 110 111 112 113 114 115 -116 117 118 119 120 121 122 123 124 -125 126 127 128 129 130 131 132 133 134 0

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c running_time(sec)                     33.21
c number_of_nodes                       1713023
c number_of_binary_nodes                856511
c number_of_1-reductions                20916887
c max_number_changes                    10052
c file_name                             VanDerWaerden_2-3-12_135.cnf

> oklib timing
# cswsok (one other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
28.11 28.22 28.16 28.08 28.43
tawSolver VanDerWaerden_2-3-12_135.cnf
32.94 32.80 33.22 32.76 33.21

# ID 6a22ecdde554f19b56cbf7faaaa40429cf3d650d
# Replaced clause-indices by clause-pointers.

> oklib timing
# cswsok (one other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
24.80 24.82 24.94 24.86 24.77
tawSolver VanDerWaerden_2-3-12_135.cnf
28.96 29.06 29.26 28.98 29.15

# a considerable speed-up

# ID c730dc830fb5df241fdc81697c389afcce6ba117
# Now with product as projection, and weight-basis 2.

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c running_time(sec)                     15.55
c number_of_nodes                       957164
c number_of_binary_nodes                478575
c number_of_1-reductions                10859220
c max_number_assignments                134
c max_number_changes                    9780
c file_name                             VanDerWaerden_2-3-12_134.cnf

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c running_time(sec)                     18.58
c number_of_nodes                       1122651
c number_of_binary_nodes                561325
c number_of_1-reductions                12787692
c max_number_assignments                130
c max_number_changes                    9900
c file_name                             VanDerWaerden_2-3-12_135.cnf

> oklib timing
# cswsok (one other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
15.55 15.54 15.53 15.56 15.64
tawSolver VanDerWaerden_2-3-12_135.cnf
18.58 18.58 18.45 18.60 18.52

# a big improvement.

# ID 215c1f65d3368dab47052b512de966741940ab94
# w_2=7.5, w_3=1, w_4=2^-1, w_5=2^-2, ...

c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c running_time(sec)                     14.11
c number_of_nodes                       857366
c number_of_binary_nodes                428676
c number_of_1-reductions                9983978
c max_number_assignments                134
c max_number_changes                    9815
c file_name                             VanDerWaerden_2-3-12_134.cnf

c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c running_time(sec)                     16.74
c number_of_nodes                       1003777
c number_of_binary_nodes                501888
c number_of_1-reductions                11774917
c max_number_assignments                132
c max_number_changes                    9884
c file_name                             VanDerWaerden_2-3-12_135.cnf

> oklib timing
# cswsok (one other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
14.11 14.03 14.00 14.01 14.07
tawSolver VanDerWaerden_2-3-12_135.cnf
16.74 16.73 16.76 16.72 16.81

# A further improvement; this should now be the final version (before deeper
## research into the weight-structure might reveal better weights).

# ID 7da84e6c2727eb51d61117aea7a7938092d5712c
# Simplified change-handling.
# No longer output of "max_number_assignments" and max_number_changes
# (wasn't very informative).

c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c running_time(sec)                     13.63
c number_of_nodes                       857366
c number_of_binary_nodes                428676
c number_of_1-reductions                9983978
c file_name                             VanDerWaerden_2-3-12_134.cnf

c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c running_time(sec)                     16.31
c number_of_nodes                       1003777
c number_of_binary_nodes                501888
c number_of_1-reductions                11774917
c file_name                             VanDerWaerden_2-3-12_135.cnf

> oklib timing
# cswsok (one other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
13.63 13.62 13.68 13.77 13.68
tawSolver VanDerWaerden_2-3-12_135.cnf
16.31 16.42 16.26 16.36 16.31

# small run-time improvement, and code simplified.

# ID 4d644cd9007a1351d8c263db7dfbb50df33db946
# Optimised weights.

> oklib timing
# cswsok (one other process running)

c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c running_time(sec)                     13.57
c number_of_nodes                       864314
c number_of_binary_nodes                432150
c number_of_1-reductions                10105278
c file_name                             VanDerWaerden_2-3-12_134.cnf

c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c running_time(sec)                     15.80
c number_of_nodes                       977415
c number_of_binary_nodes                488707
c number_of_1-reductions                11522820
c file_name                             VanDerWaerden_2-3-12_135.cnf

tawSolver VanDerWaerden_2-3-12_134.cnf
13.57 13.58 13.53 13.58 13.60
tawSolver VanDerWaerden_2-3-12_135.cnf
15.80 15.68 15.67 15.66 15.74

# reduced node count.

# ID 3a87d8d3cccb7056bb23597a2c6fcfb5eddc68ca
# Improved implementation.

> oklib timing
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
11.89 11.97 11.98 11.94 11.94
tawSolver VanDerWaerden_2-3-12_135.cnf
13.74 13.89 13.79 13.77 13.74

# An improvement of 10% (due to the faster computation of the heuristics).

# ID 0ae7feac814d528dcb24c31cf65a5f83931ddf07
# Improved implementation.

> oklib timing
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
11.78 11.72 11.77 11.89 11.87
tawSolver VanDerWaerden_2-3-12_135.cnf
13.62 13.56 13.63 13.61 13.61

# Small improvement due to improved data-locality.

# ID 5689384b9693b8dca1caf8469b19a7ae8d66700a
# Improved structures.

> oklib timing
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
11.76 11.74 11.67 11.76 11.62
tawSolver VanDerWaerden_2-3-12_135.cnf
13.58 13.50 13.68 13.62 13.58

# Tiny improvement (at least no impairment).

# ID b2f597d7f36c6c7cfadca8e4a9809ca94a4af029
# Version 1.9.10; introduced ADT's and improved their implementations

> oklib timing
# cswsok (one other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
11.87 11.89 11.84 11.80 11.84
tawSolver VanDerWaerden_2-3-12_135.cnf
13.63 13.72 13.69 13.64 13.65

# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
11.47 11.61 11.57 11.55 11.59
tawSolver VanDerWaerden_2-3-12_135.cnf
13.35 13.29 13.35 13.32 13.35

# A tiny improvement.

# ID d8cab850756bfc0d1a8e7c1d61012630aa79f3d3
# Version 1.10.0; changed unit-clause propagation (now BFS instead of DFS)

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     11.41
c number_of_nodes                       864314
c number_of_binary_nodes                432150
c number_of_1-reductions                9949777
c file_name                             VanDerWaerden_2-3-12_134.cnf

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     13.18
c number_of_nodes                       977415
c number_of_binary_nodes                488707
c number_of_1-reductions                11324097
c file_name                             VanDerWaerden_2-3-12_135.cnf

> oklib timing
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
11.41 11.51 11.43 11.42 11.46
tawSolver VanDerWaerden_2-3-12_135.cnf
13.18 13.16 13.18 13.21 13.27

# A tiny improvement.

# ID ce0dd7e1da0f70bfe0c8b560a74e0110a01e2099
# Version 1.10.1
# Processing first 0-assignments.

> oklib timing
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
9.73 9.51 9.57 9.53 9.54
tawSolver VanDerWaerden_2-3-12_135.cnf
11.12 11.10 11.12 11.12 11.13

# 15% faster.

# ID 6d82c68830eccc0a5f3490bfddbdc24ea5fa8ce7
# Version 1.10.2
# Now back to dfs as default for ucp.

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     9.52
c number_of_nodes                       864314
c number_of_binary_nodes                432150
c number_of_1-reductions                10105278
c file_name                             VanDerWaerden_2-3-12_134.cnf

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     10.98
c number_of_nodes                       977415
c number_of_binary_nodes                488707
c number_of_1-reductions                11522820
c file_name                             VanDerWaerden_2-3-12_135.cnf

> oklib timing
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
9.52 9.53 9.53 9.49 9.58
tawSolver VanDerWaerden_2-3-12_135.cnf
10.98 10.95 10.93 11.08 10.94

# Back to the slightly increased 1-reductions-count, but the improved
# implementation compensates for this.

# ID 0fe3cad13317e4ed66f7f2612b2dcc6e1641156a
# Version 2.1.0; should be basically the final version.

> oklib timing
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
9.65 9.65 9.61 9.54 9.56
tawSolver VanDerWaerden_2-3-12_135.cnf
11.05 11.08 11.26 11.16 11.12

# Roughly 1% slower. Where does this come from?

# ID 705c3829ba28332862e8e67f2aa92a62db7f08de
# Version 2.1.1

> oklib timing
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
9.44 9.48 9.44 9.49 9.44
tawSolver VanDerWaerden_2-3-12_135.cnf
10.88 10.91 10.91 10.85 10.97

# Strange, these small changes; they look consistent, but are hard to explain.

# ID 742c25c36779fedcd73e8a8b4dfb13f5559cccea
# Version 2.1.2
# ttawSolver:
#  - elimination of pure literals (those that are found -- not iterated)
#  - using tau as projection (5 iterations of Newton method).

# ttawSolver:

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     30.61
c number_of_nodes                       778208
c number_of_binary_nodes                389095
c number_of_1-reductions                9157137
c number_of_pure_literals               1229
c file_name                             VanDerWaerden_2-3-12_134.cnf

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     37.90
c number_of_nodes                       953179
c number_of_binary_nodes                476589
c number_of_1-reductions                11285589
c number_of_pure_literals               1317
c file_name                             VanDerWaerden_2-3-12_135.cnf

> oklib timing
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
9.42 9.46 9.46 9.52 9.48
tawSolver VanDerWaerden_2-3-12_135.cnf
10.85 10.85 10.82 10.90 10.83
ttawSolver VanDerWaerden_2-3-12_134.cnf
30.60 30.52 30.59 30.60 30.55
ttawSolver VanDerWaerden_2-3-12_135.cnf
37.90 38.12 38.05 38.13 38.12

# ID 8f849791377c512a2a34286892f7fa62e198d64d
# Version 2.1.2
# Revised weights: 5.4, 0.31, 0.13, 1.6

# tawSolver:

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     9.14
c number_of_nodes                       829473
c number_of_binary_nodes                414729
c number_of_1-reductions                9709393
c file_name                             VanDerWaerden_2-3-12_134.cnf

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     10.89
c number_of_nodes                       977215
c number_of_binary_nodes                488607
c number_of_1-reductions                11510241
c file_name                             VanDerWaerden_2-3-12_135.cnf

# ttawSolver:

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     36.26
c number_of_nodes                       916407
c number_of_binary_nodes                458196
c number_of_1-reductions                10821102
c number_of_pure_literals               1418
c file_name                             VanDerWaerden_2-3-12_134.cnf

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     37.40
c number_of_nodes                       928273
c number_of_binary_nodes                464136
c number_of_1-reductions                11010312
c number_of_pure_literals               1196
c file_name                             VanDerWaerden_2-3-12_135.cnf

> oklib timing
# cswsok (no other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
9.13 9.14 9.08 9.13 9.07
tawSolver VanDerWaerden_2-3-12_135.cnf
10.89 10.87 10.84 10.86 10.95
ttawSolver VanDerWaerden_2-3-12_134.cnf
36.26 36.56 36.43 36.35 36.41
ttawSolver VanDerWaerden_2-3-12_135.cnf
37.40 37.22 37.43 37.42 37.53

# Somewhat improved node-counts (ignoring satisfiable instances).

# ID cc8bbb4f92750e728ca1c1461034903a3cff1e40
# Improved implementation of tau-heuristics.

# ttawSolver:

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     19.07
c number_of_nodes                       916403
c number_of_binary_nodes                458194
c number_of_1-reductions                10821105
c number_of_pure_literals               1418
c file_name                             VanDerWaerden_2-3-12_134.cnf

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     19.52
c number_of_nodes                       928273
c number_of_binary_nodes                464136
c number_of_1-reductions                11010516
c number_of_pure_literals               1196
c file_name                             VanDerWaerden_2-3-12_135.cnf

> oklib timing
# cswsok (one other process running)

tawSolver VanDerWaerden_2-3-12_134.cnf
9.31 9.29 9.30 9.28 9.27
tawSolver VanDerWaerden_2-3-12_135.cnf
11.25 11.15 11.11 11.11 11.19
ttawSolver VanDerWaerden_2-3-12_134.cnf
19.07 19.04 18.99 18.98 19.04
ttawSolver VanDerWaerden_2-3-12_135.cnf
19.52 19.60 19.49 19.54 19.51

# Speed improved nearly by a factor of two for the tau-version (while
# node-counts are nearly identical).
   \endverbatim
   </li>
  </ul>


  \todo Using the tau-function
  <ul>
   <li> It is interesting to replace the product as projection by the
   tau-function. </li>
   <li> For the OKsolver this replacement did not reduce the node-count, but
   here it might be different? </li>
   <li> A simple implementation (returning 1/tau(a,b), so that maximisation can
   still be used):
   \verbatim
inline Weight_t tau(const Weight_t a, const Weight_t b) {
  if (a == 0 or b == 0) return 0;
  assert(a > 0); assert(b > 0);
  constexpr Weight_t eps = 1e-12;
  Weight_t x = std::pow(4,1/(a+b));
  while (true) {
    const Weight_t pa = std::pow(x,-a), pb = std::pow(x, -b);
    const Weight_t dx = x * (pa + pb - 1) / (a*pa + b*pb);
    if (dx <= eps) return 1 / x;
    x += dx;
  }
}
   \endverbatim
   resp.
   \verbatim
inline Weight_t tau(const Weight_t a, const Weight_t b) {
  if (a == 0 or b == 0) return 0;
  assert(a > 0); assert(b > 0);
  constexpr int iterations = 5;
  Weight_t x = std::pow(4,1/(a+b));
  for (int i = 0; i < iterations; ++i) {
    const Weight_t pa = std::pow(x,-a), pb = std::pow(x,-b);
    x += x * (pa + pb - 1) / (a*pa + b*pb);
  }
  return 1/x;
}
   \endverbatim
   </li>
   <li> On csltok (2.7 GHz) we get:
   \verbatim
# With termination-condition "dx < 10^-12":

> ./tawSolver VanDerWaerden_2-3-12_135.cnf
s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     57.14
c number_of_nodes                       953175
c number_of_binary_nodes                476587
c number_of_1-reductions                11286082
c file_name                             VanDerWaerden_2-3-12_135.cnf

> ./tawSolver VanDerWaerden_2-3-13_160.cnf
s UNSATISFIABLE
c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     414.03
c number_of_nodes                       5868909
c number_of_binary_nodes                2934454
c number_of_1-reductions                75157685
c file_name                             VanDerWaerden_2-3-13_160.cnf

> ./tawSolver VanDerWaerden_pd_2-3-21_405.cnf
s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     290.98
c number_of_nodes                       2178341
c number_of_binary_nodes                1089170
c number_of_1-reductions                32404324
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf

# With 5 iterations:

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     49.13
c number_of_nodes                       953175
c number_of_binary_nodes                476587
c number_of_1-reductions                11286082
c file_name                             VanDerWaerden_2-3-12_135.cnf

> ./tawSolver VanDerWaerden_2-3-13_160.cnf
0.618034
s UNSATISFIABLE
c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     354.67
c number_of_nodes                       5868893
c number_of_binary_nodes                2934446
c number_of_1-reductions                75158028
c file_name                             VanDerWaerden_2-3-13_160.cnf

> ./tawSolver VanDerWaerden_pd_2-3-21_405.cnf
s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     251.19
c number_of_nodes                       2229093
c number_of_binary_nodes                1114546
c number_of_1-reductions                33402234
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf
   \endverbatim
   </li>
   <li> 5 Iterations might be sufficient; interestingly, the more iterations
   the smaller the node-count (with diminishing return). </li>
   <li> A reduction in node-count (10% for ordinary, and 20% for the
   palindromic problem) is achieved, but, as expected, a big increase in time. </li>
   <li> One needed to re-run the optimisation of the weights. </li>
  </ul>


  \todo Handle signals : DONE
  <ul>
   <li> As the OKsolver, when interrupted, then the current statistics should
   be printed (before exit). </li>
   <li> And SIGUSR1 should result in just printing the statistics (while
   continuing the computation). </li>
  </ul>


  \todo Provide Doxygen documentation


  \todo Introduce macros : DONE
  <ul>
   <li> DONE (no longer needed)
   MAX_NUMBER_CLAUSES, MAX_NUMBER_VARIABLES. </li>
   <li> And GIT_ID. </li>
   <li> DONE (different design)
   And CLAUSE_CONTENT (default "unsigned int"). </li>
   <li> DONE (not needed)
   Then a static_assert is needed, that the type for CLAUSE_CONTENT
   is unsigned integral. </li>
  </ul>


  \todo DONE
  Check number of variables and clauses
  <ul>
   <li> Errors must happen if there are too many variables or clauses. </li>
  </ul>


  \todo Version information : DONE
  <ul>
   <li> DONE
   Like the OKsolver, when using command-line argument "-v", the solver
   should output version information and date of compilation. </li>
   <li> DONE
   Let's also handle "--version". </li>
   <li> Also the value of MAX_CLAUSE_LENGTH, LIT_TYPE and GIT_ID. </li>
   <li> DONE
   And the git-ID (this needs to be added for the OKsolver as well). </li>
   <li> There should be an official version of the tawSolver:
    <ol>
     <li> The one on the solver's homepage is 1.0.0. </li>
     <li> DONE (using 1.3 now)
     Let's call the current version 1.1 (improved handling of maximal
     clause-length, improved output, code improvements). </li>
    </ol>
   </li>
  </ul>


  \todo Improving the clause-bitsets
  <ul>
   <li> The following should be transferred to a the general library.
   Otherwise this todo is completed (from the solver these tools were
   removed after version 1.3.7). </li>
   <li> http://graphics.stanford.edu/~seander/bithacks.html
   contains various bit-level operations. </li>
   </li>
   <li> At
   http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious
   we find (slightly adapted):
   \verbatim
// IF YOU KNOW v IS A POWER OF 2:
unsigned int v;  // 32-bit value to find the log2 of
const unsigned int b[] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 0xFF00FF00, 0xFFFF0000};
unsigned int r = (v & b[0]) != 0;
for (i = 4; i > 0; --i) r |= ((v & b[i]) != 0) << i;

# The values b[0], ..., b[4]:
1010 1010 1010 1010 1010 1010 1010 1010
1100 1100 1100 1100 1100 1100 1100 1100
1111 0000 1111 0000 1111 0000 1111 0000
1111 1111 0000 0000 1111 1111 0000 0000
1111 1111 1111 1111 0000 0000 0000 0000
   \endverbatim
   </li>
   <li> The result computed is
     (bool(v&b[0]) << 0) | ... | (bool(v&b[4]) << 4)
   Note that the order doesn't matter. </li>
   <li> A proper C++ implementation of "log2s" ("s" for "special"):
   \verbatim
#incluce <limits>
#include <cassert>
#include <cmath>

constexpr int B {std::numeric_limits<unsigned int>::digits};
static_assert(B == 32, "Type \"unsigned int\" has not 32 bits.");
constexpr int N = 5;

constexpr int log2s(const unsigned int v) {
  assert((unsigned int) std::exp2(std::log2(v)) == v);
  constexpr unsigned int b[N] = {0xAAAAAAAA,0xCCCCCCCC,0xF0F0F0F0,0xFF00FF00,0xFFFF0000};
  unsigned int r = (v & b[0]) != 0;
  for (int i = 1; i < N; ++i) r |= ((v & b[i]) != 0) << i;
  return r;
}
   \endverbatim
   </li>
   <li> For B=64 (N=6) we have:
   \verbatim
const unsigned int b[N] = {0xAAAAAAAAAAAAAAAA, 0xCCCCCCCCCCCCCCCC, 0xF0F0F0F0F0F0F0F0, 0xFF00FF00FF00FF00, 0xFFFF00000FFFF0000, 0xFFFFFFFF00000000};
   \endverbatim
   </li>
   <li> A recursive computation of the array b of length N, via the values
   bp(N,0), ..., bp(N,N-1) ("bp" for bit pattern"), is:
   \verbatim
constexpr unsigned int pow2(const unsigned e) { return (e==0)?1:2*pow2(e-1); }
constexpr unsigned int pow22(const unsigned e) {return pow2(pow2(e));}

constexpr unsigned int bp(const unsigned N, const unsigned i) {
  // assert(N >= 0);
  // assert(i >= 0);
  // assert(i < N);
  return (i < N-1) ? bp(N-1,i) * (1 + pow22(N-1)) : pow22(N) - pow22(N-1);
}
   \endverbatim
   </li>
   <li> Then the general computation is:
   \verbatim
#include <limits>
#include <cassert>
#include <cmath>

constexpr int log2(const unsigned int n) { return (n <= 1) ? 0 : 1+log2(n/2); }

constexpr int B {std::numeric_limits<unsigned int>::digits};
constexpr unsigned int N = log2((unsigned int) B);
static_assert(pow2(N) == (unsigned int) B, "Number of bits in \"unsigned int\" not a power of 2.");

static_assert(N==5 or N==6, "Unexpected size of type \"unsigned int\".");
const unsigned int b[6] {bp(N,0),bp(N,1),bp(N,2),bp(N,3),bp(N,4), (N==6) ? bp(N,5) : 0};
// Unfortunately there is no reasonable way in C++ to just define b[N].

inline int log2s(const unsigned int v) {
  assert((unsigned int) std::exp2(std::log2(v)) == v);
  unsigned int r = (v & b[0]) != 0;
  for (unsigned int i = 1; i < N; ++i) r |= ((v & b[i]) != 0) << i;
  return r;
}
   \endverbatim
   </li>
   <li> As constexpr-functions:
   \verbatim
constexpr int log2sr(const unsigned v, const unsigned i) {
  // assert(i < N);
  return (i == 0) ? ((v & bp(N,0)) != 0)  : log2sr(v, i-1) | (((v & bp(N,i)) != 0) << i);
}
constexpr unsigned int log2sc(const unsigned int v) {
  return log2sr(v, N-1);
}
   \endverbatim
   (Remark: In OKs repository under "B149Bitposition.cpp" a more general,
   templatised version can be found.)
   </li>
  </ul>

*/
