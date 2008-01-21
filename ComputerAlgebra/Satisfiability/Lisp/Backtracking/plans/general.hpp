// Oliver Kullmann, 30.11.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/general.hpp
  \brief Plans for Maxima-components regarding backtracking algorithms


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo optimal_splitting_tree in
  ComputerAlgebra/Satisfiability/Lisp/Backtracking/SplittingTrees.mac
  <ul>
   <li> Perhaps storing the trees also for the worst case. </li>
   <li> When searching only for the best case, then branches which
   cannot improve the current minimal size can be cut off. </li>
   <li> Instead of minimising the number of overall nodes, that is,
   minimising the number of leaves (since every inner node has exactly
   two successors), one can also minimise the number of true-leafes (the
   number of clauses in the DNF-representation) or the number of false-leaves
   (the number of clauses in the CNF-representation). </li>
   <li> Introduce an additional parameter d (from 0 to inf), and for depth < d
   then the orbits of the automorphism group acting on the set of 2n literals
   is computed, and instead of evaluated all 2n trees only one element of
   each orbit is chosen (evaluated appropriately).
    <ol>
     <li> See "Symmetries" in
     ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp. </li>
    </ol>
   </li>
   <li> Instead of just getting the minimal size, a variations return
   the complete distribution (for every occurring tree size the number of trees
   for that size). </li>
  </ul>


  \todo Processing splitting trees
  <ul>
   <li> Latex output of splitting trees by "tex_st" in
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/SplittingTrees.mac
    <ol>
     <li> For the docu:
      <ul>
       <li> Use "\usepackage{pstricks,pst-node,pst-tree}". </li>
       <li> For the outermost "pstree" preferably the optional argument
       "[nodesep=2pt]" is added (otherwise the edges are too close to
       the nodes). </li>
       <li> Via "[nodesep=2pt,levelsep=20pt]" furthermore the distance
       between levels could be reduced. </li>
       <li> And via "treemode=r" the tree grows from left to right. </li>
       <li> The graphics is only viewable via the ps-file (use dvips);
       for creating pdf-files, first create a ps-file, and then apply
       ps2pdf. </li>
      </ul>
     </li>
     <li> Yet everything on one line --- could we add nice indentation? </li>
    </ol>
   </li>
   <li> Resolution tree creation
    <ol>
     <li> Write a function which takes a splitting for an unsatisfiable
     clause-set F, and F as second argument, and produces a resolution
     tree. </li>
     <li> Apply tree pruning if an unnecessary splitting was applied. </li>
     <li> A little helper function is needed which returns for a partial
     assignment phi and a clause-set F the clauses of F falsified by
     phi. </li>
    </ol>
  </ul>


  \todo optimal_r_splitting_tree in
  ComputerAlgebra/Satisfiability/Lisp/Backtracking/SplittingTrees.mac
  <ul>
   <li> Given an r_k-splitting tree for F, and F, counting the satisfying
   assignments. </li>
   <li> DONE (amend_r_splitting_tree does the job, for each node : the
   assignments are not unique anyway, only the results)
   We need the notion of an "extended r_k-splitting tree", which at
   each node contains also the (forced) partial assignment leading to the
   reduced clause-set at this node (except for the case where an inconsistency
   has been found, since here this partial assignment is not well-defined).
   </li>
  </ul>


  \todo constraint_backtracking in
  ComputerAlgebra/Satisfiability/Lisp/Backtracking/ConstraintSatisfaction.mac
  <ul>
   <li> Also output the corresponding splitting trees; perhaps they
   contain the enforced assignments. </li>
   <li> Given such splitting trees, again one can represent all solutions,
   and/or count all solutions. </li>
   <li> The simple code for "constraint_backtracking_counting" shows
   the big problem of the notion of "propagators": Only total solutions
   are recognised! The generalised SAT approach should overcome this. </li>
  </ul>


  \todo Heuristics in
  ComputerAlgebra/Satisfiability/Lisp/Backtracking/ConstraintSatisfaction.mac
  <ul>
   <li> For the purpose of comparisons implement also the 3 other heuristics
   from [Beek, 2006, Backtracking Search Algorithms] (which use only the
   the domain sizes from the look-ahead).
    <ol>
     <li> Brown and Purdom: Minimise the remaining domain size + the
     minimum over all other variables of the sum of their domain
     sizes in the branches. </li>
     <li> Geelen: Minimise for a variable the sum over its values
     of the product of the remaining domain sizes over all other
     variables. (This is closest to our, but it replaces the tau-rule
     by the sum, which should be a bad approximation.) </li>
     <li> Freeman: As Geelen, but instead of taking the product again
     the sum is chosen. </li>
    </ol>
    To me (OK) this all looks like guessing around, where the true rule is
    the tau-rule of variable_heuristics_tau.
   </li>
   <li> The heuristics should be optimal, given that only the domain sizes
   for the potential branches are available ?! </li>
  </ul>


  \todo Further propagators
  <ul>
   <li> "Enigma 1277" (see
   Satisfiability/Transformers/Generators/FaronsPuzzle.cpp) </li>
  </ul>

*/

