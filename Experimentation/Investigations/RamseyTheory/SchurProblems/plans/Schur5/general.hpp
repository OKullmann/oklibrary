// Oliver Kullmann, 17.4.2009 (Swansea)
/* Copyright 2009, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/plans/Schur5/general.hpp
  \brief On computing schur(5) and variations

  The conjecture is schur(5) = 161.

  (In [Ramsey Theory on the Integers; Landman, Robertson, 2003] we have the
  conjecture "schur(5) = 160", but already 1994 it was shown schur(5) >= 161
  (see below).)


  \todo Overview
  <ul>
   <li> See RamseyTheory/SchurProblems/plans/Schur5/LocalSearch/general.hpp
   for using local search. </li>
   <li> See RamseyTheory/SchurProblems/plans/Schur5/CompleteSolvers/general.hpp
   for using DPLL-solvers. </li>
  </ul>


  \todo Statistics
  <ul>
   <li> Creation for direct encoding:
   \verbatim
output_schur_stdname(5,160);
   \endverbatim
   </li>
   <li> Counts:
   \verbatim
> cat Schur_5_160.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    800   33495    800    800   33495    98805     NA     NA   33495    98805   803
 length   count
      2    2000
      3   31335
      5     160
   \endverbatim
   </li>
  </ul>


  \todo Certificates
  <ul>
   <li> From [A Lower Bound for Schur Numbers and Multicolor Ramsey Numbers of
   K_3; Exoo, 1994]:
   \verbatim
C : uncompresss_palindromic_subsets(160,[
{4,5,15,16,22,28,29,39,40,41,42,48,49,59},
{2,3,8,14,19,20,24,25,36,46,47,51,62,73},
{7,9,11,12,13,17,27,31,32,33,35,37,53,56,57,61,79},
{1,6,10,18,21,23,26,30,34,38,43,45,50,54,65,74},
{44,52,55,58,60,63,64,66,67,68,69,70,71,72,75,76,77,78,80}
]);
certificate_pdschur_p(5,160,C);
  true
   \endverbatim
   and thus schur(5) >= 161 (it seems that was overlooked in some parts of the
   literature). </li>
   <li> Interesting is here to find all palindromic) solutions; [Exoo] found
   four "different". </li>
   <li> In general (for arbitrary partitions) [Exoo] posed the question whether
   a partition with some part containing at least 45 elements exists. Such
   cardinality-constraints are interesting to investigate. </li>
  </ul>

*/
