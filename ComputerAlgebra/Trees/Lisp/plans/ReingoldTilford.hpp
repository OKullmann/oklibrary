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
  
  
  \todo Reingold-Tilford algorithm at abstract level
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

  \todo The details of coordinates computation of Reingold-Tilford algorithm
  <ul>
   <li>The Reingold-Tilford algorithm can be simply considered as a recursive
   algorithm, which consists of two traversals.The first one is a post-order
   traversal, and the second one is pre-order traversal. The post-order
   traversal determines preliminary x coordinate and offset
   value of each node. Pre-order traversal produces the final x coordinate.
   Y-coordinates can be easily defined as same as its level in the tree.</li>
   <li>Steps of coordinates computation:
    <ol>
     <li>Recusively set the root's preliminary x coordinate of left subtree
     to 0, and the root's preliminary x coordinate of right subtree to 0 plus
     the minimum distance between two nodes, in the case, the minimum distance 
     is 2, so the root's preliminary x coordinate of right subtree is 2.</li>
     <li>In each level, the distance between the right most node of left 
     subtree and left most node of right subtree must be at least 2 which can
     be determined by rl-lr, which the lr is the preliminary x coordinate of 
     the right most node of left subtree plus the accumulation of all its ancestors in left subtrees, rl is the preliminary x coordinate of the 
     left most node of right subtree plus the accumulation of all its 
     ancestors in right subtrees. If the distance is strictly less than 2, 
     both the preliminary x coordinate and the  offset of the root of right subtree need to be plus by 2-(RL-LR). This checking must be performed 
     in each level from the roots of left and right subtree until leaves.
     The step is performed everytime before the post-order traversal
     go up to a higher level.</li>
     <li>Set the offset value of each node from leaves to root. The offset is 
     calculated by subtracting the average value of preliminiary x coordinates
     of the children of current node from the preliminary x coordinate of 
     current node. For example, p is the parent of left child l and right 
     child r. The preliminary x coordinates of p, l and r are 2, 0 and 2 respectively so the offset of r is 2-(0+2)/2=1. If current node has 
     only one child, the offset is preliminary x coordinate of current node
     minus 1. The offset of leaves are 0.</li>
     <li>Recusively set the final x coordinate of each node from root by accumulating the offset of all the ancestors of current node, then 
     plus its preliminary x coordinate.</li>
    </ol>
   </li> 
  </ul>


      
      
  \todo Reingold-Tilford algorithm in Maxima. 
  <ul>
   <li> Define a Maxima function reingold_tilford_rt(T,x,y).
    <ul>
     <li> Input: An "unlabelled rooted binary tree" T and the coordinates
     x, y of its root. </li>
     <li> An "unlabelled rooted binary tree" is 
     recursively defined as a list [T_1, ..., T_n], where the T_i are
     the subtrees and the maximum value of n is 2. (The case n=0, i.e., 
     the empty list, represents the trivial tree.) </li>
     <li> Output: labelled T with coordinates at each node. </li>
    </ul>
   </li>
   <li> Examples:
    <ul>
     <li> Input: reingold_tilford_rt([],x,y); Output: [[x,y]] </li>
     <li> Input: reingold_tilford_rt([[],[]],x,y); Output: [[x,y],[[x-1,y-1]],[[x+1,y-1]]].
     </li>
    </ul>
   </li> 
  </ul> 
 
*/

