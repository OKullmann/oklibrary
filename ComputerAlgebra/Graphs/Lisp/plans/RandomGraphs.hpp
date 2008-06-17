// Oliver Kullmann, 17.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/plans/RandomGraphs.hpp
  \brief Plans regarding generation of (general) random graphs


  \todo %Implementation
  <ul>
   <li> First we need an overview on the existing generators for
   random (general) graphs. </li>
   <li> One general method is to construct a random adjacency matrix
   or a random incidence matrix. </li>
   <li> In the Maxima graphs-module we have:
    <ol>
     <li> random_bipartite_graph </li>
     <li> random_digraph </li>
     <li> random_regular_graph </li>
     <li> random_graph </li>
     <li> random_graph1 </li>
     <li> random_tournament (complete digraphs) </li>
     <li> random_tree </li>
    </ol>
    It seems only (simple) graphs are generated.
   </li>
   <li> In Sage:
    <ol>
     <li> RandomBarabasiAlbert </li>
     <li> RandomDirectedGN </li>
     <li> RandomDirectedGNC </li>
     <li> RandomDirectedGNR </li>
     <li> RandomGNM </li>
     <li> RandomGNP </li>
     <li> RandomHolmeKim </li>
     <li> RandomLobster </li>
     <li> RandomNewmanWattsStrogatz </li>
     <li> RandomRegular </li>
     <li> RandomShell </li>
     <li> RandomTreePowerlaw </li>
    </ol>
    It seems only (simple) graphs are generated.
   </li>
  </ul>


  \todo Overview on theoretical possibilities
  <ul>
   <li> We need an overview on the models studied in the theory
   of random graphs. </li>
   <li> We are especially interested in random multigraphs!
    <ol>
     <li> These could be obtained by taking the union of random graphs. </li>
     <li> Again, the natural alternative is to consider random matrices. </li>
     <li> Or one consider any given graph, and multiplies each edge in a
     random way. </li>
    </ol>
   </li>
  </ul>

*/

