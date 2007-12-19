// Oliver Kullmann, 23.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
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
    $(ExternalSources) (variable "ExternalSources"):
    <ol>
     <li> The sources one finds in $(ExternalSources_sources) (variable
     "ExternalSources_sources"). </li>
     <li> The installed software then is located in $(ExternalSources_installations)
     (variable "ExternalSources_installations"). </li>
     <li> Building the packages is done (if possible) in separate directories,
     which can be find in $(ExternalSources_builds) (variable
     "ExternalSources_builds"). </li>
     <li> Finally the extracted documentation is located in $(ExternalSources_doc)
     (variable "ExternalSources_doc"). </li>
    </ol>
   </li>
   <li> The scripts etc. for building the external sources are in
   $(OKbuildsystem)/ExternalSources, with the following sub-directories:
    <ol>
     <li> "SpecialBuilds" contains information on how to build the packages. </li>
     <li> The standard "systematic directories", "docus" and "plans", contain,
     as usual, user documentation (like this file) and plans. </li>
     <li> "sources" mirrors $(ExternalSources_sources), and contains the
     checksums for all external sources (obtained by the tool
     <code> $(ext_src_checksum) </code>). </li>
    </ol>
   </li>
   <li> Finally the configuration information (like Version numbers etc.) is in
   $(OKbuildsystem)/Configuration/ExternalSources/all.mak. </li>
  </ul>

  $(OKbuildsystem)/ExternalSources/Makefile is the central makefile:
  <ul>
   <li> $(ExternalSources)/Makefile is a symbolic link to it XXX </li>
   <li> Defines the main targets for building and cleaning of
   external libraries. XXX </li>
   <li> It includes the makefiles from $(OKbuildsystem)/ExternalSources/SpecialBuilds,
   each responsible for building one external source. XXX </li>
  </ul>


  <h2> Adding new external sources </h2>

  In the following the directory $(OKbuildsystem) containing the buildsystem for
  the OKlibrary is abbreviated by "Buildsystem". Assume that "ExS" is to be built:
  <ol>
   <li>
    First in directory <code>Buildsystem/Configuration/ExternalSources</code>
    a configuration file "exs.mak" is to be created (copying an appropriate role
    model), and it is to be included in
    <code>Buildsystem/Configuration/ExternalSources/all.mak</code>.
   </li>
  </ol>



  <h2> In case of trouble </h2>

  <ol>
   <li> See the error messages in <code>$(external_sources_log)</code>. </li>
   <li> Often some library or some tool is missing; either you can add
   the missing part yourself (perhaps just using the package management
   of your Linux distribution), or you might find it in the OKlibrary. </li>
   <li> If this doesn't help, consult the mailing list XXX </li>
   <li> If we can't resolve it, since it seems to depend on specialities
   of your system, then
    <ol>
     <li> extract the sequence of build instruction as follows XXX </li>
     <li> reproduce the problem independently of the OKlibrary, by using
     the extracted build instructions --- if the problem goes away, report
     back to our mailing list XXX. </li>
     <li> Otherwise contact the support of the respective package, and ask
     what's wrong with the build instructions. </li>
     <li> Either you now can "repair" your own system, so that the build
     of the OKlibrary now works, or you report back to our mailing list XXX.
     </li>
    </ol>
   </li>
  </ol>

*/

