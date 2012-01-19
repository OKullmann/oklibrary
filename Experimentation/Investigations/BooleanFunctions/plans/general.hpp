// Oliver Kullmann, 5.12.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/general.hpp
  \brief On investigations regarding boolean functions


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
   <li> The hardness of pigeon-hole formulas:
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
   </li>
   <li> The hardness of the following should be investigated:
    <ul>
     <li> pigeon hole formulas </li>
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


  \todo Algebraic normal form
  <ul>
   <li> The algebraic normal form of an n x 1 boolean function f is as a
   sum of monomials in variables x_1, ..., x_n over the field (boolean ring)
   ZZ_2. </li>
   <li> Implementations of ANFs are discussed in "Algebraic normal form" in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/Basics.hpp. </li>
   <li> Hardness of CNF representations of ANF formulas:
    <ul>
     <li> If an ANF formula isn't directly 0 (i.e., the sum of 0 terms)
     then it represents the constant 0 boolean function, and otherwise it is
     satisfiable. </li>
     <li> So, given a formula in ANF, we can trivially decide if some partial
     assignment falsifies it (just evaluate and check "= 0?"). </li>
     <li> This brings up the obvious relation to hardness, as discussed in
     "Hardness of boolean function representations" above. </li>
     <li> In general, what is the hardness of an ANF formula translated
     to CNF in the standard way?
      <ul>
       <li> Translating the ANF formula p := x_1 * x_2 + x_1 * x_3 as
       standard:
       \verbatim
F := prime_imp(y_1 <-> x_1 * x_2) union prime_imp(y_2 <-> x_1*x_3) union prime_imp(xor(y_1,y_2,1))
       \endverbatim
       yields a 2-hard representation, because the assignment
       phi = <x_2 -> 1, x_3 -> 1> applied to F gives
       \verbatim
phi * F = prime_imp(y_1 <-> x_1) union prime_imp(y_2 <-> x_1) union prime_imp(xor(y_1,y_2,1))
       \endverbatim
       representing the (unnormalised) "ANF" formula x_1 + x_1 which
       normalised to ANF is 0. However, there are no unit-clauses in phi * F,
       hence F is 2-hard.
       </li>
       <li> Note the translation of p being 2-hard does not depend on the
       representations for the boolean functions y_i <-> X and XOR in the
       standard ANF to CNF translation. </li>
       <li> MG conjectures that for any number of variables n in NN, there
       exists an ANF formula for which the standard CNF translation is
       (n-2)-hard:
        <ul>
         <li> Consider such an n, and the formula:
         \verbatim
x_3 * x_1 + x_3 * x_2 + ... + x_n * x_1 + x_n * x_2
         \endverbatim
         which has the translation with new variables y_1, ..., y_{2(n-2)};
         each new variable representing a product in the ANF formula.
         </li>
         <li> The assignment <x_1 -> 1, x_2 -> 1> falsifies this formula,
         and leaves us with 2(n-2) = 2n -4 equalitys y_1 <-> x_3, y_2 <-> x_3,
         ..., y_{2n-5} <-> x_n, y_{2n-4} <-> x_n in the CNF representation. </li>
         <li> It appears that r_{n-2} is needed to "break" all of these
         equalities (one level each). </li>
        </ul>
       </ul>
      </ul>
     </li>
     <li> Are there 1-soft translations of all ANF formulas into CNF? </li>
     <li> We should implement the functions needed to deal with ANF formulas
     and their conversion to CNF and then investigate this empirically. </li>
    </ul>
   </li>
  </ul>


  \todo Boolean function representations
  <ul>
   <li> We need a list of all the representations we use for
   boolean functions. </li>
  </ul>


  \bug analyse_random_permutations does not create experiment-directories
  <ul>
   <li> Every experiment takes places in its own directory. </li>
   <li> This standard has been explained and used many times. </li>
   <li> Perhaps MG would finally take notice of it. </li>
   <li> Also the weak Bash-usage has to be corrected. </li>
   <li> And the other scripts in this directory need also to be corrected.
   </li>
   <li> Every program and so on carries a proper name, and thus scripts use
   capital letters. </li>
  </ul>


  \todo All essential computations of analyse_random_permutations at C++ level
  <ul>
   <li> The C++ simulation of the Maxima random-generator then needs to be
   reconsidered. </li>
  </ul>


  \todo Write "analyse_all_permutations"
  <ul>
   <li> Similar to analyse_random_permutations, but now considering all
   permutations. </li>
   <li> See investigate_permutations in
   Experimentation/Investigations/BooleanFunctions/Permutations.mac for
   a Maxima role-model. </li>
   <li> Once we have a full-blown C++ implementation, then likely the simple
   Maxima-function should be removed. </li>
  </ul>


  \todo Add milestones


  \bug (DONE Added modules to buildsystem makefile)
  Application tests not run
  <ul>
   <li> Yet no recursive descend takes place in module BooleanFunctions. </li>
  </ul>


  \todo DONE (see BooleanFunctions.hpp)
  Plans for investigating random boolean functions
  <ul>
   <li> We need to investigate random boolean functions to
   compare with the AES investigations (see 
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp).
   </li>
   <li> Likely, we should create a new plans file for arbitrary boolean
   functions which may not fit into other categories? </li>
   <li> See "First considerations of random permutation" in 
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp for an
   example of similar investigations into permutations. </li>
  </ul>



*/

