// Oliver Kullmann, 25.8.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/MasterScript/docus/general.hpp
  \brief Documentation on the program for executing build-actions

  <h1> The masterscript "oklib" </h1>


  <h2> The underlying algorithm </h2>

  Calls to <code>oklib</code> depend on the calling directory:
  <ul>
   <li> Starting with the calling directory, all directories on the
   path to the enclosing OKplatform-directory (defined by having
   a <code>.oklib</code> subdirectory) are considered, and the first
   time a directory D is recognised defines this directory as determinative
   for the makefile to call:
    <ol>
     <li> If D is "ExternalSources" then external sources building (see
     Buildsystem/ExternalSources/docus/general.hpp) is invoked. </li>
     <li> If D contains a file "definitions.mak" then the generic
     makefile for compiling the components of the library is invoked
     (see Buildsystem/OKlibBuilding/docus/general.hpp). </li>
     <li> If D is "Transitional" then a recursive makefile is invoked,
     calling the generic makefiles at lower levels. </li>
     <li> If D is "OKsystem" then another recursive makefile is invoked,
     this time designed to call different types of makefiles for
     the different repositories placed at OKsystem-level. </li>
     <li> At OKplatform-level currently there are no build-actions
     (other than given by the options below). </li>
    </ol>
    <li> If you are in a different directory than the one from which
    you want to call <code>oklib</code>, but you do not want
    to change the working directory, then use
    <code>(cd target-directory; oklib)</code>. </li>
  </ul>
  
  
  <h2> Options </h2>

  <ul>
   <li> <code>--version</code> </li>
   <li> <code>--setup</code>
    <ol>
     <li> Only used for initialisation. </li>
     <li> Calls the set-up makefile with trailing arguments (creating the
     .oklib-directory at OKplatform-level) .</li>
    </ol>
   </li>
   <li> <code>--prebuild</code>
    <ol>
     <li> Used to update the directory-structure and especially to update
     links. </li>
     <li> Creates basic directories. </li>
     <li> Creates makefile-links. </li>
     <li> Creates links (in $(public_bin_dir_okl)) to the main executables
     produced by the OKlibrary. </li>
    </ol>
   </li>
   <li> <code>--maxima</code> (configures and calls Maxima; see
   ComputerAlgebra/docus/Maxima.hpp) </li>
   <li> <code>--create-package</code> calls the ReleaseProcess makefile
   with trailing arguments, creating three packages in the directory given by
   variable packages_dir (current value is <code>$(packages_dir)</code>),
   one basic, one including documentation, and one full package including
   the external sources. Special settings (supplied as parameters) are:
    <ol>
     <li> With <code>git_repository=R</code> repository R (possibly
     remote) is cloned instead of the current repository. </li>
     <li> With <code>git_upstream=R</code> repository R (possibly
     remote) is stated as mother-clone (to be pulled from) instead
     of the current repository. </li>
     <li> With <code>ExternalSources=""</code> the additional creation
     of the full package (including external sources) is disabled. </li>
    </ol>
   </li>
  </ul>

  Otherwise all targets are delegated to the respective makefiles.
  Output is logged as follows:
  <ul>
   <li> In the anchor directory <code>.oklib</code> one finds a symbolic
   link <code>log</code>, whose default value is $(log_dir). </li>
   <li> When building external sources, error output is copied to
   <code>log/ExternalSources</code>. </li>
   <li> Otherwise all output is copied to <code>log/OKlibBuilding</code>. </li>
  </ul>

*/

