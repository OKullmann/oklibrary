// Oliver Kullmann, 16.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Heuristics/plans/general.hpp
  \brief Super-module for heuristics


  \todo Create further milestones
  <ul>
   <li> Perhaps Heuristics/Projections is most important for now, then
   comes Heuristics/StatisticalAnalysis and then
   Heuristics/StructureRecognition. </li>
  </ul>


  \todo Using different algorithms for different parameter values
  <ul>
   <li> Once the SAT problem is small enough, special algorithms should be
   employed for faster processing.
   <li> Perhaps this should be part of the general control of the SAT
   solving process, which employes different algorithms / data-structures
   for different parameter regimes. </li>
   <li> Calling this module "Controlling" ? </li>
   <li> The algorithms as discussed below would then better be considered
   as general SAT algorithms (each in its own appropriate modules), just
   employed in special contexts. </li>
   <li> An attractive possibility is to use for formulas F with k
   variables the free boolean algebra with k generators, and thus
   to use an unsigned type INT with 2^k bits and to plug-in the k generating
   values corresponding to the functions x_1, ..., x_k (computed by
   meta-programming), evaluated then using the ordinary bit-operations for INT.
    <ol>
     <li> See "Representing boolean functions by their truth tables" in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/general.hpp
     for implementations at Maxima/Lisp level of these ideas. </li>
     <li> When installing, one could find out which k-values are supported,
     and where it pays off (definitely k=5, i.e., 32 bits, and k=6, i.e.,
     64 bits, but perhaps also bigger k). </li>
     <li> Arbitrary boolean functions can be handled here. </li>
     <li> The result represents the boolean function F, and thus
     many other tasks could be considered (counting satisfying assignments
     e.g. is just counting the number of 1's in the result). </li>
     <li> Can this approach be generalised, so that the algorithm which
     runs through all 2^n assignments now needs only to consider 2^{n-k}
     assignments (with "big booleans") ? Should be possible, and then
     the direct enumeration using Gray codes (see
     Algorithms/AllSolutions/plans/Enumeration.hpp) could be further boosted.
     </li>
     <li> This has connections with many modules (counting, finding all
     solutions, etc.), but perhaps is best handled in its own module. </li>
     <li> Perhaps this is best placed in a new module
     Satisfiability/FiniteFunctions/Boolean, with a sub-module BigBooleans?
     </li>
     <li> See the article of [Heule, van Maaren; 2007] on the use of "big
     booleans". </li>
    </ol>
   </li>
  </ul>


*/

/*!
  \namespace OKlib::Satisfiability::Heuristics
  \brief Components for heuristics

  Alias "Heu".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Heuristics {
    }
    namespace Heu = Heuristics;
  }
}

