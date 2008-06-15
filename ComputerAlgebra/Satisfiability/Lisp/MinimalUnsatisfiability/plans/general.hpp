// Oliver Kullmann, 19.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp
  \brief Plans for Maxima-components regarding minimally unsatisfiable clause-sets


  \todo Create milestones


  \todo Create tests


  \todo Connections to other modules
  <ul>
   <li> In
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/HittingClauseSets.mac
   we have 2 generators for elements of SMUSAT(1). </li>
   <li> Further generators in
   ComputerAlgebra/Satisfiability/Lisp/Generators/Generators.mac. </li>
   <li> See Experimentation/Investigations/plans/general.hpp. </li>
  </ul>


  \todo Overview on generators for MUSAT
  <ul>
   <li> We need an overview on all possibilities to create elements of
   MUSAT. </li>
   <li> In ComputerAlgebra/Satisfiability/Lisp/Generators/Generators.mac
   we have, besides the hitting clause-sets:
    <ol>
     <li> weak_ph(n+1,n) </li>
     <li> usat_musat(FF) for unsatisfiable FF </li>
     <li> sat_musat(FF) for satisfiable FF </li>
    </ol>
   </li>
  </ul>


  \todo Maximal min-var-degrees
  <ul>
   <li> The general quest here is for example which show that the bound
   max_min_var_degree_def is sharp. </li>
   <li> See Experimentation/Investigations/plans/MaximiseMinVarDegrees.hpp
   for the general investigation. </li>
   <li> Perhaps the considerations regarding hitting clause-sets should go
   to a module regarding hitting clause-sets ?
    <ol>
     <li> The smallest deficiency where we do not have an example is 6. </li>
     <li> See "derived_hitting_cs_pred_isoelim" in
     ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
     for how to find examples. </li>
    </ol>
   </li>
  </ul>


  \todo Singular extensions
  <ul>
   <li> Transfer from Orthogonal.mup (see
   ComputerAlgebra/Mupad/plans/general.hpp).
    <ol>
     <li> MUSAT1 </li>
     <li> SMUSAT1 </li>
     <li> MMUSAT1 </li>
     <li> DONE (si_inverse_singulardp_fcs; M -> FF, k -> b, p, q as before)
     SingExt </li>
     <li> DONE (it_si_inverse_singulardp_fcs)
     ISingExt </li>
    </ol>
   </li>
   <li> The function it_si_inverse_singulardp_fcs is very simple,
   and perhaps we can provide more intelligent functionality:
      <ul>
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
     </li>
     <li> Though likely also the simple iteration with fixed p,q,a,b
     has its uses:
      <ul>
       <li> The question is how to handle failures. </li>
       <li> The old implementation aborted; this is a possibility. </li>
       <li> But we could also offer the same approximations as above. </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> Creation of random subsets:
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
   </li>
   <li> We want three types of extensions:
    <ol>
     <li> DONE (arbitrary p,q-values)
     the non-degenerated extensions, characteristic for singular
     DP-reductions on MU; </li>
     <li> DONE(p=q=1)
     the saturated extensions, characteristic for singular
     DP-reductions on SMU; </li>
     <li> the hitting extensions, characteristic for singular
     DP-reductions on UHIT. </li>
    </ol>
   </li>
   <li> We should have systematic versions, which try to generate
   all isomorphism types for a given number of steps.
    <ol>
     <li> But it appears that needs to be goal-oriented, since just
     all extensions appear to be far too many. </li>
    </ol>
   </li>
   <li> And we should have simple randomised versions (as with the current
   Mupad-code). </li>
   <li> The first questions concern the characterisation of MU(delta=2),
   SMU(delta=2) and UHIT(delta=2) (always looking at all clause-sets, not
   just the non-singular one). </li>
  </ul>

*/

