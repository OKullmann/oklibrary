// Oliver Kullmann, 9.7.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Visualisation/GraphDrawing/plans/Trees.hpp
  \brief Plans for drawing trees (especially search trees)


  \todo Create milestones.


  \todo Investigate existing software
  <ul>
   <li> See "Graph drawing" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Graphs.hpp. </li>
   <li> http://gridweb.rrc.uic.edu/twiki/bin/view/TreeVisualization/
    <ol>
     <li> Contains links to many existing implementations. </li>
     <li> See especially
     http://gridweb.rrc.uic.edu/twiki/bin/view/TreeVisualization/LargeTrees
     </li>
     <li> However, it seems that trees are always used as a kind of
     hierarchical organisation of information, while for search trees, as we
     study them, the focus is on the geometrical shape of the "whole tree"
     (which for the visualisation of hierarchical structures likely is
     completely irrelevant --- and thus you get the various space-saving
     drawings, which likely are irrelevant for us since they do not show,
     e.g., the sizes of the subtrees (for us very important). </li>
    </ol>
   </li>
  </ul>


  \todo Reingold-Tilford algorithm outline
  <ul>
   <li> See the MSc thesis of Warren Bailey (supervisor OK). </li>
   <li> Investigate colouring schemes (see discussion of OK with Mark Jones).
   </li>
   <li> The nodes are labelled with numerical vectors, and also the edges.
   </li>
   <li> Additionally we have also strings as labels, like branching variables
   (for the nodes) and branching literals (for the edges). </li>
   <li> First we implement the algorithm at Maxima/Lisp level.
    <ol>
     <li> See "Tree drawing" in ComputerAlgebra/Trees/Lisp/plans/general.hpp.
     </li>
     <li> Input is a tree, output is the same tree, but labelled with the
     coordinates. </li>
     <li> We have the basic form of the algorithm, just implementing directly
     the definitions (so very clear, but also very slow). </li>
     <li> And then we have the linear-time algorithm. </li>
    </ol>
   </li>
  </ul>


  \todo Four aesthetic criterions for Reingold-Tilford algorithm
  <ul>
   <li> The Reingold-Tilford algorithm realises four basic criterions for
   tree drawing. We need to discuss these criterions, and whether they are
   appropriate for our application. </li>
   <li> These four aesthetics for tree drawing, using a cartesian coordinate
   system, with root at (0, 0), growing downward, are as follows:
    <ol>
     <li> Aesthetics 1: Nodes at the same depth (length of path from root to
     node) of a tree should have same y-coordinate. Thus nodes at depth d
     should be positioned with y-coordinate -d. </li>
     <li> Aesthetics 2: A left child should be positioned with the
     x-coordinate that is strictly less than the x-coordinate of its
     parent; similarly, a right child should be positioned with the
     x-coordinate that is strictly more than the x-coordinate of its parent.
     </li> 
     <li> Aesthetics 3: A parent should be positioned with the x-coordinate 
     (L+R)/2 over its left child and right child, whose x-coordinates are L
     and R respectively. </li>
     <li> Aesthetics 4: The drawing of a tree and its mirror image should be 
     reflected to each other.
      <ol>
       <li> The mirror image is a tree in which the left subtree is the mirror
       image of the right subtree of the original tree and the right subtree
       is the mirror image of the left subtree of the original tree. </li>
       <li> The mirror image of a trivial tree is also a trivial tree. </li>
       <li> Furthermore, the drawing of equal subtrees should be the same
       regardless of the position in the tree (i.e., there is one translation
       vector (given by the positions of the roots of the subtrees)
       such that translating the coordinates of the drawing of one subtree 
       yields the coordinates of the other). </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo %Environment
  <ul>
   <li> We need to decide whether we go for
    <ol>
     <li> a general graph library, which includes graph drawing facilities,
     </li>
     <li> a special graph drawing library </li>
     <li> or just a general graphics library. </li>
    </ol>
    See "Graphics library" and "Graph visualisation library" in
    GraphDrawing/plans/general.hpp. </li>
  </ul>


  \todo Applications
  <ul>
   <li> The main application is the direct investigation of search trees
   as created by the OKsolver and other backtracking solvers. </li>
   <li> Labellings:
    <ol>
     <li> Nodes are labelled with some symbolic information (like the
     branching variable), which can be shown by clicking on the node. </li>
     <li> Then there is numerical information on the nodes, which can be
     selected, and then shown for the whole tree, using some colour scheme.
     </li>
     <li> Also the edges can carry numerical information; one could try also
     to use here some colour schemes. </li>
     <li> See "Applications" in GraphDrawing/plans/general.hpp. </li>
    </ol>
   </li>
   <li> These tools should also be useful to investigate branching heuristics
   according to the theory by OK as handled in
   ComputerAlgebra/Satisfiability/Lisp/BranchingTuples/plans/general.hpp. </li>
  </ul>

*/

