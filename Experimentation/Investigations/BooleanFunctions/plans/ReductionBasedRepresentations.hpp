// Matthew Gwynne, 10.8.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/ReductionBasedRepresentations.hpp
  \brief On investigations into boolean function representations with different inference properties

  \todo r-based representations
  <ul>
   <li> Defining an "r-based representation":
    <ol>
     <li> Let r be a reduction and F be an active clause-set. </li>
     <li> F now is "r-based" if for a partial assignment phi with 
     var(phi) <= var(F) such that phi * F is unsatisfiable we have that 
     r-reduction applied to F yields a contradiction. </li>
     <li> F is "partially r-based" if F is r-based with respect
     to partial assignments on some subset V of var(F), and F is totally
     r-based if V = var(F). </li>
     <li> F is "strongly r-based" if F is r-based and for all partial 
     assignments phi, we have that the r-reduction applied to F has
     no forced assignments. </li>
     <li> Natural examples are r = r_k (k a natural number >= 0). </li>
     <li> The weaker r the stronger is the representation (w.r.t. inference
     power). </li>
     <li> If F is a CNF-clause-set, then being r_0-based is equivalent to F
     containing all prime implicates of C. </li>
     <li> See "Bases of the set of prime clauses relative to some reduction" in
     ComputerAlgebra/Satisfiability/Lisp/Primality/plans/general.hpp
     for the case where no additional variables are considered. </li>
     <li> The function rand_rbase_cs(F,r) in
     ComputerAlgebra/Satisfiability/Lisp/Primality/RBases.mac randomply
     computes an r-base of F. </li>
    </ol>
   </li>
  </ul>


  \todo Positive representations
  <ul>
   <li> Defining a positive representation 
    <ol>
     <li> Consider a boolean condition C on variables V (that is, C(f)
     is a boolean value for total assignments f, and C depends only on V).
     </li>
     <li> A "positive representation" R is an active clause-set on variables
     V' >= V such that if R(f) is true then also C(f) is true, and for f
     such that C(f) is true there is a unique f' which differs from f only
     on V' - V such that C'(f) is true. </li>
    </ol>
   </li>
   <li> r-based positive representations 
   <ol>
     <li> See "r-based representations". </li>
     <li> In general, if we say a positive representation R of a condition
     C is r-based, then R is *partially* r-based with respect to
     V. If R is r-based with respect to V', then we make this explicit by
     calling R "totally r-based". </li>
     <li> Note that, in general, we do not consider how the effect of 
     assignments to variables in V' - V. It seems sensible to me (OK) to 
     consider this as the basis, but for further refinements considerations of
     V' - V might be needed. </li>
     <li> Another sensible condition here is for f with C(f) true how difficult
     it is to compute the extension f'. For example one could demand that
     the additional assignments follow by unit-clause propagation. </li>
   </ol>
   </li>
  </ul>

*/

