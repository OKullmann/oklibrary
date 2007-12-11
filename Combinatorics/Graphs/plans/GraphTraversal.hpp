// Oliver Kullmann, 11.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Graphs/plans/GraphTraversal.hpp
  \brief Plans for the sub-module on graph-traversal


  \todo Implement the generic graph_traversal algorithm from
  the CS-232 script (generic enough, and with enough event-points,
  so that protocolling and visualisation are possible).
  <ul>
   <li> The buffer concept is the same as in Boost::graph. </li>
   <li> For the visitor concept we should look at the concept of "BFS Visitor"
   in Boost::graph. </li>
   <li> The marking object should follow property maps from Boost::graph. </li>
   <li> Yet we only manage undirected graphs? </li>
  </ul>


  \todo Implement the main applications or graph_traversal from
  the CS-232 script
  <ul>
   <li> Visitor, buffer and marking objects, as supplied to graph_traversal,
   in most of these cases depend on the input graph. Perhaps one could have then
   an overloaded version with just one aggregate as input? </li>
  </ul>


  \todo Graph traversal is a basic technique for all forms of
  applications of graph theory to SAT; a general overview on
  algorithms and implementations is needed, and also on overview
  on the potential applications.
  <ul>
   <li> Boost::breadth_first_search is verys similar to graph_traversal. </li>
   <li> Boost::depth_first_search should be simulatable by graph_traversal.
   </li>
  </ul>

*/

