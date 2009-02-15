// Oliver Kullmann, 3.2.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_2-5-5.hpp
  \brief Investigations on greentao_2(5)


  \todo greentao_2(5) > 33500
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
  </ul>


  \todo Survey propagation: greentao_2(5) > 34000
  <ul>
   <li> Running it on GreenTao_2_5_33500.cnf:
   \verbatim
builds/SAT/SurveyPropagation/sp-1.4> ./sp -l GreenTao_2_5_33500.cnf
.................................:-)
<bias>:0.000000
paramagnetic state
sub-formula has:
        10479 2-clauses
        34136 3-clauses
        51732 4-clauses
        49904 5-clauses
        18548 variables
calling walksat on the sub-formula (see output in wsat.tmp.out)
allocating memory...
ASSIGNMENT FOUND
WSAT did find the solution of the sub-formula
merging spsol.tmp.lst over wsatsol.tmp.lst
verifying solution solution.tmp.lst...33500 variables found
679372 clauses sat, 0 unsat (out of 679372 read)
   \endverbatim
   </li>
   <li> So here, different from GreenTao_2_4_5_4100.cnf (see
   Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_2-4-5.hpp
   ), SP achieves something. </li>
   <li> Could be that the diagonal problems are closer to random (uniform)
   CNF (or perhaps the size --- it's big enough?). </li>
   <li> GreenTao_2-5-5_33600: Again successful!
   \verbatim
<bias>:0.000000
paramagnetic state
sub-formula has:
        10639 2-clauses
        34383 3-clauses
        51428 4-clauses
        47824 5-clauses
        18528 variables
calling walksat on the sub-formula (see output in wsat.tmp.out)
allocating memory...
ASSIGNMENT FOUND
WSAT did find the solution of the sub-formula
merging spsol.tmp.lst over wsatsol.tmp.lst
verifying solution solution.tmp.lst...33600 variables found
682872 clauses sat, 0 unsat (out of 682872 read)
   \endverbatim
   </li>
   <li> GreenTao_2-5-5_33800 found satisfiable:
   \verbatim
<bias>:0.000000
paramagnetic state
sub-formula has:
        11504 2-clauses
        31414 3-clauses
        44208 4-clauses
        38888 5-clauses
        17237 variables
calling walksat on the sub-formula (see output in wsat.tmp.out)
allocating memory...
ASSIGNMENT FOUND
WSAT did find the solution of the sub-formula
merging spsol.tmp.lst over wsatsol.tmp.lst
verifying solution solution.tmp.lst...33800 variables found
690062 clauses sat, 0 unsat (out of 690062 read)
   \endverbatim
   </li>
   li> GreenTao_2-5-5_34000 found satisfiable:
   \verbatim
<bias>:0.000000
paramagnetic state
sub-formula has:
        11843 2-clauses
        30512 3-clauses
        40129 4-clauses
        33516 5-clauses
        16729 variables
calling walksat on the sub-formula (see output in wsat.tmp.out)
allocating memory...
ASSIGNMENT FOUND
WSAT did find the solution of the sub-formula
merging spsol.tmp.lst over wsatsol.tmp.lst
verifying solution solution.tmp.lst...34000 variables found
697154 clauses sat, 0 unsat (out of 697154 read)
   \endverbatim
   </li>
  </ul>

*/

