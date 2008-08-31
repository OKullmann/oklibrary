// Oliver Kullmann, 7.10.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp
  \brief Plans regarding installation of Maxima


  \todo Update to Maxima 5.16.x
  <ul>
   <li> With 5.16.3 now there appear no errors anymore. </li>
   <li> However, now the gf-package is very slow, such that
   "time oklib new_check" on cs-wsok goes up from
   \verbatim
real    4m1.167s
user    3m21.613s
sys     0m21.669s
   \endverbatim
   to
   \verbatim
real    5m18.519s
user    4m31.577s
sys     0m22.377s
   \endverbatim
   which seems solely due to the gf-package. </li>
   <li> Perhaps we should set "GF_IRREDUCIBILITY_CHECK : false".
    <ol>
     <li> With 5.15.0 the default value is "false". </li>
     <li> And actually it seems that whenever it is set to true, and
     gf-operations are performed, then it is reset to false!. This looks
     like a bug, which might be corrected in 5.16.3. </li>
    </ol>
   </li>
   <li> Perhaps also "largefield : false" should be used, but then we
   should not permanently reset the field.
    <ol>
     <li> With 5.15.0 the default value is "true". </li>
     <li> We should ask for the possibility to store and re-store actively
     the values computed by gf_set. </li>
     <li> Setting largefield to false results in 5.15.0 in the error
     "Use `fasttimes' only on CRE polynomials with same varlists" (when
     adding 0 to a polynomial). </li>
    </ol>
   </li>
  </ul>


  \todo Enabling use of ECL
  <ul>
   <li> okltest_set_sets_union (in
   ComputerAlgebra/Hypergraphs/Lisp/tests/SetSystems.mac) contains a testcase
   which shows a clear Maxima/Ecl bug.
    <ol>
     <li> However, this seems specific to 32-bit machines, since on cs-wsok
     it works:
     \verbatim
Maxima 5.16.3 http://maxima.sourceforge.net
Using Lisp ECL 0.9l (CVS 2008-06-19 17:09)
Distributed under the GNU Public License. See the file COPYING.
Dedicated to the memory of William Schelter.
The function bug_report() provides bug reporting information.
(%i1) apply(union,listify({{-6,-5,-4,-3,-2,-1},{-6,-5,-4,-3,-2,1},{-6,-5,-4,-3,-1,2},{-6,-5,-4,-3,1,2},{-6,-5,-4,-2,-1,3},{-6,-5,-4,-2,1,3},{-6,-5,-4,-1,2,3},{-6,-5,-4,1,2,3}, {-6,-5,-3,-2,-1,4},{-6,-5,-3,-2,1,4},{-6,-5,-3,-1,2,4},{-6,-5,-3,1,2,4},{-6,-5,-2,-1,3,4},{-6,-5,-2,1,3,4},{-6,-5,-1,2,3,4},{-6,-5,1,2,3,4},{-6,-4,-3,-2,-1,5},{-6,-4,-3,-2,1,5},{-6,-4,-3,-1,2,5},{-6,-4,-3,1,2,5},{-6,-4,-2,-1,3,5},{-6,-4,-2,1,3,5},{-6,-4,-1,2,3,5},{-6,-4,1,2,3,5},{-6,-3,-2,-1,4,5},{-6,-3,-2,1,4,5},{-6,-3,-1,2,4,5},{-6,-3,1,2,4,5},{-6,-2,-1,3,4,5},{-6,-2,1,3,4,5},{-6,-1,2,3,4,5},{-6,1,2,3,4,5},{-5,-4,-3,-2,-1,6},{-5,-4,-3,-2,1,6},{-5,-4,-3,-1,2,6},{-5,-4,-3,1,2,6},{-5,-4,-2,-1,3,6},{-5,-4,-2,1,3,6}, {-5,-4,-1,2,3,6},{-5,-4,1,2,3,6},{-5,-3,-2,-1,4,6},{-5,-3,-2,1,4,6},{-5,-3,-1,2,4,6},{-5,-3,1,2,4,6},{-5,-2,-1,3,4,6},{-5,-2,1,3,4,6},{-5,-1,2,3,4,6},{-5,1,2,3,4,6}, {-4,-3,-2,-1,5,6},{-4,-3,-2,1,5,6},{-4,-3,-1,2,5,6},{-4,-3,1,2,5,6},{-4,-2,-1,3,5,6},{-4,-3,-2,-1,5,6},{-4,-3,-2,1,5,6},{-4,-3,-1,2,5,6},{-4,-3,1,2,5,6},{-4,-2,-1,3,5,6},{-4,-2,1,3,5,6},{-4,-1,2,3,5,6},{-4,1,2,3,5,6},{-3,-2,-1,4,5,6},{-3,-2,1,4,5,6}, {-3,-1,2,4,5,6},{-3,1,2,4,5,6},{-2,-1,3,4,5,6},{-2,1,3,4,5,6},{-1,2,3,4,5,6},{1,2,3,4,5,6}}));
Evaluation took 0.0000 seconds (0.0030 elapsed)
(%o1) {-6,-5,-4,-3,-2,-1,1,2,3,4,5,6}
     \endverbatim
     </li>
     <li> While on csltok we get
     \verbatim
Function union expects a set, instead found union
 -- an error.  To debug this try debugmode(true);
     \endverbatim
     </li>
    </ol>
   </li>
   <li> DONE (for now our build system repairs the rmaxima-script)
   rmaxima:
    <ol>
     <li> rmaxima is broken since it just calls "maxima", i.e., it assumes
     system-wide installation.
     <li> So "maxima" in rmaxima has to be replaced by the full path. </li>
    </ol>
   </li>
   <li> DONE
   See bug "False assumption for testing hash-maps" in
   ComputerAlgebra/DataStructures/Lisp/plans/HashMaps.hpp. </li>
   <li> DONE (Maxima-bug was corrected)
   Building Maxima works now, but it seems that make_random_state
   is now broken (we get test failures for our tests)? </li>
  </ul>


  \todo Additional packages
  <ul>
   <li> Real algebraic geometry
    <ol>
     <li> The book [Algorithms in Real Algebraic Geometry; Basu, Pollack, Roy]
     is available at http://www.math.gatech.edu/~saugata/bpr-posted1.html.
     </li>
     <li> Also maxima-code for various chapters is available. </li>
    </ol>
   </li>
  </ul>


  \todo System-wide installation
  <ul>
   <li> The build-system should be extended, taking for example the
   Gmp-installation as an example, so that we can also produce/use in the
   standard way system-wide installations of Libsigsegv, CLisp and Maxima.
   </li>
  </ul>


  \todo Handling of redefined load-function
  <ul>
   <li> See "How to eliminate the annotation of lists" in
   ComputerAlgebra/plans/Maxima.hpp. </li>
   <li> And see the e-mail thread (May 2008)
   "how to stop the annotation of lists by filenames?" on the Maxima
   mailing list. </li>
   <li> We need documentation on this regarding the build-system:
    <ol>
     <li> We need to watch the Maxima distribution, whether the
     two functions involved, "load" and "add-lineinfo", ever are
     changed, and then we need to incorporate these changes. </li>
     <li> Hopefully, this is not a big thing, since these functions
     are rather small. </li>
     <li> The good point about this solution is its non-intrusiveness:
     The Maxima system doesn't know about these changes. </li>
    </ol>
   </li>
  </ul>


  \todo Install Maxima : DONE
  <ul>
   <li> Write configuration file for Maxima. DONE </li>
   <li> Write installation script for Maxima. DONE </li>
   <li> Provide links to the documentation for Maxima (including
   the maxima-book and the tutorial. DONE </li>
   <li> What about the documentation for Maxima provided with Sage? DONE
   (removed) </li>
   <li> Write docu for installation of Maxima. DONE </li>
   <li> Put information on the main-external-sources-page. DONE </li>
  </ul>


  \todo Install Maxima 5.13 on csltok : DONE (works now (with 5.14))
  <ul>
   <li> First install clisp (should we supply this?); see below. </li>
   <li>
   \verbatim
Installations> mkdir Maxima
builds> mkdir Maxima
Maxima> tar -xzf ../../sources/Maxima/maxima-5.13.0.tar.gz
Maxima> cd maxima-5.13.0/
maxima-5.13.0> LANG=C ./configure --prefix=${OKplatform}/ExternalSources/Installations/Maxima
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


  \todo DONE Install Maxima 5.13 on cs-wsok
  <ul>
   <li>
   \verbatim
Installations> mkdir Maxima
Installations> cd Maxima/
Maxima> tar -xzf ../../sources/Maxima/maxima-5.13.0.tar.gz
Maxima> cd maxima-5.13.0
maxima-5.13.0> ./configure --prefix=${OKplatform}/ExternalSources/Installations/Maxima
   \endverbatim
   yields, as above
   \verbatim
Compiling file /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Maxima/maxima-5.13.0/src/clmacs.lisp ...
*** - READ from
#<INPUT BUFFERED FILE-STREAM CHARACTER
  #P"/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Maxima/maxima-5.13.0/src/clmacs.lisp"
  @317>: #<PACKAGE CUSTOM> has no external symbol with name "*FLOATING-POINT-RATIONAL-CONTAGION-ANSI*"
The following functions were used but not defined:
 MAXIMA::MFUNCALL
0 errors, 0 warnings
make[1]: *** [binary-clisp/maxima.mem] Error 1
make[1]: Leaving directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Maxima/maxima-5.13.0/src'
make: *** [all-recursive] Error 1
   \endverbatim
   again with cslisp version 2.33.
   </li>
   <li> After installation of clisp version 2.43
   \verbatim
maxima-5.13.0> ./configure --prefix=$OKPLATFORM/ExternalSources/Installations/Maxima
maxima-5.13.0> make
maxima-5.13.0> make check
maxima-5.13.0> make install
   \endverbatim
   </li>
   <li> Now under Maxima/bin one finds the executable, and under Maxima/share
   one finds the documentation. </li>
  </ul>


*/
