// Oliver Kullmann, 12.8.2005 (Swansea)

/*!
  \file Buildsystem/plans/Buildsystem.hpp
  \brief Plans for the buildsystem in general

  \todo system_definitions.mak:
  <ul>
   <li> The role of system_definitions.mak must be clarified.
   Do those library variables (Boost, Ubcsat) belong to it??
   <ul>
   <li> system_definitions.mak should
   contain definitions of Make variables which are used by
   several makefiles. However, the prefix "system_" also implies that 
   these
   variables belong to the OKlibrary and not to the external
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

  \todo external_sources_versions.mak:
  <ul>
   <li> The role of external_sources_versions.mak must
   be clarified. </li>
   <li> Names like
   <code> doxygen_supported_not_recommended_version_numbers </code>
   are misleading (these versions are not "not recommended"), and
   they are too long. </li>
  </ul>

  \todo Make-variables for external libraries
  <ul>
   <li> How to define variables like Boost and Ubcsat in general?
     We need a nice little method (plus documentation), so that we can add easily as
     many external libraries as we want. </li>
   <li> And then we have make-variables "gcc_version_number" (for generic.mak) and
     "gcc-version" (for boost.mak) which likely should have the same names. </li>
   <li> In system_definitions.mak we have a definition of Boost which seems wrong
     to me (OK) (or at least misleading). DONE (resp. moved to other todos) </li>
  </ul>

  \todo Testing the build system
   - We need some test system for the build system. Optimally, it would run like our normal test
     system; perhaps this is hard to achieve, but at least we need a list of manual checks, well specified,
     which cover all functions of the build system, and which is performed from time to time (manually).
     Then we can partially automate it.

  \todo License:
  It seems LGPL is suitable; we have to check this (is it compabible with all
  other licenses (for external sources) involved? I guess so). Once decided,
  we have to add appropriate licence texts to every file.

  \todo Notification list:
  We had a list with names and addresses of people interested in the library,
  which shall be informed personally about the release --- where is this
  gone ??? There were also plans in it, which have been moved to the new
  plans-file-system, but where is the other information???
  Perhaps a good place for such system-wide plans is in Transitional/plans.
  People to add to that list:
   - Marina de Vos and Martin XXX (Bath)
   - Sam Buss
   - Tobias Nipkow (TU Muenchen)

  \todo Role of srcdir
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

  \todo System documentation:
   - Document the basic version-control settings (location of server, configuration, how to use it).
   - Document the build-system (general ideas, functionality).

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

  \todo Overhaul of the general targets:
   - "all" should not compile the test-programs. Rather we want to have it so 
     that "make new_check" and "make check" does the compilation. For performing
     the checks they need to be compiled; we also need then special targets to 
     just compile the test-programs. OK (31.3.2007): this seems to be achieved?
   - we must look at the support for linking with .o files from
     the library itself (including linking with different versions)
   - What is the role of prebuild? Still it is not eliminated --- do we need it?.

  \todo Force make:
  Calling make with the option "-B" (or "--always-make") does not
  work (it leads to an infinite loop) --- why is this so? What can be done
  about it --- it would be nice to be able to force a rebuild, without having to delete
  some directories (this might be dangerous).

  \todo Design   
   - Larger makefiles should be composed (via inclusion) out of smaller makefiles (if possible;
     otherwise there must be a "copy-and-paste"-comment at each place.
   - We should use (more) make-functions.

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

  \todo Special runs
   - It seems we should create a special target "valgrind-check" where the files are
     especially compiled for Valgrind --- this seems to be needed to do automatic
     checks. So then the build system and the test system would be affected.

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

  \todo Package construction script
   - Writing the package-construction script. (Internal versions should
     have date and time in the name.)

  \todo Competition extraction tools
   - these tools, for a given program
     build a package, which contains all necessary source code and can be build
     using make (producing then the executable). A problem here are the link-libraries.
     Without them, one could use the ability of g++ to produce compilation units (using
     the option "-E" like "g++ -E Program.cpp -o Program.ii"), putting everything into one big file.
     Knowing the link-libraries, just compiling Program.ii with the right .o-files yields the program.
     So when we know all compilation units, we can preprocess them all and putting them
     into the package; the package just compiles the units separately, and then links them together.

  \todo Distributing the library (needs update):
   - We must study how to distribute a copy of the whole CVS-system (so that later a re-merge is
     possible; also updates must be studied).
   - Two download possibilities: Either the whole library (with all included libraries like
     Boost, doxygen, PostgreSQL etc.), or only the minimum.
     This gives two user types: "full user" and "minimal user".
     We must make sure, that also the minimal user can use the test and the complexity system
     (compiling it himself, or using the build system). And we must study, how a full user can keep his
     file structure separate from the library by using links.

  \todo New targets (needs update):
   - "create_new_module" and "create_new_submodule",
     which create a new subdirectory with test-program respectively new
     .hpp, _Tests.hpp and _Testobjects.hpp files (with additional inclusion
     in the testprogram).

  \todo Higher-order build tools:
   - We should investigate CMake (http://www.cmake.org/HTML/Index.html), whether it would be
     useful for us. (autoconf and the like seems outdated ?)

  \todo Renaming module Buildsystem:
  To be consistent with the other module names, it should be called "BuildSystem" ?

*/
