// Oliver Kullmann, 30.8.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Lisp.hpp
  \brief Plans regarding installation of Lisp compiler/interpreters othen then Ecl (the current default)


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

