// Oliver Kullmann, 12.8.2005 (Swansea)
/* Copyright 2005 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/plans/general.hpp
  \brief Plans for the buildsystem in general


  \todo Evaluation mode for oklib
  <ul>
   <li> In the plans, one places shell commands referencing
   locations given by make variables. This is especially true
   when discussing new external sources. </li>
   <li> For example, see "Minion" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp . </li>
   <li> These commands can not be run directly in a shell. The
   make variables must be replaced by hand. </li>
   <li> We should have a "--evaluate" option for oklib. This option
   would evaluate the command given to it in the context of the
   BuildSystem environment. </li>
  </ul>


  \todo Making make-variables compatible with shell-variables:
  <ul>
   <li> To avoid problems, we should use C(++) naming conventions for
   make-variables, which means that in all make-variables a hyphen "-"
   is replaced by an underscore "_" (being careful not to introduce
   ambiguities). </li>
  </ul>


  \todo Further renamings:
  <ul>
   <li> Buildsystem/recursive_noneincluded.mak ->
   Buildsystem/Recursive_noneincluded.mak (perhaps also name should change)
   </li>
   <li> Buildsystem/standardgoals.mak -> Buildsystem/Standardgoals.mak </li>
  </ul>


  \todo System documentation
  <ul>
   <li> Document the basic version-control settings (location of server, configuration,
   how to use it). </li>
   <li> Document the build-system (general ideas, functionality). </li>
  </ul>


  \todo Renaming module Buildsystem
  <ul>
   <li> To be consistent with the other module names, it should be called
   "BuildSystem". </li>
   <li> And we should move the module to part System. </li>
   <li> But perhaps we leave it as it is --- it is a rather special system?
   </li>
  </ul>


  \todo Documentation (Examples and Concepts) OK : this needs to be discussed and updated
  <ul>
   <li> At OKplatform-level we have a new directory Documentation, with the
   following sub-directories:
    <ol>
     <li> Examples : contains a mirror of the OKlibrary with (many) example
     applications (build by the build system, so that it's always
     up-to-date). </li>
     <li> Concepts : contains for each module a latex-file discussing the
     concepts for this module.</li>
    </ol>
   </li>
  </ul>


  \todo Modes of Usage
  <ul>
   <li> Two modes of usage of the build system:
    <ol>
     <li> As we have it now, everything in one directory (and also with the
     possibility of having different versions of the OKplatform). ("One world",
     extending directly the library.) </li>
     <li> Having one global OKplatform, and then a local directory with also
     aux, bin, doc, include and lib, but only regarding the local directory.
     So that for example students in the Linux lab can use a central
     installation of OKplatform, and in the local directory only the files
     related to their production is stored. </li>
    </ol>
   </li>
  </ul>


  \todo makefile_recursive
  <ul>
   <li> This should go, and makefile_generic should be able to do all jobs,
   gathering all relevant files from all underlying subdirectories (but as soon
   as one subdirectory doesn't contain makefile_generic, then it and its
   descendants are ignored). </li>
   <li> It seems important that for example every subdirectory
   of a module can itself contain tests- and testobjects-subdirectories. </li>
   <li> The directory-tree in system_directories must mirror the primary
   directory-tree. </li>
  </ul>

   
  \todo Source code directory structure
  <ul>
   <li> We need a rational system for the naming of header files. We should
   study the Boost library here. </li>
   <li> The namespace should be the directory part of the name for the OKlib
   (for example OKlib::Algorithms is the file OKlib/Algorithms). </li>
   <li> Directory OKsystem/OKlibrary should then be called OKsystem/OKlib. </li>
   <li> As proposed in [C++ Templates], we should also provide files per module
   only containing declarations (perhaps parallel to each .hpp file). Ending
   "_decl" ? </li>
   <li> Module.hpp provides all includes, Module_decl.hpp all declarational
   includes. </li>
   <li> Perhaps special naming conventions for files with messages or
   exceptions ?! DONE --- we use sub-directories like "messages" instead of
   systematic name-extensions. Whether we also use sub-directories "exceptions"
   needs to be seen. </li>
   <li> Should we use <> or "" for includes from our library? DONE --- always
   use the "system header" form with angle brackets! </li>
  </ul>


  \todo Full test
  <ul>
   <li> makefile at the OKplatform level --- there the
   different versions of gcc and boost are visible. </li>
   <li> "make check" at this level should run through all appropriate
   combinations of gcc and libraries and test them. </li>
  </ul>


  \todo Integration testing:
  <ul>
   <li> We need some file format (likely some xml) to specify one integration test:
    <ol>
     <li> Specify the input file. (Possibly more input files.) </li>
     <li> Specify the program to be run with this file as input. </li>
     <li> Specify the command-line options for running the program. </li>
     <li> Specify the output-check (perhaps a regular expression?). </li>
    </ol>
   <li> Then we need a way to execute such integration tests and to report errors. </li>
   <li> Could we use the unit-testing framework here? Likely not. </li>
  </ul>


  \todo Compiler versions
  <ul>
   <li> Do we get slower run-times with g++ 4.1.2 compared to 4.1.1, and thus
   we need to write no-throw-declarations? (Perhaps this needs to be done
   anyway?) </li>
  </ul>


  \todo Version numbers
  <ul>
   <li> It would be good to have not only version numbers for modules,
   but also for single files (to indicate the state of
   development) ?! </li>
  </ul>

*/
