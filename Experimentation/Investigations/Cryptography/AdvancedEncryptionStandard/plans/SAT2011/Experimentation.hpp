// Matthew Gwynne, 2.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp
  \brief AES experiments related to the SAT 2011 paper

  We consider experiments investigating the AES and small scale translations 
  to CNF, as well as the translations of the associated AES boxes such as the
  Sbox, field multiplication, field inversion and so on.

  For information on the scope of the AES and small scale translations, see
  "Investigation dimensions" below. For an idea of the open problems, please
  see "Open problems".


  \todo Links
  <ul>
   <li> See 
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/general.hpp
   for experiments regarding breaking AES (i.e. finding the key given the
   plaintext and ciphertext). </li>
   <li> See 
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/EncryptionDecryption.hpp 
   for experiments regarding computing encryption and decryption using the
   AES SAT translation. </li>
   <li> See 
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/general.hpp
   for experiments regarding representations of the AES and small scale boxes.
   </li>
  </ul>


  \todo Investigation dimensions
  <ul>
   <li> Using the translation scheme offered at 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac, we
   consider the following set of translations for each AES variant.
   Each of the following constitutes an independent dimension to the 
   translation, and given a fixed variant, are different translations of the
   same function:
   <ul>
    <li> MixColumns direction (3 options):
    <ol>
     <li> Translation of the MixColumn component using both the encryption and
     decryption components. </li>
     <li> Translation using only the encryption direction. </li>
     <li> Translation using only the decryption direction. </li>
    </ol>
    </li>
    <li> Movement of the Sbox linear map into MixColumns (2 options):
    <ol>
     <li> Sbox linear map is left in the Sbox. </li>
     <li> Sbox linear map is moved through the ShiftRows operation and 
     combined with the MixColumn multiplication components. </li>
    </ol>
    </li>
    <li> Sbox affine constant (2 options):
    <ol>
     <li> Sbox affine constant addition remains in Sbox. </li>
     <li> Sbox affine constant addition moved through ShiftRows and MixColumns
     and added to the end of the round. </li>
    </ol>
    </li>
    <li> Box representation (4 options):
    <ol>
     <li> Canonical translation. </li>
     <li> Prime representation. </li>
     <li> Minimum representation. </li>
     <li> r_1-based representation. </li>    
    </ol>
    </li>
   </ul>
   </li>
   <li> We then have the following generalised AES parameters and
   possibilities, describing different variants of the standard
   AES:
   <ul>
    <li> Number of rounds (10+ options): 1 - 10. </li>
    <li> Size of field (8+ options) : 1-8. </li>
    <li> Number of columns in AES block (4+ options): 1-4. </li>
    <li> Number of rows in AES block (4+ options): 1-4. </li>
    <li> Inclusion of special final round or not (2 options):
    <ol>
     <li> Final round is the same as every other (in line with small scale). 
     </li>
     <li> Final round doesn't include the MixColumns operation. </li>
    </ol>
    </li>
    <li> Box replacements (3 options):
    <ol>
     <li> Original Sbox, field multiplication etc are used. </li>
     <li> Random permutations are used. </li>
     <li> The identity is used. </li>
    </ol>
    </li>
   </ul>
   </li>
   <li> In each case, the first option is considered the default. </li>
   <li> We need instructions on how to generate each translation. These
   should occur here (in this plans file). </li>
   <li> For more information, see 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp .
   </li>
  </ul>


  \todo Open problems
  <ul>
   <li> For a description of the investigation, please see
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/general.hpp .
   </li>
   <li> Here is a list of experiments which still need to be run, or
   questions for which we still do not know the answer. 
   <ul>
    <li> We need to find out which solvers and local search algorithms
    perform best on different minimisation problems, see 
    Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/general.hpp.
    </li>
    <li> We need basic data on the prime implicates and subsumption 
    hypergraphs for the following:
    <ul>
     <li> Small scale Sbox (no plans file yet). </li>
     <li> Small scale field multiplications (no plans file yet). </li>
     <li> Small scale field inversion (no plans file yet). </li>
     <li> Small scale linear map (no plans file yet). </li>
     <li> Small scale linear map with field multiplications (no plans file 
     yet). </li>
     <li> 
     <li> AES field multiplications (255):
     <ul>
      <li> 00, 01, 02, 03, 09, 11, 13, 14 : DONE. </li>
      <li> The other 247. </li>
     </ul>
     </li>
     <li> AES Sbox linear map with field multiplications (no plans file yet). 
     </li>
     <li> AES Sbox (see 
     Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp) : DONE. </li>
     <li> AES field inversion (see 
     Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Inv_8.hpp) : DONE. </li>
     <li> AES Sbox linear map (see 
     Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/LinearMap_8.hpp) : DONE. </li>
    </ul>
    </li>
    <li> We need to find minimum CNF representations for the following
    functions:
    <ul>
     <li> AES Sbox; no minimum yet, smallest 294 (see 
     Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp). </li>
     <li> AES field inversion (see 
     Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Inv_8.hpp). </li>
     <li> AES Sbox linear map (see 
     Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/LinearMap_8.hpp). </li>
     <li> AES Sbox linear map with field multiplications (no plans file yet). 
     </li>
     <li> Small scale Sbox (no plans file yet). </li>
     <li> Small scale field multiplications (no plans file yet). </li>
     <li> Small scale field inversion (no plans file yet). </li>
     <li> Small scale linear map (no plans file yet). </li>
     <li> Small scale linear map with field multiplications (no plans file 
     yet). </li>
     <li> 
     <li> AES field multiplications (255):
     <ul>
      <li> The remaining 251. </li>
      <li> 03, see 
      Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_3_8.hpp. </li>
      <li> 02, minimum size 20 (see 
      Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_2_8.hpp) : 
      DONE. </li>
      <li> 00, 01 : DONE (trivial) </li>
     </ul>
     </li>
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
     Experimentation/Investigations/BooleanFunctions/plans/Permutations.hpp). </li>
     <li> Random boolean functions (see 
     Experimentation/Investigations/BooleanFunctions/plans/general.hpp). </li>
    </ul>
    </li>
   </ul>
   </li>
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
    </ol>
   </li>
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
     <li> minisat2 </li>
     <li> ubcsat (1-0-0) </li>
     <li> picosat913 </li>
     <li> precosat236 </li>
     <li> precosat570 </li>
     <li> Satz215 </li>
     <li> Grasp </li>
     <li> sp </li>
     <li> march_pl </li>
    </ul>
   </li>
   <li> The following solvers (from the SAT 2010 race) are not in the library,
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
   <li> Is it possible in the time constraints to run ALL of these solvers
   for every experiment? </li>
   <li> Any solvers to be used which are not currently in the library, need
   to be added to ExternalSources (see also "Add todos for SAT-Race 2010 SAT 
   solvers" in BuildSystem/ExternalSources/SpecialBuilds/plans/SAT.hpp) .
   </li>
  </ul>

*/
