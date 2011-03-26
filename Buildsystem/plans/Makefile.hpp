// Oliver Kullmann, 6.12.2006 (Swansea)
/* Copyright 2006 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/plans/Makefile.hpp
  \brief Plans for the buildsystem-makefile


  \todo Documentation
  <ul>
   <li> How use "oklib" is clear from the documentation. </li>
   <li> What isn't clear is when one must update BuildSystem.
   What steps are needed to ensure "oklib app_tests" recurses
   into sub-modules? </li>
   <li> It seems one should add modules and sub-modules to "OKlib_links"? 
   Otherwise makefiles are not created by "oklib --prebuild". Then 
   "oklib app_tests" does not recurse. </li>
   <li> For example, in BuildSystem/Makefile. Without adding 
   "makefile_investigations" to "OKlib_links", running "oklib app_tests"
   in Experimentation/Investigations does nothing. After adding
   "makefile_investigations", running "oklib app_tests"
   in Experimentation/Investigations runs
   Experimentation/Investigations/BooleanFunctions tests. </li>
  </ul>


  \todo Future of this makefile:
  <ul>
   <li> The ugly variable generic_dirs will vanish with the appearance of
   the masterscript "oklib" (which will call the appropriate makefile).
   And also all other links will likely vanish. </li>
   <li> So the only task left is to create some directories? However this can be handled
   by Buildsystem/SetUp.mak. </li>
   <li> So perhaps after the transition SetUp.mak is renamed to Makefile (taking over
   Buildsystem/Makefile. </li>
  </ul>

*/

