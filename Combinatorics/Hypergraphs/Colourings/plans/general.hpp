// Oliver Kullmann, 25.11.2005 (Swansea)
/* Copyright 2005 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/Colourings/plans/general.hpp
  \brief Plans for the module on hypergraph (and graph) colouring


  \todo Update namespace-usage.


  \todo Module structure
  <ul>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Colouring.hpp. </li>
   <li> Should we make a distinction between graph colouring and hypergraph
   colouring? In this case GreedyColouring.cpp (etc) should go to
   Combinatorics/Graphs. Yes, this should be (compare "Connections to other
   modules" in ComputerAlgebra/Graphs/Lisp/plans/Colouring.hpp). </li>
  </ul>


  \todo Translations
  <ul>
   <li> Given a (hyper)graph as input, translate the k-colouring problem into
   generalised or P-clause-sets. </li>
  </ul>


  \todo Other implementations
  <ul>
   <li> What open-source graph colouring implementations are available? </li>
   <li> Problem collections? </li>
   <li> Are there competitions? </li>
  </ul>


  \todo Active clause-sets
  <ul>
   <li> For interesting systematically generated classes of (hyper)graphs,
   create active clause-sets translating k-colouring problems. </li>
  </ul>


  \todo Investigate the problem of determining the chromatic number:
  <ul>
   <li> For finding an optimal colouring for a graph G the following strategy seems
   natural:
    <ol>
     <li> Find a reasonable upper bound chi(G) <= u by some quick heuristics
     (different forms of greedy colouring).
     </li>
     <li> Find a reasonable lower bound l <= chi(G) by some quick heuristics,
     among them finding cliques in G.
     </li>
     <li> Comprising the found cliques into hyperedges, a hypergraph G' is
     constructed (with chi_s(G') = chi(G), where chi_s is the strong colouring
     number), and then via a (generalised) SAT solver the interval
     l <= chi_s(G') <= u is narrowed to a point (perhaps here bisection is
     sensible?!); see Hypergraphs/Colourings/plans/StrongColouring.hpp
     for strong colourings of hypergraphs.
     </li>
    </ol>
   </li>
   <li> For finding an optimal colouring for a hypergraph G we should try the
   same strategy:
    <ol>
     <li> Find some lower bound l and some upper bound u. </li>
     <li> Close the gap with a (generalised) SAT solver. </li>
    </ol>
   </li>
   <li> And the same for strong hypergrap colouring. </li>
  </ul>


  \todo Property B
  <ul>
   <li> Erdoes and Hajnal introduced the function m(k) associated with property B:
   The minimal number of hyperedges of non-2-colourable k-uniform hypergraphs. </li>
   <li> Known are only m(0) = m(1) = 1, m(2) = 3, m(3) = 7 (see the latest OK-report (XXX)
   on autarkies and hypergraph colouring), while 19 <= m(4) <= 23. </li>
   <li> Given n, m, k, it should be possible to construct a nice SAT problem whose
   solutions are the k-uniform non-2-colourable hypergraphs G with n vertices and m
   edges. </li>
   <li> Then we can try to compute values for m(k). </li>
   <li> This can also be generalised to higher chromatic numbers (in the literature there
   is the generalised property B(s)? it seems more natural to consider c >= 2 for
   "non-c-colourable", and to consider m(c, k) (with m(2, k) = m(k))). </li>
  </ul>


  \todo Square hypergraphs
  <ul>
   <li> In [Robertson, Seymour, Thomas: Permanents, Pfaffian orientations, and even
   directed circuits] a poly-time algorithm for deciding whether a square hypergraph
   is minimal non-2-colourable is given; very interesting to investigate this
   algorithm. </li>
  </ul>

*/

/*!
  \namespace OKlib::Combinatorics::Hypergraphs::Colourings
  \brief Components for hypergraph colouring

  Alias "Col".
*/


namespace OKlib {
  namespace Combinatorics {
    namespace Hypergraphs {
      namespace Colourings {
      }
      namespace Col = Colourings;
    }
  }
}

