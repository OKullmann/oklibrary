// Oliver Kullmann, 9.1.2009 (Swansea)
/* Copyright 2009, 2010, 2011, 2012, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Ecl.hpp
  \brief Plans regarding installation of the Common-Lisp Ecl


  \todo Update to 12.12.1
  <ul>
   <li> Building Ecl seems to work, but when building Maxima we get
   \verbatim
;;; Loading "/home/kullmann/OKplatform/ExternalSources/builds/Maxima/ecl/maxima-5.29.0/src/binary-ecl/maxima-package.fas"

Condition of type: SEGMENTATION-VIOLATION
Detected access to an invalid or protected memory address.
   \endverbatim
   </li>
   <li> Let's wait for an update of Maxima (we can't use 5.29.1 due to a bug).
   </li>
  </ul>


  \todo Update to 12.2.1 : DONE (now outdated)
  <ul>
   <li> Building Ecl seems to work, but when building Maxima we get
   \verbatim
;      - Loading binary file "binary-ecl/intl.fas"
;;; Loading "/home/kullmann/OKplatform/ExternalSources/builds/Maxima/ecl/maxima-5.26.0/src/binary-ecl/intl.fas"
An error occurred during initialization:
In form
(PROGN
 (LOAD "../lisp-utils/defsystem.lisp")
 (FUNCALL (INTERN (SYMBOL-NAME :OPERATE-ON-SYSTEM) :MK)
          "maxima"
          :COMPILE
          :VERBOSE
          T)
 (COMMON-LISP-USER::BUILD-MAXIMA-LIB))
Unknown keyword :RENAME-AND-DELETE.
make[2]: *** [binary-ecl/maxima] Error 1
make[2]: Leaving directory `/home/kullmann/OKplatform/ExternalSources/builds/Maxima/ecl/maxima-5.26.0/src'
   \endverbatim
   </li>
   <li> OK asked on the Maxima mailing-list. </li>
  </ul>


  \todo Update to 11.1.1.2
    <ul>
   <li> DONE (that repository is not to be used anymore)
   Version 11.1.1.2 is an update by
   \verbatim
> ADDRESS=anonymous@ecls.cvs.sourceforge.net:/cvsroot/ecls
> cvs -z3 -d:pserver:$ADDRESS checkout ecl
   \endverbatim
   </li>
   <li> The cvs-repository is actually outdated, and the git-repository
   has to be used:
   \verbatim
Ecl> git clone git://ecls.git.sourceforge.net/gitroot/ecls/ecl
ea0fdff00734051a1c775c2740ddb2a92122e93c
> mv ecl ecl-11.1.1.2
> tar -cjf ecl-11.1.1.2.tar.bz2 ecl-11.1.1.2
   \endverbatim
   </li>
   <li> The above change of repository needs to be entered into our
   docus (where we still mention the cvs-repository). </li>
   <li> DONE (provided oklib_log)
   Needed to get writefile to work. </li>
  </ul>


  \todo Update to 11.1.1 : DONE
  <ul>
   <li> Version 11.1.1.1 is an update by
   \verbatim
> ADDRESS=anonymous@ecls.cvs.sourceforge.net:/cvsroot/ecls
> cvs -z3 -d:pserver:$ADDRESS checkout ecl
   \endverbatim
   </li>
   <li> DONE (now using conversion to upper-case)
   Test failure with 11.1.1.1:
    <ol>
     <li>
     \verbatim
okltest_dec2hex(dec2hex)
ASSERT: Expression " "0b" = "0B" " does not evaluate to true.

> printf(false,"~x",11);
 "b"
     \endverbatim
     </li>
     <li> Before we got "B" --- what is correct? </li>
     <li> This is now the only test-failure. </li>
    </ol>
   </li>
   <li> DONE (now using a local installation of libffi)
   Should we provide a local installation of libffi? Apparently Ecl can
   live without it --- is this important for us?
    <ol>
     <li> On cs-oksvr the build of 11.1.1.1 fails with
     \verbatim
cp /home/csoliver/OKplatform/ExternalSources/builds/Ecl/ecl-11.1.1.1/src/../contrib/unicode/ucd.dat .
if [ -f CROSS-COMPILER ]; then \
                ./CROSS-COMPILER compile; \
        else \
                ECLDIR=`pwd`/ ./ecl_min compile; \
        fi
./ecl_min: error while loading shared libraries: libffi.so.4: cannot open shared object file: No such file or directory
     \endverbatim
     </li>
     <li> libffi is available at http://sourceware.org/libffi/ . </li>
     <li> Making a system-installation:
     \verbatim
builds> tar -xzf ../sources/Ecl/libffi-3.0.9.tar.gz
builds> cd libffi-3.0.9/
libffi-3.0.9> ./configure
libffi-3.0.9> make
libffi-3.0.9> make install
     \endverbatim
     </li>
     <li> However this doesn't help since we build Ecl with the local gcc, and
     thus we also need to build libffi with the local gcc. </li>
    </ol>
   </li>
   <li> DONE (likely not needed)
   Apparently Ecl can also be built by a C++ compiler --- shall we do so?
   We already provide CC and CXX, so that hopefully suffices? </li>
   </li>
   <li> DONE (not relevant for the current configuration)
   Maxima test failure (5.21.1, but not 5.23.2):
   \verbatim
Running tests in rtest8:
********************** Problem 101 ***************
Input:
ev(e5, au = 0, omega = 2)
Result:
            - u
quad_qawf(%e   , u, 0, 2, sin, epsabs = 1.e-10, limit = 32, maxp1 = 100,
                                                                   limlst = 10)
This differed from the expected result:
[.4000000000000001, 2.216570948815925e-11, 175, 0]
133/134 tests passed
The following 1 problem failed: (101)
   \endverbatim
   This looks like a new problem. </li>
   <li> DONE (corrected with 11.1.1.1)
   We get an OKlib test-failure (with Maxima 5.21.1 and 5.23.2):
    <ol>
     <li>
     \verbatim
okltest_primes_interval(primes_interval)
ASSERT: Expression " 231 = 168 " does not evaluate to true.
ERROR: /home/kullmann/OKplatform/system_directories/aux/tests/maxima/OKlib/ComputerAlgebra/NumberTheory/Lisp/testobjects/PrimeNumbers
make[2]: *** [/home/kullmann/OKplatform/OKsystem/OKlib/ComputerAlgebra/NumberTheory/Lisp/testobjects/PrimeNumbers.mac] Error 1
make[2]: Leaving directory `/home/kullmann/OKplatform/OKsystem/OKlib/ComputerAlgebra'
     \endverbatim
     </li>
     <li> The problem is that next_prime(113) = 121 = 11^2 ! </li>
     <li> Inform the Maxima mailing-list! (In any case this error should have
     been caught by the tests --- next_prime here is very buggy.) </li>
    </ol>
   </li>
  </ul>


  \todo Documentation
  <ul>
   <li> Documentation is not included in the package, so we need
   to download it. </li>
   <li> Creating then a wrapper Ecl.html (as CLisp.html). </li>
  </ul>

*/

