// Oliver Kullmann, 9.4.2007 (Swansea)
/* Copyright 2007, 2008, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/docus/general.hpp
  \brief General documentation for the makefile "Buildsystem/ExternalSources/Makefile", responsible for external builds
  
  <h1> General overview </h1>


  <h2> Basic ideas </h2>

  <ul>
   <li> The <strong>recommended version</strong> is the version installed by default;
   using other versions might or might not work. </li>
   <li> Two basic types of installations:
    <ol>
     <li> <strong>System-wide installation</strong>: installation in directories like
     <code>/usr/local</code> (depending on the package's own build system); requires
     at the end of the installation to enter the appropriate super-user password
     (if this was missed, then just re-run the build, which should quickly arrive
     at the entry-point again). </li>
     <li> <strong>Local installation</strong>: installation to directory
     <code>$(ExternalSources_installations)</code>. </li>
    </ol>
    In both case, if possible, we will extract documentation to the local documentation
     directory <code>$(ExternalSources_doc)</code>.
   </li>
   <li> The build system provides access to the results of the builds:
    <ol>
     <li> executable programs </li>
     <li> source-code libraries </li>
     <li> link-libraries </li>
     <li> documentation </li>
     <li> other resources. </li>
    </ol>
   </li>
   <li> As part of a "holistic" or "active" library, the part of the build system
   responsible for building external sources is highly configurable and extensible;
   details are explained in Buildsystem/ExternalSources/docus/Internals.hpp. </li>
   <li> The OKlibrary (and its support "staff") can offer only very restricted
   installation support (especially for general-purpose tools like web servers).
   For more information on the internal build process see
   Buildsystem/ExternalSources/docus/Internals.hpp. </li>
  </ul>


  <h2> Configuration </h2>

  For more permanent changes, enter into <code>$(OKoverrideconfig)</code>
  the overriding definitions, for example
  \code
ocaml_bin_dir := /usr/local/bin
  \endcode
  to use the system-wide ocaml-installation instead of the local installation.
  (These definitions prevent the involved variables from being (re-)defined by
  the configuration system.)

  <p />

  Since directory ExternalSources (<code>$(ExternalSources)</code>) can become
  quite big, one might want to move its content to directory "OtherLocation":
  <ol>
   <li> Enter
   \verbatim
ExternalSources = FullPathToOtherLocation
   \endverbatim
   into <code>$(OKoverrideconfig)</code>; a concrete example would be
   \verbatim
ExternalSources = /home/user/ExternalSources
   \endverbatim
   </li>
   <li> Move the content of directory ExternalSources to the new place (so that
   the elements of ExternalSources become the elements of OtherLocation). </li>
   <li> Now the old directory ExternalSources has become empty, and is only to
   be used for the oklib-calls (like <code>oklib maxima</code> for installing
   Maxima). </li>
  </ol>

  If just a different version shall be installed, without changing settings
  permanently, provide the variable-settings on the command line, for example
  \verbatim
oklib gcc gcc_recommended_version_number_okl=4.2.4
  \endverbatim
  to install gcc with version 4.2.4 (similarly for any other variable setting).

  
  <h2> Installations </h2>

  User documentation on how to install supported software and basic usage
  (sorted alphabetically):

  <ol>
   <li> Asciidoc (Buildsystem/ExternalSources/SpecialBuilds/docus/Asciidoc.hpp)
   </li>
   <li> Boost (Buildsystem/ExternalSources/SpecialBuilds/docus/Boost.hpp) </li>
   <li> CLisp (Buildsystem/ExternalSources/SpecialBuilds/docus/CLisp.hpp) </li>
   <li> Coq (Buildsystem/ExternalSources/SpecialBuilds/docus/Coq.hpp) </li>
   <li> Doxygen (Buildsystem/ExternalSources/SpecialBuilds/docus/Doxygen.hpp)
   </li>
   <li> ECL (Buildsystem/ExternalSources/SpecialBuilds/docus/Ecl.hpp) </li>
   <li> Gap (Buildsystem/ExternalSources/SpecialBuilds/docus/Gap.hpp) </li>
   <li> Gcc (Buildsystem/ExternalSources/SpecialBuilds/docus/Gcc.hpp) </li>
   <li> Git (Buildsystem/ExternalSources/SpecialBuilds/docus/Git.hpp) </li>
   <li> Gmp (Buildsystem/ExternalSources/SpecialBuilds/docus/Gmp.hpp) </li>
   <li> Grasp (Buildsystem/ExternalSources/SpecialBuilds/docus/Gmp.hpp) </li>
   <li> Maxima (Buildsystem/ExternalSources/SpecialBuilds/docus/Maxima.hpp)
   </li>
   <li> Mhash (Buildsystem/ExternalSources/SpecialBuilds/docus/Mhash.hpp) </li>
   <li> Ocaml (Buildsystem/ExternalSources/SpecialBuilds/docus/Ocaml.hpp) </li>
   <li> Picosat (Buildsystem/ExternalSources/SpecialBuilds/docus/Picosat.hpp)
   </li>
   <li> Postgresql (Buildsystem/ExternalSources/SpecialBuilds/docus/Postgresql.hpp)
   </li>
   <li> R (Buildsystem/ExternalSources/SpecialBuilds/docus/R.hpp) </li>
   <li> Sage (Buildsystem/ExternalSources/SpecialBuilds/docus/Sage.hpp) </li>
   <li> Ubcsat (Buildsystem/ExternalSources/SpecialBuilds/docus/Ubcsat.hpp) </li>
   <li> Valgrind (Buildsystem/ExternalSources/SpecialBuilds/docus/Valgrind.hpp)
   </li>
  </ol>

  The build-directories (which can always be removed after a build) are
  located in directory <code>$(ExternalSources_builds)</code>.
  
*/

