// Oliver Kullmann, 13.6.2007 (Swansea)
/* Copyright 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/OKlibBuilding/plans/TargetSpecifications.hpp
  \brief Precise definitions of the targets for the build-system

  See Buildsystem/OKlibBuilding/plans/general.hpp for overall todos.


  \todo General look and feel
  <ul>
   <li> The directory structure is "recursive", names with capital first
   letters for (sub-)modules, while "systematic directories" have all
   small letters. </li>
   <li> Yet we can call for example "make -f Path/makefile all" from
   anywhere, and it will have always the same effect (that is the role
   of variable srcdir) --- will this be automatically achieved with
   the makefiles created by cmake? DONE (with the masterscript oklib
   we get full control over the effect of the calling place) </li>
  </ul>


  \todo New target for main action
  <ul>
   <li> Yet very often "oklib all check new_check app_tests html" is called.
   </li>
   <li> The should be a super-target for that; how to call it? "update" ??
   </li>
   <li> Perhaps "build_test_docu". </li>
   <li> Perhaps better "buildtestdocu": shorter to write, and we use such
   names for targets in general. </li>
   <li> Then however the order of processing is changed, due to the recursive
   makefiles: in every part all of "all check new_check app_tests html" is
   performed, before going to another part! </li>
   <li> Yet, we can not express dependencies directly, but they are somehow
   handled by the global order; now there could be interferences? </li>
   <li> Likely "check" and/or "new_check" needs "all" to be completed? </li>
   <li> So perhaps this new target is only to be used in "non-critical"
   contexts? </li>
   <li> So perhaps we postpone the introduction of this new target, until
   we have a better grasp on the dependency problems. </li>
   <li> Compare "Test system" in Buildsystem/OKlibBuilding/plans/general.hpp
   for a similar problem; don't we have a global todo regarding the
   consideration of all dependencies? </li>
  </ul>


  \todo Target "all":
  <ul>
   <li> Prerequisites: targets "apps", "demos", "exp" (for experiments). </li>
   <li> Let Dir be the directory where the master-make-script is present:
    <ol>
     <li> All .cpp-files below Dir in directories
      <ol>
       <li> apps </li>
       <li> demos </li>
       <li> exp </li>
      </ol>
      must be programs and are compiled (respectively for the sub-targets).
     </li>
     <li> The generated programs go to bin/Dir/path, where path is the
     relative path in Dir to the .cpp-file. </li>
     <li> Perhaps bin has subdirectories apps, demos, exp, and inside we
     put the binaries to Dir/path', where path' has been cleaned resp.
     from "apps", "demos", "exp". ?! Looks better. </li>
     <li> The .o-files are not of importance here. </li>
    </ol>
   </li>
   <li> Similar for .c-files. </li>
   <li> Customisation of compiler and linker options:
    <ol>
     <li> The link-libraries for a .cpp-file with %main() are collected from
     the link-libraries of the .cpp-files linked to it,
     and from the link-library-file for this .cpp-file itself. </li>
     <li> The (recursive) make-variable source_libraries is kept,
     predefined as "$(OKSystem_include) $(Boost_include)" (potentially
     changed in the locale definitions-makefile): If file .source_libraries
     exists, then it overrides "$(source_libraries)". </li>
     <li> Every .cpp-file has a .source_libraries file (if needed). </li>
     <li> We need to customise naming of generated executables, by a new
     make-variable which allows to override the current (automatic) setting
     of the -o-option. </li>
     <li> More precisely, when compiling "programs" then there need
     to be two versions of the action, one with "-o $@" as now,
     one using "-o $(program_name_okl)" resp.
     "-o $(program_name_optimised_okl)". </li>
     <li> Or can this be done by simply using "-o" a second time, re-defining
     the name? The question here is whether the first or the second usage
     of "-o" is decisive. </li>
     <li> DONE : Every .cpp-file has a .link_libraries and
     .link_libraries_optimised file (if needed), and a .compiler_options
     and a .compiler_options_optimised file (if needed). </li>
    </ol>
   </li>
   <li> What to do with the unoptimised and the optimised versions?
    <ol>
     <li> The names of the created .o-files and executables should reflect
     "all" compiler options. </li>
     <li> There are generic links to the unoptimised and the optimised
     version. </li>
    </ol>
   </li>
   <li> What to do if we create link-libraries in the OKlibrary? Perhaps
   this should go into another systematic subdirectory, say, "libraries" ?
   The created libraries then go to lib/Dir/path'. where path' has been
   cleaned from "libraries". </li>
   <li> For every created "file.o" and "file" we have the
   "file.compilation_log" (in the same directory where these files go).
   Optionally we can switch it off. </li>
   <li> There is a make-variable for optional name extensions. </li>
  </ul>


  \todo Target "check"
  <ul>
   <li> Compiles and runs all tests (unoptimised and optimised). </li>
   <li> The tests are exactly those in directories "testobjects" (which
   contain only .cpp-files (without %main())). </li>
   <li> With "check" all new .o-files are created, and the new (single)
   test-executable only links these new .o-files together. Optionally we can
   also link all .o-files together (new and old ones). Of course, if we create
   a test for the whole library, then the executable will be big, but that's
   how it is. Since we have dynamic test-circumstances (different
   testobject-files linked together), which might, due to global dependencies,
   influence the test-outcome, we need logging of the precise
   compilation-process. </li>
   <li> The test-programs go to subdirectories in bin/tests, reflecting
   appropriately the source-directory structure. </li>
   <li> Subtargets "test" and "testop" for compiling and running the
   unoptimised resp. optimised tests. </li>
   <li> For just compiling the tests there are subtargets "test_compile"
   and "testop_compile" (combined "check_compile"). </li>
   <li> For just running the tests there are subtargets "test_run" and
   "testop_run" (combined "check_run"). For cleaning the tags here we have
   "clean_test_run" and "clean_testop_run" (combined "clean_check_run"). </li>
   <li> It seems we should create a special target "valgrind-check" where the
   files are especially compiled for Valgrind --- this seems to be needed to
   do automatic checks.
    <ol>
     <li> valgrind has a (new) flag "--error-exitcode=", by which we should
     be able to get an error making the build-process stop in case valgrind
     finds an error. So perhaps special compilation is not needed? </li>
     <li> We should use "gcov" from time to time (supported by the build
     system) to check whether at least all lines of code are covered by the
     tests. </li>
    </ol>
   </li>
  </ul>


  \todo Target "prebuild"
  <ul>
   <li> Creating symbolic links (for the master-makefile-script), some
   directories, and all preparations which are needed to get the build-system
   running at all. Needed when installing the system. </li>
  </ul>


  \todo Target "html"
  <ul>
   <li> See "Definitions for doxygen" in Buildsystem/plans/Configuration.hpp
   for the dependency on the doxyfile. </li>
  </ul>


  \todo Target "app_tests"
  <ul>
   <li> We should have a general set-up to test SAT solvers w.r.t. the
   standard SAT functionality. </li>
   <li> Including various "fuzzing" tools; see "Fuzzing" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp. </li>
   <li> Of course, we should also involve our own generators. </li>
  </ul>


  \todo DONE (likely not needed, since at least for now we don't use cmake)
  Target "reset"
  <ul>
   <li> This target is handled by the master-script, and it reruns cmake
   (while all other targets are passed to the created make-files --- this
   assumes, that cmake was run at least once, so perhaps reset is also part of
   "prebuild" ??). </li>
  </ul>


  \todo Target "new-module"
  <ul>
   <li> Creates a new module (with the appropriate sub-directories, links,
   etc.), and runs "reset". </li>
  </ul>

*/
