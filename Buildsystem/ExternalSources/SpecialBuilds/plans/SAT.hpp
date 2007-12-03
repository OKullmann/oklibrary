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
     <li> http://www.sat4j.org/ </li>
     <li> http://www-verimag.imag.fr/~cotton/jat/ </li>
     <li> http://sourceforge.net/projects/dpt </li>
     <li> http://www.cs.chalmers.se/Cs/Research/FormalMethods/MiniSat/
      <ol>
       <li> Minisat </li>
       <li> Minisat+ </li>
      </ol>
     </li>
     <li> Chaff: doesn't seem to be open source, and thus can't be included.
     </li>
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
       <li> An e-mail was sent to MJHH. However it seems that this old solver
       won't be updated (unlike the following), and so we don't include it.
       </li>
       <li> A better page seems
       http://www.st.ewi.tudelft.nl/sat/download.php </li>
       <li> Installing march_ks:
       \verbatim
ExternalSources/Installations/SAT> unzip $OKPLATFORM/ExternalSources/sources/SAT/March/march_ks.zip
ExternalSources/Installations/SAT> cd march_ks/
ExternalSources/Installations/SAT/march_ks> make
ExternalSources/Installations/SAT/march_ks> ./march_ks $OKPLATFORM/OKsystem/Transitional/Satisfiability/Solvers/OKsolver/SAT2002/app_tests/test_cases/full/uuf250-011.cnf
       \endverbatim
       seems to work now. </li>
       <li> Installing march_pl:
       \verbatim
ExternalSources/Installations/SAT> unzip $OKPLATFORM/ExternalSources/sources/SAT/March/march_pl.zip
ExternalSources/Installations/SAT> cd march_pl/
ExternalSources/Installations/SAT/march_pl> make
ExternalSources/Installations/SAT/march_pl> ./march_pl $OKPLATFORM/OKsystem/Transitional/Satisfiability/Solvers/OKsolver/SAT2002/app_tests/test_cases/full/uuf250-011.cnf 
       \endverbatim
       seems to work.
       </li>
       <li> Installing UnitMarch:
       \verbatim
ExternalSources/Installations/SAT> unzip $OKPLATFORM/ExternalSources/sources/SAT/March/UnitMarch.zip
ExternalSources/Installations/SAT> cd UnitMarch64/
ExternalSources/Installations/SAT/UnitMarch64> make
ExternalSources/Installations/SAT/UnitMarch64> ./UnitMarch_32_bits $OKPLATFORM/OKsystem/Transitional/Satisfiability/Solvers/OKsolver/SAT2002/app_tests/test_cases/full/uuf250-011.cnf 
       \endverbatim
       ??? Only a 32-bits version?
       </li>
      </ol>
     </li>
     <li> Installing HeerHugo http://www.win.tue.nl/~jfg/heerhugo.html :
     \verbatim
ExternalSources/Installations/SAT> tar -xzf $OKPLATFORM/ExternalSources/sources/SAT/HeerHugo/heerhugo0.3.tar.gz
ExternalSources/Installations/SAT> cd heerhugo0.3/
ExternalSources/Installations/SAT/heerhugo0.3> make
cc  -O   -c scan.c
scan.l: In function ‘yylook’:
scan.l:262: warning: cast from pointer to integer of different size
scan.l:276: warning: cast from pointer to integer of different size
cc  -O   -c termnode.c
cc  -O   -c main.c
main.c:27:1: warning: "NULL" redefined
In file included from /usr/include/_G_config.h:14,
                 from /usr/include/libio.h:32,
                 from /usr/include/stdio.h:72,
                 from main.c:17:
/usr/lib64/gcc/x86_64-suse-linux/4.0.2/include/stddef.h:403:1: warning: this is the location of the previous definition
main.c: In function ‘main’:
main.c:2610: warning: return type of ‘main’ is not ‘int’
cc  -O   -c parse.c
cc  -O   -o heerhugo scan.o termnode.o parse.o main.o
     \endverbatim
     Comments:
     <ol>
      <li> The first 2 warnings seems to indicate, that the code won't run on
      a 64-bit machine? </li>
      <li> In scan.c we find
      \verbatim
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	
	The copyright notice above does not evidence any   	
	actual or intended publication of such source code.	
      \endverbatim
      thus we shouldn't use this file anyway. </li>
      <li> We should contact the authors and ask about the 64-bit
      issue, and this copyright statement (possibly rewriting the code). </li>
     </ol>
     </li>
     <li> SatMate : http://www.cs.cmu.edu/~modelcheck/satmate </li>
     <li> SIMO ? SIM ? </li>
     <li> Kcnfs: likely not under active development anymore. </li>
     <li> http://sat.inesc-id.pt/OpenSAT/index.php : DONE (not to be
     included by request of the authors) </li>
    </ol>
   </li>
  </ul>


  \todo Model checking and variations
  <ul>
   <li> Alloy http://alloy.mit.edu/alloy4/ is open source. </li>
   <li> BAT http://www.cc.gatech.edu/~manolios/bat/ </li>
   <li> ABC http://www.eecs.berkeley.edu/~alanmi/abc/ </li>
  </ul>


  \todo BDD's
  <ul>
    <li> CUDD http://vlsi.colorado.edu/~fabio/CUDD/ </li>
  </ul>


  \todo Automated theorem proving
  <ul>
   <li> Prover9 http://www.cs.unm.edu/~mccune/prover9/ </li>
  </ul>


  \todo CSP
  <ul>
   <li> http://en.wikipedia.org/wiki/Constraint_programming gives an
   overview. </li>
   <li> Eclipse http://eclipse-clp.org/ </li>
   <li> Gnu Prolog http://www.gprolog.org/ </li>
   <li> Swi Prolog http://www.swi-prolog.org/ </li>
   <li> Gecode http://www.gecode.org/
   <li> Minion http://minion.sourceforge.net/ </li>
   <li> Benchmarks? Competitions? </li>
  </ul>

*/

