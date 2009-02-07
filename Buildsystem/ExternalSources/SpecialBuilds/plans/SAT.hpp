// Oliver Kullmann, 16.10.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp
  \brief Plans regarding building of SAT solvers and libraries


  \bug GRASP cannot be compiled with gcc version 4.3
  <ul>
   <li> The problem is the reference to non-standard headers like
   "iostream.h". </li>
   <li> Perhaps we always provide gcc version 4.1.2 (besides the newest
   version), and build grasp with this version.
    <ol>
     <li> Then we need special buildsystem-variables for this "last old
     version". </li>
     <li> See "Providing gcc 4.1.2" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp. </li>
    </ol>
   </li>
   <li> But we should also contact Joao Marques-Silva about a possible
   update of his code. </li>
  </ul>


  \todo Installation in general
  <ul>
   <li> Perhaps we put building of the different solvers all into one makefile
   "sat.mak". Better not; but via "oklib SAT" all SAT-solvers are build. </li>
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
   <li> Compare "Organisation of links" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp. </li>
  </ul>

  
  \todo SAT4J
  <ul>
   <li> http://www.sat4j.org/ </li>
  </ul>


  \todo Jat
  <ul>
   <li> http://www-verimag.imag.fr/~cotton/jat/ </li>
  </ul>


  \todo SMT (SAT modulo theories)
  <ul>
   <li> Dpt
    <ol>
     <li> http://sourceforge.net/projects/dpt </li>
    </ol>
   </li>
   <li> OpenSMT
    <ol>
     <li> http://verify.inf.unisi.ch/opensmt </li>
    </ol>
   </li>
  </ul>


  \todo Minisat
  <ul>
   <li> http://www.cs.chalmers.se/Cs/Research/FormalMethods/MiniSat/
   <li> Minisat
   \verbatim
Installations/SAT> unzip ../../sources/SAT/MiniSat/MiniSat_v1.14.2006-Aug-29.src.zip
Installations/SAT> cd MiniSat_v1.14
> make
   \endverbatim
   creates the executable "minisat". It should be "make rs" for the
   "statically linked release version".
   </li>
   <li> Minisat+ ? Let's ignore it at this time. </li>
   <li> What is minisat2-070721.zip ? The new version, apparently
   their new best piece. </li>
   <li> SatElite: According to the Minisat page, this is subsumed by
   Minisat2 (see above). </li>
  </ul>


  \todo Picosat
  <ul>
   <li> http://picosat.com/ </li>
  </ul>


  \todo March solvers
  <ul>
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
  </ul>


  \todo Satz
  <ul>
   <li> ParaSatz
   \verbatim
Satz> tar -xzf ../../../sources/SAT/Satz/ParaSatz.tar.gz
Satz> cd ParaSatz
   \endverbatim
   this is mainly a script to run Satz (214) in parallel. </li>
   <li> adaptg2wsat+
   \verbatim
Satz> tar -xf ../../../sources/SAT/Satz/adaptg2wsat+ForCompetition.tar
Satz> cd adaptg2wsat+ForCompetition
adaptg2wsat+ForCompetition> make
   \endverbatim
   creates executable adaptg2wsat+. And there is a pdf-file. </li>
   <li> adaptg2wsat0
   \verbatim
Satz> tar -xf ../../../sources/SAT/Satz/adaptg2wsat0.tar
Satz> cd adaptg2wsat0
adaptg2wsat0> make
   \endverbatim
   processes executable adaptg2wsat0, and there is a pdf-file. </li>
   <li> adaptg2wsat0, competition version:
   \verbatim
Satz> tar -xf ../../../sources/SAT/Satz/adaptg2wsat0ForCompetition.tar
Satz> cd adaptg2wsat0ForCompetition
   \endverbatim
   the make-file is erreneous (easy to be corrected: wrong file-name). </li>
   <li> eqsatz
   \verbatim
Satz> cp ../../../sources/SAT/Satz/eqsatz20.c .
Satz> gcc -O3 -o eqsatz eqsatz20.c
   \endverbatim
   </li>
   <li> g2wsat
   \verbatim
kullmann-0:Satz> cp ../../../sources/SAT/Satz/g2wsat2005.c .
kullmann-0:Satz> gcc -O3 -o g2wsat g2wsat2005.c
   \endverbatim
   </li>
   <li> Maxsat
   \verbatim
Satz> tar -xzf ../../../sources/SAT/Satz/maxsatz.tar.gz
Satz> cd maxsatz
maxsatz> make
   \endverbatim
   creates executables maxsatz, maxsat0, maxsat12, maxsat1234.
   </li>
   <li> Compactor
   \verbatim
Satz> cp ../../../sources/SAT/Satz/my_compact.c .
Satz> gcc -O3 -o compact my_compact.c
   \endverbatim
   </li>
   <li>
   \verbatim
Satz> cp ../../../sources/SAT/Satz/satz214.2.c .
Satz> gcc -O3 -o satz214 satz214.2.c
Satz> cp ../../../sources/SAT/Satz/satz215.2.c .
Satz> gcc -O3 -o satz215 satz215.2.c
   \endverbatim
   </li>
  </ul>


  \todo Walksat
  <ul>
   <li> http://www.cs.rochester.edu/u/kautz/walksat/ </li>
   <li> We should install Walksat, Gsat and MaxWalksat.
   \verbatim
builds/SAT/Walksat> tar -xzf ../../../sources/SAT/Walksat/walksat-dist.tgz
builds/SAT/Walksat> cd Walksat_v46
SAT/Walksat/Walksat_v46> make
   \endverbatim
   seems to work, and creates executables "makewff" (random formula generator),
   "makequeens", and "walksat"."walksat --help" also works. </li>
   <li> 
   <li> Sources under sources/SAT/Walksat. </li>
   <li> There is also "Clone" http://reasoning.cs.ucla.edu/clone/. </li>
  </ul>


  \todo BASolver
  <ul>
   <li> http://logic.pdmi.ras.ru/~basolver/index.html </li>
   <li> Apparently doesn't have a licence? So well? </li>
   <li> Contains also documentation about boolean function formats etc. </li>
  </ul>


  \todo HeerHugo
  <ul>
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
   </li>
   <li> Comments:
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
  </ul>


  \todo SatMate
  <ul>
   <li> http://www.cs.cmu.edu/~modelcheck/satmate </li>
  </ul>


  \todo SIMO? SIM?
  <ul>
   <li> *SAT http://www.mrg.dist.unige.it/~tac/StarSAT.html </li>
  </ul>


  \todo Kcnfs
  <ul>
   <li> http://www.laria.u-picardie.fr/~dequen/sat/ </li>
   <li> One obtains there kcnfs-2006; but the link to kcnfs-smp is
   broken. </li>
   <li> On this site one finds als qgs, a "quasigroup existence problem
   solver", which is also interesting for us. However this solver is
   not open-source. </li>
  </ul>


  \todo Belief and Survey propagation
  <ul>
   <li> http://users.ictp.it/~zecchina/SP/
    <ol>
     <li> OK should contact Riccardo Zecchina to tell them about the OKlibrary,
     and discuss whether they are interested in contributing (first at the
     Maxima/Lisp level). </li>
     <li> Installing sp-1.4b for k-SAT
     \verbatim
builds/SAT/SurveyPropagation> tar -xzf ../../../sources/SAT/SurveyPropagation/sp-1.4b.tgz
builds/SAT/SurveyPropagation> cd sp-1.4
builds/SAT/SurveyPropagation/sp-1.4> make
builds/SAT/SurveyPropagation/sp-1.4> ./sp -h
./sp [options]
  formula
        -n <numvars>
        -m <numclauses>
        -a <alpha>
        -R <rho>         modified dynamics (0=sp, 1=bp)
                         (real values inbetween may make sense)
        -l <filename>    reads formula from file
  solving
        -d <danger>      fix this much PAP spins (experimental)
        -f <fixes>       per step
        -% <fixes>       per step (%)
        -e <error>       criterion for convergence
        -z               use lazy convergence instead of sequential
        -i <iter>        maximum number of iterations until convergence
  stats
        -p <filename>    output a magneticity plot
        -r <filename>    replay file
        -c               computes complexity
        -F               print fields
        -v               increase verbosity
  misc
        -s <seed>        (0=use time, default=1)
        -/               stop after first convergence
        -h               this help
     \endverbatim
     successful.
     </li>
    </ol>
   </li>
   <li> There should be other implementations out there (belief propagation
   etc.). </li>
  </ul>


  \todo tts
  <ul>
   <li> http://www.cs.qub.ac.uk/~i.spence/tts/ </li>
   <li> However some form is to be filled out; we should contact the author.
   </li>
  </ul>


  \todo Boehm solver
  <ul>
   <li> Still on the Internet? </li>
  </ul>


  \todo C-Sat
  <ul>
   <li> Daniel LeBerre mentioned he still should have the code. </li>
  </ul>


  \todo Posit


  \todo Answer set programming
  <ul>
   <li> Potassco
    <ol>
     <li> See http://potassco.sourceforge.net/. </li>
     <li> Sources in ExternalSources/sources/SAT/Potassco. </li>
    </ol>
   </li>
  </ul>


  \todo Generators
  <ul>
   <li> http://www.dis.uniroma1.it/~massacci/cryptoSAT should contain
   the DES -> SAT translator. </li>
   <li> http://www.tcs.ifi.lmu.de/~friedman/#paritysat </li>
  </ul>


  \todo Symmetries
  <ul>
   <li> Saucy http://vlsicad.eecs.umich.edu/BK/SAUCY/
    <ol>
     <li> There shouldn't be a problem with distributing this package
     with the OKlibrary. </li>
     <li> OK contacted the authors. </li>
    </ol>
   </li>
   <li> ZAP ? </li>
   <li> shatter http://www.eecs.umich.edu/~faloul/Tools/shatter/ is not
   open source. </li>
  </ul>


  \todo DONE (problems solved by new package from Joao Marques-Silva, and
  complete installation)
  Grasp
  <ul>
   <li> Web site to cite: http://users.ecs.soton.ac.uk/jpms/soft/ </li>
   <li> Manual build:
   \verbatim
builds/SAT/Grasp/tar -xjf ../../../sources/SAT/GRASP/grasp-release-20080622.tar.bz2
cd grasp-release-20080622
make
./sat-grasp ../../../../../OKsystem/Transitional/Satisfiability/Solvers/OKsolver/SAT2002/app_tests/test_cases/basic/DHNExample.cnf
   \endverbatim
   </li>
   <li> Code needs update (warnings). </li>
   <li> How to read "sat-grasp.doc" ?
    <ol>
     <li> We provide an amended version. </li>
     <li> Then running
     \verbatim
asciidoc --doctype manpage --backend xhtml11 --unsafe --out-file=sat-grasp_amended.html sat-grasp_amended.doc
     \endverbatim
     creates a reasonable html-page, while by
     \verbatim
less sat-grasp_amended.doc
     \endverbatim
     the man-page can be viewed. </li>
     <li> Using "--doctype article" produces a slight variation instead. </li>
     <li> What is the meaning of "--unsafe"?? Without this option, some
     style-sheets are not used. </li>
    </ol>
   </li>
   <li> Using "sat-grasp -V file", the only output is "-V", and the return
   value is 1. This seems to be a bug (this happens for all kinds of DIMACS-
   files). </li>
   <li> Using "+V", which seems to be the default (different from what
   sat-graps.doc says), it works. </li>
  </ul>


  \todo Maxsat
  <ul>
   <li> Some solvers are at http://web.udl.es/usuaris/m4372594/software.html.
   </li>
  </ul>


  \todo DONE Chaff
  <ul>
   <li> Not open source, and thus can't be included. </li>
  </ul>


  \todo DONE Rsat
  <ul>
   <li> Not open source, and thus can't be included. </li>
   <li> Like (unfortunately) all the other software at
   http://reasoning.cs.ucla.edu/index.php?p=software.php. </li>
  </ul>


  \todo DONE Fahiem Bacchus' solvers
  <ul>
   <li> http://www.cs.toronto.edu/~fbacchus/sat.html </li>
   <li> Not available under a free licence. </li>
  </ul>


  \todo DONE OpenSAT
  <ul>
   <li> Not to be included by request of the authors. </li>
   <li> http://sat.inesc-id.pt/OpenSAT/index.php </li>
  </ul>

*/

