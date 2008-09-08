// Oliver Kullmann, 17.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/plans/Basic.hpp
  \brief Plans on the basic graph functionality


  \todo DONE
  Naming conventions
  <ul>
   <li> DONE
   "g" for graphs, "gl" for graphs with loops, "dg" for
   directed graph, "dgl" for directed graphs with loops, "gg"
   for general graph, and "gdg" for "general directed graph". </li>
   <li> DONE
   "og" for ordered graphs, similarly "ogl", "odg", "odgl", "ogg"
   and "ogdg". </li>
   <li> DONE
   For Maxima-graphs we use "mg" and "mdg". </li>
   <li> So for multigraphs we need to use the abbreviation "mug", for
   multigraphs with loops mugl, for directed multigraphs mudg, for
   directed multigraphs with loops mudgl, and finally omug, omugl, omudg,
   omudgl denote the ordered versions. </li>
   <li> DONE
   There is a clash for "directed graphs":
    <ol>
     <li> Either we speak of "directed graphs", "directed multigraphs",
     "directed general graphs", or of "digraphs", "multi-digraphs",
     "general digraphs". </li>
     <li> The "digraph" has the advantage that there is no
     confusion about the order of adjectives. </li>
    </ol>
   </li>
  </ul>


  \todo Graph concepts
  <ul>
   <li> DONE
   A "graph" is just a 2-element list, consisting of the set of
   vertices, and a set of 2-element vertex sets. </li>
   <li> DONE
   A "graph with loops" also allows 1-element vertex sets. </li>
   <li> DONE
   A "general graph" is a triple [V,E,f], where V, E are sets and
   f is a map from E to 1-2-element subsets of V. </li>
   <li> DONE
   The same with directed graphs, only that this time we have
   vertex-lists (always of size 2) instead of vertex-sets. </li>
   <li> There is also the (cryptomorphic) notion of a "precategory",
   which is a 4-tuple [V,E,source,target]; abbreviation "precat". </li>
   <li> Multigraphs:
    <ol>
     <li> A "multigraph" is a triple [V,E,f], where [V,E] is a graph, and f
     is a map which maps every element of E to a natural number (that is, >0).
     </li>
     <li> While a "multigraph with loops" is similarly a triple [V,E,f] such
     that [V,E] is a graph with loops. </li>
     <li> And a "directed multigraph" is a triple [V,E,f], where now the
     elements of E or pairs, while a "directed multigraph with loops" allows
     the elements of the pairs to be identical. </li>
     <li> However, it seems more natural to actually make the domain of f all
     possible edges (directed or undirected, with or without loops); then
     possibly the edge set could be dropped. </li>
     <li> We must then change all functions concerned with multi(di)graphs.
     </li>
    </ol>
   </li>
   <li> Since also for general graphs the edge set needs to be given, we don't
   have the possibility of "lazy graph representations". Seems unavoidable.
   </li>
   <li> "Oriented" versions:
    <ol>
     <li> An "oriented general digraph" has additionally a function o, defined
     on edge-labels and returning -1 or +1. </li>
     <li> We use "or", so " orgdg, ordgl, ordg, and oorgdg,oordgl,
     and oordg. </li>
     <li> From an oriented digraph one gets a digraph by oriented the edges
     accordingly. </li>
     <li> Morphisms of oriented digraphs can additionally flip the orientations
     (and the result must be an ordinary morphism). </li>
     <li> Shall we also use "oriented graphs", as graphs with such orientation
     functions, to be interpreted according to the natural order on vertices?
     </li>
    </ol>
   </li>
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


  \todo Subgraph concepts
  <ul>
   <li> Perhaps we should create "Subgraphs.mac". </li>
   <li> Likely the subgraphs of ordered graphs-types should inherit the
   order(s). </li>
  </ul>

*/

