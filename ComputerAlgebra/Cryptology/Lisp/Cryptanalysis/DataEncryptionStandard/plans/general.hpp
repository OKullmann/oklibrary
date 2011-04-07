// Matthew Gwynne, 10.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/plans/general.hpp
  \brief Plans for the cryptanalysis of the Data Encryption Standard in Maxima/Lisp

 
  \todo Combining 6-to-1 bit Sbox functions
  <ul>
   <li> Consider the 4 boolean 6 x 1 functions for each DES Sbox. </li>
   <li> We have functions output_dessbox_bit_fulldnf_stdname and 
   output_dessbox_bit_fullcnf_stdname to compute the these. </li>
   <li> We can compute other representations of these functions
   (minimum, canonical, r_1-base etc). </li>
   <li> Whatever the representation, these clause-sets need to be
   recombined to derive a new representation of each Sbox. </li>
   <li> Therefore we need convenience functions to handle this
   recombination. </li>
   <li> An initial attempt:
   \verbatim
bit_sboxes2sbox(FF1,FF2,FF3,FF4) := block(
  [num_aux_vars : 0, FF1_renamed, FF2_renamed, FF3_renamed, FF4_renamed],
  FF1_renamed :
    rename_fcl(
      FF1,
      append(
        [1,2,3,4,5,6,7],
        create_list(desaux_var(num_aux_vars+j),j,1,length(FF1[1])-7))),
  num_aux_vars : num_aux_vars + length(FF1[1])-7,
  FF2_renamed : 
    rename_fcl(
      FF2,
      append(
        [1,2,3,4,5,6,8],
        create_list(desaux_var(num_aux_vars+j),j,1,length(FF2[1])-7))),
  num_aux_vars : num_aux_vars + length(FF2[1])-7,
  FF3_renamed :
    rename_fcl(
      FF3,
      append(
        [1,2,3,4,5,6,9],
        create_list(desaux_var(num_aux_vars+j),j,1,length(FF3[1])-7))),
  num_aux_vars : num_aux_vars + length(FF3[1])-7,
  FF4_renamed :
    rename_fcl(
      FF4,
      append(
        [1,2,3,4,5,6,10],
        create_list(desaux_var(num_aux_vars+j),j,1,length(FF4[1])-7))),
  lreduce(
    lambda([FF1,FF2],
      [stable_unique(append(FF1[1],FF2[1])),
       stable_unique(append(FF1[2],FF2[2]))]),
    [FF1_renamed,FF2_renamed,FF3_renamed,FF4_renamed]))$
   \endverbatim
   </li>
   <li> Is there some general function here? </li>
   <li> Perhaps we could ask for the number of input, output and auxilliary
   bits? </li>
   <li> Then we assume the variables are in the order, input, output variable,
   auxilliary variables. </li>
  </ul>


  \todo Add variants with reduced number of rounds
  <ul>
   <li> Variants of DES with smaller numbers of rounds (from 1 to 3) are
   considered in [Logical cryptanalysis as a SAT problem; Massaci and
   Marraro]. </li>
   <li> We need to implement translations of these variants to compare. </li>
   <li> We should offer generalised versions of the current functions. </li>
   <li> These generalised functions should take the number r of rounds. </li>
   <li> For discussions of the notion of "DES with reduced number of rounds",
   see "Add variants with reduced number of rounds" in 
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/plans/general.hpp.
   </li>
   <li> We do not use the full 16-round DES constraint translation for
   encryption purposes. XXX What does this mean??? XXX </li>
   <li> The translations of variants with smaller numbers of rounds will be
   investigated just as much as the full DES. </li>
   <li> Therefore, we should add a round parameter to the following functions:
   XXX if parameters are added, then something is REPLACED??? XXX
    <ul>
     <li> des_xor. </li>
     <li> des_sboxc. </li>
     <li> des_var. </li>
     <li> des_cipher2fcl. </li>
     <li> des2fcl. </li>
    </ul>
   </li>
   <li> We should then add convenience functions for the full DES translation.
   </li>
   <li> Variable ordering:
    <ul>
     <li> How to order the variables for variants with smaller numbers of
     rounds? </li>
     <li> Should variables given by des_var, round by round?
     XXX why a question mark??? MG must stop the permanent question-mode,
     as if there would be an alternative --- only then (or if it is really
     doubtful) are question marks to be used XXX
     </li>
    </ul>
   </li>
  </ul>


  \todo Improve tests
  <ul>
   <li> The tests "okltest_des_sbox_fulldnf_cl" and 
   "okltest_des_sbox_fullcnf_fcs" only test Sboxes 1 and 5. </li>
   <li> The tests for "okltest_des_sbox_bit_fulldnf_cl" and
   "okltest_des_sbox_bit_fullcnf_fcs" need improving in the same
   way. </li>
   <li> The tests should cover all Sboxes! </li>
   <li> The tests should also test properties of the Sboxes. 
   See "Analysing the S-boxes" in
   Experimentation/Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp .
   </li>
   </li>
  </ul>


  \todo Create constraint evaluation system
  <ul>
   <li> We wish to provide tests for functions such as "des_round_xor".
   To do this, we must evaluate the generated constraints. </li>
   <li> We need a constraint evaluation system. </li>
   <li> This system should:
    <ul>
     <li> Take a list of constraints. </li>
     <li> Take an assignment to all "input varables". The "input variables"
     should be enough to determine all others. </li>
     <li> Evaluate each constraint using an associated evaluation function.
     For example functions in 
     ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/Cipher.mac.
     </li>
     <li> Propagate the inferred variables for each constraint to all others.
     </li>
     <li> Return the assignment to the variables after all propagations. </li>
    </ul>
   </li>
  </ul>

  
  \todo Links
  <ul>
   <li> For the implementation of the DES see
   Lisp/CryptoSystems/DataEncryptionStandard/plans/general.hpp. </li>
   <li> See 
   Experimentation/Investigations/Cryptography/DataEncryptionStandard/plans/general.hpp.
   </li>
  </ul>


  \todo Translating the constraint-system into SAT, CSP, ...
  <ul>
   <li> In Cryptanalysis/DataEncryptionStandard/ConstraintTranslation.mac
   we compute the "constraint system" corresponding to the DES-constraint.
   </li>
   <li> Now translations into SAT, CSP etc. are needed. </li>
  </ul>

*/

