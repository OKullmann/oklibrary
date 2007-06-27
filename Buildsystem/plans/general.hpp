// Oliver Kullmann, 12.8.2005 (Swansea)

/*!
  \file Buildsystem/plans/general.hpp
  \brief Plans for the buildsystem in general


  \todo Renaming:
  According to the general scheme, where all "genuine" files/directories
  carry capitalised names, the following files shall be renamed:
  <ul>
   <li> Buildsystem/plans/generic.hpp -> Buildsystem/plans/Generic.hpp </li>
   <li> Buildsystem/generic.mak -> Buildsystem/Generic.mak </li>
   <li> Buildsystem/recursive.mak -> Buildsystem/Recursive.mak </li>
   <li> Buildsystem/recursive_noneincluded.mak -> Buildsystem/Recursive_noneincluded.mak </li>
   <li> Buildsystem/standardgoals.mak -> Buildsystem/Standardgoals.mak </li>
   <li> Buildsystem/system_definitions.mak -> Buildsystem/System_definitions.mak </li>
  </ul>


  \todo system_definitions.mak: UPDATE --- some content goes to
  plans/Configuration.hpp!
  <ul>
   <li> The role of system_definitions.mak must be clarified.
   Do those library variables (Boost, Ubcsat) belong to it??
   <ul>
   <li> system_definitions.mak should
   contain definitions of Make variables which are used by
   several makefiles. However, the prefix "system_" also implies that 
   these variables belong to the OKlibrary and not to the external
   sources, so perhaps the definitions of variables relevant
   for the external sources are moved elsewhere.</li>
   <li> Perhaps all variable definitions relevant for the 
   external sources (including version numbers) should go into
   ExternalSources/definitions.mak? OK:
    <ol>
     <li> What about that "ExternalSources/definitions_.mak" ?
     (This name seems strange to me anyway.) </li>
     <li> The main question is about visibility. There needs to be fixed
     the architecture of our makefiles (w.r.t. inclusion and variable
     definitions). </li>
    </ol>
   </li>
   </ul>
   </li>
   <li> What is the precise relation to
   external_sources_versions.mak ? </li>
   <li> Boost:
    <ul>
     <li> The default value of Boost
     <code> -I/usr/local/boost-1_33_1 </code>
     is stale; the recommended Boost version number is needed. 
     <ul>
     <li>So for every external library Extlib the default value
     of the Make variable Extlib should be the recommended version 
     number?</li>
     </ul>
     </li>
     <li> If the variable includes the "-I", then for other
     variables we use a suffix "_include", which we should also
     do for the boost-variables. </li>
     <li> We need a precise and central definition of those
     boost-variables. </li>
     <li> The boost-variable definitions are inconsistent with
     the rest of the build-system, so they should be reverted (as
     discussed). DONE </li>
    </ul>
   </li>
  </ul>


  \todo System documentation:
   - Document the basic version-control settings (location of server, configuration, how to use it).
   - Document the build-system (general ideas, functionality).


  \todo Role of srcdir : UPDATE as soon as the usage of CMake becomes clearer
  <ul>
   <li>
     What is the role of variable srcdir ? Isn't the definition in makefile_recursive
     superfluous now?
     It is used in makefile_generic (so that we can call makefiles from other places,
     without a change in behaviour; we should also document this), but why the
     definition in makefile_recursive ? Can't makefile_generic define it on its own
     (respectively, shouldn't the definition in makefile_generic  suffice) ?

     Within recursive makefile-invocations we can use the option "--directory=DIR",
     while from the command line, when calling a makefile from another directory,
     the option "-C DIR" can be used. This seems to make srcdir superfluous?
     If so, then is it worth to keep it for convenience?
   </li>
   <li>
     Those settings of srcdir which remain should (if at all) receive some inline comments
     (these settings are quite arcane).
   </li>
  </ul> 


  \todo Documentation (Examples and Concepts) OK : this needs to be discussed and updated
  <ul>
    <li> At OKplatform-level we have a new directory Documentation, with the following sub-directories:
      <ol>
        <li> Examples : contains a mirror of the OKlibrary with (many) example applications (build by the
        build system, so that it's always up-to-date).</li>
        <li> Concepts : contains for each module a latex-file discussing the concepts for this module.</li>
      </ol>
    </li>
  </ul>


  \todo Force make:
  Calling make with the option "-B" (or "--always-make") does not
  work (it leads to an infinite loop) --- why is this so? What can be done
  about it --- it would be nice to be able to force a rebuild, without having to delete
  some directories (this might be dangerous).


  \todo Modes of Usage:
  Two modes of usage of the build system:
   - As we have it now, everything in one directory (and also with the possibility of
     having different versions of the OKplatform). ("One world", extending directly
     the library.)
   - Having one global OKplatform, and then a local directory with also aux, bin, doc
     include and lib, but only regarding the local directory. So that for example students
     in the Linux lab can use a central installation of OKplatform, and in the local
     directory only the files related to their production is stored.


  \todo makefile_recursive
   - This should go, and makefile_generic should be able to do all jobs,
     gathering all relevant files from all underlying subdirectories (but as soon
     as one subdirectory doesn't contain makefile_generic, then it and its
     descendants are ignored). It seems important that for example every subdirectory
     of a module can itself contain tests- and testobjects-subdirectories. The
     directory-tree in system_directories must mirror the primary directory-tree.

   
  \todo Compilation information: 
   - We need a standardised way of how to make information about the compilation
     process available to a program (and also the name of the program, etc.), so that
     for example via --version we get as much information as possible.


  \todo Source code directory structure:
   - We need a rational system for the naming of header files. We should study the
     Boost library here.
   - The namespace should be the directory part of the name for the OKlib (for example OKlib::Algorithms
     is the file OKlib/Algorithms).
   - Directory OKsystem/OKlibrary should then be called OKsystem/OKlib
   - As proposed in [C++ Templates], we should also provide files per module
     only containing declarations (perhaps parallel to each .hpp file). Ending "_decl" ?
   - Module.hpp provides all includes, Module_decl.hpp all declarational includes
   - Perhaps special naming conventions for files with messages or exceptions ?! DONE --- we use sub-directories like "messages" instead of systematic name-extensions. Whether we also use sub-directories "exceptions" needs to be seen.
   - Should we use <> or "" for includes from our library? DONE --- always use the "system header" form with angle brackets!


  \todo Full test
   -  makefile at the OKplatform level --- there the
     different versions of gcc and boost are visible. "make check" at
     this level should run through all appropriate combinations of gcc
     and libraries and test them.


  \todo Nightly build
   - Full check-out of the library (yet Transitional and 
     OKlibrary) and full compilation and testing (i.e., create the package,
     un-archive it, build it with "make" and then run "make check" in it).
     Testing should invoke valgrind (with Test_tool="valgrind --quit").


  \todo Complexity system: 
   - "make measurements" will create an xml-file
     (via the boost serialisation library) with information about all
     operations which have been registered. A little viewing-program
     allows to monitor these measurements (as they evolve over time).


  \todo Measurements:
   - Each "make test" etc. should gather summary statistics of the tests performed,
     like the total number of testobjects, the total time spend and so on.
     In order to do so, a test program can be asked to serialise the statistics
     to a file or to standard output, and a simple evaluation program gathers
     these statistics.
   - Similar to the complexity measurements, for every task performed by the build
     system it should be possible to save the measured run time, so that the development over
     time for example of compile times, link times, test times can be followed, and also
     the influence of compilers can be considered. By default, when running make only
     the total times are output, but in a protocol mode everything is written to a file
     (as for the complexity system; it should be possible for example to use the visualisation
     tools there to look at the developments here).


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


  \todo Compiler versions:
  Do we get slower run-times with g++ 4.1.2 compared to 4.1.1, and thus we
  need to write no-throw-declarations? (Perhaps this needs to be done anyway?)


  \todo New targets (needs update):
   - "create_new_module" and "create_new_submodule",
     which create a new subdirectory with test-program respectively new
     .hpp, _Tests.hpp and _Testobjects.hpp files (with additional inclusion
     in the testprogram).


  \todo Renaming module Buildsystem:
  To be consistent with the other module names, it should be called "BuildSystem" ?
  But perhaps not --- it really is one "block", not composed of of two
  somewhat indepedent parts?! We have "ComplexitySystem" and "TestSystem",
  perhaps "Buildsystem" is different (somewhat more fundamental) ?!


  \todo Version numbers
  <ul>
   <li> It would be good to have not only version numbers for modules,
   but also for single files (to indicate the state of
   development) ?! </li>
  </ul>

*/
