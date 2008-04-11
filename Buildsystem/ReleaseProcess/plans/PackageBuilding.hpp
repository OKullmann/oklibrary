// Oliver Kullmann, 26.6.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ReleaseProcess/plans/PackageBuilding.hpp
  \brief Plans regarding package building


  \bug The binary coming with the package (for creating links) does not work
  on standard 32-bit platforms
  <ul>
   <li> The problem is Programming/InputOutput/PathDifference.cpp. </li>
   <li> Is it possible to compile it in such a way that it will run on all
   standard 32-bit and 64-bit systems? </li>
   <li> See "Compiler options for Programming/InputOutput/PathDifference.cpp"
   in Programming/InputOutput/plans/general.hpp. </li>
   <li> Ask on the gcc mailing list! </li>
  </ul>


  \todo Providing update mechanisms for the package-clone
  <ul>
   <li> The goal is that the user can use "git pull", which pulls
   from the public repository. </li>
   <li> Currently the local repository, from which the package
   is created, is stored via "git pull", which isn't suitable. </li>
  </ul>


  \todo CreatePackage extensions
  <ul>
   <li> Regarding the git-ignore-file, ignoring the makefiles should also be
   incorporated into the package (as long as we still need those makefiles). </li>
   <li> Perhaps there should be additional targets for the oklib-masterscript
   to "compile and test" everything (to ease building a new environment). </li>
   <li> For a package created, a ((semi-)automatical) e-mail to the notification list
   should be sent. </li>
   <li> Release-packages must be stored in an accessible depot. At least the
   core-packages need to be backed-up on cs-oksvr. See
   "ExternalSources repository" in Buildsystem/ReleaseProcess/plans/Release.hpp. </li>
   <li> Checking is separate, and has to be done (fully) in advance
   see Buildsystem/plans/CheckBuildSystem.hpp. </li>
   <li> The appropriate "user clone" has to be created in advance
   (see Buildsystem/ReleaseProcess/plans/Release.hpp) --- and also this clone has to be
   tested. </li>
  </ul>

*/
