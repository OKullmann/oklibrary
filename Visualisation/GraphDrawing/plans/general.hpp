// Oliver Kullmann, 26.12.2005 (Swansea)
/* Copyright 2005 - 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file GraphDrawing/plans/general.hpp
  \brief Plans for the graph drawing module (as a whole)


  \todo Update namespaces.


  \todo Graphics library
  <ul>
   <li> The fundamental problem is to determine a suitable graphics library.
   </li>
   <li> Possibilities:
    <ol>
     <li> Qt : very intrusive, and thus perhaps not the right choice </li>
     <li> OpenGL in the form of the Mesa library: Perhaps this is suitable.
     </li>
     <li> Perhaps easiest is FLTK http://www.fltk.org/ . </li>
    </ol>
   </li>
   <li> This graphics library must offer the following functionality:
    <ol>
     <li> a window with graphical elements can be created; </li>
     <li> graphical elements can be handled as objects, and placed on the
     canvas using coordinates; </li>
     <li> graphical elements include at least pixels, points (in various sizes)
     and lines; it should be possible to vary their appearance (including
     giving them colours), and possibly these elements can be moved; </li>
     <li> regarding drawing big trees, we need to zoom in and out easily. </li>
    </ol>
    One should play around with Mesa and FLTK to see what they can, and to
    estimate the quality of these libraries.
   </li>
  </ul>


  \todo Graph visualisation library
  <ul>
   <li> http://www.ogdf.net/doku.php looks like a
   reasonable choice; but apparently not much newer developments. </li>
   <li> Then perhaps we don't need the graphics library? </li>
   <li> See "Graph drawing" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Graphs.hpp. </li>
  </ul>


  \todo External libraries / programs:
  <ul>
   <li> there is this C-program especially for drawing large trees XXX </li>
  </ul>


  \todo Concepts
  <ul>
   <li> Concepts for rooted oriented trees and for graphs are needed. </li>
   <li> For graphs the Boost graph library is not optimal, but not too bad.
   </li>
   <li> See "Graph drawing" in ComputerAlgebra/Graphs/Lisp/plans/general.hpp.
   </li>
   <li> Important for communication is to get an overview on
   graph-file-formats, both for "abstract" and for "drawn" graphs.
    <ol>
     <li> See "Input/Output" in Graphs/BoostSupport/plans/general.hpp. </li>
    </ol>
   </li>
  </ul>


  \todo Applications
  <ul>
   <li> Likely the different sub-points deserve their own modules. </li>
   <li> SAT solvers output the search tree in xml format; the tree is drawn,
   and at least one selected value labeling the vertices is visualised using
   some colour scheme; see GraphDrawing/plans/Trees.hpp.
    <ol>
     <li> The information for each node varies from solver to solver, so we
     need several xml-fields. </li>
     <li> Possible information is
      <ul>
       <li> The branching variable. </li>
       <li> Number of r_k-reductions for available k. </li>
       <li> Number of pure literals. </li>
       <li> Autarkie reductions. </li>
       <li> Measures for the clause-set. </li>
       <li> Time needed to process the node. </li>
      </ul>
      Should we treat this information all as numerical? The branching variable
      is different --- here we need the precise value (and also a name is
      attached to it).
     </li>
     <li> Besides node-labellings with numerical values we also have
     edge labellings:
      <ul>
       <li> The distances for the branching (the one used to select the
       branching, and also additional ones). <li>
       <li> The probability in the probability distribution corresponding
       to the branching tuple (see
       Heuristics/StatisticalAnalysis/plans/BacktrackingProbing.hpp) </li>
       <li> the optimal probability (from the canonical probability distribution
       of the tree; see OK's SAT-handbook article on heuristics) </li>
       <li> The estimated probability for satisfiability (for choosing the
       first branch; see
       Heuristics/StatisticalAnalysis/plans/SatisfiabilityPrediction.hpp).
       </li>
      </ul>
      As a non-numerical labelling we have the branching literal.
     </li>
    </ol>
    This all for backtracking-solvers; conflict-learning is difficult. </li>
   </li>
   <li> The various (multi)graphs associated with CNFs should be visualised
   (static snapshots, and also dynamic simulations). </li>
   <li> For CS-232 a visualisation of the graph_traversal algorithm is needed,
   starting with a drawing of some graph, where first all vertices and edges
   are black, and then stepwise vertices get marked red, and edges are
   blinking if they are just examined, yellow if they are in the buffer,
   green if the are tree edges, blue if they are back edges (perhaps dashed as
   long as they are still in the buffer). At the end every edge made the
   transition from black through yellow to either green or blue. </li>
   <li> For CS-342 a backtracking tree labeled with clause-sets would be useful
   (possible output in Latex-format). </li>
  </ul>


  \todo Conversions
  <ul>
   <li> It should be possible to create ps-, png- and Latex-files from the
   drawings. </li>
  </ul>

*/

/*!
  \namespace OKlib::GraphDrawing
  \brief Tools for drawing graphs and trees
*/

namespace OKlib {
  namespace GraphDrawing {
  }
}
