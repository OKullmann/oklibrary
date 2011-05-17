// Oliver Kullmann, 6.2.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/Minisat/Minisat2/plans/general.hpp
  \brief Plans for maintaining Minisat2


  \todo Create milestones


  \todo Newer versions
  <ul>
   <li> Likely this module should also include minisat-2.2.0 and further
   versions. </li>
   <li> While perhaps version 2 is abandoned (compare "Minisat" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp). </li>
  </ul>


  \todo Links
  <ul>
   <li> See "Minisat" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp for further
   improvements. </li>
  </ul>


  \todo Output learned clauses
  <ul>
   <li> Compare "Output of learned clauses" in
   Satisfiability/Solvers/Argosat/plans/general.hpp. </li>
   <li> It would be useful, if we could add another option which makes
   minisat2 output (certain) learned clauses. </li>
   <li> Definitely learned unit-clauses are of interest, and perhaps in general
   one could use a size bound (with default value 1). </li>
  </ul>


  \todo DONE Improve patches
  <ul>
   <li> OK doesn't know the "standard" way of providing and applying patches.
   </li>
   <li> MG should look into this, and update the patches and their application
   accordingly. </li>
   <li> Consider http://www.linuxjournal.com/article/3991?page=0,0 on 
   contributing to the linux kernel, as well as the Linux kernel mailing
   list. </li>
   <li> See "Patches" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp . </li>
  </ul>

*/

