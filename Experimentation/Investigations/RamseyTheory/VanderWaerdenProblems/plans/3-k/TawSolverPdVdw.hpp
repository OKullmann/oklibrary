// Oliver Kullmann, 28.7.2013 (Swansea)
/* Copyright 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/TawSolverPdVdw.hpp
  \brief On tawSolver computing palindromic vdW-numbers pdvdw_2(3,k)


  \todo Versions before version 2.0
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li> k=17, n=279:
   \verbatim
c running_time(sec)                     6.65
c number_of_nodes                       143319
c number_of_binary_nodes                71659
c number_of_1-reductions                1511306

# version b45bc81344b378d0130138af761e397f2b3a87c7
c running_time(sec)                     6.31
c max_number_changes                    19398

# ID 4d644cd9007a1351d8c263db7dfbb50df33db946
c number_of_variables                   140
c number_of_clauses                     10536
c maximal_clause_length                 17
c running_time(sec)                     1.04
c number_of_nodes                       31401
c number_of_binary_nodes                15700
c number_of_1-reductions                380917
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf
   \endverbatim
   </li>
   <li> k=18, n=312:
   \verbatim
c running_time(sec)                     59.59
c number_of_nodes                       1063979
c number_of_binary_nodes                531989
c number_of_1-reductions                12050046

# version b45bc81344b378d0130138af761e397f2b3a87c7
c running_time(sec)                     56.28
c max_number_changes                    25693

# ID 4d644cd9007a1351d8c263db7dfbb50df33db946
c number_of_variables                   156
c number_of_clauses                     13277
c maximal_clause_length                 18
c running_time(sec)                     12.13
c number_of_nodes                       320147
c number_of_binary_nodes                160073
c number_of_1-reductions                3883694
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf
   \endverbatim
   </li>
   <li> k=19, n=347:
   \verbatim
c running_time(sec)                     134.32
c number_of_nodes                       2009635
c number_of_binary_nodes                1004817
c number_of_1-reductions                23490743

# version b45bc81344b378d0130138af761e397f2b3a87c7
c running_time(sec)                     126.75
c max_number_changes                    30281

# ID 4d644cd9007a1351d8c263db7dfbb50df33db946
c number_of_variables                   174
c number_of_clauses                     16208
c maximal_clause_length                 19
c running_time(sec)                     16.70
c number_of_nodes                       349133
c number_of_binary_nodes                174566
c number_of_1-reductions                4687791
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf
   \endverbatim
   </li>
   <li> k=20, n=389:
   \verbatim
c running_time(sec)                     738.25
c number_of_nodes                       9076261
c number_of_binary_nodes                4538130
c number_of_1-reductions                110593054

# version b45bc81344b378d0130138af761e397f2b3a87c7
c running_time(sec)                     693.71
c max_number_changes                    38297

# ID 4d644cd9007a1351d8c263db7dfbb50df33db946
c number_of_variables                   195
c number_of_clauses                     20327
c maximal_clause_length                 20
c running_time(sec)                     61.96
c number_of_nodes                       1067245
c number_of_binary_nodes                533622
c number_of_1-reductions                15048643
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf
   \endverbatim
   </li>
   <li> k=21, n=405:
   \verbatim
c running_time(sec)                     2541.11
c number_of_nodes                       30470349
c number_of_binary_nodes                15235174
c number_of_1-reductions                374871138

# version b45bc81344b378d0130138af761e397f2b3a87c7
c running_time(sec)                     2374.85
c max_number_changes                    41676

# ID 4d644cd9007a1351d8c263db7dfbb50df33db946
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c running_time(sec)                     155.18
c number_of_nodes                       2656801
c number_of_binary_nodes                1328400
c number_of_1-reductions                37456772
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf
   \endverbatim
   </li>
   <li> k=22, n=463:
   \verbatim
c running_time(sec)                     18305.95
c number_of_nodes                       170414771
c number_of_binary_nodes                85207385
c number_of_1-reductions                2236999086

# version dbfe40729515a5643f18e5953abe91d704626edf
c number_of_variables                   232
c number_of_clauses                     28650
c running_time(sec)                     20753.85
c number_of_nodes                       170414771
c number_of_binary_nodes                85207385
c number_of_1-reductions                2236999086
c max_number_changes                    54879

# version b45bc81344b378d0130138af761e397f2b3a87c7
c running_time(sec)                     16633.30

# ID 4d644cd9007a1351d8c263db7dfbb50df33db946
c number_of_variables                   232
c number_of_clauses                     28650
c maximal_clause_length                 22
c running_time(sec)                     1014.33
c number_of_nodes                       13431053
c number_of_binary_nodes                6715526
c number_of_1-reductions                205341960
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf
   \endverbatim
   </li>
   <li> k=23, n=507:
   \verbatim
# version dbfe40729515a5643f18e5953abe91d704626edf
c number_of_variables                   254
c number_of_clauses                     34289
c running_time(sec)                     86869.06
c number_of_nodes                       573190251
c number_of_binary_nodes                286595125
c number_of_1-reductions                7857883219
c max_number_changes                    65646

# version b45bc81344b378d0130138af761e397f2b3a87c7
c running_time(sec)                     65916.72

# ID 4d644cd9007a1351d8c263db7dfbb50df33db946
c number_of_variables                   254
c number_of_clauses                     34289
c maximal_clause_length                 23
c running_time(sec)                     2320.95
c number_of_nodes                       26126051
c number_of_binary_nodes                13063025
c number_of_1-reductions                424416424
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf
   \endverbatim
   </li>
   <li> k=24, n=593:
   \verbatim
# ID 4d644cd9007a1351d8c263db7dfbb50df33db946
c number_of_variables                   297
c number_of_clauses                     46881
c maximal_clause_length                 24
c running_time(sec)                     54376.47
c number_of_nodes                       445735819
c number_of_binary_nodes                222867909
c number_of_1-reductions                7790489169
c file_name                             VanDerWaerden_pd_2-3-24_593.cnf
   \endverbatim
   </li>
  </ul>


  \todo Towards version 2.0
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li>
   \verbatim
# ID 5689384b9693b8dca1caf8469b19a7ae8d66700a
# Version 1.9.7

c number_of_variables                   140
c number_of_clauses                     10536
c maximal_clause_length                 17
c number_of_literal_occurrences         45139
c running_time(sec)                     0.89
c number_of_nodes                       31401
c number_of_binary_nodes                15700
c number_of_1-reductions                380917
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf

c number_of_variables                   156
c number_of_clauses                     13277
c maximal_clause_length                 18
c number_of_literal_occurrences         58763
c running_time(sec)                     10.30
c number_of_nodes                       320147
c number_of_binary_nodes                160073
c number_of_1-reductions                3883694
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf

c number_of_variables                   174
c number_of_clauses                     16208
c maximal_clause_length                 19
c number_of_literal_occurrences         70414
c running_time(sec)                     14.15
c number_of_nodes                       349133
c number_of_binary_nodes                174566
c number_of_1-reductions                4687791
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf

c number_of_variables                   195
c number_of_clauses                     20327
c maximal_clause_length                 20
c number_of_literal_occurrences         88944
c running_time(sec)                     52.38
c number_of_nodes                       1067245
c number_of_binary_nodes                533622
c number_of_1-reductions                15048643
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf

c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     130.36
c number_of_nodes                       2656801
c number_of_binary_nodes                1328400
c number_of_1-reductions                37456772
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf

c number_of_variables                   232
c number_of_clauses                     28650
c maximal_clause_length                 22
c number_of_literal_occurrences         126560
c running_time(sec)                     861.77
c number_of_nodes                       13431053
c number_of_binary_nodes                6715526
c number_of_1-reductions                205341960
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf

c number_of_variables                   254
c number_of_clauses                     34289
c maximal_clause_length                 23
c number_of_literal_occurrences         152236
c running_time(sec)                     1963.18
c number_of_nodes                       26126051
c number_of_binary_nodes                13063025
c number_of_1-reductions                424416424
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf

# ID 55e2b7c270a9b63ee3029d3a597a43069ff6f8b5
# Version 1.9.10

c running_time(sec)                     0.88
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf

c running_time(sec)                     10.15
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf

c running_time(sec)                     14.13
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf

c running_time(sec)                     51.85
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf

c running_time(sec)                     129.93
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf

c running_time(sec)                     856.35
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf

c running_time(sec)                     1955.02
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf

# ID d8cab850756bfc0d1a8e7c1d61012630aa79f3d3
# Version 1.10.0 (different 1-reductions-count)
# cswsok with one other process running.

c running_time(sec)                     0.93
c number_of_nodes                       31401
c number_of_binary_nodes                15700
c number_of_1-reductions                418064
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf

c running_time(sec)                     10.80
c number_of_nodes                       320147
c number_of_binary_nodes                160073
c number_of_1-reductions                4329656
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf

c running_time(sec)                     14.95
c number_of_nodes                       349133
c number_of_binary_nodes                174566
c number_of_1-reductions                5251353
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf

c running_time(sec)                     55.29
c number_of_nodes                       1067245
c number_of_binary_nodes                533622
c number_of_1-reductions                16851106
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf

c running_time(sec)                     137.56
c number_of_nodes                       2656801
c number_of_binary_nodes                1328400
c number_of_1-reductions                41808320
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf

c running_time(sec)                     909.39
c number_of_nodes                       13431053
c number_of_binary_nodes                6715526
c number_of_1-reductions                232055443
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf

c running_time(sec)                     2098.11
c number_of_nodes                       26126051
c number_of_binary_nodes                13063025
c number_of_1-reductions                478879784
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf

# Interesting that the FIFO-order for processing unit-clauses leads to 10%
# increase in the number of processed unit-assignments.

# ID ce0dd7e1da0f70bfe0c8b560a74e0110a01e2099
# Version 1.10.1

c running_time(sec)                     0.77
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf

c running_time(sec)                     9.08
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf

c running_time(sec)                     12.56
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf

c running_time(sec)                     46.62
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf

c running_time(sec)                     116.57
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf

c running_time(sec)                     777.03
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf

c running_time(sec)                     1777.15
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf
   \endverbatim
   </li>
  </ul>


  \todo Version 2.0 and later
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li>
   \verbatim
# Version 2:
# cswsok, with one other process running

tawSolver:
 author: Tanbir Ahmed
 url: http://sourceforge.net/projects/tawsolver/
 Changes by Oliver Kullmann
 Version: 1.10.2
 Last change date: 14.7.2013
 Clause-weight parameters: 7, 1.7
  Mapping k -> weight for weights specified at compile-time:
   2->7  3->1  4->0.31  5->0.19
 Macro settings:
  LIT_TYPE = std::int32_t (with 31 binary digits)
 Compiled with NDEBUG
 Compiled with optimisation options
 Compilation date: Jul 14 2013 12:53:10
 Compiler: g++, version 4.7.3
 Provided in the OKlibrary http://www.ok-sat-library.org
 Git ID = 6d82c68830eccc0a5f3490bfddbdc24ea5fa8ce7
s UNSATISFIABLE
c number_of_variables                   140
c number_of_clauses                     10536
c maximal_clause_length                 17
c number_of_literal_occurrences         45139
c running_time(sec)                     0.74
c number_of_nodes                       31401
c number_of_binary_nodes                15700
c number_of_1-reductions                380917
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf
s UNSATISFIABLE
c number_of_variables                   156
c number_of_clauses                     13277
c maximal_clause_length                 18
c number_of_literal_occurrences         58763
c running_time(sec)                     8.68
c number_of_nodes                       320147
c number_of_binary_nodes                160073
c number_of_1-reductions                3883694
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf
s UNSATISFIABLE
c number_of_variables                   174
c number_of_clauses                     16208
c maximal_clause_length                 19
c number_of_literal_occurrences         70414
c running_time(sec)                     12.05
c number_of_nodes                       349133
c number_of_binary_nodes                174566
c number_of_1-reductions                4687791
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf
s UNSATISFIABLE
c number_of_variables                   195
c number_of_clauses                     20327
c maximal_clause_length                 20
c number_of_literal_occurrences         88944
c running_time(sec)                     44.77
c number_of_nodes                       1067245
c number_of_binary_nodes                533622
c number_of_1-reductions                15048643
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf
s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     112.47
c number_of_nodes                       2656801
c number_of_binary_nodes                1328400
c number_of_1-reductions                37456772
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf
s UNSATISFIABLE
c number_of_variables                   232
c number_of_clauses                     28650
c maximal_clause_length                 22
c number_of_literal_occurrences         126560
c running_time(sec)                     748.81
c number_of_nodes                       13431053
c number_of_binary_nodes                6715526
c number_of_1-reductions                205341960
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf
s UNSATISFIABLE
c number_of_variables                   254
c number_of_clauses                     34289
c maximal_clause_length                 23
c number_of_literal_occurrences         152236
c running_time(sec)                     1700.44
c number_of_nodes                       26126051
c number_of_binary_nodes                13063025
c number_of_1-reductions                424416424
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf

s UNSATISFIABLE
c number_of_variables                   297
c number_of_clauses                     46881
c maximal_clause_length                 24
c number_of_literal_occurrences         209792
c running_time(sec)                     41908.22
c number_of_nodes                       445735819
c number_of_binary_nodes                222867909
c number_of_1-reductions                7790489169
c file_name                             VanDerWaerden_pd_2-3-24_593.cnf


# Version 2.2:
# cswsok, with one other process running

tawSolver:
 author: Tanbir Ahmed and Oliver Kullmann
 url's:
  http://sourceforge.net/projects/tawsolver/
  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/TawSolver/tawSolver.cpp
 Version: 2.2.2
 Last change date: 29.7.2013
 Clause-weight parameters: 5.4, 1.6
  Mapping k -> weight for weights specified at compile-time:
   2->5.4  3->1  4->0.31  5->0.13
 Macro settings:
  LIT_TYPE = std::int32_t (with 31 binary digits)
  UCP_STRATEGY = 1
 Compiled without TAU_ITERATION
 Compiled without PURE_LITERALS
 Compiled with NDEBUG
 Compiled with optimisation options
 Compilation date: Jul 29 2013 12:07:31
 Compiler: g++, version 4.7.3
 Provided in the OKlibrary http://www.ok-sat-library.org
 Git ID = 8f849791377c512a2a34286892f7fa62e198d64d

s UNSATISFIABLE
c number_of_variables                   140
c number_of_clauses                     10536
c maximal_clause_length                 17
c number_of_literal_occurrences         45139
c running_time(sec)                     0.74
c number_of_nodes                       31329
c number_of_binary_nodes                15664
c number_of_1-reductions                379292
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf

s UNSATISFIABLE
c number_of_variables                   156
c number_of_clauses                     13277
c maximal_clause_length                 18
c number_of_literal_occurrences         58763
c running_time(sec)                     7.98
c number_of_nodes                       296105
c number_of_binary_nodes                148052
c number_of_1-reductions                3631408
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf

s UNSATISFIABLE
c number_of_variables                   174
c number_of_clauses                     16208
c maximal_clause_length                 19
c number_of_literal_occurrences         70414
c running_time(sec)                     11.59
c number_of_nodes                       337825
c number_of_binary_nodes                168912
c number_of_1-reductions                4537587
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf

s UNSATISFIABLE
c number_of_variables                   195
c number_of_clauses                     20327
c maximal_clause_length                 20
c number_of_literal_occurrences         88944
c running_time(sec)                     43.48
c number_of_nodes                       1043529
c number_of_binary_nodes                521764
c number_of_1-reductions                14726262
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf

s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     93.39
c number_of_nodes                       2230547
c number_of_binary_nodes                1115273
c number_of_1-reductions                31580855
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf

s UNSATISFIABLE
c number_of_variables                   232
c number_of_clauses                     28650
c maximal_clause_length                 22
c number_of_literal_occurrences         126560
c running_time(sec)                     498.42
c number_of_nodes                       9177993
c number_of_binary_nodes                4588996
c number_of_1-reductions                141153524
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf

s UNSATISFIABLE
c number_of_variables                   254
c number_of_clauses                     34289
c maximal_clause_length                 23
c number_of_literal_occurrences         152236
c running_time(sec)                     1667.48
c number_of_nodes                       25504041
c number_of_binary_nodes                12752020
c number_of_1-reductions                415969229
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf

s UNSATISFIABLE
c number_of_variables                   297
c number_of_clauses                     46881
c maximal_clause_length                 24
c number_of_literal_occurrences         209792
c running_time(sec)                     32542.92
c number_of_nodes                       344472093
c number_of_binary_nodes                172236046
c number_of_1-reductions                6011310082
c file_name                             VanDerWaerden_pd_2-3-24_593.cnf

# For k=24 a big improvement of around 25%, based on a reduced node-count
# (due to the improved weights).

# With new weights:
tawSolver:
 authors: Tanbir Ahmed and Oliver Kullmann
 url's:
  http://sourceforge.net/projects/tawsolver/
  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/TawSolver/tawSolver.cpp
 Version: 2.4.0
 Last change date: 2.8.2013
 Mapping k -> weight, for clause-lengths k specified at compile-time:
   2->4.85  3->1  4->0.354  5->0.11  6->0.0694
 Divisor for open weights: 1.46
 Macro settings:
  LIT_TYPE = std::int32_t (with 31 binary digits)
  UCP_STRATEGY = 1
 Compiled without TAU_ITERATION
 Compiled without PURE_LITERALS
 Compiled with NDEBUG
 Compiled with optimisation options
 Compilation date: Aug  2 2013 01:05:21
 Compiler: g++, version 4.7.3
 Provided in the OKlibrary http://www.ok-sat-library.org
 Git ID = 301618c09a0a1add42a0e4f54a038f2dd457be66
s UNSATISFIABLE
c number_of_variables                   140
c number_of_clauses                     10536
c maximal_clause_length                 17
c number_of_literal_occurrences         45139
c running_time(sec)                     0.80
c number_of_nodes                       32697
c number_of_binary_nodes                16348
c number_of_1-reductions                395461
c reading-and-set-up_time(sec)          0.004
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf
s UNSATISFIABLE
c number_of_variables                   156
c number_of_clauses                     13277
c maximal_clause_length                 18
c number_of_literal_occurrences         58763
c running_time(sec)                     7.89
c number_of_nodes                       279309
c number_of_binary_nodes                139654
c number_of_1-reductions                3483271
c reading-and-set-up_time(sec)          0.007
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf
s UNSATISFIABLE
c number_of_variables                   174
c number_of_clauses                     16208
c maximal_clause_length                 19
c number_of_literal_occurrences         70414
c running_time(sec)                     9.98
c number_of_nodes                       285037
c number_of_binary_nodes                142518
c number_of_1-reductions                3862578
c reading-and-set-up_time(sec)          0.006
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf
s UNSATISFIABLE
c number_of_variables                   195
c number_of_clauses                     20327
c maximal_clause_length                 20
c number_of_literal_occurrences         88944
c running_time(sec)                     39.22
c number_of_nodes                       897529
c number_of_binary_nodes                448764
c number_of_1-reductions                12863905
c reading-and-set-up_time(sec)          0.009
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf
s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     101.82
c number_of_nodes                       2239371
c number_of_binary_nodes                1119685
c number_of_1-reductions                32017061
c reading-and-set-up_time(sec)          0.009
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf
s UNSATISFIABLE
c number_of_variables                   232
c number_of_clauses                     28650
c maximal_clause_length                 22
c number_of_literal_occurrences         126560
c running_time(sec)                     524.61
c number_of_nodes                       8683035
c number_of_binary_nodes                4341517
c number_of_1-reductions                135103699
c reading-and-set-up_time(sec)          0.014
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf
s UNSATISFIABLE
c number_of_variables                   254
c number_of_clauses                     34289
c maximal_clause_length                 23
c number_of_literal_occurrences         152236
c running_time(sec)                     1695.63
c number_of_nodes                       21565129
c number_of_binary_nodes                10782564
c number_of_1-reductions                357204133
c reading-and-set-up_time(sec)          0.018
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf
s UNSATISFIABLE
c number_of_variables                   297
c number_of_clauses                     46881
c maximal_clause_length                 24
c number_of_literal_occurrences         209792
c running_time(sec)                     26724.61
c number_of_nodes                       198685857
c number_of_binary_nodes                99342928
c number_of_1-reductions                3496177817
c reading-and-set-up_time(sec)          0.021
c file_name                             VanDerWaerden_pd_2-3-24_593.cnf

# Quite an improvement regarding the node-count!
   \endverbatim
   </li>
  </ul>


  \todo With tau-projection
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li>
   \verbatim
tawSolver:
 author: Tanbir Ahmed and Oliver Kullmann
 url's:
  http://sourceforge.net/projects/tawsolver/
  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/TawSolver/tawSolver.cpp
 Version: 2.1.2
 Last change date: 28.7.2013
 Clause-weight parameters: 7, 1.7
  Mapping k -> weight for weights specified at compile-time:
   2->7  3->1  4->0.31  5->0.19
 Macro settings:
  LIT_TYPE = std::int32_t (with 31 binary digits)
  UCP_STRATEGY = 1
  TAU_ITERATION = 5
 Compiled with PURE_LITERALS
 Compiled with NDEBUG
 Compiled with optimisation options
 Compilation date: Jul 28 2013 15:17:12
 Compiler: g++, version 4.7.3
 Provided in the OKlibrary http://www.ok-sat-library.org
 Git ID = 742c25c36779fedcd73e8a8b4dfb13f5559cccea

s UNSATISFIABLE
c number_of_variables                   140
c number_of_clauses                     10536
c maximal_clause_length                 17
c number_of_literal_occurrences         45139
c running_time(sec)                     1.86
c number_of_nodes                       32739
c number_of_binary_nodes                16369
c number_of_1-reductions                405689
c number_of_pure_literals               2
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf

s UNSATISFIABLE
c number_of_variables                   156
c number_of_clauses                     13277
c maximal_clause_length                 18
c number_of_literal_occurrences         58763
c running_time(sec)                     17.12
c number_of_nodes                       274157
c number_of_binary_nodes                137078
c number_of_1-reductions                3510308
c number_of_pure_literals               8
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf

s UNSATISFIABLE
c number_of_variables                   174
c number_of_clauses                     16208
c maximal_clause_length                 19
c number_of_literal_occurrences         70414
c running_time(sec)                     20.81
c number_of_nodes                       282747
c number_of_binary_nodes                141373
c number_of_1-reductions                3957446
c number_of_pure_literals               8
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf

s UNSATISFIABLE
c number_of_variables                   195
c number_of_clauses                     20327
c maximal_clause_length                 20
c number_of_literal_occurrences         88944
c running_time(sec)                     76.46
c number_of_nodes                       904693
c number_of_binary_nodes                452346
c number_of_1-reductions                13524266
c number_of_pure_literals               20
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf

s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     186.92
c number_of_nodes                       2229093
c number_of_binary_nodes                1114546
c number_of_1-reductions                33402131
c number_of_pure_literals               133
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf

s UNSATISFIABLE
c number_of_variables                   232
c number_of_clauses                     28650
c maximal_clause_length                 22
c number_of_literal_occurrences         126560
c running_time(sec)                     870.22
c number_of_nodes                       8456681
c number_of_binary_nodes                4228340
c number_of_1-reductions                139132341
c number_of_pure_literals               217
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf
s UNSATISFIABLE
c number_of_variables                   254
c number_of_clauses                     34289
c maximal_clause_length                 23
c number_of_literal_occurrences         152236
c running_time(sec)                     2300.44
c number_of_nodes                       19645719
c number_of_binary_nodes                9822859
c number_of_1-reductions                344619878
c number_of_pure_literals               472
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf

s UNSATISFIABLE
c number_of_variables                   297
c number_of_clauses                     46881
c maximal_clause_length                 24
c number_of_literal_occurrences         209792
c running_time(sec)                     11569.80
c number_of_nodes                       73835279
c number_of_binary_nodes                36917639
c number_of_1-reductions                1447968422
c number_of_pure_literals               159
c file_name                             VanDerWaerden_pd_2-3-24_593.cnf
   \endverbatim
   </li>
   <li> For k=24 a big improvement also in running time! </li>
   <li> k=25:
   \verbatim
> ttawSolver VanDerWaerden_pd_2-3-25_607.cnf
s UNSATISFIABLE
c number_of_variables                   304
c number_of_clauses                     48979
c maximal_clause_length                 25
c number_of_literal_occurrences         219525
c running_time(sec)                     30172.05
c number_of_nodes                       198554071
c number_of_binary_nodes                99277035
c number_of_1-reductions                3891461513
c number_of_pure_literals               2030
c file_name                             VanDerWaerden_pd_2-3-25_607.cnf
   \endverbatim
   </li>
   <li> With new weights, and faster computation of tau-comparisons:
   \verbatim
tawSolver:
 authors: Tanbir Ahmed and Oliver Kullmann
 url's:
  http://sourceforge.net/projects/tawsolver/
  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/TawSolver/tawSolver.cpp
 Version: 2.3.1
 Last change date: 29.7.2013
 Mapping k -> weight, for clause-lengths k specified at compile-time:
   2->5.4  3->1  4->0.31  5->0.13
 Divisor for open weights: 1.6
 Macro settings:
  LIT_TYPE = std::int32_t (with 31 binary digits)
  UCP_STRATEGY = 1
  TAU_ITERATION = 5
 Compiled with PURE_LITERALS
 Compiled with NDEBUG
 Compiled with optimisation options
 Compilation date: Jul 29 2013 22:52:12
 Compiler: g++, version 4.7.3
 Provided in the OKlibrary http://www.ok-sat-library.org
 Git ID = cc8bbb4f92750e728ca1c1461034903a3cff1e40

s UNSATISFIABLE
c number_of_variables                   140
c number_of_clauses                     10536
c maximal_clause_length                 17
c number_of_literal_occurrences         45139
c running_time(sec)                     1.14
c number_of_nodes                       32385
c number_of_binary_nodes                16192
c number_of_1-reductions                403560
c number_of_pure_literals               2
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf

s UNSATISFIABLE
c number_of_variables                   156
c number_of_clauses                     13277
c maximal_clause_length                 18
c number_of_literal_occurrences         58763
c running_time(sec)                     10.47
c number_of_nodes                       262071
c number_of_binary_nodes                131035
c number_of_1-reductions                3381577
c number_of_pure_literals               16
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf

s UNSATISFIABLE
c number_of_variables                   174
c number_of_clauses                     16208
c maximal_clause_length                 19
c number_of_literal_occurrences         70414
c running_time(sec)                     13.08
c number_of_nodes                       279745
c number_of_binary_nodes                139872
c number_of_1-reductions                3894783
c number_of_pure_literals               8
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf

s UNSATISFIABLE
c number_of_variables                   195
c number_of_clauses                     20327
c maximal_clause_length                 20
c number_of_literal_occurrences         88944
c running_time(sec)                     49.53
c number_of_nodes                       884629
c number_of_binary_nodes                442314
c number_of_1-reductions                13219781
c number_of_pure_literals               24
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf

s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     118.44
c number_of_nodes                       2119991
c number_of_binary_nodes                1059995
c number_of_1-reductions                31830683
c number_of_pure_literals               132
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf

s UNSATISFIABLE
c number_of_variables                   232
c number_of_clauses                     28650
c maximal_clause_length                 22
c number_of_literal_occurrences         126560
c running_time(sec)                     583.24
c number_of_nodes                       8224025
c number_of_binary_nodes                4112012
c number_of_1-reductions                134481839
c number_of_pure_literals               278
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf

s UNSATISFIABLE
c number_of_variables                   254
c number_of_clauses                     34289
c maximal_clause_length                 23
c number_of_literal_occurrences         152236
c running_time(sec)                     1587.37
c number_of_nodes                       19443879
c number_of_binary_nodes                9721939
c number_of_1-reductions                336912499
c number_of_pure_literals               608
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf

s UNSATISFIABLE
c number_of_variables                   297
c number_of_clauses                     46881
c maximal_clause_length                 24
c number_of_literal_occurrences         209792
c running_time(sec)                     8589.03
c number_of_nodes                       76795329
c number_of_binary_nodes                38397664
c number_of_1-reductions                1460338150
c number_of_pure_literals               259
c file_name                             VanDerWaerden_pd_2-3-24_593.cnf

s UNSATISFIABLE
c number_of_variables                   304
c number_of_clauses                     48979
c maximal_clause_length                 25
c number_of_literal_occurrences         219525
c running_time(sec)                     24875.90
c number_of_nodes                       224907723
c number_of_binary_nodes                112453861
c number_of_1-reductions                4231237387
c number_of_pure_literals               3481
c file_name                             VanDerWaerden_pd_2-3-25_607.cnf
   \endverbatim
   A nice speed-up, and for k <= 23 also small decreases in node-counts,
   however for k = 24,25 an increase, for k=24 around 5%, for k=25 around
   10%, so these might not just be random glitches. A thorough study of
   the weight-setting is needed.
   </li>
   <li> New weights:
   \verbatim
tawSolver:
 authors: Tanbir Ahmed and Oliver Kullmann
 url's:
  http://sourceforge.net/projects/tawsolver/
  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/TawSolver/tawSolver.cpp
 Version: 2.4.0
 Last change date: 2.8.2013
 Mapping k -> weight, for clause-lengths k specified at compile-time:
   2->5  3->1  4->0.295  5->0.122  6->0.0756
 Divisor for open weights: 1.6
 Macro settings:
  LIT_TYPE = std::int32_t (with 31 binary digits)
  UCP_STRATEGY = 1
  TAU_ITERATION = 5
 Compiled with PURE_LITERALS
 Compiled with NDEBUG
 Compiled with optimisation options
 Compilation date: Aug  2 2013 01:05:22
 Compiler: g++, version 4.7.3
 Provided in the OKlibrary http://www.ok-sat-library.org
 Git ID = 301618c09a0a1add42a0e4f54a038f2dd457be66
s UNSATISFIABLE
c number_of_variables                   140
c number_of_clauses                     10536
c maximal_clause_length                 17
c number_of_literal_occurrences         45139
c running_time(sec)                     1.22
c number_of_nodes                       32441
c number_of_binary_nodes                16220
c number_of_1-reductions                405000
c number_of_pure_literals               2
c reading-and-set-up_time(sec)          0.004
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf
s UNSATISFIABLE
c number_of_variables                   156
c number_of_clauses                     13277
c maximal_clause_length                 18
c number_of_literal_occurrences         58763
c running_time(sec)                     11.13
c number_of_nodes                       263245
c number_of_binary_nodes                131622
c number_of_1-reductions                3396978
c number_of_pure_literals               11
c reading-and-set-up_time(sec)          0.007
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf
s UNSATISFIABLE
c number_of_variables                   174
c number_of_clauses                     16208
c maximal_clause_length                 19
c number_of_literal_occurrences         70414
c running_time(sec)                     14.40
c number_of_nodes                       280119
c number_of_binary_nodes                140059
c number_of_1-reductions                3920224
c number_of_pure_literals               8
c reading-and-set-up_time(sec)          0.009
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf
s UNSATISFIABLE
c number_of_variables                   195
c number_of_clauses                     20327
c maximal_clause_length                 20
c number_of_literal_occurrences         88944
c running_time(sec)                     53.43
c number_of_nodes                       879109
c number_of_binary_nodes                439554
c number_of_1-reductions                13182704
c number_of_pure_literals               21
c reading-and-set-up_time(sec)          0.009
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf
s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     136.52
c number_of_nodes                       2124123
c number_of_binary_nodes                1062061
c number_of_1-reductions                31939538
c number_of_pure_literals               127
c reading-and-set-up_time(sec)          0.009
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf
s UNSATISFIABLE
c number_of_variables                   232
c number_of_clauses                     28650
c maximal_clause_length                 22
c number_of_literal_occurrences         126560
c running_time(sec)                     667.54
c number_of_nodes                       8354653
c number_of_binary_nodes                4177326
c number_of_1-reductions                136523860
c number_of_pure_literals               259
c reading-and-set-up_time(sec)          0.015
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf
s UNSATISFIABLE
c number_of_variables                   254
c number_of_clauses                     34289
c maximal_clause_length                 23
c number_of_literal_occurrences         152236
c running_time(sec)                     2157.06
c number_of_nodes                       20316697
c number_of_binary_nodes                10158348
c number_of_1-reductions                350733951
c number_of_pure_literals               568
c reading-and-set-up_time(sec)          0.016
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf
s UNSATISFIABLE
c number_of_variables                   297
c number_of_clauses                     46881
c maximal_clause_length                 24
c number_of_literal_occurrences         209792
c running_time(sec)                     12882.21
c number_of_nodes                       80405305
c number_of_binary_nodes                40202652
c number_of_1-reductions                1527856299
c number_of_pure_literals               313
c reading-and-set-up_time(sec)          0.022
c file_name                             VanDerWaerden_pd_2-3-24_593.cnf
s UNSATISFIABLE
c number_of_variables                   304
c number_of_clauses                     48979
c maximal_clause_length                 25
c number_of_literal_occurrences         219525
c running_time(sec)                     31399.84
c number_of_nodes                       231371313
c number_of_binary_nodes                115685656
c number_of_1-reductions                4353059262
c number_of_pure_literals               4291
c reading-and-set-up_time(sec)          0.023
c file_name                             VanDerWaerden_pd_2-3-25_607.cnf
   \endverbatim
   An increase in node-counts by around 5%; the large increase in time seems
   to be due to the high processor-load. </li>
   <li> New weights:
   \verbatim
tawSolver:
 authors: Tanbir Ahmed and Oliver Kullmann
 url's:
  http://sourceforge.net/projects/tawsolver/
  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/TawSolver/tawSolver.cpp
 Version: 2.4.4
 Last change date: 3.8.2013
 Mapping k -> weight, for clause-lengths k specified at compile-time:
   2->5.5  3->1  4->0.31  5->0.122  6->0.0455
 Divisor for open weights: 1.6
 Macro settings:
  LIT_TYPE = std::int32_t (with 31 binary digits)
  UCP_STRATEGY = 1
  TAU_ITERATION = 5
 Compiled with PURE_LITERALS
 Compiled with NDEBUG
 Compiled with optimisation options
 Compilation date: Aug  3 2013 19:02:52
 Compiler: g++, version 4.7.3
 Provided in the OKlibrary http://www.ok-sat-library.org
 Git ID = cf10158af74f7190c8a5cf4ed835e23c8203e499
s UNSATISFIABLE
c number_of_variables                   140
c number_of_clauses                     10536
c maximal_clause_length                 17
c number_of_literal_occurrences         45139
c running_time(sec)                     1.12
c number_of_nodes                       32361
c number_of_binary_nodes                16180
c number_of_1-reductions                401718
c number_of_pure_literals               3
c reading-and-set-up_time(sec)          0.003
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf
s UNSATISFIABLE
c number_of_variables                   156
c number_of_clauses                     13277
c maximal_clause_length                 18
c number_of_literal_occurrences         58763
c running_time(sec)                     10.35
c number_of_nodes                       264355
c number_of_binary_nodes                132177
c number_of_1-reductions                3381405
c number_of_pure_literals               14
c reading-and-set-up_time(sec)          0.006
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf
s UNSATISFIABLE
c number_of_variables                   174
c number_of_clauses                     16208
c maximal_clause_length                 19
c number_of_literal_occurrences         70414
c running_time(sec)                     12.48
c number_of_nodes                       273275
c number_of_binary_nodes                136637
c number_of_1-reductions                3782937
c number_of_pure_literals               9
c reading-and-set-up_time(sec)          0.007
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf
s UNSATISFIABLE
c number_of_variables                   195
c number_of_clauses                     20327
c maximal_clause_length                 20
c number_of_literal_occurrences         88944
c running_time(sec)                     47.04
c number_of_nodes                       864261
c number_of_binary_nodes                432130
c number_of_1-reductions                12783251
c number_of_pure_literals               21
c reading-and-set-up_time(sec)          0.009
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf
s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     114.12
c number_of_nodes                       2093661
c number_of_binary_nodes                1046830
c number_of_1-reductions                30867447
c number_of_pure_literals               154
c reading-and-set-up_time(sec)          0.008
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf
s UNSATISFIABLE
c number_of_variables                   232
c number_of_clauses                     28650
c maximal_clause_length                 22
c number_of_literal_occurrences         126560
c running_time(sec)                     577.18
c number_of_nodes                       8391515
c number_of_binary_nodes                4195757
c number_of_1-reductions                135216895
c number_of_pure_literals               322
c reading-and-set-up_time(sec)          0.013
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf
s UNSATISFIABLE
c number_of_variables                   254
c number_of_clauses                     34289
c maximal_clause_length                 23
c number_of_literal_occurrences         152236
c running_time(sec)                     1783.59
c number_of_nodes                       22414131
c number_of_binary_nodes                11207065
c number_of_1-reductions                373963702
c number_of_pure_literals               768
c reading-and-set-up_time(sec)          0.016
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf
s UNSATISFIABLE
c number_of_variables                   297
c number_of_clauses                     46881
c maximal_clause_length                 24
c number_of_literal_occurrences         209792
c running_time(sec)                     16405.15
c number_of_nodes                       139770235
c number_of_binary_nodes                69885117
c number_of_1-reductions                2524979169
c number_of_pure_literals               1102
c reading-and-set-up_time(sec)          0.021
c file_name                             VanDerWaerden_pd_2-3-24_593.cnf
s UNSATISFIABLE
c number_of_variables                   304
c number_of_clauses                     48979
c maximal_clause_length                 25
c number_of_literal_occurrences         219525
c running_time(sec)                     34485.68
c number_of_nodes                       299794077
c number_of_binary_nodes                149897038
c number_of_1-reductions                5507807197
c number_of_pure_literals               7948
c reading-and-set-up_time(sec)          0.021
c file_name                             VanDerWaerden_pd_2-3-25_607.cnf
   \endverbatim
   For k <= 21 small improvements in node-counts, but for k >= 22 quite a
   drastic increase. This shows that the weights can matter (for differences
   up to 50%; not taking "disastreous weights" into account), while the
   current way of optimisation doesn't seem right. Perhaps optimising on
   palindromic instances is "too special", doesn't generalise? </li>
   <li> After optimisation on VanDerWaerden_2-3-13_160.cnf:
   \verbatim
tawSolver:
 authors: Tanbir Ahmed and Oliver Kullmann
 url's:
  http://sourceforge.net/projects/tawsolver/
  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/TawSolver/tawSolver.cpp
 Version: 2.4.5
 Last change date: 4.8.2013
 Mapping k -> weight, for clause-lengths k specified at compile-time:
   2->5.15  3->1  4->0.305  5->0.117  6->0.056
 Divisor for open weights: 1.6
 Macro settings:
  LIT_TYPE = std::int32_t (with 31 binary digits)
  UCP_STRATEGY = 1
  TAU_ITERATION = 5
 Compiled with PURE_LITERALS
 Compiled with NDEBUG
 Compiled with optimisation options
 Compilation date: Aug  4 2013 22:11:31
 Compiler: g++, version 4.7.3
 Provided in the OKlibrary http://www.ok-sat-library.org
 Git ID = 57bf2f09e14dd502547ca22e07adc31eb1d03e19
s UNSATISFIABLE
c number_of_variables                   140
c number_of_clauses                     10536
c maximal_clause_length                 17
c number_of_literal_occurrences         45139
c running_time(sec)                     1.08
c number_of_nodes                       32011
c number_of_binary_nodes                16005
c number_of_1-reductions                396129
c number_of_pure_literals               2
c reading-and-set-up_time(sec)          0.005
c file_name                             VanDerWaerden_pd_2-3-17_279.cnf
s UNSATISFIABLE
c number_of_variables                   156
c number_of_clauses                     13277
c maximal_clause_length                 18
c number_of_literal_occurrences         58763
c running_time(sec)                     10.05
c number_of_nodes                       263293
c number_of_binary_nodes                131646
c number_of_1-reductions                3374489
c number_of_pure_literals               13
c reading-and-set-up_time(sec)          0.010
c file_name                             VanDerWaerden_pd_2-3-18_312.cnf
s UNSATISFIABLE
c number_of_variables                   174
c number_of_clauses                     16208
c maximal_clause_length                 19
c number_of_literal_occurrences         70414
c running_time(sec)                     12.43
c number_of_nodes                       276897
c number_of_binary_nodes                138448
c number_of_1-reductions                3860839
c number_of_pure_literals               7
c reading-and-set-up_time(sec)          0.008
c file_name                             VanDerWaerden_pd_2-3-19_347.cnf
s UNSATISFIABLE
c number_of_variables                   195
c number_of_clauses                     20327
c maximal_clause_length                 20
c number_of_literal_occurrences         88944
c running_time(sec)                     46.51
c number_of_nodes                       875083
c number_of_binary_nodes                437541
c number_of_1-reductions                13027439
c number_of_pure_literals               17
c reading-and-set-up_time(sec)          0.012
c file_name                             VanDerWaerden_pd_2-3-20_389.cnf
s UNSATISFIABLE
c number_of_variables                   203
c number_of_clauses                     21950
c maximal_clause_length                 21
c number_of_literal_occurrences         96305
c running_time(sec)                     113.17
c number_of_nodes                       2116911
c number_of_binary_nodes                1058455
c number_of_1-reductions                31436580
c number_of_pure_literals               134
c reading-and-set-up_time(sec)          0.015
c file_name                             VanDerWaerden_pd_2-3-21_405.cnf
s UNSATISFIABLE
c number_of_variables                   232
c number_of_clauses                     28650
c maximal_clause_length                 22
c number_of_literal_occurrences         126560
c running_time(sec)                     559.30
c number_of_nodes                       8331157
c number_of_binary_nodes                4165578
c number_of_1-reductions                134582107
c number_of_pure_literals               306
c reading-and-set-up_time(sec)          0.019
c file_name                             VanDerWaerden_pd_2-3-22_463.cnf
s UNSATISFIABLE
c number_of_variables                   254
c number_of_clauses                     34289
c maximal_clause_length                 23
c number_of_literal_occurrences         152236
c running_time(sec)                     1513.93
c number_of_nodes                       19522233
c number_of_binary_nodes                9761116
c number_of_1-reductions                334565286
c number_of_pure_literals               592
c reading-and-set-up_time(sec)          0.018
c file_name                             VanDerWaerden_pd_2-3-23_507.cnf
s UNSATISFIABLE
c number_of_variables                   297
c number_of_clauses                     46881
c maximal_clause_length                 24
c number_of_literal_occurrences         209792
c running_time(sec)                     12360.94
c number_of_nodes                       114347277
c number_of_binary_nodes                57173638
c number_of_1-reductions                2087185157
c number_of_pure_literals               729
c reading-and-set-up_time(sec)          0.022
c file_name                             VanDerWaerden_pd_2-3-24_593.cnf
s UNSATISFIABLE
c number_of_variables                   304
c number_of_clauses                     48979
c maximal_clause_length                 25
c number_of_literal_occurrences         219525
c running_time(sec)                     24250.43
c number_of_nodes                       230372423
c number_of_binary_nodes                115186211
c number_of_1-reductions                4300321002
c number_of_pure_literals               3724
c reading-and-set-up_time(sec)          0.022
c file_name                             VanDerWaerden_pd_2-3-25_607.cnf
   \endverbatim
   An improvement over the previous weights, and for k <= 23 also (small)
   improvements over the original node-count (for version 2.1.2), but for
   k=24,25 still quite substantially worse. </li>
   <li> Thus for now we reset the weights for the tau-heuristic to the
   original values.
    <ol>
     <li> Optimisation on ordinary vdW-problems seems superior to optimisation
     on palindromic problems, since the latter instances have too many
     "peculiarities". </li>
     <li> But still, apparently from a certain size on the
     optimisation-results do not generalise. </li>
    </ol>
   </li>
  </ul>


  \todo Performance evaluation
  <ul>
   <li> From version 1.0 to version 1.8 an enormous improvement; when
   comparing the run-times and node-counts:
   \verbatim
> round(c(7,60,134,738,2541,18306,86869)/c(1,12,17,62,155,1014,2321), digits=1)
[1] 7.0  5.0  7.9 11.9 16.4 18.1 37.4
> round(c(143319,1063979,2009635,9076261,30470349,170414771,573190251)/c(31401,320147,349133,1067245,2656801,13431053,26126051), digits=1)
[1] 4.6  3.3  5.8  8.5 11.5 12.7 21.9
   \endverbatim
   </li>
   <li> Version 1.8 is now the fastest single solver XXX
   Cube&Conquer via OKsolver&minisat22
   </li>
  </ul>

*/
