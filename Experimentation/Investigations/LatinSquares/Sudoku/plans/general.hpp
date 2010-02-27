// Oliver Kullmann, 1.11.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LatinSquares/plans/Sudoku.hpp
  \brief On investigations regarding Sudoku problems (standard, and extended)

  Besides experimentations in creating and solving big Sudoku problems (with
  or without the uniqueness constraint) one should harness the popularity
  of Sudoku to teach (generalised) satisfiability.
  
  
  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Sudoku.hpp.
   </li>
   <li> See Applications/LatinSquares/plans/Sudoku.hpp. </li>
  </ul>


  \todo Studying translations to boolean CNFs
  <ul>
   <li> In ComputerAlgebra/Satisfiability/Lisp/Generators/Sudoku.mac we
   provide three translations:
    <ol>
     <li> output_weak_sdk_stdname </li>
     <li> output_dual_weak_sdk_stdname </li>
     <li> output_strong_sdk_stdname </li>
    </ol>
   </li>
   <li> Running times for the empty problems for box dimension p:
    <ol>
     <li> p=3, OKsolver_2002:
     \verbatim
> OKsolver_2002-O3-DNDEBUG Weak_Sudoku_Box_dim_3.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             8829
c initial_number_of_literal_occurrences 18225
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   8748
c running_time(sec)                     0.1
c number_of_nodes                       39
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                119
c number_of_pure_literals               68
c number_of_autarkies                   3
c number_of_missed_single_nodes         0
c max_tree_depth                        38
c number_of_table_enlargements          0
c number_of_1-autarkies                 12965
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Weak_Sudoku_Box_dim_3.cnf

> OKsolver_2002-O3-DNDEBUG Dual_Weak_Sudoku_Box_dim_3.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             3159
c initial_number_of_literal_occurrences 8019
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   2916
c running_time(sec)                     0.1
c number_of_nodes                       122
c number_of_single_nodes                13
c number_of_quasi_single_nodes          0
c number_of_2-reductions                671
c number_of_pure_literals               39
c number_of_autarkies                   10
c number_of_missed_single_nodes         6
c max_tree_depth                        50
c number_of_table_enlargements          0
c number_of_1-autarkies                 1974
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Dual_Weak_Sudoku_Box_dim_3.cnf

> OKsolver_2002-O3-DNDEBUG Strong_Sudoku_Box_dim_3.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             11988
c initial_number_of_literal_occurrences 26244
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   11664
c running_time(sec)                     0.3
c number_of_nodes                       36
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                25
c number_of_pure_literals               0
c number_of_autarkies                   2
c number_of_missed_single_nodes         0
c max_tree_depth                        35
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Strong_Sudoku_Box_dim_3.cnf
     \endverbatim
     So with weak and strong form no backtracking is needed, but it is needed
     with the dual weak form.
     </li>
     <li> p=3, march_pl:
     \verbatim
> march_pl Weak_Sudoku_Box_dim_3.cnf
c main():: nodeCount: 4104
c main():: dead ends in main: 1370
c main():: lookAheadCount: 96244
c main():: unitResolveCount: 68904
c main():: time=0.830000
c main():: necessary_assignments: 4418
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 2839, succes #: 676
c main():: doublelook: overall 3.037 of all possible doublelooks executed
c main():: doublelook: succesrate: 23.811, average DL_trigger: 4.051
c main():: SOLUTION VERIFIED :-)
s SATISFIABLE

> march_pl Dual_Weak_Sudoku_Box_dim_3.cnf

> march_pl Strong_Sudoku_Box_dim_3.cnf
c main():: nodeCount: 143
c main():: dead ends in main: 0
c main():: lookAheadCount: 25395
c main():: unitResolveCount: 729
c main():: time=1.200000
c main():: necessary_assignments: 24
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 1539, succes #: 0
c main():: doublelook: overall 6.062 of all possible doublelooks executed
c main():: doublelook: succesrate: 0.000, average DL_trigger: 6.053
c main():: SOLUTION VERIFIED :-)
s SATISFIABLE
     \endverbatim
     <li> p=3, satz215:
     \verbatim
> satz215 Weak_Sudoku_Box_dim_3.cnf
**** The instance is satisfiable. *****
NB_MONO= 89, NB_UNIT= 553, NB_BRANCHE= 41, NB_BACK= 0
Program terminated in 0.000 seconds.
satz215 Weak_Sudoku_Box_dim_3.cnf 0.000 41 0 24973 1 1 729 8829 -1458 0 0

> satz215 Dual_Weak_Sudoku_Box_dim_3.cnf


     </li>
     <li> p=4, OKsolver_2002:
     \verbatim
> OKsolver_2002-O3-DNDEBUG Weak_Sudoku_Box_dim_4.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           4096
c initial_number_of_clauses             92416
c initial_number_of_literal_occurrences 188416
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   92160
c running_time(sec)                     32.7
c number_of_nodes                       139
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                319
c number_of_pure_literals               469
c number_of_autarkies                   3
c number_of_missed_single_nodes         0
c max_tree_depth                        137
c number_of_table_enlargements          0
c number_of_1-autarkies                 262599
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Weak_Sudoku_Box_dim_4.cnf

> OKsolver_2002-O3-DNDEBUG Dual_Weak_Sudoku_Box_dim_4.cnf
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         16
c initial_number_of_variables           4096
c initial_number_of_clauses             31488
c initial_number_of_literal_occurrences 73728
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   30720
c running_time(sec)                     9121.6
c number_of_nodes                       2170917
c number_of_single_nodes                9990
c number_of_quasi_single_nodes          0
c number_of_2-reductions                15034724
c number_of_pure_literals               2225993
c number_of_autarkies                   11547
c number_of_missed_single_nodes         5343
c max_tree_depth                        189
c number_of_table_enlargements          0
c number_of_1-autarkies                 208610462
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Dual_Weak_Sudoku_Box_dim_4.cnf

> OKsolver_2002-O3-DNDEBUG Strong_Sudoku_Box_dim_4.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         16
c initial_number_of_variables           4096
c initial_number_of_clauses             123904
c initial_number_of_literal_occurrences 262144
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   122880
c running_time(sec)                     108.9
c number_of_nodes                       144
c number_of_single_nodes                0
c number_of_quasi_single_nodes          1
c number_of_2-reductions                117
c number_of_pure_literals               0
c number_of_autarkies                   2
c number_of_missed_single_nodes         0
c max_tree_depth                        143
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Strong_Sudoku_Box_dim_4.cnf
     \endverbatim
     So for the weak form one backtrack was needed, none for the strong
     form, while the dual weak form gets very hard. </li>
     <li> p=4, march_pl: none of the problems is solved within an hour. </li>
     <li> p=4, minisat2:
     \verbatim
> minisat2 Weak_Sudoku_Box_dim_4.cnf
restarts              : 1
conflicts             : 0              (0 /sec)
decisions             : 678            (1.33 % random) (478 /sec)
propagations          : 3840           (2707 /sec)
conflict literals     : 0              ( nan % deleted)
Memory used           : 9.52 MB
CPU time              : 1.41878 s

> minisat2 Dual_Weak_Sudoku_Box_dim_4.cnf
*** INTERRUPTED ***
restarts              : 25
conflicts             : 4412395        (483 /sec)
decisions             : 5597098        (1.91 % random) (613 /sec)
propagations          : 101006508      (11062 /sec)
conflict literals     : 786048341      (2.91 % deleted)
Memory used           : 239.70 MB
CPU time              : 9131.2 s

> minisat2 Strong_Sudoku_Box_dim_4.cnf
restarts              : 1
conflicts             : 0              (0 /sec)
decisions             : 677            (1.03 % random) (799 /sec)
propagations          : 4096           (4837 /sec)
conflict literals     : 0              ( nan % deleted)
Memory used           : 7.06 MB
CPU time              : 0.846871 s
     \endverbatim
     </li>
     <li> So at least for complete solvers and the empty board the dual weak
     translation makes the problem very hard. </li>
    </ol>
   </li>
  </ul>


  \todo Finding hard instances for box-dimension 3
  <ul>
   <li> A source for "hard" instances is [extreme Su Doku; Wayne Gould].
    <ol>
     <li> The cover-text says "... Wayne Gould, who'll take you deep inside his
     genius psyche.", and "..., he has been named by Time Magazine as one of
     the 100 people who shaped the world.". </li>
     <li> One sees that enlightenment is needed. </li>
     <li> As one can see in
     ComputerAlgebra/Satisfiability/Lisp/Generators/docus/Sudoku.hpp,
     apparently most problems only need one r_2-reduction, while perhaps the
     most difficult instance in that collection needs five r_2-reductions
     (without backtracking, and thus also uniqueness is confirmed). </li>
     <li> So likely this source is without further interest. </li>
    </ol>
   </li>
   <li> A better source is [Denis Berthier, The Hidden Logic of Sudoku, 2007]
    <ol>
     <li> See the collections at http://www.carva.org/denis.berthier . </li>
     <li> This source only consider these "humanoid rules" (AI-nonsense). </li>
     <li> Perhaps all these rules are covered by r_2 for the strong encoding?
     </li>
     <li> In principle the all-different constraint can not be covered, but
     perhaps this doesn't happen for these small problems (box-dimension 3)?
     </li>
    </ol>
   </li>
   <li> We should search for problems which are as hard as possible.
    <ol>
     <li> See
     "Sampling of minimally uniquely satisfiable problems" in
     ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Sudoku.hpp. </li>
     <li> The "EasterMonster" (see
     Satisfiability/Lisp/Generators/docus/Sudoku.hpp) needs 9 nodes with
     the OKsolver_2002; this is the hardest example yet encountered. </li>
     <li> There should be harder *unsatisfiable* problems. </li>
     <li> While for the uniquely satisfiable problems actually it would be
     good if the OKsolver could enumerate all solution --- since here this
     would yield the proof of uniqueness, and only this would be comparable
     to the rule-based approach, which also always shows uniqueness. </li>
     <li> At least we need to extract the solution, add it negated to the
     conditions, and see what the complexity of the refutation is. </li>
     <li> This refutation also contains the direct proof that the given total
     assignment is the unique solution. </li>
     <li> Perhaps it is then enough to search for short (tree-like) resolution
     refutations of the original sdk-clause-set ("empty field") plus the
     negated (total) satisfying assignment. </li>
     <li> The refutation of the "EasterMonster" with negated solution actually
     doesn't get any harder than finding the solution; see
     Satisfiability/Lisp/Generators/docus/Sudoku.hpp. </li>
     <li> See below for the "human editing". </li>
    </ol>
   </li>
   <li> For the "human" solution of such problems, "explanations" are needed
   for the solution, which is just a demonstration of the sequence of forced
   assignments.
    <ol>
     <li> In [extreme Su Doku; Wayne Gould] the condition "without reductio ad
     absurdum" is stated. Can this be made precise? </li>
     <li> In [Denis Berthier The Hidden Logic of Sudoku, 2007] one finds a
     more systematic approach, and also an attempt at defining what "without
     guessing" could mean. </li>
     <li> Perhaps r_2-reduction would not be eligible here, and perhaps
     one should use full-strength bijectivity-constraints? </li>
     <li> Of course, one can simulate r_2-reduction via resolution, which
     is "forward reasoning"; so perhaps one should search for short
     resolution derivations of the forced assignments. </li>
     <li> Actually, the last remark seems crucial, showing that all that talk
     about "trial and error" and so on is nonsense: Every forced assignment
     can be demonstrated by r_k for k big enough, and this "trial-and-error"
     derivation can be translated into a "direct logical reasoning", namely
     a resolution refutation (of the unit-clause corresponding to the
     forced assignment)! </li>
     <li> One should do so for example for the above "EasterMonster"-example,
     where [Denis Berthier] claims "no resolution rule is yet known to be
     applicable to this puzzle". </li>
     <li> He speaks of "rules", while the resolution proof is not a general
     rule; so well, perhaps some form of automated generalisation is possible?
     </li>
     <li> The full refutation of the EasterMonster with negation is just 9
     nodes (and 100 r_2-reductions). Of course a lot of unit-propagations are
     involved. A good representation of this refutation would be valuable.
     </li>
    </ol>
   </li>
  </ul>


  \todo Finding hard instances for box-dimension >3
  <ul>
   <li> Some puzzles of box-dimension greater than 3 are available on these
   webpages:
    <ol>
     <li> http://www.setbb.com/phpbb/viewtopic.php?mforum=sudoku&p=10870 </li>
     <li> http://www.menneske.no/sudoku/eng/ </li>
    </ol>
   </li>
  </ul>

*/

