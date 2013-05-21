// Rui Wang, 21.10.2009 (Swansea)
/* Copyright 2009, 2010, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Trees/Lisp/plans/Visualisation.hpp
  \brief Plans regarding visualisation of trees


  \bug False labels at the leaves
  <ul>
   <li> The following needs to be updated; see also "Specify draw_lrt"
   below. </li>
   <li> draw_rt creates labels at the leaves, which makes no sense for trees
   where the leaves have no labels. </li>
   <li> So it has no place in "draw_rt" which is about *unlabelled* trees.
   </li>
   <li> There needs to be another version, "draw_lrt", which takes labels
   into account. </li>
   <li> And printing out the labels is a bad idea --- it's just unreadable.
   </li>
   <li> Obviously, for leaf-labels with two possibilities, one should just use
   shape and/or colour for the leaves to differentiate the two possibilities.
   </li>
   <li> It seems the function should not just be called "draw_lrt", but
   "draw_lrt_X", where X gives information on the special assumptions about
   the labels: For different assumptions one has different drawing functions.
   </li>
   <li> For example "draw_lrt_bl" for boolean at the leaves (while
   labels for inner nodes are currently ignored. </li>
   <li> Actually, a reasonable easy way to show the labels at (arbitrary) nodes
   is just to print them instead the node-symbols; one should use an additional
   parameter d, a natural number >= -1, and for nodes of depth <= d then
   the label is printed, while otherwise the (normal) node-symbol is shown.
   </li>
   <li> So we would have "draw_lrt_d", where the labels of nodes are just
   printed up to depth d, and "draw_lrt_dbl", which treats the leaves
   differently (requiring the labels there of special form, namely just
   booleans, and showing them out graphically). </li>
   <li> In order to be able to cope with labels, obviously we also need
   a version reingold_tilford_lrt, which just ignores the labels, but
   *keeps them*. </li>
  </ul>


  \todo Specify draw_lrt
  <ul>
   <li> What are the requirements on the node-labels of T? </li>
   <li> There needs to be a general version which just accepts arbitrary
   terms as labels. </li>
   <li> Compare with "False labels at the leaves" above. </li>
   <li> Likely "draw_lrt" shouldn't make any assumptions on the labels,
   only more specialised versions perform special drawings. </li>
   <li> Especially we need a version which doesn't make a special case for
   leaves. </li>
  </ul>


  \todo Better organisation
  <ul>
   <li> Currently, ComputerAlgebra/Trees/Lisp/Visualisation.mac is pretty
   unorganised, which helper and main functions mixed. </li>
  </ul>
  
  
  \todo Create milestones
  
  
  \todo DONE
  A basic example of visualising a tree using Gnuplot
  <ul>
   <li> The coordinates of points used in this example are produced by the
   Reingold-Tilford algorithm; for the details please refer to
   ComputerAlgebra/Trees/Lisp/plans/ReingoldTilford.hpp. </li>
   <li> Define p as a list of points:
   \verbatim
   p: points([[0,0],[-2,-1],[2,-1],[-3,-2],[-1,-2],[1,-2],[3,-2],[-4,-3],
   [-2,-3],[0,-3],[2,-3],[4,-3]]);
   \endverbatim </li>
   <li> Define a 2-dimensional drawing scene:
   \verbatim
   s: gr2d(xrange = [-15,15],
   yrange = [-8,8],
   point_size = 4,
   point_type = circle,
   color = red,
   p,
   line_width = 1,
   polygon([[0,0],[-2,-1]]),
   polygon([[0,0],[2,-1]]),
   polygon([[-2,-1],[-3,-2]]),
   polygon([[-2,-1],[-1,-2]]),
   polygon([[-3,-2],[-4,-3]]),
   polygon([[-1,-2],[-2,-3]]),
   polygon([[-1,-2],[0,-3]]),
   polygon([[2,-1],[1,-2]]),
   polygon([[2,-1],[3,-2]]),
   polygon([[3,-2],[2,-3]]),
   polygon([[3,-2],[4,-3]]));
   \endverbatim </li>
   <li> Perform the drawing using "draw(s);". </li>
   <li> The task is now, given a tree labelled with 2-dimensional coordinates,
   to extract this data algorithmically. For that it would be helpful to have
   more explanations on the above. </li>
  </ul> 


  \todo DONE Full implementation of tree drawing using Gnuplot
  <ul> 
   <li> In the basic example, all points and edges must be specified manually.
   For the requirement of algorithmically extracting the data of points and
   edges from the results of reingold_tilford_rt, tdlrt2points and tdlrt2deges
   are provided. Then the extracted data can be used by draw_rt to perform tree
   drawing.  
   </li>
   <li> The usage of draw_rt. 
    <ul>
     <li> The function takes two parameters, the first one must be a binary
     unlabelled rooted tree; the second one is a list containing drawing
     attributes. If an empty list [] is provided, the automatic computation and
     drawing will be performed. Otherwise, the drawing attributes in the list
     can be defined as [name_1, value_1, ... , name_N, value_N], n >= 0. Each
     attribute contains two fields, which are name and value, both of which
     must be provided. Possible attribute names are 'root', 'x_ran', 'y_ran',
     'p_size', 'p_type', 'p_colour', 'e_colour'. The details are listed below.
      <ol>
       <li> root: the root coordinates which is a list [x,y] contains a pair
       of coordinates. </li>
       <li> x_ran: the range for the x coordinate which is a list
       [x_min,x_max] contains a pair of extreme x coordinates. </li>
       <li> y_ran: the range for the y coordinate which is a list
       [y_min,y_max]  contains a pair of extreme y coordinates. </li>
       <li> p_size: the points size, it must be a non-negative number.
       (eg. 0, 1, 3.4) </li>
       <li> p_type: the points type. (Possible values: dot, plus, multiply,
       asterisk, square, filled_square, circle, filled_circle, up_triangel,
       filled_up_triangle, down_triangle, filled_down_triangle, diamant,
       filled_diamant). </li>
       <li> p_colour: the points colour. Colours can be given as names or in
       hexadecimal rgb code "#rrggbb". </li>
       <li> e_colour: the edges colour. Colours can be given as names or in
       hexadecimal rgb code "#rrggbb". </li>
      </ol>
     </li>
     <li> Somes examples: draw_rt([[],[]],[]); draw_rt([[],[[],[]]],[root,[1,1]]). </li>
    </ul> 
   </li>
  </ul>


  \todo Colouring schemes
  <ul> 
   <li> Handle labelled trees where the labels are lists of values (constant
   length). Now the label is a list which contains just one value. </li>
   <li> Investigate RGB colour model which will be used for the colouring
   schemes. </li>
  </ul>
  
  
  \todo draw_lrt_x
  <ul>
   <li> "draw_lrt_X", where X gives information on the special assumptions
   about the labels: For different assumptions one has different drawing
   functions. </li>
  </ul> 
  
  
  
*/

