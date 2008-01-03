// Oliver Kullmann, 8.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Hypergraphs/Matchings/plans/general.hpp
  \brief Plans regarding disjoint hyperedge sets in hypergraphs


  \todo Update namespaces.


  \todo Relations
  <ul>
   <li> The dual notion is that of vertex sets such that no hyperedge
   exists which contains two of them; explore this notion (compare
   "strong independence" in Hypergraphs/IndependentSets/plans/general.hpp). </li>
   <li> The special graph-case is treated in Graphs/Matchings/plans/general.hpp. </li>
   <li> Exact coverings (disjoint coverings):
    <ol>
     <li> Does this belong to this module, or to
     Hypergraphs/HyperedgeCoverings (see
     Hypergraphs/HyperedgeCoverings/plans/general.hpp). </li>
     <li> Perhaps it's most naturally viewed as the dual of the exact
     transversal problem? See "Exact covers" in
     Hypergraphs/Transversals/plans/ExactTransversals.hpp. </li>
     <li> See http://en.wikipedia.org/wiki/Exact_cover for more information.
     </li>
     <li> There reductions of n-queens (see
     Configurations/NQueens/plans/general.hpp) and Sudoku (see
     Applications/LatinSquares/plans/Sudoku.hpp) to the exact-cover-problem
     are mentioned. </li>
     <li> An algorithm is Knuth's "dancing links" (see
     http://en.wikipedia.org/wiki/Dancing_Links); likely this is rather
     naive (seems to be just the special case of Boehm's 2-dimensional
     linked list representation of clause-sets?). </li>
    </ol>
   </li>
  </ul>

*/

