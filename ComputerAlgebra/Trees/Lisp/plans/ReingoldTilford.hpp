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
   <li> Pseudocode
    <ul>
     <li> Define a type Tree and type Node.
      \verbatim
       Type Tree {
         Tree leftsubtree, rightsubtree;
         Node leftmost, rightmost;
         Node root;
       }
 
       Type Node {
         Integer x, y;
         Integer level;
       }
      \endverbatim
      </li>
      <li>
      \verbatim
       Function Tree RT(Tree T) {
 
        If T.leftsubtree == empty && T.rightsubtree == empty
           
           T.leftmost.x=0, T.rightmost.x=0;
           T.root.x=0, T.root.y=T.root.level;
           Return T;
        
        Else
          
          T.leftsubtree = RT(T.leftsubtree);
          T.rightsubtree = RT(T.rightsubtree);
 
          tempx = (T.leftsubtree.rightmost.x-T.rightsubtree.leftmost.x+2)/2; 
          T.leftsubtree = Translating(T.leftsubtree, [-tempx-T.leftsubtree.root.x, 0]);
          T.rightsubtree = Translating(T.rightsubtree, [tempx-T.rightsubtree.root.x, 0]);
          T.leftmost.x = T.leftsubtree.leftmost - tempx;
          T.rightmost.x = T.rightsubtree.rightmost + tempx;
          T.root.x = 0, T.root.y = T.root.level;
 
        Return T;
      }
      \endverbatim
      </li>
    </ul>
   </li>
  </ul> 


  \todo The full Reingold-Tilford algorithm
  <ul>
   <li> First we implement the "abstract" algorithm, which elegantly
   implements the idea, without taking care of efficiency. </li>
   <li> Then, additionally, the full algorithm is implemented, which refines
   the abstract version, making it linear-time. Here now we describe the
   full version. </li>
   <li> The Reingold-Tilford algorithm can be simply considered as a recursive
   algorithm, which consists of two traversals. The first one is a post-order
   traversal, and the second one is pre-order traversal. The post-order
   traversal determines the preliminary x coordinate and offset value of each
   node. Pre-order traversal produces final x coordinates. Y coordinates can 
   be easily defined as same as its level in the tree. </li>
   <li> Steps of coordinates computation:
    <ul>
     <li> Post-order traversal, recursively do the following steps from leaves.
      <ol>
       <li> If current node is the left child of its parent, its preliminary x 
       coordinate is 0. If current node is the right child of its parent, its 
       preliminary x coordinate is 0 plus the defined minimum distance between 
       two nodes. In the case, the defined minimum distance is 2, so its 
       preliminary x coordinate is 0+2=2.( If the current node is the only one 
       child of its parent, we define it as the left child.) </li>
       <li> Set the offset value of each node from leaves to root. The offset
       is calculated by subtracting the average value of preliminary x
       coordinates of the children of current node from the preliminary x
       coordinate of current node. For example, p is the parent of left child
       l and right child r. The preliminary x coordinates of p, l and r are 2,
       0 and 2 respectively so the offset of r is 2-(0+2)/2=1. If current node
       has only one child (left child), the offset is the preliminary x
       coordinate of current node minus 1 (1 is the result of the minimum
       distance 2 divided by 2). The offset of leaves are 0. </li>
       <li> In each level, the distance between the right most node of left 
       subtree and left most node of right subtree must be at least 2 which can
       be determined by rl-lr, which the lr is the preliminary x coordinate of 
       the right most node of left subtree plus the accumulation of all its
       ancestors in left subtrees, rl is the preliminary x coordinate of the 
       left most node of right subtree plus the accumulation of all its 
       ancestors in right subtrees. If the distance is strictly less than 2, 
       both the preliminary x coordinate and the  offset of the root of right 
       subtree need to be plus by 2-(RL-LR). This checking must be performed 
       in each level from the roots of left and right subtree until leaves.
       The step is performed every time before the post-order traversal
       go up to a higher level. </li>
      </ol>
     </li>
     <li> Pre-order traversal, recursively do the following step from root.
      <ol>
       <li> Set the final x coordinate of each node from root by accumulating 
       the offset of all the ancestors of current node, then plus its 
       preliminary x coordinate. </li>
      </ol> 
     </li>
    </ul>
   </li>
   <li> An example:
    <ul>
     <li> Define a tree T: [A, [B,[C,[D]],[E,[F],[G]]], [H,[I],[J,[K],[L]]]], 
     which has 12 nodes.
     \verbatim
           A
         /  \
         B   H 
        /\   /\
       C E  I J
      / /\   /\ 
     D F G  K L
     \endverbatim
       <ul>
        <li> Post-order traversal, order D,C,F,G,E,B,I,K,L,J,H,A
         <ol>
          <li> D is a leaf and left child, thus its preliminary x coordinate
          is 0, offset is 0. </li>
          <li> C is a left child and the parent of D, so its preliminary x 
          coordinate is 0, because D is the only one child of C, so the offset 
          of C is 0-1=-1. </li>
          <li> F is the same as D, so px=0, offset=0, px is used to indicate
          the preliminary x coordinate, it will be used in all of the
          following example. </li>
          <li> G is the right child of E, so px=0+2=2, offset=0. </li>
          <li> E is the right child of C, and the parent of F and G, so
          px=0+2=2, offset=2-(0+2)/2=1. </li>
          <li> Determine the separation between the right most node of left 
          subtree and the left most node of right subtree in same level, in
          the case, they are D and F .So use the method given above to
          calculate the distance between the two nodes, (0+1)-(0+(-1))=2,
          thus no further move needed. </li>
          <li> B px=0, offset=0-(0+2)/2=-1. </li>
          <li> I, K px=0, offset=0. </li>
          <li> L px=0+2=2, offset=0. </li>
          <li> J px=0+2=2, offset=2-(0+2)/2=1. </li>
          <li> I doesn't have any child, so we don't need perform the distance 
          check between the two subtrees of H. </li> 
          <li> H px=0+2=2, offset=2-(0+2)/2=1. </li>
          <li> Check the separation of E and I, (0+1)-(2+(-1))=0<2. So the px 
          and offset of H, which is the root of the subtree, need to be added 
          by 2-0=2. </li>
          <li> Updated H, px=2+2=4, offset=1+2=3. </li>
          <li> Go down the lower level of subtrees to check the separation of G
          and K, (0+1+3)-(2+(-1)+1)=2. </li>
          <li> It's the bottom of subtrees, so checking is completed. </li>
          <li> A px=0, offset=0-(0+4)/2=-2. </li>
          <li> Now, post-order traversal is finished. </li>
         </ol>
        </li>
        <li> Pre-order traversal, order A,B,C,D,E,F,G,H,I,J,K,L
         <ol>
          <li> A x=0, x indicates x coordinates, the followings are the same 
          meaning. </li>
          <li> B x=0+(-2)=-2. </li>
          <li> C x=0+(-2)+(-1)=-3. </li>
          <li> D x=0+(-2)+(-1)+(-1)=-4. </li>
          <li> E x=2+(-2)+(-1)=-1. </li>
          <li> F x=0+(-2)+(-1)+1=-2. </li>
          <li> G x=2+(-2)+(-1)+1=0. </li>
          <li> H x=4+(-2)=2. </li>
          <li> I x=0+(-2)+3=1. </li>
          <li> J x=2+(-2)+3=3. </li>
          <li> K x=0+(-2)+3+1=2. </li>
          <li> L x=2+(-2)+3+1=4. </li>
         </ol>
        </li>
       </ul>
     </li>
    </ul>
   </li>
  </ul>
      
      
  \todo Reingold-Tilford algorithm in Maxima. 
  <ul>
   <li> Define a Maxima function reingold_tilford_rt(T,[x,y]) (for the
   abstract version).
    <ul>
     <li> Input: An "unlabelled rooted binary tree" T and the coordinates
     [x, y] of its root. </li>
     <li> An "unlabelled rooted binary tree" is 
     recursively defined as a list [T_1, ..., T_n], where the T_i are
     the subtrees and the maximum value of n is 2. (The case n=0, i.e., 
     the empty list, represents the trivial tree.) </li>
     <li> Output: labelled T with coordinates at each node. </li>
    </ul>
   </li>
   <li> Examples:
    <ul>
     <li> Input: reingold_tilford_rt([], [x,y]); Output: [[x,y]] </li>
     <li> Input: reingold_tilford_rt([[],[]], [x,y]);
     Output: [[x,y],[[x-1,y-1]],[[x+1,y-1]]].
     </li>
    </ul>
   </li> 
  </ul> 
  
 
*/

