// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Mhash.hpp
  \brief Plans regarding installation of the Mhash library


  \todo Mhash : DONE
  <ul>
   <li> DONE
   mhash.mak needs a complete review; the usage of make-variables
   seems pretty random (we should synchronise it with
   Configuration/ExternalSources/all.mak
   and with the other build-files; and we need specifications for the
   variables,
   at least examples). </li>
   <li> DONE (has been corrected)
   Make a bug report about version 0.9.9 of Mhash (
   see the two files in Kryptologie/Mhash/corrected/mutils --- two
   commas too much). </li>
   <li> DONE
   We need some user-information for mhash (how to use the installed
   files). </li>
   <li> DONE (local is enough for now)
   It seems yet only a local build is available? As discussed above, then
   also the option for a global installation should be available. </li>
   <li> DONE
   The build-instructions for mhash should be as similar as possible
   to those for Boost. </li>
   <li> DONE
   Shouldn't the default for building mhash be using the recommended
   local gcc-version? (Is linking necessary?) </li>
   <li> DONE (cleanmhash should just remove the build-directories)
   It seems that cleanmhash should also remove the installation directory.
   (MH) Isn't it the case that we want cleanmhash only to remove the build
   directory - so that the local installation can still be used after cleaning?
   Then to clean everything there is already the target "cleanallmhash". </li>
  </ul>


  \todo Complete MHash documentation
   
*/

