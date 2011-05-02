// Matthew Gwynne, 2.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp
  \brief AES experiments related to the SAT 2011 paper

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
   AdvancedEncryptionStandard/plans/SAT2011/EncryptionDecryption.hpp
   for experiments regarding computing encryption and decryption using the
   AES SAT translation. </li>
   <li> See
   AdvancedEncryptionStandard/plans/SAT2011/Representations/general.hpp
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
     <li> <em>Variant</em> - which AES function we consider (small-scale
     parameters etc). This level defines the whole boolean function we
     consider. </li>
     <li> <em>Decomposition</em> - how to decompose the large boolean
     function into smaller functions. </li>
     <li> <em>Representation</em> - how to represent the small boolean
     functions. </li>
    </ul>
   </li>
   <li> The names of these levels, and the idea needs discussion. </li>
   <li> Discussion of each of the possible dimensions of the translations
   should become a separate todo. </li>
   <li> A separate todo is needed on the decomposition of the AES. </li>
   <li> Further discussion is needed on the decompositions of the key
   schedule. </li>
   <li> Variants (AES parameters):
    <ul>
     <li> Number of rounds (10+ options): 1 - 10. ??? what is "10+" ??? </li>
     <li> Size of field (8+ options) : 1-8. </li>
     <li> Number of columns in AES block (4+ options): 1-4. </li>
     <li> Number of rows in AES block (4+ options): 1-4. </li>
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
       random" in AdvancedEncryptionStandard/plans/SAT2011/general.hpp; this
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
       <li> "By definition" computation (not implemented): ??? what does this
       mean ???
        <ul>
         <li> Translates the Sbox operations as individual functions. </li>
         <li> Column i of the new round key is computed by
         SBox(K_p) + C + sum(K_1,...,K_i). K_i is the i-th column of the
         previous round key. K_p is a column of certain key words from the
         previous round key. </li>
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
       <li> Canonical translation. ??? here we have more options ??? </li>
       <li> r_0-based representation. </li>
       <li> Minimum representation. </li>
       <li> r_1-based representation. </li>
      </ol>
     </li>
    </ul>
   </li>
   <li> In each case, the first option is considered the default. </li>
   <li> We need instructions on how to generate each translation. These
   should occur here (in this plans file). </li>
   <li> See "Separate key-schedule and block-cipher" in
   AdvancedEncryptionStandard/plans/SAT2011/general.hpp; this aspects needs
   better explanations. </li>
   <li> For more information, see
   Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp. </li>
  </ul>


  \todo Open problems
  <ul>
   <li> For a description of the investigation, please see
   Cryptography/AdvancedEncryptionStandard/plans/SAT2011/general.hpp. </li>
   <li> Here is a list of experiments which still need to be run, or
   questions for which we still do not know the answer. </li>
   <li> We need to find out which solvers and local search algorithms
   perform best on different minimisation problems, see
   AdvancedEncryptionStandard/plans/SAT2011/Representations/general.hpp.
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
     <li> AES field multiplications (255): ??? what is the meaning of "255" ???
      <ul>
       <li> 00, 01, 02, 03, 09, 11, 13, 14 : DONE. </li>
       <li> The other 247. </li>
      </ul>
     </li>
     <li> AES Sbox linear map with field multiplications (no plans file yet).
     </li>
     <li> AES Sbox (see
     AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp)
     : DONE. </li>
     <li> AES field inversion (see
     AdvancedEncryptionStandard/plans/SAT2011/Representations/Inv_8.hpp)
     : DONE. </li>
     <li> AES Sbox linear map (see
     AdvancedEncryptionStandard/plans/SAT2011/Representations/LinearMap_8.hpp)
     : DONE. </li>
    </ul>
   </li>
   <li> We need to find minimum CNF representations for the following
   functions:
    <ul>
     <li> AES Sbox; no minimum yet, smallest 294 (see
     AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp).
     </li>
     <li> AES field inversion (see
     AdvancedEncryptionStandard/plans/SAT2011/Representations/Inv_8.hpp).
     </li>
     <li> AES Sbox linear map (see
     AdvancedEncryptionStandard/plans/SAT2011/Representations/LinearMap_8.hpp).
     </li>
     <li> AES Sbox linear map with field multiplications (no plans file yet).
     </li>
     <li> Small scale Sbox (no plans file yet). </li>
     <li> Small scale field multiplications (no plans file yet). </li>
     <li> Small scale field inversion (no plans file yet). </li>
     <li> Small scale linear map (no plans file yet). </li>
     <li> Small scale linear map with field multiplications (no plans file
     yet). </li>
     <li> AES field multiplications (255):
      <ul>
       <li> The remaining 251. </li>
       <li> 03, see
       AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_3_8.hpp.
       </li>
       <li> 02, minimum size 20 (see
       AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_2_8.hpp) :
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
     Experimentation/Investigations/BooleanFunctions/plans/Permutations/general.hpp).
     </li>
     <li> Random linear maps (see
     "Affine bijections over ZZ_2" in
     Experimentation/Investigations/BooleanFunctions/plans/Permutations/Linear.hpp).
     </li>
     <li> Random boolean functions (see
     Experimentation/Investigations/BooleanFunctions/plans/general.hpp). </li>
    </ul>
   </li>
  </ul>


  \todo Prepare experiments for the SAT 2011 paper ??? what is the status of
  this ???
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
  </ul>


  \todo Solvers to be used for experimentation
  <ul>
   <li> We need a list of the solvers we will use for all experiments. </li>
   <li> As time goes on, some solvers might not be used for later experiments
   based on earlier results, and this can be recorded here. </li>
   <li> Solvers to be used: ??? not up-to-date ???
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
   BuildSystem/ExternalSources/plans/SAT.hpp): ??? this should go to the
   build-system plans ???
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
   solvers" in BuildSystem/ExternalSources/SpecialBuilds/plans/SAT.hpp).
   </li>
  </ul>

*/
