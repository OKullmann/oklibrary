// Oliver Kullmann, 21.4.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/SAT2002/plans/SimpleParallelisation.hpp
  \brief Plans on extending OKsolver-2002 for using it to compute a splitting of a large problem into subproblems


  \todo The basic idea
  </ul>
   <li> As a simple means to achieve parallelisation, for a given depth
   the clause-sets at that level should be output (to files, with
   suitable names indicating the decisions leading to that clause-set). </li>
   <li> This should simply employ the monitoring-mode, outputting the (reduced)
   clause-sets at the monitoring nodes, while not going into the branches, but
   immediately backtracking (stipulating result "unsat", using all variables,
   but at the end outputting "unknown" if no solution was found and at least
   one open node was created). </li>
   <li> Then a simple script would allocate these problem-instances to
   the available machines, monitor their execution, record the results,
   and schedule new jobs if possible (and needed). </li>
   <li> For monitoring-depth 0 one would get the OKsolver_2002 to act
   as preprocessor (applying r_2-reduction and autarky-reduction). </li>
  </ul>


  \todo Planning for computing the sub-problems
  <ul>
   <li> A new option "-S" (for "splitting") is introduced. </li>
   <li> The monitoring-depth D (>= 0) is also used here. </li>
   <li> When the OKsolver comes to branch at depth D, then the current
   partial assignment is output, representing the sub-problem to be solved,
   and for both branches the answer UNSAT is assumed, using all variables
   in the sub-problems. </li>
   <li> This is repeated until the complete problem is processed. </li>
   <li> If no open problems was created, then we output the decision. </li>
   <li> A counter open_problem_index=1,.. counts the open problems. </li>
   <li> The output goes into a directory whose name follows our usual
   standard for experiment-directories. </li>
   <li> Insight we have a file containing all the OKsolver-parameters,
   and the files 1.pass, ..., containing the partial assignments yielding
   the subproblems. </li>
   <li> DONE (we don't do this, but we compute all sub-problems, in the
   form of partial assignments, once at the beginning)
   Installing an additional parameter to just compute one node:
    <ol>
     <li> To handle a large number of jobs, it must also be possible to
     just output a certain clause-set from the long list; for example
     when using a depth D=13, then by specifying a number from 1 to 8192
     one should get the corresponding clause-set; this actually then
     didn't need to be output, but could just be processed. </li>
     <li> However it is less intrusive to just use the above output facilities
     to output the clause-set obtained and then to stop (for solving the
     instance then the OKsolver (or any other solver) is called again). </li>
     <li> A script would then just manage the distribution of the jobs
     to the available machines. </li>
     <li> The index (in the above case 1 <= i <= 8192) would be represented
     in binary, and the bits would specify the decisions; if a decision would
     be achieved on the way, then nothing needed to be done. </li>
     <li> Simplest is just to choose the branches according to the path
     given by i; if a decision is obtained earlier then there is no need
     to go further. </li>
     <li> So the simplest mode is just to follow a path given by a binary word,
     and either output SAT, UNSAT or the (reduced) clause-set finally reached.
      <ol>
       <li> Say the parameter would be "-P=word" (--path=word), where "word"
       is a string of 1's and 2's (first branch or second branch). </li>
       <li> Or perhaps better "-P=i" for 0 <= i < 2^D, with 0 meaning "left",
       and the higher the power-order the close to the root. </li>
       <li> Or using the enumeration indices 1 <= i <= 2^D. </li>
       <li> Whenever a decision for branching is needed, then this path is used
       as oracle. And the computation is stopped the first time the depth
       is reached. </li>
       <li> Then one also needed to output the index of the possible next
       branch (to continue). </li>
       <li> Perhaps the list of all indices (with jumps!) should be
       computed at the beginning, so that one doesn't need to process the
       branches from left to right, but can use for example a random order.
       </li>
       <li> This output to file "filename_OKs_indexlist", with one index
       per line. </li>
       <li> Printing out the total number of indices (if it is 2^D, then we
       just have all of them). </li>
       <li> Branchings with just one successor are ignored here. </li>
       <li> If an output flag is set, then the final clause-set is output
       to a file (if no decision was reached). </li>
       <li> Alternatively, not the final clause-set but the assignment leading
       to it, in form of unit-clauses, is output. This would minimise the
       communication. </li>
       <li> Just at the given depth the current assignment needs to be output.
       </li>
       <li> Perhaps to a file, with a standardised name, say
       "filename_OKs_i" for the index i. Should this output be in Dimacs
       format, that is, including the parameter line? Should n then be the
       attained maximum, or the original n? Let's say the original n. </li>
       <li> The program is exit then (using the standard exit, thus printing
       the statistics line). </li>
      </ol>
     </li>
    </ol>
   </li>
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

*/

