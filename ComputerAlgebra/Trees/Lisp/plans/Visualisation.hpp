// Rui Wang, 21.10.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Trees/Lisp/plans/Visualisation.hpp
  \brief Plans regarding visualisation of trees using Gnuplot
  
  
  \todo Create milestones
  
  
  \todo A basic example of visualising a tree using Gnuplot
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


  \todo Full implementation of tree drawing using Gnuplot
  <ul> 
   <li> In the basic example, all points and edges must be specified manually.
   For the requirement of algorithmically extracting the data of points and edges
   from the results of reingold_tilford_rt, tdlrt2points and tdlrt2deges are
   provided. Then the extracted data can be used by draw_rt to perform tree
   drawing.  
   </li>
   <li> The usage of draw_rt. 
    <ul>
     <li> The following parameters must be provided and in correct order.
      <ol>
       <li> A binary unlabelled rooted tree T. </li>
       <li> The root coordinates p which is a list [x,y] contains a pair of 
       coordinates. </li>
       <li> The range for the x coordinate which is a list [x_min,x_max] contains
       a pair of extreme x coordinates. If the range is given as `auto', the
       range for the x coordinate will be computed automatically. </li>
       <li> The range for the y coordinate which is a list [y_min,y_max] contains
       a pair of extreme y coordinates. If the range is given as `auto', the
       range for the y coordinate will be computed automatically. </li>
       <li> The points size, it must be a non-negative number.(eg. 0, 1, 3.4) </li>
       <li> The points type. (dot, plus, multiply, asterisk, square, filled_square,
       circle, filled_circle, up_triangel, filled_up_triangle, down_triangle,
       filled_down_triangle, diamant, filled_diamant) </li>
       <li> The points colour. Colours can be given as names or in hexadecimal rgb
       code "#rrggbb". </li>
       <li> The edges colour. Colours can be given as names or in hexadecimal rgb
       code "#rrggbb". </li>
      </ol>
     </li>
     <li> An example: draw_rt([[],[]],[0,0],auto,auto,1,circle,red,blue). </li>
    </ul> 
   </li>
  </ul>

*/

