// Oliver Kullmann, 4.12.2005 (Swansea)
/* Copyright 2005 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Refactoring/plans/Renaming.hpp
  \brief Tools for replacing names.


  \todo ReplaceStrings
  <ul>
   <li> First we need a class ReplaceStrings, which can
   replace all occurrences of a set of strings in a file by
   appropriate other strings. </li>
   <li> A typical application is renaming of functions or classes. </li>
   <li> Similar to ProgramRepresentationIncludes, we need a class
   which parses a program (or just a text) and presents the
   text in a way which allows easy iteration over all occurrences
   of the strings to be replaced. </li>
   <li> A string to be replaced might be stored in a structure
   (begin of line, handle to string, end of line, context). </li>
   <li> Similar to Extend_include_directives, then a class is needed which
   performs the replacements (in the representation), allowing for the
   following policies (the following is meant incremental, that is, each
   further level includes the previous levels):
    <ol>
     <li> no output </li>
     <li> output of the filename </li>
     <li> output of the full line where the replacement happens together
     with the line number </li>
     <li> asking for acknowledgement of the change. </li>
    </ol>
   </li>
  </ul>

*/

