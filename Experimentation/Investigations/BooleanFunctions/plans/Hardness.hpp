// Oliver Kullmann, 28.2.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/Hardness.hpp
  \brief On investigations regarding the hardness of representations of boolean functions


  \todo Hardness of boolean function representations
  <ul>
   <li> The hardness h(F) of a CNF F is defined to be the minimum k such that
   for all implicates C of F, we have that r_k(phi_C * F) yields the
   empty clause. </li>
   <li> We have that:
    <ul>
     <li> h(F) = 0 if F contains all it's prime implicates. </li>
     <li> h(F) <= h(F') where F' is the canonical (full) CNF representation
     of F. </li>
     <li> If F is a full clause-set, i.e., only clauses of length n(F)),
     then h(F) = n(F) - m where m is the size of a prime implicate of
     F with the minimum clause-length. </li>
     <li> If F is renamable horn then h(F) <= 1. </li>
     <li> If F is a 2-SAT formula then h(F) <= 2. </li>
    </ul>
   </li>
   <li> For a boolean function f, then the "hardest" representation F, without
   new variables, is the canonical CNF representation of f. This follows
   from the fact that for every partial assignment phi, applying phi to F
   yields the canonical unsatisfiable CNF on the remaining variables, which
   is of maximum hardness. </li>
   <li> How does this hardness notion relate to the performance of SAT solvers
   on existing problems? </li>
   <li> Investigations into how representations with different hardness
   affect SAT solving is being investigated in:
    <ul>
     <li> AES: Cryptography/AdvancedEncryptionStandard/plans/general.hpp. </li>
     <li> DES: Cryptography/DataEncryptionStandard/plans/general.hpp. </li>
    </ul>
   </li>
   <li> The hardness of the following should be investigated:
    <ul>
     <li> cardinality constraints, see
     Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp.
     </li>
    </ul>
   </li>
   <li> Computing the hardness of small functions:
    <ul>
     <li> We do not, in general, compute the hardness of boolean function
     representations, but construct representations with low hardness. </li>
     <li> However, for experimental evaluation and curiousities sake, we can
     and should measure the hardness of certain representations of small
     boolean functions. </li>
     <li> For example, we should measure the hardness of the minimum
     representations of the DES and AES boxes. See
     AdvancedEncryptionStandard/plans/Representations/general.hpp.
     </li>
     <li> See "Computing the hardness of a clause-set representation"
     in Satisfiability/Lisp/Reductions/plans/general.hpp. </li>
     </li>
    </ul>
   </li>
  </ul>


  \todo The hardness of pigeon-hole formulas
  <ul>
   <li> See "Extended Resolution clauses for the Pigeon Hole Principle" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/general.hpp. </li>
   <li> The hardness of weak_php(m,m-1) vs weak_php_unsat_ext_fcs(m):
   \verbatim
maxima> for m : 1 while true do
  print([hardness_wpi_cs(weak_php_fcs(m,m-1)[2],{{}}),
         hardness_wpi_cs(weak_php_unsat_ext_fcs(m)[2],{{}})])$
[0,0]
[1,1]
[2,2]
[3,3]
[4,4]
     \endverbatim
   </li>
   <li> weak_php_unsat_ext_fcs(m) has a polynomial size resolution proof.
   However, what is the complexity of the smallest tree resolution proof?
   </li>
  </ul>

*/
