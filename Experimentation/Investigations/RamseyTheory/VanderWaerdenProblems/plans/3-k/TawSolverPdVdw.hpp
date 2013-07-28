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
   \endverbatim
   </li>
  </ul>


  \todo Performance evaluation
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
