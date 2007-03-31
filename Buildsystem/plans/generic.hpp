// Oliver Kullmann, 28.2.2006 (Swansea)

/*!
  \file Buildsystem/plans/generic.hpp
  \brief Plans for the generic makefile

  \bug Mixing C and C++:
  If we have files XXX.c and XXX.cpp (with the same prefix) then
  the build-system apparently does not work properly.
  Either this gets corrected, or at least documented
  (it appears we should forbid this).
  The build-system should also warn about this (and perhaps
  also about other assumptions not met).

  \bug Incorrect build of new tests I:
  The buildsystem does not handle the dependencies for the
  module-test-program.

  \bug Incorrect build of new tests II:
  While error I is as expected (at this time), the buildsystem behaves
  differently when called within a module or from a higher level:
   - Target new_check works from for example from level OKsystem w.r.t.
     building test-object files if needed,
   - but from within the module this doesn't work.

  \todo Specification of build system II
  <ul>
    <li> Moving stable documentations to "docus":
      <ul>
       <li> Buildsystem/generic.mak: </li>
       <li> ExternalSources.mak: </li>
      </ul>
      And others.
    </li>
    <li>Targets
      <ul>
       <li>
        The targets should be more precisely specified. For example, in the 
        documentation of the targets all, optimised and unoptimised we need 
        to speak about compilation of *test*-programs!.
       </li>
      </ul>
    </li>
    <li> Specify the functions of the build system as precise as possible. </li>
  </ul>

  \todo .source_libraries
  - Like we have for link libraries, the specification of source libraries should
  be (potentially) given in a .source_libraries file.

  \todo Setting the paths to GCC and Boost link libraries
    - The setting of the Make variable "alternative_library_path" needs to be reviewed.
      For example, the name "alternative_library_path" is no longer appropriate.
    - The build system should find out whether the platform is 32- or 64-bit, and select then
      only the needed links (environment variables HOSTTYPE and CPU)
  
  \todo User control of system versions
    - There should be a simple method for the user to specify that the buildsystem
      uses the system-wide installations of GCC and Boost. It should be possible
      to specify only the system-wide GCC or only the system-wide Boost, or both.
  
  \todo General_options:
  The current function of "General_options" is taken over by the new variable "Debug_options".

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

  \todo Directory structure:
    - A module can have arbitrary submodules (with capital names) for (only) .hpp-files, each
      with its own generic sub-directories "plans", "tests" and "messages". However only at the base level
      we have the sub-directories "testobjects", "applications" and "implementations", which contain (only)
      .cpp (and .c) files. Every file in applications implements main(), and no other files. Compiled
      testobject-files go to lib/tests/Module, compiled implementations go to lib, compiled test-programs go
      to bin/tests, compiled applications go to bin. Thus names for implementation files and application files
      need to be unique for the whole library, while names for testobject-files need to be unique within a module.
    - Or we allow more freedom --- seems to be needed!

  \todo Test system:
    - The most immediate thing is to make the test system work correctly, which (at least?)
      means that the make-system gets to know the relevant dependencies, so that recompilation
      of tests happens whenever necessary (at this time (18.6.2006) the test object files need
      to be manually deleted (via "make cleantestobj") to enforce recompilation).
    - We need special test-modes for more extensive messages.
    - The output of testprograms is copied into a suitable subdirectory of SystemDirectories/log.
      Perhaps also the test time-stamps should go here?

  \todo OKsystem/Buildsystem structure:
  <ul>
  <li>Probably the directory Generic should be elimated and it's subdirectories lifted to OKsystem/Buildsystem?</li>
   <li>In Transitional/Buildsystem the two makefiles 
    <ul>
       <li>OKsystem/Buildystem/Generic/documentation_index.mak</li>
       <li>OKsystem/Buildystem/Generic/doxygen_documentation.mak</li>
     </ul>
     should be merged into a single file  Transitional/Buildsystem/documentation_building.mak 
   </li>
   <li>
    All the plans files should be merged into a single documentation_building.hpp plans file
    in Buildsystem/plans.
   </li>
  </ul> 

  \todo Compilation:
    - The names of the created .o-files and executables should reflect "all" compiler options.
    - There are generic links to the unoptimised and the optimised version (the latest).
      The test system uses these.
    - For every created file.o and file we have file.compilation_log (in the same directory
      where these files go). Optionally we can switch it off.
    - There is a make-variable for optional name extensions.

  \todo Customisation: 
    - We keep our two-stages process: Every .cpp-files yields a .o-file, and finally exactly those .cpp-files with main()
      yield executables (the same for .c).
    - Every .cpp-file has a .source_libraries file (if needed), a .link_libraries and .link_libraries_optimised file (if needed),
      and a .compile_options and a .compile_options_optimised file (if needed).
    - The link-libraries for a .cpp-file with main() are collected from the link-libraries of the .cpp-files linked to it,
      and from the link-library-file for this .cpp-file itself.
    - The (recursive) make-variable source_libraries is kept, predefined as "$(OKSystem_include) $(Boost_include)" (potentially
      changed in the locale makefile): If .source_libraries exists, then it overrides $(source_libraries).


  \todo Log directory:
  SystemDirectories gets a new sub-directory log, where the current
  aux/DoxygenErrorMessages is placed. Every run of make copies it
  output by default (can be switched off) into a file
  log/makefile_generic resp. log/makefile_ExternalSources resp. log/makefile_buildsystem.
  <ul>
   <li> One possibility to achieve this is by letting makefile_recursive call makefile_generic
   with appropriately redirected output. </li>
  </ul>
  
  \todo Targets:
  - It should be possible to build just one application, or just one implementation.    

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

  \todo Error messages of gcc should be processed:
    - We should support using a tool like TextFilt or STLFilt.

  \todo Tools for testing:
    - valgrind has a (new) flag "--error-exitcode=", by which we should be able to get an error
      making the build-process stop in case valgrind finds an error.
    - We should use gcov from time to time (supported by the build system) to check
      whether at least all lines of code are covered by the tests.

  \todo Linking and options:
  <ul>

  <li>We need global control over dynamic/static linking. Is it the case that dynamic
  linking only happens with .so files? Can investigate with strace tool..  </li>

  <li>Is it possible to specify the path exactly to a link-library at runtime?</li>

   <li>
   Which compiler options are effective when linking? "Our" options are 
   <ul>
    <li> -ansi (cc1plus) </li>
    <li> -pedantic (cc1plus)</li>
    <li> -Wall (cc1plus)</li>
    <li> -g (cc1plus) (mentioned as "ignored" in the ld man page)</li>
    <li> -03 (cc1plus)</li>
    <li> -DNDEBUG (cc1plus)</li>
   </ul>
  
   Here, the names in brackets indicate, in the
   case of ld (linker) and cpp (preprocessor), the executables whose man 
   page documents the option and, in the case of cc1plus whether the help
   file documents the option.
  
   It seems, at first glance, that all of "our" options are only effective
   for the compiler.</li>
  
   <li>
   CXXFLAGS is not used when linking the compilation units together --- is this
   how it should be, and how to set options for the linking stage?!
   </li>

   <li>
   What is the meaning of the strip-binutil-tool? Shall we use it? (Always? Sometimes?)
   At least we should have the option.
   </li>

  </ul>

*/
