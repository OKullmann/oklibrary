// Oliver Kullmann, 26.12.2005 (Swansea)

/*!
  \file GraphDrawing/plans/general.hpp
  \brief Plans for the graph drawing module (as a whole)


  \todo Update namespaces.


  \todo Graphics library:
  The fundamental problem is to determine a suitable graphics library.
  <ul>
   <li> Possibilities:
    <ol>
     <li> Qt : very intrusive, and thus perhaps not the right choice </li>
     <li> OpenGL in the form of the Mesa library: Perhaps this is suitable. </li>
    </ol>
   </li>
   <li> This graphics library must offer the following functionality:
    <ol>
     <li> a window with graphical elements can be created </li>
     <li> graphical elements can be handled as objects, and placed on the canvas
     using coordinates </li>
     <li> graphical elements include at least pixels, points (in various sizes) and
     lines; it should be possible to vary their appearance (including giving
     them colours), and possibly these elements can be moved. </li>
    </ol>
    One should play around with Mesa to see what it can, and to estimate
    the quality of the library.
   </li>
  </ul>


  \todo External libraries / programs:
  <ul>
   <li> there is this C-program especially for drawing large trees XXX </li>
  </ul>


  \todo Concepts for rooted oriented trees and for graphs are needed.
  For graphs the Boost graph library is not optimal, but not too bad.


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
  \namespace OKlib::GraphDrawing
  \brief Tools for drawing graphs and trees
*/

namespace OKlib {
  namespace GraphDrawing {
  }
}
