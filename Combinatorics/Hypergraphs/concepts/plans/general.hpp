// Oliver Kullmann, 25.9.2007 (Swansea)
/* Copyright 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Hypergraphs/concepts/plans/general.hpp
  \brief Plans for concepts regarding hypergraphs


  \todo Update namespaces.


  \todo Collect possible requirements on a hypergraph concept
  <ul>
   <li> We must connect to the similar (and more refined) considerations
   for clause-sets. </li>
   <li> Operation * of crossing out vertices:
    <ol>
     <li> A framework for "doing/undoing" is needed. </li>
     <li> As usual we have the "eager" and the "lazy" approaches. </li>
     <li> For the eager approach the Boehm datastructure seems
     appropriate (as used in the original OKsolver). </li>
     <li> The information which hyperedges have not been touched is
     also useful (for example for autarky-purposes). </li>
    </ol>
   </li>
   <li> Sorting of hyperedges:
    <ol>
     <li> Often smallest and largest hyperedges are sought. </li>
     <li> Two possibilities: Only for heuristical purposes (thus
     an "approximation" is enough), or for decision purposes (thus
     we need the precise information). </li>
     <li> For heuristical purposes the simplest solution is to sort
     the hyperedges according to size just once at the beginning.
     Likely this should (nearly) always happen. </li>
     <li> The most powerful solution is to keep the hyperedges
     sorted throughout (according to size). Datastructures like
     std::set (balanced trees) are then appropriate. </li>
     <li> For decision purposes (for example in a parameterised
     setting) the largest hyperedge may be needed, and this
     can be realised by a heap datastructure. </li>
    </ol>
   </li>
   <li> Sizes of hyperedges:
    <ol>
     <li> The information on the largest and smallest hyperedge should be
     offered (nearly) always. </li>
     <li> And always for an hyperedge we should be able to ask for its size.
     Most often stored and updated. </li>
     <li> For every size the number of hyperedges of this size is also often
     useful. </li>
    </ol>
   </li>
   <li> Sorting of vertices : dual to sorting of hyperedges. </li>
   <li> Vertex degrees:
    <ol>
     <li> Smallest and largest vertex degrees. </li>
     <li> For every vertex its degree. </li>
     <li> For every possible degree the number of vertices. </li>
    </ol>
    Can we exploit here duality ?!?
   </li>
  </ul>


  \todo Concept design in general
  <ul>
   <li> Writing the basic concepts for hypergraphs, similar to the concepts
   for graphs from the Boost graph library (sequences of vertices and
   hyperedges, where hyperedges are sequences of vertices). </li>
   <li> The direct access from vertices to incident edges could be
   "outsourced" to the concept of an associated bipartite graph (the
   vertex-hyperedge bipartite graph; associated, so that update operations are
   reflected), but it seems better to only "embed" this bipartite graph into
   the hypergraph data structure, as the bipartite vertex-edge graph is
   embedded into the boost graph concept via the refinement of an "incidence
   graph": This seems to model better the main nature of that bipartite graph
   as a data structure; and also in this way generalisations like access only
   to hyperedges where the vertex is the "first" or the "last" vertex are
   perhaps modelled more easily (in this way we then can model directed graphs,
   and also methods like "watched literals" (such bipartite graphs are actually
   vertex-edge graphs)!). </li>
   <li> However, it should be the case
   that such "incidence hypergraphs" directly yield models of bipartite graphs
   via regarding either the vertices or the hyperedges as "left part". </li>
   <li> Clarify the relations to module Graphs (see Graphs/plans/Graphs.hpp).
   </li>
   <li> Clarify the relation to Concepts/plans/BipartiteGraphs.hpp. </li>
   <li> Clause-sets are refinements of hypergraphs, where also application of
   partial assignments may be supported (perhaps also resolution etc. ?). </li>
   <li> See QuantumPhysics/plans/OrthogonalTriples.hpp for some examples. </li>
  </ul>


  \todo Move the related files from OKlib/Concepts here : DONE


  \todo The simplest form of hypergraphs
  <ul>
   <li> The simplest concept of a hypergraph represents a hypergraph just
   as a range of ranges. </li>
   <li> A first example is Hypergraphs::Generators::Hindman_k2 in
   OKlib/Combinatorics/Hypergraphs/Generators/Hindman.hpp; see also
   Combinatorics/Hypergraphs/Generators/plans/Hindman.hpp. </li>
   <li> Perhaps here already one has some hierarchy:
    <ol>
     <li> Simplest just a range of ranges, with no further requirements. </li>
     <li> The set of vertices is here not directly accessible (only through
     the union of all hyperedges). </li>
     <li> Next a simple concept which provides some simple wrapper, providing
     for example traits like "hyperedge_type". </li>
     <li> As a next level two explicit statistics-functions nver and nhyp might
     be added (where nhyp was as the size of the hyperedge-range already
     accessible before, but nhyp is new). </li>
     <li> Does one need two size-types, one for the vertices, one for the
     hyperedges? Seems necessary. </li>
     <li> One could also have a concept inbetween: A hypergraph just provides
     some type- and statistics-members plus access to the range of ranges.
     </li>
    </ol>
   </li>
  </ul>

*/

