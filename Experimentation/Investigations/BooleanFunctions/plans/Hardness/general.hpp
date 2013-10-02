// Oliver Kullmann, 28.2.2012 (Swansea)
/* Copyright 2012, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/Hardness/general.hpp
  \brief On investigations regarding the hardness of representations of boolean functions


  \todo Connections
  <ul>
   <li> See Satisfiability/Lisp/FiniteFunctions/plans/TseitinTranslation.hpp.
   </li>
   <li> See Investigations/PigeonholeFormulas/plans/Hardness.hpp. </li>
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
     <li> h(F) = 0 if F contains all its prime implicates. </li>
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


  \todo Hardness of canonical translation of positive DNF
  <ul>
   <li> For a positive DNF-clause-set F, the set of CNF-prime-clauses is
   the transversal hypergraph of F, with element-wise negation. </li>
   <li> One would guess that if F is far away from a variable-disjoint
   hitting clause-set, then the canonical translation dualts_cs(F) should have
   high hardness. </li>
   <li> The boolean function of the DNF F could then also have the property
   of not having a small CNF-representation of small hardness. </li>
   <li> If F is a positive 2-CLS, then we have hardness at most 1:
   \verbatim
test1(n) := hardness_cs(dualts_cs(powerset(setn(n),2)))$
   \endverbatim
   n <= 1 yield hardness 0, all other n yield hardness 1. </li>
   <li> 3-CLS:
   \verbatim
hardness_cs(dualts_cs({{1,2,3},{2,3,4}});
  1
   \endverbatim
   </li>
  </ul>


  \todo Hardness of Sudoku problems
  <ul>
   <li> As discussed in "Finding hard instances for box-dimension 3" in
   Investigations/LatinSquares/Sudoku/plans/general.hpp, the right approach
   for determining the "hardness" of Sudoku problems should be to determine
   their t-, p- and w-hardness! </li>
   <li> Also the direct encoding is very natural here. </li>
   <li> An article about this should be very attractive! </li>
   <li> For this we need hardness-computation at C++ level. </li>
  </ul>


  \todo Relation of whardness=2 and hardness
  <ul>
   <li> Example for whardness=2 and hardness=3:
   \verbatim
F : {{2,3,4},{-4,2},{-2,1,5},{-5,-2},{-3,1,6},{-6,-3},{7,8,9},{-9,7},{-7,-1,10},{-10,-7},{-8,-1,11},{-11,-8}};
current_satsolver(cs2fcs(F));
  false
hardness_u_cs(F);
  3
whardness_u_cs(F);
  2
   \endverbatim
   </li>
  </ul>

*/
