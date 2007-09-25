// Oliver Kullmann, 25.9.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Graphs/DominatingSets/plans/general.hpp
  \brief Plans for the general module on graph-components


  \todo Reduction to the hypergraph transversal problem
  <ul>
   <li> For a graph G let Gamma(G) be the general hypergraph with
   E(Gamma(G)) = V(Gamma(G)) = V(G) and with the hyperedge associated
   to v in V(G) the set of neighbours of v in G (i.e., adjacent
   vertices). </li>
   <li> And let Gamma_r(G) ("r" like "reflexive") be the variation,
   obtained from the reflexive version of G. </li>
   <li> Now a strict dominating set for G is a transversal of Gamma(G),
   while a dominating set for G is a transversal of Gamma_r(G). </li>
   <li> The constructions Gamma(G) and Gamma_r(G) are fundamental,
   and should be provided ("eager" and "lazy"). </li>
   <li> Then the general algorithms for the hypergraph transversal
   problem can all be applied. </li>
   <li> Are there special algorithms, exploiting that Gamma(G) is square? </li>
  </ul>
  
*/

