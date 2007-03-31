// Matthew Henderson, 15.11.2006 (Swansea)

/*!
  \file Buildsystem/Generic/documentation_building/plans/documentation_building.hpp
  \brief Plans for documentation-creation

  \todo Doxygen general:
   - Instead of defining the exceptional macros by Doxyfile-tag PREDEFINED, we should use
     the tag EXPAND_AS_DEFINED (since we do not alter the definitions), however this
     does not work?
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
   - What is the role of the doxygen configuration tag BUILTIN_STL_SUPPORT?
     Should we enable this? : Enabled; it should help --- check.
   - See the doxygen-related problems in Messages. (should be solvable by expanding those
     macros) : DONE
   - Can we have nested lists? Numbered lists? These things should go into
     our general documentation --- or we have some example files, which demonstrate
     our use of doxygen :
     For simple lists we do it as here, otherwise we use the html-elements. Documented in the main documentation-document. DONE
   - How to integrate a *general* todo list into Doxygen? With xrefitem. However it seems we no longer need it : DONE
   - Can doxygen extract a list with all modules and their current version numbers? Installed module_version : DONE
   - We should get the newest configuration file, and transfer our information from the old configuration file : DONE

  \todo CSS : Improving the cascading style sheet used by the documentation
   index.

  \todo Doxygen menu review
   -  Is the file list still needed (with version 0.1.4 of Transitional it
      was kind of useful to see for example how many files are documented)?
      Otherwise dropping it could make the system more user-friendly, by
      restricting the attention to the most appropriate choice?
   - Sometimes it would be good to show also other files (especially for Buildsystem).
     This is mainly the issue of how to get doxygen showing makefiles etc. (perhaps
     just as plain text files, without any special formatting).



*/
