// Oliver Kullmann, 19.4.2009 (Swansea)
/* Copyright 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/plans/DistributedSolving.hpp
  \brief Plans for distributed SAT solving via a splitting tree

  Here we are concerned more with experimentation, collaboration of
  different groups etc., while in
  Interfaces/DistributedSolving/plans/general.hpp the emphasise is on
  specific tools in the realm of SplittingViaOKsolver.


  \todo Connections
  <ul>
   <li> See Interfaces/DistributedSolving/plans/general.hpp for tools in the
   realm of SplittingViaOKsolver. </li>
  </ul>


  \todo Adding distribution power to SplittingViaOKsolver
  <ul>
   <li> Basically a script which has given a list of machines where it
   launches and monitors solving of the sub-instances computed by
   SplittingViaOKsolver. </li>
   <li> See "Simple script for monitoring remote processes" and
   "Launching and monitoring" in ExperimentSystem/plans/general.hpp. </li>
   <li> The "database" perhaps is just as described below in "Organising
   distributed solving". </li>
  </ul>


  \todo Distribution via splitting trees
  <ul>
   <li> The basic idea is to have a possibly amended r_k-splitting tree T (see
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/SplittingTrees.mac)
   for the hard input clause-set F, such that T has no [true] or [false]
   leaves. </li>
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
   <li> T could be computed by the OKsolver_2002 (see "Simple parallelisation"
   in Solvers/OKsolver/SAT2002/plans/general.hpp), but also stronger reduction
   means could be used. </li>
   <li> First we implement all this at the Maxima/Lisp level. </li>
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

*/

