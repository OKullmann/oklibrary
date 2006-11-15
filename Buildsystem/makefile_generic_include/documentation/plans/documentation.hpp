// Matthew Henderson, 15.11.2005 (Swansea)

/*!
  \file Buildsystem/makefile_generic_include/documentation/plans/documentation.hpp
  \brief Plans for documentation in general 

  \todo (DONE) Documentation index page creation:
   - In $(system_directories)/doc there is a index to all the OKplatform
    documentation, including external resources. This index also contains
    links to the web pages of Boost, Doxygen, Gcc etc

  \todo (DONE) html target:
   - "html" should be possible from any level (creating always the
     complete documentation) (this should be automatically achieved
     once makefile_recursive is eliminated) 

  \todo Parameters:
   - (DONE) Full overview on the parameters for makefile_generic (including the environment
     variables).

  \todo Doxyfile:
   - What is the role of the doxygen configuration tag BUILTIN_STL_SUPPORT?
     Should we enable this?

  \todo Doxygen menu
   -  Perhaps we should drop the file-list altogether?
     (This could make the system more user-friendly, by restricting the attention to
     the most appropriate choice?) On the other hand, always having to click on the
     directory items might not be always best, so perhaps we should keep the file-list.
   - Sometimes it would be good to show also other files (especially for Buildsystem).
     This is mainly the issue of how to get doxygen showing makefiles etc. (perhaps
     just as plain text files, without any special formatting).

  \todo System documentation:
   - Document the basic version control settings (location of server, configuration, how to use it).
   - Document the build system (general ideas, functionality).

  \todo Updating:
   - Checking on the existing documentation, and updating if necessary.               
   - The documentation needs to be rewritten to be aimed more at the user.
     The documentation of the main targets should remain in the makefile
     preamble, but the documentation of local variables should be moved
     to inline comments. One needs to distinguish between targets and variables.
     The disctinction below about make-variables should be reflected
     (somehow) in their documentation.
   - What is required from those makefile.definitions.mak ? Update the list in
     makefile_generic.mak, and reflect on it.
   - Adding inline comments for an overview on functionality.

  \todo Meta-documentation:
   - Meta-documentation about usage of the buildsystem.

  \todo CSS : Improving the cascading style sheet used by the documentation
   index.

  \todo ERROR REPORTING : regarding system_directories/doc/index.html

   - Local documentation:

     An error occurred while loading file:///h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Boost/boost_1_33_1/index.htm:
The file or folder /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Boost/boost_1_33_1/index.htm does not exist.

     Doxygen appears with the wrong version number

     An error occurred while loading file:///h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Mhash/0.9.7.1+4.1.1/man/man3/mhash.3:
The file or folder /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Mhash/0.9.7.1 4.1.1/man/man3/mhash.3 does not exist.

     An error occurred while loading file:///h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Postgresql/doc/postgresql/html/index.html:
The file or folder /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Postgresql/doc/postgresql/html/index.html does not exist.

*/
