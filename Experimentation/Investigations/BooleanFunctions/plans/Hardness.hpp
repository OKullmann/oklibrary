// Oliver Kullmann, 28.2.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/Hardness.hpp
  \brief On investigations regarding the hardness of representations of boolean functions


  \todo Connections
  <ul>
   <li> See "Hardness" in
   Investigations/PigeonholeFormulas/plans/ExtendedResolution.hpp. </li>
  </ul>


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
     <li> If F is renamable horn, then h(F) <= 1. </li>
     <li> If F is a 2-CNF clause-set, then h(F) <= 2. </li>
    </ul>
   </li>
   <li> For a boolean function f the "hardest" representation F, without
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
     <li> We have the Maxima-functions hardness_wpi_cs, hardness_cs, and
     hardness_u_cs for computing hardness. </li>
     </li>
    </ul>
   </li>
  </ul>


  \todo Propagation hardness of 2-CNF
  <ul>
   <li> F in 2-CLS should even have phardness(F) <= 2. </li>
   <li> This should be investigated also experimentally (to strengthen our
   system). </li>
  </ul>


  \todo Hardness of prime-extremal satisfiable general Horn clause-sets
  <ul>
   <li> The clause-sets sat_genhorn_cs(k,l) are candidates to separate
   representations without new variables of softness l+1 from softness l:
    <ol>
     <li> sat_genhorn_cs(k,l) itself has hardness min(k,l). </li>
     <li> So for l >= 0 the sequence S = (sat_genhorn_cs(k,l+1))_{k >= l+1} has
     a linear-size representation without new variables of hardness precisely
     l+1, namely itself. </li>
     <li> Now the conjecture is that S has no polysize representations without
     new variables of softness l. </li>
     <li> This is true for l=0, as it follows from the results of the article
     [Sloan, Soereny, Turan, On k-term DNF with the largest number of prime
     implicants, 2007]. </li>
    </ol>
   </li>
   <li> A systematic study of the boolean functions of sat_genhorn_cs(k,l) is
   needed:
    <ol>
     <li> A good representation (in the general sense) of the underlying
     boolean function is needed. Perhaps this should go to investigations on
     boolean functions in general. </li>
     <li> What is the precise number of variables, clauses, literal occurrences
     ? </li>
     <li> What are precisely the prime implicates and prime implicants? </li>
     <li> What are the m-bases for 0 <= m <= l ? </li>
    </ol>
   </li>
  </ul>

*/
