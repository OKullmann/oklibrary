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
shell> $OKplatform/OKlib/Experimentation/Investigations/Cryptography/DataEncryptionStandard/GenerateArgoDESExp ExternalSources/Installations/SAT/SAT09/APPLICATIONS/crypto/desgen/* .
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

*/
