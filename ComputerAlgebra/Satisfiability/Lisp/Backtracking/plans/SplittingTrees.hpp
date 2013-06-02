// Oliver Kullmann, 7.8.2008 (Swansea)
/* Copyright 2008, 2009, 2011, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/SplittingTrees.hpp
  \brief Plans on finding and handling splitting trees


  \todo Connections
  <ul>
   <li> See "Finding short tree-resolution proofs" in
   ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/Search.hpp. </li>
  </ul>


  \todo Write tests
  <ul>
   <li> Optimal splitting trees:
    <ol>
     <li> We need test cases for satisfiable instances! </li>
     <li> See the CS-342-script for already established optimal
     trees (which can be used as test data). </li>
    </ol>
   </li>
  </ul>


  \todo optimal_splitting_tree and optimal_r_splitting_tree
  <ul>
   <li> Improving "branch and bound" for the version which only seeks to find
   a best tree (not more exhaustive statistics):
    <ol>
     <li> Perhaps a heuristic can be used which tries to choose the best
     variable first and the worst branch (this to get the bounds tigther for
     the second branch). </li>
     <li> The problem seems to be that for the first branch currently we have
     only a very weak bound. But for that case the second branch had a very
     tight bound. So for that one could actually first consider the second
     branch whether it can produce the trivial tree, and if not then
     the upper bound for the first branch can be lowered. This looks reasonable
     in any case. </li>
     <li> This looks like a combination of upper and *lower* bounds,
     which are explored in a more distributed fashion. </li>
     <li> But likely this heuristic versions get more complicated, and should
     be realised by a different function. </li>
    </ol>
   </li>
   <li> Perhaps storing the trees also for the worst case. </li>
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
     <li> Alternatively, one keeps a repository of clause-sets already
     processed, and does not process isomorphic cases; similarly to
     e,g, derived_hitting_cs_pred_isoelim. </li>
     <li> The repository can comprise all of the clause-sets considered, or
     only those considered at the current node. </li>
    </ol>
   </li>
   <li> Instead of just getting the minimal size, a variations return
   the complete distribution (for every occurring tree size the number of trees
   for that size). </li>
   <li> DONE When searching only for the best case, then branches which
   cannot improve the current minimal size can be cut off. </li>
  </ul>


  \todo Processing splitting trees
  <ul>
   <li> Latex output of splitting trees by "tex_st" in
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


  \todo optimal_r_splitting_tree
  <ul>
   <li> Given an r_k-splitting tree for F, and F, counting the satisfying
   assignments. </li>
   <li> With the current algorithm computing an optimal r_2-splitting
   tree for weak_php_fcs(6,5) is hopeless. Hopefully, as described above,
   introducing also lower bounds enables us to process this. </li>
   <li> DONE (amend_r_splitting_tree does the job, for each node : the
   assignments are not unique anyway, only the results)
   We need the notion of an "extended r_k-splitting tree", which at
   each node contains also the (forced) partial assignment leading to the
   reduced clause-set at this node (except for the case where an inconsistency
   has been found, since here this partial assignment is not well-defined).
   </li>
  </ul>

*/

