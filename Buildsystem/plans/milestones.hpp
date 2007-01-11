// Oliver Kullmann, 4.3.2006 (Swansea)

/*!
  \file Buildsystem/plans/milestones.hpp
  
  \module_version Buildsystem 0.1.2 (9.12.2006)
  
  \par Version 0.1.3 : TO BE REACHED 14.1.2007
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Make-variables for external libraries
    - Role of srcdir
    - System documentation
    - Documentation (Examples and Concepts)
    - Clean-up
    - makefile_generic_include (DONE)
    - Linking to recursive.mak (DONE)
    - Naming of local definition makefiles (DONE)
    - Recommended versions (DONE)

  \par
   In Buildsystem/plans/generic.hpp the following topics are handled:
    - Documentation of generic.mak
    - Valgrind test variable (DONE)

  \par
   In Buildsystem/plans/generic.hpp the following topics have been updated:
    - General clean-up of make-variables
    - Cleaning
    - Generic makefile redesign
    - OKsystem/Buildsystem structure

  \par
   In Buildsystem/Generic/documentation_building/plans/documentation_building.hpp
   the following topics are handled:
    - Extending and completing OKBuildsystem/ExternalSources/doc.mak
    - Doxygen general
    - Doxygen menu
    - Local documentation : regarding system_directories/doc/index.html (DONE)

  \par
   In Buildsystem/Generic/documentation_building/plans/documentation_index.mak
   the following topics are handled
    - Version numbers
    - Different organisation (DONE)
    - Logo (DONE)
    - OKlibrary link (DONE)

  \par
   In Buildsystem/plans/ExternalSources.hpp the following topics are handled:
    - Building gcc_doc 
    - Corrections mhash
    - Building Boost
    - General
    - Gcc
    - Boost
    - Usage documentation location (DONE)
  
  \par Version 0.1.4 : TO BE REACHED 14.2.2007
   In Buildsystem/plans/generic.hpp the following topics are handled:
    - Incorrect build of new tests I
    - Incorrect build of new tests II
    - General clean-up of make-variables
    - General_options
    - Linking and options
    - Log directory
    - Cleaning
    - Test cleaning
    - Compilation
    - Linking
    - Directory structure
    - Test system
    - Customisation
    - Generic makefile redesign
    - OKsystem/Buildsystem structure
  
  \par
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Overhaul of the general targets
    - Force make
    - Design
    - Modes of Usage
    - makefile_recursive
    - Compilation information
    - Special runs
    - Source code directory structure
    - Version control

  \par Version 0.1.5 :
   In Buildsystem/plans/generic.hpp the following topics are handled:
    - Targets
    - Error messages of gcc should be processed
    - Tools for testing
    - Dependency files
    - Verbosity

  \par
   In Buildsystem/plans/ExternalSources.hpp the following topics are handled:
    - PostgreSQL

  \par Version 0.1.6 :
   In Buildsystem/plans/ExternalSources.hpp the following topics are handled:
    - Other sources
    - Tools

  \par Version 0.1.7 :
   In Buildsystem/plans/README.hpp the following topics are handled:
    - Supported versions of gcc
    - Update
  \par Version 0.1.8 :
   In Buildsystem/plans/makefile.hpp the following topics are handled:
    - Directories with makefile-links

  \par Version 0.2 : THEME is "Basic functionality"
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Testing the build system
    - Full test
    - Nightly build
    
  \par Version 0.3 : THEME is "Complexity measurements"
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Complexity system
    - Measurements

  \par Version 0.4 : THEME is "Packages and Distribution"
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Package construction script
    - Competition extraction tools
    - Distributing the library
  
  \par Version 0.5 : THEME is "Rounding up"
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - New targets
    - Higher-order build tools
  
  \par Version 1.0 :
   


  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.1   : 4.3.2006; initial version number (the build system is under work for around 2 years).
   - 0.1.1 : 3.11.2006; some smaller corrections and modifications regarding naming issues.
   - 0.1.2 : 9.12.2006; systematic naming, placement and decomposition of makefiles; documentation of generic.mak; documentation-building improved.

*/

