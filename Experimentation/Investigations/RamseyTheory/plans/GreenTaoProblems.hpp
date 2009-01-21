// Oliver Kullmann, 14.11.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/plans/GreenTaoProblems.hpp
  \brief On investigations into Green-Tao problems


  \todo Trivial Green-Tao numbers
  <ul>
   <li> greentao(p,k) for natural numbers p,k >= 0 is the smallest natural
   number n >= 0 such that partitioning the first n prime numbers into p
   parts is guaranteed to contain an arithmetic progression of size k. </li>
   <li> The most natural sequences are likely those for fixed p. </li>
   <li> greentao(2,0) = 0 </li>
   <li> greentao(2,1) = 1 </li>
   <li> greentao(2,2) = 3 </li>
  </ul>


  \todo greentao(2,3) = 23
  <ul>
   <li> greentao(2,3) = 23 (partitioning the first 23 prime numbers into 2
   parts, one part is guaranteed to contain an arithmetic progression of
   size 3, while using a smaller initial segment of prime numbers won't do).
   </li>
   <li> Trivial (for OKsolver-2002, and likely for any solver). </li>
  </ul>


  \todo greentao(2;3,4) = 79
  <ul>
   <li> Easy for OKsolver:
   \verbatim
> OKsolver_2002-O3-DNDEBUG GreenTao_2_3_4_78.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=4 initial_number_of_variables=77 initial_number_of_clauses=450 initial_number_of_literal_occurrences=1439 running_time(s)=0.0 number_of_nodes=25 number_of_single_nodes=0 number_of_quasi_single_nodes=1 number_of_2-reductions=56 number_of_pure_literals=7 number_of_autarkies=3 number_of_missed_single_nodes=0 max_tree_depth=10 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=25 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_3_4_78.cnf
   \endverbatim
   and
   \verbatim
> OKsolver_2002-O3-DNDEBUG GreenTao_2_3_4_79.cnf
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=4 initial_number_of_variables=78 initial_number_of_clauses=460 initial_number_of_literal_occurrences=1472 running_time(s)=0.0 number_of_nodes=41 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=133 number_of_pure_literals=3 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=6 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=37 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_3_4_79.cnf
   \endverbatim
   </li>
  </ul>


  \todo greentao(2,4) = 512
  <ul>
   <li> greentao(2,4) > 400 (trivial for OKsolver-2002). </li>
   <li> greentao(2,4) > 420 with 6683 nodes (OKsolver-2002). </li>
   <li> greentao(2,4) > 430 with 22267 nodes (OKsolver-2002). </li>
   <li> greentao(2,4) > 440 with 77791 nodes (OKsolver-2002). </li>
   <li> greentao(2,4) > 450 with 349914 nodes (OKsolver-2002). </li>
   <li> greentao(2,4) > 460 with 12777 nodes (OKsolver-2002). </li>
   <li> greentao(2,4) > 470 with 59506 nodes (OKsolver-2002). </li>
   <li> greentao(2,4) > 471 with 58889 nodes (OKsolver-2002). </li>
   <li> greentao(2,4) > 472 with 370222 nodes (OKsolver-2002). </li>
   <li> For this n, rnovelty finds a solution quickly (~ 100000 steps).
   </li>
   <li> n = 473: stopped after 884587 nodes (OKsolver). </li>
   <li> But found satisfiable with rnovelty. </li>
   <li> And rnovelty+ is even better. </li>
   <li> n = 480: stopped after 1536394 nodes (OKsolver). </li>
   <li> n = 500: Running it with
   \verbatim
OKsolver_2002-O3-DNDEBUG -M -D18 -F GreenTao_2_4_500.cnf
   \endverbatim
   found a solution after 11 hours (38,937,288 nodes; roughly 2% of the
   search space, looking at depth 18). </li>
   <li> Found satisfiable with rnovelty+. </li>
   <li> n = 510 found satisfiable with rnovelty+. </li>
   <li> n = 511 found satisfiable with rnovelty+. </li>
   <li> n = 512: rnovelty+ yields constantly 1 falsified clause
   (also with "-runs 500 -cutoff 10000000"). </li>
   <li>
   \verbatim
OKplatform> OKsolver_2002-O3-DNDEBUG -M -D16 GreenTao_2_4_512.cnf
   \endverbatim
   finished the first branch (~ 32768 nodes at depth 16) after 9 days
   (cs-wsok) and thus
   <center> greentao(2,4) = 512. </center> </li>
   <li>  Density is 8.7734375; here always counting all variables (though
   2 variables are missing). </li>
   <li> Stopped the computation:
   \verbatim
33082:  7497, 780471.5, 765655.7

s UNKNOWN
c sat_status=2 initial_maximal_clause_length=4 initial_number_of_variables=510 initial_number_of_clauses=4492 initial_number_of_literal_occurrences=17968 running_time(s)=780475.6 number_of_nodes=765469925 number_of_single_nodes=58 number_of_quasi_single_nodes=0 number_of_2-reductions=4696146230 number_of_pure_literals=728446 number_of_autarkies=2 number_of_missed_single_nodes=11248 max_tree_depth=61 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=31556781 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_4_512.cnf
   \endverbatim
   </li>
   <li> n = 515: rnovelty+ yields constantly 1 falsified clause. </li>
   <li> n = 520: rnovelty+ yields constantly 1 falsified clause. </li>
   <li> n = 530: rnovelty+ yields constantly 1 falsified clause. </li>
   <li> n = 540: rnovelty+ yields constantly 1 falsified clause. </li>
   <li> n = 550: rnovelty+ yields constantly 2 falsified clauses. </li>
   <li> n = 600: rnovelty+ yields constantly 6 falsified clauses. </li>
   <li> Minisat looks weak on these instances. </li>
   <li> And Grasp likely doesn't work here neither. </li>
   <li> But march_pl seems stronger on the satisfiable instances than
   OKsolver-2002, though n=500 seems also difficult. </li>
  </ul>


  \todo DONE (the basic algorithm works not too bad now)
  Better creation of problems
  <ul>
   <li> For n in this magnitude the Maxima computation of the hypergraph is
   already very slow --- a more intelligent algorithm for finding the
   arithmetic progression amongst the prime numbers is needed (likely we
   cannot exploit the speciality of prime numbers, but we do it for arbitrary
   lists of numbers).
   <ol>
    <li> DONE
     One could use memoisation in the form that for every n we store
     the additional hyperedges (k-progressions). </li>
     <li> DONE (doesn't improve a single computation, but several)
     This would it make rather quick, without imposing big memory
     burdens. </li>
    </ol>
   </li>
  </ul>


  \todo greentao(2;4,5) > 4194
  <ul>
   <li> n=2000 trivial for ubcsat-rnovelty+. </li>
   <li> n=4000 found satisfiable by ubcsat-rnovelty+ with
   msteps=8376482 and seed=2773039201. </li>
   <li> n=4100 found satisfiable by ubcsat-rnovelty+ with
   msteps=7230012 and seed=3310338573. </li>
   <li> n=4150 found satisfiable by ubcsat-rnovelty+ with
   msteps=17897909 and seed=1856779838. </li>
   <li> n=4164 found satisfiable by ubcsat-rnovelty+ with
   msteps=39295024 and seed=1480959763. </li>
   <li> n=4166 found satisfiable by ubcsat-rnovelty+ with
   msteps=83573644 and seed=871144102. </li>
   <li> n=4167 found satisfiable by ubcsat-rnovelty+ with
   msteps=891150901 and seed=1913694368. </li>
   <li> n=4168 seems unsatisfiable (but isn't): 58 runs with rnovelty+ and
   cutoff 100000000 yielded often min=1, but no satisfying assignment
   was found. But
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 100 -cutoff 1000000000 -i GreenTao_2_4_5_4168.cnf
       sat  min     osteps     msteps       seed
      1 0     1  738268145 1000000000  373431756
      2 0     1  160332016 1000000000 1761599948
      3 1     0  936705634  936705634  313191661
      4 0     1  325465137 1000000000 1569292939
      5 0     1  671883843 1000000000 3451537904
      6 1     0  193351757  193351757  679157717
      7 0     2  146929304 1000000000 1406719143
      8 0     1   94854828 1000000000 1206648114
      9 1     0  399415279  399415279 3266707576
   \endverbatim
   So a cutoff of 10^9 is needed to progress further. </li>
   <li> n=4175 found satisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 100 -cutoff 100000000 -i GreenTao_2_4_5_4175.cnf
Clauses = 98055
Variables = 4175
TotalLiterals = 401986
FlipsPerSecond = 304718
BestStep_Mean = 52672891.020000
Steps_Mean = 99224814.270000
Steps_Max = 100000000.000000
PercentSuccess = 3.00
BestSolution_Mean = 2.210000
BestSolution_Median = 2.000000
BestSolution_Min = 0.000000
BestSolution_Max = 6.000000
   \endverbatim
   (quickest solution: msteps=40542872, seed=2849343332).
   </li>
   <li> n=4188 found satisfiable by ubcsat-rnovelty+ with
   msteps=89754713 and seed=1408284365. </li>
   <li> n=4194 found satisfiable by ubcsat-rnovelty+ with
   msteps=922634517 and seed=1864969928. </li>
   <li> n=4197 looks unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 100 -cutoff 1000000000 -i GreenTao_2_4_5_4197.cnf
       sat  min     osteps     msteps       seed
      1 0     1  340323691 1000000000  744588579
      2 0     2   14686272 1000000000 3518539282
      3 0     1  408985275 1000000000  945832409
      4 0     1  110211777 1000000000  573383613
      5 0     1  267528773 1000000000 1177614520
      6 0     2   74866438 1000000000 2491079329
      7 0     3   21999403 1000000000 1319388202
      8 0     2  120329369 1000000000 1210367914
      9 0     1  806760625 1000000000 1611973161
     10 0     1  135781486 1000000000 2074714037
     11 0     1  536129169 1000000000 2952378764
   \endverbatim
   </li>
   <li> n=4200 looks unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 100 -cutoff 100000000 -i GreenTao_2_4_5_4200.cnf
Clauses = 99125
Variables = 4200
TotalLiterals = 406380
FlipsPerSecond = 294132
BestStep_Mean = 55051674.390000
Steps_Mean = 100000000.000000
Steps_Max = 100000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 3.210000
BestSolution_Median = 3.000000
BestSolution_Min = 1.000000
BestSolution_Max = 6.000000
   \endverbatim
   </li>
   <li> n=4250
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 10000000 -i GreenTao_2_4_5_4250.cnf
       sat  min     osteps     msteps       seed
      1 0    10    5899907   10000000 1673200738
      2 0     5    6814994   10000000 1380530593
      3 0    10    7817348   10000000 1800085398
      4 0    12    9804471   10000000   81344402
      5 0    12    3880176   10000000  432120567
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 100000000 -i GreenTao_2_4_5_4250.cnf
       sat  min     osteps     msteps       seed
      1 0     6   94885885  100000000  282232988
      2 0     4   88868758  100000000 3253876323
      3 0     6   66409455  100000000  366509890
      4 0     7   43725056  100000000 2558278115
      5 0     5   42205257  100000000  564216858
      6 0     7   75727900  100000000  382452377
      7 0     8   15438757  100000000 2144637212
      8 0     6   56511298  100000000 2375147134
      9 0     5   76323464  100000000  847932947
     10 0     6   69876640  100000000 2763622722
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 1000000000 -i GreenTao_2_4_5_4250.cnf
       sat  min     osteps     msteps       seed
      1 0     3  957789520 1000000000 1347268192
      2 0     4   51871287 1000000000 2848316142
      3 0     2  998189178 1000000000 1876955045
      4 0     4  365650041 1000000000 1659925539
      5 0     4  153451341 1000000000 2857726654
      6 0     3  434671408 1000000000  159827785
      7 0     2  775236975 1000000000 1138200854
      8 0     3  429842430 1000000000 3112078298
      9 0     4  729284027 1000000000  737916580
     10 0     2  636479752 1000000000 3888433655
     11 0     3  458473821 1000000000 2600521927
     12 0     4  473667998 1000000000 1903692056
     13 0     6   15416778 1000000000 2974956240
     14 0     3  523470787 1000000000 2717980127
   \endverbatim
   </li>
   <li> n=4500 perhaps is unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 10000000 -i GreenTao_2_4_5_4500.cnf
       sat  min     osteps     msteps       seed
      1 0    63    9763671   10000000 1769275251
      2 0    64    7217982   10000000 1761999770
      3 0    50    7495970   10000000    2286273
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 100000000 -i GreenTao_2_4_5_4500.cnf
       sat  min     osteps     msteps       seed
      1 0    32   52302516  100000000  404695228
      2 0    34   92977284  100000000 2884982016
   \endverbatim
   </li>
   <li> n=5000 looks unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 10000000 -i GreenTao_2_4_5_5000.cnf
       sat  min     osteps     msteps       seed
      1 0   198    7643948   10000000  220666133
      2 0   202    2445858   10000000 2677059783
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 100000000 -i GreenTao_2_4_5_5000.cnf
       sat  min     osteps     msteps       seed
      1 0   171   25452928  100000000  576886492
      2 0   170   61069864  100000000 1322237633
   \endverbatim
   (remarkable the controllability of rnovelty+ on these instances!)
   </li>
   <li> n=6000 looks unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 10000000 -i GreenTao_2_4_5_6000.cnf
       sat  min     osteps     msteps       seed
      1 0   572    8204482   10000000  987022785
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 100000000 -i GreenTao_2_4_5_6000.cnf
       sat  min     osteps     msteps       seed
      1 0   548   71357965  100000000 1291665640
      2 0   544   98261120  100000000 2085465932
   \endverbatim
   </li>
  </ul>


  \todo greentao(2,5) : threshold behaviour of OKsolver-2002
  <ul>
   <li> Investigating the threshold of solvability for the OKsolver.
   One main conjecture is that Green-Tao problems behave like random problems.
   </li>
   <li> And, similarly to random problems, there seems
   to be a threshold, where below it OKsolver solves the problem
   without backtracking, while as soon as backtracking starts, the
   problems get very hard. </li>
   <li> This threshold seems to lie between n=20000 and n = 25000.
   <li> n = 20625, density = 14.29895757575758 below the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=20623 initial_number_of_clauses=294916 initial_number_of_literal_occurrences=1474580 running_time(s)=1961.7 number_of_nodes=6058 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=31532 number_of_pure_literals=4170 number_of_autarkies=17 number_of_missed_single_nodes=0 max_tree_depth=4548 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=876707 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_20625.cnf
   \endverbatim
   number_of_nodes=6058, max_tree_depth=4548, number_of_autarkies=17. </li>
   <li> n = 20938, density = 14.45181010602732 below the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=20936 initial_number_of_clauses=302592 initial_number_of_literal_occurrences=1512960 running_time(s)=-134.9 number_of_nodes=13317 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=166810 number_of_pure_literals=10313 number_of_autarkies=10 number_of_missed_single_nodes=1 max_tree_depth=4642 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=915632 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_20938.cnf
   \endverbatim
   number_of_nodes=13317, max_tree_depth=4642, number_of_autarkies=10. </li>
   <li> n = 21094, density = 14.52175974210676 below the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=21092 initial_number_of_clauses=306322 initial_number_of_literal_occurrences=1531610 running_time(s)=-1124.3 number_of_nodes=11418 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=141272 number_of_pure_literals=6025 number_of_autarkies=10 number_of_missed_single_nodes=0 max_tree_depth=4599 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=901676 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21094.cnf
   \endverbatim
   number_of_nodes=11418, max_tree_depth=4599, number_of_autarkies=10. </li>
   <li> n = 21133, density = 14.53802110443382 below the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=21131 initial_number_of_clauses=307232 initial_number_of_literal_occurrences=1536160 running_time(s)=-1440.9 number_of_nodes=42649 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=768325 number_of_pure_literals=37735 number_of_autarkies=29 number_of_missed_single_nodes=0 max_tree_depth=4607 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=1173442 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21133.cnf
   \endverbatim
   number_of_nodes=42649, max_tree_depth=4607, number_of_autarkies=29. </li>
   <li> n = 21153, density = 14.5477237271309 below the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=21151 initial_number_of_clauses=307728 initial_number_of_literal_occurrences=1538640 running_time(s)=-1644.1 number_of_nodes=37709 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=692225 number_of_pure_literals=23458 number_of_autarkies=7 number_of_missed_single_nodes=1 max_tree_depth=4601 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=1952904 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21153.cnf
   \endverbatim
   number_of_nodes=37709, max_tree_depth=4601, number_of_autarkies=7. </li>
   <li> n = 21163, density = 14.55086707933658 below (or about) the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=21161 initial_number_of_clauses=307940 initial_number_of_literal_occurrences=1539700 running_time(s)=-1325.5 number_of_nodes=103433 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=1964466 number_of_pure_literals=53123 number_of_autarkies=12 number_of_missed_single_nodes=5 max_tree_depth=4593 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=2295888 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21163.cnf
   \endverbatim
   number_of_nodes=103433, max_tree_depth=4593, number_of_autarkies=12. </li>
   <li> n = 21167, density = 14.55246374072849 below (or about) the threshold:
   \verbatim
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=5 initial_number_of_variables=21165 initial_number_of_clauses=308032 initial_number_of_literal_occurrences=1540160 running_time(s)=728.8 number_of_nodes=114686 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=2174254 number_of_pure_literals=58330 number_of_autarkies=12 number_of_missed_single_nodes=4 max_tree_depth=4620 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=2659174 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21167.cnf
   \endverbatim
   number_of_nodes=114686, max_tree_depth=4620, number_of_autarkies=12. </li>
   <li> n = 21170, density = 14.55370807746812 above the threshold:
   \verbatim
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=21168 initial_number_of_clauses=308102 initial_number_of_literal_occurrences=1540510 running_time(s)=522.1 number_of_nodes=498048 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=9099379 number_of_pure_literals=200245 number_of_autarkies=8 number_of_missed_single_nodes=19 max_tree_depth=2378 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=23567938 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21170.cnf
   \endverbatim
   number_of_nodes=498048, max_tree_depth=2378, number_of_autarkies=8. </li>
   <li> Regarding the 13 missed single nodes: It would be interesting to run
   the OKsolver-2002 with the order of branches inverted. </li>
   <li> n = 21172, density = 14.55403362932175 above the threshold:
   \verbatim
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=21170 initial_number_of_clauses=308138 initial_number_of_literal_occurrences=1540690 running_time(s)=-1760.2 number_of_nodes=410857 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=7531575 number_of_pure_literals=166713 number_of_autarkies=8 number_of_missed_single_nodes=15 max_tree_depth=2366 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=18406255 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21172.cnf
   \endverbatim
   number_of_nodes=410857, max_tree_depth=2366, number_of_autarkies=8 </li>
   <li> n = 21250, density = 14.59143529411765 above the threshold:
   \verbatim
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=21248 initial_number_of_clauses=310068 initial_number_of_literal_occurrences=1550340 running_time(s)=138.1 number_of_nodes=421583 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=8373297 number_of_pure_literals=187646 number_of_autarkies=14 number_of_missed_single_nodes=39 max_tree_depth=1187 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=15088691 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_21250.cnf
   \endverbatim
   number_of_nodes=421583, max_tree_depth=1187, number_of_autarkies=14. </li>
   <li> This should be repeated, until around 400000 nodes:
   n = 22500, density = 15.2064 above the threshold:
   \verbatim
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=22498 initial_number_of_clauses=342144 initial_number_of_literal_occurrences=1710720 running_time(s)=-1872.7 number_of_nodes=28974 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=516260 number_of_pure_literals=2207 number_of_autarkies=0 number_of_missed_single_nodes=2 max_tree_depth=613 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=63014 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_22500.cnf
   \endverbatim
   number_of_nodes=28974, max_tree_depth=613. </li>
   <li> Again, zero autarkies. </li>
  </ul>


  \todo greentao(2,5) > 33500
  <ul>
   <li> Trivial for n=5000:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_5_5000.cnf
Clauses = 26470
Variables = 5000
TotalLiterals = 132350
FlipsPerSecond = 254850
BestStep_Mean = 509.700000
Steps_Mean = 509.700000
Steps_Max = 538.000000
PercentSuccess = 100.00
   \endverbatim
   Density = 5.294
   </li>
   <li> Also OKsolver-2002 solves it in 2157 nodes. </li>
   <li> Also very easy for march_pl. </li>
   <li> Still trivial for n=10000:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_5_10000.cnf
Clauses = 85596
Variables = 10000
TotalLiterals = 427980
FlipsPerSecond = 126621
BestStep_Mean = 1772.700000
Steps_Mean = 1772.700000
Steps_Max = 1912.000000
PercentSuccess = 100.00
   \endverbatim
   Density = 8.5596
   </li>
   <li> Also OKsolver-2002 solves it in 4089 nodes. </li>
   <li> march_pl has more problems, but also solves it in
   about a minute. </li>
   <li> Still trivial for n=15000:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_5_15000.cnf
Clauses = 170830
Variables = 15000
TotalLiterals = 854150
FlipsPerSecond = 85313
BestStep_Mean = 4521.600000
Steps_Mean = 4521.600000
Steps_Max = 4785.000000
PercentSuccess = 100.00
   \endverbatim
   Density = 11.38866666666667
   </li>
   <li> For OKsolver-2002 it might be hard (or one has to wait a bit longer,
   since processing of the nodes now takes quite some time). </li>
   <li> march_pl is at least quicker, and finds a solution rather fast (9091
   nodes). Perhaps the difference is just that there are (nearly) no
   2-reductions possible, where march_pl reacts to this and gets quicker,
   while OKsolver-2002 always checks all possibilities. </li>
   <li> And yes, that's the case: OKsolver-2002 solves it with 4634 nodes,
   without backtracking, and with just 17 2-reductions. </li>
   <li> Still trivial for n=20000:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_5_20000.cnf
Clauses = 279592
Variables = 20000
TotalLiterals = 1397960
FlipsPerSecond = 115071
BestStep_Mean = 25775.900000
Steps_Mean = 25775.900000
Steps_Max = 80685.000000
PercentSuccess = 100.00
   \endverbatim
   Density = 13.9796
   </li>
   <li> OKsolver-2002 solves it with nearly no backtracking (4637 nodes, tree
   depth 4583, 2061 2-reductions). </li>
   <li> Still trivial for n=25000:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_5_25000.cnf
Clauses = 410070
Variables = 25000
TotalLiterals = 2050350
FlipsPerSecond = 106614
BestStep_Mean = 159174.900000
Steps_Mean = 159174.900000
Steps_Max = 330394.000000
PercentSuccess = 100.00
   \endverbatim
   Density = 16.4028 </li>
   <li> OKsolver-2002 however seems no longer be able to solve it
   (after 22h no node from depth 30 has been solved, so it got lost in
   an unsatisfiable (possibly very large) subproblem):
   \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D30 GreenTao_2_5_25000.cnf
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=24998
 initial_number_of_clauses=410070 initial_number_of_literal_occurrences=2050350
 running_time(s)=-797.6 number_of_nodes=38607 number_of_single_nodes=0
 number_of_quasi_single_nodes=0 number_of_2-reductions=817872
 number_of_pure_literals=2037 number_of_autarkies=0 number_of_missed_single_nodes=3
 max_tree_depth=602 number_of_table_enlargements=0 reduced_maximal_clause_length=0
 reduced_number_of_variables=0 reduced_number_of_clauses=0
 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=51044
 number_of_initial_unit-eliminations=0number_of_new_2-clauses=0
 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0
 file_name=GreenTao_2_5_25000.cnf
   \endverbatim
   number_of_nodes=38607, max_tree_depth=602.
   </li>
   <li> For minisat it looks hopeless:
   \verbatim
> minisat GreenTao_2_5_25000.cnf
*** INTERRUPTED ***
restarts              : 25
conflicts             : 4305064        (50 /sec)
decisions             : 5206820        (61 /sec)
propagations          : 623516770      (7282 /sec)
conflict literals     : 1379646880     (12.04 % deleted)
Memory used           : 1826.34 MB
CPU time              : 85622.6 s
     \endverbatim
   </li>
   <li> Also for march_pl it looks hopeless (after 51 hours
   no progress visible; 138348 clauses were added by
   preprocessing). </li>
   <li> UnitMarch also looks hopeless (after 10 hours). </li>
   <li> Gets harder for n = 30000, but still rather easy:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_5_30000.cnf
Clauses = 561550
Variables = 30000
TotalLiterals = 2807750
FlipsPerSecond = 138854
BestStep_Mean = 7262075.200000
Steps_Mean = 9474284.500000
Steps_Max = 10000000.000000
PercentSuccess = 30.00
BestSolution_Mean = 2.300000
BestSolution_Median = 1.000000
BestSolution_Min = 0.000000
BestSolution_Max = 6.000000
   \endverbatim
   Density = 18.71833333333333
   </li>
   <li> n = 31000; density = 19.17083870967742
   Is now somewhat harder, but still not too hard (and satisfiable)
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 30000000 -i GreenTao_2_5_31000.cnf
      1 0     1   20181859   30000000   78663179
      2 0     3   29320783   30000000 2403373042
      3 0     2   29566022   30000000 2265090975
      4 0     3   29515868   30000000   75192440
      5 1     0   12954554   12954554  128676484
   \endverbatim
   </li>
   <li> n = 31500; density = 19.3928253968254
   gets harder now:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 80000000 -i GreenTao_2_5_31500.cnf
      1 0     8   69795401   80000000  536637085
      2 0     3   30444816   80000000 1191026050
      3 0     6   74650775   80000000 2140749066
   \endverbatim
   </li>
   <li> But can be solved:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 160000000 -i GreenTao_2_5_31500.cnf
      1 0     4  156694733  160000000    3180540
      2 0     2   90427544  160000000  486451281
      3 0     5  137171125  160000000 2156452703
      4 1     0  128497989  128497989  345549340
   \endverbatim
   </li>
   <li> n = 32000, density = 19.6105625, looks unsatisfiable:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 50 -cutoff 100000000 -i GreenTao_2_5_32000.cnf
Clauses = 627538
Variables = 32000
TotalLiterals = 3137690
FlipsPerSecond = 78737
BestStep_Mean = 82114778.440000
Steps_Mean = 100000000.000000
Steps_Max = 100000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 7.580000
BestSolution_Median = 6.500000
BestSolution_Min = 1.000000
BestSolution_Max = 19.000000
   \endverbatim
   </li>
   <li> However re-running the best run yields a satisfying assignment:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 1 -cutoff 300000000 -i GreenTao_2_5_32000.cnf -seed 1236786762
      1 1     0  106169034  106169034 1236786762
   \endverbatim
   </li>
   <li> n = 32500, density = 19.83889230769231 still satisfiable:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 300000000 -i GreenTao_2_5_32500.cnf
      1 0     7  295919077  300000000 1113819587
      2 0     8  209899032  300000000 3804267295
      3 0    11  186387649  300000000 2937582742
      4 1     0  274647920  274647920 1925137726
   \endverbatim
   </li>
   <li> n = 32750, density = 19.94418320610687
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 300000000 -i GreenTao_2_5_32750.cnf
      1 0     6  298602469  300000000  109588602
      2 0    11  268672776  300000000 2399089340
      3 0    10  281462863  300000000 2595364500
      4 0    10  117371618  300000000 3009654463
      5 0    15  223873643  300000000  179019469
      6 0    10  289708347  300000000  481958740
      7 0    16  172316102  300000000 4009576113
      8 0     8  274867018  300000000  617951956
   \endverbatim
   </li>
   <li> Still satisfiable:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 1000000000 -i GreenTao_2_5_32750.cnf -seed 109588602
      1 0     3  708126506 1000000000  109588602
      2 1     0  732967358  732967358 4054695673
   \endverbatim
   </li>
   <li> n = 32800, density = 19.96231707317073 </li>
   <li> n = 33000, density = 20.06012121212121; now getting hard:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 300000000 -i GreenTao_2_5_33000.cnf
      1 0    24  297678494  300000000 1757893452
      2 0    21  296939047  300000000 1820921595
   \endverbatim
   </li>
   <li>
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 600000000 -i GreenTao_2_5_33000.cnf
      1 0     3  412863959  600000000 1780844303
      2 0    15  282783807  600000000 1069196708
      3 0    14  598198966  600000000  827526861
      4 0    17  335449491  600000000  360171182
   \endverbatim
   and then
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 1 -cutoff 3000000000 -i GreenTao_2_5_33000.cnf -seed 1780844303
      1 0     2 2479149232 3000000000 1780844303
   \endverbatim
   Hm; one has to run more extensive experiments (for example, running it
   with this seed and 6 * 10^9 steps on a 64-bit machine), but it might be
   unsatisfiable. </li>
   <li> BUT, also on a 64-bit machine the cutoff-value just is an unsigned 32-bit value,
   and thus can be at most 4294967295. </li>
   <li> Down to one falsified clause with seed 1782112367. </li>
   <li> n = 33000 actually is satisfiable:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 4000000000 -i GreenTao_2_5_33000.cnf
      1 0     3 1797890838 4000000000  776867833
      2 0     3 2650818254 4000000000  405532870
      3 0     4 3810517828 4000000000 2381509817
      4 0     8 2313839935 4000000000 1498125638
      5 0     1 2905850294 4000000000 3216165566
      6 1     0 3225491509 3225491509 4198934964
      7 1     0 3776261282 3776261282 3642546655
   \endverbatim
   while 20 runs with 2 * 10^9 steps didn't find a solution:
   \verbatim
ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 2000000000 -i GreenTao_2_5_33000.cnf
Clauses = 661984
Variables = 33000
TotalLiterals = 3309920
FlipsPerSecond = 85199
BestStep_Mean = 1418109273.400000
Steps_Mean = 2000000000.000000
Steps_Max = 2000000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 6.250000
BestSolution_Median = 6.000000
BestSolution_Min = 1.000000
   \endverbatim
   </li>
   <li> n = 33100, density = 20.10416918429003
   found satisfiable with seed = 734547414. </li>
   <li> n = 33200, density = 20.15144578313253
   found satisfiable with seeds 951985690, 2179554572, 871568587 (where the
   latter used 1,880,208,869 steps; bit it might be more efficient to use
   4,000,000,000 as cutoff than 2,000,000,000). </li>
   <li> n = 33300, density = 20.19633633633634
   found satisfiable with seed 1621683624 (3079471773 steps). </li>
   <li> n = 33400, density = 20.23904191616766
   found satisfiable with seed 3762888341 (3859835241 steps; 60
   runs were necessary --- it seems the cutoff of 4*10^9 is becoming
   too small, since the optimum is always reached in the high 3*10^9!).
   </li>
   <li> n = 33500, density = 20.27976119402985
   Found satisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 100 -cutoff 4000000000 -i GreenTao_2_5_33500.cnf
    15 1     0 3847168275 3847168275 2745070232
   \endverbatim
   </li>
   <li> n = 33600 (to be investigated by MG)

   </li>
  </ul>


  \todo Connecting different n
  <ul>
   <li> We should find out what the falsified clause for the above nearly
   satisfying assignment for n=33000 is --- if m is the maximum variable
   (index) in the clause then we have a satisfying assignment for n = m-1.
    <ol>
     <li> This holds in general for such monotone sequences of clause-sets.
     </li>
     <li> We should write a little C++ program, which takes the assignment
     returned by Ubcsat (output by using option "-r best") and the clause-set,
     and outputs the falsified clauses. </li>
    </ol>
   </li>
  </ul>


  \todo Phase transition
  <ul>
   <li> One should study the density of the clause-sets (and the "threshold")
   here.
    <ol>
     <li> The density 3.5 for unsatisfiable k=4 is somewhat
     similar to the random 3-SAT threshold (around 4.25 --- though for
     larger n). </li>
     <li> The density 8.8 for unsatisfiable k=4 is similar to
     the random 4-SAT threshold (around 9.8). </li>
     <li> The random 5-SAT threshold is around 20. </li>
     <li> One could guess that the unsatisfiability-density comes closer
     to the random-k-SAT threshold density? </li>
     <li> Then one needed to figure out how many k-progressions are in the
     first n primes; see "The distribution of arithmetic progression amongst
     primes" in Experimentation/Investigations/plans/AdditiveNumberTheory.hpp.
     </li>
     <li> It would be interesting to study random complement-invariant
     k-SAT clause-sets (choose a random k-clause-set, and take the union
     with the complement)! </li>
     <li> I (OK) would assume that the van-der-Waerden clause-sets are much
     more redundant than the Gree-Tao clause-sets, and that the latter are
     much closer to random clause-sets. </li>
     <li> A general conjecture is that we have the Ramsey property if the
     density goes to infinity for each fixed size k of the structure required
     to exist (arithmetic progressions, cliques, etc.; in this general
     form likely one should find a counter-example, but perhaps it holds
     if the the structures "spread a bit"). </li>
    </ol>
   </li>
  </ul>


  \todo Faster generation of arithmetic progression of primes
  <ul>
   <li> A major bottleneck is the time needed to create Green-Tao problems.
   </li>
   <li> Via local search we might even investigate greentao(2,6), but here
   n might go into the millions, and we need a much faster generator. </li>
   <li> In RamseyTheory/plans/Van_der_Waerden_hypergraph.hpp a C++ program
   should be written (that is, planned) with the functionality of
   arithprog_primes_hg. </li>
   <li> And also the sequences length(arithprog_primes_finish[k,n]) for fixed k
   and length(arithprog_primes(k,n)) for fixed k should be of interest.
    <ol>
     <li> Shall this go into a PostgreSQL database, or into a simple file,
     containing lines
     "no., prime, count of sequences ending with prime, cumulative count".
     ? The file looks alright (and can be easily expanded). </li>
     <li> We should also provide column headings, so that it can be directly
     read into R. </li>
     <li> But also Maxima should have no problems reading these files. </li>
     <li> These files need to be provided in a data section of the OKlibrary.
     </li>
    </ol>
   </li>
  </ul>


  \todo Faster local search
  <ul>
   <li> For greentao it seems the only structure which can be exploited is
   the complement-invariance.
    <ol>
     <li> More precisely, we have a hypergraph colouring problem. </li>
     <li> So we have complement-invariant clause-sets, and furthermore we
     have a PN-clause-set (so regarding space, we can save one half of the
     clauses, and the clauses need only contain positive literals, i.e.,
     variables, not literals). </li>
     <li> It seems desirable to have a specialised local search solver for
     hypergraph colouring (as in instance of generalised SAT); since local
     search solvers only use total assignments, the non-stability of
     hypergraph colouring under partial assignments is no hindrance (while
     we have stability under autarkies). </li>
     <li> The more colours are to be used, the bigger the savings. </li>
     <li> And one would assume the various heuristics are influenced by
     the restriction to hypergraph colouring. </li>
    </ol>
   </li>
   <li> For vanderwaerden there is much more structure which could be exploited
   (using "virtual" clause-sets). </li>
   <li> We should try to understand why the different local search algorithms
   behave so differently on the various problem classes.
    <ol>
     <li> See chapter 6 in [Hoos, Stuetzle, Stochastic Local Search] for
     background information on the algorithms involved. </li>
     <li> For the van der Waerden problems and the Green-Tao problems it should
     be possible to gain quite good quantitative experimental understanding.
     </li>
     <li> See chapter 4 in [Hoos, Stuetzle, Stochastic Local Search] for
     material on statistical evaluation. </li>
    </ol>
   </li>
   <li> "Meta-heuristics":
    <ol>
     <li> General meta-heuristics are needed, which can be adapted to the
     specific problems. </li>
     <li> A natural first example would be first to identify the best solver
     from the suite, then trying to optimise it, and then search for solutions
     by starting with, say (just an example) 1000 seeds, running them a bit,
     filtering out the 100 most promising ones, running them further, filtering
     out the 10 best, running them, finally filtering out the best one (or more
     --- depending on the number of processes to be run!). </li>
     <li> Of course, this all automatic (with good monitoring). </li>
     <li> One needs to gain quantitative understanding of the local search
     process, so that progress can be evaluated; see above. </li>
     <li> All algorithms and programs are written in a natural generative
     style, but specific to the problem set (van der Waerden and Green-Tao
     problems here --- even them treated individually). </li>
     <li> Perhaps the whole thing is written in R first, using Ubcsat; see
     ExperimentSystem/plans/RunUBCSAT.hpp. </li>
     <li> And (of course) also at the Maxima/Lisp level, this time using
     Maxima/Lisp local search algorithms; see
     ComputerAlgebra/Satisfiability/Lisp/LocalSearch/plans/general.hpp. </li>
    </ol>
   </li>
  </ul>


  \todo Survey propagation
  <ul>
   <li> If the Green-Tao problems are similar to random problems, then
   perhaps survey propagation is also successful on them ?! </li>
  </ul>


  \todo Literature
  <ul>
   <li> To search for literature, we can search on the Internet for the
   sequence (1,3,23,512) (greentao(2,i) for i=1,2,3,4). </li>
   <li> Likely this sequence is not in that Internet database, and
   we should submit it (once our article has appeared; or perhaps
   the report is enough). </li>
  </ul>

*/

