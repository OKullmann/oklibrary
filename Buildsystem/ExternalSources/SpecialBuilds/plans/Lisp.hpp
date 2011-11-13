// Oliver Kullmann, 30.8.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Lisp.hpp
  \brief Plans regarding installation of Lisp compiler/interpreters othen then Ecl (the current default)


  \todo Installing and using Sbcl
  <ul>
   <li> http://www.sbcl.org/ </li>
   <li> Installation:
   \verbatim
SBCL> md5sum sbcl-1.0.52-source.tar.bz2
e9e186637155184ec662b5a37634b457  sbcl-1.0.52-source.tar.bz2
> tar -xjf ../../sources/SBCL/sbcl-1.0.52-source.tar.bz2
> cd ExternalSources/builds/Sbcl/sbcl-1.0.52
> sh make.sh "clisp"
Bye.
//testing for consistency of first and second GENESIS passes
//header files match between first and second GENESIS -- good
real    40m15.446s
user    39m16.092s
sys     0m51.738s
//entering make-target-2.sh
//doing warm init - compilation phase
mmap: Cannot allocate memory
ensure_space: failed to validate 8589869056 bytes at 0x1000000000
(hint: Try "ulimit -a"; maybe you should increase memory limits.)

sbcl-1.0.52> ulimit -a
core file size          (blocks, -c) unlimited
data seg size           (kbytes, -d) unlimited
scheduling priority             (-e) 0
file size               (blocks, -f) unlimited
pending signals                 (-i) 30378
max locked memory       (kbytes, -l) 64
max memory size         (kbytes, -m) 3329364
open files                      (-n) 1024
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
real-time priority              (-r) 0
stack size              (kbytes, -s) 8192
cpu time               (seconds, -t) unlimited
max user processes              (-u) 30378
virtual memory          (kbytes, -v) 4821120
file locks                      (-x) unlimited

Sbcl> md5sum sbcl-1.0.53-source.tar.bz2
28bdb8d65b240bcc45370f19b781f9b8  sbcl-1.0.53-source.tar.bz2
> tar -xjf sbcl-1.0.53-source.tar.bz2
> cd sbcl-1.0.53/
> sh make.sh "clisp"
...
Bye.
//testing for consistency of first and second GENESIS passes
//header files match between first and second GENESIS -- good

real    32m36.069s
user    31m49.528s
sys     0m41.529s
//entering make-target-2.sh
//doing warm init - compilation phase
mmap: Cannot allocate memory
ensure_space: failed to validate 8589869056 bytes at 0x1000000000
(hint: Try "ulimit -a"; maybe you should increase memory limits.)
   \endverbatim
   That error doesn't make sense. Contacted sbcl-bugs@lists.sourceforge.net.
   According to the Maxima mailing-list this is a Suse-specific problem,
   which can be suppressed as follows, and then the build succeeds:
   \verbatim
> ulimit -v unlimited
> sh make.sh "clisp"
> cd tests/
> sh ./run-tests.sh
> cd ..

# INSTALLING:
> chmod u+x install.sh
> INSTALL_ROOT=/home/kullmann/OKplatform/ExternalSources/builds/Sbcl/Install ./install.sh

# BUILDING MAXIMA:
ExternalSources> ulimit -v unlimited
ExternalSources> export SBCL_HOME=/home/kullmann/OKplatform/ExternalSources/builds/Sbcl/Install/lib/sbcl; sbcl_call_okl="/home/kullmann/OKplatform/ExternalSources/builds/Sbcl/Install/bin/sbcl" maxima_lisp_name_okl=sbcl oklib maxima

# USING MAXIMA:
OKsystem> ulimit -v unlimited; maxima_lisp_name_okl=sbcl time oklib maxima_test
   \endverbatim
   </li>
   <li> logxor does not work under Sbcl; the Maxima mailing list has been
   contacted (12.11.2011). </li>
   <li> Speed comparison with Ecl:
    <ol>
     <li> Loading files under Sbcl is considerably slower (say 50%). </li>
     <li> arithprog_hg(5,1000)$ takes from 9.4s to 10.1s under Ecl, and 6.9s
     under Sbcl. </li>
     <li>
     \verbatim
(%i5) G66 : arithprog_hg(6,1132)$
Evaluation took 11.7490 seconds (11.8160 elapsed)
(%i6) f2 : mirrorfold(2,1132)$
Evaluation took 0.0010 seconds (0.0000 elapsed)
(%i7) G66f2 : transport_hg(f2,G66)$
Evaluation took 108.8940 seconds (109.2330 elapsed)
(%i8) ncl_list_fcs(G66f2);
Evaluation took 1.4190 seconds (1.4210 elapsed)
(%i9) G66f2m : min_hg(G66f2)$
Evaluation took 28.0670 seconds (28.1400 elapsed)
(%i10) ncl_list_fcs(G66f2m);
Evaluation took 1.3740 seconds (1.3780 elapsed)
(%i11) F66f2 : tcol2sat_stdhg2stdfcs(G66f2m)$
Evaluation took 3.7610 seconds (3.7690 elapsed)
(%i12) outputext_fcs("transport_hg(mirrorfold(2,1132),arithprog_hg(6,1132))",F66f2,"VanDerWaerden_f2_1132.cnf");
Evaluation took 37.5540 seconds (38.3150 elapsed)

G66 : arithprog_hg(6,1132)$
Evaluation took 8.0400 seconds (8.0580 elapsed) using 363.612 MB.
(%i3) f2 : mirrorfold(2,1132)$
Evaluation took 0.0000 seconds (0.0000 elapsed) using 32.000 KB.
(%i4) G66f2 : transport_hg(f2,G66)$
Evaluation took 72.7520 seconds (72.8920 elapsed) using 2145.035 MB.
(%i5) ncl_list_fcs(G66f2);
Evaluation took 1.2870 seconds (1.2900 elapsed) using 31.595 MB.
(%o5) [[3,170],[4,175],[5,157],[6,47342]]
(%i6) G66f2m : min_hg(G66f2)$
Evaluation took 16.5330 seconds (16.5650 elapsed) using 639.423 MB.
(%i7) ncl_list_fcs(G66f2m);
Evaluation took 1.2540 seconds (1.2560 elapsed) using 31.001 MB.
(%o7) [[3,170],[4,37],[6,46722]]
(%i8) F66f2 : tcol2sat_stdhg2stdfcs(G66f2m)$
Evaluation took 2.9250 seconds (2.9300 elapsed) using 58.645 MB.
(%i9) outputext_fcs("transport_hg(mirrorfold(2,1132),arithprog_hg(6,1132))",F66f2,"VanDerWaerden_f2_1132.cnf");
Evaluation took 27.6420 seconds (27.6960 elapsed) using 1211.097 MB.
     \endverbatim
     </li>
    </ol>
   </li>
   <li> DONE (according to Raymond Toy from the Maxima mailing list Sbcl could
   be faster, and so we should try it out)
   Anyway, since it uses CLisp, and they compare themselves with CLisp,
   it seems not faster than Ecl, and then we don't need to try it anyway.
   </li>
  </ul>


  \todo DONE (not possible, since binary is needed)
  Installing and using Cmucl
  <ul>
   <li> http://www.cons.org/cmucl/ </li>
   <li>
   \verbatim
Cmucl> tar -xjf cmucl-src-20c.tar.bz2
> BUILDING
says
In order to build CMU CL, you will need:
a) A working CMU CL binary.  There is no way around this requirement!
   \endverbatim
   and thus we can't use Cmucl. </li>
  </ul>


  \todo DONE (works now (but CLisp is very slow))
  Update to CLisp version 2.49
  <ul>
   <li> Installation works on cs-wsok and cs-oksvr. </li>
   <li> We must now generalise the Maxima-system, so that special Ecl-code
   is provided only when Ecl is used. See "Using CLisp"
   in MasterScript/SpecialProcessing/plans/Maxima_Init.hpp. </li>
  </ul>


  \todo Libffcall not installing on AB's 64-bit Xeon machine
  <ul>
   <li> With "oklib libffcall64" we just get
   \verbatim
gcc -g -O2 -x none minitests.o -o minitests  ./.libs/libavcall.a
./minitests > minitests.out
LC_ALL=C uniq -u < minitests.out > minitests.output.x86_64-unknown-linux-gnu
test '!' -s minitests.output.x86_64-unknown-linux-gnu
make[2]: *** [check] Error 1
make[2]: Leaving directory `/home/kullmann/OKplatform/ExternalSources/builds/CLisp/Libffcall/ffcall-1.10/avcall'
make[1]: *** [check] Error 2
make[1]: Leaving directory `/home/kullmann/OKplatform/ExternalSources/builds/CLisp/Libffcall/ffcall-1.10'
   \endverbatim
   which doesn't say at all what's wrong? </li>
   <li> Could it be a libc-problem? </li>
  </ul>


  \todo DONE (no longer in active development, and thus not considered anymore)
  Installing and using GCL
  <ul>
   <li> http://savannah.gnu.org/projects/gcl </li>
   <li>
   \verbatim
> tar -xzf gcl-2.6.7.tar.gz
> cd gcl-2.6.7
> ./configure
> make
/usr/lib64/libbfd.a(compress.o): In function `bfd_compress_section_contents':
/usr/src/packages/BUILD/binutils-2.21/build-dir/bfd/../../bfd/compress.c:100: undefined reference to `compressBound'
collect2: ld returned 1 exit status
make[1]: *** [raw_pre_gcl_map] Error 1
   \endverbatim
   </li>
   <li> One needed to get newer versions from their cvs-repository. </li>
  </ul>


  \todo Installing other Lisp's
  <ul>
   <li> We should summarise our experiences in the docus. </li>
   <li> DONE (can't be installed, since it needs a binary, which is not
   available with Linux distributions)
   CMUCL http://www.cons.org/cmucl/ looks reasonable. But apparently
   there are Maxima-problems with it? </li>
   <li> DONE (see above)
   SBCL http://www.sbcl.org/ looks alright (and maintained).
    <ol>
     <li> However, it needs another Lisp to be compiled! And none of the
     Lisps we provide is supported. </li>
     <li> So let's try the other's first. </li>
    </ol>
   </li>
   <li> DONE (see above)
   GCL http://savannah.gnu.org/projects/gcl seems a bit outdated, but
   we should try.
    <ol>
     <li> However, if the argument-length is really only 64, then it's
     not usable. </li>
    </ol>
   </li>
   <li> DONE
   CCL http://ccl.clozure.com/ also requires a binary, and thus we
   can't use it. </li>
  </ul>

*/

