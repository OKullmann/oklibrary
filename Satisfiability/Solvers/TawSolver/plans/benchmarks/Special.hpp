// Oliver Kullmann, 30.1.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/benchmarks/Special.hpp
  \brief Special benchmarks


  \todo Concrete instances
  <ul>
   <li> SAT: VanDerWaerden_2-3-12_135.cnf </li>
   <li> SAT: VanDerWaerden_pd_2-3-21_405.cnf </li>
   <li> #SAT: QueensProblem_015.cnf </li>
   <li> We need statistics on these instances (where (c)tawSolver seems
   competitive) for all solver-variants. </li>
  </ul>


  \todo Random instances for SAT
  <ul>
   <li> BRG "1350*320,3" "" seed (less than a half instances are hard for
        minisat) </li>
   <li> Additionally "1550*350,3"; this is now to the right of the threshold.
   </li>
   <li> BRG "1250*130,4" "" seed (more that a half instances are hard for
        minisat) </li>
   <li> Additionally "1500*150,4" (seems to be to the right of the threshold).
   </li>
   <li> BRG "1700*80,5" "" seed (the vast majority of instances are hard for
        minisat) </li>
   <li> Additionally "2000*90,5" (seems to be to the right of the threshold).
   </li>
   <li> An assumption here is that the higher k (the clause-length) the
        relatively weaker the heuristics, and thus the difference to
        ftawSolver should become less.
    <ol>
     <li> E.g., for k=6:
            BRG "3000*50,6"
          seems to yield very stable runtime of tawSolver (all unsatisfiable),
          and ftawSolver seems by a factor 6 worse (runtime).
     </li>
     <li> E.g., for k=7:
            BRG "30000*55,7"
          seems to yield again very stable runtime of tawSolver
          (all unsatisfiable), and ftawSolver seems by a factor 3 worse
          (runtime).
     </li>
     <li> With
            BRG "300000*50,8"
          it seems that ftawSolver finally might be faster.
     </li>
    </ol>
   </li>
  </ul>


  \todo Random instances for #SAT
  <ul>
   <li> BRG "110*110,2" "" seed --- these instances are all easy for
        sharpSAT, while tawSolver does not have a proper heuristics for it,
        and thus we don't consider them here. </li>
    <ol>
     <li> A hard instance here is
     \verbatim
> BRG "110*110,2" "" 12 | ./ctawSolver_lambda -cin
s SATISFIABLE
c program_name                          ctawSolver_lambda
c   version_number                      2.16.1
c   options                             "A19L100"
c   weights                             4.8499999999999996,1,0.35399999999999998,0.11,0.069400000000000003;1.46
c file_name                             -cin
c   p_param_variables                   103
c   p_param_clauses                     110
c   number_tautologies                  0
c   max_occurring_variable              103
c   number_of_clauses                   110
c   maximal_clause_length               2
c   number_of_literal_occurrences       220
c running_time(sec)                     262.9728
c   system_time                         0.0000
c   elapsed_wall_clock                  262.9753
c   max_memory(MB)                      4.4687
c number_of_nodes                       3333611519
c   number_of_binary_nodes              1666805759
c   number_of_single_child_nodes        0
c   number_of_leaves                    1666805760
c   number_of_internal_nodes            1666805759
c   inodes_per_second                   6.3383e+06
c number_of_1-reductions                1666924197
c   1-reductions_per_second             6.3388e+06
c   1-reductions_per_node               0.5000
c lambda                                100
c number_all_pure_nodes                 0
c number_of_solutions                   853451399678883840
c reading-and-set-up_time(sec)          0.000258
c   system_time                         0.000000
c   elapsed_wall_clock                  0.001134

> BRG "110*110,2" "" 12 | ./ctawSolver -cin
s SATISFIABLE
c program_name                          ctawSolver
c   version_number                      2.16.1
c   options                             "A19"
c   weights                             4.8499999999999996,1,0.35399999999999998,0.11,0.069400000000000003;1.46
c file_name                             -cin
c   p_param_variables                   103
c   p_param_clauses                     110
c   number_tautologies                  0
c   max_occurring_variable              103
c   number_of_clauses                   110
c   maximal_clause_length               2
c   number_of_literal_occurrences       220
c running_time(sec)                     293.1820
c   system_time                         0.0040
c   elapsed_wall_clock                  293.2096
c   max_memory(MB)                      4.4687
c number_of_nodes                       4032921599
c   number_of_binary_nodes              2016460799
c   number_of_single_child_nodes        0
c   number_of_leaves                    2016460800
c   number_of_internal_nodes            2016460799
c   inodes_per_second                   6.8778e+06
c number_of_1-reductions                2016635547
c   1-reductions_per_second             6.8784e+06
c   1-reductions_per_node               0.5000
c number_all_pure_nodes                 2016455040
c number_of_solutions                   853451399678883840
c reading-and-set-up_time(sec)          0.000258
c   system_time                         0.000000
c   elapsed_wall_clock                  0.001115

> BRG "110*110,2" "" 12 | ./cttawSolver_lambda -cin
s SATISFIABLE
c program_name                          cttawSolver_lambda
c   version_number                      2.16.1
c   options                             "TA19L100"
c   weights                             7,1,0.31,0.19,0.11176470588235295;1.7
c file_name                             -cin
c   p_param_variables                   103
c   p_param_clauses                     110
c   number_tautologies                  0
c   max_occurring_variable              103
c   number_of_clauses                   110
c   maximal_clause_length               2
c   number_of_literal_occurrences       220
c running_time(sec)                     707.4265
c   system_time                         0.0023
c   elapsed_wall_clock                  707.4563
c   max_memory(MB)                      4.4687
c number_of_nodes                       3276241919
c   number_of_binary_nodes              1638120959
c   number_of_single_child_nodes        0
c   number_of_leaves                    1638120960
c   number_of_internal_nodes            1638120959
c   inodes_per_second                   2.3156e+06
c number_of_1-reductions                1638243495
c   1-reductions_per_second             2.3158e+06
c   1-reductions_per_node               0.5000
c number_wtau_calls                     1638237796
c   number_wtau_calls_per_inode         1.0001
c number_tau_iterations                 6552951228
c   average_tau_iterations              4.0000
c lambda                                100
c number_all_pure_nodes                 0
c number_of_solutions                   853451399678883840
c reading-and-set-up_time(sec)          0.000000
c   system_time                         0.000271
c   elapsed_wall_clock                  0.001193

r> BRG "110*110,2" "" 12 | ./cttawSolver -cin
s SATISFIABLE
c program_name                          cttawSolver
c   version_number                      2.16.1
c   options                             "TA19"
c   weights                             7,1,0.31,0.19,0.11176470588235295;1.7
c file_name                             -cin
c   p_param_variables                   103
c   p_param_clauses                     110
c   number_tautologies                  0
c   max_occurring_variable              103
c   number_of_clauses                   110
c   maximal_clause_length               2
c   number_of_literal_occurrences       220
c running_time(sec)                     277.3816
c   system_time                         0.0000
c   elapsed_wall_clock                  277.3833
c   max_memory(MB)                      4.4687
c number_of_nodes                       3789711359
c   number_of_binary_nodes              1894855679
c   number_of_single_child_nodes        0
c   number_of_leaves                    1894855680
c   number_of_internal_nodes            1894855679
c   inodes_per_second                   6.8312e+06
c number_of_1-reductions                1895030791
c   1-reductions_per_second             6.8319e+06
c   1-reductions_per_node               0.5000
c number_wtau_calls                     299
c   number_wtau_calls_per_inode         0.0000
c number_tau_iterations                 1196
c   average_tau_iterations              4.0000
c number_all_pure_nodes                 1894849152
c number_of_solutions                   853451399678883840
c reading-and-set-up_time(sec)          0.000314
c   system_time                         0.000000
c   elapsed_wall_clock                  0.001146
     \endverbatim
     </li>
    </ol>
   </li>
   <li> BRG "820*200,3" "" seed (seem still relatively easy for sharpSAT) </li>
   <li> BRG "920*110,4" "" seed (seem hard for sharpSAT) </li>
   <li> BRG "800*60,5" "" seed (seem hard for sharpSAT) </li>
  </ul>


  \todo Plot alpha and lambda
  <ul>
   <li> For lambda the range
         1,2,...,100,200,...,1000
        looks reasonable at first.
   </li>
   <li> For alpha the range
         0.01, ..., 0.99
        should be feasible.
   </li>
   <li> Runtimes for now are not important (and thus the workload and machine
        isn't important. </li>
   <li> So for the single instances just a single run. </li>
   <li> And for the random instances we use the number R of for each instance-
        type (that is, using seeds=0,...,R-1); say R=100. </li>
   <li> This for the current version with alpha and/or lambda. </li>
   <li> For comparison also running the corresponding standard-versions
        (without alpha and/or lambda) . </li>
   <li> Using the current experiment-system, with all instances created in
        advances. </li>
   <li> All generated BRG instances are on the server in
        /data/benchmarks/SAT/BRG/ . </li>
   <li> BRG version 0.4.8 was used in all cases.
   <li> For each of three SAT families, 500 unsatisfiable instances were selected
        for experiments. For this purpose 2000, 7000, and 1200 instances were
        generated for 1350*320,3, 1250*130,4, and 1700*80,5, respectively.
        Then the first 500 unsatisfiable ones were chosen among each set.
   <li> The unsatisfiable instances are in subdirectories /unsat_1350-320-3/ ,
        /unsat_1250-130-4/ , /unsat_1700-80-5/ </li>
   <li> The best mean number of nodes (meannds) among tawSolver_alpha are:
        family         meannds    alpha
        BRG-1350-320-3 3787967    0.24
        BRG-1250-130-4 6336224    0.2
        BRG-1700-80-5  3051005    0.07
   </li>
   <li> Mean number of nodes for tawSolver and ttawSolver are:
        family         tawSolver ttawSolver
        BRG-1350-320-3 3804076   3529716
        BRG-1250-130-4 6352402   6405030
        BRG-1700-80-5  3051803   3226562
   </li>
   <li> On BRG-1350-320-3, ttawSolver is about 7% better than tawSolver,
        while on both BRG-1250-130-4 and BRG-1700-80-5 it is slightly worse.
   </li>
   <li> 3000 benchmarks were generated for each of three BRG families:
        1550*350,3, 1500*150,4, and 2000*90,5.
        The seeds 1,...,3000 were used for this purpose.
   </li>
   <li> The following table contains mean number of nodes for tawSolver
        (product-projection), ttawSolver (tau-projection), tawSolver_alpha1.0
        (sum-projection), and the best tawSolver_alpha* for each family.
        Here tawS stands for tawSolver, ttawS for ttawS, tawS_a for
        tawSolver_alpha.
        family         tawS     ttawS    tawS_a0.1 tawS_a0.22 tawS_a0.26 tawS_a1.0
        BRG-1550-350-3 5815767  5252542  -         -          5784030    6939636
        BRG-1500-150-4 24507069 24518512 -         24363194   -          -
        BRG-2000-90-5  10271021 10889953 10262091  -          -          -
   </li>
   <li> On BRG-1550-350-3, the mean number of nodes on ttawSolver is almost
        10% lower (better) than that on tawSolver. On both BRG-1500-150-4 and
        BRG-2000-90-5, ttawSolver is slighlty worse.
   </li>
   <li> For each of three #SAT famlies 500 instances were genereted by the
        following bash script (so, seeds 1..500 were used):
   \verbatim
#!/bin/bash
for i in {1..500};
do
    BRG "820*200,3" "" ${i} > BRG-820-200-3_${i}.cnf
    BRG "920*110,4" "" ${i} > BRG-920-110-4_${i}.cnf
    BRG "800*60,5" "" ${i} > BRG-800-60-5_${i}.cnf
done
   \endverbatim
   </li>
   <li> The generated #SAT instances are in subdirectories /820-200-3/ ,
        /920-110-4/ , /800-60-5/ . Both satisfiable and unsatisfiable instances
        occur there, no selection was made in this case. </li>
  </ul>
*/


