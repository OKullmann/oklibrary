// Oliver Kullmann, 12.8.2005 (Swansea)

/*!
  \file Buildsystem_Plans.hpp

  \todo makefiles in general should always have the suffix .mak,
  so that for examples xemacs regocnises the format.

  \todo Overhaul of the general targets:
   - "all" should not compile the test-programs
   - what is the role of prebuild?
   - it should be possible to build just one application
   - there should be special versions of "clean" which delete only applications
     or only the test programs
   - "html" should be possible from any level (creating always the complete documentation).

   \todo Each "make test" etc. should gather summary statistics of the tests performed,
   like the total number of testobjects, the total time spend and so on.
   In order to do so, a test program can be asked to serialise the statistics
   to a file or to standard output, and a simple evaluation program gathers
   these statistics.

   \todo Similar to the complexity measurements, for every task performed by the build
   system it should be possible to save the running time, so that the develepment over
   time for example of compile times, link times, test times can be followed, and also
   the influence of compilers can be considered. By default, when running make only
   the total times are output, but in a protocol mode everything is written to a file
   (as for the complexity system; it should be possible for example to use the visualisation
   tools there to look at the developments here).

   \todo Error in the build system: If when creating the .d-files an error occurs (for example
   due to an inaccessible header file), then for some reason subsequent "make check"
   erroneously succeeds.
   
   \todo makefile_generic
    - Updating the comments.
    - From makefile.definitions.mak only "Root" is to be extracted, while the rest is handled by
      this makefile, inspecting its directory and all subdirectories,
      and collecting the required information individually from
      all .hpp and .cpp files by means of annotating files (we have it already for link libraries,
      now we need it also for source libraries and for the compiler options; the test program
      always is called "TestModule" (with "Module" to be replaced by the corresponding module
      name), and the remaining .cpp-files are the applications).
    - "General_options" should become "Debug_options" (if used at all).
    - CXXFLAGS is not used when linking the compilation units together --- is this
      how it should be, and how to set options for the linking stage?!
    - The names of the created applications should reflect all compiler options.
    - We have the following problem:
      If one is using different paths due to symbolic links, then the
      dependency files contain unusable information (and must be
      deleted with "make cleandep").
      This problem seems hard to solve (one had to find out that
      different paths lead to the same file).
      So it must be documented well.

  \todo Doxygen:
   - Can makefiles be incorporated?!
   - How to integrate a general todo list into Doxygen?!
   - How to avoid that a leading "include" in a Doxygen-comment is interpreted as
     a doxygen-command?
   - How to obtain general statistics?

  \todo We need a standardised way of how to make information about the compilation
  process available to a program (and also the name of the program, etc.), so that
  for example via --version we get as much information as possible.

  \todo Special runs
   - Perhaps we create also a special target to compile with "-Wall" (possibly
     also other warnings enabled); from time to time checking the warnings
     is useful (though we DO NOT aim at eliminating the warnings; the
     only thing what we possibly do about spurious warnings is that we tell
     the user about them, so that they can ignore them).

  \todo ExternalSources:
   - When building some gcc-version, only the necessary directories
     should be created. This should be handled as we have it now with boost (using
     timestamp-files).
   - When building boost (in some variation) using "gcc-version=...", then as a subtarget
     we have the build of the gcc-version (so that, if necessary, gcc is build).
   - Installation of bjam should be improved: Having exactly one bjam-installation for each boost-version,
     and no need to recreate it if it's already there. Or, perhaps better: We just leave it in
     the distribution directory?
   - It would ge good, if after doing a local installation, easily the installation could also be
     make global.
   - If variable "gcc-version" is set, then it should have one of the allowed values (while otherwise we get
     an error).
    - Optionally there should be also local versions of valgrind and doxygen.
   - "make initialise-database" should work with the recommended version.
   - Build a local version of gmp.
   - Update PostgreSQL to version 8.1 (and test it).
   - Build the R system (locally and globally).

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
  Testing should invoke valgrind (with Test_tool="valgrind --quit").

  \todo Improving the directory structure (which yet is "flat"): Inclusion
  from for example Transitional should happen as, e.g.,
  #include "OKlib/Transitional/SumProduct/Marginalisation.hpp".
  (Thus compilation of local copies of the library then only work with
  the *standard place* (i.e., OKplatform/include).) This is achieved by
  eliminating the include-directory at OKplatform-level (and all associated
  environment- and make-variables), and including then from level
  OKsystem (which thus needs the subdirectory "OKlib" with its own recursive makefile).

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
