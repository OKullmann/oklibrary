// Oliver Kullmann, 16.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/Bicliques/plans/Sampling.hpp
  \brief Plans regarding sampling of biclique partitions in (general) graphs


  \todo Sampling maximal vertex-bicliques
  <ul>
   <li> First task, given an ordered graph, find a maximal ordered biclique.
    <ol>
     <li> Parameter 0 <= p <= 1. </li>
     <li> Run through the vertices in the given order, try to add it to
     one of the sides of the biclique, and if both are possible, then
     use p for a random choice. </li>
     <li> One possibility for the check is that the new vertex is always
     checked against all vertices already in the biclique. </li>
     <li> The other possibility is that for both parts of the biclique
     we maintain the set of vertices addable to it, and then for a new
     vertex we only need to check membership. </li>
    </ol>
   </li>
  </ul>


  \todo Sampling maximal edge-bicliques
  <ul>
   <li> The algorithms from [Crama/Hammer et al, Consensus algorithms ...,
   DAM 2004] yield poly-delay algorithms for enumerating all maximal
   bicliques; see
   ComputerAlgebra/Graphs/Lisp/Bicliques/plans/Consensus.hpp. </li>
   <li> Another algorithm is [Sanderson et al, Molecular Biology and Evolution,
   20(7)]. </li>
   <li> A central question for us now is whether, just running them once,
   randomised, we also obtain (reasonable) sampling of all maximal
   bicliques. </li>
   <li> More precisely, randomising the starting conditions appropriately,
   can every maximal biclique be created (and this with reasonable fair
   distribution)? </li>
   <li> This then could be used in the natural greedy biclique partitioning
   algorithm (grabbing some maximal bicliue, remove the edges, repeat ...).
   </li>
  </ul>

*/

