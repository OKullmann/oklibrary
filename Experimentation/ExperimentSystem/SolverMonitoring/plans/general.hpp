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


  \todo Add monitoring for all other solvers


  \bug DONE (types of columns specified)
  False treatment of "decisions"
  <ul>
   <li> Column "decisions" is treated as factor! This is obviously false.
   </li>
   <li> The cause of the problem is that the dataframe was never specified,
   but just some "data put into it". </li>
  </ul>


  \todo Inappropriate column names
  <ul>
   <li> Column names must be short, never like
   "decisions_per_sec" or "bin_clauses_after" etc.:
    <ol>
     <li> Most of the names regarding the OKsolver_2002 are badly chosen. </li>
     <li> For example "max_tree_depth": Why "max"? And to what could a depth
     refer than to a tree? </li>
     <li> Instead of "clauses" we use "c". Etc. </li>
     <li> However "l" in "cl" refers to "clause-length"! </li>
    </ol>
   </li>
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

