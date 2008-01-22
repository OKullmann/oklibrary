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

*/

