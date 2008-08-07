// Oliver Kullmann, 25.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/Isomorphisms/plans/Homomorphisms.hpp
  \brief Plans regarding graph homomorphisms in general


  \todo Organisation
  <ul>
   <li> Likely we should move automorphism-related functions to
   "Automorphisms.mac". </li>
   <li> Though graph colouring is a special case, it deserves a separate
   module (Graphs/Lisp/Colouring). </li>
  </ul>
 

  \todo Brute-force automorphism enumeration
  <ul>
   <li> It would be useful to have the possibility to specify a set of vertices
   which need to be fixed by the automorphisms. </li>
  </ul>


  \todo Handling of automorphism groups
  <ul>
   <li> Given some set of automorphisms, compute the generated sub-group. </li>
   <li> This is better handled in a submodule Algebra/Groups/Permutations.
   </li>
  </ul>


  \todo Visualising automorphisms
  <ul>
   <li> A reasonable visualisaton of an automorphism is to have a second
   drawing, identical as drawing of an "anonymous" graph, but with the
   new vertex labels showing the renaming. </li>
   <li> For this first a drawing needs to be fixed, and then a second one
   is produced, using the old coordinates and adding just new verttex
   labels. </li>
   <li> There is a "redraw"-option for the draw_graph-function, but this
   options seems to have no effect. A bug? Ask on the Maxima mailing list!
   </li>
   <li> For the Petersen-graph apparently draw_graph has a fixed built-in
   drawing, and vertex-labels can be added (while still showing the same
   fixed drawing) --- so it should be possible! </li>
   <li> It would also be needed here to have several gnuplot windows open
   at the same time --- how to do this? </li>
   <li> See "Graph drawing" in ComputerAlgebra/Graphs/Lisp/plans/general.hpp.
   </li>
  </ul>


  \todo Deciding whether for given H there is a graph morphism G -> H
  <ul>
   <li> Implement [Pavol Hell and  Jaroslav Nesetril, On the complexity of
   H-coloring, Journal of Combinatorial Theory, Series B, 1990]. </li>
   <li> If H is bipartite, then (for input G) the problem is decidable
   in polynomial time (and likely we also find a homomorphism), while
   otherwise the problem is NP-complete. </li>
   <li> That is, for the NP-completeness result implement the reductions,
   and for the poly-time results implement the algorithms. </li>
   <li> I have seen improved presentations of this result --- do we also
   have improved algorithms and reductions? </li>
  </ul>

*/

