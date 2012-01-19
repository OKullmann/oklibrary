// Oliver Kullmann, 21.4.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/SAT2002/plans/SimpleParallelisation.hpp
  \brief Plans on extending OKsolver-2002 for using it to compute a splitting of a large problem into subproblems

  The main tool is SplittingViaOKsolver.


  \todo Connections
  <ul>
   <li> See module Satisfiability/Interfaces/DistributedSolving for general
   tools for distributed solving; especially see
   Interfaces/DistributedSolving/plans/general.hpp for the general plans. </li>
  </ul>


  \todo More information on the splittings
  <ul>
   <li> The information on the length of the partial assignments should
   be directly computed and output by the OKsolver. </li>
   <li> See "More information on the current path" in
   Solvers/OKsolver/SAT2002/plans/general.hpp. </li>
   <li> The sorting of Data should then be made optional. </li>
   <li> Perhaps the distinction between decision variables and inferred
   variables is shown by an additional space before the decision variable.
   </li>
   <li> Important is also to know for decision variables whether their
   setting is the first or the second branch.
    <ol>
     <li> This information is available in the OKsolver. </li>
     <li> The above space-encoding of decision variables could be adjusted:
     one additional space for first branch, two additional spaces for the
     second branch. </li>
    </ol>
   </li>
   <li> Also useful is, given the decision-assignments, how many assignments
   are then created by unit-clause propagation (UCP):
    <ol>
     <li> This is different to what the OKsolver reports: on the one hand,
     the OKsolver uses the r_2-assignments for its UCP, on the other hand here
     now we have all decisions available. </li>
     <li> This will yield a subset of the assignments made by the OKsolver, and
     it is relevant to see, since it is what the sub-solvers like minisat will
     see. </li>
     <li> We should write a C++ application, which reads once the main
     clause-set, and then uses our UCP component, outputting the sizes of
     the partial assignments computed for each decision-partial-assignment
     to standard output. </li>
     <li> This should be reasonably fast, since it doesn't need resetting.
     </li>
     <li> This should be done just before the md5sum-computation. </li>
    </ol>
   </li>
  </ul>


  \todo The order of the md5sum-computation (SplittingViaOKsolver)
  <ul>
   <li> The files are not processed in numerical order (unlike with
   "Data"). </li>
   <li> Currently just "ls" is used. </li>
   <li> Using "ls" here has the advantage that it could be easier to use an
   external tool (which just needed to duplicate the ls-order). </li>
   <li> We stick to the ls-order, but provided reliably via our own tool
   (see "Improving SplittingViaOKsolver" below). </li>
  </ul>


  \todo Script for applying partial assignments
  <ul>
   <li> DONE (see below)
   The script ApplyPASS_UCP takes one argument, the file with the partial
   assignment. </li>
   <li> DONE (see below)
   It reads from standard input the CNF F (DIMACS). </li>
   <li> DONE (see below)
   Internally, it then adds the assignments as unit-clauses, and performs
   unit-clause propgatation. </li>
   <li> DONE (see below)
   The result (in DIMACS format) is output to standard output. </li>
   <li> DONE (see below)
   Actually, the partiall assignments computed by the OKsolver-2002
   already contain all derived unit-clause-eliminations, and thus better a
   script ApplyPass is provided, which just applies the partial assignment.
   </li>
   <li> DONE
   See "Application ApplyPass" in
   Satisfiability/Assignments/PartialAssignments/plans/Boolean.hpp. </li>
   <li> And after that, we need to apply ExtendedToStrictDimacs, which makes
   for gapless variable-numbers and correct parameter-values. </li>
   <li> However for this we need to improve
   InputOutput/ExtendedToStrictDimacs.cpp. See "Parameter handling" and
   "Extended Dimacs format" in
   Interfaces/InputOutput/plans/ExtendedToStrictDimacs.hpp. </li>
   <li> Also the renaming should be shown in the comments. Or perhaps
   better in an additional file? </li>
   <li> DONE
   After applying the partial assignment we have the correct number
   of clauses. And also the correct number of (occurring) variables could
   be determined. This could be passed to the renaming script. This then
   needed only one pass, without storing the instance (when outputting the
   variable-renaming to a separate file). </li>
   <li> Fastest however would be to integrate application of partial
   assignments and renaming. This can be done in one pass, without storage.
   At the end the new number of variables and clauses is known. </li>
   <li> DONE (glucose is not (shows wrong number of variables), and thus
   we need the renaming and the precise number of variables)
   Or are SAT solvers nowadays able to handle these things themselves?
   The OKsolver definitely is. </li>
  </ul>


  \todo Improving SplittingViaOKsolver
  <ul>
   <li> The computation of the md5sum-value of the directory Instances is very
   slow, and we need a dedicated application of it.
    <ol>
     <li> On cs-oksvr the computation for a directory with 3209535+1 elements
     took from Fri Aug 26 21:56:11 BST 2011 to Fri Sep  2 10:58:57 BST 2011.
     That is 6 1/2 days, where a day has ~ 90000s, so roughly only 6 md5sum-
     computations per second. </li>
     <li> See "Computing the
     md5sum-value of a directory" in Structures/Cryptology/plans/MHash.hpp.
     </li>
    </ol>
   </li>
   <li> DONE (also sorting the lines now)
   The computation of file Data is very slow: we need a little C++
   application for it. </li>
  </ul>


  \todo Good splitting
  <ul>
   <li> How to find good values for the d-parameter? </li>
   <li> Yet we first use the depth-interpretation, see how it works out, and
   then switch to the n-interpretation. </li>
   <li> This is reasonable for exploration, but of course for (just) solving
   instances we need to find a direct strategy. </li>
   <li> Still, if splitting doesn't take too long, one can first split via
   the depth-interpretation, estimating how much higher one can set d for the
   n-interpretation. </li>
   <li> Or one does n-splitting, makes a few samples from the last 100
   sub-instances in the Data-file, and if solving takes more than 1 or 2
   seconds, then one increases n. </li>
   <li> The rough idea is that conflict-driven solvers are "good for easy
   instances" (considering here a conflict-driven solver at the distributed
   end). </li>
  </ul>


  \todo Computing a splitting tree
  <ul>
   <li> In order to see what really needs to be done, for a given depth
   one actually wants also the list of indices of nodes which still need to be
   processed; if a satisfying assignment was found then the whole process
   can stop (likely we don't do counting in this framework). </li>
   <li> So this task is to compute a complete splitting tree according to
   ExperimentSystem/plans/DistributedSolving.hpp.
    <ol>
     <li> Here one could detect that branches don't need to be processed due
     to resolution tree pruning or autarky reduction. </li>
     <li> This could be achieved by forced backtracking when the given depth D
     is reached, assuming for the intelligent backtracking that all variables
     (respectively available) are used. Here then there are no "missed single
     nodes", but tree resolution pruning is always to be performed. </li>
    </ol>
   </li>
  </ul>


  \todo Distributing the tasks to machines
  <ul>
   <li> Some means are needed for allocating the sub-problem-instances to
   the available machines, monitoring their execution, recording the results,
   and scheduling new jobs if possible (and needed). </li>
  </ul>

*/

