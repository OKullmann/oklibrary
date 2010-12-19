// Matthew Gwynne, 10.8.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/ReductionBasedRepresentations.hpp
  \brief On investigations into boolean function representations with different inference properties


  \todo Connections
  <ul>
   <li> See "Investigating conditions and their representations" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp
   for applications to the cryptoanalysis of AES. </li>
  </ul>


  \todo r-based clause-sets
  <ul>
   <li> Let r be a reduction and F be an active clause-set. </li>
   <li> F now is "r-based" if for a partial assignment phi such that
   phi * F is unsatisfiable we have that r-reduction applied to F yields a
   contradiction. </li>
   <li> F is "r-based w.r.t. V" for some set V of variables, if F is r-based
   with respect to partial assignments in V, and F is "totally r-based" if we
   can use V = var(F). </li>
   <li> F is "strongly r-based" if F is r-based and for all partial 
   assignments phi we have that the r(phi*F) has no forced
   assignments, that is, there is no variable v in var(F) and e in {0,1}
   such that <v -> e> * phi*F is unsatisfiable. </li>
   <li> Natural examples for r are r = r_k (k a natural number >= 0). </li>
   <li> The weaker r the stronger is the representation (w.r.t. inference
   power). </li>
   <li> If F is a CNF-clause-set, then being r_0-based is equivalent to F
   containing all prime implicates of C. </li>
   <li> See "Bases of the set of prime clauses relative to some reduction" in
   ComputerAlgebra/Satisfiability/Lisp/Primality/plans/RBases.hpp
   for the case where no additional variables are considered. </li>
   <li> The function rand_rbase_cs(F,r) in
   ComputerAlgebra/Satisfiability/Lisp/Primality/RBases.mac randomply
   computes an r-base of F. </li>
  </ul>


  \todo Positive representations
  <ul>
   <li> Defining a positive representation 
    <ol>
     <li> Consider a boolean condition C on variables V (that is, C(f)
     is a boolean value for total assignments f, and C depends only on V).
     </li>
     <li> A "positive representation" R of C is an active clause-set on
     variables V' >= V such that if R(f) is true then also C(f) is true, and
     for f such that C(f) is true there is a unique f' which differs from f
     only on V' - V such that C'(f) is true. </li>
     <li> Perhaps this should better be called a "unique positive
     representation", while in general we demand only the existence of some
     f'. This is equivalently to saying that the projection of the satisfying
     assignments for R to V yield exactly the satisfying assignments for C.
     </li>
    </ol>
   </li>
   <li> r-based positive representations 
   <ol>
     <li> See "r-based representations" above. </li>
     <li> In general, if we say a positive representation R of a condition
     C is r-based, then R is r-based w.r.t. V (where V is the underlying set
     of original variables, while the representation uses V' >= V). </li>
     <li> If R is r-based with respect to V', then we make this explicit by
     calling R "totally r-based". </li>
     <li> Note that, in general, we do not consider the effect of 
     assignments to variables in V' - V. It seems sensible to me (OK) to 
     consider this as the basis, but for further refinements considerations of
     V' - V might be needed. </li>
     <li> Another sensible condition here is for satisfying f for C(f) how
     difficult it is to compute the extension f'. For example one could demand
     that the additional assignments follow by unit-clause propagation. </li>
   </ol>
   </li>
  </ul>

*/

