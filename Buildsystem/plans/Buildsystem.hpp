// Oliver Kullmann, 12.8.2005 (Swansea)

/*!
  \file Buildsystem/plans/Buildsystem.hpp
  \brief Plans for the buildsystem in general

  \todo Makefiles in general:
   - OKSystem is defined at one place, and imported at all other places. Additionally
     OKSystem_include is defined (with "-I").
     In the local definition-makefiles then we only use (not define) OKSystem_include.
   - Makefiles should always have the suffix .mak,
     so that for examples xemacs recognises the format.
   - Larger makefiles should be composed (via inclusion) out of smaller makefiles (if possible;
     otherwise there must be a "copy-and-paste"-comment at each place.
   - We should use (more) make-functions.

  \todo Overhaul of the general targets:
   - "all" should not compile the test-programs
   - we must look at the support for linking with .o files from
     the library itself (including linking with different versions)
   - what is the role of prebuild? (Yet it builds the directory
     structure --- perhaps this should be performed by each target if
     needed (so that prebuild wouldn't be needed anymore here)?
     What else is done by prebuild?)
   - "html" should be possible from any level (creating always the
   complete documentation) (this should be automatically achieved
   once makefile_recursive is eliminated).

   \todo Two modes of usage of the build system:
    - As we have it now, everything in one directory (and also with the possibility of
      having different versions of the OKplatform). ("One world", extending directly
      the library.)
    - Having one global OKplatform, and then a local directory with also aux, bin, doc
      include and lib, but only regarding the local directory. So that for example students
      in the Linux lab can use a central installation of OKplatform, and in the local
      directory only the files related to their production is stored.

   \todo We need some "competition extraction tools", which for a given program
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
    -Similar to the complexity measurements, for every task performed by the build
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
   
  \todo Doxygen:
   - Can doxygen tell which other files include a file (not in graph form, but in text form)?
   - For functions there is a \callergraph --- shall we use it?
   - Since sub-directories of Transitional are modules for us, it would be
     good to emphasise the directory structure; in the file list in the
     doxygen documentation this is kind of ugly. And can we have
     a general "directory list" ?!
   - How to integrate a *general* todo list into Doxygen?
   - How to avoid that a leading "include" in a Doxygen-comment is interpreted as
     a doxygen-command? And how to avoid that apparently certain "keywords" are captured by
     doxygen?
   - How to obtain general statistics?
   - We should get the newest configuration file, and transfer our information from the old
     configuration file.
   - It appears that all .cpp-files are considered as linked together?
   - Can makefiles be incorporated?!

   \todo Documentation in general
    - At OKplatform-level we have a new directory Documentation, with the following sub-directories:
     1. Examples : contains a mirror of the OKlibrary with (many) example applications (build by the
     build system, so that it's always up-to-date).
     2. Concepts : contains for each module a latex-file discussing the concepts for this module.
    - Documentation contains also the following (symbolic file) links:
     1. to the doxygen-main-site;
     2. to the Boost main site
     3. to all other documentations relevant here.
    - There needs to be a main page for the OKplatform.

  \todo We need a standardised way of how to make information about the compilation
  process available to a program (and also the name of the program, etc.), so that
  for example via --version we get as much information as possible.

  \todo Special runs
   - It seems we should create a special target "valgrind-check" where the files are
     especially compiled for Valgrind --- this seems to be needed to do automatic
     checks. So then the build system and the test system would be affected.

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

  \todo Directory structure:
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

  \todo Higher-order build tools
   - We should investigate CMake (http://www.cmake.org/HTML/Index.html), whether it would be
     useful for us. (autoconf and the like seems outdated)

*/
