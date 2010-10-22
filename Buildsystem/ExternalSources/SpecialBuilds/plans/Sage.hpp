// Oliver Kullmann, 19.7.2007 (Swansea)
/* Copyright 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Sage.hpp
  \brief Plans regarding installation of Sage


  \todo Update to new Sage
  <ul>
   <li> Version 4.5.3:
    <ol>
     <li> On csltok (Suse 11.3, 64bit Intel i5) we get the error
     \verbatim
Thread model: posix
gcc version 4.5.0 20100604 [gcc-4_5-branch revision 160292] (SUSE Linux) 
****************************************************
bash: symbol lookup error: bash: undefined symbol: rl_filename_rewrite_hook

real    0m0.015s
user    0m0.001s
sys     0m0.003s
sage: An error occurred while installing sqlite-3.6.22
     \endverbatim
     </li>
     <li> On cs-wsok (Suse 10.0, 64bit AMD) we get the usual error
     \verbatim
make[3]: Leaving directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-4.5.3/spkg/build/libfplll-3.0.12.p1/src'
Error building libfplll
     \endverbatim
     </li>
     <li> It is quite unbelievable what a bad software this is --- on no
     system since version 2.7.2 was a build succesful! </li>
    </ol>
   </li>
   <li> Likely the failures below indicate missing "-fPIC"-options to gcc
   in the respective sub-builds; and perhaps this is corrected with
   version 4 of Sage. </li>
   <li> On cs-wsok we get a build-error:
   \verbatim
/usr/local/lib/../lib/libstdc++.so: could not read symbols: File in wrong format
collect2: ld returned 1 exit status
make[4]: *** [libfplll.la] Error 1
make[4]: Leaving directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-4.2.1/spkg/build/libfplll-3.0.12.p0/src'
make[3]: *** [all-recursive] Error 1
make[3]: Leaving directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-4.2.1/spkg/build/libfplll-3.0.12.p0/src'
Error building libfplll

real    0m48.806s
user    0m36.738s
sys     0m9.989s
sage: An error occurred while installing libfplll-3.0.12.p0
   \endverbatim
   </li>
   <li> Let's wait whether over time the situation improves. </li>
   <li> Still not corrected with Sage 4.2.1. </li>
   <li> Same with version 4.4.1:
   \verbatim
/bin/sh ./libtool --tag=CXX --mode=link g++  -fPIC -I/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-4.4.1/local/include/ -L/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-4.4.1/local/lib   -o libfplll.la -rpath /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-4.4.1/local/lib -version-info 1:0:1 dummy.lo -lgmp -lmpfr -lmpfr -lgmp -lmpfr -lgmp
g++ -shared -nostdlib /usr/lib/../lib64/crti.o /usr/local/lib/gcc/x86_64-unknown-linux-gnu/4.1.2/crtbeginS.o  .libs/dummy.o  -Wl,--rpath -Wl,/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-4.4.1/local/lib -Wl,--rpath -Wl,/usr/local/lib/../lib -Wl,--rpath -Wl,/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-4.4.1/local/lib -Wl,--rpath -Wl,/usr/local/lib/../lib -L/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-4.4.1/local/lib /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-4.4.1/local/lib/libmpfr.so /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-4.4.1/local/lib/libgmp.so -L/usr/local/lib/gcc/x86_64-unknown-linux-gnu/4.1.2 -L/usr/local/lib/gcc/x86_64-unknown-linux-gnu/4.1.2/../../../../x86_64-unknown-linux-gnu/lib -L/usr/local/lib/gcc/x86_64-unknown-linux-gnu/4.1.2/../../../../lib64 -L/lib/../lib64 -L/usr/lib/../lib64 /usr/local/lib/../lib/libstdc++.so -lm -lc -lgcc_s /usr/local/lib/gcc/x86_64-unknown-linux-gnu/4.1.2/crtendS.o /usr/lib/../lib64/crtn.o  -Wl,-soname -Wl,libfplll.so.0 -o .libs/libfplll.so.0.1.0
/usr/local/lib/../lib/libstdc++.so: could not read symbols: File in wrong format
collect2: ld returned 1 exit status
make[4]: *** [libfplll.la] Error 1
make[4]: Leaving directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-4.4.1/spkg/build/libfplll-3.0.12.p0/src'
   \endverbatim
   </li>
   <li> Now it is impossible to contact Sage without becoming
   a member of "google" --- since this is the same (likely worse) as
   "microsoft", I (OK) don't want to be a "member" of this organisation in
   any form. </li>
   <li> MG has sent a message to the list asking what the problem
   could be with the above error. </li>
   <li> So it seems best to drop support for Sage; or just use packages
   which install without further efforts.
    <ol>
     <li> Perhaps this is best: 2.7.2 is installing fine (apparently),
     and from time to time we can try to see whether the installation
     problems disappear or not with some newer version. </li>
     <li> Same error with 3.0, 2.11. </li>
    </ol>
   </li>
   <li> If we would continue with Sage, then we need to update the
   documentation files (apparently in the doc-subdirectory of the
   installation directory). </li>
   <li> Yet I have removed Sage from the list of external sources installed
   by "all"; if we include it again, then we have to add to README the
   following:
   \verbatim
 - GNU tar
  (check: run "tar --version")
 - GNU ranlib
  (check: run "ranlib --version")
 - perl
  (check: run "perl --version")
   \endverbatim
   since installing Sage needs these tools. </li>
  </ul>

*/
