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


  \todo SplittingViaOKsolver
  <ul>
   <li> We want to investigate the benefits on using the "Cube and Conquer"
   technique as described in
   [Cube and Conquer: Guiding CDCL SAT Solvers by Lookaheads;
    Heule, Kullmann, Wieringa, and Biere], on hard cryptographic instances. </li>
   <li> We use SplittingViaOKsolver here. </li>
   <li> For the individual ciphers, we have the following investigations:
    <ul>
     <li> DES: "SplittingViaOKsolver" in
     Investigations/Cryptography/DataEncryptionStandard/plans/general.hpp.
     </li>
     <li> AES: "SplittingViaOKsolver" in
     Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp.
     </li>
     <li> KeeLoq: "SplittingViaOKsolver" in
     Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp.
     </li>
    </ul>
   </li>
   <li> We need an overview on such investigations at this level. </li>
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


  \todo Create milestones.

*/
