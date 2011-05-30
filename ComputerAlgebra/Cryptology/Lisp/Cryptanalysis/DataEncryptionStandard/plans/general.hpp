// Matthew Gwynne, 10.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/plans/general.hpp
  \brief Plans for the cryptanalysis of the Data Encryption Standard in Maxima/Lisp


  \todo Add 6-to-1 bit Sbox functions
  <ul>
   <li> The 10-to-4 bit boolean functions for the DES Sboxes can be split
   into 4 6-to-1 bit functions. </li>
   <li> We must provide these functions at the Maxima level. </li>
   <li> See "Analysing the S-boxes" in
   Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp.
   </li>
  </ul>


  \todo Add variants with reduced number of rounds
  <ul>
   <li> Variants of DES with smaller numbers of rounds (from 1 to 3) are
   considered in [Logical cryptanalysis as a SAT problem; Massaci and
   Marraro]. </li>
   <li> For discussions of the notion of "DES with reduced number of rounds",
   see "Add variants with reduced number of rounds" in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/plans/general.hpp.
   </li>
   <li> The translations of variants with smaller numbers of rounds will be
   investigated just as much as the full DES. </li>
   <li> Tests are needed for all functions. </li>
   <li> DONE Variable ordering:
    <ul>
     <li> The variables returned by des_var should be ordered
     "round by round". That is, the variables for round i+1 should occur after
     round i in the list. </li>
     <li> Using a "round by round" order on the variables ensures that all
     variables from round i always have the same index after standardisation,
     no matter the variant we use. </li>
    </ul>
   </li>
   <li> DONE This "round by round" variable ordering scheme differs from the
   original ordering used for the full DES translation. It is more complex
   and so we keep the original DES translation and put the functions
   for the generalised version in "GeneralisedConstraintTranslation.mac".
   </li>
   <li> DONE We need to implement translations of these variants to compare. </li>
   <li> DONE We should offer generalised versions of the current functions. </li>
   <li> DONE These generalised functions should take the number r of rounds. </li>
   <li> DONE So the following functions should be created with an additional round
   parameter r:
    <ul>
     <li> des_xor() |-> des_xor_gen(r). </li>
     <li> des_sboxc() |-> des_sboxc_gen(r). </li>
     <li> des_var() |-> des_var_gen(r). </li>
     <li> des_cipher2fcl(cipher) |-> des_cipher2fcl_gen(r,cipher). </li>
     <li> des2fcl(sbox_l) |-> des2fcl_gen(r,sbox_l). </li>
    </ul>
   </li>
   <li> DONE (To test the water first, we implement "_gen" versions of
   each function)
   Therefore in this case, the full DES should be considered a special
   case of the general scheme with an arbitrary number of rounds. A special
   convenience function needs then to be provided for the translation of
   the 16 round DES. </li>
  </ul>


  \todo Improve tests
  <ul>
   <li> The tests "okltest_des_sbox_fulldnf_cl" and 
   "okltest_des_sbox_fullcnf_fcs" only test Sboxes 1 and 5. </li>
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

