// Matthew Gwynne, 20.8.2011 (Swansea)
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
   <li> The files we consider are gss-i-s100.cnf where i in
   {13,...,17,19,...,28,31,34} is the number of unknown key bits. </li>
   <li> We need the statistics for the instances here. </li>
   <li> Overview:
    <ul>
     <li> "minisat-2.2.0", solving up to unknown_bits=25 in maximum time ~11h.
     </li>
     <li> See also "Applying SplittingViaOKsolver". </li>
    </ul>
   </li>
  </ul>


  \todo Experiment scripts
  <ul>
   <li> We need scripts to generate, and process experiments.  </li>
   <li> An Argo experiment directory ArgoDESExp_date should have
   sub-directories and files:
    <ul>
     <li> Instances: directory with files i.cnf for i in
     {13,...,17,19,...,28,31,34} corresponding to the files gss-i-s100.cnf;
     </li>
     <li> Data: a statistics file with the number of unknown bits, i,
     as the first column, and then statistics on each instance as the
     remaining columns. </li>
    </ul>
   </li>
   <li> We need ProcessArgoDES_solver for all other solvers; currently
   we have minisat. </li>
   <li> We should add GenerateArgoDESExp and ProcessArgoDES_minisat to
   the system path. </li>
   <li> We need a script which can take a list of CNF files, and produce
   an R-formatted statistics file with columns for the number of variables and
   clauses and then columns for the number of clause of each size from the
   minimum to maximum clause-length across the files. This should allow
   better output to the Data file in ProcessArgoDES_solver. </li>
   <li> DONE (see Cryptography/DataEncryptionStandard/ProcessArgoDESExp)
   Running experiments:
    <ul>
     <li> We need an example script ProcessArgoDES_solver where solver
     is the name of the solver to run on the instances. </li>
     <li> ProcessArgoDES_solver should take the path to the Argo experiment
     directory. </li>
     <li> ProcessArgoDES_solver should create a directory
     ProcessArgoDES_minisat_date where date is a timestamp. </li>
     <li> ProcessArgoDES_solver should run solver on each gss-i-s100.cnf
     from i=13 to i=34, outputting i to CurrentExperiment. </li>
     <li> As an optional second argument, ProcessArgoDES_solver should
     take the one of it's output directories (ProcessArgoDES_solver_date)
     to restart the experiment from (including) the instance in
     CurrentExperiment. </li>
     <li> ProcessArgoDES_solver should output it's results in R-format to
     ProcessArgoDES_solver_date/Statistics. </li>
    </ul>
   </li>
   <li> DONE (see Cryptography/DataEncryptionStandard/GenerateArgoDESExp) We
   need a script "GenerateArgoDESExp" which takes a list of Argo
   DES instances of the form gss-i-s100.cnf and generates an Argo DES
   experiment directory. </li>
  </ul>


  \todo minisat-2.2.0
  <ul>
   <li> Running minisat-2.2.0 on the gss-*-s100.cnf instances:
   \verbatim
shell> $OKplatform/OKlib/Experimentation/Investigations/Cryptography/DataEncryptionStandard/GenerateArgoDESExp ExternalSources/Installations/SAT/SAT09/APPLICATIONS/crypto/desgen/*
shell> $OKplatform/OKlib/Experimentation/Investigations/Cryptography/DataEncryptionStandard/ProcessArgoDES_minisat ArgoDESExp_2011-08-20-161443
Created new processing-directory Process_ArgoDESExp_2011-08-20-161443_2011-08-20-161745.
ub  n c t sat cfs dec rts r1 mem ptime stime cfl
13 30867 92535 4.27935 1 4163 4782 22 32241454 20.00 0.05 0.17 53421
14 31229 93655 13.6159 1 13103 14368 59 105018641 20.00 0.05 0.18 162452
15 31238 93678 5.72913 1 5452 6451 29 43914380 20.00 0.05 0.18 78501
16 31248 93704 6.80497 1 6260 7575 30 51698260 20.00 0.05 0.18 98687
17 31318 93916 33.6599 1 32100 35152 120 253976822 20.00 0.04 0.18 579619
19 31435 94348 317.759 1 265675 280077 636 2213616862 33.00 0.05 0.18 4803791
20 31503 94548 1378.86 1 1266646 1339078 2428 8324740787 51.00 0.05 0.18 34985574
21 31613 94904 6230.85 1 4975819 5225600 8191 35394063729 83.00 0.05 0.19 121366390
22 31616 94910 17506.6 1 11661878 12189100 16891 92082425131 120.00 0.05 0.19 280720131
23 31711 95200 2167.9 1 1719380 1849480 3322 12190504675 68.00 0.05 0.19 51786283
24 31821 95535 39073.5 1 23385776 24574689 32767 187419566166 176.00 0.05 0.19 630593530
25 31931 95911 215.137 1 194200 217984 510 1389553850 42.00 0.05 0.19 6692588
26 31942 95938 107735 1 59528887 62568695 77051 476482081268 285.00 0.05 0.19 1787397560
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
shell> $OKplatform/OKlib/Experimentation/Investigations/Cryptography/DataEncryptionStandard/GenerateArgoDESExp ExternalSources/Installations/SAT/SAT09/APPLICATIONS/crypto/desgen/*
   \endverbatim
   </li>
   <li> unknown_bits = 19:
    <ul>
     <li> See "minisat-2.2.0" which takes 323.576s on this instance. </li>
     <li> Determining a reasonable splitting-depth:
     \verbatim
> SplittingViaOKsolver -D3400 gss-19-s100.cnf
> cat SplitViaOKsolver_D3400gss19s100cnf_2011-08-18-214700/Statistics

3556 3559 3562 3566 3572 3573 3574 3575 3576 3577 3578 3579 3580 3581 3582 3583
   1    1    1    1    1    1    2    2    1    3    2    3    4    1    2    3
3584 3587 3588 3589 3590 3591 3592 3596 3600 3601 3602 3604 3605 3606 3607 3609
   1    1    2    1    1    2    2    1    1    2    1    1    1    2    2    2
3610 3614 3628 3630 3631 3633 3634 3635 3638 3639 3641 3642 3643 3644 3646 3647
   2    1    1    1    2    2    1    1    1    1    2    2    2    2    1    1
3648 3649 3650 3651 3652 3653 3656 3657 3658 3659 3660 3661 3667 3668 3669 3670
   1    2    1    1    2    1    2    2    1    1    2    2    2    2    1    3
3671 3672 3673 3675 3676 3678 3679 3680 3681 3683 3684 3688 3693 3694 3695 3697
   2    1    1    3    3    2    2    1    1    2    2    1    1    1    1    2
3698 3699 3700 3701 3702 3703 3705 3706 3707 3708 3709 3710 3711 3715 3720 3723
   3    2    3    2    5    3    2    2    2    3    1    2    2    1    1    1
3725 3755 3757 3760 3765 3782 3787 3792 3824
   1    1    1    1    1    1    1    1    1


> ProcessSplitViaOKsolver SplitViaOKsolver_D3400gss19s100cnf_2011-08-18-205406
Created new processing-directory Process_SplitViaOKsolver_D3400gss19s100cnf_2011-08-18-205406_2011-08-18-211550.
1 ... 114
Found a SATISFYING ASSIGNMENT.
115 ... 171

> cd Process_SplitViaOKsolver_D3400gss19s100cnf_2011-08-18-205406_2011-08-18-211550/

> E=read_processsplit_minisat()
171: 0.29h, sum-cfs=3.174528e+06, mean-t=6.066s, mean-cfs=18564
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  3.780   4.330   5.400   6.066   6.660  13.820
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  15280   16800   17670   18560   19450   28590


> SplittingViaOKsolver -D3800 gss-19-s100.cnf
> cat SplitViaOKsolver_D3800gss19s100cnf_2011-08-18-234435/Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   3948    4006    4055    4077    4147    4288

> ProcessSplitViaOKsolver SplitViaOKsolver_D3800gss19s100cnf_2011-08-18-234435
Created new processing-directory Process_SplitViaOKsolver_D3800gss19s100cnf_2011-08-18-234435_2011-08-18-235848.
1 ... 243
Found a SATISFYING ASSIGNMENT.
244 ... 876

> E=read_processsplit_minisat()
  876: 0.46h, sum-cfs=7.395593e+06, mean-t=1.903s, mean-cfs=8442
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.290   1.170   1.270   1.903   2.372   6.210
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
    716    5538    6492    8442   10340   21740
     \endverbatim
     </li>
    </ul>
   </li>
   <li> unknown_bits = 20:
    <ul>
     <li> See "minisat-2.2.0" which takes 1378.86s on this instance. </li>
     <li> Finding a good splitting depth:
     \verbatim
> SplittingViaOKsolver -D3400 gss-20-s100.cnf
> cat SplitViaOKsolver_D3400gss20s100cnf_2011-08-19-085137/Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   3548    3592    3638    3635    3673    3738
> table(E$n)


> ProcessSplitViaOKsolver SplitViaOKsolver_D3400gss20s100cnf_2011-08-19-085137
Created new processing-directory Process_SplitViaOKsolver_D3400gss20s100cnf_2011-08-19-085137_2011-08-19-085916
1 ... 106
Found a SATISFYING ASSIGNMENT.
107 ... 402
Found a SATISFYING ASSIGNMENT.
403 ... 498

> cd Process_SplitViaOKsolver_D3400gss20s100cnf_2011-08-19-085137_2011-08-19-085916
> oklib --R
R> E=read_processsplit_minisat()
498: 0.55h, sum-cfs=7.028355e+06, mean-t=3.983s, mean-cfs=14113
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.460   2.370   3.810   3.983   4.650  14.560
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   2090    9788   13350   14110   16340   38220

> SplittingViaOKsolver -D3600 gss-20-s100.cnf
> cat SplitViaOKsolver_D3600gss20s100cnf_2011-08-19-093412/Result
c running_time(sec)                     1102.6
c number_of_nodes                       2423
c number_of_2-reductions                26301
c splitting_cases                       1212

> ProcessSplitViaOKsolver SplitViaOKsolver_D3600gss20s100cnf_2011-08-19-093412
Created new processing-directory Process_SplitViaOKsolver_D3600gss20s100cnf_2011-08-19-093412_2011-08-19-095240
1 ... 636
Found a SATISFYING ASSIGNMENT.
637 ... 970
Found a SATISFYING ASSIGNMENT.
971 ... 1212

> cd Process_SplitViaOKsolver_D3600gss20s100cnf_2011-08-19-093412_2011-08-19-095240
> oklib --R
R> E=read_processsplit_minisat()
1212: 0.71h, sum-cfs=1.122742e+07, mean-t=2.097s, mean-cfs=9264
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.640   1.370   1.910   2.097   2.750   4.390
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   3378    6192    9018    9264   12130   18660
     \endverbatim
     </li>
    </ul>
   </li>
   <li> unknown_bits = 21:
    <ul>
     <li> See "minisat-2.2.0" which takes 6230.85s on this instance. </li>
     <li> Finding a good splitting depth:
     \verbatim
> SplittingViaOKsolver -D3400 gss-21-s100.cnf
> cat SplitViaOKsolver_D3400gss21s100cnf_2011-08-19-114411/Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   3548    3594    3632    3631    3665    3738
> table(E$n)


> ProcessSplitViaOKsolver SplitViaOKsolver_D3400gss21s100cnf_2011-08-19-114411/
Created new processing-directory Process_SplitViaOKsolver_D3400gss21s100cnf_2011-08-19-114411_2011-08-19-121149.
1 ...  773
Found a SATISFYING ASSIGNMENT.
774 ... 846

> cd Process_SplitViaOKsolver_D3400gss21s100cnf_2011-08-19-114411_2011-08-19-121149
> oklib --R
R> E=read_processsplit_minisat()
 846: 1.26h, sum-cfs=1.430335e+07, mean-t=5.341s, mean-cfs=16907
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  2.818   4.401   4.984   5.341   5.752  14.560
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   7311   14640   17000   16910   18280   33240

> SplittingViaOKsolver -D3600 gss-21-s100.cnf
> cat SplitViaOKsolver_D3600gss21s100cnf_2011-08-19-134936/Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   3748    3785    3816    3816    3842    3934
> table(E$n)

> ProcessSplitViaOKsolver SplitViaOKsolver_D3600gss21s100cnf_2011-08-19-134936/Statistics
     \endverbatim
     </li>
    </ul>
   </li>
   <li> unknown_bits = 22:
    <ul>
     <li> See "minisat-2.2.0" which takes 17361.05s on this instance. </li>
     <li> Finding a good splitting depth:
     \verbatim
> SplittingViaOKsolver -D3400 gss-22-s100.cnf
> cat SplitViaOKsolver_D3400gss22s100cnf_2011-08-19-135100/Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   3548    3599    3638    3693    3807    4014
> table(E$n)

> ProcessSplitViaOKsolver SplitViaOKsolver_D3400gss22s100cnf_2011-08-19-135100
Created new processing-directory Process_SplitViaOKsolver_D3400gss22s100cnf_2011-08-19-135100_2011-08-19-160255.
1 ... 388
Found a SATISFYING ASSIGNMENT.
389 ... 7231
Found a SATISFYING ASSIGNMENT.
7231 ... 9326

> cd Process_SplitViaOKsolver_D3400gss22s100cnf_2011-08-19-135100_2011-08-19-160255
> oklib --R
R> E=read_processsplit_minisat()
9326: 23.15h, sum-cfs=3.228821e+08, mean-t=8.937s, mean-cfs=34622
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
 0.9719  4.7420  9.5960  8.9370 12.7300 17.6300
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   2260   19420   37960   34620   48830   59530

> SplittingViaOKsolver -D3800 gss-22-s100.cnf
     \endverbatim
     </li>
    </ul>
   </li>
   <li> unknown_bits = 23:
    <ul>
     <li> See "minisat-2.2.0" which takes 2167.9 on this instance. </li>
     <li> Finding a good splitting depth:
     \verbatim
> SplittingViaOKsolver -D3400 gss-23-s100.cnf
> cat SplitViaOKsolver_D3400gss23s100cnf_2011-08-19-140339/Result
c splitting_cases                       10849
> cat SplitViaOKsolver_D3400gss23s100cnf_2011-08-19-140339/Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   3548    3590    3623    3654    3659    4015
> table(E$n)

> ProcessSplitViaOKsolver SplitViaOKsolver_D3400gss23s100cnf_2011-08-19-140339
1 ... 2069
     \endverbatim
     </li>
    </ul>
   </li>
   <li> unknown_bits = 24:
    <ul>
     <li> See "minisat-2.2.0" which takes 39073.5 on this instance. </li>
     <li> Finding a good splitting depth:
     \verbatim
> SplittingViaOKsolver -D3400 gss-24-s100.cnf
> cat SplitViaOKsolver_D3400gss24s100cnf_2011-08-19-140557/Result
c splitting_cases                       10532
> cat SplitViaOKsolver_D3400gss24s100cnf_2011-08-19-140557/Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   3548    3591    3623    3641    3655    4018
> table(E$n)

> SplittingViaOKsolver -D3800 gss-24-s100.cnf
     \endverbatim
     </li>
    </ul>
   </li>
   <li> unknown_bits = 25:
    <ul>
     <li> See "minisat-2.2.0" which takes 215.137 on this instance. </li>
     <li> Finding a good splitting depth:
     \verbatim
> SplittingViaOKsolver -D2800 gss-25-s100.cnf
> cat SplitViaOKsolver_D2800gss25s100cnf_2011-08-19-144628/Result
c splitting_cases                       313
> cat SplitViaOKsolver_D2800gss25s100cnf_2011-08-19-144628/Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   2948    3009    3044    3037    3070    3111
> table(E$n)

> ProcessSplitViaOKsolver SplitViaOKsolver_D2800gss25s100cnf_2011-08-19-144628
Created new processing-directory Process_SplitViaOKsolver_D2800gss25s100cnf_2011-08-19-144628_2011-08-19-145152.
1 ...  134
Found a SATISFYING ASSIGNMENT.
135 ... 249
Found a SATISFYING ASSIGNMENT.
250 ... 313


> cd Process_SplitViaOKsolver_D2800gss25s100cnf_2011-08-19-144628_2011-08-19-145152
> oklib --R
R> E=read_processsplit_minisat()
313: 19.90h, sum-cfs=9.576940e+07, mean-t=228.879s, mean-cfs=305973
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  62.21   98.60  179.40  228.90  292.00  989.90
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  88970  137100  238200  306000  387000 1320000

> SplittingViaOKsolver -D3400 gss-24-s100.cnf
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>

*/
