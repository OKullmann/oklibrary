// Oliver Kullmann, 21.9.2007 (Durham)
/* Copyright 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/plans/general.hpp
  \brief Plans on the supermodule on SAT algorithms in general

  General algorithms, complete and incomplete ones, for (generalised)
  SAT solving.
  

  \todo Create module for SAT problems with bounded maximal deficiency.


  \todo Create module for solving small SAT problems
  <ul>
   <li> An attractive possibility is to use for formulas F with k
   variables the free boolean algebra with k generators, and thus
   to use an unsigned type INT with 2^k bits and to plug-in the k generating
   values corresponding to the functions x_1, ..., x_k (computed by
   meta-programming), evaluated then using the ordinary bit-operations for INT.
    <ol>
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
    </ol>
   </li>
   <li> This has connections with many modules (counting, finding all
   solutions, etc.), but perhaps is best handled in its own module. </li>
   <li> Perhaps we have a dedicated super-module
   Satisfiability/BooleanFunctions, with a sub-module BigBooleans? </li>
   <li> See the article of [Heule, van Maaren; 2007] on the use of "big
   booleans". </li>
  </ul>


  \todo Update namespaces : DONE


  \todo Create milestones : DONE

*/

/*!
  \namespace OKlib::Satisfiability::Algorithms
  \brief Components for (generalised) SAT algorithms

  Alias "Alg".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Algorithms {
    }
    namespace Alg = Algorithms;
  }
}

