// Oliver Kullmann, 25.8.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2011 Oliver Kullmann
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
     <li> If D is "OKlib" then a recursive makefile is invoked,
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
     <li> Calls the set-up makefile (<code>${buildsystem_dir}/SetUp.mak</code>)
     with trailing arguments (creating the .oklib-directory at
     OKplatform-level) .</li>
    </ol>
   </li>
   <li> <code>--prebuild</code>
    <ol>
     <li> Used to update the directory-structure and especially to update
     links. </li>
     <li> Calls the prebuild-makefile
     (<code>$(OKbuildsystem)/Makefile</code>) with target "prebuild"
     in case no further arguments are supplied, and otherwise with trailing
     arguments (as targets). </li>
     <li> Target "prebuild" comprises the sub-targets "make_directories",
     "create_links" and "create_public_links", for
      <ol>
       <li> creating basic directories; </li>
       <li> creating makefile-links; </li>
       <li> creating links to the main executables produced by the OKlibrary
        (in <code>$(public_bin_dir_okl)</code>). </li>
      </ol>
     </li>
     <li> The target "clean_links" removes all makefile-links. </li>
    </ol>
   </li>
   <li> <code>--maxima</code>
    <ul>
     <li> Configures and calls Maxima; see ComputerAlgebra/docus/Maxima.hpp.
     </li>
     <li> Further arguments are passed to Maxima. </li>
     <li> The initialisation file is
     <code>$(maxima_init_okl)</code> (managed by the OKlibrary). </li>
     <li> The environment variable HOME is set to
     <code>$(maxima_homedir_okl)</code>. </li>
     <li> In this directory in file ".maxima_history" the command-line
     history is stored when using Ecl (while yet for CLisp we do not
     use "rlwrap", and thus no command-line history is available). </li>
     <li> While for all Lisps in sub-directory ".maxima" created binaries
     are stored. </li>
    </ul>
   </li>
   <li> <code>--R</code>
    <ul>
     <li> Calls R; see
     Buildsystem/ExternalSources/SpecialBuilds/docus/R.hpp. </li>
     <li> The details of the invokation are as follows:
      <ul>
       <li> The standard parameters are <code>$(R_call_parameters)</code>.
       </li>
       <li> While R_PROFILE is set to <code>$(R_site_profile_okl)</code>. </li>
       <li> So the command-history is stored in file ".Rhistory" in the
       calling directory. </li>
       <li> And in file ".RData" in the calling directory the workspace
       of the last session is stored, though it is not restored for new
       sessions. </li>
       <li> While by a file ".Rprofile" in the calling directory additional
       initialisation instruction can be performed (after the initialisation
       provided by the OKlibrary, which uses R_PROFILE as mentioned above).
       </li>
      </ul>
     </li>
     <li> If however further parameters are given as part of the oklib-call,
     then R is called with just these parameters (while still setting
     R_PROFILE as above). </li>
     <li> Finally, via <code>--Rr</code> (which ignores further parameters)
     the workspace of the last session is restored (while the user is asked
     whether on exit the workspace shall be saved). </li>
    </ul>
   </li>
   <li> Push and pull for oklib-repositories (where the public repository
   is $(git_http_address_okl)):
    <ol>
     <li> <code>--push-developer arguments</code> pushes to the
     public repository, transferring arguments to the git-command. Note that
     pushing to the public repository is only possible if write-access has
     been granted. </li>
     <li> <code>--pull-public arguments</code> pulls from the public
     repository, transferring arguments to the git-command. Note that the
     branch-name is required as an argument, if git has not been configured
     otherwise. </li>
     <li> <code>--push-ssh remote-machine remote-OKplatform-dir arguments</code>
     pushes via ssh to an arbitrary OKlibrary-repository. If
     "remote-OKplatform-dir" contains for example "~" (for
     the home directory), then it is to be put in quotation marks. The
     "remote-machine" must be of the form "user@machine" if the username
     on the remote machine is different.</li>
     <li> <code>--pull-ssh remote-machine remote-OKplatform-dir arguments</code>
     pulls via ssh from an arbitrary OKlibrary-repository. If
     "remote-OKplatform-dir" contains characters only meaningful on the remote
     machine, then it is to be put in quotation marks. The "remote-machine"
     must be of the form "user@machine" if the username on the remote machine
     is different. </li>
    </ol>
   </li>
   <li> <code>--create-package</code> calls the ReleaseProcess makefile
   with trailing arguments, creating three packages in the directory given by
   variable packages_dir (current value is <code>$(packages_dir)</code>),
   one basic, one including documentation, and one full package including
   the external sources. Special settings (supplied as parameters) are:
    <ol>
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

  Setting of configuration variables:
  <ul>
   <li> If configuration is needed, for example using Maxima with a different
   Lisp, then specify the corresponding make-variables <em>before</em>
   the <code>oklib</code> call (on the command-line), e.g.
   \code
OKplatform> maxima_lisp_name_okl=clisp oklib --maxima
   \endcode
   </li>
   <li> In case <code>oklib</code> invokes a make-file, and thus all parameters
   are passed to the make-process, in principle configuration-variables can be
   also placed as arguments of <code>oklib</code>, however if the string has
   spaces in it, then the quotes will break if not doing tricky things. So
   also here specifying the configuration-variable(s) as environment-variable
   before the <code>oklib</code>-call is needed, e.g.:
   \code
OKsystem> Test_tool="valgrind --quiet" oklib all check new_check app_tests html test_level=full
   \endcode
   </li>
  </ul>

*/

