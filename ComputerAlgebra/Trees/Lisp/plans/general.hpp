// Oliver Kullmann, 12.1.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Trees/Lisp/plans/general.hpp
  \brief General plans regarding trees in Maxima/Lisp


  \todo Create milestones.


  \todo Reorganisation
  <ul>
   <li> Gather all general tree-functionality, and move it here. </li>
   <li> We handle trees in
    <ol>
     <li> ComputerAlgebra/Satisfiability/Lisp/Backtracking/SplittingTrees.mac
     </li>
     <li> ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/Formulas.mac
     </li>
     <li> ComputerAlgebra/Satisfiability/Lisp/BranchingTuples/Trees.mac
     </li>
    </ol>
   </li>
  </ul>


  \todo Concepts and abbreviations
  <ul>
   <li> An "unlabelled rooted tree" perhaps abbreviated by "rt". </li>
   <li> And a "labelled rooted tree" perhaps abbreviated by "lrt". </li>
   <li> The functions need to be renamed. </li>
  </ul>


  \todo Tree drawing (output as Latex code)
  <ul>
   <li> Generalise the various forms of transforming trees to ps-trees. </li>
   <li> The output should be nicely indented. </li>
   <li> We need general conventions on transferring boolean and propositional
   literals to Latex. </li>
   <li> We need docus on how to adjust the Latex-code to various standard
   circumstances (scaling, rotation, etc.). </li>
   <li> We also should offer output using "\xygraph". </li>
   <li> For that we need to compute the horizontal displacement. So the
   Rheingold-Tilford algorithm should be implemented. See
   Visualisation/GraphDrawing/plans/Trees.hpp. </li>
  </ul>


  \todo Graph drawing (output as Latex code)
  <ul>
   <li> It would be nice to have the possibility to output graphs
   in the "\xygraph"-format. </li>
   <li> And also using the pstricks-package. </li>
   <li> One possibility is to compute coordinates and then to adjust them
   to a grid. </li>
   <li> It would also be good if somehow one could specify rough placements.
   </li>
  </ul>

*/

