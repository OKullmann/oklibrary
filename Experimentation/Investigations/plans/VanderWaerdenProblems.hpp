// Oliver Kullmann, 20.9.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/VanderWaerdenProblems.hpp
  \brief On investigations into van-der-Waerden-like problems


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/general.hpp. </li>
   <li> See VanderWaerden.cpp (to be updated) for a C++ generator. </li>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/plans/VanderWaerdenProblems.hpp.
   </li>
  </ul>


  \todo Van der Waerden numbers
  <ul>
   <li> Via
   \verbatim
VanderWaerden-O3-DNDEBUG k n > VanderWaerden_2_k_n.cnf
   \endverbatim
   boolean problems can be generated quickly. </li>
   <li> In Maxima we have "output_vanderwaerden2_stdname(k,n)" (in
   ComputerAlgebra/Satisfiability/Lisp/Generators/VanderWaerdenProblems.mac. </li>
   <li> vanderwaerden(2,3) = 9 (density = 3.6) </li>
   <li> vanderwaerden(2,4) = 35 (density = 10.7) </li>
   <li> vanderwaerden(2,5) = 178 (density = 43.5)
    <ol>
     <li> Relatively easy for OKsolver-2002 (710s, 109703 nodes). </li>
     <li> march_pl needs only 51529 nodes and 238 seconds, so the "double
     look-ahead" seems to be successful here. </li>
     <li> minisat needs 213 seconds, so also clause-learning seems applicable
     here. </li>
    </ol>
   </li>
   <li> vanderwaerden(2,6) = 1132 (density = 225.6) </li>
    <ol>
     <li> n = 1000
      <ul>
       <li> ubcsat-irots seems best. </li>
       <li> But finding a solution seems hopeless; less than 1400 falsified
       clauses seems to require billions of steps --- the difference between
       100,000 and 10,000,000 steps is very small. </li>
       <li> We need to look into the Heule-et-al article. </li>
      </ul>
     </li>
     <li> n = 1132
      <ol>
       <li> OKsolver-2002
       \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D30 VanderWaerden_2_6_1132.cnf
       \endverbatim
       looks hopeless (no node of depth 30 solved in one hour). </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo Symmetry breaking for van der Waerden numbers
  <ul>
   <li> As for any hypergraph colouring problems, one can choose any variable
   and set it to true (or to false), due to the symmetry between 0 and 1. </li>
   <li> This is best "told" the solver, so that it can use this for the
   branching variable at the root. </li>
   <li> No other symmetry breaking seems possible without conditioning. </li>
   <li> Is there not also, at least, the symmetry about the number line? i.e. 
   if there is a colouring of the numbers such that there is no arithmetic 
   progression of size k, then reversing the colouring should also have this 
   property. </li>  
   <li> For small problems one needs to determine the full automorphism group
   of the clause-sets. </li>
  </ul>


  \todo Overview on performance of SAT solvers
  <ul>
   <li> We need an overview how SAT solvers perform on the different van der
   Waerden problems. </li>
  </ul>


  \todo Green-Tao numbers
  <ul>
   <li> greentao(2,3)
    <ol>
     <li> greentao(2,3) = 23 (partitioning the first 23 prime numbers into 2
     parts, one part is guaranteed to contain an arithmetic progression of
     size 3, while using a smaller initial segment of prime numbers won't do).
     </li>
     <li> Trivial (for OKsolver-2002, and likely for any solver). </li>
    </ol>
   </li>
   <li> greentao(2,4)
    <ol>
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
     <li> DONE (the basic algorithm works not too bad now)
     For n in this magnitude the Maxima computation of the hypergraph is
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
    </ol>
   </li>
   <li> greentao(2,5)
    <ol>
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
     (within hours):
     \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D16 GreenTao_2_5_25000.cnf
c sat_status=2 initial_maximal_clause_length=5 initial_number_of_variables=24998 initial_number_of_clauses=410070 initial_number_of_literal_occurrences=2050350 running_time(s)=892.7 number_of_nodes=26511 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=555647 number_of_pure_literals=1349 number_of_autarkies=0 number_of_missed_single_nodes=2 max_tree_depth=602 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=37760 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=GreenTao_2_5_25000.cnf
     \endverbatim
     </li>
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
bcsat-okl -alg rnovelty+ -runs 20 -cutoff 2000000000 -i GreenTao_2_5_33000.cnf
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
     <li> n = 33200, density = </li>
     <li> n = 33500, density = 20.27976119402985
     \verbatim
ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 1000000000 -i GreenTao_2_5_33500.cnf
Clauses = 679372
Variables = 33500
TotalLiterals = 3396860
FlipsPerSecond = 69292
BestStep_Mean = 789596735.700000
Steps_Mean = 1000000000.000000
Steps_Max = 1000000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 20.600000
BestSolution_Median = 19.000000
BestSolution_Min = 13.000000
BestSolution_Max = 32.000000
     \endverbatim
     Best seeds: 3438409963, 1267451597. </li>
     <li> This is likely not the optimum, but I would be surprised if these
     instances would still be satisfiable. </li>
    </ol>
   </li>
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
   <li> One should study the density of the clause-sets (and the "threshold")
   here.
    <ol>
     <li> The density 3.5 for unsatisfiable k=4 is somewhat
     similar to the random 3-SAT threshold (around 4.25 --- though for
     larger n). </li>
     <li> The (assumed) density 8.8 for unsatisfiable k=4 is similar to
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
   <li> One can also consider non-diagonal Green-Tao numbers. </li>
   <li> One needs to investigate whether bounds are known. </li>
  </ul>


  \todo Faster generation of arithmetic progression of primes
  <ul>
   <li> A major bottleneck is the time needed to create Green-Tao problems.
   </li>
   <li> Via local search we might even investigate greentao(2,6), but here
   n might go into the millions, and we need a much faster generator. </li>
   <li> In Transitional/Structures/NumberTheory a C++ program should be
   written with the functionality of arithprog_primes.
    <ol>
     <li> Perhaps we only output this hypergraph as a positive clause-set,
     in DIMACS format, into a file. </li>
     <li> Some components should go to Transitional/Combinatorics/Hypergraphs.
     </li>
     <li> Another program then standardises the variable names and adds the
     complemented clauses (producing then the same output as yet
     output_greentao2_stdname(k,n)). </li>
     <li> Such functionality shall likely go to
     Transitional/Satisfiability/Transformers. </li>
     <li> Best we provide this functionality as Unix tools as well as at
     library level. </li>
    </ol>
   </li>
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
   <li> Enumerating the primes:
    <ol>
     <li> Gmp has a "next_prime" function, by which we first create the complete
     list of primes and the corresponding boolean array (for the primality
     predicate). </li>
     <li> Optionally, if one of the randomised tests is not secure, then the
     whole computation is checked via a simple sieve of Erathostenes. </li>
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

*/

