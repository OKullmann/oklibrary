// Oliver Kullmann, 5.12.2009 (Swansea)
/* Copyright 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/general.hpp
  \brief On investigations regarding boolean functions


  \todo Algebraic normal form
  <ul>
   <li> The algebraic normal form of an n x 1 boolean function f is as a
   sum of monomials in variables x_1, ..., x_n over the field (boolean ring)
   ZZ_2. </li>
   <li> Implementations of ANFs are discussed in "Algebraic normal form" in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/Basics.hpp. </li>
   <li> Hardness of CNF representations of ANF formulas:
    <ul>
     <li> An ANF formula represents the constant 0 function iff it is 0. </li>
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

*/

