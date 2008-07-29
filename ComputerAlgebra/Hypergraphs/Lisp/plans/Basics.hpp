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

*/

