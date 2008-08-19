// Oliver Kullmann, 18.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Refactoring/plans/general.hpp
  \brief Plans for the module on refactoring


  \todo Connections to other parts
  <ul>
   <li> We not only need refactoring for C++, but also for Maxima/Lisp. </li>
   <li> See "File load and include" in ComputerAlgebra/plans/Maxima.hpp
   for an example. </li>
  </ul>


  \todo Simple Unix/Linux tools
  <ul>
   <li> We need a little methodology for using these tools for simple
   text-replacement (using regular expressions). </li>
   <li> An example:
    <ol.
     <li>
     \verbatim
for F in $(find . -type f -name '*.mac' | xargs grep -l "^[[:space:]]*load("); do
  sed 's/^\([[:space:]]*\)load(/\1oklib_plain_include(/' ${F} > temp_file; cat temp_file > ${F}; done
     \endverbatim
     <li>
     <li> This replaces all occurrences of "load(" at the beginning of a line
     (possibly with spaces, which are preserved) by "oklib_plain_include".
     </li>
     <li> This in files with extension ".mac". To just see which files are
     involved, replace the body of the loop by "echo ${F};". </li>
     <li> In the directory where the call is isssued, the auxiliary file
     "temp_file" has been created, which needs to be removed. </li>
    </ol>
   </li>
  </ul>


  \todo Investigating existing tools for parsing and refactoring code
  <ul>
   <li> There was an announcement recently (September 2007) on the boost
   e-mail list about a C/C++ parser etc. library (supported by Apple). </li>
   <li> See what they have on refactoring. </li>
   <li> There should be C tools for refactoring --- those should be also
   useful. </li>
   <li> C++ and Maxima/Lisp are most relevant (at this time). </li>
  </ul>


  \todo Update namespaces.


  \todo Update namespace-usage.


  \todo Update the doxygen-documentation, and create further plans(-files).


  \todo Move Concepts/RefactoringIncludeHandling.hpp here.


  \todo Transfer the tests to the new test system.

*/

/*!
  \namespace OKlib::Refactoring
  \brief Tools for refactoring C++ code.
*/

namespace OKlib {
  namespace Refactoring {
  }
}

