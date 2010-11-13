// Oliver Kullmann, 13.2.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/AlgorithmSelection.hpp
  \brief On best algorithms for %Ramsey problems


  \todo Finding best UBCSAT algorithm for %Ramsey problems
  <ul>
   <li> See
   ExperimentSystem/ControllingLocalSearch/plans/AlgorithmSelection.hpp for
   general approaches. </li>
   <li> In the following where the notation "n for x <= n <= y" is used, the
   notation means that the experiment is run for values of n in that range
   and these results are tabulated together. </li>
   <li> However, the data frame created keeps track of the input file used 
   in a field called "input" and so after an initial, very naive glance, at 
   "lumped together" statistics across different "n", one can filter the data 
   frame and examine results for specific "n".
   <li> This "lumped together" approach also makes it easy to make quick 
   comparisons across different "n" to see how each algorithm scales and to 
   make quick plots to analyse this. </li>
   <li> Some initial testing, using the "run_ubcsat" 
   (see Experimentation/ExperimentSystem/ControllingLocalSearch/Evaluation.R),
   investigating parameter tuple [5,5;2] for 30 <= n <= 33,
   with runs=3, cutoff=1000:
   \verbatim
ramsey_cnfs <- list.files(".","Ramsey.*\\.cnf$")
for (ramsey_cnf in ramsey_cnfs) {
  result_df <- run_ubcsat(ramsey_cnf)
  result_df <- add_constant_column(result_df, ramsey_cnf, "input")
  if (exists("ramsey_df")) {
    ramsey_df <- rbind(ramsey_df, result_df)
  } else {
    ramsey_df <- result_df
  }
  write.table(ramsey_df, paste("After_", ramsey_cnf,"_Result", sep=""))
}
   \endverbatim
   and then some very basic statistics: 
   \verbatim
> ramsey_mean_df <- aggregate(list(avg_beststep=ramsey_df$beststep,avg_cputime_mean=ramsey_df$CPUTime_Mean), list(alg=ramsey_df$alg), mean)
> ramsey_mean_df[order(ramsey_mean_df$cputime_mean),]
                      alg  avg_beststep avg_cputime_mean
16                 sapsnr     100.41667       0.03583325
18                  irots     185.08333       0.03583325
4               gsat-tabu     175.00000       0.03666650
5                    hsat     219.50000       0.03750000
2          gsat -v simple     255.25000       0.04333350
14                   saps      96.33333       0.04333350
17                   rots     230.91667       0.04416650
19                   samd     226.75000       0.04416650
15                  rsaps     104.91667       0.04416675
1                    gsat     233.75000       0.04583325
6                   hwsat     236.75000       0.04750000
20 walksat-tabu -v nonull     141.41667       0.05166650
8            walksat-tabu     140.00000       0.05416675
12               rnovelty     102.08333       0.06500000
10               novelty+     107.08333       0.06916650
9                 novelty     113.58333       0.07416675
3                   gwsat     408.16667       0.07666700
13              rnovelty+      94.58333       0.09416650
7                 walksat     326.75000       0.14000000
11          adaptnovelty+     124.66667       0.15583325
   \endverbatim
   </li>
   <li> The first column in the result above is simply an artifact of the data
   frame along with the fact that it has been reordered/sorted. Ordinarily a 
   data.frame will list the "number" of each row at the side. As the data frame
   has been sorted, these original numbers have been reordered as well. </li>
   <li> It seems that the first "column" is printed by print.data.frame, and 
   is not an actual column in the data frame. This has now been fixed by making
   "find_best_ubcsat_alg" pass "row.names=FALSE" to the print command to force
   it to print the data frame without these row names instead of simply 
   returning a data frame. </li>
   <li> Based solely on a simple average of the cputimes, "sapsnr" and 
   "irots" seem to do well, although it seems that such a simple metric is
   unreasonable due to the possible mechanics of CPU scheduling etc.;
   moreover, 3 runs mean nothing. </li>
   <li> Therefore this is just a simple example of how one can aggregate data 
   etc in R and present certain results nicely. </li>
   <li> Looking at the number of falsfied clauses and number of steps involved
   (using the data from above): 
   \verbatim
> ramsey_mean_df <- aggregate(list(avg_falsified_clauses=ramsey_df$best, avg_best_steps=ramsey_df$beststep), list(alg=ramsey_df$alg), mean)
> ramsey_mean_df[order(ramsey_mean_df$avg_falsified_clauses, ramsey_mean_df$avg_best_steps),]
                      alg avg_falsified_clauses    avg_best_steps
13              rnovelty+        0.00000000              101.0833
14                   saps        0.00000000              101.5833
9                 novelty        0.00000000    		 107.5833
10               novelty+        0.00000000    		 109.3333
16                 sapsnr        0.00000000    		 113.0000
15                  rsaps        0.00000000    		 114.4167
20 walksat-tabu -v nonull        0.00000000    		 132.7500
12               rnovelty        0.00000000    		 141.6667
8            walksat-tabu        0.00000000    		 158.9167
17                   rots        0.00000000    		 171.4167
18                  irots        0.00000000    		 189.8333
19                   samd        0.00000000    		 205.5000
2          gsat -v simple        0.00000000    		 221.1667
6                   hwsat        0.00000000    		 225.7500
4               gsat-tabu        0.00000000    		 227.7500
1                    gsat        0.00000000    		 254.8333
5                    hsat        0.00000000    		 285.2500
11          adaptnovelty+        0.08333333    		 208.6667
3                   gwsat        0.58333333    		 448.1667
7                 walksat        1.75000000    		 362.9167
   \endverbatim
   seems to suggest "rnovelty+" and "saps" as good algorithms for %Ramsey
   problems. 
   </li>
   <li> Introducing a small utility function to list the algorithms in order
   of decreasing performance (based on falsified clauses and best_steps):
   \verbatim
find_best_ubcsat_alg <- function(df) {
 mean_df <- aggregate(list(avg_falsified_clauses=df$best, avg_best_steps=df$beststep), list(alg=df$alg), mean)
 mean_df[order(mean_df$avg_falsified_clauses,mean_df$avg_best_steps),]
 print(mean_df,row.names = FALSE)
}
   \endverbatim
   </li>
   <li> Using "ramsey_2^2(5) <= n" with n for  30 <=  n <= 40 with runs=100,
   cutoff=10000: 
   \verbatim
> find_best_ubcsat_alg(ramsey_df)
                    alg avg_falsified_clauses avg_best_steps
              gsat-tabu              10.13909      2886.4927
                   samd              10.19909      2956.7991
                   rots              10.33000      2796.9582
          adaptnovelty+              12.69364      3638.5318
                 sapsnr              13.04455      2337.0755
                   saps              13.09455      2282.8964
                  rsaps              13.18727      2091.9591
               rnovelty              15.35727      2806.9618
              rnovelty+              15.44091      2868.1300
                  irots              16.00455      1989.1127
                  hwsat              17.53364      2912.4755
                  gwsat              18.18727      3960.1482
         gsat -v simple              22.36273       937.9009
                   gsat              22.52273       929.2727
                   hsat              22.54545       786.2127
                novelty              23.45455      2421.1018
               novelty+              24.01455      2518.1327
 walksat-tabu -v nonull              29.75182      3010.9000
           walksat-tabu              29.86545      2859.1036
                walksat              80.75909      3517.2764
   \endverbatim
   seems to suggest that "gsat-tabu" and "samd" perform well and looking
   across the individual instances for each n = 30,35,40, we get: 
   \verbatim
> find_best_ubcsat_alg(subset(ramsey_df, ramsey_df$input=="Ramsey_5_2_30.cnf"))
                    alg avg_falsified_clauses avg_best_steps
                   saps                     0          61.55
                 sapsnr                     0          61.57
                novelty                     0          63.10
               rnovelty                     0          63.43
                  rsaps                     0          63.90
               novelty+                     0          65.34
              rnovelty+                     0          71.79
           walksat-tabu                     0          79.73
 walksat-tabu -v nonull                     0          80.57
                walksat                     0         102.85
                  irots                     0         103.28
              gsat-tabu                     0         109.58
          adaptnovelty+                     0         111.73
                  hwsat                     0         113.96
                   rots                     0         114.32
                   samd                     0         114.89
         gsat -v simple                     0         119.09
                   gsat                     0         124.18
                   hsat                     0         128.74
                  gwsat                     0         185.98
> find_best_ubcsat_alg(subset(ramsey_df, ramsey_df$input=="Ramsey_5_2_35.cnf"))
                    alg avg_falsified_clauses avg_best_steps
                 sapsnr                  0.00         855.11
                   saps                  0.00         899.63
                  rsaps                  0.00         958.51
                novelty                  0.00        1120.64
               novelty+                  0.00        1315.88
              gsat-tabu                  0.00        1690.29
                   samd                  0.00        1709.61
                   rots                  0.00        1938.08
                  hwsat                  0.04        3192.42
                   hsat                  0.33        2844.31
              rnovelty+                  0.34        2591.36
         gsat -v simple                  0.45        3494.01
               rnovelty                  0.47        2285.03
                   gsat                  0.49        3424.83
 walksat-tabu -v nonull                  0.84        4698.46
           walksat-tabu                  1.02        4414.14
                  irots                  1.11        3596.73
          adaptnovelty+                  2.33        3248.09
                  gwsat                  3.43        5529.64
                walksat                 37.48        4872.00
> find_best_ubcsat_alg(subset(ramsey_df, ramsey_df$input=="Ramsey_5_2_40.cnf"))
                    alg avg_falsified_clauses avg_best_steps
              gsat-tabu                 50.50        6752.04
                   rots                 50.69        5765.56
                   samd                 50.72        7015.10
                   saps                 64.90        4927.95
                 sapsnr                 65.09        4909.03
                  rsaps                 65.33        4135.28
          adaptnovelty+                 70.54        6298.52
                  irots                 71.92        3058.22
                  gwsat                 77.25        6582.99
                  hwsat                 81.76        5797.83
               rnovelty                 85.34        5284.96
              rnovelty+                 86.74        5475.05
                   gsat                 98.69         364.64
         gsat -v simple                 98.97         333.75
                   hsat                100.49         295.62
                novelty                112.99        4814.45
               novelty+                114.47        4581.74
 walksat-tabu -v nonull                135.82        5517.99
           walksat-tabu                136.84        5148.30
                walksat                279.78        4776.30
   \endverbatim
   which seems to suggest gsat-tabu improves in performance as we increase
   the complexity of the problem, while others such as saps perform 
   better for lower "n" but don't perform as well later on. </li>
   <li> Perhaps some linear regression based on "n" vs some combination of
   "avg_falsified_clauses" and "avg_best_steps" would give more indication
   on how each algorithm scales with regard to this problem? </li>
   <li> Using "ramsey_2^2(5) <= n" with n for n = 40 with runs=200,
   cutoff=100000:
   \verbatim
> find_best_ubcsat_alg(ramsey_df)
                      alg avg_falsified_clauses avg_best_steps
              gsat-tabu                 50.34        6826.04
                   samd                 50.56        6784.06
                   rots                 51.71        5957.93
                   saps                 64.40        5058.40
                 sapsnr                 64.73        5078.79
                  rsaps                 65.28        4136.76
          adaptnovelty+                 70.91        6063.12
                  irots                 71.76        3844.65
                  gwsat                 77.53        6567.07
                  hwsat                 81.45        5947.01
              rnovelty+                 85.67        4845.05
               rnovelty                 88.06        5429.28
         gsat -v simple                 98.82         333.62
                   hsat                 99.54         301.37
                   gsat                101.52         354.20
                novelty                112.54        5319.51
               novelty+                113.57        4817.34
           walksat-tabu                134.99        5123.96
 walksat-tabu -v nonull                135.07        5679.43
                walksat                276.30        5191.63
   \endverbatim
   seems to provide further confirmation of the success of "gsat-tabu" and 
   "samd" on larger Ramsey problems. </li> 
   <li> Currently running an experiment with runs=200, cutoff=100000 and
   n = 41. </li>
  </ul>


  \todo Understanding local search algorithms
  <ul>
   <li> First one needs to establish which algorithm from the Ubcsat
   collection performs best (hopefully sooner or later we can use
   also version 1.1.0); see above. </li>
   <li> For this best algorithm we should also try to optimise the
   parameters. </li>
   <li> Important to study the search landscapes; see "Examing search
   landscapes" in
   ComputerAlgebra/Satisfiability/Lisp/LocalSearch/plans/general.hpp. </li>
   <li> The goal is at least to understand why the best algorithm is best,
   and perhaps to further improve this algorithm. </li>
  </ul>

*/

