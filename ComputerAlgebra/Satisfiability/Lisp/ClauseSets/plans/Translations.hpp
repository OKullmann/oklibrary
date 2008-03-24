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
     <li> In effect, we eliminated one occurrence of literal x, and we
     introduced a new variable v occurring (2,1). </li>
    </ol>
   </li>
   <li> Performing this basic step iteratedly for all literals such that
   the underlying variable occurs at least 4 times (so we never need
   to consider the newly introduced variables), we obtain F' such that
    <ol>
     <li> All variables occur at most 3 times. </li>
     <li> All original clauses kept their length. </li>
     <li> All new clauses are binary. </li>
    </ol>
   </li>
   <li> Alternatively, by [Tovey, 1984, DAM] we obtain a different general
   translation, which for a variable v in F occurring at least 4 times
   removes all occurrences at once as follows:
    <ol>
     <li> Consider a variable v occurring m >= 4 times. </li>
     <li> For each occurrence of v, introduce a new variable w_i. </li>
     <li> Replace each occurrence of v by the corresponding w_i, using the
     same sign. </li>
     <li> Add the equivalence ring (w_1 -> w_2), ..., (w_{m-1} -> w_m),
     (w_m -> w_1). </li>
    </ol>
    This translation has the same 3 properties as the above translation,
    and additionally when considering only the new binary clauses, then
    every variable occurs only 2 times.
   </li>
   <li> There is also interest in obtaining also a uniform k-CNF at the end
   (with a small variable degree):
    <ol>
     <li> First reduce or change the clause-length, by any method, for
     example reducing the clause-length as in the todo above, or increasing
     the clause-length by inverse 2-subsumption resolution. </li>
     <li> Then reduce the number of literal occurrences by any of the
     above two methods. </li>
     <li> Perhaps integration of these two operations can save variables.
     </li>
     <li> The final (and crucial step) is to expand the new binary clauses
     to size k:
      <ul>
       <li> The simplest method is inverse 2-subsumption resolution, but this
       introduces too many literal occurrences. </li>
       <li> A more powerful general method (used by [Tovey, 1984, DAM]) is to
       expand a clause C simply by new variables v to the desired length k,
       and then to add for each new variable v a dedicated clause-set F_v,
       variable-disjoint except of v, which forces v to be false, and such that
       the variable-degree of F_v is at most s, except of v, which has a
       variable-degree at most s-1. </li>
       <li> Such F_v are trivially obtained from any (minimally) unsatisfiable
       clause-set F which is k-uniform and has variable-degree at most s
       (by removing one clause). </li>
       <li> See "Pumping up binary clauses" in
       ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp
       for dedicated constructions of such F_v. </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>

*/

