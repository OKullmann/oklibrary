// Oliver Kullmann, 13.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file HamiltonianPaths/plans/general.hpp
  \brief Plans for the module on Hamiltonian paths (cycles etc.)


  \todo Update namespaces.


  \todo Create milestones.


  \todo Given a graph G, the alliance expressing the Hamiltonian
  path (cycle) property has variables v_1, ..., v_n for n = |V(G)|
  with domains V(G), and two active clause-sets:
   - injectivity (bijectivity) for v_1, ..., v_n
   - v_i, v_{i+1} must get adjacent vertices in G (if considering
     Hamiltonian cycles, then this wraps around); call this active
     clause-set the adjacency constraint.

  This is a special case of graph-embedding (embedding a path resp. a cycle).

   
  \todo One could add "opportunistically" some elements of the bijectivity
  constraint to the adjacency constraint; for example if there are v_i and v_j
  such that the minimal distance between their possible values in G is
  larger than the distance between i and j, then the current
  partial assignment is unsatisfiable (an extreme case here is given by a
  disconnected G) --- this inconsistency cannot be anticipated by any of the
  two active clause-sets alone (but must be realised by the alliance).
  Is there a general method behind that, leading to a meaningfully
  strengthened adjacency constraint?! Perhaps it is better (this might
  also be true in general) to create new active clause-sets capturing
  these constraints (for example a "distance constraint", maintaining
  that the minimal distance in G of the possible values of any two vertices
  v_i and v_j is not larger than the distance of i and j).


  \todo Creating a catalogue with Hamiltonian graphs and
  non-Hamiltonian graphs (for creating benchmarks).


  \todo Are there competitions on the subject?


  \todo The Hamiltonian Path/Cycle problem is a special case
  of the graph embedding problem: The adjacency constraint
  just has to consider a general graph G_0 with vertices v_1, ..., v_n
  instead of the path resp. cycle on v_1, ..., v_n. One needs good
  data structures for the adjacency constraint.


  \todo The graph isomorphism problem (see module Isomorphisms)
  is a special case of the graph embedding problem: How do
  graph embedding algorithms perform on graph isomorphism problems?


  \todo TSP
  <ul>
   <li> Using appropriate weights, hamiltonian paths can be found
   via TSP; see Applications/CombinatorialOptimisation/plans/general.hpp. </li>
  </ul>

*/

/*!
  \namespace OKlib::HamiltonianPaths
  \brief Active clause-sets to search for Hamiltonian paths etc.
*/

namespace OKlib {
  namespace HamiltonianPaths {
  }
}

