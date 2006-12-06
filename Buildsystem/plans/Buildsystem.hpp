// Oliver Kullmann, 12.8.2005 (Swansea)

/*!
  \file Buildsystem/plans/Buildsystem.hpp
  \brief Plans for the buildsystem in general

  \todo Naming and placement of makefiles:
   - For the module-makefiles OKsystem and OKplatform should be defined at only one place,
     and imported at all other places. However the module-makefiles somehow must get
     some information about the directory-structure?! In any way, there are too many
     places where these variables are defined (and redefined).
     With the module-makefiles we have that problem, that cvs does not handle
     links (or?); but perhaps we ignore that, and those links are simply not
     in the repository. Then in Buildsystem we had one special "makefile", to
     which all module-makefiles link.Because this special makefile sits in
     the buildsystem, it can include whatever it wants! Perhaps in every module
     we just have a link to Buildsystem/makefile_generic.mak ?!?!
     This seems reasonable to me (OK). (MH) But, it seems as though this is not so easy.
     When the module makefiles are links to the generic makefile (generic.mak) in 
     Transitional/Buildsystem then if we try to include, within generic.mak, another
     makefile of definitions: for example, a file 
     Transitional/Buildsystem/system_definitions.mak, by using the include statement 
     "include system_definitions.mak" 
     then we get an error from Make. Make looks for the file system_definitions.mak 
     in the directory containing the the link and not in Transitional/Buildsystem as 
     we intend. So it seems that we are still left with the problem generic.mak really 
     needs to know the location of the Transitional/Buildsystem directory. This can be
     done by following the link to makefile generic, rather than having to define the 
     OKBuildsystem Make variable.

   - (DONE) Makefiles should either be called "makefile", or otherwise have the suffix .mak,
     so that for examples xemacs recognises the format.
     Now having the suffix .mak should suffice, and names like "makefile_XXX.mak"
     seem then cumbersome?! So it seems either it's (exactly) "makefile" or "XXX.mak".
   - It seems to follow now that makefile.definitions.mak should just be definitions.mak?

  \todo Role of srcdir
   - What is the role of variable srcdir ? Isn't the definition in makefile_recursive
     superfluous now?
     It is used in makefile_generic (so that we can call makefiles from other places,
     without a change in behaviour; we should also document this), but why the
     definition in makefile_recursive ? Can't makefile_generic define it on its own
     (respectively, shouldn't the definition in makefile_generic  suffice) ?

     Within recursive makefile-invocations we can use the option "--directory=DIR",
     while from the command line, when calling a makefile from another directory,
     the option "-C DIR" can be used. This seems to make srcdir superfluous?
     If so, then is it worth to keep it for convenience?
   - Those settings of srcdir which remain should (if at all) receive some inline comments
     (these settings are quite arcane).

  \todo Linking to makefile_recursive:
   The occurrences of makefile_recursive should be replaced by links.

  \todo System documentation:
   - Document the basic version-control settings (location of server, configuration, how to use it).
   - Document the build-system (general ideas, functionality).

  \todo Meta-documentation:
   - Meta-documentation about usage of the buildsystem.

  \todo Documentation (Examples and Concepts)
   - At OKplatform-level we have a new directory Documentation, with the following sub-directories:
     1. Examples : contains a mirror of the OKlibrary with (many) example applications (build by the
     build system, so that it's always up-to-date).
     2. Concepts : contains for each module a latex-file discussing the concepts for this module..

  \todo Overhaul of the general targets:
   - "all" should not compile the test-programs. Rather we want to have it so 
     that "make new_check" and "make check" does the compilation. For performing
     the checks they need to be compiled; we also need then special targets to 
     just compile the test-programs.
   - we must look at the support for linking with .o files from
     the library itself (including linking with different versions)
   - What is the role of prebuild? Still it is not eliminated --- do we need it?.

  \todo Calling make with the option "-B" (or "--always-make") does not
  work (it leads to an infinite loop) --- why is this so? What can be done
  about it --- it would be nice to be able to force a rebuild, without having to delete
  some directories (this might be dangerous).

  \todo Testing the build system
   - We need some test system for the build system. Optimally, it would run like our normal test
     system; perhaps this is hard to achieve, but at least we need a list of manual checks, well specified,
     which cover all functions of the build system, and which is performed from time to time (manually).
     Then we can partially automate it.

  \todo Design   
   - Larger makefiles should be composed (via inclusion) out of smaller makefiles (if possible;
     otherwise there must be a "copy-and-paste"-comment at each place.
   - We should use (more) make-functions.

  \todo Modes of Usage 
   - Two modes of usage of the build system:
   - As we have it now, everything in one directory (and also with the possibility of
     having different versions of the OKplatform). ("One world", extending directly
     the library.)
   - Having one global OKplatform, and then a local directory with also aux, bin, doc
     include and lib, but only regarding the local directory. So that for example students
     in the Linux lab can use a central installation of OKplatform, and in the local
     directory only the files related to their production is stored.

  \todo Competition extraction tools
   - these tools, for a given program
     build a package, which contains all necessary source code and can be build
     using make (producing then the executable). A problem here are the link-libraries.
     Without them, one could use the ability of g++ to produce compilation units (using
     the option "-E" like "g++ -E Program.cpp -o Program.ii"), putting everything into one big file.
     Knowing the link-libraries, just compiling Program.ii with the right .o-files yields the program.
     So when we know all compilation units, we can preprocess them all and putting them
     into the package; the package just compiles the units separately, and then links them together.

  \todo Measurements:
   - Each "make test" etc. should gather summary statistics of the tests performed,
     like the total number of testobjects, the total time spend and so on.
     In order to do so, a test program can be asked to serialise the statistics
     to a file or to standard output, and a simple evaluation program gathers
     these statistics.
   - Similar to the complexity measurements, for every task performed by the build
     system it should be possible to save the measured run time, so that the develepment over
     time for example of compile times, link times, test times can be followed, and also
     the influence of compilers can be considered. By default, when running make only
     the total times are output, but in a protocol mode everything is written to a file
     (as for the complexity system; it should be possible for example to use the visualisation
     tools there to look at the developments here).

  \todo makefile_recursive
   - This should go, and makefile_generic should be able to do all jobs,
     gathering all relevant files from all underlying subdirectories (but as soon
     as one subdirectory doesn't contain makefile_generic, then it and its
     decendants are ignored). In this way it is then also possible that a directory
     contains a test program, and has also sub-directories with test programs.
     However, best we postphone this, and first make experiences with a more extensive
     directoy structure.
   
  \todo Compilation information: 
   - We need a standardised way of how to make information about the compilation
     process available to a program (and also the name of the program, etc.), so that
     for example via --version we get as much information as possible.

  \todo Special runs
   - It seems we should create a special target "valgrind-check" where the files are
     especially compiled for Valgrind --- this seems to be needed to do automatic
     checks. So then the build system and the test system would be affected.

  \todo Full test
   -  makefile at the OKplatform level --- there the
     different versions of gcc and boost are visible. "make check" at
     this level should run through all appropriate combinations of gcc
     and libraries and test them.

  \todo Package construction script
   - Writing the package-construction script. (Internal versions should
     have date and time in the name.)

  \todo Nightly build
   - Full check-out of the library (yet Transitional and 
     OKlibrary) and full compilation and testing (i.e., create the package,
     un-archive it, build it with "make" and then run "make check" in it).
     Testing should invoke valgrind (with Test_tool="valgrind --quit").

  \todo Source code directory structure:
   - We need a rational system for the naming of header files. We should study the
     Boost library here.
   - The namespace should be the directory part of the name for the OKlib (for example OKlib::Algorithms
     is the file OKlib/Algorithms).
   - Directory OKsystem/OKlibrary should then be called OKsystem/OKlib
   - Should we use <> or "" for includes from our library?
   - As proposed in [C++ Templates], we should also provide files per module
     only containing declarations (perhaps parallel to each .hpp file). Ending "_decl" ?
   - Perhaps special naming conventions for files with messages or exceptions ?!
   - Module.hpp provides all includes, Module_decl.hpp all declarational includes

  \todo Complexity system: 
   - "make measurements" will create an xml-file
     (via the boost serialisation library) with information about all
     operations which have been registered. A little viewing-program
     allows to monitor these measurements (as they evolve over time).

  \todo New targets:
   - "create_new_module" and "create_new_submodule",
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

  \todo Higher-order build tools:
   - We should investigate CMake (http://www.cmake.org/HTML/Index.html), whether it would be
     useful for us. (autoconf and the like seems outdated)

*/
