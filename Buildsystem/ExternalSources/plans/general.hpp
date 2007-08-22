// Oliver Kullmann, 28.2.2006 (Swansea)

/*!
  \file Buildsystem/ExternalSources/plans/general.hpp
  \brief Plans for building external sources in general


  \todo Directory structure of OKplatform/ExternalSources
  <ul>
   <li> Perhaps two new sub-directories
    <ol>
     <li> OKplatform/ExternalSources/sources </li>
     <li> OKplatform/ExternalSources/installations </li>
     <li> OKplatform/ExternalSources/doc </li>
    </ol>
    where to "sources" all the tarballs go, while to "installations"
    all current sub-directories go. </li>
  </ul>


  \todo Documenting ExternalSources.mak
  <ul>
   <li> General explanations in ExternalSources/docus/general.hpp:
    <ol>
     <li> Only restricted installation support for special builds. </li>
     <li> How to obtain the special installation instructions, such that
     in case of a problem the problem can be reproduced independently of
     the OKlibrary (necessary for requests to mailing lists associated
     with the special packages).
    </ol>
   </li>
   <li> Another file in ExternalSources/docus explains how to add the installation
   of a new external source. </li>
   <li> The files in ExternalSources/SpecialBuilds/docus are for the *user*
   documentation, that is, how to install it, and very basic usage. </li>
   <li> In ExternalSources/docus we should have general.hpp for general
   explanations and links to the documentations for special builds, which
   actually likely should go to SpecialBuilds/docus. DONE </li>
   <li> The old OKBuildsystem/ExternalSources/doc.mak is gone?
   So documentation building should happen automatically (when building
   the package). DONE (extracting the documentation and moving to an appropriate
   subdirectory of OKplatform/ExternalSources/doc is part of every installation) </li>
   <li> Move the documentation from ExternalSources.mak to
   Buildsystem/docus/ExternalSources.hpp. DONE </li>
  </ul>


  \todo Downloading sources
  <ul>
   <li> Via "make getsources" the system should download all the packages from
   \verbatim
cs-oksvr.swan.ac.uk:/work/Repositories/ExternalSources_recommended
   \endverbatim
   </li>
   <li> Perhaps best via anonymous ftp? </li>
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
   <li> And then we have make-variables "gcc_version_number" (for generic.mak) and
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


  \todo %Tools
  <ul>
   <li> Investigate the NiX system (a system for managing libraries). </li>
  </ul>


  \todo Renaming : DONE
  <ul>
   <li> Perhaps we better call now "Buildsystem/ExternalSources/ExternalSources.mak"
   simply "Buildsystem/ExternalSources/makefile". DONE (these issues are now
   handled in Buildsystem/plans/MasterScript.hpp) </li>
   <li> Perhaps we should move "Buildsystem/external_sources_versions.mak"
   to directory Buildsystem/ExternalSources, and then call it
   simply "versions.mak". DONE (moved it to Buildsystem/Configuration) </li>
  </ul>


  \todo Plans-structure for ExternalSources : DONE
  <ul>
   <li> Perhaps the version number of sub-module ExternalSources
   is initially set to the current version number of Buildsystem? DONE (used a lower
   number to indicate, that the ExternalSources-part of the buildsystem is less
   refined) </li>
   <li> Likely we should now have also a milestones.hpp file. DONE </li>
  </ul>

*/
