// Rui Wang, 21.10.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Visualisation/GraphDrawing/plans/ReingoldTilford.hpp
  \brief Plans regarding visualisation of trees produced by Reingold-Tilford algorithm using Gnuplot
  
  
  \todo Create milestones
  
  
  \todo A basic example of visualising a tree using Gnuplot
  <ul>
   <li> The coordinates of points using in this example is produced by the
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

*/

