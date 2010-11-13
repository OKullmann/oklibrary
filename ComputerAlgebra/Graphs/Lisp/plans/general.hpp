// Oliver Kullmann, 24.11.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
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
   <li> See module "Drawings" below. </li>
   <li> Likely we should have modules "Homomorphisms" and "Isomorphisms".
    <ol>
     <li> It seems yet we have nothing on graph homo/isomorphisms. </li>
     <li> Though isomorphisms are special homomorphisms, it seems their
     treatment is so specialised that we should have two independent
     modules. </li>
     <li> There are also categories of graphs: Likely the organisation here
     should be the same as for hypergraphs and for clause-sets. </li>
     <li> In ComputerAlgebra/Satisfiability we have "Symmetries" instead of
     "Isomorphisms", and we have "Categories" which includes "Homomorphisms".
     </li>
     <li> So perhaps also here we should have modules "Symmetries" and
     "Categories". </li>
    </ol>
   </li>
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
     <li> Checking the defining properties : DONE</li>
     <li> Checking equality : DONE </li>
     <li> Promotions : DONE </li>
     <li> Downcasts : DONE </li>
     <li> Conversions : DONE </li>
     <li> Basic graph operations : DONE </li>
     <li> Basic graph constructions : DONE </li>
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


  \todo Representing edge and vertex labellings
  <ul>
   <li> One needs to be able to represent an edge, or vertex labelling of a 
   given graph in the maxima system. </li>
   <li> It should then be possible to visualise this somehow using some graph
   drawing package, or produce LaTeX output to draw the graph with the
   appropriate labels or colours. </li>
   <li> See '"Visualising" solutions' in
   Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/general.hpp 
   for potential use in visualising counter examples for Ramsey number SAT 
   problems. </li>
   <li> See also "Representing edge and vertex labellings" in 
   ComputerAlgebra/Hypergraphs/Lisp/plans/general.hpp . </li>
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


  \todo Graphs as hypergraphs
  <ul>
   <li> The topic is discussed in "Polar hypergraphs" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Basics.hpp. </li>
   <li> We need a good overview on the different representations of graphs
   via hypergraphs, incidence structures and combinatorial matrices:
    <ol>
     <li> On the one hand, these representations show different points of
     view. </li>
     <li> And on the other hand they provide different data structures! </li>
    </ol>
   </li>
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
   <li> DONE (solved with version 5.18.1)
   Problems with the graph without vertices:
    <ol>
     <li>
     \verbatim
is_bipartite(empty_graph(0));
Evaluation took 0.0040 seconds (0.0001 elapsed) using 1.148 KB.
(%o294) false
     \endverbatim
     though it should be true. </li>
     <li> We also have
     \verbatim
complete_graph(0);
Argument 1 to complete_graph is not a positive integer
 -- an error.  To debug this try debugmode(true);
     \endverbatim
     </li>
    </ol>
   </li>
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
   <li> In general we need to write docus for all the undocumented notions
   w.r.t. the Maxima graphs-module. </li>
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


  \todo Graph drawing
  <ul>
   <li> This should become module "Drawings". </li>
   <li> We need our own graph-drawing functions:
    <ol>
     <li> Maxima cannot handle loops nor parallel edges. </li>
     <li> The drawing algorithms are rather weak (but we should also try
     the possible plug-ins). </li>
     <li> We need reference-algorithms. </li>
     <li> And we need the possibility to fix our own drawings (for an example
     see "Visualising automorphisms" in
     ComputerAlgebra/Graphs/Lisp/Isomorphisms/plans/Homomorphisms.hpp). </li>
    </ol>
   </li>
   <li> We need to find out to what extend the Gnuplot functions can be used
   (this is likely our own possibility anyway). </li>
   <li> But we must establish an abstract level, where the algorithms
   only produce abstract tree-drawings (just usual Maximal-lists, with
   coordinates and other information), and as a separate layer then these
   abstractions are drawn. </li>
   <li> Does there exist some basic "vocabulary" ? </li>
   <li> Likely other graph-drawing packages can read such drawings (in a
   suitable output, of course). See "Concepts" in
   GraphDrawing/plans/general.hpp. </li>
  </ul>

*/

