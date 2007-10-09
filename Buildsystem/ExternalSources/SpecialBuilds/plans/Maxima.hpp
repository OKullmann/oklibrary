// Oliver Kullmann, 7.10.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp
  \brief Plans regarding installation of Maxima


  \todo Install Maxima
  <ul>
   <li> Maxima 5.13 seems appropriate.
    <ol>
     <li> First install clisp (should we supply this?). </li>
     <li>
     \verbatim
Installations> mkdir Maxima
builds> mkdir Maxima
Maxima> tar -xzf ../../sources/Maxima/maxima-5.13.0.tar.gz
Maxima> cd maxima-5.13.0/
maxima-5.13.0> LANG=C ../configure --prefix=${OKplatform}/ExternalSources/Installations/Maxima
maxima-5.13.0> LANG=C make
     \endverbatim
     yields
     \verbatim
Compiling file /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/Maxima/maxima-5.13.0/src/clmacs.lisp ...
*** - READ from
#<INPUT BUFFERED FILE-STREAM CHARACTER
  #P"/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/Maxima/maxima-5.13.0/src/clmacs.lisp"
  @317>: #<PACKAGE CUSTOM> has no external symbol with name "*FLOATING-POINT-RATIONAL-CONTAGION-ANSI*"
The following functions were used but not defined:
 MAXIMA::MFUNCALL
0 errors, 0 warnings
make[1]: *** [binary-clisp/maxima.mem] Error 1
make[1]: Leaving directory `/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/Maxima/maxima-5.13.0/src'
make: *** [all-recursive] Error 1
     \endverbatim
     ??? The same error without setting prefix.
     \verbatim
maxima-5.13.0> clisp --version
WARNING: *FOREIGN-ENCODING*: reset to ASCII
GNU CLISP 2.33.2 (2004-06-02) (built on g242.suse.de [10.10.103.242])
Software: GNU-C 3.3.4 (pre 3.3.5 20040809) ANSI-C-Programm
Features:
(CLOS LOOP COMPILER CLISP ANSI-CL COMMON-LISP LISP=CL INTERPRETER SOCKETS GENERIC-STREAMS
 LOGICAL-PATHNAMES SCREEN FFI GETTEXT UNICODE BASE-CHAR=CHARACTER PC386 UNIX)
Installation directory: /usr/lib/clisp/
User language: ENGLISH
Machine: I686 (I686) csltok.swan.ac.uk [128.163.146.167]
     \endverbatim
   </li>
   <li> The installation has a sub-target "access-script":
    <ol>
     <li> This Bash-script is copied to ~/bin (overwriting existing
     files), under the name "Maxima", and it calls the maxima-program,
     passing parameters and loading all functions from the OKlibrary. </li>
     <li> All path-information is hardcoded into this script, and it is
     created from a template with "m4_SHELL"-use. </li>
     <li> Perhaps a different script is used for performing tests. </li>
    </ol>
   </li>
   <li> Move all maxima-documention-links from the Sage-documentation
   page to the Maxima documentation. </li>
  </ul>

*/
