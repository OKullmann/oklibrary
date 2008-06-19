// Oliver Kullmann, 17.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/InverseSingularDP.hpp
  \brief Plans on transforming clause-sets by applying inverse singular DP-reduction


  \todo Transfer from Orthogonal.mup
  <ul>
   <li> See ComputerAlgebra/Mupad/plans/general.hpp. </li>
   <li> MUSAT1 </li>
   <li> SMUSAT1 </li>
   <li> MMUSAT1 </li>
   <li> DONE (si_inverse_singulardp_fcs; M -> FF, k -> b, p, q as before)
   SingExt </li>
   <li> DONE (it_si_inverse_singulardp_fcs)
   ISingExt </li>
  </ul>


  \todo More intelligent iteration
  <ul>
   <li> The function it_si_inverse_singulardp_fcs is very simple,
   and perhaps we can provide more intelligent functionality. </li>
   <li> The problem is that the parameters p,q,a,b (where "a" now
   generalises the old variable "nonempty") are fixed in this approach.
   </li>
   <li> "Typically", at the beginning no only very low values of
   a are possible (if b > 1). </li>
   <li> So perhaps a kind of gradient is introduced. </li>
   <li> Or we try to fullfil the given values of a, b (while p,q are
   fixed), and if not possible, then we look at the "best"
   approximation. </li>
   <li> Different strategies for such a "best approximation" are
   possible, lowering b, lowering a, or both. </li>
  </ul>


  \todo Creation of random subsets:
  <ul>
   <li> Looks as if random_sublist, random_subset_si should go to
   the submodule on set-systems (or hypergraphs?). </li>
   <li> random_subset_si is very inefficient:
    <ol>
     <li> If the number of s-subsets is large, then the dual approach
     can be (much) more efficient, searching through all i-subsets
     of literals which have a common occurrence and checking whether
     there are actually s such common occurrences. </li>
    </ol>
   </li>
  </ul>


  \todo We want three types of extensions:
  <ul>
   <li> DONE (arbitrary p,q-values)
   The non-degenerated extensions, characteristic for singular DP-reductions
   on MU. </li>
   <li> The saturated extensions, characteristic for singular DP-reductions
   on SMU.
    <ol>
     <li> The condition is that, additionally to p=q=1, for the main clause,
     given by I in the implementation, there is no other clause, not involved
     in the extension, such that I subsumes this clause (one could then
     add -v to this clause). </li>
     <li> First we just implement this condition, via
     "basic_inverse_saturated_singulardp_fcs(FF,G)", and return false if the
     condition is not fulfilled. </li>
    </ol>
   </li>
   <li> The hitting extensions, characteristic for singular
   DP-reductions on UHIT.
    <ol>
     <li> These are special saturated extensions. </li>
     <li> The additional criterion is that the intersection must clash with
     all old clauses except of the choosen one. </li>
     <li> Here we have special problems with the control, since in most
     cases such an extension won't be possible. </li>
    </ol>
   </li>
  </ul>


  \todo All types of extensions
  <ul>
   <li> We should have systematic versions, which try to generate
   all isomorphism types for a given number of steps. </li>
   <li> But it appears that needs to be goal-oriented, since just
   all extensions appear to be far too many. </li>
  </ul>


  \todo Randomised extensions


  \todo Investigations
  <ul>
   <li> The first questions concern the characterisation of MU(delta=2),
   SMU(delta=2) and UHIT(delta=2) (always looking at all clause-sets, not
   just the non-singular one). </li>
  </ul>

*/

