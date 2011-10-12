// Oliver Kullmann, 21.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/Bases/plans/general.hpp
  \brief Plans regarding the computations of "r-bases" of clause-sets


  \todo Add application tests for RandomUcpBases


  \todo Connections
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Reductions/plans/RBases.hpp
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
   ComputerAlgebra/Satisfiability/Lisp/Reductions/plans/RBases.hpp), but it
   seems appropriate for the "computational" C++ level. </li>
   <li> Consider now a clause-set F. An "r-base" for F is a sub-clause-set F'
   with the property that every clause C in F-F' follows from F' "via r", that
   is, we have r(vp_C * F') = {{}} (where vp_C is the partial assignment which
   sets every literal in C to false), and where furthermore no clause from F'
   can be removed without destroying this property. </li>
   <li> See rand_rbase_cs(F,red_) in
   ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac for a randomised
   computation at Maxima-level of some base. </li>
   <li> Without the assumption of minimality we call F' an "r-generating subset
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
   <li> So likely we should have a dedicated application. </li>
  </ul>


  \todo Docus for RUcpGen
  <ul>
   <li> The notion of an "r-generating" needs to be extracted from "The notion
   of r-bases" and moved to the docus. </li>
   <li> In this way we can refer to these generating sets in specifications
   and other documentation in the library. For example in
   Satisfiability/Reductions/Bases/RandomRUcpBases. </li>
  </ul>


  \todo Computing 1-bases with k clauses using SAT
  <ul>
   <li> The problem of whether there is a 1-base of size k, for a full CNF F,
   can be encoded into SAT as follows:
    <ul>
     <li> Compute the prime implicates prc_0(F) for F. </li>
     <li> Compute the subsumption hypergraph shg(F) for prc_0(F), as done by
     QuineMcCluskeySubsumptionHypergraph. Consider shg(F) as a CNF clause-set.
     </li>
     <li> An assignment to the variables of shg(F) defines a clause-set F'
     which is a representation of F without new variables. </li>
     <li> To shg(F), add the following:
      <ul>
       <li> A new variable v_phi for each falsifying partial assignment
       phi of F. This variable will indicate whether the clause-set F'
       contains the empty-clause or a unit-clause under phi. </li>
       <li> The clauses prc_0(v_phi <-> or(v_1,...,v_m)) for every
       falsifying partial assignment phi of F, where v_1,...,v_m are the
       variables in shg(F) representing the prime implicates in prc_0(F) which
       produce a unit-clause or the empty-clause under phi. </li>
       <li> The clause {v_phi | phi is a falsifying partial assignments of F}.
       </li>
       <li> Cardinality constraint clauses forcing that exactly
       k of the variables in shg(F) must be set to true. </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> By iteratively decreasing k until the clause-set becomes unsatisfiable,
   we can compute a minimum-sized 1-base. </li>
   <li> We should implement this translation and begin experiments
   on the AES and DES instances discussed in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/general.hpp
   and
   Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp.
   </li>
  </ul>


  \bug DONE RandomRUcpBases sorts clauses in ascending order of clause-length
  <ul>
   <li> DONE The documentation for RandomRUcpBases states that it sorts the
   generating set in descending order of clause-length before passing
   it to RUcpBase. </li>
   <li> DONE However, it sorts the generating set in ascending order. </li>
   <li> DONE This must be corrected. </li>
   <li> DONE Also all references must be updated. </li>
  </ul>


  \bug DONE RandomRUcpBases defect
  <ul>
   <li> DONE (improved documentation, layout and variable names)
   At the same time, all aspects should be improved. </li>
   <li> DONE Also documentation should be checked. </li>
   <li> DONE And a link to it needs to be provided. </li>
   <li> DONE The name is not appropriate: "find" is too general, and programs
   start with capital letters. </li>
   <li> DONE This script is also mislocated here. Obviously it belongs to
   Reductions/Bases. </li>
   <li> DONE As usual, when files are created, then an experiment-directory
   needs to be created. </li>
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

