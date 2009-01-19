// Oliver Kullmann, 27.12.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/Quasigroups/plans/general.hpp
  \brief Plans in general regarding quasi-groups


  \todo Connections to other modules
  <ul>
   <li> See todo "Quasigroups" in
   Applications/AlgebraicStructures/plans/Groupoids.hpp. </li>
   <li> See
   ComputerAlgebra/CombinatorialMatrices/Lisp/LatinSquares/plans/general.hpp.
   </li>
  </ul>


  \todo Conversions
  <ul>
   <li> The conversions between quasigroups and latin squares work as well
   for the translation between groupoids and "composition tables". </li>
   <li> So perhaps we should generalisations (simply appropriately named)
   for this most general level. </li>
  </ul>


  \todo Associativity resp. group test
  <ul>
   <li> A quasigroup Q is a group iff Q is associative. </li>
   <li> A groupoid V with a rightneutral element is associative iff
   the composition of two rows of V (as a transformation of V) is again a
   row of V. </li>
   <li> So we can test the quasigroup Q for the associativity/groupness
   by first checking whether we have a rightneutral element, and then
   checking the stability of the rows under composition. </li>
   <li> The crucial part is the stability property:
    <ol>
     <li> We assume Q is standardised, of order n. </li>
     <li> Consider two rows R_i, R_j of Q, where we want to check whether
     R_i * R_j = R_p for some p. </li>
     <li> First we evaluate p' := (R_i * R_j)(1) in constant time. </li>
     <li> Now there is a unique p with R_p(1) = p' (which can be found
     in constant time, given the obvious preparations). </li>
     <li> It remains to check whether R_i * R_j = R_p holds ---
     the whole algorithm will run faster than the trivial method (n^3
     steps) iff this can be done faster (amortised) than by running
     through all the n arguments. </li>
     <li> Likely we should do the equivalent test R_i * R_j * R_p^(-1) = id
     (or R_p^(-1) * R_i * R_j = id). </li>
     <li> Say we check R_p^(-1) * R_i * R_j; each permutation is just available
     as a constant time function, with R_p^(-1) precomputed. </li>
     <li> Actually for each p there must be q with R_q = R_p^(-1);
     so better than to store the R_p^{-1} we first find these q,
     which might show that we don't have a group, or otherwise saves
     some storage. And we can use the same trick again, first find the
     right row q, and then just check whether R_q actually is
     the inverse of R_p. </li>
     <li> So we have to check for C := R_q * R_i * R_j whether we
     have C = id. </li>
     <li> We have precomputed the supports (changed places) S_i for
     all rows R_i. Now if |S_q| + |S_i| + |S_j| >= n, then we run
     simple through x = 1, ..., n, and check whether the composition C
     gives x back (it's hard to see how to do better here). </li>
     <li> Otherwise, using an array a[t], t in {1,...,n} of natural
     numbers >= 0, initialised to 0, we first run through x in S_j,
     set a[x] to the current round r (one round for each pair (i,j)),
     and check whether the composition C applied to x yields x. </li>
     <li> If yes, we run through y from S_i, if a[y] = r, then ignore y,
     otherwise set a[y] to r, and plug in y to the composition
     S_q * S_i, checking whether y is the result. </li>
     <li> If yes, then finally we check whether for all z in S_q we have
     a[z] = r (otherwise we return false). </li>
    </ol>
   </li>
   <li> The whole thing should be implemened and tested (for run time (using,
   say, random matrices, and comparing it with the trivial test).
    <ol>
     <li> Though creating random quasigroups might be a bit tricky? </li>
    </ol>
   </li>
   <li> We obtain in this way also a test for checking whether a groupoid V
   is a group: First check whether V is a quasigroup, and then apply the
   above. </li>
  </ul>

*/

