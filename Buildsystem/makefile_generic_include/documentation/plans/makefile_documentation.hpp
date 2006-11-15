// Matthew Henderson, 14.11.2006 (Swansea)

/*!
  \file Buildsystem/makefile_generic_include/documentation/plans/makefile_documentation.hpp
  \brief Plans for the documentation makefile

  \todo Update todos.

  \todo Documentation :
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
*/
