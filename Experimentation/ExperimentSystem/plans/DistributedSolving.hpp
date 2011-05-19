// Oliver Kullmann, 19.4.2009 (Swansea)
/* Copyright 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/plans/DistributedSolving.hpp
  \brief Plans for distributed SAT solving via a splitting tree


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

*/

