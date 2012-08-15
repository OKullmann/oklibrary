// Oliver Kullmann, 12.5.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
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
   <li> See ExperimentSystem/plans/DistributedSolving.hpp for general plans
   on distributed solving. </li>
   <li> See "Launching and monitoring" in ExperimentSystem/plans/general.hpp
   for general plans on experimentation on remote machines. </li>
   <li> See
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/Parallelisation/general.hpp
   for solving hard van-der-Waerden problems, as an application. </li>
  </ul>


  \todo Adding distribution power to SplittingViaOKsolver
  <ul>
   <li> Basically a script which has given a list of machines where it
   launches and monitors solving of the sub-instances computed by
   SplittingViaOKsolver. </li>
   <li> Better actually, just to split the SplitViaOKsolver-data into chunks,
   which can be processed simply by ProcessSplitViaOKsolver. </li>
   <li> See "Simple script for monitoring remote processes" and
   "Launching and monitoring" in ExperimentSystem/plans/general.hpp. </li>
   <li> For very large tasks, a "database" as described below in "Organising
   distributed solving" might be involved. </li>
  </ul>


  \todo Organising distributed solving
  <ul>
   <li> This is about larger tasks, involving separated groups, attacked by
   SplittingViaOKsolver. </li>
   <li> Would be good if it could be organised via Github. </li>
   <li> The "database" could be just a file with one entry per line,
   either a single number i, or a range i-j, for the sub-instances which
   have been solved already. </li>
   <li> Should we have a compression tool:
    <ol>
     <li> This would create maximal ranges. </li>
     <li> And also sorting the lines. </li>
     <li> However this would likely incur a large Git-overhead. </li>
     <li> So perhaps we avoid this, and trust that entries will be already
     condensed enough. </li>
     <li> In this way just the repository with its mechanisms is sufficient
     to organise the distributed solving. </li>
    </ol>
   </li>
   <li> How to distribute the workload:
    <ol>
     <li> Given that everything needs to be solved, and with the new
     organisation of splitting (according to the number of variables in the
     paths) we have a rather balanced workload, distributing just means
     assigning chunks, i.e., intervals i-j of indices of sub-problems. </li>
     <li> One need then to have a file with participants, and their assigned
     intervals. </li>
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


  \todo Distribution via splitting trees
  <ul>
   <li> The basic idea is to have a possibly amended r_k-splitting tree T (see
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/SplittingTrees.mac)
   for the hard input clause-set F, such that T has no [true] or [false]
   leaves. </li>
   <li> This generalises SplittingViaOKsolver. </li>
   <li> The leaves of F are enumerated by indices 1, ..., L, and the
   distributed solving of F just distributes problem instances F_i for i in
   {1,...,L} to available machines (using appropriate solvers S_i). </li>
   <li> The problem instance F_i is just the instance at the corresponding
   node of T, which is created by adding the assignments on the path to leaf i
   to F (as unit-clauses; so the reduction of S_i must be strong enough
   to recover all forced assignments (possibly helped by the amendments)).
   </li>
   <li> The main task is to find a suitable T; but even if we split the
   instance into, say,  2^16 subinstances F_i, the tree T is still relatively
   small. </li>
   <li> And given F, T and i, computing F_i is a rather simple task (assuming
   that F_i will take at least, say, hours). </li>
   <li> DONE (realised by SplittingViaOKsolver)
   T could be computed by the OKsolver_2002 (see "Simple parallelisation"
   in Solvers/OKsolver/SAT2002/plans/general.hpp), but also stronger reduction
   means could be used. </li>
   <li> First we implement all this at the Maxima/Lisp level. </li>
  </ul>

*/

