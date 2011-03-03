// Oliver Kullmann, 4.8.2009 (Swansea)
/* Copyright 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/SolverMonitoring/plans/general.hpp
  \brief General plans regarding monitoring solvers


  \todo Create milestones


  \bug Inappropriate column names
  <ul>
   <li> Column names must be short, never like
   "decisions_per_sec". </li>
   <li> We also use standardised notations where possible:
    <ol>
     <li> "n" for the number of variables </li>
     <li> "c" for the number of clauses </li>
    </ol>
   </li>
   <li> Once the new names are in places (for all such functions), then also
   specification must be provided (not just some example). </li>
   <li> And there is also a type mismatch: we get warnings
   "1: inaccurate integer conversion in coercion". </li>
  </ul>

*/

