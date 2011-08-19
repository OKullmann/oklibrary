// Matthew Gwynne, 18.8.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/Argo.hpp
  \brief On investigations into the Data Encryption Standard key discovery instances from the ARGO group


  \todo Overview
  <ul>
   <li> See "ArgoSAT DES instances" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp. </li>
   <li> We consider the key discovery instance on these problems. </li>
   <li> We need the statistics for the instances here. </li>
   <li> See
    <ul>
     <li> "minisat-2.2.0". </li>
    </ul>
   </li>
  </ul>


  \todo minisat-2.2.0
  <ul>
   <li> Satisfiable instances:
   \verbatim
shell> dir=des-argosat-sat09-keydiscovery_$(date +"%Y-%m-%d-%H%M%S"); mkdir ${dir};
shell> cd $dir;
shell> cp ExternalSources/Installations/SAT/SAT09/APPLICATIONS/crypto/desgen/* .
shell> mkdir raw_data
shell> echo -n "ub  "; ExtractMinisat header-only | tee minisat_results; for unknown_bits in $(seq 13 17) $(seq 19 28) $(seq 31 34); do
  echo -n "${unknown_bits}  ";
  (minisat-2.2.0 gss-${unknown_bits}-s100.cnf) 2>&1 | tee raw_data/${unknown_bits}.minisat22 | ExtractMinisat data-only;
done | tee minisat_results
ub  n c t sat cfs dec rts r1 mem ptime stime cfl
13  30867 92535 4.27935 1 4163 4782 22 32241454 20.00 0.05 0.17 53421
14  31229 93655 13.6159 1 13103 14368 59 105018641 20.00 0.05 0.18 162452
15  31238 93678 5.72913 1 5452 6451 29 43914380 20.00 0.05 0.18 78501
16  31248 93704 6.80497 1 6260 7575 30 51698260 20.00 0.05 0.18 98687
17  31318 93916 33.6599 1 32100 35152 120 253976822 20.00 0.04 0.18 579619
19  31435 94348 317.759 1 265675 280077 636 2213616862 33.00 0.05 0.18 4803791
20  31503 94548 1378.86 1 1266646 1339078 2428 8324740787 51.00 0.05 0.18 34985574
21  31613 94904 6230.85 1 4975819 5225600 8191 35394063729 83.00 0.05 0.19 121366390
22  31616 94910 17506.6 1 11661878 12189100 16891 92082425131 120.00 0.05 0.19 280720131
23  31711 95200 2167.9 1 1719380 1849480 3322 12190504675 68.00 0.05 0.19 51786283
24  31821 95535 39073.5 1 23385776 24574689 32767 187419566166 176.00 0.05 0.19 630593530
25  31931 95911 215.137 1 194200 217984 510 1389553850 42.00 0.05 0.19 6692588
...
   \endverbatim
   </li>
  </ul>


  \todo Applying SplittingViaOKsolver
  <ul>
   <li> See also "Applying SplittingViaOKsolver" in
   Cryptography/DataEncryptionStandard/plans/KeyDiscovery/KnownKeyBits.hpp. </li>
   <li> Considering unknown bits in 19,...,28,31,...,34,
   with instances gss-${unknown_bits}-s100.cnf. </li>
   <li> For unknown_bits = 17,..., 19, minisat-2.2.0 solves them within a few
   seconds, so there is no utility there. </li>
   <li> Make an experiment directory:
   \verbatim
shell> dir=des-argosat-sat09-keydiscovery_splitting_$(date +"%Y-%m-%d-%H%M%S"); mkdir ${dir};
shell> cd $dir;
shell> cp ExternalSources/Installations/SAT/SAT09/APPLICATIONS/crypto/desgen/* .
   \endverbatim
   </li>
   <li> unknown_bits = 19:
    <ul>
     <li> See "minisat-2.2.0" which takes 323.576s on this instance. </li>
     <li> Basic data:
     \verbatim
shell> cat gss-19-s100.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
31435 94548 222806 0 222806 16 1
 length count
1 64
2 60710
3 33774
     \endverbatim
     </li>
     <li> Determining a reasonable splitting-depth:
     \verbatim
> time SplittingViaOKsolver -D3400 gss-19-s100.cnf
real	2m55.280s
user	2m54.530s
sys	0m0.050s


> ProcessSplitViaOKsolver SplitViaOKsolver_D3400gss19s100cnf_2011-08-18-205406
Created new processing-directory Process_SplitViaOKsolver_D3400gss19s100cnf_2011-08-18-205406_2011-08-18-211550.
1 ... 114
Found a SATISFYING ASSIGNMENT.
115 ... 171
COMPLETED; see
 Process_SplitViaOKsolver_D3400gss19s100cnf_2011-08-18-205406_2011-08-18-211550/Result
 Process_SplitViaOKsolver_D3400gss19s100cnf_2011-08-18-205406_2011-08-18-211550/SubinstanceStatistics
Instance SATISFIABLE.

> cd Process_SplitViaOKsolver_D3400gss19s100cnf_2011-08-18-205406_2011-08-18-211550/

> E=read_processsplit_minisat()
171: 0.29h, sum-cfs=3.174528e+06, mean-t=6.066s, mean-cfs=18564
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  3.780   4.330   5.400   6.066   6.660  13.820
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  15280   16800   17670   18560   19450   28590
     \endverbatim
     This is a slow down by a factor of 6.
     </li>
     <li> "-D 3800":
     \verbatim
> SplittingViaOKsolver -D3600 gss-19-s100.cnf
> cat SplitViaOKsolver_D3800gss19s100cnf_2011-08-18-234435/Result
c running_time(sec)                     848.5
c number_of_nodes                       1751
c number_of_2-reductions                17929
c max_tree_depth                        12

> ProcessSplitViaOKsolver SplitViaOKsolver_D3800gss19s100cnf_2011-08-18-234435
Created new processing-directory Process_SplitViaOKsolver_D3800gss19s100cnf_2011-08-18-234435_2011-08-18-235848.
1 ... 243
Found a SATISFYING ASSIGNMENT.
244 ... 876
COMPLETED; see
 Process_SplitViaOKsolver_D3800gss19s100cnf_2011-08-18-234435_2011-08-18-235848/Result
 Process_SplitViaOKsolver_D3800gss19s100cnf_2011-08-18-234435_2011-08-18-235848/SubinstanceStatistics
Instance SATISFIABLE.

> E=read_processsplit_minisat()
  876: 0.46h, sum-cfs=7.395593e+06, mean-t=1.903s, mean-cfs=8442
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.290   1.170   1.270   1.903   2.372   6.210
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    716    5538    6492    8442   10340   21740

> sum(subset(E, i <=243)["t"]) # Sum of times instances 1 to 243 (first satisfiable instance).
420.78
     \endverbatim
     So at this splitting depth, minisat-2.2.0 solves the instances in less
     time. However, the total time, 848.5 + 420.78 = 1269.28, is a factor
     of ~4x slower. </li>
    </ul>
   </li>
   <li> unknown_bits = 20:
    <ul>
     <li> See "minisat-2.2.0" which takes 1378.86s on this instance. </li>
    </ul>
   </li>
  </ul>

*/
