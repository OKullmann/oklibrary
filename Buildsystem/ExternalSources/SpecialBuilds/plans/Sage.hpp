// Oliver Kullmann, 19.7.2007 (Swansea)
/* Copyright 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Sage.hpp
  \brief Plans regarding installation of Sage


  \todo Update to new Sage
  <ul>
   <li> Likely the failures below indicate missing "-fPIC"-options to gcc
   in the respective sub-builds; and perhaps this is corrected with
   version 4 of Sage. </li>
   <li> Building sage-4.2.1 from source on cspcmg (64 bit OpenSUSE 11.1) 
   works perfectly. MG: How is OK building the package? Is this using the 
   OKlib build system or just the standard Sage makefile for testing? OK:
   this is an inappropriate question, since building something in the
   OKlibrary means always using the buildsystem (if it is available, which
   is the case here). So apparently MG didn't use the buildsystem? Obviously
   this needs to be used; information on building outside the buildsystem
   is obviously useful in case of problems, but only for finding out
   about the problems. </li>
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
   <li> Now it is impossible to contact Sage without becoming
   a member of "google" --- since this is the same (likely worse) as
   "microsoft", I (OK) don't want to be a "member" of this organisation in
   any form. </li>
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
