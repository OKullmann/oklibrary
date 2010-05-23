// Oliver Kullmann, 9.1.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Ecl.hpp
  \brief Plans regarding installation of the Common-Lisp Ecl


  \bug Shared library not found when using local Gmp
  <ul>
   <li> When building Ecl with the local (OKlibrary) 
   version of Gmp, where Gmp is not otherwise installed on
   the system, "oklib ecl" fails complaining that libgmp.so.10 
   can not be found. </li>
   <li> This is due to the fact that within the Ecl build
   process an executable "ecl_min" is built which uses Gmp, but 
   which doesn't link in the shared library correctly. Running
   "ldd ecl_min" on this executable confirms that libgmp.so.10
   could not be found. </li>
   <li> To resolve this, it seems that one must heed the warning
   given during Gmp's build process
   \verbatim
----------------------------------------------------------------------
Libraries have been installed in:
   /home/aeternus/Work/OKlibrary/OKplatform/ExternalSources/Installations/Gmp/4.1.2/5.0.1/lib

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the `-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the `LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the `LD_RUN_PATH' environment variable
     during linking
   - use the `-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to `/etc/ld.so.conf'

See any operating system documentation about shared libraries for
more information, such as the ld(1) and ld.so(8) manual pages.
----------------------------------------------------------------------
   \endverbatim
   by changing the configure line in ecl.mak from
   \verbatim
LDFLAGS=-Wl,-rpath=$(ecl_installation_dir_okl)/lib ./configure --prefix=$(ecl_installation_dir_okl) --with-gmp-prefix=$(gmp_installation_dir_okl) CC=$(gcc_call_okl) CXX=$(gpp_call_okl); $(postcondition) \
   \endverbatim
   to 
   \verbatim
LDFLAGS="-Wl,-rpath=$(ecl_installation_dir_okl)/lib -Wl,-rpath=$(gmp_install_directory_okl)/lib" ./configure --prefix=$(ecl_installation_dir_okl) --with-gmp-prefix=$(gmp_installation_dir_okl) CC=$(gcc_call_okl) CXX=$(gpp_call_okl); $(postcondition) \
   \endverbatim
   so that the linker is aware of the location of the shared library.
   </li>
   <li> Such changes have been tested by MG, but as this problem
   hasn't otherwise been noticed, should be tested by other users
   before being applied. </li>
  </ul>


  \todo DONE (now only the usual Bessel-problem)
  Update to newest version
  <ul>
   <li> The newest version is 9.8.2. </li>
   <li> It builds, and also Maxima builds with it, but showing quite a few
   failing tests. It seems the tests of not of relevance to us. </li>
  </ul>


  \todo Documentation
  <ul>
   <li> Documentation is not included in the package, so we need
   to download it. </li>
   <li> Creating then a wrapper Ecl.html (as CLisp.html). </li>
  </ul>

*/

