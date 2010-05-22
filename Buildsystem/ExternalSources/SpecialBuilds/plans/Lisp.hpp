// Oliver Kullmann, 30.8.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Lisp.hpp
  \brief Plans regarding installation of Lisp compiler/interpreters othen then Ecl (the current default)


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


  \bug DONE
  CLisp doesn't use libsigsegv (though specified)
  <ul>
   <li> According to http://www.clisp.org/impnotes/faq.html#faq-with-prefix
   the build of libsigsegv has to be adopted on 64-bit machines by adding
   "--libdir=='${exec_prefix}/lib64'" as configure-parameter:
    <ol>
     <li> It also works just to rename the directory "lib" to "lib64". </li>
     <li> For that we need a way to figure out whether the platform is 32-bit
     or 64-bit.
      <ul>
       <li> Perhaps we just use a make-variable? </li>
       <li> But via "uname -i" we can get the information:
       \verbatim
> uname -i
x86_64
       \endverbatim
       Apparently we have just to extract the number after the underscore.
       </li>
       <li> Hm, on csltok (32 bit) I get "i386". This is the "hardware
       platform"? Perhaps "uname -m" (the machine type) is better, which
       on csltok yields "i686"? </li>
      </ul>
     </li>
     <li> Perhaps for 64-bit platforms we also need to distinguish between
     "bi-arch" and "mono-arch"? No, it should be just the case that
     the build of libsigsegv figures out that this is not a "pure"
     64-bit platform, and thus just uses "lib", while the build of clisp
     by some reasons insists to use "lib64". So if we are on 64-bit, then
     we just rename the libs for libsigsegv and libffcall to "lib64". </li>
     <li> The above faq-link contains more information. </li>
     <li> DONE (doesn't solve it; perhaps the problem is on the clisp-side)
     The new libsigsegv-version is 2.6 --- perhaps this solves the problem
     already? No, still the same problem. </li>
    </ol>
   </li>
   <li> DONE (Yes, also there the lib-directory has to be renamed to "lib64".)
   Isn't there a similar problem with libffcall ?  </li>
   <li> We should try Clisp 2.46 : DONE (also with 2.47 the same problem)
    <ol>
     <li> Seems alright on cs-wsok (warnings about non-portable linking between
     static and shared libraries, but uses the local libraries). </li>
     <li> Now needs to be tested on cs-oksvr: Same problem again! </li>
    </ol>
   </li>
   <li> DONE (Ecl is now the default)
   Thus apparently we need to use another Lisp; let's see whether Ecl
   installs properly on all our systems. </li>
  </ul>

*/

