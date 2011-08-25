// Oliver Kullmann, 15.8.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/DistributedSolving/plans/ProcessSplitViaOKsolver.hpp
  \brief Plans regarding distributed SAT solving via SplittingViaOKsolver


  \todo Option for using only the decision variables
  <ul>
   <li> First extracting the partial assignments, via some tool. </li>
   <li> And then just running through this file. </li>
  </ul>


  \todo Simple tool for running through all sub-instances
  <ul>
   <li> DONE (we have ProcessSplitViaOKsolve)
   Evaluation of the sub-instances as computed by SplittingViaOKsolver.
   </li>
   <li> DONE (we have ProcessSplitViaOKsolve)
   The script just runs through the instances in the order given in the
   Data-file, and stores the statistics in an R-readable file. </li>
   <li> DONE (we have ProcessSplitViaOKsolve)
   This is for investigations into the effectiveness of the splitting.
   And also harder instances can be attacked in this way, since we can easily
   stop and continue. </li>
   <li> See the todos in the source-code. </li>
   <li> DONE (all transferred)
   Simple sequences of Bash-commands, plus corresponding R-commands to
   evaluate the data:
   \verbatim
> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",5),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
   \endverbatim
   </li>
   <li> DONE
   This must now be transferred into a proper script:
    <ol>
     <li> DONE Called "ProcessSplitViaOKsolver". </li>
     <li> DONE There is exactly one parameter, the directory produced by
     SplittingViaOKsolver. </li>
     <li> DONE An experiment-directory is created. </li>
     <li> DONE One file contains all the set-up data. </li>
    </ol>
   </li>
   <li> DONE Stop and re-start:
    <ol>
     <li> We need the possibility to stop (sending the signal from Ctrl-C)
     and to restart (just with the experiment-directory as parameter). </li>
     <li> We have a file "current_subinstance", which contains the row in
     Data of the next instance. </li>
     <li> Initialised with "1" by SplittingViaOKsolver. </li>
     <li> Incremented after completition of the sub-instance. </li>
     <li> After completion it contains "Finished." (this needs to be checked).
     </li>
     <li> The user can adjust this file. </li>
    </ol>
   </li>
   <li> DONE If a satisfying assignment is found, the script will continue, but
   there must be clear indications about that. </li>
   <li> DONE
   Of course, with extraction of all statistical data:
    <ol>
     <li> Containing also the size of the partial assignment, where the
     column is called "npa" ("n" of "partial assignment"). </li>
     <li> See "Extraction tools" in
     ExperimentSystem/SolverMonitoring/plans/general.hpp. </li>
    </ol>
   </li>
   <li> DONE Output:
    <ol>
     <li> To the file "SubinstanceStatistics", which is the R-readable output,
     the output-lines are appended. </li>
     <li> Console output: the indices of the current sub-instance (as above).
     </li>
     <li> If a satisfying sub-instance was found:
      <ol>
       <li> A file "SatisfyingAssignments" is created, which contains all
       information about these assignments (cumulated). </li>
       <li> This file is not created by default; so its existence shows
       that a satisfying assignment was found. </li>
       <li> And the concole output should strongly highlight this event.
       </li>
       <li> The process just continues otherwise; if the user wants to stop
       then, this needs to be done manually. </li>
       <li> At completion, it is output (to console and to a special file
       "result"), whether all instances were unsatisfiable or not. </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>

*/
