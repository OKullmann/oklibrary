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


  \todo S-box boolean 6xm functions
  <ul>
   <li> Consider the 4 boolean 6 x 1 functions for each DES Sbox;
   see des_sbox_bit_fulldnf_cl and des_sbox_bit_fullcnf_fcs in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/Sboxes.mac.
   </li>
   <li> We wish to translate the DES where the S-box is considered as 4 6x1
   boolean functions. </li>
   <li> There is also the possibility of generalising this to 4/m boolean 6xm
   functions, or even combinations of 1 6x2 and 2 6x1 boolean functions. </li>
   <li> This is possible in several ways:
    <ul>
     <li> We provide several constraint generators:
      <ul>
       <li> des_sboxc: generator for standard S-box constraints as boolean 6x4
       functions.
       </li>
       <li> des_sboxc_6tm: generator for the DES S-boxes as boolean 6xm
       functions, where m in {1,2,4} is given as an additional parameter.
       </li>
       <li> des_sboxc_6t_l: generator for the DES S-boxes for arbitrary
       list L where the S-boxes occur as 6xL[1], 6xL[2],..., 6xL[n]
       boolean functions where n is the length of L. </li>
       <li> des_sboxc_6t_l is likely too much of a generalisation, but
       des_sboxc_6tm and generalisations of des2fcl etc, using des_sbox_6tm
       are definitely necessary. </li>
      </ul>
     </li>
     <li> Translating the 6x4 DES S-boxes to smaller 6x1 or 6x2 constraints
     when we translate to SAT or CSP:
      <ul>
       <li> In this way, des_sboxc and des2fcl remain the same, but when using,
       for instance, des2fcl, the formal clause-lists for each S-box are the
       union of the clause-lists for 4 S-box boolean 6x1 functions. </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> It seems best to provide several constraint generators, so that we
   see all different types of constraint in their own right. </li>
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

 
  \todo Improve constraint generation tests
  <ul>
   <li> %Generators such as des_xor_desi and des_round_xor needs their
   defining properties tested. </li>
   <li> The fact that the result of the generator should represent
   the associated computation must be tested. </li>
   <li> See "Create constraint evaluation system". </li>
  </ul>


  \todo Create constraint evaluation system
  <ul>
   <li> We wish to provide tests for functions such as "des_round_xor".
   To do this, we must evaluate the generated constraints. </li>
   <li> We need a constraint evaluation system. </li>
   <li> Without a constraint evaluation system, it is hard to write
   concise tests. </li>
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
   <li> See "Evaluation functions" in
   ComputerAlgebra/Satisfiability/Lisp/LinearConditions/plans/general.hpp .
   </li>
   <li> The Sbox specific constraint evaluation functions should be defined in
   this module. </li>
  </ul>


  \todo Triple-DES
  <ul>
   <li> We should provide a translation of the Triple-DES into SAT. </li>
   <li> This translation should make use of the current DES translation. </li>
   <li> See also "Triple-DES" in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/plans/general.hpp.
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
   <li> SAT translation:
    <ul>
     <li> A translation into SAT is now available with the function
     des2fcl in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/ConstraintTranslation.mac.
     </li>
    </ul>
   </li>
   <li> Constraint translation:
    <ul>
     <li> We already translate to a system of high level constraints. </li>
     <li> We have two types of constraints in the DES:
      <ul>
       <li> S-box constraints:
        <ul>
         <li> The constraint for the 6x4 boolean function for DES S-box i is
         a list of the form [i,[v_1,...,v_6],[w_1,...,w_4]]. </li>
         <li> i indicates the constraint represents DES S-box i. </li>
         <li> [v_1,...,v_6] are the 6 input variables. </li>
         <li> [w_1,...,w_4] are the 4 output variables. </li>
         <li> How to represent the 6x1 S-box boolean functions? Or more
         generally 6xm for m < 4? </li>
         <li> For the 6x1 S-box boolean functions, we must specify the output
         bit, so we have the constraint
         \verbatim
[i,[v_1,...,v_6],w_1,j]
         \endverbatim
         where w_1 is the j-th output variable of the i-th DES S-box. </li>
         <li> More generally, we have 6xm S-box constraints:
         \verbatim
[i,[v_1,...,v_6],[w_1,...,w_m],[j_1,...,j_m]]
         \endverbatim
         where w_l is the j_l-th output bit of the i-th DES S-box for
         l in {1,...,m}. </li>
        </ul>
       </li>
       <li> XOR constraints:
        <ul>
         <li> See 'The notion of a "linear constraint"' for the details of
         the XOR constraint. </li>
         <li> An XOR constraint is a list [x,b] where x is a list of literals
         and b is in {0,1}, such that the constraint is that
         \verbatim
x[1] + ... + x[l] = b
         \endverbatim
         where l is the length of x.
         </li>
        </ul>
       </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> We treat the S-box (6x4 and 6x1) and XOR constraints as the "boxes" we
   consider, and therefore (currently) do not consider further decompositions.
   </li>
   <li> As with the AES, we do not translate these constraints to boolean
   constraints as given in
   ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/Conditions.mac.
   See "Translating the constraint-system into SAT, CSP, ..." in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp.
   </li>
   <li> Translating DES constraints to Minion:
    <ul>
     <li> We should provide a translation for input into the Minion solver.
     </li>
     <li> S-box constraints should be translated to "table" constraints where
     we specify the truth table for each S-box. </li>
     <li> XOR constraints should be translated to "watchsumleq" and
     "watchsumgeq" constraints. </li>
     <li> We need a function "output_des_minion" which takes the number of
     rounds and uses des_sboxc and des_xor etc to generate the constraints
     associated with the DES given the input parameters, and outputs the
     corresponding Minion input file. </li>
     <li> We also need a function "output_des_minion_6tm" which takes the
     number of rounds, and the parameter m and uses des_sboxc_6tm and
     des_xor_6tm etc to generate the constraints associated with the DES given
     the input parameters, and outputs the corresponding Minion input file.
     See "Sbox boolean 6xm functions". </li>
    </ul>
   </li>
   <li> We should also translate to one of the constraint modelling languages;
   see "Constraint modelling languages" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp. </li>
   <li> See also "Translating the constraint-system into SAT, CSP, ..."
   in ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp.
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

