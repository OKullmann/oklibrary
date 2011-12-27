// Matthew Gwynne, 27.3.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp
  \brief On investigations into the Advanced Encryption Standard


  \todo Connections
  <ul>
   <li> See
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   for a AES translation into SAT at the Maxima level. </li>
   <li> See Applications/Cryptanalysis/plans/Rijndael.hpp for the C++ level.
   </li>
   <li> See Experimentation/Investigations/plans/CurrentExperiments.hpp for
   current experiments. </li>
   <li> "Add section on AES experiments to be run" in
   Experimentation/Investigations/plans/general.hpp needs urgent update.
   </li>
   <li> See "SAT 2012". </li>
  </ul>


  \todo Links
  <ul>
   <li> For experiments and open problems, see
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp .
   This includes a overview on the scope of the experiments in "Dimensions".
   </li>
   <li> For investigations into the various component/box (Sbox,
   field multiplication etc) representations (CNF and otherwise) see
   Cryptography/AdvancedEncryptionStandard/plans/Representations/general.hpp .
   </li>
   <li> For investigations on using the AES translations for key discovery,
   see
   Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/general.hpp .
   </li>
   <li> For investigating encryption and decryption using our CNF translation
   scheme, see
   Cryptography/AdvancedEncryptionStandard/plans/EncryptionDecryption.hpp .
   </li>
  </ul>


  \todo DONE (removed directory and integrated milestones with higher level)
  Merge SAT2011 plans with module one level higher
  <ul>
   <li> The experiments in the SAT2011 module are not SAT2011 specific. </li>
   <li> The SAT2011 module should be removed and/or how to present or link to
   paper/presentation specific experiments reconsidered. </li>
  </ul>


  \todo Notions for AES operation
  <ul>
   <li> When considering the AES round, we don't consider the
   ShiftRows operation as a separate operation, as it rewires
   bits of the AES block, and nothing else. As it is simply
   a renaming, it isn't represented by any clauses in the
   translation. </li>
   <li> To remove the ShiftRows as a separate operation, we
   can move it into the MixColumns operation. </li>
   <li> We need the correct notion and name for this
   operation, and at the same time we should consider the
   notions of the AES round and key schedule. </li>
   <li> Possible notions for the combined MixColumns and ShiftRows
   operation:
   <ul>
    <li> "linear diffusion operation" is a reasonable name which
    brings attention to the following relevant facts:
    <ul>
     <li> The reason for the inclusion of MixColumns and ShiftRows in the AES
     is for their "diffusion" properties and linearity, and both operations
     share these properties while the SubBytes is non-linear.
     </li>
     <li> In [Algebraic Aspects of the Advanced Encryption Standard;Cid,
     Murphy and Robshaw], they call these operations, together, the "diffusion
     layer". </li>
     <li> The two operations in the core of the AES round (i.e., excluding the
     key addition) are made up of a non-linear component and a linear
     component. </li>
    </ul>
    </li>
   </ul>
   </li>
  </ul>


  \todo Determining a "good" local search algorithm to use on AES instances
  <ul>
   <li> Question: what is a good algorithm to use, considering the algorithms
   tested by "run_ubcsat" (see
   Experimentation/ExperimentSystem/ControllingLocalSearch/Evaluation.R) </li>
   <li> Presumably local search will not do well on AES instances compared to
   DPLL/CDCL solvers, as often local search solvers perform
   poorly on industrial and crafted instances, which the AES translations
   are likely similar to, considering the large number of new variables
   involved. </li>
   <li> What sort of metrics to use to determine a good algorithm once the
   experiment has been run?
   <ul>
    <li> Sorting first by the average number of falsified clauses and then
    by the number of steps seems reasonable, as we wish to minimise the number
    of falsified clauses as much as possible. </li>
   </ul>
   </li>
  </ul>


  \todo Explain how to replace various AES boxes with identity or random
  boxes
  <ul>
   <li> As part of our investigations, we make various parts of
   the AES sbox the identity, and then introduce the various boxes (Sbox,
   field multiplications etc), to determine which combinations of boxes
   "make AES difficult". </li>
   <li> To do this, we need to be able to generate AES translations which
   "make sense" (i.e. are permutations, given the key). </li>
   <li> These translations are possible with the current translation system
   without writing additional rewrite functions, however instructions
   and/or additional helper functions are necessary to make sure things
   easy to experiment with. </li>
  </ul>


  \todo Separate key-schedule and block-cipher
  <ul>
   <li> Using the notions from
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/IteratedBlockCipher.mac,
   we should split off the key schedule. </li>
   <li> That is, we consider two variations, one where the
   round-composition ignores the round-keys, and so the key-schedule can
   just be removed, and one where the message round-function is the identity.
   </li>
   <li> However, cryptographically, removing the key schedule makes
   cracking AES trivial. One just applies the inverted round a number of
   times and then the key is just the plaintext XORed with the ciphertext.
   </li>
   <li> Therefore, we should also consider a key schedule where the round key
   for every round is the input key. </li>
   <li> "Compositional iterated block ciphers with iterated key schedule" in
   the sense of
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/IteratedBlockCipher.mac
   are compositions of two crypto-systems, the pure iteration of block ciphers
   and the pure addition of round-keys in every round; this likely should also
   be studied in separation. </li>
  </ul>


  \todo Using SBSAT
  <ul>
   <li> As discussed in the manual
   http://gauss.ececs.uc.edu/sbsat_user_manual/, the input can consist of a
   list of files containing DNFs ir XOR constraints, and so we can easily
   specify AES and generalisations. </li>
   <li> The MixColumns constraint can either be represented via the xor of
   boxes, or by the direct representation via 32 xor-constraints (for
   standard AES). </li>
   <li> We need to get the respective file-formats and the translations into
   our system (while we can't get the solver into our system, since it is not
   open-source). </li>
   <li> Most basic is to get the sizes of smurfs for the various boolean
   functions ("boxes"); see "Smurfs" in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/BDDs.hpp for the
   general theory. </li>
  </ul>


  \todo Investigating conditions and their representations
  <ul>
   <li> A first central research question is how to choose appropriate
   representations of the conditions which altogether specify AES. </li>
   <li> More specifically, clause-sets representing the S-box and the
   various multiplications (with constants) are to be investigated upon
   their influence on SAT solving. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Preprocessing/plans/general.hpp
   for the general perspective, where one starts from a CNF and derives
   DNFs from it by partitioning, while we have already given the DNFs (but
   not some original CNF). </li>
   <li> Studying the representations of the Sbox
    <ol>
     <li> See
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp.
     </li>
     <li> That module is not properly maintained: Actions for MG. </li>
     <li> See "r-based representations" in
     Investigations/BooleanFunctions/plans/ReductionBasedRepresentations.hpp.
     </li>
     <li> A 0-based CNF-representation without additional variables is likely
     infeasible to use. </li>
     <li> Via dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac
     we obtain a (totally) properly r_1-based representation of every box
     (see "Understanding dualts_fcl" in
     Lisp/FiniteFunctions/plans/TseitinTranslation.hpp). </li>
     <li> It seems most promising (for now) to consider here r=r_1 and r=r_2,
     applied to the sets of prime implicates of the various "boxes" involved.
     </li>
     <li> Perhaps actually the hitting-CNFs representing the Sbox might have
     values, since we have special algorithms. So other reductions than just
     the r_k-reductions are useful to consider in general. </li>
    </ol>
   </li>
  </ul>


  \todo Summary of previous experimental results
  <ul>
   <li> Before various improvements were made to the AES translation,
   experiments were run for 1-4 rounds translations of the AES
   and on the various boxes. </li>
   <li> A basic summary (even if not reproducible due to the new state of the
   system), should be made available here. </li>
  </ul>


  \bug DONE (corrected kernel bug described in
  "Rerun time-sensitive experiments" of
  Experimentation/Investigations/plans/general.hpp)
  MiniSAT2 based solvers return incorrect times using experiment script
  <ul>
   <li> minisat2 based solvers (glucose and minisat2) return run times of
   a fraction of a second using when run using "run_all_solvers_2by2"
   while they clearly take much much longer when run separately. </li>
   <li> See "Using the canonical translation" in
   AdvancedEncryptionStandard/plans/KeyDiscovery/032/4_2_4/1_13.hpp.
   </li>
   <li> Just use "time"! </li>
  </ul>


  \todo DONE Add milestones
  <ul>
   <li> We urgently need milestones at this level. </li>
  </ul>


  \todo DONE (misplaced scripts moved; todos added for improvement of others)
  Just an unstructured morass of scripts
  <ul>
   <li> DONE (See "Update experiment script" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp
   and "Experiment scripts" in
   Experimentation/Investigations/plans/general.hpp)
   They do not link to the rest of the library. </li>
   <li> DONE (See "Update experiment script" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp)
   They are just random attempts. Obviously a systematic approach is
   needed, collecting all methods currently in use in the *whole* library.
   </li>
   <li> DONE (see "Improve minimisation scripts" in
   Satisfiability/Optimisation/plans/general.hpp)
   Regarding the computation of minimum CNF representations, the scripts
   here are misplaced. </li>
  </ul>


  \todo DONE (moved to "Improve minimisation scripts" in
  Satisfiability/Optimisation/plans/general.hpp)
  Update scripts
  <ul>
   <li> The following scripts:
    <ul>
     <li> Satisfiability/Optimisation/minimise_cnf_oksolver. </li>
     <li> Satisfiability/Optimisation/cardinality_bounded_cnf. </li>
     <li> Satisfiability/Optimisation/extend_strict_dimacs_with_extended. </li>
     <li> Satisfiability/Optimisation/all_minimum_cnf. </li>
    </ul>
    need the following changes:
    <ul>
     <li> Tests (URGENT). </li>
     <li> Error checking code. </li>
     <li> Merging with other scripts in other investigations into general tools.
     </li>
     <li> Update documentation in comments. </li>
     <li> Add version numbers. DONE </li>
    </ul>
   </li>
  </ul>


  \todo DONE Replace "merge_cnf.sh" with "AppendDimacs"
  <ul>
   <li> All instances of the "merge_cnf.sh" script at
   Investigations/Cryptography/AdvancedEncryptionStandard/merge_cnf.sh
   should be replaced with "AppendDimacs-O3-DNDEBUG" as
   "AppendDimacs-O3-DNDEBUG" performs the same operation,
   is correct and has tests. </li>
  </ul>

*/
