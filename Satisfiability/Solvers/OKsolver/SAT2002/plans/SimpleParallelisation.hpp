// Oliver Kullmann, 21.4.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
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


  \todo Handling the information about the decision variables
  <ul>
   <li> DONE
   The information on the number of decision variables needs to be added
   to the data file. </li>
   <li> And we need a tool for extracting the CNF corresponding to the
   collection of partial assignments as DNF; the dual or the negation
   (the difference is just whether we negate all literals or not; on its
   own not a big issue, but one should create a natural connection). </li>
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
   <li> Also the renaming should be shown in the comments, Or perhaps
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
   slow, and we need a dedicated application of it. See "Computing the
   md5sum-value of a directory" in Structures/Cryptology/plans/MHash.hpp. </li>
   <li> DONE (also sorting the lines now)
   The computation of file Data is very slow: we need a little C++
   application for it. </li>
  </ul>


  \todo DONE
  Taking the length of the partial assignments into account
  <ul>
   <li> An alternative interpretation of the depth-parameter for the OKsolver
   is needed:
    <ol>
     <li> Not the depth of the tree, but the size of the partial assignment.
     </li>
     <li> As soon as the size is reached, splitting occurs. </li>
     <li> This should make more balanced splitting possible. </li>
    </ol>
   </li>
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
   <li> The rought idea is that conflict-driven solvers are "good for easy
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

