// Oliver Kullmann, 6.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Hypergraphs/Transversals/plans/general.hpp
  \brief Plans for the module on hypergraph transversals


  \todo Update namespaces.


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/Transversals/plans/general.hpp.
   </li>
   <li> See
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/general.hpp
   for experiments. </li>
  </ul>


  \todo Organisation of the module
  <ul>
   <li> Perhaps we should have submodules "Bounded", "Minimum", "All",
   "Exact". </li>
   <li> "Exact" could again have submodules "Bounded", "Minimum", "All". </li>
  </ul>


  \todo Literature exploration:
  <ul>
   <li> What is known meanwhile about the existence of polynomial
   delay enumeration of Tr(G) for hypergraphs G? </li>
   <li> For example Wahlstroem has an algorithm for enumerating
   Tr(G) for 3-uniform G --- worth implementing? Likely one needs
   to extract a heuristical generalisation. </li>
   <li> The dual problem to hypergraph transversal enumeration
   (using the dual hypergraph) is enumeration of all edge covers
   of a hypergraph (see Hypergraphs/HyperedgeCoverings/plans/general.hpp)
   --- has this formulation advantages / disadvantages? </li>
   <li> The complementary problem (by hyperedge-complementation)
   to hypergraph transversal enumeration is enumeration of all
   independent sets (see Hypergraphs/IndependentSets/plans/general.hpp)
   --- again, has this formulation advantages / disadvantages? </li>
   <li> The graph case:
    <ul>
    <li> Transversals for graphs are called "vertex covers" (see
    Graphs/VertexCovers/plans/general.hpp); are there special algorithms? </li>
    <li> Also the independent sets in graph deserve attention. </li>
    <li> For graphs, finding a minimum edge cover can be done
    in polynomial time by means of maximum matching; worth
    investigating? Dualisation yields, that minimum transversals
    can be computed for hypergraphs with vertex degrees at most
    2. </li>
    </ul>
   </li>
  </ul>


  \todo Duality
  <ul>
   <li> So the three problems
    <ol>
     <li> enumerating transversals </li>
     <li> enumerating edge covers </li>
     <li> enumerating independent sets </li>
    </ol>
    for hypergraphs can each be reduced to each other by a
    combination of dualisation and hyperedge-complementation. </li>
   <li> We should develop hypergraph adapters, which do not need to
   actually perform the transformation, but provide a "view". </li>
   <li> Perhaps this should be supported by the hypergraph concepts
   (otherwise we have to handle expression templates). </li>
  </ul>


  \todo Compare with the SAT-generalisation
  <ul>
   <li> All the algorithms in this module can be naturally generalised to
   (generalised) SAT; see AllSolutions/plans/MinimalAssignments.hpp. </li>
   <li> So shall we only implement the general SAT case? </li>
   <li> The embedding G -> F is given here by using the vertices v as
   variables with domains D_v = {0}, and where all literals are
   positive monosigned, i.e., "v=0". </li>
   <li> An alternative is to use boolean variables: Then we have also the
   possibility to cross out vertices (by setting the corresponding
   literals to false). </li>
  </ul>

*/

/*!
  \namespace OKlib::HypergraphTransversals
  \brief Components for handling hypergraph transversals
*/

namespace OKlib {
  namespace HypergraphTransversals {
  }
}

