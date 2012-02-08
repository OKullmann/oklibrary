// Matthew Gwynne, 2.1.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp
  \brief AES experiments related to the SAT 2012 paper

  We consider experiments investigating the AES and small-scale translations
  to CNF, as well as the translations of the associated AES boxes such as the
  Sbox, field multiplication, field inversion and so on.

  For information on the scope of the AES and small scale translations, see
  "Investigation dimensions" below. For an idea of the open problems, please
  see "Open problems".


  \todo Links
  <ul>
   <li> See
   Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/general.hpp
   for experiments regarding breaking AES (i.e. finding the key given the
   plaintext and ciphertext). </li>
   <li> See
   AdvancedEncryptionStandard/plans/EncryptionDecryption.hpp
   for experiments regarding computing encryption and decryption using the
   AES SAT translation. </li>
   <li> See
   AdvancedEncryptionStandard/plans/Representations/general.hpp
   for experiments regarding representations of the AES and small scale boxes.
   </li>
  </ul>


  \todo Investigating dimensions
  <ul>
   <li> The translation is available at
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac.
   </li>
   <li> The translation system (irrespective of the AES) has three conceptual
   levels:
    <ul>
     <li> <em>Variant</em>: which AES function we consider (small-scale
     parameters etc). This level defines the whole boolean function we
     consider. </li>
     <li> <em>Decomposition</em>: how to decompose the large boolean
     function into smaller functions. </li>
     <li> <em>Representation</em>: how to represent the small boolean
     functions. </li>
    </ul>
   </li>
   <li> Discussion of each of the possible dimensions of the translations
   should become a separate todo. </li>
   <li> A separate todo is needed on the decomposition of the AES. </li>
   <li> Further discussion is needed on the decompositions of the key
   schedule. </li>
   <li> Variants (AES parameters):
    <ul>
     <li> Number of rounds (10 options): 1,...,10. </li>
     <li> Size of field (8 options) : 1,...,8. </li>
     <li> Number of columns in AES block (4 options): 1,...,4. </li>
     <li> Number of rows in AES block (4 options): 1,...,4. </li>
     <li> Inclusion of special final round or not (2 options):
      <ol>
       <li> Final round is the same as every other (in line with small-scale).
       </li>
       <li> Final round doesn't include the MixColumns operation. </li>
      </ol>
     </li>
     <li> Box replacements (3 options):
      <ol>
       <li> Original Sbox, field multiplication etc. are used. </li>
       <li> Random permutations are used. </li>
       <li> The identity is used. </li>
       <li> See "Explain how to replace various AES boxes with identity or
       random" in AdvancedEncryptionStandard/plans/general.hpp; this
       aspects needs better explanations. </li>
      </ol>
     </li>
    </ul>
   </li>
   <li> Decomposition:
    <ul>
     <li> We consider the AES function as:
      <ul>
       <li> Rounds, which have:
        <ul>
         <li> Subbytes operation which is the Sbox applied to each consecutive
         8-bits of the input. </li>
         <li> MixColumn operation applied to each column. This operation is
         a matrix multiplication applied to the vector of 8-bit words in
         the column. </li>
        </ul>
       </li>
       <li> Key schedule, which has:
        <ul>
         <li> The Sbox applied to each 8-bit word in the first column
         of the previous round key. </li>
         <li> Additions of the Sbox result, a round constant and certain
         8-bit words from the previous round key. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> So far, the decompositions we consider revolve around the
     Sboxes and MixColumn operation. </li>
     <li> Sbox decompositions:
      <ul>
       <li> Movement of the Sbox linear map into MixColumns (2 options):
        <ol>
         <li> Sbox linear map is left in the Sbox. </li>
         <li> Sbox linear map is moved through the ShiftRows operation.
         It is then combined with the MixColumn multiplication components.
         </li>
        </ol>
       </li>
       <li> Sbox affine constant (2 options):
        <ol>
         <li> Sbox affine constant addition remains in Sbox. </li>
         <li> Sbox affine constant addition moved through ShiftRows and
         MixColumns. It is then added to the end of the round. </li>
        </ol>
       </li>
      </ul>
     </li>
     <li> MixColumn decomposition:
      <ul>
       <li> MixColumns direction (3 options):
        <ol>
         <li> Translation of the MixColumn component using both the encryption
         and decryption components. </li>
         <li> Translation using only the encryption direction. </li>
         <li> Translation using only the decryption direction. </li>
        </ol>
       </li>
       <li> Function decomposition (3 options):
        <ol>
         <li> Multiplication function decomposition (default):
          <ul>
           <li> Representing the MixColumns using individual multiplication
           functions as well as addition constraints. </li>
          </ul>
         </li>
         <li> Bit-matrix representation:
          <ul>
           <li> Representing the MixColumns as a bit-matrix multiplication.
           </li>
           <li> Representing the multiplication of each row in the bit
           matrix using addition constraints. </li>
          </ul>
         </li>
         <li> Whole function representation:
          <ul>
           <li> Taking the MixColumn as a single boolean function. </li>
          </ul>
         </li>
        </ol>
       </li>
      </ul>
     </li>
     <li> Key schedule decomposition (2 option):
      <ul>
       <li> Shared sub-expression translation (default):
        <ul>
         <li> Translates the Sbox operations as individual functions. </li>
         <li> The first column of a new round key is computed by
         K_1 + Sbox(K_p') + C. K_p is a column of certain key words from the
         previous round key. K_1 is the first column of the previous
         round key and C is a constant. </li>
         <li> Column i+1 in the new round key is computed by K_(i-1) + K_p''.
         K_(i-1) is the previous column of the new round key. K_pi is i-th
         column from the previous round key. </li>
        </ul>
       </li>
       <li> Larger arity addition computation (not implemented):
        <ul>
         <li> Translates the Sbox operations as individual functions. </li>
         <li> Column i of the new round key is computed by
         SBox(K_p) + C + sum(K_1,...,K_i). K_i is the i-th column of the
         previous round key. K_p is a column of certain key words from the
         previous round key. </li>
         <li> The point here is that later columns of the *new* round key are
         defined using earlier columns of the *new* round key. </li>
         <li> In this translation, instead of using earlier columns of the new
         round key, we expand the computation out. Each new round key is
         specified using only columns from the previous round key (no reusing
         results). </li>
        </ul>
       </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> Representation:
    <ul>
     <li> Box representation (4 options):
      <ol>
       <li> Canonical translation. </li>
       <li> Canonical+ translation. </li>
       <li> 0-based representation. </li>
       <li> 1-based representation. </li>
       <li> Minimum representation (i.e., |var(F)|-based representation).
       </li>
      </ol>
     </li>
    </ul>
   </li>
   <li> This todo needs to be moved to docus in
   Cryptology/Lisp/Cryptanalysis/Rijndael/docus/. </li>
   <li> Note that addition constraints are always represented using their
   prime implicates. </li>
   <li> In each case, the first option is considered the default. </li>
   <li> We need instructions on how to generate each translation. These
   should occur here (in this plans file). </li>
   <li> See "Separate key-schedule and block-cipher" in
   AdvancedEncryptionStandard/plans/general.hpp; this aspects needs
   better explanations. </li>
   <li> For more information, see
   Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp. </li>
  </ul>


  \todo Open problems
  <ul>
   <li> For a description of the investigation, please see
   Cryptography/AdvancedEncryptionStandard/plans/general.hpp. </li>
   <li> Here is a list of experiments which still need to be run, or
   questions for which we still do not know the answer. </li>
   <li> We need to find out which solvers and local search algorithms
   perform best on different minimisation problems, see
   AdvancedEncryptionStandard/plans/Representations/general.hpp.
   </li>
   <li> We need basic data on the prime implicates and subsumption
   hypergraphs for the following:
    <ul>
     <li> Small scale S-box (see
     AdvancedEncryptionStandard/plans/Representations/Sbox_4.hpp).
     </li>
     <li> Small scale field multiplications (see
     AdvancedEncryptionStandard/plans/Representations/Mul_e_4.hpp).
     </li>
     <li> Small scale field inversion (no plans file yet). </li>
     <li> Small scale linear map (no plans file yet). </li>
     <li> Small scale linear map with field multiplications (no plans file
     yet). </li>
     <li> AES field multiplications:
      <ul>
       <li> We consider 256 multiplications: {00,...,255}. </li>
       <li> 00, 01, 02, 03, 09, 11, 13, 14 : DONE. </li>
       <li> The other 247. </li>
      </ul>
     </li>
     <li> AES Sbox linear map with field multiplications (no plans file yet).
     </li>
     <li> AES Sbox (see
     AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp)
     : DONE. </li>
     <li> AES field inversion (see
     AdvancedEncryptionStandard/plans/Representations/Inv_8.hpp)
     : DONE. </li>
     <li> AES Sbox linear map (see
     AdvancedEncryptionStandard/plans/Representations/LinearMap_8.hpp)
     : DONE. </li>
    </ul>
   </li>
   <li> We need to find minimum CNF representations for the following
   functions:
    <ul>
     <li> AES Sbox; no minimum yet, smallest 294 (see
     AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp).
     </li>
     <li> AES field inversion (see
     AdvancedEncryptionStandard/plans/Representations/Inv_8.hpp).
     </li>
     <li> AES Sbox linear map (see
     AdvancedEncryptionStandard/plans/Representations/LinearMap_8.hpp).
     </li>
     <li> AES Sbox linear map with field multiplications (no plans file yet).
     </li>
     <li> Small scale field inversion (no plans file yet). </li>
     <li> Small scale linear map (no plans file yet). </li>
     <li> Small scale linear map with field multiplications (no plans file
     yet). </li>
     <li> AES field multiplications (255):
      <ul>
       <li> The remaining 251. </li>
       <li> 03, see
       AdvancedEncryptionStandard/plans/Representations/Mul_3_8.hpp.
       </li>
       <li> 02, minimum size 20 (see
       AdvancedEncryptionStandard/plans/Representations/Mul_2_8.hpp) :
       DONE. </li>
       <li> 00, 01 : DONE (trivial) </li>
      </ul>
     </li>
     <li> Small scale field multiplications (see
     AdvancedEncryptionStandard/plans/Representations/Mul_e_4.hpp) :
     DONE. </li>
     <li> Small scale Sbox; minimum 22 (see
     AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp)
     : DONE. </li>
    </ul>
   </li>
   <li> Investigations into the prime implicates, subsumption hypergraphs
   and minimum representations of:
    <ul>
     <li> Random permutations (see
     "First considerations of random permutation" in
     Experimentation/Investigations/BooleanFunctions/plans/Permutations.hpp).
     </li>
     <li> Random linear maps (see
     "Affine bijections over ZZ_2" in
     Experimentation/Investigations/BooleanFunctions/plans/Permutations.hpp).
     </li>
     <li> Random boolean functions (see
     Experimentation/Investigations/BooleanFunctions/plans/general.hpp). </li>
    </ul>
   </li>
  </ul>


  \todo Update experiment script
  <ul>
   <li> First experiment-running script:
    <ol>
     <li> Using three steps: experiment-creation, experiment-running,
     experiment-evaluation. </li>
     <li> An experiment is representing via a directory with a good descriptive
     name, where inside all information is found for reproduction and
     restarting. </li>
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
       <li> For what is Maxima needed?
        <ol>
         <li> Creation of the template-SAT-problems should be possible in
         advance. </li>
         <li> If a key is computed, we should check whether this key is
         correct. </li>
         <li> At least for the small-scale system the could be done using Sage;
         hopefully Sage is more amenable to script-usage. </li>
        </ol>
       </li>
      </ol>
     </li>
     <li> DONE It seems best to have first a script which takes as parameters
     the generalised AES-parameters plus optionally a seed (default=1),
     so that these parameters fully specify a sequence of generalised
     AES problems, and then all combinations of SAT solvers and translations
     are executed. </li>
    </ol>
   </li>
   <li> Experiment creation:
    <ul>
     <li> See Cryptography/AdvancedEncryptionStandard/generate_aes_experiment.
     </li>
     <li> The current experiment creation script allows one to specify the
     number of rounds, number of random plaintext-ciphertext-pairs to generate
     and the parameters specifying the AES instance and translation (see
     output_ss_fcl_std in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac).
     </li>
     <li> This generator generates several AES instances (one for each round)
     at once. </li>
     <li> The generator should be split up into two scripts, one generating
     a single AES instance, another generating random
     plaintext-ciphertext-pairs. </li>
     <li> The script should output the parameters for the computation to
     a file "instance_parameters" in the experiment directory; the directory
     might be renamed. </li>
     <li> The script badly needs documentation. </li>
    </ul>
   </li>
   <li> Experiment running:
    <ul>
     <li> See Cryptography/AdvancedEncryptionStandard/run_aes_experiment.
     </li>
     <li> The AES experiment script should be split up into per-solver scripts:
      <ul>
       <li> We need a "run_aes_experiment_oksolver",
       "run_aes_experiment_minisat-2.2.0" and so on. </li>
       <li> Each solver-specific experiment script can then handle outputting
       solver output to the correct directory. </li>
       <li> Each script should examine "instance_parameters" to determine how
       many AES rounds and number of plaintext-ciphertext-pairs to run the
       experiment on. </li>
       <li> The script should then create the AES key discovery
       instances from the AES instances and plaintext-ciphertext-pairs. </li>
       <li> Finally it should run the selected solver on the instance
       and record the output into a file "r${r}_s${i}.cnf" where
        <ul>
         <li> r is the number of rounds for the AES key discovery instance
         the solver is being run on; </li>
         <li> s is the seed for the plaintext-ciphertext-pair used to
         instantiate the AES key discovery instance. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> Documentation is badly needed for the script. </li>
    </ul>
   </li>
   <li> Experiment monitoring:
    <ul>
     <li> See "Extraction tools" in
     ExperimentSystem/SolverMonitoring/plans/general.hpp. </li>
     <li> Scripts should be made to extract key data from each solver. </li>
    </ul>
   </li>
   <li> All directories created should have the datetime included in the
   name. </li>
   <li> Each script should have the option to continue generating, running or
   monitoring from where it left off. This should be a case of rerunning the
   last experiment, and perhaps outputting a special file to indicate the
   experiment has been finished. </li>
   <li> This todo should be split into todos for scripts for generating,
   running and monitoring. </li>
   <li> For a list of other experiment scripts see
   "Experiment scripts" in Experimentation/Investigations/plans/general.hpp. </li>
   <li> See "Update scripts" and "Just an unstructured morass of scripts" in
   Cryptography/AdvancedEncryptionStandard/plans/general.hpp.
   </li>
  </ul>


  \todo Move experiment data to investigation-reports
  <ul>
   <li> All data, especially that generated for LCC and CP 2011 must be
   transferred to Annotations. </li>
   <li> For LCC and CP there must be docus page with links to the relevant
   experiment plans, and summaries of the results. </li>
   <li> Consolidation is needed of summary todos such as "Boundaries" in
   Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/general.hpp;
   we must be able to *see* results. </li>
  </ul>


  \todo Solvers to be used for experimentation
  <ul>
   <li> We need a list of the solvers we will use for all experiments. </li>
   <li> As time goes on, some solvers might not be used for later experiments
   based on earlier results, and this can be recorded here. </li>
   <li> Solvers to be used:
    <ul>
     <li> ArgoSAT </li>
     <li> OKsolver_2002 </li>
     <li> minisat-2.2.0 </li>
     <li> glucose </li>
     <li> ubcsat (1-2-0) </li>
     <li> picosat913 </li>
     <li> precosat236 </li>
     <li> precosat570 </li>
     <li> (p)lingeling </li>
     <li> Satz215 </li>
     <li> Grasp </li>
     <li> sp </li>
     <li> march_pl </li>
    </ul>
   </li>
   <li> Is it possible in the time constraints to run ALL of these solvers
   for every experiment? </li>
   <li> Any solvers to be used which are not currently in the library, need
   to be added to ExternalSources (see also "Add todos for SAT-Race 2010 SAT
   solvers" in BuildSystem/ExternalSources/SpecialBuilds/plans/SAT.hpp).
   </li>
   <li> This data needs to be moved to docus. </li>
   <li> DONE (moved to Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp)
   The following solvers (from the SAT 2010 race) are not in the library,
   but should be added to the library (see
   BuildSystem/ExternalSources/plans/SAT.hpp):
   <ul>
    <li> Barcelogic </li>
    <li> borg-sat </li>
    <li> CircleSAT </li>
    <li> CirCUs </li>
    <li> CryptoMiniSAT </li>
    <li> glucose </li>
    <li> glucoER </li>
    <li> kw </li>
    <li> lingeling </li>
    <li> LySAT </li>
    <li> oprailleur </li>
    <li> riss </li>
    <li> rcl </li>
    <li> SApperloT </li>
    <li> SAT-Power </li>
    <li> SATHYS </li>
   </ul>
   </li>
  </ul>


  \todo DONE (individual issues moved to separate todos)
  Prepare experiments for the SAT 2012 paper
  <ul>
   <li> DONE (moved to "Move experiment data to investigation-reports")
   A full update of the existing investigations-reports is needed. </li>
   <li> DONE (moved to "Update experiment script")
   First experiment-running script:
    <ol>
     <li> Using three steps: experiment-creation, experiment-running,
     experiment-evaluation. </li>
     <li> An experiment is representing via a directory with a good descriptive
     name, where inside all information is found for reproduction and
     restarting. </li>
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
       <li> For what is Maxima needed?
        <ol>
         <li> Creation of the template-SAT-problems should be possible in
         advance. </li>
         <li> If a key is computed, we should check whether this key is
         correct. </li>
         <li> At least for the small-scale system the could be done using Sage;
         hopefully Sage is more amenable to script-usage. </li>
        </ol>
       </li>
      </ol>
     </li>
     <li> DONE It seems best to have first a script which takes as parameters
     the generalised AES-parameters plus optionally a seed (default=1),
     so that these parameters fully specify a sequence of generalised
     AES problems, and then all combinations of SAT solvers and translations
     are executed. </li>
    </ol>
   </li>
   <li> DONE (moved to "Explain how to replace various AES boxes with identity
   or random boxes" in
   /AdvancedEncryptionStandard/plans/Experimentation.hpp)
   Systematic variations on the various boxes:
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
   <li> DONE Milestones are needed. </li>
  </ul>

*/
