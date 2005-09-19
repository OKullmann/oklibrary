// Oliver Kullmann, 12.8.2005 (Swansea)

/*!
  \file Buildsystem_Plans.hpp

  \todo makefiles in general should always have the suffix .mak,
  so that for examples xemacs regocnises the format.

  \todo Doxygen:
   - Can makefiles be incorporated?!
   - How to integrate a general todo list into Doxygen?!
   - How to avoid that a leading "include" in a Doxygen-comment is interpreted as
     a doxygen-command?

  \todo Create a new target ("check_valgrind")
  which runs the test with "valgrind --leak-check=yes" (possibly with
  other options as well; perhaps as default the option "--quit").

  \todo ExternalSources:
   - When buildinng some gcc- or some boost-version, only the necessary directories
     should be created (so that looking at the list of directories one sees what is
     installed; at present always all directories for all gcc- and boost-version are
     created).
   - Any build will be considered as already completed, if the "finally created directory"
     is already there.
   - "make gcc_all" will create all gcc-version, while "make gcc" will create
     the recommended version.
   - When building boost (in some variation) using "gcc-version=...", then as a subtarget
     we have the build of the gcc-version (so that, if necessary, gcc is build).
   - "make boost_all" respectively "make gcc-version=... boost_all" will create
     all boost versions, while just using "boost" will only create the recommended version.
   - "make gcc_boost_all" will create all boost versions for all compiler versions (including
     the global one).
   - When making the (final) installations, links should be used instead of copies (making
     copies as an option; default is links).
   - Optionally there should be also local versions of valgrind and doxygen.
   - "make doxygen", "make valgrind", "make postgresql" will create the recommended
     version.
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
  allows to monitor these measurements (as the evolve over time).
*/
