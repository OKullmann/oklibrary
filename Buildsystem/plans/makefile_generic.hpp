// Oliver Kullmann, 28.2.2006 (Swansea)

/*!
  \file Buildsystem/plans/makefile_generic.hpp
  \brief Plans for the generic makefile

  \todo ERROR REPORTING regarding system_directories/doc/index.html

   - Local documentation:

     An error occurred while loading file:///h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Boost/boost_1_33_1/index.htm:
The file or folder /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Boost/boost_1_33_1/index.htm does not exist.

     Doxygen appears with the wrong version number

     An error occurred while loading file:///h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Mhash/0.9.7.1+4.1.1/man/man3/mhash.3:
The file or folder /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Mhash/0.9.7.1 4.1.1/man/man3/mhash.3 does not exist.

     An error occurred while loading file:///h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Postgresql/doc/postgresql/html/index.html:
The file or folder /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Postgresql/doc/postgresql/html/index.html does not exist.
  

  \todo Documentation (in makefile_generic.mak):
    - Full overview on the parameters for makefile_generic (including the environment
      variables). (DONE)
    - Checking on the existing documentation, and updating if necessary. (DONE)

    DISCUSSION : (MH) The documentation needs to be rewritten to be aimed more at the user.
                 The documentation of the main targets should remain in the makefile
		 preamble, but the documentation of local variables should be moved
		 to inline comments.

		 There should be some meta-documentation about the usage of this
		 makefile.

                 (OK) One needs to distinguish between targets and variables.
                 The disctinction below about make-variables should be reflected
                 (somehow) in their documentation.
                 

    - What is required from those makefile.definitions.mak ? Update the list in
      makefile_generic.mak, and reflect on it. (DONE)
    - Adding inline comments for an overview on functionality.

    DISCUSSION : (MH) It seems that a redesign of makefile_generic.mak to 
                 outsource the different functions of makefile_generic.mak
                 to seperate files may at least help to make the makefile 
                 design and usage easier to document clearly.

                 I propose the following redesign:

                 $(OKBuildsystem)/generic_makefile.mak should only be directly
                 responsible for defining the current source directory (srcdir), 
                 getting the module-specific definitions from 
                 makefile_definitions.mak, defining the make targets which are 
                 directly aimed at the user : "all", "clean", "check" and so on 
                 and providing documentation for those main targets. All the 
                 other targets and variable definitions are then defined by 
                 calling functions (with srcdir as the parameter) from makefiles
                 in the directory OKBuildsystem/makefile_generic_functions. 
                 Those makefiles contain the relevant documentation.

                 The directory OKBuildsystem/makefile_generic_functions would 
                 have a structure something like :

                 - makefile_generic_functions
                   - apps
                     - cleaning.mak
                     - compilation.mak
                     - linking.mak
                   - options
                     - compilation.mak
                     - errors.mak
                     - language.mak
                     - linking.mak
                     - log.mak
                     - messages.mak
                     - preprocessor.mak
                   - tests
                     - cleaning.mak
                     - compilation.mak
                     - linking.mak
                   - tests_old
                     - cleaning.mak
                     - compilation.mak
                     - linking.mak

                 In addition, makefile_generic_functions would contain some 
                 further makefiles for definitions for the directory structure 
                 of system_directories, internal make targets, running tests, 
                 environment and system Make variables.

  \todo (DONE) Extracting documentation building to seperate makefile:
  Those parts of makefile_generic.mak which are responsible for building the
  doxygen documentation and the main documentation index page are extracted
  to seperate makefiles:
    $(OKBuildsystem)/makefile_generic/makefile_documentation.mak
    $(OKBuildsystem)/makefile_generic/makefile_documentation_index.mak
 
  \todo Make-variables for compilation and linking:
    - instead of specifying a specific alternative gcc or boost version, it
      should be possible to use the recommended version
    - also it should be possible to use recommended versions of all external
      resources, such as Mhash.

    DISCUSSION : (MH) Here there seem to be several possibilities. The recommended
                 versions are defined in makefile_ExternalSources.mak (GCC) and
                 makefile_boost.mak. So one possibility would be to get them from
                 there by searching for the right version number string. Another
		 possibility would be to define the different versions including
		 recommended versions in seperate makefiles (e.g. gcc_versions.mak)
		 and then this can be included by all the makefiles which need it.
		 The other possibility would just be to copy and paste the recommended
		 versions in makefile_generic.mak with a copy and paste comment.

		 It seems like the right thing to do here is to create a make file
		 external_sources_versions.mak which contains variable definitions
		 for all the supported versions of Gcc, Boost, Mhash, Postgresql etc..
		 This file is then included by those other makefiles which also need
		 those version numbers.

                 (MH) This task is partially complete. The file 
                 external_sources_versions.mak contains the recommended versions of
                 all external resources. However, the recommended version
                 numbers are got by the buildsystem through a little hack. Namely,
		 there are seperate variables for both the version number and the
                 version name. This should be changed so that the version number
		 is only defined once.

    - also it should be possible to use the system-wide versions of all
      external resources.

    DISCUSSION : (MH) Here a possible solution is to have a make variable 
                 system_versions, say, which, if defined to have any value, 
                 prompts the build system to use the system-wide versions of
                 all external resources. 

    - general clean-up of make-variables

   DISCUSSION : (MH) There should be a scheme for the naming of variables which
                distinguishes those variables : 
		-# which are internal (purely for the use of the build system), 
		-# which are mostly for internal use of the build system, but
                can be redefined by the user with suitable precaution. 
		-# which are intended for use by the user, typically to specify
                options.

                (OK : why are you using "-#" ?)
                (MH : This is how to create numbered lists in Doxygen. You prefer to
                just write the numbers?)

		One possible scheme is:
		-# _variable (lowercase with preceeding underscore)
		-# variable (lowercase)
		-# Variable (uppercase first character)

                (OK) One has to check whether leading underscore is allowed. Perhaps a trailing underscore
                is better.

                (MH) An alternative scheme then:
                -# variable_
                -# variable
                -# Variable

                The uppercase first variables have actually often default values (namely
                the all-uppercase environment variables). So this explanation is not really valid.

                Perhaps the distinction aimed at is, that some variables can be tinkered with, but
                one somehow need to know about them, while other variables have no function than
                enabling the user to insert some options (for example for compilation).

                Perhaps first a complete list of make-variables and the status is needed here, to
                see what we have.
                
                (MH) Perhaps this should be postponed for a little while,
                because the changes I propose under documentation and cleaning
                involved changing many variable names and definitions.

    - the current function of "General_options" is taken over by the new variable "Debug_options"

    - Which compiler options are effective when linking?

   DISCUSSION : (MH) 

    Here is a list of compiler options which are effective when linking, taken from
    "Using the Gnu Compiler Collection".

    - -nostartfiles
       Do not use standard system startup files.      
    - -nodefaultlibs
       Do not use standard system libraries.
    - -nostdlib
       Do not use either standard system startup files or standard system libraries.
    - -pie
       Produce a "position independent executable".
    - -s
       Remove symbol table and relocation information from the executable.
    - -static
       Prevent linking with dynamic libraries.
    - -shared-libgcc
    - -static-libgcc
       On systems which provide libgcc as a shared library these options force either
       the shared or static version to be used.    
    - -symbolic
       Bind references to global symbols when building a shared object.
    - -u symbol
       Pretend that symbol is undefined, to force linking of library modules to define
       it.

      (OK) The point here is to know exactly what options should/must be used when compiling
      respectively linking (and "options" refers first of all to *our* options).

      (MH) "Our" options are 
      -  -ansi (cc1plus)
      -  -pedantic (cc1plus)
      -  -Wall (cc1plus)
      -  -g (cc1plus) (mentioned as "ignored" is the ld man page)
      -  -03 (cc1plus)
      -  -DNDEBUG (cc1plus)

      Here, the names in brackets are those executables indicate that, in the
      case of ld (linker) and cpp (preprocessor), the executables whose man 
      page documents the option and, in the case of cc1plus whether the help
      file documents the option.

      It seems, at first glance, that all of "our" options are only effective
      for the compiler.

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

    DISCUSSION : (MH) it seems that a minor redesign of the directory 
    structure of system_directories is desirable. This is partly because there
    is a little inconsistency in the current design (for example, some things 
    are put in module dependent subdirectories, some are not) and partly 
    because it would seem logical that cleaning tests for a module corresponds 
    to removing a module-dependent subdirectory of tests from a bin directory, 
    or cleaning dependencies for the tests in some module means to remove a 
    directory of dependencies from a module-dependent subdirectory of a tests 
    subdirectory of a bin directory.

    The current design is:

    - system_directories
      - aux
        - dependencies
        - latex
        - tests
          - module_1
          - module_2
      - bin
        - tests
      - doc
        - dvi
        - html
      - lib
        - tests
          - module_1
          - module_2
  
    I propose the following redesign:

    - system_directories
      - aux
        - apps
          - module_1
            - dependencies
          - module_2
            - dependencies
        - latex
        - tests
          - module_1
            - dependencies
          - module_2
            - dependencies      
      - bin
        - apps
          - module_1
          - module_2
        - tests
          - module_1
          - module_2
      - doc
        - dvi
        - html
      - lib
        - apps
          - module_1
          - module_2
        - tests
          - module_1
          - module_2

        OK: do you really mean bin etc as subdirectories of aux?
        I don't understand what you mean here.
        Is your main point to always use module-directories?

        MH: No, this was a mistake with indentation. Yes, I think
        it makes sense to have module-directories in every case.

    So now to clean the dependencies for the tests from module_1 means to remove
    the directory system_directories/aux/tests/module_1/dependencies. In fact we
    provide both targets clean_dependency_files (to remove just the files) and 
    cleandep (to remove the directory). The naming chosen to encourage the user
    to use the version which removes the whole directory.

  \todo Test cleaning:
    We need specialised cleaning regarding the test system :
    - Cleaning of special or all versions of the test-timestamps.
    - Cleaning of test-objectfiles and test-programs.
    - Cleaning of test-depencies.

  \todo Compilation:
    - The names of the created .o-files and executables should reflect "all" compiler options.
    - There are generic links to the unoptimised and the optimised version (the latest).
      The test system uses these.
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
