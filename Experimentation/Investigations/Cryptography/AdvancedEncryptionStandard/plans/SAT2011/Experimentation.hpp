// Matthew Gwynne, 2.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp
  \brief AES experiments related to the SAT 2011 paper


  \todo Links
  <ul>
   <li> See 
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/AESKeyDiscovery.hpp
   for experiments regarding breaking AES (i.e. finding the key given the
   plaintext and ciphertext. </li>
   <li> See 
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/SmallScaleKeyDiscovery.hpp
   for experiments regarding breaking small scale AES (i.e. finding the key
   given the plaintext and ciphertext. </li>
   <li> See 
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/EncryptionDecryption.hpp 
   for experiments regarding computing encryption and decryption using the
   AES SAT translation. </li>
   <li> See 
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/general.hpp
   for experiments regarding representations of the AES and small scale boxes.
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
