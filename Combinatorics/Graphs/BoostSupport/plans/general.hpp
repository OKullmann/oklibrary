// Oliver Kullmann, 21.8.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Graphs/BoostSupport/plans/general.hpp
  \brief General plans regarding support for the Boost graph library


  \todo Investigate whether there is something usable in the
  following old files:
  <ol>
   <li> Auxiliary.hpp </li>
   <li> Concepts_Graphs.hpp </li>
   <li> Generators.hpp </li>
   <li> Graphs.hpp
    <ol>
     <li> Class Graphs::MutableAdjacencyGraph_ViaIntegers looks
     dubious; likely the point was to be able to construct a vertex
     from a given integer-index, but this should be possible with
     some given boost-graph-models. </li>
    </ol>
   </li>
   <li> makefile </li>
   <li> Support.hpp </li>
   <li> Test.cpp </li>
   <li> Tests_Generators.hpp </li>
  </ol>

  
  \todo Collect:
  Collect all related components (and requests) from modules in
  the library, and transfer the todos to this module:
  <ul>
   <li> Hypergraphs/Colourings/GreedyColouring.cpp </li>
   <li> QuantumPhysics/OrthogonalTriples.cpp </li>
  </ul>


  \todo Input/Output
  <ul>
   <li> Compare with GraphDrawing/plans/general.hpp. </li>
   <li> Should we use the "DOT language" (graphviz) as the primary
   graph-representation-format? (see "Graph drawing" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp) </li>
   <li> This seems to be the Boost input language, so we should use it. </li>
   <li> Download the document specifying the language, and make it
   accessible. </li>
   <li> See Graphs/BoostSupport/demo_read_graph.cpp. </li>
   <li> The complete graph with 3 vertices a, b, c can be specified as follows:
   \verbatim
graph G {
a -- b;
a -- c;
b -- c
}
   \endverbatim
   </li>
   The directed cycle with three vertices a, b, c:
   \verbatim
digraph G {
a -> b;
b -> c;
c -> a;
}
   \endverbatim
   </li>
   <li> Are comments possible? </li>
   <li> What is the use of the graph name ("G") ? </li>
   <li> Should we offer additional help for using boost::read_graphviz
   and boost::write_graphviz? Or do we only provide good examples?
   Definitely we first start with the examples. </li>
   <li> Examples:
    <ol>
     <li> Hypergraphs/Colourings/GreedyColouring.cpp </li>
     <li> Graphs/Isomorphisms/SimpleBacktracking.cpp </li>
    </ol>
   </li>
  </ul>


  \todo Update namespaces.

*/

