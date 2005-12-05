// Oliver Kullmann, 4.12.2005 (Swansea)

/*!
  \file Renaming.hpp
  \brief Tools for replacing names.
  \todo First we need a class ReplaceStrings, which can
  replace all occurrences of a set of strings in a file by
  appropriate other strings.
  A typical application is renaming of functions or classes.
  Similar to ProgramRepresentationIncludes, we need a class
  which parses a program (or just a text) and presents the
  text in a way which allows easy iteration over all occurrences
  of the strings to be replaced.
  A string to be replaced might be stored in a structure
  (begin of line, handle to string, end of line, context).
  Similar to Extend_include_directives, then a class is needed which performs
  the replacements (in the representation), allowing for the following policies
  (the following is meant incremental, that is, each further level includes the previous levels):
   - no output
   - output of the filename
   - output of the full line where the replacement happens together with the line number
   - asking for acknowledgement of the change.  
*/

#ifndef RENAMING_7yhHHgf4

#define RENAMING_7yhHHgf4

namespace OKlib {

  namespace Refactoring {

  }

}

#endif
