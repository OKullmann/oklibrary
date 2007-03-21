// Oliver Kullmann, 4.3.2006 (Swansea)

/*!
  \file Buildsystem/plans/milestones.hpp
  
  \module_version Buildsystem 0.1.4 (8.2.2007)

  \par Version 0.1.5 : THEME is "A specified and testable build system" : TO BE REACHED \deadline_014
   In Buildsystem/plans/generic.hpp the following topics are handled:
    - Specification of buildsystem
 
  \par
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Version control (AS SOON AS POSSIBLE)

  \par
   In Buildsystem/plans/Buildsystem.hpp the following topic is updated:
    - Testing the build system

  \par
   In Buildsystem/Generic/documentation_building/plans/documentation_building.hpp
   the following topics are handled:
    - Doxygen menu
    - Doxygen variables : DONE

  \par
   In Buildsystem/Generic/documentation_building/plans/documentation_index.mak.hpp
   the following topics are handled
    - Page organisation
    - Version numbers : DONE

  \par
   In Buildsystem/plans/generic.hpp the following topics have been updated:
    - General clean-up of make-variables (DONE)
    - Cleaning (DONE)
    - Generic makefile redesign (DONE)
    - OKsystem/Buildsystem structure (DONE)


  \par Version 0.1.6 : THEME is "Build system consolidation" : TO BE REACHED \deadline_015
   In Buildsystem/plans/generic.hpp the following topics are handled:
    - Incorrect build of new tests I
    - Incorrect build of new tests II
    - Dependency files
    - Verbosity
    - Directory structure
    - Test system
    - OKsystem/Buildsystem structure
    - Setting the paths to GCC and Boost link libraries
    - User control of system versions
    - General clean-up of make-variables
    - General_options
    - Linking and options
    - Compilation
    - Linking
    - Customisation
    - .source_libraries

  \par
   In Buildsystem/plans/ExternalSources.hpp the following topics are handled:
    - Extending and completing OKBuildsystem/ExternalSources/doc.mak
    - Building documentation
    - Building gcc_doc 
    - Corrections mhash
    - Building Boost Errors

  \par
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Overhaul of the general targets
    - Force make
    - Design
    - Modes of Usage
    - makefile_recursive
    - Documentation (Examples and Concepts)
    - Compilation information
    - Source code directory structure
    - Make-variables for external libraries
    - Role of srcdir
    - System documentation
    - Special runs

  \par
   In Buildsystem/Generic/documentation_building/plans/documentation_building.hpp
   the following topics are handled:
    - Doxygen general

  \par
   In Buildsystem/Generic/documentation_building/plans/documentation_index.mak.hpp
   the following topics are handled
    - Using css
    - Meta tags
    - Improved logo

  \par Version 0.1.7 : THEME is "Build system extensions"
   In Buildsystem/plans/generic.hpp the following topics are handled:
    - Targets
    - Error messages of gcc should be processed
    - Tools for testing
    - Log directory
    - Cleaning
    - Test cleaning

  \par
   In Buildsystem/plans/ExternalSources.hpp the following topics are handled:
    - Other sources
    - Tools
    - PostgreSQL
    - General
    - Gcc
    - Boost

  \par
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - License
    - Notification list

  \par Version 0.1.8 :
   In Buildsystem/plans/README.hpp the following topics are handled:
    - Supported versions of gcc
    - Update

  \par Version 0.1.9 :
   In Buildsystem/plans/makefile.hpp the following topics are handled:
    - Directories with makefile-links

  \par Version 0.2 : THEME is "Basic functionality"
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
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
   - 0.1.3 : 14.1.2007; further improvements on naming and placement of makefiles, improved building of external libraries, improved html-documentation.
   - 0.1.4 : 8.2.2007; Steps towards a specified and testable buildsystem. Further documentation of generic.mak; corrections to makefile plans files suffixes; corrections to system-wide variable definitions.

*/

