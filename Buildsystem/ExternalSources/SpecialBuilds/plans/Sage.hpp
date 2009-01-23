// Oliver Kullmann, 19.7.2007 (Swansea)
/* Copyright 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Sage.hpp
  \brief Plans regarding installation of Sage


  \bug DONE
  We need to update the created sage-script
  <ul>
   <li> DONE Set SAGE_ROOT. </li>
   <li> What if "|" occurs in the path? </li>
   <li> Likely sed is used in other places, so we need to add it to the
   list of build-requirements. </li>
   <li> Perhaps we better use awk here, then we don't have problems:
   Replace every line starting with "SAGE_ROOT=" by our substitution. </li>
  </ul>


  \todo Update the Sage installation : DONE
  <ul>
   <li> DONE
   Update the makefile to the current standard. </li>
   <li> DONE
   Also Buildsystem/ExternalSources/SpecialBuilds/docus/Sage.hpp
   needed update of the texts. </li>
   <li> DONE
   Likely we should remove all links to documentation etc. which are
   not related to Sage itself. </li>
  </ul>


  \todo Update to Sage 3.2.3
  <ul>
   <li> On cs-wsok we get a build-error:
   \verbatim
usr/local/lib/../lib/libstdc++.so: could not read symbols: File in wrong format
collect2: ld returned 1 exit status
make[4]: *** [libfplll.la] Error 1
make[4]: Leaving directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-3.2.3/spkg/build/libfplll-2.1.6-20071129.p5/src'
make[3]: *** [all-recursive] Error 1
make[3]: Leaving directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Sage/sage-3.2.3/spkg/build/libfplll-2.1.6-20071129.p5/src'
Error building libfplll

real    1m7.638s
user    0m14.141s
sys     0m5.712s
sage: An error occurred while installing libfplll-2.1.6-20071129.p5
   \endverbatim
   </li>
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
     <li> But no attempts to communicate with Sage developers, except,
     of course, there is a reasonable (e-mail, not mindless chatting)
     possibility on a publically available mailing list. </li>
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
