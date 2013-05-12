// Oliver Kullmann, 4.6.2007 (Swansea)
/* Copyright 2007, 2010, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/OKlibBuilding/plans/MakeSystem.hpp
  \brief Plans for the core part of the build system, the
  make- or make-like engine


  \todo Makepp
  <ul>
   <li> See http://makepp.sourceforge.net/ . </li>
   <li> A disadvantage is the use of Perl, but perhaps soon with Git we also
   get this dependency. </li>
   <li> However, currently the make-functions "call, error, warning" are
   not supported, so it seems we can't use it in its current form. </li>
   <li> Otherwise its features (automatic scanning for include-files, a
   stronger system for hierarchical builds, handling of conditional
   compilation) should be very useful. </li>
   <li> Though it seems to be a rather small project. </li>
  </ul>


  \todo DONE (we don't use CMake ourselves, and let the Linux distribution
  provide it)
  CMake
  <ul>
   <li> The following needs a complete update, considering whether CMake gained
   more power in these 3 years, and regarding the changes in our buildsystem.
   </li>
   <li> The basic idea of using cmake despite of the problems described below
   (see also the e-mail to the CMake-mailing list from 7.6.2007): Use make to
   run the cmake-process! In Detail:
    <ul>
     <li> As now, every module where we wish to have build-functionality
     contains a symbolic link to a generic makefile (a new one --- just a very
     simple one (hopefully)). Perhaps we call this makefile the "master
     makefile". </li>
     <li> Such a module contains a .build-subdirectory (seems better to hide
     it, since normally one doesn't want to look into it). </li>
     <li> Creating a new directory is now a target for the master makefile: It
     creates the .build-directory, places inside it the cmake-file, and runs
     the cmake-process. </li>
     <li> For the normal tasks like "all" or "check" the master makefile just
     calls the (created) makefile in .build. </li>
     <li> While adding a new file is now also a task of the master makefile,
     which calls the cmake-makefile in .build (to recreate the makefile in
     .build). </li>
     <li> For this to work we need the capability of cmake to place the
     cmake-makefile as well as the makefile somewhere else than in the
     source-directory. </li>
     <li> Having the .build-directory in the source-tree creates considerable
     space overhead (relevant when copying the source-tree, as it happens when
     using it on the laptop), and likely it is not advisable to have
     build-information in the source-tree: So, given that we are free where to
     place the cmake-file, we should place the build-directory in
     system_directories/aux (or system_directories/build?). </li>
     <li> What is the cmake-file in .build? Three possibilities:
      <ol>
       <li> A copy of the generic cmake-file. </li>
       <li> A symbolic link to the generic cmake-file. </li>
       <li> A created cmake-file (created from a generic template and some
       parameters). </li>
      </ol>
      Obviously, creating a customised cmake-file is most powerful, but one
      needs good reasons for that --- and a good design, with a clear idea
      about the different levels of indirection! </li>
     <li> The aim is to use in this way all the build-in facilities of cmake,
     while overcoming its restricted power. </li>
     <li> If cmake has not enough power for computing (and manipulating)
     file-lists, then the master makefile could compute them and pass them to
     cmake; a problem here could be that this must happen via the command line
     (?), and then there is the restricted command-line length?!? </li>
     <li> The additional level of indirection (given by the master makefile)
     seems necessary, since we want complete automisation, while cmake itself
     does nothing than creating makefile --- and the makefiles created are not
     "dynamic" but "static" (rather restricted in power). The abstract
     division of work between the three layers can be understood as follows:
      <ol>
       <li> The master-makefile-level does not do real building, but only its
       fileprocessing capabilities are needed (so actually a shell-script
       could be used?!). </li>
       <li> The cmake-level has the knowledge of how to build things. </li>
       <li> Finally, the created makefiles are purely static, and master
       dependencies (and, or course, actually perform the build-actions). </li>
      </ol>
      One should think about using a shell-script instead of a
      master-makefile! The main disadvantage seems to be that the syntax
      "make target" couldn't be used. </li>
    </ul>
   <li> Can cmake handle our "fractal" or "recursive" directory structure,
   where we have arbitrary nested subdirectories, inside we find standard
   functional (sub-sub-)directories like "tests", "testobjects", "demos",
   "apps", "messages", "plans", "docus", and from these functional directories
   the source-code files are to be extracted, and name-surgery is applied to
   compute the corresponding directories under system_directories ?!? </li>
   <li> Can cmake handle the uniformity of our system, where for OKlib we only
   have 2 makefiles (OKlibBuilding/Makefile and Recursive.mak), and every
   directory contains (if wished) only a symbolic link to one of those
   (utilising then the definitions.mak files) ?!? We do not want to clutter
   the source directory with code for the build system, but at any point we
   want to be able to issue a make-command. </li>
   <li> Can cmake handle the dynamic aspects of our build system, where with
   every make-command all the programs and libraries to be compiled and all
   executables to be executed are first determined dynamically, namely what
   exists currently in the source-code tree?!?
    <ol>
     <li> The possibilities to manipulate variables seem very restricted??
     There seem to exist no functions??? </li>
     <li> What is needed is a "meta-make", which from configuration values,
     parameter values and the current state of the file-system first creates
     all targets, corresponding names and paths etc., and then creates the
     make-file. One would guess that this exactly should be possible with
     cmake?? With make we have the possibility due to the two-stages concept,
     but it's hard to create the dependencies on the target. </li>
     <li> So ADD_EXECUTABLE and ADD_LIBRARY depend on the output of a
     shell-command (typically "find") --- this doesn't seem possible? But with
     FOREACH and WHILE it should be possible. </li>
    </ol>
   </li>
   <li> It seems one doesn't have fine-grained control over the separation of
   the cmake-specific files and the library- and binary-files? Apparently
   cmake expects the user to issue a final "install" command, which will put
   the lib- and bin-files to their "final" destination?!? But we have
   a continuous process! </li>
   <li> It seems that the basic idea between the higher-level "cmake" and the
   lower-level "make" is here, that every time a new files is introduced (or
   something similar), one must run cmake, while otherwise one can just run
   make. Seems not unreasonable, but of course one introduces a further
   possibility for making mistakes ?!? </li>
   <li> It seems that the special makefile for external sources is the best
   candidate for employing cmake; see "Using CMake" in
   Buildsystem/ExternalSources/plans/general.hpp. </li>
   <li> Investigate how our main tasks can be handled by cmake. </li>
   <li> How would the work flow be with the new system (the 2 levels,
   the cmake- and the make-level)? </li>
   <li> The cmake-files should use decomposition into smaller cmake-files
   (this should now be easier), and we also need a library of cmake-macros.
   </li>
   <li> Is ccmake useful? DONE (just a program for setting configuration
   variables, so yes, it is useful for that purpose) </li>
  </ul>


  \todo Competition extraction tools
  <ul>
   <li> These tools, for a given program build a package, which contains
   all necessary source code and can be build using make (producing then
   the executable). </li>
   <li> A problem here are the link-libraries. Without them, one could use
   the ability of g++ to produce compilation units (using the option "-E"
   like "g++ -E Program.cpp -o Program.ii"), putting everything into one
   big file. </li>
   <li> Knowing the link-libraries, just compiling Program.ii with the
   right .o-files yields the program. </li>
   <li> So when we know all compilation units, we can preprocess them all
   and putting them into the package; the package just compiles the units
   separately, and then links them together. </li>
  </ul>

*/

