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
   <li> By changing the configure line in ecl.mak from
   \verbatim
LDFLAGS=-Wl,-rpath=$(ecl_installation_dir_okl)/lib ./configure --prefix=$(ecl_installation_dir_okl) --with-gmp-prefix=$(gmp_installation_dir_okl) CC=$(gcc_call_okl) CXX=$(gpp_call_okl); $(postcondition) \
   \endverbatim
   to 
   \verbatim
LDFLAGS="-Wl,-rpath=$(ecl_installation_dir_okl)/lib -Wl,-rpath=$(gmp_install_directory_okl)/lib" ./configure --prefix=$(ecl_installation_dir_okl) --with-gmp-prefix=$(gmp_installation_dir_okl) CC=$(gcc_call_okl) CXX=$(gpp_call_okl); $(postcondition) \
   \endverbatim
   the problem is solved. </li>
   <li> As one can see in Buildsystem/Configuration/ExternalSources/gmp.mak,
   a case distinction needs to be made, since we might also use
   system-installation of Gmp. </li>
   <li> It seems to OK that this is a bug of the Ecl-build. It has been
   somewhat discussed on the Ecl mailing list, but best we solve the problem
   ourselves. </li>
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

