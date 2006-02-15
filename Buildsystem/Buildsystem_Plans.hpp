// Oliver Kullmann, 12.8.2005 (Swansea)

/*!
  \file Buildsystem_Plans.hpp

  \todo makefiles in general should always have the suffix .mak,
  so that for examples xemacs regocnises the format.

  \todo Overhaul of the general targets:
   - "all" should not compile the test-programs
   - we must look at the support for linking with .o files from
     the library itself (including linking with different versions)
   - what is the role of prebuild?
   - it should be possible to build just one application
   - there should be special versions of "clean" which delete only applications
     or only the test programs
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

   \todo Error messages of gcc should be processed.
   \todo Type names output by gcc (in test-system messages) should be readable
   (decoded, and possibly using abbreviations).

   \todo Each "make test" etc. should gather summary statistics of the tests performed,
   like the total number of testobjects, the total time spend and so on.
   In order to do so, a test program can be asked to serialise the statistics
   to a file or to standard output, and a simple evaluation program gathers
   these statistics.
   \todo Similar to the complexity measurements, for every task performed by the build
   system it should be possible to save the measured run time, so that the develepment over
   time for example of compile times, link times, test times can be followed, and also
   the influence of compilers can be considered. By default, when running make only
   the total times are output, but in a protocol mode everything is written to a file
   (as for the complexity system; it should be possible for example to use the visualisation
   tools there to look at the developments here).

   \todo The test system in the context of the build system knows the following modes (with
   the following associated 3 streams):
    - konsole-output : cerr cout NULL (the default)
    - extend-konsole-output : cerr cout cout
    - file-output : "ofstream-w=error-file|ofstream-w=log-file" "ofstream-w=log-file" "ofstream-w=log-file"
    - mixed-output : "ofstream-w=error-file|ofstream-w=log-file|cerr" "ofstream-w=log-file|cout" "ofstream-w=log-file"
    - extended-mixed-output : "ofstream-w=error-file|ofstream-w=log-file|cerr" "ofstream-w=log-file|cout" "ofstream-w=log-file|cout"
  Here error-file and log-file are placed in aux; actually, we should have another directory
  called "log", where these files go, and also the doxygen-error messages.

   \todo Errors in the build system:
    - If when creating the .d-files an error occurs (for example
   due to an inaccessible header file), then for some reason subsequent "make check"
   erroneously succeeds.
    - Using the gcc option "-MP" ?

   \todo makefile_recursive
    - This should go, and makefile_generic should be able to do all jobs,
      gathering all relevant files from all underlying subdirectories (but as soon
      as one subdirectory doesn't contain makefile_generic, then it and its
      decendants are ignored). In this way it is then also possible that a directory
      contains a test program, and has also sub-directories with test programs.
   
   \todo makefile_generic
    - Except of in Buildsystem, all other makefile_generic-versions should be links.
      See makefile_recursive above.
      A problem here is, that it seems that links are not handled by CVS ?
    - We should likely call ("execute") these (links to) makefile_generic in this way, and
      not just "makefile" (even if it's a bit inconvenient).
    - Currently makefile_generic takes special actions to ensure that it works the same
      from wherever we call it. This seems to create some trouble, and doesn't seem
      to be compatible with using links? So it should be abandoned? But then effectively
      it doesn't make sense to call makefile_generic from another place, and one always
      has to use cd first. One issue here is that the testsystem cannot any longer use
      any local directories (since it won't find them); perhaps the solution is to provide
      some central place for testdata.
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
    - With the new system with its nested directory structure, names need to be unique
      only relativ to the whole library-path; however for linking it is common to have all
      files just in one directory, and so here should be no name clashes: For the rare cases
      where one has to link against the OKlibrary we must ensure this by hand, while
      for the cases of test-object-files, which are under the control of the build system,
      perhaps the best is that the build system mimics the source directory structure in
      lib/Tests.
    - We have the following problem:
      If one is using different paths due to symbolic links, then the
      dependency files contain unusable information (and must be
      deleted with "make cleandep").
      This problem seems hard to solve (one had to find out that
      different paths lead to the same file).
      So it must be documented well.
    - We should support using a tool like TextFilt or STLFilt.

  \todo Doxygen:
   - Can doxygen show which other files include a file?
   - How to integrate a *general* todo list into Doxygen?
   - How to avoid that a leading "include" in a Doxygen-comment is interpreted as
     a doxygen-command?
   - How to obtain general statistics?
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

  \todo ExternalSources:
   - When building some gcc-version, only the necessary directories
     should be created. This should be handled as we have it now with boost (using
     timestamp-files).
   - When building boost (in some variation) using "gcc-version=...", then as a subtarget
     we have the build of the gcc-version (so that, if necessary, gcc is build).
   - Installation of bjam should be improved: Having exactly one bjam-installation for each boost-version,
     and no need to recreate it if it's already there. Or, perhaps better: We just leave it in
     the distribution directory?
   - Building boost should include copying the documentation to doc (in the subdirectory
   boost-1_33_1 for example).
   - It would ge good, if after doing a local installation, easily the installation could also be
     make global.
   - If variable "gcc-version" is set, then it should have one of the allowed values (while otherwise we get
     an error).
    - Optionally there should be also local versions of valgrind and doxygen.
   - "make initialise-database" should work with the recommended version.
   - Build a local version of gmp.
   - Update PostgreSQL to version 8.1 (and test it).
   - Build the R system (locally and globally).
   - SOCI (soci.sourceforge.net)
   - gcc-xml (www.gccxml.org)

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

  \todo Improving the directory structure (which yet is "flat"):
  - Inclusion from for example Transitional should happen as, e.g.,
  #include "Transitional/SumProduct/Marginalisation.hpp"
  or
  #include "OKlib/Algorithms/Sat.hpp"
  (Thus compilation of local copies of the library then only work with
  the *standard place* (i.e., OKplatform/include).) This is achieved by
  eliminating the include-directory at OKplatform-level (and all associated
  environment- and make-variables), and including then from level
  OKsystem.
  - We need a rational system for the naming of header files. We should study the
    Boost library here.
  - The namespace should be the directory part of the name (in the above examples
    Transitional::SumProduct and OKlib::Algorithms).
  - Directory OKsystem/OKlibrary should then be called OKsystem/OKlib
  - Directory OKsystem/Transitional should be renamed OKsystem/TOKlib, and thus the namespace
    "Transitional" is renamed to "TOKlib".
  - As proposed in [C++ Templates], we should also provide files per module
  only containing declarations (perhaps parallel to each .hpp file). Ending "_decl" ?
  - Perhaps special naming conventions for files with messages or exceptions ?!
  - Module.hpp provides all includes, Module_decl.hpp all declarational includes

  \todo Perhaps we should have a sub-directory "SystemDirectories" of OKplatform,
  where then aux, bin, doc, include, lib, and log, ... is located.
  \todo Once we get the library directly from "the sources", what is then the
  role of OKplatform/include ? Seems we can abandon it.

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
