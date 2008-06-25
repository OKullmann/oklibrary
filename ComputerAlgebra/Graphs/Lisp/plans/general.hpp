// Oliver Kullmann, 24.11.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/plans/general.hpp
  \brief Plans for graphs in Maxima/Lisp


  \todo Create milestones


  \todo Complete tests
  <ul>
   <li> ComputerAlgebra/Graphs/Lisp/Basic.mac
    <ol>
     <li> Downcasts </li>
     <li> Basic graph operations </li>
     <li> Basic graph constructions </li>
     <li> Tests </li>
     <li> mg2g </li>
    </ol>
   </li>
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


  \todo Naming conventions
  <ul>
   <li> "g" for graphs, "gl" for graphs with loops, "dg" for
   directed graph, "dgl" for directed graphs with loops, "gg"
   for general graph, and "gdg" for "general directed graph". </li>
   <li> "og" for ordered graphs, similarly "ogl", "odg", "odgl", "ogg"
   and "ogdg". </li>
   <li> For Maxima-graphs we use "mg" and "dmg". </li>
   <li> So for multigraphs we need to use the abbreviation "mug", for
   multigraphs with loops mugl, for directed multigraphs dmug, for
   directed multigraphs with loops dmugl, and finally omug, omugl, odmug,
   odmugl denote the ordered versions. </li>
   <li> There is a clash for "directed graphs":
    <ol>
     <li> Either we speak of "directed graphs", "directed multigraphs",
     "directed general graphs", or of "digraphs", "multi-digraphs",
     "general digraphs". </li>
     <li> Perhaps the "digraph" has the advantage that there is no
     confusion about the order of adjectives. </li>
    </ol>
   </li>
   <li> So "g2mg" (now) becomes "g2mg" etc. </li>
  </ul>


  \todo Graph concepts
  <ul>
   <li> A "graph" is just a 2-element list, consisting of the set of
   vertices, and a set of 2-element vertex sets. </li>
   <li> A "graph with loops" also allows 1-element vertex sets. </li>
   <li> A "general graph" is a triple [V,E,f], where V, E are sets and
   f is a map from E to 1-2-element subsets of V. </li>
   <li> The same with directed graphs, only that this time we have
   vertex-lists instead of vertex-sets. </li>
   <li> There is also the (cryptomorphic) notion of a "precategory",
   which is a 4-tuple [V,E,source,target]. </li>
   <li> A "multigraph" is a triple [V,E,f], where [V,E] is a graph, and f
   is a map which maps every element of E to a natural number (that is, >0).
   </li>
   <li> While a "multigraph with loops" is similarly a triple [V,E,f] such that
   [V,E] is a graph with loops. </li>
   <li> And a "directed multigraph" is a triple [V,E,f], where now the elements
   of E or pairs, while a "directed multigraph with loops" allows the elements
   of the pairs to be identical. </li>
   <li> Since also for general graphs the edge set needs to be given, we don't
   have the possibility of "lazy graph representations". Seems unavoidable. /li>
   <li> Standardisation of vertex-names:
    <ol>
     <li> From the point of view of the OKlibrary, the best choice for
     standard vertex sets would be {1,..,n}. </li>
     <li> This coincides with mathematical conventions, and also 0 is not
     possible as a clause-set-variable. </li>
     <li> However Maxima graphs by themselves are labelled starting with 0,
     and array-indices start with 0 as well. </li>
     <li> So well, when translating Maxima graphs to graphs we translate
     vertex names, if needed, to start with 1. </li>
     <li> That is, when these or "original" Maxima graphs, which one can
     apparently recognise by the vertex list in reverse order, finishing
     with a zero. </li>
    </ol>
   </li>
   <li> Given a vertex, we need the set of edges incident to the vertex;
   this can be handled via the dual hypergraph, but we should provide some
   more convenient methods. </li>
   <li> Likely, our graph-etc-concepts are conceptually appropriate, but when
   using or implementing algorithms then we should, if possible, use the
   maxima-graphs. </li>
   <li> How to handle "properties" ? DONE (these shall just be maps) </li>
   <li> Can we tag such objects as being "graphs" ? DONE (we are living
   type-free) </li>
  </ul>


  \todo Memoisation for general graphs and multigraphs
  <ul>
   <li> The function f in a general graph [V,E,f] could show three different
   types of computational strategies for computing f(e):
    <ol>
     <li> (i) f(e) could always be computed from scratch. </li>
     <li> (ii) The other extreme is to store f as an array (and thus only
     compute the values once, when creating the general graph). </li>
     <li> (iii) The intermediate stratety is the "lazy" one, where f(e) is
     computed only when needed, and then stored; see "Memoisation" in
     ComputerAlgebra/CombinatorialMatrices/Lisp/plans/general.hpp for how
     to do this. </li>
     <li> It seems that in many cases the first strategy is alright, since
     different from combinatorial matrices, the edges in general graphs
     typically are just simple naming-issues. </li>
     <li> Also for multigraphs, in the typical applications the multiplicity
     of an edge is typically "given", not computed. </li>
     <li> However for example for conflict-graphs of clause-sets it seems best
     to compute f upfront (using (ii)). </li>
     <li> Using a hash-map inside f is made easy by lambda_hm; though, since
     E is fixed, an array would be more efficient but this works only
     for edge-labels which are consecutive integers. </li>
    </ol>
   </li>
  </ul>


  \todo Maxima package "graphs"
  <ul>
   <li> We need conversions between graphs and maxima-graphs.
    <ol>
     <li> We can use the vertex-labels (assuming they are present). </li>
     <li> Currently, mg2g doesn't use the vertex labels (since they might not
     be there). </li>
     <li> So we need a second version, which also translates the vertex labels.
     </li>
     <li> How to call it? Which version is standard? Perhaps mg2g should
     translate the vertex labels, and "mg2g_nvl" doesn't translate them.
     </li>
     <li> What about loops? Are they possible with Maxima graphs? </li>
    </ol>
     <li> DONE (the vertex names become vertex labels)
     Given a graph, we can either just forget the vertex names, or use
     them as vertex labels. </li>
     <li> DONE And given a maxima-graph, we can use the standard-vertex-names
     0, ... </li>
   </li>
   <li> We need conversions between directed graphs and maxima-digraphs.
    <ol>
     <li> "dg2mdg" needs to be complemented by two inverses, mdg2dg and
     mdg2dg_nvl. </li>
     <li> What about loops? </li>
    </ol>
   </li>
   <li> Do Maxima graphs allow parallel edges? </li>
   <li> Find out about the output-formats for maxima-graphs. </li>
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


  \todo InputOutput.mac
  <ul>
   <li> Extend the documentation. </li>
   <li> Write documentation. </li>
   <li> Output :
    <ol>
     <li> Can we add comments to the graph-output? </li>
     <li> Should we give the graphs in the dot-output a dedicated name? </li>
     <li> Generalise the graph output, so that arbitrary graphs
     can be output (making function "string_vertex" a parameter). </li>
     <li> DONE Move graph output to dedicated "InputOutput.mac". </li>
    </ol>
   </li>
  </ul>


  \todo Graph traversal
  <ul>
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

*/

