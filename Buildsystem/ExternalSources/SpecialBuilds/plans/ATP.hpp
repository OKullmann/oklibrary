// Oliver Kullmann, 17.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/ATP.hpp
  \brief Plans regarding installation of automated theorem provers


  \todo ACL2
  <ul>
   <li> Especially interesting since they are using SAT. </li>
   <li> http://www.cs.utexas.edu/~moore/acl2/ </li>
  </ul>


  \todo Prover 9
  <ul>
   <li> http://www.cs.unm.edu/~mccune/prover9/ </li>
   <li> Installation:
   \verbatim
ExternalSources/Installations/Prover9> tar -xzf $OKPLATFORM/ExternalSources/sources/Prover9/LADR-Dec-2007.tar.gz
ExternalSources/Installations/Prover9> cd LADR-Dec-2007/
LADR-Dec-2007> make all
LADR-Dec-2007> make test1
LADR-Dec-2007> make test2
LADR-Dec-2007> make test3
LADR-Dec-2007> bin/prover9 --version
   \endverbatim
   </li>
   <li> DONE Otter http://www.mcs.anl.gov/AR/otter/ ? (it seems that it was
   replaced by Prover9) </li>
  </ul>


  \todo Maude


  \todo First-order theory of the reals
  <ul>
   <li> QEPCAD http://www.cs.usna.edu/qepcad (or perhaps
   http://www.cs.usna.edu/~qepcad ) seems to be the main implementation. </li>
  </ul>

*/

