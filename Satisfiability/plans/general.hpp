// Oliver Kullmann, 16.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Transitional/Satisfiability/plans/general.hpp
  \brief Contains all super-modules regarding (generalised) satisfiability
  issues


  \todo Move older modules here:
  <ol>
   <li> Variables to ProblemInstances/Variables (a merge). </li>
   <li> Literals to ProblemInstances/Literals (a merge). </li>
   <li> SATAlgorithms to Algorithms (a merge). DONE </li>
   <li> Utilities to Algorithms or to ProblemInstances. DONE
   (actually moved to Programming; one shall not forget it there) </li>
   <li> AtomicConditions to ProblemInstances/Conditions. DONE </li>
   <li> PartialAssignments to Assignments. DONE </li>
   <li> LocalSearch to Algorithms. DONE </li>
  </ol>


  \todo All sub-supermodules have milestones.


  \todo Module structure:
  <ul>
   <li> Perhaps under Variations we should have a module Counting?
    <ol>
     <li> There we should implement the algorithm for approximating
     the number of falsifying assignments by sampling. </li>
    </ol>
   </li>
   <li> Of course, counting algorithms are also obtained by representing
   all solutions, which is under AllSolutions/plans/general.hpp ? </li>
   <li> We need to find a clear structure where to place what. </li>
  </ul>


  \todo Milestones : DONE
  (we have enough structure in this part, so also here milestones are introduced,
  establishing a proper hierarchy)
  <ul>
   <li> It seems that at least for now, there are no milestones at the level
   of super-module Satisfiability. </li>
   <li> Having milestones also at this level seems artificial (for now).
   One wants to say directly that Solvers or Heuristics should advance,
   without introducing artificial combination-milestones. </li>
   <li> However, like namespace administration, there are todos also at
   this level? Perhaps for now they go directly to Transitional/plans? </li>
   <li> The tree structure of the planning system (setting milestones must
   follow the hierarchies: the "manager" cannot directly give orders
   to the "workers") is a bit of a hindrance here, but in general it
   seems appropriate. </li>
  </ul>


  \todo Update namespaces : DONE

*/

/*!
  \namespace OKlib::Satisfiability
  \brief The part of the OKlibrary concerned with (generalised) satisfiability

  Alias "Sat".
*/

namespace OKlib {
  namespace Satisfiability {
  }
  namespace Sat = Satisfiability;
}

