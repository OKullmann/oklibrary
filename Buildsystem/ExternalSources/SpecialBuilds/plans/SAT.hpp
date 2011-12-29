// Oliver Kullmann, 16.10.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp
  \brief Plans regarding building of SAT solvers and libraries


  \todo Parallel SAT solvers
  <ul>
   <li> See "ParaSatz" below. </li>
   <li> manySAT is proprietary?
    <ul>
     <li> See LICENSE in
     http://www.cril.univ-artois.fr/~jabbour/manysat2.0.zip. </li>
     <li> manySAT lists the minisat LICENSE as its LICENSE file. </li>
     <li> Technically, the license might only be for the minisat "portion"
     of manysat. We should contact the authors. </li>
    </ul>
   </li>
   <li> NAGSAT? </li>
   <li> PMSAT
    <ul>
     <li> PMSAT is based on minisat, and uses the same license (MIT). </li>
     <li> From the website:
     \verbatim
Both programs are under MIT License:

PMSat -- Copyright (c) 2006-2007, Luís Gil
MiniSat -- Copyright (c) 2003-2005, Niklas Eén, Niklas Sörensson
<snip>
     \endverbatim
     See http://algos.inesc-id.pt/algos/software.php?soft=2.
     </li>
    </ul>
   </li>
   <li> DONE
   There is a parallel version of Precosat: See Plingeling below. </li>
  </ul>


  \todo SAT solvers
  <ul>
   <li> The following solvers are currently not in the library, and
   must be investigated, and then added to the library:
    <ul>
     <li> Barcelogic
      <ul>
       <li> See http://www.lsi.upc.edu/~oliveras/bclt-main.html . </li>
       <li> This doesn't appear to be open-source; we should contact the
       authors. </li>
       <li> This is an SMT solver, although might also solve SAT instances.
       </li>
      </ul>
     </li>
     <li> borg-sat
      <ul>
       <li> See http://nn.cs.utexas.edu/pages/research/borg/ . </li>
       <li> Github repository: https://github.com/borg-project/borg . </li>
      </ul>
     </li>
     <li> CircleSAT
      <ul>
       <li> No website. </li>
       <li> See
       http://baldur.iti.uka.de/sat-race-2010/descriptions/solver_7.pdf .
       </li>
      </ul>
     </li>
     <li> CirCUs
      <ul>
       <li> See "CUSP" at http://vlsi.colorado.edu/~vis/ . </li>
      </ul>
     </li>
     <li> glucoER
      <ul>
       <li> See http://www.lri.fr/~simon/ .</li>
      </ul>
     </li>
     <li> kw
      <ul>
       <li> No website. </li>
       <li> See
       http://baldur.iti.uka.de/sat-race-2010/descriptions/solver_8+9.pdf .
       </li>
      </ul>
     </li>
     <li> LySAT
      <ul>
       <li> See http://www.msr-inria.inria.fr/~jabbour/ .</li>
      </ul>
     </li>
     <li> oprailleur
      <ul>
       <li> No website. </li>
       <li> See
       http://baldur.iti.uka.de/sat-race-2010/descriptions/solver_20.pdf . </li>
      </ul>
     </li>
     <li> riss
      <ul>
       <li> See http://www.wv.inf.tu-dresden.de/~norbert/html/riss.php . </li>
      </ul>
     </li>
     <li> rcl
      <ul>
       <li> See
       http://baldur.iti.uka.de/sat-race-2010/descriptions/solver_10.pdf .
       </li>
      </ul>
     </li>
     <li> SApperloT
      <ul>
       <li> See
       http://algo.inf.uni-tuebingen.de/?site=forschung/sat/SApperloT .
       </li>
      </ul>
     </li>
     <li> SAT-Power
      <ul>
       <li> No website. </li>
       <li> See
       http://baldur.iti.uka.de/sat-race-2010/descriptions/solver_22.pdf .
       </li>
      </ul>
     </li>
     <li> SATHYS
      <ul>
       <li> See http://www.cril.univ-artois.fr/~lagniez/ressource.html . </li>
      </ul>
     </li>
     <li> DONE glucose </li>
     <li> DONE CryptoMiniSAT </li>
    </ul>
   </li>
   <li> Some of these solvers will be parallel solvers, in which case,
   discussion should go to "Parallel SAT solvers". </li>
   <li> There are all new solvers available at SAT 2011; these need to be
   added to the list. </li>
  </ul>


  \todo EagleUp
  <ul>
   <li> Website at http://www.uni-ulm.de/in/theo/m/olivergableske.html. </li>
   <li> Installation:
   \verbatim
ExternalSources/builds/SAT> mkdir EagleUp
ExternalSources/builds/SAT> cd EagleUp
EagleUp> tar zxvf ../../../sources/SAT/EagleUP/eagleup.1.565.350.tar.gz
EagleUp> make
EagleUp> ./EagleUp
c EagleUP SAT solver
c	AUTHOR:		Oliver Gableske	<oliver.gableske at uni-ulm.de>	(Implemented most of the solver.)
c	CONTRIBUTORS:	Julian Rueth	<julian.rueth at gmail.com>	(Implemented the Luby-series calculation.)
c			Marijn Heule	<marijn at heule.nl>		(Provided various ideas.)
c	VERSION:	1.565.350
c	INFO:		Re-compile without the flag COMPETITION and the following flags in global.h enabled to see further
c			output: LONGHEADER,VERBOSE_SLS,VERBOSE_CDCL,VERBOSE_PREPROC,COLLINF_SLS,COLLINF_CDCL,COLLINF_PREP.
c
c INITIALIZING...
c
c Too few parameters. Exiting.
c USAGE: ./EagleUP BENCHNAME RANDOMSEED [TIMEOUTINSECONDS]
   \endverbatim
   </li>
  </ul>


  \todo (P)Lingeling
  <ul>
   <li> (p)lingeling should be added to the ExternalSources internet page.
   </li>
   <li> DONE (installed)
   First installatioin:
   \verbatim
builds/Lingeling> tar -xzf ../../../sources/SAT/Precosat/lingeling-276-6264d55-100731.tar.gz
> cd lingeling-276-6264d55-100731
> ./configure
> make
   \endverbatim
   </li>
   <li> We need an installation page: Perhaps picosat, precosat, lingeling etc.
   all go to the same page? </li>
   <li> Basic information:
   \verbatim
> ./lingeling --help
usage: lingeling [<option> ...][<file>[.gz]]

where <option> is one of the following:

--help        | -h    print command line option summary
--force       | -f    force reading even without header
--ignore      | -i    ignore additional clauses
--ranges      | -r    print value ranges of options
--defaults    | -d    print default values of options
--nowitness   | -n    no solution (see '--witness')

The following options can also be used in the form '--<name>=<int>',
just '--<name>' for increment and '--no-<name>' for zero.  They
can be embedded into the CNF file or set through the API as well.
Their default values are displayed at higher verbose levels or
explicitly with the command line option '-d' or '--defaults'.

--agile               agility limit for restarts
--bias                variable decision order initial bias
--block               enable initial blocked clause elimination
--blkmaxeff           maximal effort in blocked clause elimination
--blkmineff           minimal effort in blocked clause elimination
--blkreleff           relative effort in blocked clause elimination
--blkocclim           maximum number of occurrences in blocked clause elimination
--decompose           enable decompose
--dcpcintinc          decompose conflict interval increment
--dcpvintinc          decompose prop interval increment
--defragint           defragmentation pushed watches interval
--defragfree          defragmentation free watches limit
--distill             enable distillation
--dstcintinc          distill conflict interval increment
--dstvintinc          distill prop interval increment
--dstmaxeff           maximal effort in distilling
--dstmineff           minimal effort in distilling
--dstreleff           relative effort in distilling
--elim                enable eliminiation
--elmcintinc          eliminiation conflict interval increment
--elmvintinc          eliminiation prop interval increment
--elmaxeff            maximal effort in eliminiation
--elmineff            minimal effort in eliminiation
--elmreleff           relative effort in eliminiation
--elmreslim           maximum resolvent size in elimination
--elmocclim           maximum number of occurences in elimination
--elmpenlim           minimum eliminated variables penalty limit
--gccintinc           gc conflict interval increment
--gcvintinc           gc prop interval increment
--hte                 enable hidden tautology elimination
--htemaxeff           maximal effort in hidden tautology elimination
--htemineff           minimal effort in hidden tautology elimination
--htereleff           relative effort in hidden tautology elimination
--lhbr                enable lazy hyber binary reasoning
--phase               default phase
--plain               plain mode disables all preprocessing
--probe               enable probing
--prbcintinc          probing conflict interval increment
--prbvintinc          probing prop interval increment
--prbmaxeff           maximal effort in probing
--prbmineff           minimal effort in probing
--prbreleff           relative effort in probing
--seed                random number generator seed
--smallve             enable small number variables elimination
--smallvevars         variables in small number variables elimination
--randec              enable random decisions
--randecint           random decision interval
--rebias              enable rebiasing phases
--rebiasint           rebias interval
--redlinit            initial reduce limit
--redlinc             reduce limit increment
--redlmininc          minimum reduce limit increment
--restartint          restart interval
--scincinc            score increment increment in per mille
--transred            enable transitive reduction
--trdcintinc          transitive reduction prop interval increment
--trdvintinc          transitive reduction prop interval increment
--trdmineff           minimial effort in transitive reduction
--trdmaxeff           maximal effort in transitive reduction
--trdreleff           relative effort in transitive reduction
--verbose     | -v    verbosity level
--witness     | -w    print witness

kullmann-0:lingeling-276-6264d55-100731> ./plingeling -h
usage: plingeling [-t <threads>][-h][-n][-p][-v][<dimacs>[.gz]]
  -t <num>   number of worker threads (default 2 on this machine)
  -h         print this command line option summary
  -n         do NOT print solution / witness
  -p         plain portfolio, no sharing
  -v         increase verbose level
  -i         ignore additional clauses
   \endverbatim
   </li>
  </ul>


  \todo Install Relsat
  <ul>
   <li> http://code.google.com/p/relsat/ has the newest version 2.02
   (January 2007). </li>
   <li> We need this solver for historical reasons, for its performance
   on the original DES instances. </li>
   <li> The sources are in sources/SAT/Relsat. </li>
   <li> Installing RelSAT:
   \verbatim
ExternalSources/builds/SAT> mkdir Relsat && cd Relsat
builds/SAT/Relsat> mkdir relsat-2.2.0 && cd relsat-2.2.0
SAT/Relsat/relsat-2.2.0> tar xvf ../../../../sources/SAT/Relsat/relsat_2.02.tar
SAT/Relsat/relsat-2.2.0> mv Makefile.linux Makefile
SAT/Relsat/relsat-2.2.0> make
SAT/Relsat/relsat-2.2.0>  ./relsat
==== relsat, version 2.20 (alpha1) ====
Usage: relsat [options] {filename}
Options include:
 -l<int>          Learn order
 -#{<int>|a|c}    {# of solutions | all | count}
 -s<int>          Random # seed (> 0)
 -r{<int>|n}      Restart interval {seconds | no restarts}
 -a{<int>}        Restart interval increment {seconds}
 -f<float>        Fudge factor, in the range [0.0-1.0]
 -t{<int>|n}      Timeout interval {seconds | no timeout}
 -o{<filename>|n} Output instance {filename | no output instance}
 -u{<int>|n}      Status update interval {seconds | no updates}
 -p{0|1|2|3}      Pre-processing level
 -c{0|1|2|3}      Post-processing level
 -i{<int>|n}      Pre & post-processing iteration bound {bound | no bound}
 -v{<filename>}   Filename containing primary variable list.

Default options:
    -p1 -l3 -#1 -s1 -rn -a0 -f.9 -t43200 -u10 -o timeout.cnf -c3 -in
   \endverbatim
   </li>
   <li> Note that relsat doesn't package its contents in a directory;
   the archive extracts its contents in the current directory.
   We should repackage it. </li>
   <li> Linking to GMP:
    <ul>
     <li> According to the "HOW_TO_BUILD" file distributed with Relsat, it
     supports linking to GMP for certain functionality. </li>
     <li> If we do not do this, then asking Relsat to count the number of
     solutions may not work properly:
     \verbatim
>  echo -e "p cnf 2 1\n 1 2 0\n" > test.cnf
> ./relsat -#c test.cnf
<snip>
c WARNING: Not using a bignum package. Solution counts may overflow.
<snip>
Number of solutions: 3
     \endverbatim
     </li>
     <li> We can force Relsat to link against our version of GMP by updating
     the Makefile to have the following at the top:
     \verbatim
GMPDIR ?= /home/bayardo/gmp-3.1

#CFLAGS= -DNDEBUG -O3 -DNO_GMP
CFLAGS= -DNDEBUG -O3 -I$(GMPDIR)
#CFLAGS= -g -I$(GMPDIR)

CC = g++

.SUFFIXES: .o .cpp

LIBS = -lgmp
#LIBS =

LINKFLAGS = -L$(GMPDIR)/.libs
#LINKFLAGS =
<snip>
     \endverbatim
     and then running (for example)
     \verbatim
> GMPDIR="../../../Gmp/4.5.3/gmp-5.0.1/" make
     \endverbatim
     </li>
     <li> Running:
     \verbatim
>  echo -e "p cnf 2 1\n 1 2 0\n" > test.cnf
> ./relsat -#a test.cnf
<snip>
Number of solutions: 3
     \endverbatim
     then yields the model count with no error.
     </li>
    </ul>
   </li>
  </ul>


  \bug Grasp uses system C++ libraries
  <ul>
   <li> sat-grasp uses the system C++ libraries (libstdc++):
   \verbatim
bin> ldd sat-grasp
        linux-vdso.so.1 =>  (0x00007fffaf9d6000)
        libstdc++.so.6 => /usr/lib/libstdc++.so.6 (0x00007f56e7a76000)
        libm.so.6 => /lib/libm.so.6 (0x00007f56e77f3000)
        libgcc_s.so.1 => /lib/libgcc_s.so.1 (0x00007f56e75dc000)
        libc.so.6 => /lib/libc.so.6 (0x00007f56e7259000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f56e7d9f000)
   \endverbatim
   whereas it should use the OKlibrary GCC libraries. </li>
   <li> We need to pass the LDFLAGS="$(gcc412_linking_okl)" to
   sat-grasp somehow, however, changing the make line in
   the Makefile to:
   \verbatim
   make CC=$(gcc412_call_okl) CXX=$(gpp412_call_okl) LDFLAGS="$(gcc412_linking_okl)" all; $(postcondition) \
   \endverbatim
   has no effect.
   </li>
  </ul>


  \todo Improvements of CryptoMiniSat
  <ul>
   <li> Learned clauses:
    <ol>
     <li> It seems for our applications information about short learned clauses
     as well as obtaining the clauses themselves is most important. </li>
     <li> So one option "showsizelearnt=k" enables showing the number of
     learned clauses up to size k. </li>
     <li> While the option "savelearnt=filename" enables saving all learned
     clauses of size up to k to a file (in the order as the come, stored
     immediately and with output-flushing). </li>
     <li> Ask on the CryptoMiniSat mailing list. </li>
    </ol>
   </li>
   <li> Wrapper:
    <ol>
     <li> Perhaps we should write a cryptominisat-wrapper, which has default
     options "dumplearnts=filename_learned.cnf" and "maxdumplearnts=3". </li>
     <li> Shall this wrapper replace cryptominisat? For that we needed to be
     able to get rid off the additional options. </li>
    </ol>
   </li>
   <li> At this occasion one might also learn whether "learned" or "learnt"
   is better. </li>
   <li> xor-clauses:
    <ol>
     <li> It seems that on Ramsey-theory problems xor-clauses don't play any
     role. </li>
     <li> So one needs an option "--noxor" which completely turns off all
     xor-related activity. </li>
     <li> Currently there is a confusing number of options related to
     xor-reasoning, and it is unclear how to achieve the complete turning-off.
     </li>
     <li> Ask on the CryptoMiniSat mailing list. </li>
    </ol>
   </li>
  </ul>


  \todo Add todos for SAT-Race 2010 SAT solvers
  <ul>
   <li> See "Solvers to be used for experimentation" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp
   for a list of solvers from SAT-Race 2010. </li>
  </ul>


  \todo Glucose outputs to STDERR not STDOUT
  <ul>
   <li> See:
   \verbatim
> echo '' | glucose > /dev/null

c This is glucose 1.0 --  based on MiniSAT (Many thanks to MiniSAT team)

Reading from standard input... Use '-h' or '--help' for help.
c ============================[ Problem Statistics ]=============================
c |                                                                             |
c |  Parsing time:         0.00         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |       0        0        0 |        0        0   -nan |  0.000 % |
===============================================================================
c restarts              : 1
c nb ReduceDB           : 0
c nb learnts DL2        : 0
c nb learnts size 2     : 0
c nb learnts size 1     : 0
c conflicts             : 0              (-nan /sec)
c decisions             : 1              (0.00 % random) (inf /sec)
c propagations          : 0              (-nan /sec)
c conflict literals     : 0              (-nan % deleted)
c Memory used           : 1.86 MB
c CPU time              : 0 s
   \endverbatim
   </li>
  </ul>


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
   <li> Explain the different versions. </li>
   <li> See "Minisat" below. </li>
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


  \todo CryptoMiniSat : DONE
  <ul>
   <li> DONE (problem solved with the "official" release 2.9.0)
   Version 2.9.0.1 asserts:
    <ol>
     <li>
     \verbatim
OKplatform> PdVanderWaerdenCNF-O3-DNDEBUG 3 24 569
OKplatform> cryptominisat VanDerWaerden_pd_2-3-24_569.cnf
...
cryptominisat: Solver.cpp:1103: Clause* Solver::analyze(PropBy, vec<Lit>&, int&, uint32_t&, bool): Assertion `level[my_var] <= (int)decisionLevel()' failed.
Aborted (core dumped)
     \endverbatim
     valgrind doesn't find an error. </li>
     <li> Building by
     \verbatim
CXXFLAGS="-Wall -Wextra -Wno-ignored-qualifiers -Wfloat-equal -Wpointer-arith -Wcast-align -Wconversion -g -O2" oklib cryptominisat
     \endverbatim
     shows a lot of conversions from 64-bit values to 32-bit values. </li>
     <li> Compiling only with "-O1", we don't get the assert. </li>
    </ol>
   </li>
   <li> DONE (the author corrected this; branch "cluster129-fixed")
   The solver runs immediately out of memory:
    <ol>
     <li> For example:
     \verbatim
> cryptominisat VanDerWaerden_pd_2-3-24_593.cnf
c This is CryptoMiniSat 2.7.1
c WARNING: for repeatability, setting FPU to use double precision
c Reading file 'VanDerWaerden_pd_2-3-24_593.cnf'
c -- header says num vars:            297
c -- header says num clauses:       46881
c -- clauses added:            0 learnts,        46881 normals,            0 xors
c -- vars added        297
c Parsing time:  0.03 s
c Ouptutting solution to console
c asymm  cl-useful: 13/46487/46487 lits-rem:13 time: 0.81
c Time to fill non-learnt binary watchlists: 0.00 s
c subs with bin:        0  lits-rem:         0  v-fix:    0  time:  0.01 s
c Subs w/ non-existent bins:      0 l-rem:      0 v-fix:     0 done:    297 time:  0.01 s
c lits-rem:         0  cl-subs:        0  v-elim:      0  v-fix:    0  time:  0.88 s
c Finding binary XORs:         0.00 s (found:       0, avg size: -nan)
c Finding non-binary XORs:     0.04 s (found:       0, avg size: -nan)
c watched sorting time: 0.00
c Calc default polars -  time:   0.00 s pos:     297 undec:       0 neg:       0
c =========================================================================================
c types(t): F = full restart, N = normal restart
c types(t): S = simplification begin/end, E = solution found
c restart types(rt): st = static, dy = dynamic
c  t rt  Rest     Confl      Vars   NormCls    BinCls   Learnts    ClLits    LtLits
c  B st     0         0       297     46487       394         0    209779         0  no matrixes
c  N dy    17      4129       297     46487       394      3951    209779     99564  no matrixes
c  N dy    20      9304       297     46487       394      5144    209779    104566  no matrixes
c  N dy    37     13866       297     46487       394      9504    209779    207845  no matrixes
c  N dy    59     17960       297     46487       394      7376    209779    150338  no matrixes
c  N dy    84     22015       297     46487       394     11258    209779    242791  no matrixes
c  F st    98     25000       297     46487       394     14158    209779    312293  no matrixes
c  N st   101     26191       297     46487       394     15295    209779    341078  no matrixes
c  N dy   104     30194       297     46487       394     11343    209779    225664  no matrixes
c  S st   104     30194       297     46487       394     11343    209779    225664  no matrixes
c  S st   104     30699       297     46487       394     11838    209779    238150  no matrixes
c x-sub:     0 x-cut:      0 vfix:      0 v-elim:      0 locsubst:     0 time:   0.01
c binary deg approx. time:  0.00 s num checked:    457 i:     891 props:   64k
c Flit:     1 Blit:      0 bXBeca:    0 bXProp:    0 Bins:      0 P:  0.4M T:  0.02
c Time to fill non-learnt binary watchlists: 0.00 s
c subs with bin:        0  lits-rem:         0  v-fix:    0  time:  0.02 s
c Subs w/ non-existent bins:      0 l-rem:      0 v-fix:     0 done:    296 time:  0.02 s
c Removed useless bin:       0  fixed:     0  props:   0.00M  time:  0.00 s
c lits-rem:         0  cl-subs:       11  v-elim:      0  v-fix:    0  time:  0.93 s
c lits-rem:        75  cl-subs:      937  v-elim:      0  v-fix:    0  time:  1.64 s
c asymm  cl-useful: 2/46476/46476 lits-rem:2 time: 1.35
c watched sorting time: 0.00
c  N dy   104     30699       296     46476        98     10901    208910    225861  no matrixes
c  N dy   121     34868       296     46476        98     14898    208910    318032  no matrixes
Cannot allocate enough memory!
     \endverbatim
     This happens reproducibly on csltok (a 64-bit machine with 4 GB). </li>
     <li> We need to contact the author (then we can also mention that it is
     "matrices" not "matrixes"). </li>
     <li> "valgrind --track-origins=yes cryptominisat VanDerWaerden_pd_2-3-24_593.cnf" shows errors:
     \verbatim
==23882== Conditional jump or move depends on uninitialised value(s)
==23882==    at 0x435246: ClauseAllocator::consolidate(Solver*) (ClauseAllocator.cpp:381)
==23882==    by 0x4129CA: Solver::new_decision(int const&, int const&, int&) (Solver.cpp:1915)
==23882==    by 0x412C93: Solver::search(int, int, bool) (Solver.cpp:1844)
==23882==    by 0x413B89: Solver::solve(vec<Lit> const&) (Solver.cpp:2403)
==23882==    by 0x4067D4: main (Solver.h:683)
==23882==  Uninitialised value was created by a heap allocation
==23882==    at 0x4C26C3A: malloc (in /usr/lib64/valgrind/vgpreload_memcheck-amd64-linux.so)
==23882==    by 0x4C26CB4: realloc (in /usr/lib64/valgrind/vgpreload_memcheck-amd64-linux.so)
==23882==    by 0x435053: ClauseAllocator::consolidate(Solver*) (Vec.h:89)
==23882==    by 0x4129CA: Solver::new_decision(int const&, int const&, int&) (Solver.cpp:1915)
==23882==    by 0x412C93: Solver::search(int, int, bool) (Solver.cpp:1844)
==23882==    by 0x413B89: Solver::solve(vec<Lit> const&) (Solver.cpp:2403)
==23882==    by 0x4067D4: main (Solver.h:683)
==23882==
==23882== Conditional jump or move depends on uninitialised value(s)
==23882==    at 0x43569F: ClauseAllocator::consolidate(Solver*) (ClauseAllocator.cpp:430)
==23882==    by 0x4129CA: Solver::new_decision(int const&, int const&, int&) (Solver.cpp:1915)
==23882==    by 0x412C93: Solver::search(int, int, bool) (Solver.cpp:1844)
==23882==    by 0x413B89: Solver::solve(vec<Lit> const&) (Solver.cpp:2403)
==23882==    by 0x4067D4: main (Solver.h:683)
==23882==  Uninitialised value was created by a heap allocation
==23882==    at 0x4C26C3A: malloc (in /usr/lib64/valgrind/vgpreload_memcheck-amd64-linux.so)
==23882==    by 0x4C26CB4: realloc (in /usr/lib64/valgrind/vgpreload_memcheck-amd64-linux.so)
==23882==    by 0x435053: ClauseAllocator::consolidate(Solver*) (Vec.h:89)
==23882==    by 0x4129CA: Solver::new_decision(int const&, int const&, int&) (Solver.cpp:1915)
==23882==    by 0x412C93: Solver::search(int, int, bool) (Solver.cpp:1844)
==23882==    by 0x413B89: Solver::solve(vec<Lit> const&) (Solver.cpp:2403)
==23882==    by 0x4067D4: main (Solver.h:683)
     \endverbatim
     </li>
     <li> Also with a newer version (from the Git repository, ID
     50b2e0dbffb980d3b802a2ceb694aad06e324919) we get an error:
     \verbatim
> valgrind --track-origins=yes cryptominisat VanDerWaerden_pd_2-3-24_593.cnf
...
==9178== Conditional jump or move depends on uninitialised value(s)
==9178==    at 0x446DC5: Subsumer::blockedClauseRemoval() (Subsumer.cpp:2097)
==9178==    by 0x44BFB2: Subsumer::simplifyBySubsumption(bool) (Subsumer.cpp:1296)
==9178==    by 0x4237E5: Solver::performStepsBeforeSolve() (Solver.cpp:2616)
==9178==    by 0x42DE0A: Solver::solve(vec<Lit> const&, int, int) (Solver.cpp:2713)
==9178==    by 0x468616: _ZN8MTSolver5solveERK3vecI3LitE.omp_fn.0 (MTSolver.cpp:93)
==9178==    by 0x4688D0: MTSolver::solve(vec<Lit> const&) (MTSolver.cpp:86)
==9178==    by 0x405FA4: Main::solve() (MTSolver.h:201)
==9178==    by 0x4064CC: main (Main.cpp:771)
==9178==  Uninitialised value was created by a heap allocation
==9178==    at 0x4C267D0: operator new(unsigned long) (in /usr/lib64/valgrind/vgpreload_memcheck-amd64-linux.so)
==9178==    by 0x42918C: Solver::Solver(SolverConf const&, GaussConf const&, SharedData*) (Solver.cpp:117)
==9178==    by 0x467B49: MTSolver::setupOneSolver(int) (MTSolver.cpp:73)
==9178==    by 0x4684D1: MTSolver::MTSolver(int, SolverConf const&, GaussConf const&) (MTSolver.cpp:44)
==9178==    by 0x405E90: Main::solve() (Main.cpp:699)
==9178==    by 0x4064CC: main (Main.cpp:771)
     \endverbatim
     </li>
    </ol>
   </li>
   <li> DONE
   The newer versions don't use Boost anymore. </li>
   <li> Wrong version number reporting:
    <ol>
     <li> We still get "This is CryptoMiniSat 2.7.0". </li>
     <li> We should report this to the author. </li>
     <li> Actually, on csltok we get "This is CryptoMiniSat 2.7.1". </li>
     <li> Was there an update (without changing the version number)? </li>
     <li> Or has this to do with the versions of the build tools? </li>
    </ol>
   </li>
   <li> DONE Can be installed like so:
   \verbatim
builds/CryptoMiniSat> tar xvf ../../sources/SAT/CryptoMiniSat/cryptominisat-2.7.1.tar.gz
builds/CryptoMiniSat> cd cryptominisat-2.7.1/
builds/CryptoMiniSat> ./configure
builds/CryptoMiniSat> make
   \endverbatim
   yielding a binary "cryptominisat" in the same directory.
   </li>
  </ul>


  \todo Documentation for CryptoMiniSat
  <ul>
   <li> Available at http://www.msoos.org/cryptominisat2 and the winner
   of the SAT-Race 2010 (see http://baldur.iti.uka.de/sat-race-2010/).
   </li>
   <li> The Git-repository is
   http://gitorious.org/cryptominisat/cryptominisat/. </li>
   <li> Installing newer versions, directly from the Git repository:
    <ol>
     <li> To create the package, newer versions of the auto-tools are needed.
     </li>
     <li> Creation of e.g. version 2.7.1.1:
     \verbatim
> cd OKplatform/ExternalSources/sources/SAT/CryptoMiniSat
> git clone $(cryptominisat_git_url_okl)
> cd cryptominisat
> make -f Makefile.cvs
> cd ..
> new_version="2.7.1.1"; mv cryptominisat cryptominisat-${new_version}; tar -cjf cryptominisat-${new_version}.tar.bz2 cryptominisat-${new_version}
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Glucose
  <ul>
   <li> http://www.lri.fr/~simon/glucose/ </li>
   <li> We need to install version 2.
    <ol>
     <li> DONE (created archive following our naming conventions out of this
     archive)
     \verbatim
3ea28a75b7a079a4415ea594931b26fb  glucose-2-compet.tgz
     \endverbatim
     </li>
     <li> DONE (same build, but now 64-bit)
     We should check whether the defincencies of the build process are
     still there (then likely we can just use the old build) or not (then
     we should use some standard form of build). </li>
     <li> DONE (compile the old version by using
     "glucose_recommended_version_number_okl=1.0")
     First we also provide the old version and see how the new behaves. </li>
     <li> But likely, since in none of our applications, glucose version 1
     was best, it replaces version 1. </li>
     <li> Of course, we keep the old sources (including the md5sum in
     Buildsystem/ExternalSources/sources). </li>
    </ol>
   </li>
   <li> Correct compilation: Does the static glucose-executable link to the
   *local* libstdc++ and libgcc ? </li>
   <li> Handling satelite:
    <ol>
     <li> The glucose-build also provides the preprocessor satelite. </li>
     <li> We want to have this. </li>
     <li> For its compilation still "-m32" is used. </li>
     <li> So 32-bit versions of libstdc++ and libgcc are needed. </li>
     <li> However we do not provide them. </li>
     <li> So building of gcc needs to be enhanced, so that also those
     additional libraries are provided. </li>
     <li> And we need to make sure (in a systematic way) that these libraries
     are always used (and not the system-libraries). </li>
    </ol>
   </li>
   <li> DONE (version 1 installed)
   Install by:
   \verbatim
builds/Glucose> tar xvf ../../sources/SAT/Glucose/glucose_1.0.tar.bz2
builds/Glucose> cd glucose_1.0
builds/Glucose> ./build.sh
   \endverbatim
   </li>
   <li> We need to create a docus-file. </li>
  </ul>


  \todo SAT4J
  <ul>
   <li> http://www.sat4j.org/ </li>
   <li> The current version is 2.3.1. </li>
   <li> For documentation on building Sat4J from source, see
   http://forge.ow2.org/project/showfiles.php?group_id=228 and
   http://download.forge.objectweb.org/sat4j/getting-started-sat4j-0.3.pdf .
   </li>
   <li> ??? This should use *our* resource for SAT4J (as usual)!
   And we never build from some repository, except for special cases. ???
   Sat4j builds with gcj-4.5.2 (system gcj on cspcmg):
   \verbatim
builds/SAT/Sat4J> svn checkout svn://svn.forge.objectweb.org/svnroot/sat4j/maven/trunk
builds/SAT/Sat4J> cd trunk
builds/SAT/Sat4J/trunk> ant
builds/SAT/Sat4J/trunk> echo "p cnf 1 2
1 -2 0
-1 2 0
" > test.cnf
builds/SAT/Sat4J/trunk> java -jar dist/CUSTOM/org.sat4j.sat.jar test.cnf
c SAT4J: a SATisfiability library for Java (c) 2004-2010 Daniel Le Berre
c This is free software under the dual EPL/GNU LGPL licenses.
c See www.sat4j.org for details.
c This software uses some libraries from the Jakarta Commons project. See jakarta.apache.org for details.
c version CUSTOM.v20111019
c java.runtime.name	null
c java.vm.name		GNU libgcj
c java.vm.version	4.5.2
c java.vm.vendor	Free Software Foundation, Inc.
c sun.arch.data.model	null
c java.version		1.5.0
c os.name		Linux
c os.version		2.6.38-11-generic
c os.arch		x86_64
<snip>
s SATISFIABLE
v -1 -2 0
c Total wall clock time (in seconds) : 0.006
   \endverbatim
   </li>
   <li> DONE (ant is like make, and just to be added to the system-software
   requirements)
   The SAT4J build system uses "ant". Likely we need to build and install
   ant as well as gcj in ExternalSources. </li>
   <li> Once we have ant, how to tell tell it to build using a specific gcj
   executable? </li>
   <li> DONE (edit the build.xml file)
   Sat4j builds with gcj, however, a library from Eclipse is
   apparently needed during the build, as otherwise we get
   \verbatim
builds/SAT/Sat4J> svn checkout svn://svn.forge.objectweb.org/svnroot/sat4j/maven/trunk
builds/SAT/Sat4J> ant
BUILD FAILED
/home/csmg/Apps/trunk/build.xml:186: The following error occurred while executing this line:
/home/csmg/Apps/trunk/build.xml:291: Class not found: org.eclipse.jdt.core.JDTCompilerAdapter

Total time: 1 second
   \endverbatim
   However, using the "ecj-3.6.1.jar" (the Eclipse compiler library -
   http://download.eclipse.org/eclipse/downloads/drops/R-3.6.1-201009090800/index.php),
   we can then compile this like so -
   \verbatim
builds/SAT/Sat4J> svn checkout svn://svn.forge.objectweb.org/svnroot/sat4j/maven/trunk
builds/SAT/Sat4J> wget "http://www.eclipse.org/downloads/download.php?file=/eclipse/downloads/drops/R-3.6.1-201009090800/ecj-3.6.1.jar&url=http://www.mirrorservice.org/sites/download.eclipse.org/eclipseMirror/eclipse/downloads/drops/R-3.6.1-201009090800/ecj-3.6.1.jar&mirror_id=96" -O ecj-3.6.1.jar
builds/SAT/Sat4J> ant -lib ecj-3.6.1.jar
   \endverbatim
   </li>
   <li> DONE (edit the build.xml file)
   However, we then need to build ecj. The sources are available on the
   site, but Java 1.6 is required to build it and gcj only supports up to
   Java 1.5 -
   \verbatim
builds/SAT/Sat4J> mkdir ecj; cd ecj
builds/SAT/Sat4J/ecj> wget "http://www.eclipse.org/downloads/download.php?file=/eclipse/downloads/drops/R-3.6.1-201009090800/ecjsrc-3.6.1.zip&url=http://www.mirrorservice.org/sites/download.eclipse.org/eclipseMirror/eclipse/downloads/drops/R-3.6.1-201009090800/ecjsrc-3.6.1.zip&mirror_id=96" -O ecjsrc-3.6.1.zip
builds/SAT/Sat4J/ecj> unzip ecjsrc-3.6.1.zip
builds/SAT/Sat4J/ecj> ant
    [javac] Compiling 53 source files to /home/csmg/Apps/trunk/ecj/bin
    [javac] Compliance level '1.5' is incompatible with source level '1.6'. A compliance level '1.6' or better is required

BUILD FAILED
/home/csmg/Apps/trunk/ecj/build.xml:35: Compile failed; see the compiler error output for details.

   \endverbatim
   </li>
   <li> DONE (edit the build.xml file)
   Older versions of ecj are available, so we should try one of these.
   </li>
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
   <li> http://www.cs.chalmers.se/Cs/Research/FormalMethods/MiniSat/ </li>
   <li> The following needs to be transferred to the installation-docus. </li>
   <li> DONE (we have provided a patch)
   Conflict-count-output only 32-bit int with minisat-2.2.0:
    <ol>
     <li> For example with VanDerWaerden_pd_2-3-25_608.cnf we get
     \verbatim
| 1474310757 |     304    49427   225828 |   677887    58975     32 |  0.002 % |
| -2083501051 |     304    49427   225828 |   745676   545806     36 |  0.003 % |
| -977767819 |     304    49427   225828 |   820244   159204     38 |  0.001 % |
     \endverbatim
     </li>
     <li> This is just an output bug! </li>
     <li> We should tell the minisat-developers, but we can also do it
     ourselves. </li>
    </ol>
   </li>
   <li> DONE (we have provided a patch)
   minisat-2.2.0 does not flush output: This should be easy to fix. </li>
   <li> More information:
    <ol>
     <li> The option "-verb=2" doesn't seem interesting (only information on
     garbage collection). </li>
     <li> The stable factor for reporting conflicts seems to be 1.5. </li>
     <li> Perhaps we should make that smaller, say 1.2. </li>
     <li> The general information ("Problem Statistics", including info on
     garbage collection) is no longer exactly aligned with the "Search
     Statistics" (due to the conflict-count using one more space). It seems
     to OK that we could keep that, as a sign of our modifications. </li>
    </ol>
   </li>
   <li> minisat2 doesn't return the DIMACS-return-code:
    <ol>
     <li> However for example in VdWTransversalsInc we used minisat2 in the
     ordinary way. </li>
     <li> So that seems to be a bug of minisat2, occurring on newer platforms.
     </li>
     <li> We could wrap minisat2, or just stop using it. </li>
    </ol>
   </li>
   <li> See also Satisfiability/Solvers/Minisat/Minisat2/plans/general.hpp
   for further plans on improving minisat. </li>
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
       <li> The input format is the (now old) OPB format.
       See PB 2005 at http://www.cril.univ-artois.fr/PB05/solver_req.html.
       </li>
       <li> It seems only lines of the form
       \verbatim
+1*v1 +2*v4 -3*v11 >= 4;
       \endverbatim
       are allowed (no spaces after the semicolon!). </li>
       <li> Relations are "<=, >=, =". </li>
       <li> Variable names are identifiers, and start with a letter. </li>
       <li> The "+"-symbols are not needed. </li>
       <li> Comment lines start with "*". </li>
       <li> In more recent pseudo-boolean competitions (PB06 onwards)
       the "*" symbol in-between a variable and its weight was removed.
       See "minisat+ input format" in
       Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp.
       </li>
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


  \todo Experiment monitoring tools
  <ul>
   <li> When running SAT solvers on a non-trivial instance, one often wishes
   to place a time limit on the computation, after which we try another solver
   or try to solve some other instance. </li>
   <li> It is also useful, given the use of shared machines (such as in a
   university lab) to place constraints on the memory usage of the solver to
   avoid the solver taking over the machine. </li>
   <li> Therefore it makes sense to consider tools for running SAT solvers
   with given time and space limits:
   <ul>
    <li> run is a tool for running solvers (or any program) and monitoring
    and limiting the resource usage of the solver.
     <ul>
      <li> Available at http://fmv.jku.at/run/index.html. </li>
      <li> Installation:
      \verbatim
builds/> mkdir Run && cd Run
builds/Run> tar zxvf ../../../sources/SAT/Run/run-1.4.tar.gz
builds/Run> cd run-1.4
builds/Run/run-1.4> ./configure
builds/Run/run-1.4> make
      \endverbatim
      </li>
      <li> Allows one to specify time, real time and memory limits:
      \verbatim
$OKPLATFORM/ExternalSources/builds/Run/run-1.4/run -h
usage: run [option ...] program [arg ...]

  where option is from the following list:

    -h                         print this command line summary
    --help

    --version                  print version number

    -o <file>                  overwrite or create <file> for logging
    --output-file=<file>

    --space-limit=<number>     set space limit to <number> MB
    -s <number>

    --time-limit=<number>      set time limit to <number> seconds
    -t <number>

    --real-time-limit=<number> set real time limit to <number> seconds
    -r <number>

    -k|--kill                  propagate signals

The program is the name of an executable followed by its arguments.
      \endverbatim
      </li>
      <li> Specifying an output file with "-o" causes run to output
      various statistics detailing the resource usage and result of
      the solver:
      \verbatim
shell> $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/generate_aes_experiment 5 1 4 2 4 false aes_small_box aes_mc_forward
shell> cd ssaes_r1-5_c4_rw2_e4_f0_k1-1_aes_ts_box_aes_mc_forward
shell> AppendDimacs-O3-DNDEBUG ssaes_r3_c4_rw2_e4_f0.cnf ssaes_pcpair_r3_c4_rw2_e4_f0_s1.cnf  > experiment_r3_k1.cnf
shell> $OKPLATFORM/ExternalSources/builds/Run/run-1.4/run -o experiment_r3_k1.cnf_cryptominisat --time-limit=20 cryptominisat experiment_r3_k1.cnf
shell> cat experiment_r3_k1.cnf_cryptominisat
[run] time limit:	20 seconds
[run] real time limit:	311040000 seconds
[run] space limit:	7873 MB
[run] argv[0]:		cryptominisat
[run] argv[1]:		experiment_r3_k1.cnf
[run] start:		Wed Feb 23 13:44:39 2011
[run] main pid:	5102
[run] sample:		1.0 seconds, 5.6 MB
[run] sample:		2.0 seconds, 11.2 MB
[run] sample:		3.0 seconds, 10.9 MB
[run] sample:		4.0 seconds, 11.2 MB
[run] sample:		5.0 seconds, 12.0 MB
[run] sample:		6.0 seconds, 12.0 MB
[run] sample:		7.0 seconds, 11.9 MB
[run] sample:		8.0 seconds, 19.1 MB
[run] sample:		9.0 seconds, 18.6 MB
[run] sample:		10.0 seconds, 18.1 MB
[run] sample:		11.0 seconds, 19.1 MB
[run] sample:		12.0 seconds, 18.7 MB
[run] sample:		13.0 seconds, 24.1 MB
[run] sample:		14.0 seconds, 24.3 MB
[run] sample:		15.0 seconds, 22.8 MB
[run] sample:		16.0 seconds, 23.9 MB
[run] sample:		17.0 seconds, 24.6 MB
[run] sample:		18.0 seconds, 23.7 MB
[run] sample:		19.0 seconds, 24.4 MB
[run] sample:		20.0 seconds, 23.5 MB
[run] end:		Wed Feb 23 13:44:59 2011
[run] status:		out of time
[run] result:		1
[run] children:		0
[run] real:		20.38 seconds
[run] time:		20.09 seconds
[run] space:		25.1 MB
[run] samples:		201
      \endverbatim
      </li>
     </ul>
    </li>
   </ul>
   </li>
  </ul>


  \todo Argo
  <ul>
   <li> DONE (as stated)
   Create new release:
    <ol>
     <li> Using the repository-state 5ecd673e20cfd35a85012b1a7c25b8c73b36c93c
     (branch bcs) from http://github.com/bsilverthorn/argosat.git (author
     Bryan Silverthorn), create a release with version-number 1.0.1. </li>
     <li> So the new package argosat-1.0.1.tar.gz is to be created from
     a clone, by renaming the directory to "argosat-1.0.1". </li>
    </ol>
   </li>
   <li> The old homepage http://argo.matf.bg.ac.yu/index.html seems no longer
   available, so perhaps for argosat_homepage_url_okl we should use
   http://argo.matf.bg.ac.rs/downloads/software/argosat-1.0.tar.gz ? </li>
   <li> Development:
    <ol>
     <li> Is this software maintained? It seems that since version 1.0 no
     further developments took place, while for example documentation is
     missing? </li>
     <li> There are missing includes of cstdlib and cstring, and "rand"
     and "strcmp" are used without "std::" qualifier. </li>
     <li> At http://github.com/bsilverthorn/argosat one finds the Git
     repository http://github.com/bsilverthorn/argosat.git. </li>
     <li> This contains the addition of the missing includes. </li>
     <li> The (incorrect) use of the unqualified names "rand, strcmp" is
     not detected by gcc 4.5.2, so we ignore this problem for now. </li>
     <li> But at some point we should create a fork on Github and a submodule
     in the OKlibrary, and make further improvements to the code. </li>
    </ol>
   </li>
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
    <ul>
     <li> Installation:
     \verbatim
builds/SAT/Satz> tar -xzf ../../../sources/SAT/Satz/ParaSatz.tar.gz
Satz> cd ParaSatz
     \endverbatim
     This is mainly a script to compile and run Satz (214) in parallel. </li>
     <li> ParaSatz runs on any Linux or Unix machines in a local network with
     the shared home directory. </li>
     <li> ParaSatz can be called from an arbitray machine in the local network
     as the master, while all other machines run as slaves. </li>
     <li> A workload balancing mechanism is provided such that an idle slave
     always gets a remaining subtree from the most loaded slave. </li>
     <li> Machine names stored in file 'machine' end with '0'. </li>
     <li> Some scripts are provided for PS, Kill, Resume all ParaSatz threads.
     </li>
    </ul>
   </li>
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


  \todo DONE (no need to do something here; the number of unit-propagations
  is not interesting, and the extraction tools shall just return 0 for
  numbers of conflicts etc.)
  Correct solvers with poor handling of trivial CNFs
  <ul>
   <li> For the Dimacs file:
   \verbatim
p cnf 1 2
1 0
-1 0
   \endverbatim
   glucose and minisat2 return:
   \verbatim
c This is glucose 1.0 --  based on MiniSAT (Many thanks to MiniSAT team)

c ============================[ Problem Statistics ]=============================
c |                                                                             |
c |  Number of variables:  1                                                    |
c |  Number of clauses:    2                                                    |
c |  Parsing time:         0.00         s                                       |
c Solved by unit propagation
s UNSATISFIABLE
   \endverbatim
   </li>
   <li> If solved by UCP, the solvers should at least return the number
   of propagations! </li>
   <li> The solvers should return full statistics. </li>
   <li> Without full statistics, properly extracting statistics from
   solver runs becomes more difficult. </li>
   <li> We should contact the authors. </li>
  </ul>


  \bug DONE (now we offer a 32-bit version as well)
  Glucose build error
  <ul>
   <li> On csltok we get
   \verbatim
/home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.5.2/bin/g++ Main.or Solver.or -m32 --static -lz -o glucose_static
/usr/local/bin/ld: cannot find -lz
collect2: ld returned 1 exit status
   \endverbatim
   </li>
   <li> "-lz" refers to zlib, which we also build, however linking doesn't
   work:
   \verbatim
/home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.5.2/bin/g++ -I/home/kullmann/OKplatform/ExternalSources/Installations/Zlib/4.5.2/1.2.5/include -L/home/kullmann/OKplatform/ExternalSources/Installations/Zlib/4.5.2/1.2.5/lib -Wl,-rpath,/home/kullmann/OKplatform/ExternalSources/Installations/Zlib/4.5.2/1.2.5/lib Main.or Solver.or -m32 --static -lz -o glucose_static
/usr/local/bin/ld: skipping incompatible /home/kullmann/OKplatform/ExternalSources/Installations/Zlib/4.5.2/1.2.5/lib/libz.a when searching for -lz
/usr/local/bin/ld: cannot find -lz
collect2: ld returned 1 exit status
   \endverbatim
   </li>
   <li> We apparently have the latest version of zlib. Do we need a
   "multilib"-version (there doesn't seem to be one)? </li>
   <li> Compiling zlib with CFLAGS="-m32" makes this error go away, and so it
   seems a multilib issue, but then if we use this then we will likely get
   other errors in other packages that use zlib. </li>
   <li> DONE Perhaps we should offer a 64-bit and 32-bit zlib? </li>
  </ul>


  \bug DONE (repackaged glucose correctly)
  False Glucose build
  <ul>
   <li> The directory naming-conventions do not follow our standards. </li>
   <li> In particular glucose uses "_1.0" in its version number, rather than
   "_1.0". </li>
  </ul>

*/

