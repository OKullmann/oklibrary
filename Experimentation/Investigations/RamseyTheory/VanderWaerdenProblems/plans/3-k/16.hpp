// Oliver Kullmann, 22.11.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/15.hpp
  \brief On investigations into vdw_2(3,16) = 238


  \todo OKsolver
  <ul>
   <li> k=16, n=238: Running it with monitoring level 20 appears hopeless.
   And using the m2pp-variant doesn't make a difference, since the
   preprocessor has no effect. </li>
  </ul>


  \todo satz215
  <ul>
   <li> n=238:
   \verbatim
****the instance is unsatisfiable *****
NB_MONO= 62751, NB_UNIT= 17632939133, NB_BRANCHE= 600383827, NB_BACK= 304409259
Program terminated in 990144.910 seconds.
satz215 VanDerWaerden_2-3-16_238.cnf 990144.910 600383827 304409259 99369777769 3501709276 0 238 15812 0 3305729737 824297599
   \endverbatim
   (11 1/2 days) </li>
   <li> Stably a factor of 10 (when increasing k by one). </li>
   <li> So with some parallelisation k=17 should be feasible
   (the unsatisfiable case; around 3 months on a single processor). </li>
  </ul>


  \todo minisat2
  <ul>
   <li> n=238: Apparently after 30 restarts and 36,906,145 conflicts no
   progress was made. </li>
  </ul>


  \todo Splitting via OKsolver
  <ul>
   <li> Starting with n=30:
   \verbatim
> SplittingViaOKsolver -D30 VanDerWaerden_2-3-16_238.cnf
  30   31   32   33   34   35   36   37   38   39   40   41   42   43   44   45
3599 1104 1137 1188 1333 1450 1322 1215 1007  663  329   86   44   35   14   19
  46   47   48   49   50   51   53
  21   10    3    6    4    3    2
> more Result
c initial_maximal_clause_length         16
c initial_number_of_variables           238
c initial_number_of_clauses             15812
c initial_number_of_literal_occurrences 70446
c running_time(sec)                     431.4
c number_of_nodes                       29187
c number_of_2-reductions                580
c max_tree_depth                        27
c splitting_cases                       14594
> tail -2 Data
14593 14593 30
14594 14594 30
> cat VanDerWaerden_2-3-16_238.cnf | ApplyPass-O3-DNDEBUG Instances/14593 14593.cnf
> minisat-2.2.0 14593.cnf
conflicts             : 599566         (26353 /sec)
CPU time              : 22.7515 s
> cat VanDerWaerden_2-3-16_238.cnf | ApplyPass-O3-DNDEBUG Instances/14594 14594.cnf
> minisat-2.2.0 14594.cnf
conflicts             : 1297292        (25110 /sec)
CPU time              : 51.6651 s

> SplittingViaOKsolver -D33 VanDerWaerden_2-3-16_238.cnf
  33   34   35   36   37   38   39   40   41   42   43   44   45   46   47   48
5527 1987 2521 2892 2894 2774 2565 2191 1812 1095  560  206   94   56   37   24
  49   50   51   52   53   54   55   56
  30   30   26   25   24   20    6    1
> more Result
c running_time(sec)                     863.0
c number_of_nodes                       54793
c number_of_2-reductions                1648
c max_tree_depth                        28
c splitting_cases                       27397
> tail -2 Data
27396 27393 33
27397 27397 33
> cat VanDerWaerden_2-3-16_238.cnf | ApplyPass-O3-DNDEBUG Instances/27393 27393.cnf
> minisat-2.2.0 27393.cnf
conflicts             : 127857         (29955 /sec)
CPU time              : 4.26835 s
> cat VanDerWaerden_2-3-16_238.cnf | ApplyPass-O3-DNDEBUG Instances/27397 27397.cnf
> minisat-2.2.0 27397.cnf
conflicts             : 331349         (28054 /sec)
CPU time              : 11.8112 s

> SplittingViaOKsolver -D35 VanDerWaerden_2-3-16_238.cnf
> cd SplitViaOKsolver_D35SNVanDerWaerden_2316_238cnf_2011-05-29-100553
> more Md5sum
b1cce9e3fcbaffacc351ce4630edae2b
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
     35      36      39      39      41      59
> table(E$n)
  35   36   37   38   39   40   41   42   43   44   45   46   47   48   49   50
7893 3559 3744 3948 4068 3799 3619 2846 2155 1503  893  462  263   93   55   57
  51   52   53   54   55   56   57   58   59
  69   70   82   69   43   16   19    2    2
> more Result
c running_time(sec)                     1284.9
c number_of_nodes                       78657
c number_of_2-reductions                2824
c max_tree_depth                        30
c splitting_cases                       39329

> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")

   \endverbatim
   </li>
  </ul>


  \todo Palindromic numbers
  <ul>
   <li> pdvanderwaerden([3,16])[2] = 237 = vanderwaerden([3,16])-1 . </li>
   <li> pdvanderwaerden([3,16])[1] = 232. </li>
   <li> OKsolver_2002 rather easily determines n=236 as satisfiable and
   n=233, 237,238 as unsatisfiable. </li>
   <li> So does satz215 (somewhat faster), march_pl and minisat2 (somewhat
   faster). </li>
   <li> "RunPdVdWk1k2 3 16 gsat-tabu 100 300000" yields
   \verbatim
Break point 1: 232
Break point 2: 237
   \endverbatim
   where all solutions were found in the first 5 runs. </li>
  </ul>

*/

