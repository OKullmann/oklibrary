// Oliver Kullmann, 28.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/plans/general.hpp
  \brief Plans for building external sources in general


  \todo Elaborated directory structure
  <ul>
   <li> Four sub-directories of OKplatform/ExternalSources:
    <ol>
     <li> builds </li>
     <li> doc </li>
     <li> installations </li>
     <li> sources </li>
    </ol>
   </li>
   <li> Perhaps we rename "doc" to "docs" ? </li>
   <li> Perhaps better "Docs" and "Installations", since if
   these directories are deleted, then something is lost. </li>
   <li> Rewrite all build-make-files and make-variables to take
   account of the new directory structure. </li>
  </ul>


  \todo Downloading sources
  <ul>
   <li> Via "make getsources" the system should download all the packages from
   \verbatim
cs-oksvr.swan.ac.uk:/work/Repositories/ExternalSources_recommended
   \endverbatim
   </li>
   <li> Better, we make this directory available via the OKlibrary web site, and
   then one can (manually) download each file separately, or an archive with
   all of them, or via "make getsources". </li>
   <li> Perhaps best via anonymous ftp? Better we use a password (a universal one,
   given in Configuration). </li>
  </ul>


  \todo Building documentation
  <ul>
   <li> Documentation building should be done automatically when building. </li>
   <li> We should extract only the documentation from the archives (and
   remove everything else which is not needed). </li>
  </ul>
   

  \todo General
  <ul>
   <li> It would ge good, if after doing a local installation, easily the
   installation could also be made global. See Gmp.mak and
   Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp for the new scheme. </li>
   <li> It should always be possible, to install something independently and
   only to specify where to find something (so for every special build there
   needs to be a specification what this build actually provides).
   See "Software management" in Buildsystem/ExternalSources/plans/Configuration.hpp. </li>
   <li> Optionally there should be also local versions of valgrind and
   doxygen (and other tools). This is necessary on systems where the user
   does not have root access. </li>
   <li> For "make all" we have the following problems:
    <ul>
     <li> The user cannot wait for "sudo". </li>
     <li> Perhaps for example asciidoc is already installed, and this
     suffices? </li>
    </ul>
   </li>
   <li> Regarding documentation building: Currently boost.mak overrides old
   (existing) documentation, while gcc.mak leaves it intact --- should we
   have a general policy about it? Yet the plan is to make experiences and
   see what is better.
    <ul>
     <li> It seems that "always replace", exploiting this behaviour of
     the cp-command, seems more standard. </li>
     <li> One point in favour for the different treatment in gcc.mak is,
     that there  we want to eliminate the doc-subdirectory of the installation
     directory, and then "mv" seems safer than the sequence "cp -r rm -r"
     (what if some parameters are not set right, and rm goes wild?);
     perhaps this is not very strong. </li>
    </ul>
   </li>
  </ul>


  \todo Make-variables for external libraries : UPDATE needed (%w.r.t. the new view
  of the configuration centre)
  <ul>
   <li> Variable "prefix" in ExternalSources/Makefile should be replaced by
   variable ExternalSources. </li>
   <li> How to define variables like Boost and Ubcsat in general?
     We need a nice little method (plus documentation), so that we can add easily as
     many external libraries as we want. See See Gmp.mak and
   Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp for the new scheme. </li>
   <li> And then we have make-variables "gcc_version_number" (for OKlibBuilding/Makefile) and
     "gcc-version" (for boost.mak) which likely should have the same names. </li>
   <li> In system_definitions.mak we have a definition of Boost which seems wrong
     to me (OK) (or at least misleading). DONE (resp. moved to other todos) </li>
  </ul>


  \todo Installing SAT solvers
  <ul>
   <li> Installing external SAT solvers should be a special service. </li>
   <li> Perhaps we have a special subdirectory of OKplatform/ExternalSources
   (for example OKplatform/ExternalSources/SATsolver) ? </li>
   <li> These solvers should be somewhat more integrated into the OKlibrary
   than just ordinary external sources; on the other hand, these packages
   are usually in much worse shape. </li>
   <li> Of course, only open-source-solvers. </li>
   <li> For C++ solvers we need the ability to use older gcc-versions. </li>
   <li> We should have also a directory Buildsystem/ExternalSources/SATsolvers
   (solvers are not "SpecialBuilds"). </li>
  </ul>


  \todo Using CMake
  <ul>
   <li> It seems that the special makefile for external sources might be a suitable
   candidate for using CMake. </li>
   <li> Compare "CMake" in Buildsystem/OKlibBuilding/plans/MakeSystem.hpp. </li>
  </ul>


  \todo Complete Buildsystem/ExternalSources/docus/Internals.hpp
  <ul>
   <li> Complete the text. </li>
   <li> Explain, how to obtain the special installation instructions, such that
     in case of a problem the problem can be reproduced independently of
     the OKlibrary (necessary for requests to mailing lists associated
     with the special packages). It should be possible to run
     the make-file in protocol mode, so that the sequence of build
     instrations can be extracted. </li>
  </ul>


  \todo Finish docus for special builds


  \todo %Tools
  <ul>
   <li> Investigate the NiX system (a system for managing libraries). </li>
  </ul>

*/
