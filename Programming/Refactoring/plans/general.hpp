// Oliver Kullmann, 18.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009 Oliver Kullmann
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
    <ol>
     <li>
     \verbatim
for F in $(find . -type f -name '*.mac' | xargs grep -l "^[[:space:]]*load("); do
  sed 's/^\([[:space:]]*\)load(/\1oklib_plain_include(/' ${F} > temp_file; cat temp_file > ${F}; done
     \endverbatim
     </li>
     <li> This replaces all occurrences of "load(" at the beginning of a line
     (possibly with spaces, which are preserved) by "oklib_plain_include".
     </li>
     <li> This in files with extension ".mac". To just see which files are
     involved, replace the body of the loop by "echo ${F};". </li>
     <li> In the directory where the call is isssued, the auxiliary file
     "temp_file" has been created, which (later) needs to be removed. </li>
    </ol>
   </li>
   <li> One may use Emacs quite easily to refactor a large number of files 
   (see http://xahlee.org/emacs/find_replace_inter.html). The basic idea is
    <ol>
     <li> Open Emacs, and run (ESC-x) "find-dired" specifying the directory, 
     and then "-type f" as the args parameter. This makes emacs list all the
     files (including those in subdirectories) in the given directory.
     <li> Type "%m" and then specify ".*" as the regex to mark all listed 
     files (one could specify "^.*\.mac" or something similar to only mark 
     ".mac" files etc). </li>
     <li> Run (ESC-x) "dired-do-query-replace-regexp" and then specify the
     normal regexp and replacement. Emacs will cycle through the files that
     contain the matching pattern and offer the user the option of replacing
     the pattern for each instance as occurs during a normal search and 
     replace. </li>
     <li> Upon finishing, each of the files containing matching patterns is
     left as an open buffer in the Emacs window, and the user can save these
     files in the usual way (for instance ESC-x save-some-buffers). </li>
     <li> OK: What does this mean? Don't understand how to invoke it ---
     more details please. </li>
    </ol>
   </li>
   <li> Simple method to use xemacs:
    <ol>
     <li> A simple method to invoke xemacs is to use, e.g.
     \verbatim
ComputerAlgebra> xemacs $(grep -rl "binom(" --exclude="*~" *) &
     \endverbatim
     when searching for all occurrences of "binom(". </li>
     <li> This loads all the files into xemacs; via the replace-function
     (which has a memory) one then runs through all occurrences in a file,
     performing replacement if needed. Easiest to just delete the buffer
     after replacement. </li>
    </ol>
   </li>
  </ul>


  \todo Investigating existing tools for parsing and refactoring code
  <ul>
   <li> There was an announcement recently (September 2007) on the boost
   e-mail list about a C/C++ parser etc. library (supported by Apple).
    <ol>
     <li> Likely http://www.llvm.org/ was meant. </li>
     <li> Perhaps http://clang.llvm.org/ for us as "end users" is more
     appropriate; unfortunately yet only C (C++ will take until 2010). </li>
    </ol>
   </li>
   <li> There should be C tools for refactoring --- those should be also
   useful.
    <ol>
     <li> See "Installation of C/C++ tools" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp . </li>
    </ol>
   </li>
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

