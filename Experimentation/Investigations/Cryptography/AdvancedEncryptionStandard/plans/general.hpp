// Matthew Gwynne, 27.3.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
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
   for the Maxima-level. </li>
   <li> See
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   for an AES translation into SAT. </li>
   <li> See Applications/Cryptanalysis/plans/Rijndael.hpp for the C++ level.
   </li>
   <li> See Experimentation/Investigations/plans/CurrentExperiments.hpp for
   current experiments (todo "Add section on AES experiments to be run" in
   Experimentation/Investigations/plans/general.hpp needs urgent update). </li>
  </ul>


  \todo Prepare benchmarks for SAT 2011
  <ul>
   <li> See "Benchmarks for SAT 2011" in
   Experimentation/Investigations/plans/SATCompetition.hpp. </li>
  </ul>


  \todo Prepare experiments for the SAT 2011 paper
  <ul>
   <li> Milestones are needed. </li>
   <li> A full update of the existing investigations-reports is needed. </li>
   <li> Systematic variations on the various boxes:
    <ol>
     <li> In order to find out about the effects of box-translations, a good
     method should be to "turn off" all other boxes, i.e., making them the
     identity. </li>
     <li> This could perhaps distinguish between the various occurrences of the
     box under consideration. </li>
     <li> For this to be well-tested, we should have reasonable possibilities
     to create also the corresponding crypto-system, so that we can check
     encryption/decryption. </li>
     <li> The translation system shouldn't have any variables regarding the
     identities created by those identity-boxes. </li>
     <li> Perhaps for version 1.0 of the Maxima module Cryptology we plan to
     have a fully integrated system, which from a very general specification
     creates the cryptosystems as well as the various SAT or CSP translations.
     </li>
    </ol>
   </li>
   <li> First experiment-running script:
    <ol>
     <li> Using three steps: experiment-creation, experiment-running,
     experiment-evaluation. </li>
     <li> An experiment is representing via a directory with a good descriptive
     name, where inside all information is found for reproduction and
     restarting. </li>
     <li> It seems best to have first a script which takes as parameters
     the generalised AES-parameters plus optionally a seed (default=1),
     so that these parameters fully specify a sequence of generalised
     AES problems, and then all combinations of SAT solvers and translations
     are executed. </li>
     <li> The problems are created by computing the key by the Maxima
     random-number generator, encrypting the all-0 plaintext, and then
     taking the problem of guessing the key from given plain- and ciphertext.
     </li>
     <li> The major problem here is running Maxima in parallel:
      <ol>
       <li> See "Improve locality" in
       Buildsystem/MasterScript/SpecialProcessing/plans/Call_Maxima.hpp,
       especially point "No interference of different invocations". </li>
       <li> Until now it was not a major issue, however now running, say, 30
       experiments in parallel won't work out without a solution. </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo Open problems
  <ul>
   <li> Here is a list of experiments which still need to be run, or
   questions for which we still do not know the answer. 
   <ol>
    <li> XXX </li>
   </ol>
   </li>
  </ul>


  \todo Experiments : COMBINE WIT ABOVE
  <ul>
   <li> For the main experimental investigations, see:
    <ul>
     <li> See 
     Investigations/Cryptography/AdvancedEncryptionStandard/plans/AESKeyDiscovery.hpp
     for 
     experiments regarding breaking AES (i.e. finding the key given the
     plaintext and ciphertext. </li>
     <li> See 
     Investigations/Cryptography/AdvancedEncryptionStandard/plans/SmallScaleKeyDiscovery.hpp
     for experiments regarding breaking small scale AES (i.e. finding the key
     given the plaintext and ciphertext. </li>
     <li> See 
     Investigations/Cryptography/AdvancedEncryptionStandard/plans/EncryptionDecryption.hpp 
     for experiments regarding computing encryption and decryption using the
     AES SAT translation. </li>
     <li> See 
     Investigations/Cryptography/AdvancedEncryptionStandard/plans/BoxRepresentations.hpp
     for experiments regarding representations of the AES and small scale
     boxes. </li>
    </ul>
   </li>
   <li> We model a generalised AES system (see 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac)
   which supports the following different translations:
   <ul>
    <li> Mix columns variants with translations using:
     <ul>
      <li> the boxes in the encryption direction </li>
      <li> both encryption and decryption directions. </li>
     </ul>
    </li>
    <li> Box translations for the Sbox and field multiplications using:
     <ul>
      <li> Small CNF representations (see 
      ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp 
      and 
      ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp).
      </li>
      <li> The canonical DNF translation (see 
      ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Constructions.mac). </li>
      <li> r_k-reduced representation (see 
      OKlib/ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac). </li>
      <li> Small hitting clause-set representations (see 
      ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp). 
      </li>
     </ul>
    </li>
    <li> Combining affine components
     <ul>
      <li> We have the standard AES which has the Sbox made up of the
      word (byte) level inversion of block elements, following by the
      application of an affine transformation. </li>
      <li> On the other hand, the affine portion of the Sbox can be moved 
      through the Shiftrows operation such that it can be combined with the 
      MixColumns component, yielding a component which is entirely linear at 
      the byte level, leaving only the non-linear inversion and addition of 
      the affine constant as part of the Sbox operation. </li>
      <li> Splitting the two types of operation like this will hopefully
      yield Sbox translations which are more compact and more amenable to
      SAT solvers as the affine transformation is specifically designed to 
      increase equation size, and make the Sbox harder to model. </li>
     </ul>
    </li>
    <li> Generalised small scale parameters (see 
    ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/SmallScaleAdvancedEncryptionStandard.mac)
     <ul>
      <li> n_R: number of rows in the AES block (default 4, can be
      1, 2 or 4). </li>
      <li> n_C: number of columns in the AES block (default 4, can be
      1, 2 or 4). </li>
      <li> e: size of word field in bits in the AES (default 8, can be
      1, 2 or 4 or 8) - in general we might consider arbitrary fields. </li>
      <li> r: number of rounds (default 10, can be any positive integer).
      </li>
     </ul>
    </li>
   </ul>
   </li>
   <li> DONE (subsumed by newer list)
    The following are the main areas requiring experiments:
    <ul>
     <li> Breaking AES for reduced round variants. </li>
     <li> Investigations into different representations of
     the Sbox. </li>
     <li> Investigations into replacing the Sbox with random permutations.
     </li>
     <li> Investigations into replacing the multiplication with constant field
     elements with random permutations. </li>
     <li> Investigations into encryption and decryption using different AES 
     representation. </li>
     <li> Investigations into small-scale variations of the AES (see 
     "Generalisations" in 
     ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp).
     </li>
    </ul>
   </li>
  </ul>

  
  \todo Solvers to be used for experimentation : NEEDS UPDATE
  <ul>
   <li> We need a list of the solvers we will use for all experiments. </li>
   <li> As time goes on, some solvers might not be used for later experiments
   based on earlier results, and this can be recorded here. </li>
   <li> Solvers to be used: 
    <ul>
     <li> OKsolver_2002 </li>
     <li> minisat2 </li>
     <li> ubcsat (1-0-0) </li>
     <li> picosat913 </li>
     <li> Satz215 </li>
     <li> sp </li>
     <li> march_pl </li>
    </ul>
   </li>
   <li> Any solvers to be used which are not currently in the library, need
   to be added to BuildSystem/ExternalSources/SpecialBuilds/plans/SAT.hpp .
   </li>
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
     Experimentation/Investigations/BooleanFunctions/plans/ReductionBasedRepresentations.hpp.
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


  \todo General organisation
  <ul>
   <li> All the experiments must be fully reproducible. Thus *only* tools
   as provided by the OKlibrary are to be used, *NO* private code or private
   schemes in any form. </li>
   <li> Besides reproducibility, the point is to strengthen the OKlibrary,
   by continuous improvement, not to weaken it by hiding. </li>
   <li> DONE Directory structure:
    <ol>
     <li> DONE
     The main topic here is not investigation of certain solvers, but to
     investigate the interaction of cryptography and (generalised) SAT
     solving. </li>
     <li> DONE Thus solver-names as file-names should only occur, if at all, in
     sub-directories associated with specific investigations. </li>
     <li> The general standards for naming in the OKlibrary have to be
     observed:
      <ul>
       <li> DONE Non-generic filenames start always with a capital letter.
       </li>
       <li> No file- (or directory-)name consists only of capital letters.
       </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  \todo DONE Improve formulations
  <ul>
   <li> The general naming conventions must be followed:
    <ol>
     <li> DONE No camel-cases here (like "removeTopNVars", "AESHexToPA", 
     "NewCL", "mergeSameVarCNFs". </li>
     <li> DONE (renamed relevant functions) 
     Grammatical case indicates verb or noun! </li>
    </ol>
   </li>
   <li> DONE Clean-up minisat output (of course, no intermediate results).
   <ul>
    <li> What is meant by intermediate results here? </li>
    <li> Solver output should be concise, and individual restarts of solvers
    such as minisat should not be included in plans/docus unless there is 
    specifically something interesting to present. </li>
   </ul>
   </li>
   <li> (DONE see for example "Generating experiments" in 
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/BreakingAES.hpp)
   Explanations needed! Experiments need full specifications (to be
   reproducible). </li>
   <li> DONE More care! (Of course, the doxygen-output needs to be inspected) 
   </li>
   <li> (DONE see "Experiments") And discussions! What are the underlying 
   problems to be investigated?
   </li>
   <li> DONE No names consisting only of upper-case letters! See
   "Weak state of the whole module" in
   Experimentation/Investigations/Cryptography/plans/general.hpp. </li>
   <li> DONE No "is useful". </li>
   <li> DONE No "may be used". </li>
  </ul>


  \todo DONE More experiments
  <ul>
   <li> DONE What is the meaning of
   "Experiments on AES with the first 32 key bits not set" ? And what does
   the "summary" show? Apparently only trivial results, which are likely of
   no interest. 
   <ul>
    <li> This was regarding initial investigations where all plaintext and 
    ciphertext variables were set to relevant values and the last 96 key 
    variables were set. </li>
    <li> 32 bits were chosen as the AES translation to CNF, using the small CNF
    representations, presented a challenge to solvers for even very small 
    numbers of key bits left "unknown"/"unset" in the SAT problem. </li>
    <li> The experiments were over rounds 2,4,6,8, and 10. </li>
    <li> Experimental data and results has now been moved. See "Experiments". 
    </li>
   </ul>
   </li>
   <li> DONE For the trivial instances (message and key set) more solvers need
   to be considered. In its own file. </li>
   <li> DONE Also ciphertext and key set are to be investigated. </li>
   <li> (MOVED to Experiments) 
   Important is that we systematically study variations of AES:
    <ol>
     <li> the different sizes as belonging to Rijndael </li>
     <li> small-scale variations </li>
     <li> replacing the Sbox by other permutations (random ones, or
     very simple ones) </li>
     <li> replacing the multiplications with certain constants by other
     permutations </li>
     <li> replacing the row- and column-permutations by other permutations.
     </li>
    </ol>
   </li>
   <li> DONE Plan for more experiments! </li>
  </ul>

*/
