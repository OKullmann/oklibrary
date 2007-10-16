// Oliver Kullmann, 16.10.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp
  \brief Plans regarding building of SAT solvers and libraries


  \todo Installing SAT solvers
  <ul>
   <li> Installing external SAT solvers should be a special service. </li>
   <li> Perhaps we have a special subdirectory of OKplatform/ExternalSources
   (for example OKplatform/ExternalSources/SATsolver) ? </li>
   <li> And perhaps also for SpecialBuilds we should create a sub-module
   SATsolvers ? (We could subdivide SpecialBuilds into the different
   sections.) </li>
   <li> These solvers should be somewhat more integrated into the OKlibrary
   than just ordinary external sources; on the other hand, these packages
   are usually in much worse shape. </li>
   <li> Of course, only open-source-solvers. </li>
   <li> For C++ solvers we need the ability to use older gcc-versions. </li>
   <li> We should have also a directory Buildsystem/ExternalSources/SATsolvers
   (solvers are not "SpecialBuilds"). </li>
   <li> List of solvers/libraries:
    <ol>
     <li> http://sat.inesc-id.pt/OpenSAT/index.php </li>
     <li> http://www.sat4j.org/ </li>
     <li> http://www-verimag.imag.fr/~cotton/jat/ </li>
     <li> http://sourceforge.net/projects/dpt </li>
     <li> Minisat: is there a "golden version"? Is it under active
     development? </li>
     <li> Chaff: same questions. </li>
     <li> March: same questions, and is there a package? </li>
     <li> Kcnfs: likely not under active development anymore. </li>
    </ol>
   </li>
  </ul>

*/

