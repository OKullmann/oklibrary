// Oliver Kullmann, 18.9.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Logics.hpp
  \brief Plans regarding building of (general) logic systems

  - For systems related to automated theorem proving see
    Buildsystem/ExternalSources/SpecialBuilds/plans/ATP.hpp.
  - While for Coq see
    Buildsystem/ExternalSources/SpecialBuilds/plans/Coq.hpp.
  - For model checking see
    Buildsystem/ExternalSources/SpecialBuilds/plans/ModelChecking.hpp.


  \todo Hets
  <ul>
   <li>
   http://www.informatik.uni-bremen.de/agbkb/forschung/formal_methods/CoFI/hets/index_e.htm
   looks like a reasonable system for handling translations from one problem
   description language into another. </li>
   <li> PJ will help with the installation. </li>
   <li> To avoid problems with Haskell, perhaps we assume GHC is installed
   (or how difficult is it to install GHC?). </li>
   <li> A problem is that the graph drawing tool uDraw
   http://www.informatik.uni-bremen.de/uDrawGraph/en/index.html is not open
   source? One should send them an e-mail (after 2005 apparently nothing
   happened anymore). </li>
  </ul>
  

  \todo Proof assistants
  <ul>
   <li> http://isabelle.in.tum.de/index.html </li>
  </ul>


  \todo Modal logic
  <ul>
   <li> DLP http://www-db.research.bell-labs.com/user/pfps or
   http://www.bell-labs.com/user/pfps/dlp </li>
   <li> TA http://www.doc.mmu.ac.uk/STAFF/U.Hustadt/mdp </li>
   <li> KSATC ftp://ftp.mrg.dist.unige.it/, pub/mrg-systems or
   ftp://ftp.mrg.dist.unige.it/pub/mrg-systems/KR98-sources/KSat-source/KSatC
   </li>
   <li> *SAT http://www.mrg.dist.unige.it/~tac/StarSAT.html </li>
  </ul>


  \todo %Support for bounded arithmetic
  <ul>
   <li> We would like to have a system which supported handling of logics
   like the various systems of bounded arithmetic. </li>
   <li> Haskell:
    <ol>
     <li> Are there logic libraries? </li>
    </ol>
   </li>
   <li> What does Isabelle deliver here? </li>
   <li> Ask the group of Stephen Cook (AB). </li>
   <li> Ask the group of Sam Buss (AB). </li>
   <li> What about the field of all these "general logics" --- do they have
   some general software system?? </li>
  </ul>

*/

