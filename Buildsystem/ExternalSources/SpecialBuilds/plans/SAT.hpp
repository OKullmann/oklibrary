// Oliver Kullmann, 16.10.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp
  \brief Plans regarding building of SAT solvers and libraries


  \todo Fuzzing
  <ul>
   <li> Install tools by Biere's group, http://fmv.jku.at/software/ ,
   especially http://fmv.jku.at/fuzzddtools/. </li>
   <li> See "Target app_tests" in
   Buildsystem/OKlibBuilding/plans/TargetSpecifications.hpp for the integration
   into the application-testsystem. </li>
  </ul>


  \todo Write docus for March
  <ul>
   <li> Currently we build march_pl with the (local) gcc version 4.1.2. </li>
   <li> Once we are using a newer gcc-version, then we shall use the corrected
   versions provided in OKlib/Satisfiability/Solvers/March/ (this can't be
   used now due to a deficiency of 4.1.2). </li>
   <li> See "March solvers" below. </li>
   <li> Remark that march_pl relies on the fact that n in the parameter line
   "p cnf n c" is an upper bound on the largest occurring variable (index;
   segmentation fault occurs if a literal x is found with abs(x) > n). <li>
  </ul>


  \todo Write docus for Satz
  <ul>
   <li> Mention the patch. </li>
   <li> See "Satz" below.
  </ul>


  \todo Write docus for Minisat
  <ul>
   <li> Mention the patched makefile regarding minisat+, and the usage of the
   locally provided GMP. </li>
   <li> See "Minisat" below
  </ul>


  \todo Update of GRASP
  <ul>
   <li> When switching to a newer gcc-version, then either we need to
   fix 4.1.2, or we need to update the code. </li>
   <li> See "Providing gcc 4.1.2" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp. </li>
   <li> We should also contact Joao Marques-Silva about a possible
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
   <li> Perhaps we mention that we do not provide
    <ol>
     <li> http://www.cs.toronto.edu/~fbacchus/sat.html </li>
     <li> http://reasoning.cs.ucla.edu/rsat/ and
     http://reasoning.cs.ucla.edu/index.php?p=software.php </li>
     <li> Chaff </li>
    </ol>
    can't be included since theq are not open-source; but we provide these
    links. </li>
    <li> On this link-page perhaps also
    http://sat.inesc-id.pt/OpenSAT/index.php is mentioned, but that this
    is not included on request of the authors. </li>
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
   <li> DONE (installed locally)
   Minisat needs the library zlib http://www.zlib.net/ ; which we should
   install locally.
    <ol>
     <li> Then the directories for include-library and link-library need to
     be specified for the makefile. </li>
     <li> Easiest (in this case for minisat2) to use
     \verbatim
CFLAGS='-I$(MTL) -Wall -ffloat-store'" -I$(zlib_source_library_okl)" LFLAGS="$(zlib_link_option_okl)"
     \endverbatim
     </li>
     <li> Should building of zlib be independent (like for bzip2), or just
     embedded into building minisat? </li>
     <li> Simplest we make it independent, just copying the bzip2-approach.
     </li>
    </ol>
   </li>
   <li> DONE (building minisat2, but not minisat1)
   Minisat
   \verbatim
Installations/SAT> unzip ../../sources/SAT/MiniSat/MiniSat_v1.14.2006-Aug-29.src.zip
Installations/SAT> cd MiniSat_v1.14
> make
   \endverbatim
   creates the executable "minisat". It should be "make rs" for the
   "statically linked release version".
   </li>
   <li> However, the statically linked version is 2.5 MG large, the normal
   one ("minisat_release", created by "make r") only 38 KB ?! But it seems
   the statically linked version might be slightly faster. </li>
   <li> Minisat+
    <ol>
     <li>
     \verbatim
Installations/SAT/Minisat> unzip ../../../sources/SAT/MiniSat/minisat+_2007-Jan-05.zip
Minisat> cd minisat+
minisat+> make rx
OKplatform/bin> ln -s $OKPLATFORM/ExternalSources/Installations/SAT/Minisat/minisat+/minisat+_64-bit_static
     \endverbatim
     </li>
     <li> What is now the input format?
      <ul>
       <li> It seems only lines of the form
       \verbatim
+1*v1 +2*v4 -3*v11 >= 4;
       \endverbatim
       are allowed (no spaces after the semicolon!). </li>
       <li> Relations are "<=, >=, =". </li>
       <li> Variable names are identifiers, and start with a letter. </li>
       <li> The "+"-symbols are not needed. </li>
       <li> Comment lines start with "*". </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> What is minisat2-070721.zip ? The new version, apparently
   their new best piece.
   \verbatim
Installations/SAT/Minisat> unzip ../../../sources/SAT/MiniSat/minisat2-070721.zip
Installations/SAT/Minisat> cd minisat
/SAT/Minisat/minisat> cd simp
minisat/simp> make rs
OKplatform/bin> ln -s OKPLATFORM/xternalSources/Installations/SAT/Minisat/minisat/simp/minisat_static minisat2
OKplatform> minisat2 -h
This is MiniSat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
USAGE: minisat2 [options] <input-file> <result-output-file>
  where input may be either in plain or gzipped DIMACS.
OPTIONS:
  -pre           = {none,once}
  -asymm
  -rcheck
  -grow          = <num> [ >0 ]
  -polarity-mode = {true,false,rnd}
  -decay         = <num> [ 0 - 1 ]
  -rnd-freq      = <num> [ 0 - 1 ]
  -dimacs        = <output-file>
  -verbosity     = {0,1,2}
   \endverbatim
   where "pre" is apparently for preprocessing, and the default is "once".
   </li>
   <li> Apparently specifying "-dimacs=OUTPUTFILE" just runs the preprocessor
   (keeping the old variable names, and thus producing gaps in the numbering;
   likely only equivalence reductions are used(?)).
   </li>
   <li> SatElite: According to the Minisat page, this is subsumed by
   Minisat2 (see above). </li>
   <li> So it seems that Minisat2 subsumes the old version 1.14 and the
   preprocessor, and we need only to install this version (for a SAT
   solver; there are extensions of SAT like pseudo-boolean). Though on
   some instances Minisat is better than Minisat2, so we better keep
   both. </li>
   <li> We should give additional documentation:
    <ol>
     <li> See above for the options. </li>
     <li> Output of a new line happens when total number of conflicts =
     1.5 * old number of conflicts. </li>
     <li> Apparently the new limit for learnt clauses is 1.1 * old limit. </li>
     <li> The meaning of "Progress":
      <ol>
       <li> The code for progress estimation is
       \verbatim
double Solver::progressEstimate() const {
    double  progress = 0;
    double  F = 1.0 / nVars();
    for (int i = 0; i <= decisionLevel(); i++){
        int beg = i == 0 ? 0 : trail_lim[i - 1];
        int end = i == decisionLevel() ? trail.size() : trail_lim[i];
        progress += pow(F, i) * (end - beg);
    }
    return progress / nVars();
}
       \endverbatim
       where
       \verbatim
    vec<Lit>            trail;            // Assignment stack; stores all assigments made in the order they were made.
    vec<int>            trail_lim;        // Separator indices for different decision levels in 'trail'.
       \endverbatim
       </li>
       <li> So over the decision levels i from 0 to the current level i, it
       sums up a(i) * n^(-i-1), where a(i) is the number of assignments made
       (exactly) at level i. </li>
       <li> It seems the idea is, that if at level 0 all variables are
       assigned, then we get 100% progress (and solvers like minisat only
       consider total assignments), and in general the progress here counts
       just the percentage of assigned variables, while further levels
       contribute in the same way, only that their weight is diminished by a
       factor of n for each level. </li>
       <li> Thus for instances with at least 1000 variables only levels 0,1 are
       taken into account (since only three decimal places are printed), and
       level 1 can yield at most 0.1%. </li>
      </ol>
     </li>
    </ol>
    Once we've finished the documentation, we should ask the authors whether
    it's all correct.
   </li>
  </ul>


  \todo Picosat
  <ul>
   <li> http://fmv.jku.at/picosat/ </li>
   <li> DONE
   Likely the solver should be called "picosat913". </li>
   <li> Documentation:
    <ol>
     <li> We need to document the meaning of the output statistics. </li>
     <li> We need to document (and make precise) the options. </li>
     <li> First this documentation can go to the Picosat installation page.
     </li>
    </ol>
   </li>
   <li> Missing preprocessor:
    <ol>
     <li> Likely there exists a dedicated Picosat preprocessor? </li>
     <li> We should also write a wrapper for the minisat2-preprocessor. </li>
     <li> See "Preprocessing wrappers" in
     Satisfiability/Reductions/plans/general.hpp. </li>
    </ol>
   </li>
  </ul>


  \todo Precosat
  <ul>
   <li> At least the installation page is needed. </li>
   <li> Is there some documentation?
    <ol>
     <li> DONE (installed)
     We have two pdf-files in sources/SAT/Precosat. </li>
    </ol>
   </li>
   <li> DONE
   Install (additionally) new version 570. </li>
   <li> Install (additionally) versions "lingeling" and "plingeling". </li>
   <li> DONE (make call's make)
   How to manage several versions at the same time:
    <ol>
     <li> Perhaps target precosat calls targets precosat236 and precosat570,
     which just set precosat_recommended_version_number_okl. </li>
     <li> However, there is the problem that make-variables can not be set
     in the body of rules. </li>
     <li> So the setting of the version-number needed to happen at
     script-level, which is ugly. </li>
    </ol>
   </li>
   <li> It seems Precosat has many command-line options, and we need to
   document them. </li>
   <li> What is the meaning of the output? </li>
   <li> We need the option that Precosat outputs learned clauses up to a
   certain length to a file. </li>
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
ExternalSources/Installations/SAT/march_ks> ./march_ks $OKPLATFORM/OKsystem/OKlib/Satisfiability/Solvers/OKsolver/SAT2002/app_tests/test_cases/full/uuf250-011.cnf
   \endverbatim
   seems to work now. </li>
   <li> Installing march_pl:
   \verbatim
ExternalSources/Installations/SAT> unzip $OKPLATFORM/ExternalSources/sources/SAT/March/march_pl.zip
ExternalSources/Installations/SAT> cd march_pl/
ExternalSources/Installations/SAT/march_pl> make
ExternalSources/Installations/SAT/march_pl> ./march_pl $OKPLATFORM/OKsystem/OKlib/Satisfiability/Solvers/OKsolver/SAT2002/app_tests/test_cases/full/uuf250-011.cnf 
   \endverbatim
   seems to work.
   </li>
   <li> Installing UnitMarch:
   \verbatim
ExternalSources/Installations/SAT> unzip $OKPLATFORM/ExternalSources/sources/SAT/March/UnitMarch.zip
ExternalSources/Installations/SAT> cd UnitMarch64/
ExternalSources/Installations/SAT/UnitMarch64> make
ExternalSources/Installations/SAT/UnitMarch64> ./UnitMarch_32_bits $OKPLATFORM/OKsystem/OKlib/Satisfiability/Solvers/OKsolver/SAT2002/app_tests/test_cases/full/uuf250-011.cnf 
   \endverbatim
   ??? Only a 32-bits version?
   </li>
  </ul>


  \todo Argo
  <ul>
   <li> Is this software maintained? It seems that since version 1.0 no further
   developments took place, while for example documentation is missing? </li>
   <li> See Satisfiability/Solvers/Argosat/plans/general.hpp for our own
   developments. </li>
   <li> It seems that there is no documentation available? </li>
   <li> At least we should provide a specification of the output.
    <ol>
     <li> How to see whether unit-clauses have been derived? </li>
    </ol>
   </li>
   <li> http://argo.matf.bg.ac.yu/index.html has some interesting packages
   to offer. </li>
   <li> DONE
   Installation of ArgoSat:
   \verbatim
builds/SAT/Argo> tar -xzf ../../../sources/SAT/Argo/argosat-1.0.tar.gz
builds/SAT/Argo> cd argosat-1.0
argosat-1.0> ./configure --prefix=/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/SAT/Argo
argosat-1.0> make
argosat-1.0> make install

Installations/SAT/Argo/bin> ./argosat
ArgoSat v. 1.0
Build date: Fri Mar 20 18:15:46 UTC 2009
Usage: argosat problem [options]
    problem:
          pigeons num - pigeonhole problem
          queens  num - n queens problem
          filename    - file that contains uncompressed DIMACS format input
    options:
   \endverbatim
   looks alright (though output is somewhat uggly). </li>
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
   <li> wmaxsatz (see http://www.laria.u-picardie.fr/~cli/maxsatz2009.c)
   \verbatim
Satz> wget http://www.laria.u-picardie.fr/~cli/maxsatz2009.c -O wmaxsatz2009.c
Satz> sed -i 's/CLK_TCK/CLOCKS_PER_SEC/g' wmaxsatz2009.c
Satz> gcc -O3 -o wmaxsatz2009 wmaxsatz2009.c
   \endverbatim
   Note that wmaxsatz is not currently in ExternalSources but once moved
   there can simply be copied rather than using wget.
   </li>
   <li> DONE
   Satz 214 and 215
    <ol>
     <li> Installation:
     \verbatim
Satz> cp ../../../sources/SAT/Satz/satz214.2.c .
Satz> gcc -O3 -o satz214 satz214.2.c
Satz> cp ../../../sources/SAT/Satz/satz215.2.c .
Satz> gcc -O3 -o satz215 satz215.2.c
     \endverbatim
     </li>
     <li> DONE satz215 creates the file "timetable": Perhaps we should give
     it a better name. </li>
     <li> DONE (see "Updating the exit codes for Satz215" in
     Satisfiability/Solvers/Satz/plans/general.hpp)
     Yet the return value is always 1 --- we need to change this to
     10 (sat) resp. 20 (unsat). </li>
    </ol>
   </li>
  </ul>


  \todo MiniMaxSAT
  <ul>
   <li> http://www.lsi.upc.edu/~fheras/p6.html </li>
   <li> The source does not seem to be available on the site for this solver. 
   </li>
   <li> We should send an e-mail to the developers. </li>
  </ul>


  \todo EBDDRES
  <ul>
   <li> http://fmv.jku.at/ebddres/ </li>
   <li> Also interesting BooleForce (includes TraceCheck)
   http://fmv.jku.at/booleforce/index.html . </li>
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
     <li> DONE
     Installing sp-1.4b for k-SAT
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
     <li> Improving sp:
      <ol>
       <li> For our installation, perhaps we should write a wrapper script
       which handles the created auxiliary files (at least "formula.tmp.cnf").
       </li>
       <li> And we should improve the output: hopefully in the source code
       we find some comments on what ":-)" means, and perhaps there is also
       a counter available so that we see how many variables have been
       eliminated. </li>
      </ol>
     </li>
     <li> Then there is spy-1.2.tar.gz. </li>
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


  \todo 2cl
  <ul>
   <li> ftp://ftp.cse.ucsc.edu/pub/avg/2cl_simpDir/ </li>
  </ul>


  \todo Modoc
  <ul>
   <li> ftp://ftp.cse.ucsc.edu/pub/avg/Modoc/ </li>
  </ul>


  \todo DONE (Hantao Zhang considers it as obsolete, and doesn't want it to be
  included)
  Sato
  <ul>
   <li> http://www.cs.uiowa.edu/~hzhang/sato.html </li>
   <li> Is not open-source, and thus can't be redistributed. </li>
   <li> However one could ask the author Hantao Zhang hzhang@cs.uiowa.edu
   whether he might change the licence. </li>
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
     <li> OK contacted the authors: we can redistribute it, but we should
     show their licence on the documentation pape. </li>
     <li> shatter http://www.eecs.umich.edu/~faloul/Tools/shatter/ is not
     open source, but is now included with Saucy. </li>
    </ol>
   </li>
   <li> ZAP ? </li>
  </ul>


  \todo Maxsat
  <ul>
   <li> Some solvers are at http://web.udl.es/usuaris/m4372594/software.html.
   </li>
  </ul>


  \todo Counting
  <ul>
   <li> SampleCount and xor-addition:
   http://www.cs.cornell.edu/~sabhar/#software </li>
  </ul>

*/

