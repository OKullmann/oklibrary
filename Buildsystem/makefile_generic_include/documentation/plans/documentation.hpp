// Matthew Henderson, 15.11.2005 (Swansea)

/*!
  \file Buildsystem/makefile_generic_include/documentation/plans/documentation.hpp
  \brief Plans for documentation-creation

  \bug Building boost_doc does not work, since the archive uses "boost_",
  while the boost-versions use "boost-".

  \bug Building gcc_doc does not work, since apparently the man-pages need to be build.

  \todo Local documentation : regarding system_directories/doc/index.html
  <ul>
  <li> A new makefile OKBuildsystem/ExternalSources/doc.mak is created which
       is responsible for extracting and building all the external sources
       documentation OKplatform/ExternalSources/doc. Currently this makefile
       handles Doxygen and Boost and needs to be extended to handle those
       other external sources (Gcc, Mhash, Postgresql, UBCSAT)
  </li>
  <li> See the above bugs. </li>
  </ul>

  \todo Extending and completing OKBuildsystem/ExternalSources/doc.mak
   - Documentation for Gcc, Mhash, Postgresql

  \todo Building documentation:
  We should extract only the documentation from the archives.

  \todo Doxygen general:
   - Can doxygen tell which other files include a file (not in graph form, but in text form)?
   - For functions there is a "callergraph" --- shall we use it?
   - How to avoid that a leading "include" in a Doxygen-comment is interpreted as
     a doxygen-command? And how to avoid that apparently certain "keywords" are captured by
     doxygen?
   - How to obtain general statistics: About the number of classes, lines of code, etc. (best
     with some statistics on the change over time)
     and then also on the version numbers of the modules (again with changes over time).
     If doxygen does not support it yet, then we should submit some feature request, and postphone
     this item.
   - It appears that all .cpp-files are considered as linked together?
   - Can makefiles be incorporated?!
   - See the doxygen-related problems in Messages.
   - What is the role of the doxygen configuration tag BUILTIN_STL_SUPPORT?
     Should we enable this? : Enabled; it should help --- check.
   - Can we have nested lists? Numbered lists? These things should go into
     our general documentation --- or we have some example files, which demonstrate
     our use of doxygen :
     For simple lists we do it as here, otherwise we use the html-elements. Documented in the main documentation-document. DONE
   - How to integrate a *general* todo list into Doxygen? With xrefitem. However it seems we no longer need it : DONE
   - Can doxygen extract a list with all modules and their current version numbers? Installed module_version : DONE
   - We should get the newest configuration file, and transfer our information from the old configuration file : DONE

  \todo Doxygen menu
   -  Perhaps we should drop the file-list altogether?
     (This could make the system more user-friendly, by restricting the attention to
     the most appropriate choice?) On the other hand, always having to click on the
     directory items might not be always best, so perhaps we should keep the file-list.
   - Sometimes it would be good to show also other files (especially for Buildsystem).
     This is mainly the issue of how to get doxygen showing makefiles etc. (perhaps
     just as plain text files, without any special formatting).

  \todo CSS : Improving the cascading style sheet used by the documentation
   index.

*/
