// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp
  \brief Plans regarding installation of the Ubcsat package


  \todo Update to version 1.2.0
  <ul>
   <li> We created a new package "ubcsat-1-2-0-beta.tar.bz2", cloning
   https://github.com/dtompkins/ubcsat.git, checking out branch beta. </li>
   <li> We also changed "#define MAXPARMLINELEN 4096" to
   "#define MAXPARMLINELEN 16384". </li>
   <li> "new-ubcsat-okl" is the wrapper-script to access the new version
   (besides the old version). </li>
   <li> DONE (we didn't check out the beta-version)
   The current version is not usable at all; e-mail sent to
   ubcsat-developers. </li>
   <li> The new version still seems not usable; bug report sent to
   ubcsat-developers. </li>
  </ul>


  \bug UBCSAT 1.0.0 segfaults with weighted MaxSAT instances
  <ul>
   <li> Given the input file ("test.wcnf")
   \verbatim
p wcnf 2 4
200 1 2 0
200 1 -2 0
1 1 0
1 2 0 
   \endverbatim
   and running
   \verbatim
ubcsat-okl -alg gsat -w -i test.wcnf
   \endverbatim
   or
   \verbatim
ubcsat -alg gsat -w -i test.wcnf
   \endverbatim
   segfaults on MG's machine.
   </li>
   <li> Using ubcsat 1.1.0 doesn't segfault. </li>
   <li> Typically this behaviour depends on the algorithms, so gsat can't be
   used, but hopefully other algorithms. </li>
   <li> With ubcsat version 1.2 we hope that this problem disappears. </li>
  </ul>


  \todo Make clear the required package for dos2unix
  <ul>
   <li> Some linux distributions (such as Ubuntu, and (therefore)
   presumably Debian) do not include dos2unix by default
   and the only packaged versions of the utility they have (in 
   particular the tofrodos package which until recently created
   dos2unix and unix2dos aliases - 
   http://www.thefreecountry.com/tofrodos/index.shtml) do not match the 
   "correct" version, as used by the OKlibrary. </li>
   <li> As such alternative packages, even when creating aliases oneself,
   do not have certain options available, as well as the fact that the 
   "correct" dos2unix package is not clear from a standard google search, 
   we should provide a link to the package home page somewhere within the 
   documentation or plans to make it clear where to obtain this package. </li>
   <li> The correct package is available at 
   http://www.xs4all.nl/~waterlan/dos2unix.html . </li>
  </ul>

*/

