// Oliver Kullmann, 19.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/plans/Basics.hpp
  \brief Plans regarding the basic notions for hypergraphs


  \todo Lists instead of sets
  <ul>
   <li> Additionally to "hypergraphs" and "general hypergraphs" we introduce
   "ordered hypergraphs" and "ordered general hypergraphs":
    <ol>
     <li> Computationally these notions are considered to be more fundamental.
     </li>
     <li> An ordered hypergraphs is a pair [V,E], where V and E are lists
     without repetition, such that [setify(V),setify(E)] is a hypergraph. </li>
     <li> An ordered general hypergraph is a triple [V,E,f], where V, E
     are lists without repetition, such that [setify(V),setify(E),f] is a
     general hypergraph. </li>
    </ol>
   </li>
   <li> Multi-versions:
    <ol>
     <li> We also should have "multi-hypergraphs", triples [V,E,c], s.t.
     [V,E] is a hypergraph, and c: E -> NN. </li>
     <li> And "ordered multi-hypergraphs, [V,E,c] s.t. [setify(V),setify(E),c]
     is a multi-hypergraph, and [V,E] is an ordered hypergraph. </li>
  </ul>


  \todo Naming conventions
  <ul>
   <li> "hg" for hypergraph, "ghg" for general hypergraph, and "ohg", "oghg"
   for the ordered versions. </li>
   <li> "muhg" for multi-hypergraph, and "omuhg" for ordered multi-hypergraph.
   </li>
   <li> Conversions then as "hg2ghg", "ohg2hg" etc. </li>
   <li> Connections to graphs (see
   ComputerAlgebra/Graphs/Lisp/plans/general.hpp):
    <ol>
     <li> g, gl <= hg </li>
     <li> mug, mugl <= muhg </li>
     <li> gg <= ghg </li>
     <li> og, ogl <= ohg </li>
     <li> omug, omugl <= omuhg </li>
     <li> ogg <= oghg </li>
    </ol>
   </li>
  </ul>


  \todo Polar hypergraphs
  <ul>
   <li> While a "square hypergraph" is a hypergraph [V,E] with |V| = |E|, we
   introduce the notion of a "bipolar hypergraph" ("bphg") as a pair [V,f],
   s.t. [V,V,f] is a general hypergraph. </li>
   <li> A "polar hypergraph" is a bipolar hypergraph [V,f] s.t. for all
   y in f(x) we also have x in f(y). </li>
   <li> Specialising correspondences [R,A,B] to "relationals" [R,A],
   considering only symmetric relations, representing the relation R <= A^2
   by a function from A to P(A), and changing the order, we have exactly the
   polar hypergraphs; dropping "symmetric", we get the bipolar hypergraphs.
   </li>
   <li> Another view is that the bipolar hypergraphs [V,f] correspond exactly
   to the digraphs with vertex set V, where f(v) is the set of out-neighbours,
   while polar hypergraphs correspond exactly to the graphs with vertex set
   V. </li>
   <li> The notion of a "polar hypergraph" comes from incidence structures:
    <ol>
     <li> Given an incidence structure [I,A,B], a "polarity" p is a bijection
     p: A -> B such that I(x,p(y)) = I(y,p(x)) for all x, y in A. </li>
     <li> Given such a polarity, we obtain a "polar indicence structure" as
     a pair [I',A] with I': A^2 -> {0,1} through I'(x,y) := I(x,p(y)) (where
     I' is symmetric). </li>
     <li> And while incidence structures correspond to general hypergraphs,
     polar incidence structures correspond to polar hypergraphs. </li>
     <li> See ComputerAlgebra/IncidenceStructures/Lisp/plans/general.hpp. </li>
    </ol>
   </li>
   <li> Relations with graphs:
    <ol>
     <li> We have natural translations gl2phg, dgl2bphg and phg2gl, bphg2dgl.
     </li>
     <li> Additionally we also have bphg2g, which is the same as g2dg after
     bphg2dgl. </li>
     <li> For the direction (d)gl2(b)phg, it seems most natural to implement
     the function f through hash-maps. </li>
     <li> Additionally we have stdgl2phg and stddgl2bphg, which use arrays.
     </li>
    </ol>
   </li>
  </ul>

*/

