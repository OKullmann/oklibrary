// Oliver Kullmann, 23.5.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/plans/BicliquePartitions.hpp
  \brief Plans regarding biclique partitions of (general) graphs


  \todo Basic concepts
  <ul>
   <li> A "biclique partition" of a general graph is a partition of the
   edge set such that each part constitutes a complete bipartite graph.
    <ol>
     <li> We take "partition" here in the formal sense, that is, a set of
     subsets, none empty, union the whole, pairwise disjoint. </li>
     <li> Later we also need "polarised" versions where the parts of the
     biclique are also specified. </li>
     <li> Here we need, given an edge-subset of a general graph, to
     construct the "induced" general sub-graph, whose vertex set is
     just the vertices used by those edges. </li>
     <li> Then we need a check for simplicity of a general graph. </li>
     <li> And then we need a bipartiteness test for general graphs
     (using the Maxima function). </li>
    </ol>
   </li>
   <li> Write predicate "bicliquepartp" to check for this property. </li>
   <li> Likely this more algebraic aspects of graph theory is handled
   better by combinatorial matrices; see
   ComputerAlgebra/CombinatorialMatrices/Lisp/plans/general.hpp. </li>
   <li> Also for translating biclique partition into clause-sets we
   do most work at the level of combinatorial matrices. </li>
  </ul>


  \todo Enumerating or sampling biclique partitions
  <ul>
   <li> Most basic: Given a general graph, enumerate all biclique partitions.
    <ol>
     <li> Simplest: via set_partitions(edge_set) create all possible partitions
     of the edge-set, and then we compute the subset given by bicliquepartp.
     </li>
     <li> Can we write an efficient generator, which creates one at a time,
     in polynomial time, running through all biclique partitions, without
     repetition? </li>
     <li> For example, running through the lexicographical ordering?! </li>
    </ol>
   </li>
   <li> Given the uniform probability distribution on the set of all biclique
   partitions of a given general graph, can we efficiently sample it?
    <ol>
     <li> A simple model is, given a probability p, find a random maximal
     biclique, keep edges with probability p, remove the biclique, and
     repeat the process. </li>
     <li> For 0 < p < 1 this is complete. Likely, for every p the induced
     probability is not the uniform one, since emphasise is put on larger
     bicliques. </li>
    </ol>
   </li>
  </ul>


  \todo Finding specific biclique partitions via SAT
  <ul>
   <li> See TransformationsBiclique.hpp and
   Transitional/OKsolver/plans/general.hpp. </li>
   <li> We must also give an overview on the space of possible
   generalised SAT-Translations.
    <ol>
     <li> The most basic parameter is an upper bound on the number of
     biclique. </li>
     <li> More precisely, we could have list of biclique sizes. </li>
    </ol>
   </li>
  </ul>


  \todo Tools for graphs
  <ul>
   <li> From the Maxima graphs-library:
    <ol>
     <li> complete_bipartite_graph (n, m) </li>
     <li> bipartition (gr) </li>
     <li> is_bipartite (gr) </li>
    </ol>
   </li>
  </ul>


  \todo The conjecture of [Galesi, Kullmann]
  <ul>
   <li> Using the (deprecated) Mupad-functions, the conjecture is
   that if ist_exakt_transversal(M) is true, then
   ist_speziell_exakt_transversal(M) is true. </li>
   <li> This should be equivalent to the statement, that every minimally
   unsatisfiable clause-set F, whose conflict multigraph is a complete
   bipartite graph, has a read-once resolution refutation. </li>
   <li> First approach for searching for a counter-example:
    <ol>
     <li> Enumerate or sample biclique partitions of complete bipartite
     graphs. </li>
     <li> Get the corresponding clause-set. </li>
     <li> Perform subsumption-elimination. </li>
     <li> Remove satisfiable clause-sets (via SAT or via the decision procedure
     for exact transversal hypergraphs). </li>
     <li> Check the clause-set for the special condition. </li>
    </ol>
  </ul>

*/

