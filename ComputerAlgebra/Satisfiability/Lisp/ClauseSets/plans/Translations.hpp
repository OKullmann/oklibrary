// Oliver Kullmann, 23.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Translations.hpp
  \brief Plans for basic clause-set translations


  \todo Translations reducing the lengths of clauses
  <ul>
   <li> Shortening clauses by introducing new variables. </li>
  </ul>


  \todo Translations reducing the number of variable-occurrences
  <ul>
   <li> The basic step for a boolean clause-set F:
    <ol>
     <li> Consider a literal x occurring in two different clauses C, D in F.
     </li>
     <li> Introduce a new variable v. </li>
     <li> Replace C, D by (C - {x}) + {v}, (D - {x}) + {v}, {-v,x}. </li>
     <li> Perhaps signs are standardised, so  that the new binary clauses
     are negative. </li>
    </ol>
   </li>
   <li> Performing this basic step for all *original* literals, we obtain
   F' such that
    <ol>
     <li> All original variables occur (1,1). </li>
     <li> All new variables occur (2,1). </li>
     <li> All original clauses kept their length. </li>
     <li> All new clauses are binary. </li>
    </ol>
   </li>
   <li> There is also interest in obtaining also a uniform k-CNF at the end:
    <ol>
     <li> First reduce (change) the clause-length (see above). </li>
     <li> Then reduce the number of literal occurrences by the basic steps.
     </li>
     <li> Perhaps integration of these two operations can save variables.
     </li>
     <li> Finally expand the new binary clauses to length k (by inverse
     2-subsumption resolution). </li>
     <li> In this way we obtain (maximally) (2,2^(k-2)) occurrences. </li>
    </ol>
   </li>
  </ul>

*/

