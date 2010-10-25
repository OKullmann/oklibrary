// Matthew Gwynne, 25.10.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Optimisation/MinSAT/plans/general.hpp
  \brief Super-module for (variable) minimal satisfiability.

  Considered here is the optimisation of SAT problems such that the number
  of variables set to 1 (true) in the satisfying assignment is minimal. 

  \todo Links and plans
  <ul>
   <li> Establish links to other modules (especially computer algebra).
   </li>
   <li> See
   Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp . </li>
   <li> Create a development plan and further milestones. </li>
  </ul>


  \todo Module placement
  <ul>
   <li> The parameterised case is discussed in
   AllSolutions/plans/MinimalAssignments.hpp ---
   should this move to this new module? </li>
   <li> Or does this module actually belong to
   Satisfiability/Algorithms/AllSolutions ? </li>
   <li> It seems that the minimisation problem is different
   also from the parameterised version, and thus
   perhaps the two different modules are justifed:
   Though the minimisation problem can be solved by running
   through the parameter levels, there are many other
   possibilities (for example regarding approximation). </li>
   <li> So perhaps also for finding a transversal of minimum
   size we need a module? </li>
  </ul>

  
  \todo Weighted MaxSAT
  <ul>
   <li> Given any SAT problem, one can translate optimisation of
   the SAT problem with respect to the number of variables set to true, into
   a weighted MaxSAT problem by simply weighting all of the original
   clauses with a very high weight (greater than the sum of the weights of
   added clauses), and then adding negated unit clauses with weight 1 for
   each variable in the original SAT problem. </li>
   <li> In such a way, assuming the original SAT problem is satisfiable, the 
   assignment with satisfying clauses with the maximum weight is the one which
   minimises the number of added satisfied unit clauses. In other words, 
   the number of variables set to true in the original problem is minimised.
   </li>
   <li> The standard weight MaxSAT DIMACS files (see 
   http://www.maxsat.udl.cat/08/index.php?disp=requirements) have the same
   format as a standard DIMACS CNF file, except the parameter line states
   "wcnf" rather than "cnf", i.e.,
   \verbatim
p wcnf num_vars num_clauses
   \endverbatim
   and the beginning of each clause (before any literals) is a non-negative
   integer weight between 1 and 2^31.
   </li>
   <li> See Satisfiability/Optimisation/MinSAT/CNF2WeightedMaxSATOptimVars . 
   </li>
  </ul>


  \todo Rename CNF2WeihtedMaxSATOptimVars
  <ul>
   <li> CNF2WeightedMaxSATOptimVars is an overly long and unintelligible name,
   and therefore a more appropriate, understandable name should be chosen. 
   </li>
   <li> At the same time, one should check whether this would be more 
   appropriate in another module such as Satisfiability/Transformers . </li>
  </ul>

*/
