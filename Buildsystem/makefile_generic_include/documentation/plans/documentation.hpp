// Matthew Henderson, 15.11.2005 (Swansea)

/*!
  \file Buildsystem/makefile_generic_include/documentation/plans/documentation.hpp
  \brief Plans for documentation-creation

  \todo ERROR REPORT : regarding system_directories/doc/index.html

   - Local documentation:

     An error occurred while loading file:///h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Boost/boost_1_33_1/index.htm:
The file or folder /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Boost/boost_1_33_1/index.htm does not exist.

     Doxygen appears with the wrong version number

     An error occurred while loading file:///h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Mhash/0.9.7.1+4.1.1/man/man3/mhash.3:
The file or folder /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Mhash/0.9.7.1 4.1.1/man/man3/mhash.3 does not exist.

     An error occurred while loading file:///h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Postgresql/doc/postgresql/html/index.html:
The file or folder /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Postgresql/doc/postgresql/html/index.html does not exist.

  \todo Doxygen general:
   - Can doxygen tell which other files include a file (not in graph form, but in text form)?
   - For functions there is a "callergraph" --- shall we use it?
   - How to integrate a *general* todo list into Doxygen?
   - How to avoid that a leading "include" in a Doxygen-comment is interpreted as
     a doxygen-command? And how to avoid that apparently certain "keywords" are captured by
     doxygen?
   - How to obtain general statistics: About the number of classes, lines of code, etc. (best
     with some statistics on the change over time)
     and then also on the version numbers of the modules (again with changes over time).
     If doxygen does not support it yet, then we should submit some feature request, and postphone
     this item.
   - We should get the newest configuration file, and transfer our information from the old
     configuration file.
   - It appears that all .cpp-files are considered as linked together?
   - Can makefiles be incorporated?!
   - Can we have nested lists? Numbered lists? These things perhaps should go into
     our general documentation --- or we have some example files, which demonstrate
     our use of doxygen.

  \todo Doxygen menu
   -  Perhaps we should drop the file-list altogether?
     (This could make the system more user-friendly, by restricting the attention to
     the most appropriate choice?) On the other hand, always having to click on the
     directory items might not be always best, so perhaps we should keep the file-list.
   - Sometimes it would be good to show also other files (especially for Buildsystem).
     This is mainly the issue of how to get doxygen showing makefiles etc. (perhaps
     just as plain text files, without any special formatting).

  \todo Doxyfile:
   - What is the role of the doxygen configuration tag BUILTIN_STL_SUPPORT?
     Should we enable this?

  \todo CSS : Improving the cascading style sheet used by the documentation
   index.

*/
