// Oliver Kullmann, 2.11.2005

/*!
  \namespace GraphDrawing
  \brief Tools for drawing graphs and trees
  \todo The fundamental problem is to determine a suitable graphics library.
  Yet I see two possibilities:
   - Qt : very intrusive, and thus perhaps not the right choice
   - OpenGL in the form of the Mesa library: Perhaps this is suitable.
  This graphics library must offer the following functionality:
   - a window with graphical elements can be created
   - graphical elements can be handled as objects, and placed on the canvas
     using coordinates
   - graphical elements include at least pixels, points (in various sizes) and
     lines; it should be possible to vary their appearance (including giving
     them colours), and possibly these elements can be moved.
     One should play around with Mesa to see what it can, and to estimate
     the quality of the library.
  \todo Second concepts for rooted oriented trees and for graphs are needed.
  For graphs the Boost graph library is not optimal, but not too bad.
  For trees the Rheingold-Tilford algorithm is to be implemented (see the MSc
  thesis of Warren Bailey), adorning the tree with graphical elements, which
  then can be drawn. For graphs one or two standard graph drawing algorithm
  from the literature should be implemented, adorning in the same way the graph
  with graphical elements.
  \todo Applications:
   - SAT solvers output the search tree in xml format; the tree is drawn, and at least
   one selected value labeling the vertices is visualised using some colour scheme
   (here another module "Visualisation" is needed, providing different methods
   for mapping numbers to colours).
   - The various (multi)graphs associated with CNFs should be visualised
   (static snapshots, and also dynamic simulations).
   - For CS-232 a visualisation of the graph_traversal algorithm is needed, starting
   with a drawing of some graph, where first all vertices and edges are black,
   and then stepwise vertices get marked red, and edges are blinking if they
   are just examined, yellow if they are in the buffer, green if the are tree edges,
   blue if they are back edges (perhaps dashed as long as they are still in the
   buffer). At the end every edge made the transition from black through
   yellow to either green or blue.
   - For CS-342 a backtracking tree labeled with clause-sets would be useful
     (possible output in Latex-format).
  \todo It should be possible to create ps-, png- and Latex-files from the drawings.
*/

/*!
  \file TreeDrawing.hpp
  \brief Various algorithms for drawing rooted oriented trees.
*/

#ifndef TREEDRAWING_kjjalfgt56

#define TREEDRAWING_kjjalfgt56

namespace OKlib {

  namespace GraphDrawing {

  }

}

#endif
