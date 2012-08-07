// Oliver Kullmann, 15.8.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/DistributedSolving/plans/ProcessSplitViaOKsolver.hpp
  \brief Plans regarding distributed SAT solving via SplittingViaOKsolver

  The two current tools are:
   - ProcessSplitViaOKsolver
   - ProcessiCNF


  \todo Better stopping
  <ul>
   <li> Just pressing "Ctrl-C" on the command-line doesn't work very well,
   since from the minisat-subprocess we get then "undetermined", which is fine,
   and the process continues, until after some time the main process is hit
   (when continuing pressing Ctrl-C). It is actually unclear why it stops
   then (typically declaring the task as completed)? </li>
   <li> The logic of using signal-trapping has to be reviewed. </li>
   <li> "pkill -TERM -P pid" might be a better option? </li>
  </ul>


  \todo More powerful processing options
  <ul>
   <li> For satisfiable instances it seems important to have the options to
   restrict the time per sub-instance:
    <ol>
     <li> With minisat-2.2.0, to e.g. restrict the time to 2 seconds, one uses
     the option "-cpu-lim=2". </li>
    </ol>
   </li>
   <li> We need more sorting options:
    <ol>
     <li> Perhaps the Data-file as produced by SplittingViaOKsolver is not
     sorted, but just kept in the original order, according the the original
     creation order. </li>
     <li> Then we have various little tools to produce different orders. </li>
     <li> One is, as it is now, largest assignments first (otherwise keeping
     the order). </li>
     <li> All orders for the full partial assignments as well as for
     decision-only. </li>
     <li> Then we consider a subinstance as given by a binary string s, which
     contains 0 for a first branch for the decision variable, and a 1 for a
     second branch for the decision variable (inferred assignments are ignored
     here for the sorting). Then the strings are reversed, and lexicographical
     order is used. This should be the order as consider by Heule/van-Maren.
     </li>
    </ol>
   </li>
   <li> The tools ProcessSplitViaOKsolver and ProcessiCNF should converge
   as much as possible. </li>
  </ul>


  \todo DONE (available now with ProcessiCNF)
  Option for using only decision variables with ProcessSplitViaOKsolver
  <ul>
   <li> First extracting the partial assignments (from file
   Instances/decisions), via some tool. </li>
   <li> And then just running through this file. </li>
  </ul>

*/
