// Matthew Gwynne, 22.8.2011 (Swansea)
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
   <li> See "Argo DES instances" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Cryptography.hpp. </li>
   <li> We consider the key discovery instance on these problems. </li>
   <li> The files we consider are gss-i-s100.cnf where i in
   {13,...,17,19,...,28,31,34} is the number of unknown key bits. </li>
   <li> We need the statistics for the instances here. </li>
  </ul>


  \todo minisat-2.2.0
  <ul>
   <li> Running minisat-2.2.0 on the gss-*-s100.cnf instances:
   \verbatim
> mkdir Instances/
> for F in ExternalSources/Installations/SAT/SAT09/APPLICATIONS/crypto/desgen/*; do cp ${F} Instances/$(echo $(basename ${F}) | cut -d "-" -f "2"); done
Experiments/DES/Argo> ls Instances/
13  14  15  16  17  19  20  21  22  23  24  25  26  27  28  31  32  33  34
> for F in Instances/*; do RunMinisat ${F}; done
> cat ExperimentMinisat_Instances13_2011-08-22-111827/Environment
Linux csenceladus 2.6.37.1-1.2-desktop #1 SMP PREEMPT 2011-02-21 10:34:10 +0100 i686 i686 i386 GNU/Linux
processor       : 0
model name      : Intel(R) Core(TM)2 Duo CPU     E8400  @ 3.00GHz
cpu MHz         : 2003.000
cache size      : 6144 KB
bogomips        : 5999.41
processor       : 1
<same as processor 0>
             total       used       free     shared    buffers     cached
Mem:          3947       1113       2833          0        183        839
Swap:         2053          0       2053

> echo -n "ub "; ExtractMinisat header-only; for F in Instances/*; do echo -n "$(basename ${F}) "; tail -n1 ExperimentMinisat_Instances$(basename ${F})_*/Statistics; done
ub n c t sat cfs dec rts r1 mem ptime stime cfl
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

*/
