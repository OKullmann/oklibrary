// Oliver Kullmann, 21.7.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/BranchingTuples/plans/general.hpp
  \brief Plans regarding the tau function (see SAT-handbook article of OK)
  
  
  \todo Handling of demos
  <ul>
   <li> See "Handling of demos" in ComputerAlgebra/plans/Maxima.hpp. </li>
   <li> We must likely update the current file. </li>
   <li> We should write a dedicated demos-file with the computations
   for OK's handbook-article on heuristics. </li>
  </ul>


  \todo Cleanup BranchingTuples/Basic.mac
  <ul>
   <li> What to do with the computations under
   "Convexity considerations: line versions" ? </li>
   <li> The point should be to demonstrate that certain functions
   are not convex; see
   ComputerAlgebra/Satisfiability/Lisp/BranchingTuples/demos/Basic.mac. </li>
   <li> For example linear combination of distances, where the target is
   to minimise the variance, does not yield a convex optimisation
   problem, as shown in
   ComputerAlgebra/Satisfiability/Lisp/demos/FundamentsBranchingHeuristics.mac
   </li>
   <li> So likely all these convexity considerations should move to the
   demos. </li>
  </ul>


  \todo Reimplement the remaining functionality from Mupad/tau.mup in Maxima
  <ul>
   <li> tau : DONE
   <li> tree probability distributions:
    <ol>
     <li> tpd_flatten : DONE </li>
     <li> tpd_moment : DONE </li>
     <li> tpd_tuples </li>
     <li> td_combine </li>
     <li> tpd_ccombine </li>
     <li> tpd_ccmoment </li>
    </ol>
   </li>
  </ul>


  \todo Trees
  <ul>
   <li> Some of the tree functions should go to a general module
   on trees. </li>
   <li> For example these functions can be applied to splitting trees
   (see ComputerAlgebra/Satisfiability/Lisp/Backtracking/SplittingTrees.mac).
   </li>
  </ul>


  \todo Details of computations
  <ul>
   <li> tauprob :
    <ol>
     <li> Are there better scaling schemes? </li>
    </ol>
   </li>
   <li> tau_eps :
    <ol>
     <li> Could epsilon be eliminated, and the computation always proceeds
     until dx is 0 (for the arithmetic)? </li>
     <li> Are there better lower bounds than tau_lo ? </li>
    </ol>
   </li>
  </ul>


  \todo Optimisation
  <ul>
   <li> Support typical applications to worst-case upper bounds:
    <ol>
     <li> Given a parameter alpha and branching tuples depending
     on alpha, find the minimal tau-value depending on alpha, for
     a given domain. </li>
     <li> This is a very common task, and we should provide a convenient
     framework for this, with  some nice demos (for the upper-bounds
     community). </li>
     <li> The common case seems to be a linear dependence on alpha,
     and then the domain can be computed via linear programming. </li>
     <li> We should provide a variety of optimisation methods. </li>
     <li> See the methods for quasi-convex optimisation discussed in
     [Eppstein, 2006, Quasiconvex Analysis of Multivariate Recurrence
     Equations for Backtracking Algorithms]. </li>
    </ol>
   </li>
   <li> The other basic case is optimising a distance on a given tree,
   and again a convenient framework needs to be provided.
    <ol>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/BranchingTuples/demos/Basic.mac.
     </li>
     <li> See "Mupad/tau.mup" above. </li>
    </ol>
   </li>
   <li> Likely this should go into its own submodule. </li>
  </ul>


  \todo Investigations on approximations
  <ul>
   <li> Perhaps for k=3 it is true that for all eps > 0 and K > eps
   the integral on [eps,K]^3 of dtau_3 is positive? </li>
  </ul>

*/

