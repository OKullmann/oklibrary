// Oliver Kullmann, 13.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Matroids/Lisp/plans/general.hpp
  \brief Plans regarding matroids in Maxima/Lisp


  \todo Fundamental notions
  <ul>
   <li> A "matroid" is a special type of hypergraph. </li>
   <li> A problem are the those many variations, i.e., whether to specify
   a matroid by the "independent subsets", by "circuits" etc. </li>
   <li> Let "mtr" be the abbreviation for "matroid"; then perhaps we
   have "mtr_x" for the variations:
    <ol>
     <li> "is" for independent subsets </li>
     <li> "ds" for dependent subsets </li>
     <li> "cr" for circuits </li>
     <li> "bs" for bases </li>
     <li> "rk" for a rank-function; here we don't have a hypergraph but
     a pair [V,f], where V is a set and f yields for every subset of V
     a natural number (with zero). </li>
     <li> "clos" for closure; perhaps we have a module in
     ComputerAlgebra/Sets, where we handle closure systems, i.e.,
     pairs [X,f], X a set and f a map, which transforms subsets into subsets,
     in general. </li>
    </ol>
   </li>
   <li> The hypergraph-based notions should be given more "notational
   prominence" (than the others), perhaps always using 2-letters ? </li>
   <li> Important to be complete and precise. </li>
   <li> Important that all dual notions are easily at hand, perhaps the letter
   "d" is reserved to indicate "dual notions". </li>
   <li> We also need to be compatible with oriented matroids; see
   ComputerAlgebra/OrientedMatroids/Lisp/plans/general.hpp. </li>
  </ul>

*/

