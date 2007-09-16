// Oliver Kullmann, 11.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file MatchingAutarkies/plans/MatchingAutarkies.hpp
  \brief Algorithms regarding matching autarkies

  \todo Algorithms for the lean kernel:
  First the main algorithmic ideas have to be developed.
  <ol>
   <li> In the paper {Kullmann 2003 (DAM)]
        the algorithm in Lemma 8.6 reduces it to the
        decision problem, for which one can use Theorem 7.5 or Lemma 7.11;
        Lemma 7.11 gives also directly the matching lean kernel in Corollary 7.12.
   </li>
   <li> In [Kullmann, successor of CSR 13-2006] after Lemma 4.26 one finds the remark,
        how the characterisation of "F matching lean <=> surp(F) >= 1" yields another
        algorithm for finding matching autarkies; this algorithm should be more efficient.
   </li>
   <li> Compare with Matchings/plans/Matchings.hpp. </li>
   <li> Has Stefan Szeider a different algorithm? </li>
  </ol>

  \todo Algorithms for matching autarkies:
  Are there more heuristical approaches for finding just one matching autarky?

  \todo Implementation:
  A direct implementation of the computation
  of the matching lean kernel based on a matching algorithm.

  \todo Application:
  Write an application for computing the lean kernel,
  the largest matching autarky etc.

  \todo Incremental:
  Then one has to think about integration into a SAT solver. Likely maintenance of a maximum matching
  plays a role for incremental algorithm; the main change-operation is the application of partial
  assignments.
*/

/*!
  \namespace OKlib::MatchingAutarkies
  \brief Components related to finding matching autarkies
*/

namespace OKlib {
  namespace MatchingAutarkies {
  }
}

