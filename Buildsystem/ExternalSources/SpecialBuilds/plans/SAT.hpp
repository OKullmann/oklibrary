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
     <li> March:
      <ol>
       <li> Homepage http://www.st.ewi.tudelft.nl/sat/march_dl.php </li>
       <li> Installation attempt:
       \verbatim
ExternalSources/Installations/SAT> tar -xzf $(OKplatform)/ExternalSources/sources/SAT/march_dl.tgz
ExternalSources/Installations/SAT> cd march_dl/
nstallations/SAT/march_dl> make
gcc -c -s -O3 -fomit-frame-pointer -falign-functions=4 -funroll-loops -fexpensive-optimizations -malign-double -fschedule-insns2 -Wall   -o march.o march.c
march.c:1: error: -malign-double makes no sense in the 64bit mode
make: *** [march.o] Error 1
       \endverbatim
       </li>
       <li> An e-mail was sent to MJHH. </li>
       <li> A better page seems
       http://www.st.ewi.tudelft.nl/sat/download.php. </li>
       <li> Installing march_ks:
       \verbatim
ExternalSources/Installations/SAT> unzip $(OKplatform)/ExternalSources/sources/SAT/march_ks.zip
ExternalSources/Installations/SAT> cd march_ks/
ExternalSources/Installations/SAT/march_ks> make
       \endverbatim
       seems correct, but then
       \verbatim
ExternalSources/Installations/SAT/march_ks> ./march_ks OKplatform/OKsystem/Transitional/Satisfiability/Solvers/OKsolver/SAT2002/app_tests/test_cases/full/uuf250-011.cnf
c parseCNF():: the CNF contains 0 unary clauses.
c runParser():: parsing was successful, warming up engines...
Segmentation fault
       \endverbatim
       Valgrind shows
       \verbatim
==6582== Invalid read of size 4
==6582==    at 0x40D41B: clsCompare (in OKplatform/ExternalSources/Installations/SAT/march_ks/march_ks)
==6582==    by 0x41E439: msort_with_tmp (in OKplatform/ExternalSources/Installations/SAT/march_ks/march_ks)
       \endverbatim
       So it doesn't seem fit yet. </li>
      </ol>
     </li>
     <li> Kcnfs: likely not under active development anymore. </li>
    </ol>
   </li>
  </ul>

*/

