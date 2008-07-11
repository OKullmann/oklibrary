// Oliver Kullmann, 23.7.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/docus/Internals.hpp
  \brief Internals of how the build system for external sources works


  <h1> Internals of the build system for external sources </h1>

  <h2> General overview </h2>
  
  Main directories involved are:
  <ul>
   <li> The base directory for installations of external sources is
    <code>$(ExternalSources)</code> (variable "ExternalSources"):
    <ol>
     <li> The sources one finds in <code>$(ExternalSources_sources)</code>
     (variable "ExternalSources_sources"). </li>
     <li> The installed software then is located in
     <code>$(ExternalSources_installations)</code>
     (variable "ExternalSources_installations"). </li>
     <li> Building the packages is done (if possible) in separate directories,
     which can be found in <code>$(ExternalSources_builds)</code> (variable
     "ExternalSources_builds"). </li>
     <li> Finally the extracted documentation is located in
     <code>$(ExternalSources_doc)</code> (variable "ExternalSources_doc").
     </li>
    </ol>
   </li>
   <li> The scripts etc. for building the external sources are in
   <code>$(OKbuildsystem)/ExternalSources</code>, with the following
   sub-directories:
    <ol>
     <li> "SpecialBuilds" contains information on how to build the packages. </li>
     <li> The standard "systematic directories", "docus" and "plans", contain,
     as usual, user documentation (like this file) and plans. </li>
     <li> "sources" mirrors <code>$(ExternalSources_sources)</code>, and
     contains the checksums for all external sources (obtained by the tool
     <code> $(ext_src_checksum) </code>). </li>
    </ol>
   </li>
  </ul>


  <h2> Makefiles </h2>

 
  <ul>
   <li> <code>$(OKbuildsystem)/ExternalSources/Makefile</code> is the central
   makefile (invoked by the masterscript <code>oklib</code>).
    <ol>
     <li> Some functions are provided for unpacking archives. </li>
     <li> The global targets, given by variable "global_targets_extsrc_okl",
     with value <center> $(global_targets_extsrc_okl) </center>
     are defined. </li>
     <li> Finally it includes the makefiles from
     <code>$(OKbuildsystem)/ExternalSources/SpecialBuilds</code>, each
     responsible for building one external source. </li>
    </ol>
   </li>
   <li> The configuration information is obtained from
   <code>$(OKbuildsystem)/Configuration/ExternalSources/all.mak</code>:
    <ol>
     <li> The general configuration-variables regarding external sources are
     defined there. </li>
     <li> And via inclusion of the special makefiles, responsible for building
     single external sources, all special configuration information (like
     version numbers, package-names, etc.) is obtained. </li>
    </ol>
   </li>
   <li> For each special external source (like "git" or "gcc") there are thus
   two makefiles, %e.g., for "git" we have
   <code>$(OKbuildsystem)/Configuration/ExternalSources/git.mak</code> for the
   configuration information, and
   <code>$(OKbuildsystem)/ExternalSources/SpecialBuilds/git.mak</code> for
   the actual build-instructions.
   </li>
  </ul>


  <h2> Documentation </h2>

  <ul>
   <li> In directory
   <code>$(OKbuildsystem)/ExternalSources/SpecialBuilds/docus</code> for each
   external source one finds a dedicated documentation-file (belonging to the
   Doxygen-system; for example "git.hpp"). </li>
   <li> Some external sources need an index-page to access documentation, and
   such index-pages (which do not belong to the Doxygen-system, but to the
   system of local html-pages) are found in directory
   <code>$(OKbuildsystem)/ExternalSources/SpecialBuilds/Documentation</code>
   (for example one finds there "Maxima.html"). </li>
   <li> The overview page on the installation of external sources is
   <code>$(OKbuildsystem)/Html/Local/ExternalSources.html</code>
   (again, part of the local html-system). </li>
   <li> The variables for checking the status of installations are (currently)
   defined in
   <code>$(OKbuildsystem)/Configuration/ExternalSources/tests.mak</code>. </li>
   <li> The buildsystem relies on using absolute paths to all files. To be able
   to build the documentation under different circumstances, in
   <code>$(OKbuildsystem)/Configuration/Html/relative_paths.mak</code>
   one finds definitions for paths from local html-pages to other local
   html-pages and to pages in <code>$(ExternalSources_doc)</code>. </li>
  </ul>


  <h2> Adding new external sources </h2>

  In the following the directory <code>$(OKbuildsystem)</code>, containing
  the buildsystem for the OKlibrary, is abbreviated by "Buildsystem".

  Assume that "ExS" is to be built:
  <ol>
   <li>
    First in directory <code>Buildsystem/Configuration/ExternalSources</code>
    a configuration file "exs.mak" is to be created (copying an appropriate role
    model), and it is to be included in
    <code>Buildsystem/Configuration/ExternalSources/all.mak</code>.
   </li>
   <li>
   Then in directory <code>Buildsystem/ExternalSources/SpecialBuilds</code>
   the makefile "exs.mak" for building the source is to be created (copying
   an appropriate role model), and it is to be include in the makefile
   <code>Buildsystem/ExternalSources/Makefile</code>.
   </li>
   <li> Documentation:
    <ol>
     <li> First the file
     <code>Buildsystem/Configuration/ExternalSources/tests.mak</code>
     has to be extended with test-code for ExS, copying appropriate
     role models. </li>
     <li> And using these variables, a docu-file has to be produced
     named
     <code>Buildsystem/ExternalSources/SpecialBuilds/docus/ExS.hpp</code>.
     </li>
     <li> This docu-file should get a link in
     <code>Buildsystem/ExternalSources/docus/general.hpp</code>. </li>
     <li> And its address can now be entered into the configuration makefile
     <code>Buildsystem/Configuration/ExternalSources/exs.mak</code>. </li>
     <li> Finally we create an entry in the main external-sources overview
     page <code>Buildsystem/Html/Local/ExternalSources.html</code> (again,
     by copy-and-paste from other entries). </li>
     <li> And for the links to work new entries are added (appropriately) in
     <code>Buildsystem/Configuration/Html/relative_paths.mak</code>. </li>
    </ol>
   </li>
  </ol>



  <h2> In case of trouble </h2>

  <ol>
   <li> See the error messages in <code>$(external_sources_log)</code>. </li>
   <li> Often some library or some tool is missing; either you can add
   the missing part yourself (perhaps just using the package management
   of your Linux distribution), or you might find it in the OKlibrary. </li>
   <li> If this doesn't help, consult the mailing list. </li>
   <li> If we can't resolve it, since it seems to depend on specialities
   of your system, then
    <ol>
     <li> extract the sequence of build instruction from the makefiles; </li>
     <li> reproduce the problem independently of the OKlibrary, by using
     the extracted build instructions --- if the problem goes away, report
     back to our mailing list. </li>
     <li> Otherwise contact the support of the respective package, and ask
     what's wrong with the build instructions. </li>
     <li> Either you now can "repair" your own system, so that the build
     of the OKlibrary now works, or you report back to our mailing list.
     </li>
    </ol>
   </li>
  </ol>

*/

