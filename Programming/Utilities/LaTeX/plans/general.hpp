// Oliver Kullmann, 12.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file LaTeX/plans/general.hpp
  \brief Plans for the LaTeX-Module


  \todo Cleaning up
  <ul>
   <li> Remove code which concerns the timetable-database:
    <ol>
     <li> "Doc_Object_Implementation.hpp" </li>
     <li> "Latex_Handler_Implementation.hpp" </li>
    </ol>
   </li>
   <li> DONE (files which are concerned only with the timetable have been
   removed)
   Currently the module contains mainly code for producing
   the timetable for the department. This has to be factored out into
   some external module, so that only the latex-relateted components
   remain. (If this is useful at all.) </li>
   <li> DONE (removed) The file Data/Timetable_SQL is no longer needed? </li>
  </ul>


  \todo Future plans
  <ul>
   <li> We should provide latex-output of clause-sets and
   SAT-decision trees. </li>
   <li> Compare with the Maxima-functionality. </li>
  </ul>


  \todo Update namespaces


  \todo Update namespace-usage


  \todo Create milestones.

*/

/*!
  \namespace OKlib::LaTeX
  \brief Tools for LaTeX-Output
*/

namespace OKlib {
  namespace LaTeX {
  }
}

