// Oliver Kullmann, 24.11.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/plans/general.hpp
  \brief Plans for graphs in Maxima/Lisp


  \todo Organisation
  <ul>
   <li> See module "GraphTraversal" below. </li>
   <li> See module "Treewidth" below. </li>
   <li> See module "Matchings" below. </li>
   <li> Likely we should have modules "Homomorphisms" and "Isomorphisms". </li>
   <li> We need a submodule "Colourings". </li>
   <li> A submodule "IndependentSets". </li>
   <li> A submodule "Cliques". </li>
   <li> Likely also a submodule "RandomGraphs". </li>
   <li> And a submodule "RamseyTheory" (but this perhaps better belongs
   to Hypergraphs ?). </li>
   <li> DONE See module "Trees" below. </li>
  </ul>


  \todo Complete tests
  <ul>
   <li> ComputerAlgebra/Graphs/Lisp/Basic.mac
    <ol>
     <li> Checking the defining properties </li>
     <li> Checking equality </li>
     <li> Promotions </li>
     <li> Downcasts </li>
     <li> Conversions </li>
     <li> Basic graph operations </li>
     <li> Basic graph constructions </li>
     <li> Tests </li>
     <li> Connections to Maxima-graphs </li>
    </ol>
   </li>
   <li> Once all these tests are completed, new functions are only
   to be entered with tests. </li>
  </ul>


  \todo Redesign
  <ul>
   <li> See "Plan the redesign" in ComputerAlgebra/plans/Maxima.hpp for
   the general overview. </li>
   <li> See "Lists instead of sets" and "Naming conventions" below. </li>
  </ul>


  \todo Lists instead of sets
  <ul>
   <li> Additionally to "graphs" and "general graphs" we introduce
   "ordered graphs" and "ordered general graphs":
    <ol>
     <li> Computationally these notions are considered to be more fundamental.
     </li>
     <li> An ordered graphs is a pair [V,E], where V and E are lists
     without repetition, such that [setify(V),setify(E)] is a graph. </li>
     <li> An ordered general graph is a triple [V,E,f], where V, E
     are lists without repetition, such that [setify(V),setify(E)] is a
     general graph. </li>
     <li> An ordered multigraph is a triple [V,E,f], where [V,E] is an
     ordered graph, such that for the underlying graph [V',E'] it is
     [V',E,',f] a multigraph. </li>
    </ol>
   </li>
  </ul>


  \todo Subgraph concepts
  <ul>
   <li> Perhaps we should create "Subgraphs.mac". </li>
   <li> Likely the subgraphs of ordered graphs-types should inherit the
   order(s). </li>
  </ul>


  \todo Graphs as hypergraphs
  <ul>
   <li> Graphs G = [V,E] with loops have a natural representation as general
   hypergraphs nghg(G) = [V,V,f] (the "neighbourhood hypergraph"), where f(v)
   is the set of neighbours of v (the set of vertices adjacent to v). </li>
   <li> Such hypergraphs could be called "square"; but this notion
   has already been introduced by Seymour for general hypergraphs [V,E,f] with
   just |V| = |E| (and (combinatorial) matrices are not really adequate
   as a framework). </li>
   <li> Better we call objects [V,f] "polar general hypergraphs" ("pghg"),
   where [V,V,f] is a general hypergraph; these are sets V together with
   functions f, which assign to every element of V a subset of V. </li>
   <li> The translation function thus would be called "g2pghg(G)". </li>
   <li> These are the hypergraph-versions of polar incidence structures;
   see ComputerAlgebra/IncidenceStructures/Lisp/plans/general.hpp. </li>
   <li> Specialising correspondences [R,A,B] to "relationals" [R,A],
   considering only symmetric relations, representing the relation R <= A^2
   by a function from A to P(A), and changing the order, we have exactly the
   polar hypergraphs. </li>
  </ul>


  \todo Memoisation for general graphs and multigraphs
  <ul>
   <li> The function f in a general graph [V,E,f] could show three different
   types of computational strategies for computing f(e):
    <ol>
     <li> (i) f(e) could always be computed from scratch. </li>
     <li> (ii) The other extreme is to store f as an array (and thus only
     compute the values once, when creating the general graph). </li>
     <li> (iii) The intermediate strategy is the "lazy" one, where f(e) is
     computed only when needed, and then stored; see "Memoisation" in
     ComputerAlgebra/CombinatorialMatrices/Lisp/plans/general.hpp for how
     to do this. </li>
     <li> It seems that in many cases the first strategy is alright, since
     different from combinatorial matrices, the edges in general graphs
     typically are just simple naming-issues. </li>
     <li> Also for multigraphs, in the typical applications the multiplicity
     of an edge is typically "given", not computed. </li>
     <li> However for example for conflict-graphs of clause-sets it seems best
     to compute f up-front (using (ii)). </li>
     <li> Using a hash-map inside f is made easy by lambda_hm; though, since
     E is fixed, an array would be more efficient but this works only
     for edge-labels which are consecutive integers. </li>
    </ol>
   </li>
  </ul>


  \todo Maxima package "graphs"
  <ul>
   <li> What is petersen_graph(n,d)?
    <ol>
     <li> We always have the outer and the inner cirle (each of
     length n), with the direct connections, and d is then the "step" to
     the next vertex on the inner circle. </li>
     <li> Thus for d=0 there is no edge, for d=1 we have the two parallel
     edges, for n=2 we skip one (as for the Petersen graph), and for d=n/2
     we get parallel edges. </li>
     <li> The parallel edges shouldn't be possible? </li>
     <li> And for d=0 actually print_graph shows loops?? </li>
     <li> Tell the mailing-list, that this needs documentation, and that
     the non-graph cases needs investigation. </li>
    </ol>
   </li>
   <li> We need conversions between graphs and maxima-graphs.
    <ol>
     <li> We can use the vertex-labels (assuming they are present). </li>
     <li> Currently, mg2g doesn't use the vertex labels (since they might not
     be there). </li>
     <li> So we need a second version, which also translates the vertex labels.
     </li>
     <li> How to call it? Which version is standard? Perhaps mg2g should
     translate the vertex labels, and "mg2g_nl" doesn't translate them.
     </li>
     <li> DONE (no loops possible; so we have only "graphs" and "directed graphs")
     What about loops? Are they possible with Maxima graphs? </li>
    </ol>
   </li>
   <li> We need conversions between directed graphs and maxima-digraphs.
    <ol>
     <li> "dg2mdg" needs to be complemented by two inverses, mdg2dg and
     mdg2dg_nvl. </li>
     <li> DONE (not possible)
     What about loops? </li>
    </ol>
   </li>
   <li> DONE (the vertex names become vertex labels)
   Given a graph, we can either just forget the vertex names, or use
   them as vertex labels. </li>
   <li> DONE And given a maxima-graph, we can use the standard-vertex-names
   0, ... </li>
   <li> DONE (only "graphs" and "digraphs", nothing else)
   Do Maxima graphs allow parallel edges? Apparently not. </li>
   <li> DONE (exact values are computed)
   We need to find out whether for example the colouring function computes an
   exact value or an approximation! See my e-mail to the maxima-mailing-list. </li>
   <li> DONE (solved with installed gnuplot-version)
   Bug (with 5.14.0 and 5.15.0):
   \verbatim
draw_graph(complete_graph(3))$
gnuplot> plot '/home/kullmann/data.gnuplot' index 0 t '' w lp ps 1 pt 0 lw 1 lt 1 lc rgb 'black', '/home/kullmann/data.gnuplot' index 1 t '' w lp ps 1 pt 0 lw 1 lt 1 lc rgb 'black', '/home/kullmann/data.gnuplot' index 2 t '' w lp ps 1 pt 0 lw 1 lt 1 lc rgb 'black', '/home/kullmann/data.gnuplot' index 3 t '' w p ps 2 pt 7 lc rgb 'red'
         "/home/kullmann/maxout.gnuplot", line 22: ';' expected
   \endverbatim
   Notify maxima mailing-list. </li>
  </ul>


  \todo Graph traversal
  <ul>
   <li> This likely deserves its own module "GraphTraversal". </li>
   <li> Implement the generic graph traversal from module CS-232. </li>
   <li> This is likely best done with the maxima-graphs. </li>
   <li> Compute connected components and strong connected components. </li>
  </ul>


  \todo Treewidth
  <ul>
   <li> Implement all basic methods for computing the treewidth of a graph. </li>
   <li> See Combinatorics/Graphs/TreeDecompositions/EliminationSequences.hpp for
   one algorithm, and
   Combinatorics/Graphs/TreeDecompositions/plans/EliminationSequences.hpp for
   plans. </li>
   <li> See also Combinatorics/Graphs/TreeDecompositions/plans/general.hpp.
   </li>
  </ul>


  \todo Primitive directed graphs etc.
  <ul>
   <li> Compute the index of imprimitivity of a directed graph (with loops).
   </li>
   <li> And for primitive directed graphs (with loops) compute the index of
   primitivity. </li>
   <li> Again, perhaps best done with the maxima-graph-package. But then loops
   need to be handled specially (is this reasonable?). </li>
  </ul>


  \todo Generalised matching problems
  <ul>
   <li> Implement [David G. Kirkpatrick and Pavol Hell, On the completeness of
   a generalized matching problem, STOC'78]. </li>
   <li> That is, the NP-completeness results (as reductions), and the polytime
   algorithms. </li>
   <li> Perhaps this has been extended, and perhaps also simplified? </li>
   <li> A variant is considered in [Dorit Dor and Michael Tarsi, Graph
   Decomposition is NP-Complete: A Complete Proof of Holyer's Conjecture,
   SIAM Journal on Computing, 1997]. </li>
  </ul>

*/

