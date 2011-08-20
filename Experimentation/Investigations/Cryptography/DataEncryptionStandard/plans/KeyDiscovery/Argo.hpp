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
   <li> Running experiments:
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
   <li> We need a script which can take a list of CNF files, and produce
   an R-formatted statistics file with columns for the number of variables and
   clauses and then columns for the number of clause of each size from the
   minimum to maximum clause-length across the files. </li>
   <li> DONE (see Cryptography/DataEncryptionStandard/GenerateArgoDESExp) We
   need a script "GenerateArgoDESExp" which takes a list of Argo
   DES instances of the form gss-i-s100.cnf and generates an Argo DES
   experiment directory. </li>
  </ul>




*/
