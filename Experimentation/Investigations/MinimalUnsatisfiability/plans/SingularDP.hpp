// Oliver Kullmann, 7.6.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/MinimalUnsatisfiability/plans/SingularDP.hpp
  \brief Plans regarding singular DP-reduction for minimal unsatisfiable clause-sets


  \todo Are non-1-singular tuples totally singular?
  <ul>
   <li> Let's abbreviate this conjecture with "n1stts". </li>
   <li> Assuming that T is a singular tuple for F:
   \verbatim
counterexample_n1stts(T,F) := singulartuple_csp(T,F) and
 notonesingulartuple_csp(T,F) and not totally_singulartuple_csp_bydef(T,F)$
   \endverbatim
   </li>
   <li> For n <= 6 (num_all_smusat1(6) = 95040):
   \verbatim
all_n_n1stts(n) := block([m:0, S],
 for F in all_smusat1_cs(n) do (
  S : maxnotonesdpst_cs_bydef(F),
  if length(S) > m then m : length(S),
  for t in S do (
   if not subsetp(permutations(t),S) then print("COUNTEREXAMPLE:",F,t)
  )),
 m)$
   \endverbatim
   No counterexample for n <= 5 (return for n=5: 24). And that is true, since
   the conjecture is true for saturated F, since there a non-1-singular tuple
   contains only non-1-singular variables from F. </li>
  </ul>

*/
