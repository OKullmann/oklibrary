// Oliver Kullmann, 24.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Hypergraphs/Colourings/plans/GreedyColouring.hpp
  \brief Plans regarding the greedy graph colouring algorithm


  \todo Outsourcing
  <ul>
   <li> In GreedyColouring.hpp the components
    <ul>
     <li> Out_degree_order </li>
     <li> output_vertex_degrees </li>
     <li> Edge DONE (not needed anymore) </li>
     <li> EdgeVector DONE (not needed anymore) </li>
     <li> read DONE (replaced by using the dot-syntax) </li>
    </ul> 
    should be moved to the SupportBoost-module.
   </li>
  </ul>


  \todo Class Greedy_colouring
  <ul>
   <li> The design needs an overhaul. </li>
   <li> Use Boost::permutation_iterator (and it should be made possible
   to iterate through the vertex-permutations in some user-defined
   order). </li>
   <li> Test it. </li>
   <li> Extend doxygen documentation. </li>
  </ul>


  \todo GreedyColouring.cpp
  <ul>
   <li> Eliminate code repetition. </li>
   <li> Use general components from module Graphs. </li>
   <li> Use Messages. </li>
   <li> Use ProgramOptions. </li>
   <li> Complete doxygen documentation. </li>
   <li> Write docus. </li>
   <li> Test it. </li>
   <li> Transfer todos here : DONE </li>
  </ul>


  \todo Literature
  <ul>
   <li> Search for literature on heuristics for vertex orderings. </li>
   <li> From CS-232 we have the traversing greedy colouring; it seems
   sensible to combine this with a largest degree first-stratey.
    <ol>
     <li> If there is a choice, then a vertex with largest degree
     is chosen. </li>
     <li> Such choice points happen when entering a new connected
     component, and when choosing an edge from the buffer. </li>
     <li> The graph traversal algorithm is available in
     boost as boost::breadth_first_search; unclear whether it
     exists also for undirected graphs. Easiest to sort the
     vertices at the beginning, to store the associated ranks
     as vertex properties, and to use a priority queue as
     buffer using these ranks, while perhaps after a connected
     component has been processed, the used vertices are taken
     out, and then simply the first vertex can be chosen. </li>
     <li> Since we are computing the order dynamically, we might
     alternatively use the strategy for choice points: Maximise the
     number of yet uncoloured vertices! This is somewhat harder
     to compute (the priorities for the buffer need update),
     but should still be very fast. </li>
    </ol>
   </li>
  </ul>


  \todo How do these algorithms translate into SAT algorithms?
  <ul>
   <li> We need to consider the decision variants. </li>
   <li> One could hope to find a new heuristical principles
   for finding a satisfying assignment by generalising the
   various greedy algorithms. </li>
  </ul>


  \todo Generalisation to hypergraphs
  <ul>
   <li> Same as before, for a given order give always the lowest number
   possible. </li>
   <li> However, apparently now there does not need to exist an order
   producing an optimal colouring? </li>
   <li> Are there other greedy strategies? </li>
  </ul>

*/

