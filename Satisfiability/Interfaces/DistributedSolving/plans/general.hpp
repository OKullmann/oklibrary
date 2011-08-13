// Oliver Kullmann, 12.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/DistributedSolving/plans/general.hpp
  \brief Plans regarding distributed solving of SAT problems

  See ExperimentSystem/plans/DistributedSolving.hpp for tools/plans which
  are mostly related to managing different machines and/or different
  groups.


  \todo Create milestones


  \todo Connections
  <ul>
   <li> See Solvers/OKsolver/SAT2002/plans/SimpleParallelisation.hpp for the
   currently only splitting tool (SplittingViaOKsolver). </li>
   <li> See ExperimentSystem/plans/DistributedSolving.hpp for other plans
   on distributed solving. </li>
   <li> See
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/Parallelisation/general.hpp
   for solving hard van-der-Waerden problems, as an application. </li>
  </ul>


  \todo Simple tool for running through all sub-instances
  <ul>
   <li> Evaluation of the sub-instances as computed by SplittingViaOKsolver.
   </li>
   <li> The script just runs through the instances in the order given in the
   Data-file, and stores the statistics in an R-readable file. </li>
   <li> This is for investigations into the effectiveness of the splitting.
   And also harder instances can be attacked in this way, since we can easily
   stop and continue. </li>
   <li> Simple sequences of Bash-commands, plus corresponding R-commands to
   evaluate the data:
   \verbatim
> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
   \endverbatim
   </li>
   <li> This must now be transferred into a proper script:
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
   <li> Of course, with extraction of all statistical data:
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
  </ul>


  \todo Sampling tool
  <ul>
   <li> A tool "SamplingSplitting" is to be written, for sampling N
   splitting-instances from each batch of equally-sized partial assignments
   (which yield the splitting-instances). </li>
   <li> Arguments "SamplingSplitting data-file instance-dir N CNF-file solver":
    <ol>
     <li> instance-dir contains the partial assignments in files whose
     names are just natural numbers, as given in data-file. </li>
     <li> N is the sample size. </li>
     <li> N samples for each occurring size of the partial assignment. </li>
     <li> If not N sub-instances (i.e., partial assignments) are available,
     then all are considered. </li>
    </ol>
   </li>
   <li> Creates, as usual, an experiment-directory:
    <ol>
     <li> Containing files i_OUT in sub-directory "Runs" for the solver output
     on sub-instance (partial assignment) i. </li>
     <li> And a file "Statistics" with the extracted data in R-format. </li>
     <li> Plus a file "Parameter" specifying all the parameters of the run.
     </li>
     <li> There is the problem that there is no information on the original
     splitting. This could be solver if parameters data-file and instance-dir
     would be replaced by the splitting-directory --- the name of this
     directory shows then the original splitting-situation. </li>
     <li> The instance CNF-file might then also be inferred automatically.
     </li>
     <li> DONE
     For that the splitting directory should contain a file "F", which
     contains the name of the (original) CNF-file (as provided in the
     splitting-directory, but without a fixed name). </li>
     <li> DONE
     Or perhaps "F" should be a symbolic link? Actually, we also need
     the name, and then it should be provided. </li>
    </ol>
   </li>
  </ul>

*/

