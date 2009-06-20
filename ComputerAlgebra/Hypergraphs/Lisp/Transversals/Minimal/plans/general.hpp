// Oliver Kullmann, 20.62009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/Transversals/Minimal/plans/general.hpp
  \brief Plans regarding finding minimal hypergraph transversals


  \todo Connections
  <ul>
   <li> See
   Combinatorics/Hypergraphs/Transversals/Minimal/plans/DirectTransversalEnumeration.hpp
   for computing the transversal hypergraph via recursive splitting. </li>
  </ul>


  \todo Transfer
  <ul>
   <li> See "Rewrite hypergraph transversal functions in Maxima" in
   ComputerAlgebra/Mupad/plans/general.hpp. </li>
  </ul>


  \todo Testing
  <ul>
   <li> okltest_transversal_hg should be a generic test for computing
   the transversal hypergraph (used for all different methods).
   Further test-cases:
    <ol>
     <li> Claw-graphs. </li>
     <li> Complete graphs (and complete r-graphs). </li>
     <li> Graphs without edges. </li>
    </ol>
    It seems superfluous to also have a dedicated test-function for the
    independence hypergraph (i.e., that check should just transfer the
    test to the transversal-test)? See below.
   </li>
  </ul>


  \todo transversal_hg
  <ul>
   <li> Performance on complete graphs:
    <ol>
     <li> Likely this performs badly. </li>
     <li> And this likely holds for all splitting strategies. </li>
     <li> For example when adding one edge after another, starting
     with the empty edge-set and completing with all edges, there
     should be always some graph in the sequence with large transversal
     hypergraph. </li>
    </ol>
   </li>
  </ul>

*/

