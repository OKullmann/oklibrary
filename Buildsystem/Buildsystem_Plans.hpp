// Oliver Kullmann, 12.8.2005 (Swansea)

/*!
  \file Buildsystem_Plans.hpp

  \todo makefiles in general should always have the suffix .mak,
  so that for examples xemacs regocnises the format.

  \todo Overhaul of the general targets:
   - "all" should not compile the test-programs
   - what is the role of prebuild?

   \todo Error in the build system: If when creating the .d-files an error occurs (for example
   due to an inaccessible header file), then for some reason subsequent "make checks"
   erroneously succeeds.
   

   \todo Improved handling of flags:
    - CXXFLAGS and CPPFLAGS likely should be completely free for the user to use
      (yet the build system partially uses them).
    - CXXFLAGS is not used when linking the compilation units together --- is this
      how it should be, and how to set options for the linking stage?!

  \todo Doxygen:
   - Can makefiles be incorporated?!
   - How to integrate a general todo list into Doxygen?!
   - How to avoid that a leading "include" in a Doxygen-comment is interpreted as
     a doxygen-command?

  \todo Create a new target ("check_valgrind")
  which runs the test with "valgrind --leak-check=yes" (possibly with
  other options as well; perhaps as default the option "--quit").

  \todo ExternalSources:
   - When building some gcc- or some boost-version, only the necessary directories
     should be created (so that looking at the list of directories one sees what is
     installed; at present always all directories for all gcc- and boost-version are
     created).
   - Any build will be considered as already completed, if the "finally created directory"
     is already there.
   - When building boost (in some variation) using "gcc-version=...", then as a subtarget
     we have the build of the gcc-version (so that, if necessary, gcc is build).
   - For building boost, sub-directories of ExternalSources/Boost like 1_33 and 1_33+3.4.3 should
     be the "finally created directories", while all other directories should get removed with
     "make clean".
   - Using "gcc-version=all" means creating the required Boost version for all available gcc-versions
     (inclusing the global one).
   - "make boost_all" will create all boost versions (when combined with
     "gcc-version=all" we then get all combination of all boost libraries with gcc-versions).
   - If variable gcc-version is set, then it should have one of the allowed values.
   - "make clean" should remove for example all Gcc/gcc-* directories, leaving only the files which are really used.
     Thus for the final local installations, links cannot be used instead of copies; the exception are the boost
     header files and the boost libraries within the gcc-versions: Here we should use links instead of copies
     (perhaps making copies as an option).
   - Optionally there should be also local versions of valgrind and doxygen.
   - Optionally we can make a global version of Boost or gcc.
   - "make initialise-database" should work with the recommended version.
   - Build a local version of mhash.
   

  \todo "Full test": makefile at the OKplatform level --- there the
  different versions of gcc and boost are visible. "make check" at
  this level should run through all appropriate combinations of gcc
  and libraries and test them.
   - makefile at level OKplatform: default goal should run "prebuild all".
   - Writing the package-construction script. (Internal versions should
  have date and time in the name.)
   - "Nightly build": Full check-out of the library (yet Transitional and 
  OKlibrary) and full compilation and testing (i.e., create the package,
  un-archive it, build it with "make" and then run "make check" in it).

  \todo Improving the directory structure (which yet is "flat"): Inclusion
  from for example Transitional should happen as, e.g.,
  #include "OKlib/Transitional/SumProduct/Marginalisation.hpp".
  (Thus compilation of local copies of the library then only work with
  the *standard place* (i.e., OKplatform/include).) This is achieved by
  eliminating the include-directory at OKplatform-level (and all associated
  environment- and make-variables), and including then from level
  OKsystem (which thus needs the subdirectory "OKlib" with its own recursive makefile).

  \todo As the link-libraries are handled now we also want to handle inclusion
  of the source-libraries and handling of the options. (Options should go
  into the name!)

  \todo Complexity system: "make measurements" will create an xml-file
  (via the boost serialisation library) with information about all
  operations which have been registered. A little viewing-program
  allows to monitor these measurements (as they evolve over time).

  \todo New targets "create_new_module" and "create_new_submodule",
  which create a new subdirectory with test-program respectively new
  .hpp, _Tests.hpp and _Testobjects.hpp files (with additional inclusion
  in the testprogram).

  \todo Distributing the library:
   - We must study how to distribute a copy of the whole CVS-system (so that later a remerge is
   possible; also updates must be studied).
   - Two download possibilities: Either the whole library (with all included libraries like
   Boost, doxygen, PostgreSQL etc.), or only the minimum.
   This gives two user types: "full user" and "minimal user".
   We must make sure, that also the minimal user can use the test and the complexity system
   (compiling it himself, or using the build system). And we must study, how a full user can keep his
   file structure separate from the library by using links.
*/
