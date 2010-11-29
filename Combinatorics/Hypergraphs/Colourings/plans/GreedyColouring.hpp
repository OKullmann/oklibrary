// Oliver Kullmann, 24.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Hypergraphs/Colourings/plans/GreedyColouring.hpp
  \brief Plans regarding the greedy graph colouring algorithm


  \todo Relations to other modules
  <ul>
   <li> See "Greedy colouring" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Colouring.hpp. </li>
  </ul>


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
   <li> Transfer todos here. </li>
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
   <li> Translating the decision problem, whether a vertex ordering
   achieving at most k colours exist, into a SAT problem:
    <ol>
     <li> Variables are i_v, the index for vertex v, with domain
     {1,...,|V|}, and c_v, the colour of v, with domain {1,...,k}. </li>
     <li> Constraints are
      <ul>
       <li> the bijectivity constraint for the i_v, </li>
       <li> the active clause-set for the colouring conditions (no adjacent
       vertices get the same colour), </li>
       <li> and the active clause-set stating that if all neighbours which
       are also predecessors are coloured, then the colour of the vertex is
       determined as the minimal available colour. </li>
      </ul>
     </li>
     <li> Instead of the permutation one could also use a lineare ordering
     with the corresponding axioms reflexivity, antisymmetry, transitivity
     and totality. </li>
    </ol>
   </li>
   <li> This SAT problem could be treated as a minimisation problem for k,
   the domain size of the colour-variables. </li>
   <li> One could treat the conditions regarding the i_v as "soft constraints",
   since they are only there to guide the search for a colouring (we know that
   an ordering exists iff a colouring exists). </li>
   <li> Via this SAT formulation we also capture a backtracking search for
   an ordering achieving k colours (though the question is whether this
   SAT-approach can be as efficient as a direct implementation). </li>
   <li> Of course it is questionable whether in this way something for
   graph colouring is gained:
    <ol>
     <li> The only strength of the greedy approach seems to be that the number
     of colours is not restricted. </li>
     <li> But for the SAT translation it needs to be restricted. </li>
     <li> The direct search space condition is k^n = n! for the given n:
     for k with n! < k^n at least the primary search space is smaller. </li>
     <li> In other words, the critical k is k = (n!)^(1/n), and for larger
     k the direct search space of greedy colouring is smaller. </li>
    </ol>
    One simply should experiment with the approach.
   </li>
  </ul>


  \todo Local search for a good vertex ordering
  <ul>
   <li> The direct approach is to consider it just as a standard optimisation
   problem, where the search space is the space of all vertex permutations, and
   where the goal function is the number of colours obtained by the
   permutation, to be minimised. </li>
   <li> The natural notion of a move would be a transposition (possibly
   restricted to a neighbour swap). </li>
   <li> A more restricted approach would consider the decision problem with
   bound k, and would only construct the colouring corresponding to a
   permutation until it gets larger than k, in which case the current
   "problematic" vertex could simply be swapped with one of its coloured
   neighbours. </li>
   <li> See Optimisation/LocalSearch/plans/general.hpp for a generic local
   search algorithm. </li>
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

