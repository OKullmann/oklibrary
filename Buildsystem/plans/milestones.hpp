// Oliver Kullmann, 4.3.2006 (Swansea)

/*!
  \file Buildsystem/plans/milestones.hpp
  
  \par Current version 0.1.1 (3.11.2006)
  
  \par Version 0.1.2 :
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Naming and placement of makefiles

  \par
   In Buildsystem/plans/makefile_generic.hpp the following topics are handled:
    - Complete documentation
    - Valgrind documentation
    - (DONE) Extracting documentation building to separate makefile

  \par
   In Buildsystem/makefile_generic_include/documentation/plans/documentation.hpp
   the following topics are handled.
    - (DONE) Documentation index page creation
    - (DONE) html target
    - ERROR REPORT

  \par Version 0.1.3 :
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Role of srcdir
    - Linking to makefile_recursive
    - System documentation
    - Meta-documentation
    - Documentation (Examples and Concepts)

  \par
   In Buildsystem/plans/makefile_generic.hpp the following topics are handled:
    - Recommended versions

  \par
   In Buildsystem/makefile_generic_include/documentation/plans/documentation.hpp
   the following topics are handled:
    - Doxygen general
    - Doxygen menu

    All the following milestones need to be updated.
  
  \par Version 0.1.4 :
   In Buildsystem/plans/makefile_generic.hpp the following topics are handled:
    - Make-variables for compilation and linking
  
  \par
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Overhaul of the general targets
  
  \par Version 0.2 :
   In Buildsystem/plans/makefile_generic.hpp the following topics are handled:
    - Cleaning
  
  \par
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Testing the build system
  
  \par Version 0.2.1 :
   In Buildsystem/plans/makefile_generic.hpp the following topics are handled:
    - Test cleaning
  
  \par Version 0.2.2 :
   In Buildsystem/plans/makefile_generic.hpp the following topics are handled:
    - Compilation
    - Linking
  
  \par Version 0.3 :
   In Buildsystem/plans/makefile_generic.hpp the following topics are handled:
    - Customisation
  
  \par Version 0.4 :
   In Buildsystem/plans/makefile_generic.hpp the following topics are handled:
    - Test system
    - Targets
    - Tools for testing
    - Error messages of gcc should be processed
  
  \par Version 0.5 :
   In Buildsystem/plans/makefile_generic.hpp the following topics are handled:
    - Placement of makefile_generic
    - Dependency files
    - Verbosity
  
  \par Version 0.6 :
   In Buildsystem/plans/makefile_ExternalSources.hpp the following topics are 
   handled:
    - Gcc
    - Boost
  
  \par Version 0.7 :
   In Buildsystem/plans/makefile_ExternalSources.hpp the following topics are 
   handled:
    - General
    - Tools
    - PostgreSQL
    - Other sources

  \par
   In Buildsystem/plans/README.hpp the following topics are handled:
    - Supported versions of gcc

  \par Version 0.8 :
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Makefiles in general

  \par Version 0.9 :
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Documentation in general

  \par Version 1.0 :
   In Buildsystem/plans/Buildsystem.hpp the following topics are handled:
    - Compilation information
    - Special runs
    - "Full test"
    - Package construction script
    - Nightly build
    - Source code directory structure
    - OKsystem/Buildsystem structure

  \par
   In Buildsystem/makefile_generic_include/documentation/plans/makefile_documentation_index.hpp 
   the following topics are handled:
    - Logo
    - OKlibrary link
    - Version numbers

  \par
   In Buildsystem/makefile_generic_include/documentation/plans/documentation.hpp the following topics are handled:
    - CSS


  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.1   : 4.3.2006; initial version number (the build system is under work for around 2 years).
   - 0.1.1 : 3.11.2006 (some smaller corrections and modifications regarding naming issues).

*/

