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


  \todo Special properties of the greedoid of singular sets
  <ul>
   <li>
   \verbatim
F : {{1,2},{-1,3},{-1,-3},{-2,4},{-2,-4}};
min_unsat_bydef_cs(F);
  true
deficiency_cs(F);
  1
G : sdphg_cs_bydef(F);
  [{1,2,3,4},
   {{},{1},{1,2,3},{1,2,3,4},{1,2,4},{1,3},{1,3,4},{1,4},{2},{2,3},{2,3,4},{2,4},{3},{3,4},{4}}]
grdfs_p(G);
  true
antimtrfs_p(G);
  false
rank_grdfs(G);
  6
bases_grdfs(G);
  {{1,2,3,4}}
intervalgrdfs_p(G);
  false
gaussian_ss_p(G[2]);
  true
   \endverbatim
   </li>
   <li>
   \verbatim
F : {{1,2},{-1,3},{-2,4},{-3,5},{-3,-5},{-4,6},{-4,-6}};
min_unsat_bydef_cs(F);
  true
deficiency_cs(F);
  1
G : sdphg_cs_bydef(F);
[{1,2,3,4,5,6},
 {{},{1},{1,2},{1,2,3},{1,2,3,4,5},{1,2,3,4,5,6},{1,2,3,4,6},{1,2,3,5},{1,2,3,5,6},
  {1,2,3,6},{1,2,4},{1,2,4,5},{1,2,4,5,6},{1,2,4,6},{1,2,5},{1,2,5,6},{1,2,6},{1,3},{1,3,4},
  {1,3,4,5},{1,3,4,5,6},{1,3,4,6},{1,3,5},{1,3,5,6},{1,3,6},{1,4},{1,4,5},{1,4,5,6},{1,4,6},
  {1,5},{1,5,6},{1,6},{2},{2,3},{2,3,4},{2,3,4,5},{2,3,4,5,6},{2,3,4,6},{2,3,5},{2,3,5,6},
  {2,3,6},{2,4},{2,4,5},{2,4,5,6},{2,4,6},{2,5},{2,5,6},{2,6},{3},{3,4},{3,4,5},{3,4,5,6},
  {3,4,6},{3,5},{3,5,6},{3,6},{4},{4,5},{4,5,6},{4,6},{5},{5,6},{6}}]
grdfs_p(G);
  true
antimtrfs_p(G);
  false
rank_grdfs(G);
  6
bases_grdfs(G);
  {{1,2,3,4,5,6}}
intervalgrdfs_p(G);
  false
gaussian_ss_p(G[2]);
  true
   \endverbatim
   </li>
   <li> So is every such greedoid Gaussian ?! </li>
  </ul>

*/
