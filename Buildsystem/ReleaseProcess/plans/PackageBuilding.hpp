// Oliver Kullmann, 26.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ReleaseProcess/plans/PackageBuilding.hpp
  \brief Plans regarding package building


  \bug Missing Annotations/definitions.mak : DONE
  <ul>
   <li> It seems that "touch Annotations/definitions.mak" to create the
   empty definitions.mak in OKsystem/Annotations is missing? </li>
  </ul>


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
   <li> Some meta-data (like ignore-patterns) need to
   be added; see "Cloning" in Buildsystem/SourceControl/plans/general.hpp. </li>
   <li> DONE (there is no need anymore for the old shared repository at
   cs-oksvr, but everything is done via the Github repository; we also don't
   need to document that system)
   Documenting the old system:
    <ol>
     <li> git_public_repository must be set (typically in override.mak) to
     the full local path to the oklib-repository accessible from the
     Internet. </li>
     <li> The current value is
     "/home/csoliver/cs-svr1/csoliver/public_html/ok-sat-library/git/Transitional"
     </li>
     <li> The code
     \verbatim
cd ${git_public_repository}
git fetch ${git_main_repository} master:master
git --bare update-server-info
     \endverbatim
     in CreatePackage updated this repository from the main
     developers-repository. </li>
     <li> git_main_repository must be set (typically in override.mak) to the
     URL for the main (shared) repository. </li>
     <li> git_http_repository (again, typically in override.mak) finally is the
     http-address for git_public_repository; current value is
     "http://cs.swan.ac.uk/~csoliver/ok-sat-library/git/Transitional/". </li>
     <li> Via
     \verbatim
if [ ${git_upstream:+1} ]; then
  git_origin="--origin ${git_upstream}"
else
  git_origin=""
fi
cd ${package_directory}/OKplatform/OKsystem
git clone ${git_origin} ${git_http_repository}
     \endverbatim
     then the clone for the package is created, where via setting
     "git_upstream" the clone is instructed to pull by default from
     "git_upstream" instead of git_http_repository. </li>
    </ol>
   </li>
   <li> DONE
   This is now simplified:
    <ol>
     <li> git_http_repository is now the git-hub-repository. </li>
     <li> So there is no need for git_public_repository anymore. </li>
     <li> This repository is taken as-is (no update anymore). </li>
     <li> Pulling by user packages just from this repository. </li>
    </ol>
   </li>
   <li> DONE The goal is that the user can use "git pull", which pulls
   from the public repository. </li>
   <li> DONE Currently the local repository, from which the package
   is created, is stored via "git pull", which isn't suitable. </li>
  </ul>


  \todo CreatePackage extensions
  <ul>
   <li> Regarding the git-ignore-file, ignoring the makefiles should also be
   incorporated into the package (as long as we still need those makefiles).
    <ol>
     <li> See todo "Cloning" in Buildsystem/SourceControl/plans/general.hpp.
     </li>
    </ol>
   </li>
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
