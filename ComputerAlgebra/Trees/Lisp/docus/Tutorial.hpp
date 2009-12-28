// Rui Wang, 21.11.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Trees/Lisp/docus/Tutorial.hpp
  \brief Tutorial on trees in Maxima/Lisp


  <h1> %Trees via Maxima in the OKlibrary </h1>

  <ol>
   <li> For the OKlibrary, An "unlabelled rooted tree" ("rt") is recursively 
   defined as a list [T_1, ..., T_n], where the T_i are the subtrees (the case 
   n=0, i.e., the empty list, represents the trivial tree).
   \verbatim
(%i1) T:[[],[[],[]]];
(%o1) [[],[[],[]]]
   \endverbatim
   </li>
   <li> Similarly, a "labelled rooted tree" ("lrt") is recursively defined as a
   list [L, T_1, ..., T_n], n >= 0, where L is the label, and the T_i are the
   subtrees.
   \verbatim
(%i2) T:[1,[2],[3,[4],[5]]];
(%o2) [1,[2],[3,[4],[5]]]
   \endverbatim
   </li>
  </ol>


  <h1> Tree drawing algorithm </h1>
  
  <ul>
   <li> Currently, Reingold-Tilford algorithm is used for producing layered
   tree drawing. we implemented reingold_tilford_rt in Maxima.
   Given an unlabelled rooted tree and the root coordinate as input, a 
   labelled rooted tree with 2-dimensional coordinates will be produced.  
   \verbatim
(%i27) reingold_tilford_rt([[],[]],[0,0]);
(%o27) [[[0,0]],[[[-1,-1]]],[[[1,-1]]]]
   \endverbatim
   </li>
  </ul>
  
  
  <h1> Visualisation of trees </h1>
  
  <ul>
   <li> Both unlabelled and labelled rooted trees can be handled and visualised via Maxima in the OKlibrary. For splitting trees we use a different function to handle them. A splitting tree is a labelled rooted tree, where each inner node is labelled by a number (natural number) while leaves are labelled by the boolean values 'true' or 'false'. In the following part, we will give the detailed usage of tree drawing functions. 
    <ol>
     <li> draw_rt
      <ul>
       <li> draw_rt can handle unlabelled rooted trees; at least one argument (an unlabelled rooted tree T) must be given, all other possible parameters are optional. The default value will be set if any optional parameter is not given.
       </li>
       <li> Possible parameters are listed as follows:
        <ul>
         <li> T : an unlabelled rooted tree. </li>
         <li> p : the root coordinates of the tree [x,y]; default [0,0]. </li>
         <li> xran : the range for the x coordinate [x_min,x_max]; default auto. </li>
         <li> yran : the range for the y coordinate [y_min,y_max]; default auto. </li>
         <li> pts : the size of a point (a non-negative number); default computed. </li>
         <li> ptt : the type of points (either as name or as integer). Available names and integers are: $none (-1), dot (0), plus (1), multiply (2), asterisk (3), square (4), filled_square (5), circle (6), filled_circle (7) *default*, up_triangle (8), filled_up_triangle (9), down_triangle (10), filled_down_triangle (11), diamant (12), filled_diamant (13).
         </li>
         <li> ptc : the colour of points (red, blue, ...); default red. </li>
         <li> edgc  : the colour of edges (red, blue, ...); default blue. </li>
         <li> output: if output is set to "true", the tree drawing will be output to a file called "output.eps" in the current directory. Otherwise, the             tree drawing will be displayed normally. </li>
        </ul>
       </li>
       <li> T is a compulsory parameter that can be accepted by a function call in the following ways:
        <ol>
         <li> Set T in global (e.g. T:value; then, draw_rt()). </li>
         <li> Use "T:value" as an argument. (e.g. draw_rt(T:value)). </li>
         <li> Use the "value" of T as the first argument. (e.g. draw_rt(value)). </li>
        </ol>
       The other parameters are optional, the usage of which are similar to the
       parameter T which is listed above except 3. Furthermore, the optional 
       parameters can be set to "unknown" (e.g. draw_rt(pts:unknown)) so that
       the default values of the parameters will be used.
       </li>
      </ul>
     </li>
     <li> draw_lrt
      <ul>
       <li> draw_lrt handles labelled rooted trees. RGB colour model is used for the colouring schemes. The usage of the function is similar to draw_rt except the compulsory parameter T which is a labelled rooted tree here. (Please refer to draw_rt).
       </li>
      </ul>
     </li>
     <li> draw_st 
      <ul>
       <li> draw_st is used for visualising splitting trees. For visualisation of splitting trees, we just print the labels of inner nodes up to depth d, where d is a natural number >= -1, and for the inner nodes of depth <= d   then the labels are printed, while otherwise nothing shows. Leaves are   treated differently, where the leaves that are labelled by "true" show the   node-symbol with red colour, otherwise grey colour.
       For draw_st, the compulsory parameter T is a splitting tree. Some optional parameters are listed below:
        <ul>
         <li> lbc : the colour of labels; default red. </li>
         <li> tc : the colour of true-leaves; default red. </li>
         <li> fc : the colour of false-leaves; default grey. </li>
         <li> d : the maximum depth which the labels will be printed. (d is a natural number >= -1). </li>
        </ul>
       For the usage and other possible parameters, please refer to draw_rt.
       </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>

*/

