// Oliver Kullmann, 13.7.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/MasterScript/plans/general.hpp
  \brief Plans for the masterscript, the "user-interface" to the build system

  Similar to the use of "git" in a Git-repository, we use "oklib"
  in an OKplatform-installation.


  \todo Update of pulling and pushing facilities
  <ul>
   <li> Regarding the new public repository at Github, we should update and
   revise the push/pull facilities and their documentation. </li>
   <li> Compare with Buildsystem/SourceControl/plans/Github.hpp. </li>
  </ul>


  \todo Markers instead of links
  <ul>
   <li> Once oklib is available, the temporary definitions of OKplatform etc.
   in the makefiles shall be removed. </li>
   <li> The current system of deciding globally which makefile to select is too
   inflexible: In principle the old system (with its local signs) has its merits,
   only we do not need links anymore to the resp. makefiles, but each directory
   contains a specific marker --- initially this is just an empty file (only the
   name matters), but later it might be filled with additional information on
   targets to be compiled and on dependencies.
    <ol>
     <li> The problems with variable srcdir can easily be solved now ---
     simplest is perhaps to set this variable explicitly when calling a
     makefile. </li>
     <li> Yet the recursive makefile doesn't enter a directory if it doesn't
     contain a makefile; perhaps we get rid off the different forms of recursive
     makefiles altogether: for directories containing a marker "recursive.mak"
     oklib collects all paths through such dispensers until a marker for a
     "real" makefile is found, and then executes all such real makefiles. </li>
     <li> A technical problem here is that we want only to consider continuous
     chaines of dispensers (a missing one means that this directory is blocked):
     Via "find" we cannot realise it, while just searching for all paths to
     those markers for real makefiles is trivial via "find". One could write a
     little recursive programs for collecting all such paths. </li>
     <li> Perhaps easiest is that oklib calls itself recursively! </li>
     <li> So then oklib only enters action (if not given a special option) if
     either the current directory contains "recursive.mak" in which case oklib
     is called recursively on its subdirectories, or if it contains
     "definitions.mak" in which case the makefile for ExternalSources,
     OKlibBuilding or Annotations is called --- perhaps this is indicated in
     the name. </li>
     <li> On the other hand, as discussed in "Improved makefiles" below,
     perhaps the most general (and sufficient) model is actually just to ignore
     directories with no marker, but otherwise handle all directories below.
     </li>
     <li> Thus, whether oklib is implemented recursively, or works with a list
     created by "find", is an implementation detail. </li>
     <li> The marker "recursive.mak" would not be needed --- only positive
     markers
     (indicating local targets, and indicated which makefile is responsible)!
     </li>
    </ol>
   </li>
   <li> The content of the old files definitions.mak shall be made superfluous,
   so that at this time only an empty marker is needed. </li>
   <li> So by default always all directories below the calling directory are
   examined recursivly; by an option ("--non-recursive") this can be disabled.
   </li>
  </ul>


  \todo Improved makefiles
  <ul>
   <li> Five (partially new) main makefiles:
    <ol>
     <li> Buildsystem/ExternalSources/Makefile </li>
     <li> Buildsystem/OKlibBuilding/Makefile </li>
     <li> Buildsystem/Makefile </li>
     <li> Buildsystem/Html/Makefile (takes over targets html and internet
     from current Buildsystem/OKlibBuilding/Makefile) </li>
     <li> Buildsystem/Latex/Makefile (the current Buildsystem/Annotations.mak)
     </li>
    </ol>
    These should be updated according to their plans. </li>
   <li> In preparation for the truly global build system (which knows about
   all(!) dependencies and all rules), perhaps a new master-makefile
   "Master.mak" includes the configuration makefile and the above main
   makefiles? </li>
   <li> So the target-sets for the main makefiles then should be disjoint?
   For common targets like "all" we could introduce a scoping:
    <ol>
     <li> "esc" for ExternalSources </li>
     <li> "htm" for Html </li>
     <li> "lat" for Latex </li>
     <li> "lib" for OKlibBuilding </li>
    </ol>
    And there must be new "super"-targets like
    "all = htm::all + lat::all + lib::all". </li>
   <li> We need access to all files produced by the makefiles, so that we
   have the possibilities to express dependencies like:
    <ol>
     <li> linking dependencies within our library </li>
     <li> linking dependencies to external libraries </li>
     <li> dependencies from executables (for example for application tests)
     </li>
     <li> dependencies from tools </li>
    </ol>
    The goal is that the build system knows about all dependencies and all
    rules to resolve these dependencies, and if called at a level
    high enough (highest at OKplatform-level, with "oklib world") updates
    everything in perfect order (including external sources, compilers, tools,
    documentation). </li>
    <li> Such a dependency would typically be just a path, for example to a
    source-library directory, to a link-library, or to an executable; likely
    these paths should be full. A target like "boost" is then just translated
    into such full targets. </li>
    <li> So for example the expanded target "boost" (in at least the two
    specialised forms of source- and link-library) is a prerequisite at many
    places, but only in ExternalSources the makefile knows how to create/update
    this target (better "virtual makefile" --- managed by oklib). </li>
    <li> Every "place" (directory) has its own responsibilities, the targets it
    creates, and from what they depend (where possibly it doesn't know how to
    create/update the dependencies, since they belong into the responsibility
    of another place), and the master-makefile collects all this information
    from places below the calling place. Perhaps (for speed etc.) we still
    keep the old recursive mode alive (as an option). </li>
    <li> So we have the "recursive mode" which *calls* makefiles, and the
    "global mode" which *includes* makefiles. </li>
    <li> The current definitions.mak actually stand for created makefiles;
    perhaps we still maintain that level, have somewhere the created makefile
    available, and then for global processing we can just include all these
    local makefiles. In this way we could still design makefiles with only
    local responsibility?! The question then is how to glue them together
    on the overlapping targets like "all". In "global mode" these all's need
    to be qualified, and glued together by some super-all. </li>
    <li> In a created make-file we had definitions "srcdir=..." followed by
    "include OKlibBuilding/Makefile", for each relevant directory; perhaps
    another variable qualifies the generic targets. Such a construction should
    be constructible by a make-loop. </li>
    <li> We should experiment with this design; looks attractive, that we can
    get all theglobal power by just using (generalised) local makefiles (which
    care only about their home-directory, and not about directories below them).
    </li>
    <li> For the inclusion-model we have to solve a similar problem as in the
    call-model, namely how to arrange for inhibited directories. Perhaps all
    what we need is that certain directories are just inactive (qualified by
    not having definitions.mak), while still directories below that level might
    be active --- the current model, where we just "block" directories (which
    always includes everything below) is not flexible enough. </li>
  </ul>


  \todo Further enhancements
  <ul>
   <li> It seems we should remove the "command"-construction.
    <ol>
     <li> The point was the echo. </li>
     <li> This is applied inconsistently, and doesn't seem useful anymore. </li>
     <li> Otherwise it just causes complications. </li>
    </ol>
   </li>
   <li> And exit-values should reflect the last return-value. </li>
   <li> DONE (just use "maxima_lisp_name_okl=ecl maxima_recommended_version_number_okl=5.16.3 oklib --maxima further_arguments")
   As we have it now, passing all arguments to Maxima, we cannot pass for
   example different targets to the makefile --- we somehow need a (convenient)
   possibility to have both possibilities.
    <ul>
     <li> As an example, we want something like
     \verbatim
oklib maxima_recommended_version_number_okl=5.16.3 maxima_lisp_name_okl=ecl --maxima further_arguments_for_maxima
     \endverbatim
     (using here Maxima for a specific version with a specific Lisp). </li>
     <li> So one could search for "--maxima", and pass everything before as
     parameters to the make-file. </li>
    </ul>
   </li>
   <li> --help </li>
   <li> Write "oklib --sage", similar to "oklib --maxima".
    <ol>
     <li> First without anything else. </li>
     <li> Later we must add support for our Maxima-system. </li>
    </ol>
   </li>
   <li> DONE Git operations:
    <ol>
     <li> We handle special operations where specific settings from the
     OKlibrary are needed. </li>
     <li> But otherwise the users should use git himself: He needs to
     get used to git and its documentation, and doing it ourselfes might
     create trouble. </li>
    </ol>
   </li>
   <li> DONE (now all output is always both shown and copied to the log-file)
   It seems hard to avoid, and also not important, that if we interrupt
   the makefile for external sources, then the error messages are not
   printed --- we know it anyway. Nevertheless one could ponder again
   about it. </li>
   <li> DONE (now all output is always both shown and copied to the log-file)
   If oklib-building is interrupted, then nothing is shown (also not
   in the log-file); hard to avoid, and not really important.
   But perhaps in case everything went through, then a success report
   with date and time should be printed (totgether with the targets),
   so that one can see what's done. </li>
  </ul>


  \todo Add file template generation option
  <ul>
   <li> A command to generate the default files for a maxima file, C++ file
   etc would be very useful. </li>
   <li> For instance, when creating a new ".mac" file at the Maxima level,
   the file itself must be creator with the correct author, copyright notice,
   load and doxygen commands for that file, as well as the appropriate test
   and testobjects files again with the appropriate data inside. Much of
   these files could be automatically generated with a single command. </li>
   <li> There is also the possibility that information (such as the location
   of the author) or the inclusion of the files in the "include.mac" file,
   although not automatically generated, could be suggested to the user of
   the command as the command completes. </li>
  </ul>

*/

