// Oliver Kullmann, 21.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/Bases/plans/general.hpp
  \brief Plans regarding the computations of "r-bases" of clause-sets


  \todo Connections
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Primality/plans/RBases.hpp
   for the specifications at Maxima-level. </li>
  </ul>


  \todo The notion of r-bases
  <ul>
   <li> Assume a "reduction" r: CLS -> CLS (clause-sets understood as CNFs).
   </li>
   <li> The only assumption on r is that r(F) is sat-equivalent to F, and that
   "r found F unsatisfiable" is visible via r(F) = {{}}. </li>
   <li> The assumption "{} in r(F) <=> r(F) = {{}}" is not used at the
   Maxima-level (see
   ComputerAlgebra/Satisfiability/Lisp/Primality/plans/RBases.hpp), but it
   seems appropriate for the "computational" C++ level. </li>
   <li> Consider now a clause-set F. An "r-base" for F is a sub-clause-set F'
   with the property that every clause C in F-F' follows from F' "via r", that
   is, we have r(vp_C * F') = {{}} (where vp_C is the partial assignment which
   sets every literal in C to false), and where furthermore no clause from F'
   can be removed without destroying this property. </li>
   <li> See rand_rbase_cs(F,red_) in
   ComputerAlgebra/Satisfiability/Lisp/Primality/RBases.mac for a randomised
   computation at Maxima-level of some base. </li>
   <li> With the assumption of minimality we call F' an "r-generating subset
   of F". So the r-bases of F are the minimal r-generating subsets of F. </li>
   <li> A strong additional assumption on r is transitivity, that is,
   if F' is r-generating for F, and if F'' is r-generating for F', then F'' is
   r-generating for F. </li>
   <li> For transitive r one just needs to test at a time whether a single
   clause can be removed, and doesn't need to check whether previously removed
   clauses follows from the current clause-set via r. </li>
   <li> However r_1 doesn't have this property, where an example is as
   follows:
    <ol>
     <li> Let the core F0 be defined as
     \verbatim
F0 : {{1,2,-3,7},{1,2,3,7},{-1,7},{4,5,-6,-7},{4,5,6,-7},{-5,-7}}$
irredundant_bydef_cs(F0);
  true
     \endverbatim
     </li>
     <li> Three implied clauses:
     \verbatim
A0 : {{2,7,8},{4,-7,8},{2,4,8}}$
F1 : union(F0,A0)$
is(all_irr_cores_bydef_cs(F1) = {F0});
  true
     \endverbatim
     (note that the third clause is the resolvent of the first two clauses).
     </li>
     <li> {2,7,8} follows by input-resolution from {1,2,-3,7},{1,2,3,7},{-1,7},
     {4,-7,8} follows by input-resolution from {4,5,-6,-7},{4,5,6,-7},{-5,-7},
     but when both {2,7,8},{4,-7,8} are removed, then {2,4,8} no longer follows
     by input resolution. </li>
     <li> The three r_1-bases of F1 are
     \verbatim
rbase_p_cs(union(F0,{{2,7,8}}),F1,ucp_0_cs);
  true
rbase_p_cs(union(F0,{{4,-7,8}}),F1,ucp_0_cs);
  true
rbase_p_cs(union(F0,{{2,4,8}}),F1,ucp_0_cs);
  true
all_rbases_bydef_cs(F1,ucp_0_cs);
 {{{-7,-6,4,5},{-7,-5},{-7,4,5,6},{-7,4,8},{-3,1,2,7},{-1,7},{1,2,3,7}},
  {{-7,-6,4,5},{-7,-5},{-7,4,5,6},{-3,1,2,7},{-1,7},{1,2,3,7},{2,4,8}},
  {{-7,-6,4,5},{-7,-5},{-7,4,5,6},{-3,1,2,7},{-1,7},{1,2,3,7},{2,7,8}}}
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo r_0-base
  <ul>
   <li> Every clause-set F has exactly one r_0-base, namely the result of
   subsumption-elimination. </li>
   <li> r_0 is transitiv (see above). </li>
   <li> Perhaps the perspective of applying partial assignments is interesting
   here --- just being able to apply partial assignments and detecting the
   empty clause we are able to perform subsumption-elimination. </li>
  </ul>


  \todo r_1-bases
  <ul>
   <li> The first task is to compute r_1-bases (one could also speak of
   "UCP-bases"). </li>
   <li> The r_1_bases of a clause-set F are exactly those minimal F' <= F such
   that all C in F-F' can be derived from F' modulo subsumption by
   input-resolution. </li>
   <li> Important for efficiency, that elimination of clauses and moving to
   a different clause (as partial assignment) can be done without much
   overhead. </li>
   <li> It seems that UnitClausePropagation::CLSAdaptorUcpW basically fulfills
   these requirements:
    <ol>
     <li> The assignment can be just placed into the partial assignment,
     without the need to further actions. </li>
     <li> Once a propagation has been performed, then nothing needs to be
     reset, since the watched literals per clause are always valid. </li>
     <li> For removing a clause the interface has to be changed, but
     otherwise it should be a simple operation, just removing the two
     watched literals. </li>
     <li> Perhaps we have to use the underlying clause-class
     Clauses::WatchedLiterals_mono? </li>
    </ol>
   </li>
   <li> The problem with the Maxima-specification rand_rbase_cs is how to
   reproduce the Maxima-function random_permutation:
    <ol>
     <li> The Maxima-documentation says that the "Knuth shuffle algorithm"
     is used. </li>
     <li> And then we need to simulate the Maxima random-generator. </li>
     <li> This should all be possible without too much effort. </li>
     <li> However perhaps for now we just use examples with a unique base.
     </li>
     <li> Though we need to take these considerations into account for the
     design. </li>
    </ol>
   </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Reductions::Bases
  \brief Components for computing r-bases

  Alias "Bas".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Reductions {
      namepsace Bases
      }
      namespace Bas = Bases;
    }
  }
}

