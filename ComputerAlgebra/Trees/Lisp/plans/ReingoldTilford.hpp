// Rui Wang, 28.09.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Trees/Lisp/plans/ReingoldTilford.hpp
  \brief Plans regarding Reingold-Tilford algorithm in Maxima/Lisp
  
  
  \todo Create milestones
  
  
  \todo Reingold-Tilford algorithm in abstract level
  <ul>
   <li> See "Four aesthetic criterions for Reingold-Tilford algorithm" 
   in Visualisation/GraphDrawing/plans/Trees.hpp. </li>
   <li> Reingold-Tilford algorithm.
    <ul>
     <li> Input: a binary tree T. </li>
     <li> Output: a layered drawing of T. </li>
     <li> Base case: If T consists of a single node, its drawing is 
     trivially defined. </li>
     <li> Divide: Recursively apply the algorithm to draw the left and
     right subtrees of T. </li>
     <li> Conquer: 
      <ol>
       <li> Move the drawings of the subtrees towards each other 
       until their horizontal distance becomes equal to 2. </li>
       <li> Place the root r of T vertically one unit above and horizontally
       half way between its children. </li>
       <li> If r has only one subtree, say the left one, the place r 
       at horizontal distance 1 to the right of its left child. </li>
      </ol>
     </li>
    </ul>
   </li>
  </ul> 
      
      
  \todo Reingold-Tilford algorithm in Maxima. 
  <ul>
   <li> Define a Maxima function rt(T,x,y).
    <ul>
     <li> Input: An "unlabelled rooted binary tree" T and the x and y 
     coordinates of its root. An "unlabelled rooted binary tree" is 
     recursively defined as a list [T_1, ..., T_n], where the T_i are
     the subtrees and the maximum value of n is 2. (the case n=0, i.e., 
     the empty list, represents the trivial tree.) </li>
     <li> Output: T with coordinates of each node. </li>
    </ul>
   </li>
   <li> Examples:
    <ul>
     <li> Input: rt([],x,y); Output: [[x,y]] </li>
     <li> Input: rt([[],[]],x,y); Output: [[x,y],[[x-1,y-1]],[[x+1,y-1]]] </li>
    </ul>
   </li> 
  </ul> 
 
*/

