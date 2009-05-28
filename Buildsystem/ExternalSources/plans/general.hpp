// Oliver Kullmann, 28.2.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/plans/general.hpp
  \brief Plans for building external sources in general


  \todo DONE
  Building "all"
  <ul>
   <li> Dependencies have to be taken care of for target "all". </li>
   <li> gcc is needed often, and so is gmp; also fortran might be needed. </li>
   <li> Later we should have make-dependencies in place which handle these
   dependencies automatically; yet perhaps we should declare them in
   Configuration/ExternalSources/all.mak (in comments). </li>
  </ul>


  \todo Downloading sources
  <ul>
   <li> Via "make update" the system should download all the packages from
   the source-directory as provided on the OKlibrary web site. </li>
   <li> For this, additionally to %e.g. "git_source_okl" a variable
   "git_source_ext_okl" is needed, which for this purpose specifies
   the extension of git_source_okl (like ".tar.bz2"). </li>
   <li> And every configuration file shall also contain %e.g.
   "all_extsrc_okl += git" to update the list of all external sources. </li>
   <li> Due to the (apparent) impossibility of translating a target
   into some other target computed from it, we cannot use the source-archives
   as prerequisites of the rules, but we have to check it manually. </li>
   <li> When the source-archive is existing, then we should nevertheless
   check the md5sum. </li>
   <li> The full-source-path is translated into the url, and then obtained
   via wget. </li>
  </ul>


  \todo Interfaces via OKplatform/bin
  <ul>
   <li> We should make nearly all executables from ExternalSources
   available in the public bin-directory (typically via symbolic links). </li>
   <li> In this way after for example the version of doxygen
   has changed, still the build can work with the old version
   (since the "interface", OKplatform/bin/doxygen, is independent
   of the version). </li>
   <li> Perhaps, additionally to "doxygen_call_okl", we should
   introduce "doxygen_public_call_okl" ? </li>
   <li> Better to introduce "doxygen_internal_call_okl", which
   contains the full address for the internal call, while, as
   before, "doxygen_call_okl" contains how to call doxygen
   from the build-system. </li>
   <li> On problem arising in this way is that for example then
   the user is tempted to just call "maxima", which doesn't connect
   to the OKlibrary.
    <ol>
     <li> So well, seems alright to have the distinction between
     "oklib --maxima" and plain "maxima". </li>
     <li> But we should point out that the normal use is via
     "oklib --maxima". </li>
    </ol>
   </li>
   <li> Units to change:
    <ol>
     <li> Doxygen
      <ol>
       <li> The symbolic links are already there, we only need to change
       doxygen_call_okl etc. </li>
      </ol>
     </li>
     <li> CLisp
      <ol>
       <li> We need symbolic links to "clisp" and to "lisp.run". </li>
      </ol>
     </li>
     <li> Maxima
      <ol>
       <li> A plain symbolic link is sufficient. </li>
       <li> We need to mention
       Installations/Maxima/5.15.0/share/maxima/5.15.0/share/maxima-init.mac.
       </li>
       <li> HOWEVER, at present we should not encourage users do run maxima
       by themselfes, since we do not have any control over maxima-init.mac:
        <ul>
         <li> If one is placed in ~/.maxima, then this will be ALWAYS used,
         destroying the oklib-maxima-usage! </li>
         <li> So this directory should not be present! </li>
         <li> This is clearly a maxima-bug. </li>
        </ul>
       </li>
      </ol>
     </li>
     <li> Sage
      <ul>
       <li> Only the sage-script needs to be made available. </li>
       <li> Apparently the build is incomplete yet: The line
       \verbatim
SAGE_ROOT="....."
       \endverbatim
       in the sage-script needs to be changed to contain the full path. </li>
       <li> Then a symbolic link works. </li>
      </ul>
     </li>
     <li> R
      <ol>
       <li> A plain symbolic link is sufficient. </li>
       <li> We need to point out in the documentation which files
       are preloaded in this way. </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo Elaborated directory structure
  <ul>
   <li> Four sub-directories of OKplatform/ExternalSources:
    <ol>
     <li> builds </li>
     <li> Docs </li>
     <li> Installations </li>
     <li> sources </li>
    </ol>
   </li>
   <li> Perhaps we rename "doc" to "docs" ? </li>
   <li> Rewrite all build-make-files and make-variables to take
   account of the new directory structure. </li>
   <li> And then SpecialBuilds should be subdivided, perhaps best
   in accordance to the sub-division on the external-sources
   overview page. </li>
   <li> But this subdivision perhaps best is avoided for
   OKplatform/ExternalSources. </li>
   <li> DONE Perhaps better "Docs" and "Installations", since if
   these directories are deleted, then something is lost. </li>
  </ul>


  \todo Building documentation
  <ul>
   <li> Documentation building should be done automatically when building.
   </li>
   <li> We should extract only the documentation from the archives (and
   remove everything else which is not needed). </li>
  </ul>


  \todo Documenting changes of files outside of the OKplatform directory
  <ul>
   <li> We need to collect and document all such actions outside the realm
   of the OKlibrary. </li>
   <li> Sage creates an ".ipythonrc" directory is created in the HOME
   directory if it doesn't exist (and this should be it). </li>
   <li> Maxima
    <ol>
     <li> Gnuplot creates some files in the HOME directory. </li>
    </ol>
   </li>
  </ul>
   

  \todo General
  <ul>
   <li> It should always be possible, to install something independently and
   only to specify where to find something (so for every special build there
   needs to be a specification what this build actually provides).
   See "Software management" in
   Buildsystem/ExternalSources/plans/Configuration.hpp. </li>
   <li> The naming-scheme "gcc_recommended_version_number_okl" is no longer
   appropriate, and should to be replaced by "gcc_version_okl":
    <ol>
     <li> Of course, similar in all the other cases. </li>
    </ol>
   </li>
   <li> DONE (the default is to install everything locally)
   For "make all" we have the following problems:
    <ul>
     <li> The user cannot wait for "sudo". </li>
     <li> Perhaps for example asciidoc is already installed, and this
     suffices? </li>
    </ul>
   </li>
   <li> DONE (we always use "copy -f" resp. "copy -fr", overriding
   existing old versions; if a renaming is necessary, then perform
   first this copying, and then use "mv -f")
   Regarding documentation building: Currently boost.mak overrides old
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
   <li> DONE (transferred to
   Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp)
   It would ge good, if after doing a local installation, easily the
   installation could also be made global. See Gmp.mak and
   Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp for the new scheme.
   </li>
   <li> DONE (transferred to
   Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp)
   Optionally there should be also local versions of valgrind and
   doxygen (and other tools). This is necessary on systems where the user
   does not have root access. </li>
  </ul>


  \todo Make-variables for external libraries : UPDATE needed (%w.r.t. the new
  view of the configuration centre)
  <ul>
   <li> Variable "prefix" in ExternalSources/Makefile should be replaced by
   variable ExternalSources. </li>
   <li> How to define variables like Boost and Ubcsat in general?
   We need a nice little method (plus documentation), so that we can add
   easily as many external libraries as we want. See See Gmp.mak and
   Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp for the new
   scheme. </li>
   <li> And then we have make-variables "gcc_version_number" (for
   OKlibBuilding/Makefile) and "gcc_version_okl" (for boost.mak) which likely
   should have the same names. </li>
   <li> In system_definitions.mak we have a definition of Boost which seems
   wrong to me (OK) (or at least misleading). DONE (resp. moved to other
   todos) </li>
  </ul>


  \todo Using CMake
  <ul>
   <li> It seems that the special makefile for external sources might be a
   suitable candidate for using CMake. </li>
   <li> Compare "CMake" in Buildsystem/OKlibBuilding/plans/MakeSystem.hpp.
   </li>
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
