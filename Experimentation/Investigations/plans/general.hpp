// Oliver Kullmann, 14.2.2008 (Swansea)
/* Copyright 2008, 2011, 2012, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/general.hpp
  \brief Plans for the super-module for collecting data (not tools, but
  actual data)


  \todo Structure
  <ul>
   <li> DONE (well-established now)
   Is the name "Investigations" right? </li>
   <li> DONE (well-established now)
   Is the part Experimentation the right part? </li>
   <li> How shall we present the data? </li>
  </ul>


  \todo Rerun time-sensitive experiments WHAT IS THE STATUS OF THIS?
  <ul>
   <li> On older versions of the 32-bit linux-kernel, <= 2.6.35.6, there is
   bug which causes "user time" to be misreported as "system time" for CPU
   intensive processes which take more than ~40s. See
   https://bugzilla.kernel.org/show_bug.cgi?id=16559 and
   https://bugzilla.redhat.com/show_bug.cgi?id=633037 . </li>
   <li> As most SAT solvers report the "user time" from the "getrusage"
   function in the GNU C libraries, this results in solvers reporting
   the wrong time duration for solving. </li>
   <li> This bug was evident and caused wrong solver times to be
   reported with some AES experiments; see commit
   dec0020519c4053f1b421e059d6bcb94ab753a67. </li>
   <li> Non-trivial experiments, where time is an important consideration
   should be rerun:
    <ul>
     <li> All in Cryptography/AdvancedEncryptionStandard/plans/. </li>
     <li> All in Cryptography/DataEncryptionStandard/plans/. </li>
     <li> This list must be expanded. </li>
    </ul>
   </li>
   <li> Safety measures:
    <ol>
     <li> Time sensitive experiments could always be run with the
     "time" shell command to ensure that the solver reports the correct
     time. </li>
     <li> However "time" itself is an arcane tool, which disturbs the programs
     it shall observe. </li>
     <li> See
     http://stackoverflow.com/questions/7056700/what-is-an-elegant-way-that-a-shell-script-times-itself-really/
     for some discussion and an alternative proposed by OK. </li>
     <li> So it seems best to only make probes, for new systems and new
     solvers, and also sporadically for all experiment series. </li>
    </ol>
   </li>
   <li> We also need documentation on issues such as this, and "best practices"
   for experimentation. </li>
   <li> See also "Experiment scripts". </li>
  </ul>


  \todo Experiment scripts
  <ul>
   <li> We should provide docus containing a list of all experiment scripts
   used in the library. </li>
   <li> In this way, when creating new experiment scripts one knows where to
   look for examples. </li>
   <li> If the list is presented chronologically, one can also see the
   more recent examples of experiment scripts for more "up-to-date" practices.
   </li>
   <li> In such docus, there is also scope for discussion of best practices
   and requirements for experiment scripts (experiment directories, uniqueness
   of directory name via "date", reproducability etc) and how to achieve
   these. </li>
   <li> For now we have:
   \verbatim
find ./ -iname "*" -exec file \{} \; | grep Bourne-Again
<edited>
./Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/RunPdVdWk1k2: Bourne-Again shell script text executable
./Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/RunVdW3k: Bourne-Again shell script text executable
./Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/CRunPdVdWk1k2: Bourne-Again shell script text executable
./Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/RunVdWk1k2: Bourne-Again shell script text executable
./Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/run_all_solvers_2by2: Bourne-Again shell script text executable
./Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/run_aes_experiment: Bourne-Again shell script text executable
./Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/run_all_solvers: Bourne-Again shell script text executable
./Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/generate_aes_experiment: Bourne-Again shell script text executable
./Experimentation/Investigations/BooleanFunctions/analyse_random_inv_linear_maps: Bourne-Again shell script text executable
./Experimentation/Investigations/BooleanFunctions/analyse_random_boolean_functions: Bourne-Again shell script text executable
./Experimentation/Investigations/BooleanFunctions/analyse_random_permutations: Bourne-Again shell script text executable
./Satisfiability/Transformers/Generators/VdWTransversals: Bourne-Again shell script text executable
./Satisfiability/Transformers/Generators/VdWTransversalsInc: Bourne-Again shell script text executable
./Satisfiability/Transformers/Generators/VdWTransversalsPB: Bourne-Again shell script text executable
./Satisfiability/Transformers/Generators/GTdSat: Bourne-Again shell script text executable
./Satisfiability/Transformers/Generators/GTSat: Bourne-Again shell script text executable
./Satisfiability/Transformers/Generators/VdWTransversalsIncPB: Bourne-Again shell script text executable
./Satisfiability/Transformers/Generators/GTTransversals: Bourne-Again shell script text executable
./Satisfiability/Transformers/Generators/GTTransversalsInc: Bourne-Again shell script text executable
./Satisfiability/Reductions/Bases/RandomRUcpBases: Bourne-Again shell script text executable
   \endverbatim
   </li>
  </ul>


  \todo Sat-probability-approximations
  <ul>
   <li> Likely we should have a dedicated sub-module. </li>
   <li> Study the sequence of sat-probability-approximations (as computed by
   satprob_seqap in
   ComputerAlgebra/Satisfiability/Lisp/Counting/InclusionExclusion.mac). </li>
   <li> Clearly strict upper and lower bounds alternate, until the final
   entry is correct. </li>
   <li> One could conjecture that the distance to the correct value first
   strictly increases, and then strictly decreases.
    <ol>
     <li> Verified with weak_php(m+1,m) for 0 <= m <= 3. </li>
    </ol>
   </li>
   <li> Perhaps even stronger statements hold?!? </li>
  </ul>


  \todo Experimental investigations on heuristics
  <ul>
   <li> Above we have precise data on families of problem instances. </li>
   <li> Is now this supermodule also the right place for collecting
   data about computational experiments? </li>
   <li> Finally, a database is needed (like SatEx), and this should go
   somewhere else. </li>
   <li> Taking [Hooker, Vinay, 1995, Branching Rules for Satisfiability]
   as a starting point, we should completely update the data, using
   new heuristics and benchmarks (and also various evaluation techniques).
   But we should stay within the lookahead-paradigm, but perhaps generalised
   on the strength of reduction and look-ahead. </li>
  </ul>


  \todo Add section on AES experiments to be run WHAT IS THE STATUS OF THIS?
  SHOULD BE COMPLETED BY NOW!
  <ul>
   <li> Experiments need to be run on AES instances with different types
   of SAT algorithm (local search, dpll etc) to see how things stand
   with the AES translation (See
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp).
   </li>
   <li> Also experiments with the following need to be run :
    <ul>
     <li> Variants with less rounds. </li>
     <li> AES variants with different Sboxes. </li>
     <li> Analysis of good heuristics for AES (See "Experimental
     investigations on heuristics") . </li>
    </ul>
   </li>
  </ul>


  \todo Translations from CSP to SAT
  <ul>
   <li> For the order-encoding and variations, especially for arithmetical
   constraints, see [Tamura, Taga, Kitagawa, Banbara, Compiling finite linear
   {CSP} into {SAT}, Constraints, 2009, 14(2):254-272, doi =
   10.1007/s10601-008-9061-0Open Access]. </li>
   <li> There is also the "compact order encoding" by the same people. </li>
   <li> See also "Sugar" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp. </li>
  </ul>


  \todo SplittingViaOKsolver for random clause-sets
  <ul>
   <li> n=400, density 4.26:
   \verbatim
% output_random_fcl_stdname(400,3,round(4.26*400));

kullmann-0:OKplatform> OKsolver_2002-O3-DNDEBUG Random_400_3_1704.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         3
c initial_number_of_variables           400
c initial_number_of_clauses             1704
c initial_number_of_literal_occurrences 5112
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     231.3
c number_of_nodes                       687161
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                5625604
c number_of_pure_literals               127901
c number_of_autarkies                   0
c number_of_missed_single_nodes         9
c max_tree_depth                        38
c number_of_table_enlargements          0
c number_of_1-autarkies                 5283526
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Random_400_3_1704.cnf

> SplittingViaOKsolver -D10 Random_400_3_1704.cnf
10 11 12 13
 8 12  7  1

> SplittingViaOKsolver -D20 Random_400_3_1704.cnf
 20  21  22  23  24  25  26  27  28  29  30  36  38
127 108  88  58  14  19   4   2   5   3   2   1   1

> SplittingViaOKsolver -D30 Random_400_3_1704.cnf
 30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  49  50
725 722 634 494 327 226 137  81  52  42  22  14  13   5   3  11   7   6   2   1
 51  55  56  58  59
  2   1   1   1   2

> SplittingViaOKsolver -D40 Random_400_3_1704.cnf
  40   41   42   43   44   45   46   47   48   49   50   51   52   53   54   55
2637 2835 2588 2104 1715 1259  949  740  507  419  309  216  191  133  139   97
  56   57   58   59   60   61   62   63   64   65   66   67   68   69   70   71
  69   60   51   39   24   23   18   11   19    9    8   12   11    6    7    1
  72   73   74   75   77   79   81   85
   3    3    1    2    3    1    1    1

> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
17221: 0.25h, sum-cfs=1.125007e+07, mean-t=0.053s, mean-cfs=653
# time unusable
> summary(E$cfs)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    5.0   221.0   413.0   653.3   790.0 12490.0
   \endverbatim
   </li>
   <li> n=500, density 4.26:
   \verbatim
% output_random_fcl_ds_stdname(500,3,4.27,1);
> SplittingViaOKsolver -D40 Random_500_3_2130.cnf
XXX
> Md5sum
53b4799c8ee6cd059ececb4fc29fbac9
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   40.0    41.0    42.0    42.9    44.0    94.0
> table(E$n)
   40    41    42    43    44    45    46    47    48    49    50    51    52
10892 11432  9935  7531  5578  3848  2584  1770  1130   840   571   387   293
   53    54    55    56    57    58    59    60    61    62    63    64    65
  188   163   103   100    61    53    38    26    28    18    10    14     9
   66    67    68    69    70    71    72    73    74    75    76    77    79
    5     4     6     2     2     1     4     3     1     1     1     1     1
   81    88    94
    1     1     1
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         3
c initial_number_of_variables           500
c initial_number_of_clauses             2130
c initial_number_of_literal_occurrences 6390
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     47.6
c number_of_nodes                       115313
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                5407
c number_of_pure_literals               17443
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        25
c number_of_table_enlargements          0
c number_of_1-autarkies                 1817394
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             Random_500_3_2130.cnf
c splitting_directory                   SplitViaOKsolver_D40Random_500_3_2130cnf_2011-06-21-11244
1/Instances
c splitting_cases                       57637

> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")

   \endverbatim
   </li>
  </ul>


  \todo Order principles
  <ul>
   <li> output_ordergt_stdname(m) produces OrderGT-m.cnf. </li>
   <li> These clause-sets are all unsatisfiable, have short resolution
   refutations, but no short tree-resolution refutations. </li>
   <li> OKsolver-2002:
    <ol>
     <li> m=10:
     \verbatim
c initial_maximal_clause_length         9
c initial_number_of_variables           90
c initial_number_of_clauses             775
c initial_number_of_literal_occurrences 2340
c number_of_2-clauses_after_reduction   45
c running_time(sec)                     2.3
c number_of_nodes                       46142
c number_of_single_nodes                1477
c number_of_quasi_single_nodes          0
c number_of_2-reductions                119320
c number_of_pure_literals               8343
c number_of_autarkies                   772
c number_of_missed_single_nodes         1910
c max_tree_depth                        31
c proportion_searched                   9.200725e-01
c proportion_single                     7.992752e-02
     \endverbatim
     </li>
     <li> m=11:
     \verbatim
c initial_maximal_clause_length         10
c initial_number_of_variables           110
c initial_number_of_clauses             1056
c initial_number_of_literal_occurrences 3190
c number_of_2-clauses_after_reduction   55
c running_time(sec)                     29.2
c number_of_nodes                       461306
c number_of_single_nodes                20523
c number_of_quasi_single_nodes          1
c number_of_2-reductions                1235944
c number_of_pure_literals               139858
c number_of_autarkies                   13347
c number_of_missed_single_nodes         24245
c max_tree_depth                        44
c proportion_searched                   8.692898e-01
c proportion_single                     1.307102e-01
     \endverbatim
     </li>
     <li> m=12:
     \verbatim
c initial_maximal_clause_length         11
c initial_number_of_variables           132
c initial_number_of_clauses             1398
c initial_number_of_literal_occurrences 4224
c number_of_2-clauses_after_reduction   66
c running_time(sec)                     396.6
c number_of_nodes                       5555817
c number_of_single_nodes                302746
c number_of_quasi_single_nodes          8
c number_of_2-reductions                15253349
c number_of_pure_literals               2205452
c number_of_autarkies                   215859
c number_of_missed_single_nodes         342274
c max_tree_depth                        56
c proportion_searched                   7.947487e-01
c proportion_single                     2.052513e-01
     \endverbatim
     </li>
    </ol>
   </li>
   <li> march_pl:
   \verbatim
> march_pl OrderGT-10.cnf
c resolvent_look() :: found 810 resolvents
c main():: nodeCount: 13324
c main():: time=0.990000

> march_pl OrderGT-11.cnf
c resolvent_look() :: found 1100 resolvents
c main():: nodeCount: 170088
c main():: time=14.700000

> march_pl OrderGT-12.cnf
c main():: nodeCount: 2945927
c main():: time=284.359985
   \endverbatim
   </li>
   <li> satz-215:
   \verbatim
satz215 OrderGT-10.cnf 1.290 36170 18326 1602857 66965 0 90 775 0 255285 18491
satz215 OrderGT-11.cnf 19.480 523764 264805 23945248 763461 0 110 1056 0 2437085 144139
satz215 OrderGT-12.cnf 337.310 8520254 4299339 413603484 10534914 0 132 1398 0 23077013 1179014
   \endverbatim
   </li>
   <li> Minisat-2.2.0:
   \verbatim
> minisat-2.2.0 OrderGT-12.cnf
conflicts             : 3899           (169551 /sec)
CPU time              : 0.022996 s

> minisat-2.2.0 OrderGT-15.cnf
conflicts             : 35953          (123146 /sec)
CPU time              : 0.291955 s

> minisat-2.2.0 OrderGT-20.cnf
conflicts             : 1408470        (73061 /sec)
CPU time              : 19.2781 s
   \endverbatim
   </li>
   <li> glucose-1.0:
   \verbatim
> glucose-1.0 OrderGT-20.cnf
c conflicts             : 680760         (25776 /sec)
c CPU time              : 26.411 s

> time glucose-1.0 OrderGT-25.cnf
c conflicts             : 16226367       (8925 /sec)
c CPU time              : 1817.99 s
   \endverbatim
   </li>
   <li> glucose-2.0:
   \verbatim
> glucose-2.0 OrderGT-20.cnf
c conflicts             : 719578         (43031 /sec)
c CPU time              : 16.7225 s

> time glucose-2.0 OrderGT-25.cnf
c conflicts             : 20579648       (15815 /sec)
c CPU time              : 1301.24 s
   \endverbatim
   </li>
   <li> picosat-913:
   \verbatim
> picosat913 OrderGT-20.cnf
c 700598 conflicts
c 13.0 seconds total run time

> picosat913 OrderGT-30.cnf
c 323798892 conflicts
c 26060.5 seconds total run time
   \endverbatim
   </li>
   <li> precosat-570.1:
   \verbatim
> precosat-570.1 -v OrderGT-20.cnf
c 558 conflicts, 2354 decisions, 0 random
c 0.0 seconds, 1 MB max, 0 MB recycled

> precosat-570.1 -v OrderGT-25.cnf
c 1096 conflicts, 4978 decisions, 0 random
c 0.1 seconds, 1 MB max, 0 MB recycled

> precosat-570.1 -v OrderGT-30.cnf
c 1375 conflicts, 8945 decisions, 2 random
c 0.1 seconds, 2 MB max, 0 MB recycled

> precosat-570.1 -v OrderGT-40.cnf
c 2616 conflicts, 23538 decisions, 8 random
c 0.3 seconds, 6 MB max, 0 MB recycled

> precosat-570.1 -v OrderGT-50.cnf
c 4053 conflicts, 50671 decisions, 16 random
c 0.6 seconds, 10 MB max, 0 MB recycled

> precosat-570.1 -v OrderGT-60.cnf
c 5581 conflicts, 100137 decisions, 43 random
c 1.2 seconds, 16 MB max, 0 MB recycled

> precosat-570.1 -v OrderGT-70.cnf
c 8425 conflicts, 199397 decisions, 90 random
c 2.2 seconds, 32 MB max, 1 MB recycled

> precosat-570.1 -v OrderGT-80.cnf
c 10884 conflicts, 311653 decisions, 156 random
c 3.5 seconds, 45 MB max, 1 MB recycled

> precosat-570.1 -v OrderGT-90.cnf
c 12785 conflicts, 504797 decisions, 245 random
c 5.7 seconds, 60 MB max, 2 MB recycled

> precosat-570.1 -v OrderGT-100.cnf
c 18507 conflicts, 941534 decisions, 455 random
c 8.7 seconds, 79 MB max, 3 MB recycled

> precosat-570.1 -v OrderGT-110.cnf
c 28004 conflicts, 2770028 decisions, 1371 random
c 15.2 seconds, 101 MB max, 5 MB recycled

# m=120 not possible with 4GB.
   \endverbatim
   What is the essential difference between picosat and precosat here?!
   </li>
   <li> For bigger problems a C++ generator is needed (and the details of
   the translations, i.e., the variable-numbering and the order of clauses,
   need to be fixed). </li>
   <li> For order m in [Kullmann, ECCC, 1999]
   http://eccc.hpi-web.de/report/1999/041/ (Lemma 8.13) it is shown that
   the hardness equals m-1 (while the (improved) width is m-2). </li>
   <li> So perhaps precosat uses longer clauses than the other conflict-driven
   solvers (which still use full resolution, and have an advantage over the
   look-ahead solvers)? </li>
  </ul>


  \todo 3 different representations of HIT(1)
  <ul>
   <li> According to [Gywnne, Kullmann, 2013]. </li>
   <li> Experiment preparation:
   \verbatim
k:2;
for h in [22,32,42,52,62,72] do (output_ext1_sat_genhorn(h,k),output_ext2_sat_genhorn(h,k),output_ext3_sat_genhorn(h,k));
k:3;
for h in [23,33,43] do (output_ext1_sat_genhorn(h,k),output_ext2_sat_genhorn(h,k),output_ext3_sat_genhorn(h,k));
k:4;
for h in [24,34,44] do (output_ext1_sat_genhorn(h,k),output_ext2_sat_genhorn(h,k),output_ext3_sat_genhorn(h,k));
missing:
output_ext2_sat_genhorn(44,4); XXX
output_ext3_sat_genhorn(44,4); XXX cs-wsok
k:5;
for h in [25,35] do (output_ext1_sat_genhorn(h,k),output_ext2_sat_genhorn(h,k),output_ext3_sat_genhorn(h,k));
missing (more memory)
output_ext2_sat_genhorn(35,5); XXX cs-wsok
output_ext3_sat_genhorn(35,5); XXX

> for F in *.ecnf; do B=$(basename --suffix=".ecnf" ${F}); echo ${B}; cat ${F} | ExtendedToStrictDimacs-O3-DNDEBUG > ${B}.cnf; done
   \endverbatim
   </li>
   <li> Determining the sizes:
   \verbatim
for F in *.cnf; do echo ${F} " "; cat ${F} | ExtendedDimacsStatistics-O3-DNDEBUG; done
E1_SAT_genhorn_22_2.cnf
      n       c   l
    507     508   8604
E1_SAT_genhorn_23_3.cnf
      n       c   l
   4095    4096   80594
E1_SAT_genhorn_24_4.cnf
      n       c   l
  25901   25902   562542
E1_SAT_genhorn_25_5.cnf
      n       c   l
 136811  136812   3202912
E1_SAT_genhorn_32_2.cnf
      n       c   l
   1057    1058   24994
E1_SAT_genhorn_33_3.cnf
      n       c   l
  12035   12036   327384
E1_SAT_genhorn_34_4.cnf
      n       c   l
 105911  105912   3150408
E1_SAT_genhorn_35_5.cnf
      n       c   l
 768335  768336   24413776
E1_SAT_genhorn_42_2.cnf
      n       c   l
   1807    1808   54784
E1_SAT_genhorn_43_3.cnf
      n       c   l
  26575   26576   922524
E1_SAT_genhorn_44_4.cnf
      n       c   l
 299971  299972   11326724
E1_SAT_genhorn_52_2.cnf
      n       c   l
   2757    2758   101974
E1_SAT_genhorn_62_2.cnf
      n       c   l
   3907    3908   170564
E1_SAT_genhorn_72_2.cnf
      n       c   l
   5257    5258   264554
E2_SAT_genhorn_22_2.cnf
      n       c   l
    761    4811   17716
E2_SAT_genhorn_23_3.cnf
      n       c   l
   6143   44394   165284
E2_SAT_genhorn_24_4.cnf
      n       c   l
  38852  307174   1150986
E2_SAT_genhorn_25_5.cnf
      n       c   l
 205217 1738269   6542636
E2_SAT_genhorn_32_2.cnf
      n       c   l
   1586   13556   51046
E2_SAT_genhorn_33_3.cnf
      n       c   l
  18053  175729   666804
E2_SAT_genhorn_34_4.cnf
      n       c   l
 158867 1681117   6406728
E2_SAT_genhorn_42_2.cnf
      n       c   l
   2711   29201   111376
E2_SAT_genhorn_43_3.cnf
      n       c   l
  39863  487839   1871624
E2_SAT_genhorn_52_2.cnf
      n       c   l
   4136   53746   206706
E2_SAT_genhorn_62_2.cnf
      n       c   l
   5861   89191   345036
E2_SAT_genhorn_72_2.cnf
      n       c   l
   7886  137536   534366
E3_SAT_genhorn_22_2.cnf
      n       c   l
    761    4557   13160
E3_SAT_genhorn_23_3.cnf
      n       c   l
   6143   42346   122939
E3_SAT_genhorn_24_4.cnf
      n       c   l
  38852  294223   856764
E3_SAT_genhorn_25_5.cnf
      n       c   l
 205217 1669863   4872774
E3_SAT_genhorn_32_2.cnf
      n       c   l
   1586   13027   38020
E3_SAT_genhorn_33_3.cnf
      n       c   l
  18053  169711   497094
E3_SAT_genhorn_34_4.cnf
      n       c   l
 158867 1628161   4778568
E3_SAT_genhorn_42_2.cnf
      n       c   l
   2711   28297   83080
E3_SAT_genhorn_43_3.cnf
      n       c   l
  39863  474551   1397074
E3_SAT_genhorn_52_2.cnf
      n       c   l
   4136   52367   154340
E3_SAT_genhorn_62_2.cnf
      n       c   l
   5861   87237   257800
E3_SAT_genhorn_72_2.cnf
      n       c   l
   7886  134907   399460

  \endverbatim
  </li>
   <li> Running experiments (on cs-wsok):
   \verbatim
> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; OKsolver_2002-O3-DNDEBUG --timeout=7200 ${F} > ${B}.oksolver; done
XXX cs-wsok
> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; glucose-2.0 -cpu-lim=3600 ${F} > ${B}.glucose; done
XXX cs-wsok
> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; picosat913 ${F} > ${B}.picosat; done
> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; precosat-570.1 -v ${F} > ${B}.precosat; done
> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; minisat-2.2.0 -no-pre -cpu-lim=3600 ${F} > ${B}.minisat-no; done
> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; minisat-2.2.0 -cpu-lim=3600 ${F} > ${B}.minisat; done
   \endverbatim
   </li>
   <li> From the look-ahead solvers OKsolver2002 seems far best, from the
   conflict-driven solvers glucose-2.0 and precosat-570.1, picosat913. </li>
  </ul>

*/

