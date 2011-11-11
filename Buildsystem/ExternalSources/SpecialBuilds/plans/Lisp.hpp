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
   \endverbatim
   That error doesn't make sense. </li>
   <li> Anyway, since it uses CLisp, and they compare themselves with CLisp,
   it seems not faster than Ecl, and then we don't need to try it anyway.
   </li>
  </ul>


  \todo DONE (work now (but CLisp is very slow))
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


  \todo Installing other Lisp's
  <ul>
   <li> CMUCL http://www.cons.org/cmucl/ looks reasonable. But apparently
   there are Maxima-problems with it? </li>
   <li> SBCL http://www.sbcl.org/ looks alright (and maintained).
    <ol>
     <li> However, it needs another Lisp to be compiled! And none of the
     Lisps we provide is supported. </li>
     <li> So let's try the other's first. </li>
    </ol>
   </li>
   <li> GCL http://savannah.gnu.org/projects/gcl seems a bit outdated, but
   we should try.
    <ol>
     <li> However, if the argument-length is really only 64, then it's
     not usable. </li>
    </ol>
   </li>
  </ul>

*/

