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
   <li> Steps required to add module M to the library:
    <ul>
     <li> Add make-variable "makefile_XXX" to BuildSystem/Makefile. </li>
     <li> XXX is a shortened version of the module name. </li>
     <li> The value of this make-variable is the path to the Makefile
     Module/makefile. </li>
     <li> For example for the Experimentation/Investigations module, we add:
     \verbatim
makefile_investigations := $(OKsystem)/OKlib/Experimentation/Investigations/makefile
     \endverbatim
     </li>
     <li> Add "makefile_XXX" to "OKlib_links" in BuildSystem/Makefile. </li>
    </ul>
   </li>
   <li> These steps are not documented. They should appear in the docus. 
   </li>
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

