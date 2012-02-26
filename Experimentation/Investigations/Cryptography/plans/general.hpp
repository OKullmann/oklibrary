// Matthew Gwynne, 28.03.2009 (Swansea)
/* Copyright 2009, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/plans/general.hpp
  \brief General plans regarding investigations into cryptographic systems.


  \todo Comparing translations and solvers
  <ul>
   <li> We consider the following translations of boolean functions
   into SAT:
    <ul>
     <li> canonical translation - implemented by dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac .
     </li>
     <li> 1-base translation - discussed in
     Satisfiability/Reductions/Bases/plans/UcpBase.hpp. </li>
     <li> minimum translation - discussed in
     "Minimum CNF/DNF representations" in
     Investigations/BooleanFunctions/MinimumRepresentations/plans/general.hpp.
     </li>
    </ul>
   </li>
   <li> For all experiments in this module, we need to provide statistical
   analyses of which translations and solvers perform better than others. </li>
   <li> We also need to investigate the different relations that occur within
   the data. </li>
   <li> This todo should act as a place for linking together these
   observations and methods for analysis. </li>
  </ul>


  \todo Translating stream ciphers using our translations
  <ul>
   <li> We already offer translations of DES and AES into SAT
   in Cryptography/AdvancedEncryptionStandard/plans/general.hpp and
   Cryptography/DataEncryptionStandard/plans/general.hpp. </li>
   <li> There have been SAT translations of various other
   cryptographic ciphers, including:
    <ul>
     <li> Grain, </li>
     <li> Trivium, </li>
     <li> Bivium-B, </li>
     <li> HiTag2, and </li>
     <li> Crypto1. </li>
    </ul>
   </li>
   <li> We should investigate these translations and consider
   whether we can make further contributions in these areas.
   </li>
   <li> For example, "Grain of Salt" by Mate Soos, is a translation
   system for stream ciphers, which should be easily extendable to
   allow translation of cipher components using 1-soft translations.
   This is discussed in "Grain of salt" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Cryptography.hpp. </li>
  </ul>



  \todo CP 2012
  <ul>
   <li> We intend to submit a paper to the CP 2012 conference. </li>
   <li> The website for CP 2012 is at http://www.cp2012.org/. </li>
   <li> In this paper we will investigate translations of the DES, AES
   and small scale AES, given in [Algebraic Aspects of the Advanced
   Encryption Standard;Cid, Murphy, and Robshaw], using our notion of
   hardness to compare translations. </li>
   <li> The deadlines are:
    <ul>
     <li> Abstract: 19th April 2012. </li>
     <li> Paper: 23rd April 2012. </li>
     <li> Provisional reviews: 25th May 2012. </li>
     <li> Author feedback due: 27th May 2012. </li>
     <li> Notification of Acceptance: 3rd June 2012. </li>
     <li> Final version due: 1st July 2012. </li>
     <li> Conference: 8-12th October 2012. </li>
    </ul>
   </li>
   <li> For investigations and open problems in this area see
   Cryptography/AdvancedEncryptionStandard/plans/general.hpp.
   </li>
   <li> This should also link in with
   Experimentation/Investigations/plans/CurrentExperiments.hpp. </li>
  </ul>


  \todo SCC 2012
  <ul>
   <li> We should submit a paper to the SCC 2012 conference,
   on our work with DES and AES. </li>
   <li> The SCC conference allows submission of work in progress papers
   and so this is something that can be done in conjunction with the
   CP 2012 paper, as discussed above. </li>
   <li> The website for SCC 2012 is at http://scc2012.unican.es/ . </li>
   <li> The deadlines are:
    <ul>
     <li> Paper: 28th March 2012. </li>
     <li> Notification of Acceptance: 18th May 2012. </li>
     <li> Final version due: 30th May 2012. </li>
     <li> Deadline for registration: 12th June 2012. </li>
     <li> Conference: 11-13th July 2012. </li>
    </ul>
   </li>
   <li> This should link in with
   Experimentation/Investigations/plans/CurrentExperiments.hpp. </li>
  </ul>


  \todo DONE (Moved to CP2012) SAT 2012
  <ul>
   <li> We intend to submit a paper investigating the various translations of
   the AES and small scale AES given in [Algebraic Aspects of the Advanced
   Encryption Standard;Cid,Murphy Robshaw] to the SAT 2012 conference (see
   http://sat2012.fbk.eu/). </li>
   <li> The deadlines are:
    <ul>
     <li> Abstract: 5th Feb 2012. </li>
     <li> Paper: 12 Feb 2012. </li>
     <li> Rebuttal: 28-30 Feb 2012. </li>
     <li> Final notification: 12 April 2012. </li>
     <li> Final version due: 4 May 2012. </li>
     <li> Conference: 17-20 June 2012. </li>
    </ul>
   </li>
   <li> For investigations and open problems in this area see
   Cryptography/AdvancedEncryptionStandard/plans/general.hpp.
   </li>
   <li> This should also link in with
   Experimentation/Investigations/plans/CurrentExperiments.hpp. </li>
  </ul>


  \todo Add todos : DONE

*/
