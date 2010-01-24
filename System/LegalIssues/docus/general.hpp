// Oliver Kullmann, 23.9.2007 (Swansea)
/* Copyright 2007, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file LegalIssues/docus/general.hpp
  \brief How to handle licences in files


  <h1> Adding and maintaining licence statements </h1>

  All non-data files in the OKlibrary carry the following information:
  <ol>
   <li> Creation information about who created the file when and where, in the form
   \verbatim
// Oliver Kullmann, 23.9.2007 (Swansea)
   \endverbatim
   This information refers to the "physical"
   act of creation (and thus in the above example, OK was physically in Swansea
   when creating this file, and to what degree the creator was actually
   involved in creating the file-content doesn't matter). </li>
   <li> The copyright line with OK's copyright. </li>
   <li> The licence statement. </li>
  </ol>
  The only exceptions (besides data files) are:
  <ul>
   <li> Files copied from other places and adapted to the OKlibrary (they
   carry their old copyright/licence statements). </li>
   <li> We do not add creation-information to html-files. </li>
  </ul>


  <h2> Different file types </h2>

  <ul>
   <li> For C/C++ like files use LegalIssues/Header.h, inserted
   after the initial line with creation info. </li>
   <li> For script-files use LegalIssues/Header.s, inserted
   after the initial line with the script-info and the following line with the
   creation info. </li>
   <li> For make-files use LegalIssues/Header.s, inserted
   after the initial line with the creation info. </li>
   <li> For Latex-files uses LegalIssues/Header.tex, inserted
   after the initial line with the creation info. </li>
  </ul>
  Please see existing files for role models.


  <h2> Tools </h2>

  LegalIssues/AddLicence1 adds the licence text to C/C++ like files, while
  LegalIssues/AddLicence2 adds the licence text to script files ("1" resp. "2"
  refers to the number of initial lines with creation-information after which
  the licence text LegalIssues/Header.h resp. LegalIssues/Header.s is
  inserted). Usage:
  <ul>
   <li> Applying the licence-insertion for C/C++ like files with
   \verbatim
OKlib> for F in $(find * -name "*.hpp" -or -name "*.cpp" -or -name "*.h" -or -name "*.c")
  do 
    System/LegalIssues/AddLicence1 ${F}
  done > Exceptions
   \endverbatim
   where in file Exceptions one finds those files which do not match the
   regular expression for the creation-information-line. </li>
   <li> At the moment, LegalIssues/AddLicence2 is a bit more advanced, taking
   2 arguments, and only if the second argument is "override" then replacement
   takes place, as in
   \verbatim
OKlib> for F in $(find * -type f -and -not -type l -and -not -name "*~" -and -not -name "*.c" \
    -and -not -name "*.cpp" -and -not -name "*.h" -and -not -name "*.hpp")
  do
    ${OKPLATFORM}/OKsystem/OKlib/System/LegalIssues/AddLicence2 ${F} "override";
  done > Exceptions
   \endverbatim
   which inserts the licence text into script-files. </li>
   <li> Just the list of files which do fit in principle, but where the
   creation-info-line doesn't look right, is extracted with any other second
   argument, for example
   \verbatim
OKlib> for F in $(find * -type f -and -not -type l -and -not -name "*~" -and -not -name "*.c" \
    -and -not -name "*.cpp" -and -not -name "*.h" -and -not -name "*.hpp")
  do
    ${OKPLATFORM}/OKsystem/OKlib/System/LegalIssues/AddLicence2 ${F} "noop";
  done
   \endverbatim
   </li>
  </ul>


*/
