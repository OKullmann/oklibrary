// Oliver Kullmann, 18.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/UsingSAT.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs via SAT methods


  \todo Translation via addition
  <ul>
   <li> We have already implemented (as an old C++ program) the translation
   of linear inequalities into CNF, and so we can also use (ordinary) SAT
   solvers.
    <ol>
     <li> By "VdWTransversals 3 54 35" we create a Dimacs file, containing the
     hypergraph clauses for n=53 plus the upper bound (here b=35) on the
     transversal size (so that in this case the clause-set is unsatisfiable,
     while for b=36 it is satisfiable). </li>
     <li> And via "VdWTransversalsInc 3 1 0 OutputFile" we compute all the
     transversal number for k=3, starting with n=1. </li>
    </ol>
   </li>
   <li> VdWTransversals uses LinInequal-O3-DNDEBUG, which uses only an
   upper bound on the transversal size: It should be more efficient to
   use an (exact) equality; see "Complete LinInequal.cpp" in
   Transformers/Generators/plans/LinInequal.hpp. </li>
  </ul>


  \todo Look-ahead solvers with translation via addition
  <ul>
   <li> It seems that OKsolver_2002 and march_pl need a long time.
   \verbatim
> VdWTransversals 3 40 24
> OKsolver_2002-O3-DNDEBUG vdw_trans_3_40_24.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=169 initial_number_of_clauses=1067 initial_number_of_literal_occurrences=3459 running_time(s)=84.4 number_of_nodes=870214 number_of_single_nodes=3257 number_of_quasi_single_nodes=483 number_of_2-reductions=2279568 number_of_pure_literals=0 number_of_autarkies=4500 number_of_missed_single_nodes=6084 max_tree_depth=43 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=2 reduced_number_of_clauses=6 reduced_number_of_literal_occurrences=17 number_of_1-autarkies=74022 number_of_initial_unit-eliminations=2 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=77 file_name=vdw_trans_3_40_24.cnf
> march_pl vdw_trans_3_40_24.cnf
 main():: nodeCount: 98655
c main():: dead ends in main: 6199
c main():: lookAheadCount: 5794240
c main():: unitResolveCount: 1044234
c main():: time=28.300000
c main():: necessary_assignments: 54362
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 201867, succes #: 120077
c main():: doublelook: overall 3.646 of all possible doublelooks executed
c main():: doublelook: succesrate: 59.483, average DL_trigger: 85.056
s UNSATISFIABLE
> OKsolver_2002-O3-DNDEBUG vdw_trans_3_40_25.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=4 initial_number_of_variables=169 initial_number_of_clauses=1066 initial_number_of_literal_occurrences=3456 running_time(s)=13.5 number_of_nodes=149781 number_of_single_nodes=1054 number_of_quasi_single_nodes=155 number_of_2-reductions=372636 number_of_pure_literals=0 number_of_autarkies=1084 number_of_missed_single_nodes=1602 max_tree_depth=40 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=2 reduced_number_of_clauses=6 reduced_number_of_literal_occurrences=17 number_of_1-autarkies=17208 number_of_initial_unit-eliminations=2number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=77 file_name=vdw_trans_3_40_25.cnf
c main():: nodeCount: 32051
c main():: dead ends in main: 1503
c main():: lookAheadCount: 1972878
c main():: unitResolveCount: 369319
c main():: time=8.890000
c main():: necessary_assignments: 21652
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 63523, succes #: 37370
c main():: doublelook: overall 3.371 of all possible doublelooks executed
c main():: doublelook: succesrate: 58.829, average DL_trigger: 83.574
c main():: SOLUTION VERIFIED :-)
s SATISFIABLE
   \endverbatim
   </li>
   <li> One should investigate whether the minisat-preprocessor makes
   things easier.
   \verbatim
> minisat2 -dimacs=vdw_trans_3_40_24_m2pp.cnf vdw_trans_3_40_24.cnf
> OKsolver_2002-O3-DNDEBUG vdw_trans_3_40_24_m2pp.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=5 initial_number_of_variables=150 initial_number_of_clauses=992 initial_number_of_literal_occurrences=3159 running_time(s)=28.4 number_of_nodes=278736 number_of_single_nodes=39 number_of_quasi_single_nodes=307 number_of_2-reductions=846245 number_of_pure_literals=0 number_of_autarkies=3786 number_of_missed_single_nodes=208 max_tree_depth=34 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=85839 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=52 file_name=vdw_trans_3_40_24_m2pp.cnf
> march_pl vdw_trans_3_40_24_m2pp.cnf
c main():: nodeCount: 581980
c main():: dead ends in main: 101283
c main():: lookAheadCount: 8918023
c main():: unitResolveCount: 8120863
c main():: time=64.440000
c main():: necessary_assignments: 364624
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 504762, succes #: 147097
c main():: doublelook: overall 6.214 of all possible doublelooks executed
c main():: doublelook: succesrate: 29.142, average DL_trigger: 80.024
s UNSATISFIABLE
> minisat2 -dimacs=vdw_trans_3_40_25_m2pp.cnf vdw_trans_3_40_25.cnf
> OKsolver_2002-O3-DNDEBUG vdw_trans_3_40_25_m2pp.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=149 initial_number_of_clauses=997 initial_number_of_literal_occurrences=3184 running_time(s)=1.9 number_of_nodes=19311 number_of_single_nodes=19 number_of_quasi_single_nodes=18 number_of_2-reductions=64340 number_of_pure_literals=0 number_of_autarkies=239 number_of_missed_single_nodes=43 max_tree_depth=33 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=13171 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=47 file_name=vdw_trans_3_40_25_m2pp.cnf
> march_pl vdw_trans_3_40_25_m2pp.cnf
c main():: nodeCount: 360128
c main():: dead ends in main: 47865
c main():: lookAheadCount: 5723996
c main():: unitResolveCount: 4829333
c main():: time=39.310000
c main():: necessary_assignments: 205107
c main():: bin_sat: 0, bin_unsat 0
c main():: doublelook: #: 323254, succes #: 100136
c main():: doublelook: overall 6.250 of all possible doublelooks executed
c main():: doublelook: succesrate: 30.977, average DL_trigger: 78.028
c main():: SOLUTION VERIFIED :-)
s SATISFIABLE
   \endverbatim
   It seems to help OKsolver_2002 (quite a bit, but likely still much worse
   than minisat), while march_pl seems to perform quite a bit worse on
   the preprocessed problems. </li>
  </ul>


  \todo Conflict-driven solvers with translation via addition
  <ul>
   <li> minisat solves the problems (unsatisfiable as well as
   satisfiable) relatively easily (though with n=80 around 15 minutes are
   needed). </li>
   <li> Is minisat2 better than minisat?
   \verbatim
minisat2 vdw_trans_3_64_43.cnf
restarts              : 22
conflicts             : 1261598        (21895 /sec)
decisions             : 1423468        (1.35 % random) (24704 /sec)
propagations          : 68517361       (1189120 /sec)
conflict literals     : 17080977       (44.36 % deleted)
Memory used           : 3.25 MB
CPU time              : 57.6202 s
UNSATISFIABLE
> minisat vdw_trans_3_64_43.cnf
restarts              : 21
conflicts             : 932740         (19276 /sec)
decisions             : 1048977        (21679 /sec)
propagations          : 53595491       (1107628 /sec)
conflict literals     : 12450770       (45.50 % deleted)
Memory used           : 4.73 MB
CPU time              : 48.3876 s
UNSATISFIABLE
> minisat2 vdw_trans_3_65_44.cnf
restarts              : 22
conflicts             : 1324767        (21908 /sec)
decisions             : 1484394        (1.39 % random) (24547 /sec)
propagations          : 71723828       (1186090 /sec)
conflict literals     : 17963070       (42.79 % deleted)
Memory used           : 3.39 MB
CPU time              : 60.4708 s
UNSATISFIABLE
> minisat vdw_trans_3_64_43.cnf
restarts              : 22
conflicts             : 1331275        (18706 /sec)
decisions             : 1490338        (20941 /sec)
propagations          : 76617377       (1076568 /sec)
conflict literals     : 17680839       (44.17 % deleted)
Memory used           : 4.97 MB
CPU time              : 71.1682 s
UNSATISFIABLE
> minisat vdw_trans_3_71_50.cnf
restarts              : 25
conflicts             : 4375702        (15250 /sec)
decisions             : 4893653        (17055 /sec)
propagations          : 265492436      (925292 /sec)
conflict literals     : 65592198       (43.40 % deleted)
Memory used           : 6.57 MB
CPU time              : 286.928 s
SATISFIABLE
> minisat2 vdw_trans_3_71_50.cnf
restarts              : 26
conflicts             : 5730961        (17171 /sec)
decisions             : 6427303        (1.39 % random) (19257 /sec)
propagations          : 320252313      (959508 /sec)
conflict literals     : 86355473       (42.23 % deleted)
Memory used           : 4.94 MB
CPU time              : 333.767 s
SATISFIABLE
   \endverbatim
   It seems that actually minisat might be better on these instances
   than minisat2 (using more space, but less conflicts and less time). </li>
  </ul>


  \todo Local search solvers with translation via addition
  <ul>
   <li> The satisfiable instances seem rather hard for local search solvers:
   \verbatim
> ubcsat-okl -alg samd -runs 100 -i vdw_trans_3_40_25.cnf
Clauses = 1066
Variables = 169
TotalLiterals = 3456
FlipsPerSecond = 724638
BestStep_Mean = 1531.870000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
> ubcsat-okl -alg walksat -runs 100 -i vdw_trans_3_40_25.cnf
Clauses = 1066
Variables = 169
TotalLiterals = 3456
FlipsPerSecond = 1727116
BestStep_Mean = 2559.370000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
> ubcsat-okl -alg walksat-tabu -runs 100 -i vdw_trans_3_40_25.cnf
Clauses = 1066
Variables = 169
TotalLiterals = 3456
FlipsPerSecond = 1700680
BestStep_Mean = 6494.100000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
> ubcsat-okl -alg rsaps -runs 100 -i vdw_trans_3_40_25.cnf
Clauses = 1066
Variables = 169
TotalLiterals = 3456
FlipsPerSecond = 1050420
BestStep_Mean = 576.900000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
   \endverbatim
   So perhaps rsaps is best here.
   </li>
   <li> Higher cutoffs:
   \verbatim
> ubcsat-okl -alg rsaps -runs 100 -cutoff 1000000 -i vdw_trans_3_40_25.cnf
Clauses = 1066
Variables = 169
TotalLiterals = 3456
FlipsPerSecond = 1098388
BestStep_Mean = 35359.710000
Steps_Mean = 964824.390000
Steps_Max = 1000000.000000
PercentSuccess = 7.00
BestSolution_Mean = 0.930000
BestSolution_Median = 1.000000
BestSolution_Min = 0.000000
BestSolution_Max = 1.000000
   \endverbatim
   Finding a local-minimum=1 is very easy (a few hundred steps), but finding
   a solution takes around 400000 steps. Similar for bigger instances:
   \verbatim
> ubcsat-okl -alg rsaps -runs 100 -i vdw_trans_3_71_50.cnf
Clauses = 2593
Variables = 342
TotalLiterals = 8172
FlipsPerSecond = 974659
BestStep_Mean = 3914.710000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
> ubcsat-okl -alg rsaps -runs 100 -cutoff 1000000 -i vdw_trans_3_71_50.cnf
Clauses = 2593
Variables = 342
TotalLiterals = 8172
FlipsPerSecond = 908430
BestStep_Mean = 3887.970000
Steps_Mean = 1000000.000000
Steps_Max = 1000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
> ubcsat-okl -alg rsaps -runs 100 -cutoff 10000000 -i vdw_trans_3_71_50.cnf
Clauses = 2593
Variables = 342
TotalLiterals = 8172
FlipsPerSecond = 907663
BestStep_Mean = 3736.860000
Steps_Mean = 10000000.000000
Steps_Max = 10000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
> ubcsat-okl -alg rsaps -runs 100 -cutoff 100000000 -i vdw_trans_3_71_50.cnf
Clauses = 2593
Variables = 342
TotalLiterals = 8172
FlipsPerSecond = 916484
BestStep_Mean = 4043.990000
Steps_Mean = 100000000.000000
Steps_Max = 100000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 1.000000
BestSolution_Median = 1.000000
BestSolution_Min = 1.000000
BestSolution_Max = 1.000000
   \endverbatim
   One sees that the (very few) solutions are very hard to find for a local
   search algorithm. </li>
   <li> UnitMarch seems not to perform:
   \verbatim
> UnitMarch_32_bits vdw_trans_3_40_25.cnf
   \endverbatim
   seems unsuccesful (unfortunately, there are no statistics available, and
   also the seed can't be changed).
   </li>
  </ul>


  \todo Translation to pseudo-boolean problems
  <ul>
   <li> The first impression, using "VdWTransversalsIncPB 3 1 0 Output"
   instead of "VdWTransversalsInc 3 1 0 Output", is that it is actually
   slower! </li>
  </ul>


  \todo Using CSP solvers

*/
