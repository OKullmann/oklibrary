// Oliver Kullmann, 11.1.2006 (Swansea)
/* Copyright 2006 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Algorithms/Autarkies/MatchingAutarkies/plans/general.hpp
  \brief Plans for algorithms regarding matching autarkies


  \todo Update namespaces. DONE


  \todo Update plans. DONE


  \todo Connections to other modules
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/MatchingAutarkies.hpp.
   </li>
   <li> First everything is developed there. </li>
   <li> So most of the general plans here should be moved there. </li>
   <li> While plans are developed to fully implement everything. </li>
  </ul>


  \todo %Algorithms for the matching lean kernel
  <ul>
   <li> In the paper [Kullmann, 2003 (DAM)] the algorithm in Lemma 8.6 reduces
   it to the decision problem, for which one can use Theorem 7.5 or
   Lemma 7.11. </li>
   <li> Also there, Lemma 7.11 gives also directly the matching lean kernel
   in Corollary 7.12. </li>
   <li> In [Kullmann, CSR 12-2007] after Lemma 4.27 one finds the remark,
   how the characterisation of "F matching lean <=> surp(F) >= 1" yields
   another algorithm for finding matching autarkies; this algorithm should
   be more efficient. </li>
   <li> Compare with Combinatorics/Graphs/Matchings/plans/general.hpp. </li>
   <li> Has Stefan Szeider a different algorithm? </li>
  </ul>


  \todo Finding matching autarkies
  <ul>
   <li>  Are there more heuristical approaches for finding just one matching
   autarky? </li>
  </ul>


  \todo %Implementation
  <ul>
   <li> A direct implementation of the computation
   of the matching lean kernel based on a matching algorithm. </li>
  </ul>


  \todo Applications
  <ul>
   <li> Write an application for computing the lean kernel,
   the largest matching autarky etc. </li>
  </ul>


  \todo Incremental
  <ul>
   <li> Then one has to think about integration into a SAT solver. </li>
   <li> Likely maintenance of a maximum matching plays a role for incremental
   algorithm; the main change-operation is the application of partial
   assignments. </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Algorithms::Autarkies::MatchingAutarkies
  \brief Components related to finding matching autarkies

  Alias "MAu".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Algorithms {
      namespace Autarkies {
        namespace MatchingAutarkies {
        }
        namespace MAu = MatchingAutarkies;
      }
    }
  }
}

