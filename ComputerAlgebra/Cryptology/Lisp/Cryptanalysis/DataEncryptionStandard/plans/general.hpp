// Matthew Gwynne, 10.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/plans/general.hpp
  \brief Plans for the cryptanalysis of the Data Encryption Standard in Maxima/Lisp

  \todo Convenience functions
  <ul>
   <li> Convenience functions are needed for the translations of the
   DES and generalised-DES to SAT. </li>
   <li> A function such as output_des_fcl(sbox_l,n) should be written
   which outputs the result of des_fcl(sbox_l) to the file n. </li>
   <li> We also need a function, output_des_fcl_kd(sbox_l,seed,n), for
   outputting a random DES key discovery instance. </li>
   <li> output_des_fcl_kd should generate a random plaintext and ciphertext
   using the seed, and output the result of des_fcl, with the plaintext
   and ciphertext unit-clauses to the file n. </li>
  </ul>


  \todo Variable ordering and standardisation
  <ul>
   <li> At present, the variables in the standard translation of the full DES,
   des2fcl(...)[1], are in the order:
    <ol>
     <li> First half of input: 32 variables, desr(i,-1) for i in
     [1,...,32]; </li>
     <li> Second half of input: 32 variables, desr(i,0) for i in
     [1,...32]; </li>
     <li> Result of each round: 16 * 32 = 512 variables, desr(i,r) for r in
     [1,...,16] and i in [1,...,32]; </li>
     <li> Key variables: 64 variables, desk(i) for i in [1,...,64]; </li>
     <li> Result of xor with key in each round: 16 * 48 = 768 variables,
     desi(i,r) for r in [1,...,16] and i in [1,...,48]; </li>
     <li> Result of S-boxes: 16 * 32 = 512 variables, deso(i,r) for r
     in [1,...,16] and i in [1,...,32]. </li>
     <li> Auxilliary variables for Sbox translations. </li>
    </ol>
   </li>
   <li> The variables in the generalised translation of the m-round DES,
   des2fcl_gen(...,r)[1], are in the order:
    <ol>
     <li> Key variables: 64 variables, desk(i) for i in [1,...,64]; </li>
     <li> First half of input: 32 variables, desr(i,-1) for i in
     [1,...,32]); </li>
     <li> Second half of input: 32 variables, desr(i,0) for i in
     [1,...32]; </li>
     <li> For each round r in [1,...,16]:
      <ol>
       <li> Result of each round: 16 * 32 = 512 variables, desr(i,r) fpr
       i in [1,...,32]; </li>
       <li> Result of xor with key in each round: 16 * 48 = 768 variables,
       desi(i,r) for r in [1,...,16] and i in [1,...,48]; </li>
       <li> Result of S-boxes: 16 * 32 = 512 variables, deso(i,r) for r
       in [1,...,16] and i in [1,...,32]. </li>
      </ol>
     </li>
     <li> Auxilliary variables for Sbox translations. </li>
    </ol>
   </li>
   <li> These variable orderings were based on the the general structure
   and order of the DES specification. </li>
   <li> There is a clear separation between the standard DES and generalised
   DES, and so they have different variable orderings. </li>
   <li> We must decide whether these variable orderings are suitable,
   and whether the order makes sense when standardising with standardise_fcl:
    <ul>
     <li> In the translation of the generalised DES, the current order of
     the variables ensures that variables have the same index, whether they
     appear in a translation of 10-round DES or 12-round DES. </li>
     <li> This is a desirable property, as it makes it easier to compare
     translations, and to remember which variables are which, as the number
     of rounds in the translation changes. </li>
     <li> A key question is: should the order of the auxilliary variables
     be fixed? </li>
     <li> It is better not to include auxilliary variables in the
     "round by round" variables of the generalised translation. That way,
     the round variables, such as desi(...) etc, keep the same index across
     different Sbox translations of the DES. </li>
    </ul>
   </li>
  </ul>


  \todo Combining 6-to-1 bit Sbox functions
  <ul>
   <li> Consider the 4 boolean 6 x 1 functions for each DES Sbox;
   see des_sbox_bit_fulldnf_cl and des_sbox_bit_fullcnf_fcs in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/Sboxes.mac.
   </li>
   <li> We can compute many different representations of these functions
   (minimum, canonical, r_1-base etc). </li>
   <li> Whatever the representation, these clause-sets need to be
   recombined to derive a new representation of each Sbox. </li>
   <li> Therefore we need convenience functions to handle this
   recombination. </li>
   <li> The convenience function needs to take 4 S-box formal clause-lists,
   one for each output bit, and return a new formal clause-list which is
   the union of the four inputs with suitable renaming. </li>
   <li> To make variable renaming possible, we assume that input and output
   variables for the S-box are standardised to [1,...,6] and [7,...,10]. </li>
   <li> The variables for the four inputs do not need renaming. </li>
   <li> The output variables for the S-boxes must be renamed from 7 to
   7,8,9 and 10 in the 4 cases. </li>
   <li> Auxilliary variables used in some representations of the S-box
   functions must then also be renamed. </li>
   <li> We have two options regarding auxilliary variables:
    <ol>
     <li> standardise all variables to integers; </li>
     <li> use an unevaluated function dessbox_aux_var which can
     take parameters based on which output bit the auxilliary variable
     pertains to. </li>
    </ol>
   </li>
   <li> Standardising all variables to integers introduces dependencies
   between the clause-sets for the different output-bits when renaming.
   </li>
   <li> What the auxilliary variables for the output-bit 2 are renamed
   to would depend on how many auxilliary variables are used for output-bit 1.
   </li>
   <li> Using an unevaluated function avoids this problem, we can making the
   sets of variables disjoint by using an index to indicate which output
   bit the auxilliary variables come from. </li>
   <li> So we have the renaming:
   \verbatim
/* Auxilliary variables used in standardising a clause-set representing
   a boolean function for i-th output bit of a DES S-box. */
kill(dessbox_6t1_aux)$
declare(dessbox_6t1_aux, noun)$
declare(dessbox_6t1_aux, posfun)$
dessbox_6t1_aux_var(i,v) := nounify(dessbox_6t1_aux)(i,v)$

dessbox_6t1_vardisj_rename_fcl(F,i) := 
  rename_fcl(F,append([1,2,3,4,5,6,6+i],
                      map(lambda([v], dessbox_6t1_aux_var(i,v)),rest(F[1],7))))$
   \endverbatim
   </li>
   <li> We use dessbox_6t1_aux_var directly, rather than standardising
   "v", so that we keep as much information about where the original variable
   comes from. </li>
   <li> Standardisation to integer variables should happen only at the last
   moment. </li>
   <li> The function to combine all four S-boxes then becomes:
   \verbatim
dessbox_6t1to6t4_fcl(F1,F2,F3,F4) := block([F1_r,F2_r,F3_r,F4_r],
  F1_r : dessbox_6t1_vardisj_rename_fcl(F1,1),
  F2_r : dessbox_6t1_vardisj_rename_fcl(F2,2),
  F3_r : dessbox_6t1_vardisj_rename_fcl(F3,3),
  F4_r : dessbox_6t1_vardisj_rename_fcl(F4,4),
  [stable_unique(
     append([1,2,3,4,5,6,7,8,9,10],F1_r[1],F2_r[1],F3_r[1],F4_r[1])),
   stable_unique(append(F1_r[2],F2_r[2],F3_r[2],F4_r[2]))])$
   \endverbatim
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
   <li> A translation into SAT is now available with the function
   des2fcl in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/ConstraintTranslation.mac.
   </li>
   <li> To translate to CSP, we must pick a constraint modelling language
   and/or solver; see "Constraint modelling languages" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp. </li>
   <li> Firstly we need a translation to a representation of
   a constraint system at the Maxima level; see
   "Representing CSP problems" in
   ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/plans/general.hpp.
   </li>
   <li> This can then be translated to solver-specific formats
   such as Minion; see "Translating systems of constraints to Minion" in
   ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/plans/general.hpp.
   </li>
  </ul>


  \todo DONE (see des_sbox_bit_fulldnf_cl)
  Add 6-to-1 bit Sbox functions
  <ul>
   <li> The 10-to-4 bit boolean functions for the DES Sboxes can be split
   into 4 6-to-1 bit functions. </li>
   <li> We must provide these functions at the Maxima level. </li>
   <li> See "Analysing the S-boxes" in
   Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp.
   </li>
  </ul>

*/

