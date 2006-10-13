// Oliver Kullmann, 28.2.2006 (Swansea)

/*!
  \file Buildsystem/plans/makefile_generic.hpp
  \brief Plans for the generic makefile

  \todo Documentation (in makefile_generic.mak):
    - Full overview on the parameters for makefile_generic (including the environment
      variables).
    - Checking on the existing documentation, and updating if necessary.
    - What is required from those makefile.definitions.mak ? Update the list in
      makefile_generic.mak, and reflect on it.
    - Adding inline comments for an overview on functionality.
 
  \todo Make-variables for compilation and linking:
    - instead of specifying a specific alternative gcc or boost version, it
      should be possible to use the recommended version
    - general clean-up of make-variables
    - the current function of "General_options" is taken over by the new variable "Debug_options"
    - Which compiler options are effective when linking?
    - CXXFLAGS is not used when linking the compilation units together --- is this
      how it should be, and how to set options for the linking stage?!
    - SystemDirectories gets a new sub-directory log, where the current
      aux/DoxygenErrorMessages is placed. Every run of make copies it
      output by default (can be switched off) into a file
      log/makefile_generic resp. log/makefile_ExternalSources resp. log/makefile_buildsystem.

  \todo Cleaning:
    - We need cleaning tools which clean up directories (not single files).
    - We need specialised cleaning for applications and link-libraries.
    - We need specialised cleaning regarding the test system :
    - Cleaning of special or all versions of the test-timestamps.
    - Cleaning of test-objectfiles and test-programs.
    - Cleaning of test-depencies.

  \todo Test cleaning:
    We need specialised cleaning regarding the test system :
    - Cleaning of special or all versions of the test-timestamps.
    - Cleaning of test-objectfiles and test-programs.
    - Cleaning of test-depencies.

  \todo Compilation:
    - The names of the created .o-files and executables should reflect "all" compiler options.
    - There are generic links to the unoptimised and the optimised version (the latest).
      The test system uses theses.
    - For every created file.o and file we have file.compilation_log (in the same directory
      where these files go). Optionally we can switch it off.
    - There is a make-variable for optional name extensions.

  \todo Linking:
    - The build system should find out whether the platform is 32- or 64-bit, and select then only
      the needed links (environment variables HOSTTYPE and CPU).
    - What is the meaning of the strip-binutil-tool? Shall we use it? (Always? Sometimes?)
      At least we should have the option.

  \todo Directory structure:
    - A module can have arbitrary submodules (with capital names) for (only) .hpp-files, each with its own
      generic sub-directories "plans", "tests" and "messages". However only at the base level we have the
      sub-directories "testobjects", "applications" and "implementations", which contain (only) .cpp (and .c) files.
      Every file in applications implements main(), and no other files. Compiled testobject-files go to lib/tests/Module,
      compiled implementations go to lib, compiled test-programs go to bin/tests, compiled applications go to bin.
      Thus names for implementation files and application files need to be unique for the whole library, while
      names for testobject-files need to be unique within a module

  \todo Test system:
    - The most immediate thing is to make the test system work correctly, which (at least?)
      means that the make-system gets to know the relevant dependencies, so that recompilation
      of tests happens whenever necessary (at this time (18.6.2006) the test object files need
      to be manually deleted (via "make cleantestobj") to enforce recompilation).
    - We need special test-modes for more extensive messages.
    - The output of testprograms is copied into a suitable subdirectory of SystemDirectories/log.
      Perhaps also the test time-stamps should go here?

  \todo Customisation: 
    - We keep our two-stages process: Every .cpp-files yields a .o-file, and finally exactly those .cpp-files with main()
      yield executables (the same for .c).
    - Every .cpp-file has a .source_libraries file (if needed), a .link_libraries and .link_libraries_optimised file (if needed),
      and a .compile_options and a .compile_options_optimised file (if needed).
    - The link-libraries for a .cpp-file with main() are collected from the link-libraries of the .cpp-files linked to it,
      and from the link-library-file for this .cpp-file itself.
    - The (recursive) make-variable source_libraries is kept, predefined as "$(OKSystem_include) $(Boost_include)" (potentially
      changed in the locale makefile): If .source_libraries exists, then it overrides $(source_libraries).

  \todo Placement of makefile_generic:
    - Except of in Buildsystem, all other makefile_generic-versions should be links. See makefile_recursive.
    - A problem here is, that it seems that links are not handled by CVS ?
    - We should perhaps call these (files resp. links to) makefile_generic "makefile_generic.mak", and
      not just "makefile" (even if it's a bit inconvenient) ?!
    - Currently makefile_generic takes special actions to ensure that it works the same
      from wherever we call it. This seems to create some trouble, and doesn't seem
      to be compatible with using links? So it should be abandoned? But then effectively
      it doesn't make sense to call makefile_generic from another place, and one always
      has to use cd first. One issue here is that the testsystem cannot any longer use
      any local directories (since it won't find them); perhaps the solution is to provide
      some central place for testdata.

  \todo Targets:
       - It should be possible to build just one application, or just one implementation.

  \todo Error messages of gcc should be processed:
    - We should support using a tool like TextFilt or STLFilt.

  \todo Tools for testing:
    - valgrind has a (new) flag "--error-exitcode=", by which we should be able to get an error
      making the build-process stop in case valgrind finds an error.
    - We should use gcov from time to time (supported by the build system) to check
      whether at least all lines of code are covered by the tests.

  \todo Dependency files:
    - If when creating the .d-files an error occurs (for example
     due to an inaccessible header file), then for some reason subsequent "make check" erroneously succeeds.
    - Using the gcc option "-MP" ?
    - We have the following problem:
      If one is using different paths due to symbolic links, then the dependency files contain
      unusable information (and must be deleted with "make cleandep").
      This problem seems hard to solve (one had to find out that different paths lead to the same file).
      So it must be documented well.

  \todo Verbosity:
    - By default the make-process outputs it's basic assumptions (what compiler to be used, what
      external libraries, etc.).
*/
