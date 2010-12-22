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
   <li> However r_1 doesn't have this property: XXX </li>
  </ul>


  \todo r_0-base
  <ul>
   <li> Every clause-set F has exactly one r_0-base, namely the result of
   subsumption-elimination. </li>
   <li> Perhaps the perspective of applying partial assignments is interesting
   here --- just being able to apply partial assignments and detecting the
   empty clause we are able to perform subsumption-elimination. </li>
  </ul>


  \todo r_1-bases
  <ul>
   <li> The first task is to compute r_1-bases (one could also speak of
   "UCP-bases"). </li>
   <li> The r_1_bases of a clause-set F are exactly those F' <= F such that
   all C in F-F' can be derived from F' modulo subsumption by input-resolution.
   </li>
   <li> Important for efficiency, that elimination of clauses and moving to
   a different clause can be done without much overhead. </li>
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
     <li> Perhaps we have to just use the underlying clause-class
     Clauses::WatchedLiterals_mono. </li>
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

