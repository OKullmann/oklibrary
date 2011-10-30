// Matthew Gwynne, 28.7.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/ConstraintTemplateRewriteSystem.hpp
  \brief Plans for the constraint rewrite system


  \bug Test failure with okltest_aes_sbox_pi_cst_cl
  <ul>
   <li>
   \verbatim
okltest_aes_sbox_pi_cst_cl(aes_sbox_pi_cst_cl)
ASSERT: Expression " {-10,-9,-7,-6,-5,3,4} = {-16,-15,-14,-13,4,11,12} " does not evaluate to true.
   \endverbatim
   </li>
  </ul>


  \bug Test failure with okltest_aes_mul3_pi_cst_cl
  <ul>
   <li> What is the point of these hardcoded values, which apparently do not
   have a mathematical meaning? </li>
  </ul>


  \todo Notion of "constraint"
  <ul>
   <li> Currently in the AES translation system we use the notion of
   "constraint" and "constraint template"; see
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/docus/Translations.hpp. 
   </li>
   <li> This is not the same as the notion of a constraint used for "linear
   constraints in 'The notion of a "linear constraint"' in
   ComputerAlgebra/Satisfiability/Lisp/LinearConditions/plans/general.hpp.
   </li>
   <li> For linear constraints, a constraint is very specific and one
   always knows implicitly what it is; there is no name embedded in the object.
   </li>
   <li> In the AES translation system, each constraint has a constraint
   template name, and carries with it all the parameters it needs for
   evaluation (even if not actually evaluated). </li>
   <li> Removing type information from constraints from "AES constraints":
    <ul>
     <li> Rather than dealing with a list of constraints, where the "type"
     of the constraint is listed within the constraint itself, perhaps
     it would be better to have a list of list of constraints where
     different types of constraint are separated into different lists. </li>
     <li> So, for example, instead of:
     \verbatim
[["examp1", [1,...,n],namespace],["examp2",[1,...,m],namespace2],["examp2",[1,...,o],namespace3]]
     \endverbatim
     you have something like:
     \verbatim
[["examp1",[[[1,...,n],namespace]]],["examp2",[[[1,...,m],namespace],[[1,...,o],namespace]]]]
     \endverbatim
     </li>
     <li> The advantages here are that:
      <ul>
       <li> The system avoids pushing type information into the constraint
       object itself, and is therefore more in line with the rest of the
       OKlibrary. </li>
       <li> Determining the number of any given type of constraint is much
       easier. </li>
      </ul>
      The disadvantages are:
      <ul>
      <li> There is some lose of information, in that the order of constraints
      in relation to each other is lost. However, this isn't actually used
      for anything currently. </li>
      <li> The concept is more complicated; rather than just a list/set of
      constraints, we now partition this into lists. </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> Both notions need to be considered and somehow brought together. </li>
   <li> The notion of "constraint" rather than "constraint template" also
   needs to be standardised. </li>
  </ul>

*/

