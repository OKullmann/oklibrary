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


  \todo DONE Inappropriate column names
  <ul>
   <li> DONE Column names must be short, never like
   "decisions_per_sec" or "bin_clauses_after" etc.:
    <ol>
     <li> DONE (shortened names and ensured consistent use of "_c", "_n",
     and "_l")
     Most of the names regarding the OKsolver_2002 are badly chosen. </li>
     <li> DONE (this was from the OKsolver output)
     For example "max_tree_depth": Why "max"? And to what could a depth
     refer than to a tree? </li>
     <li> DONE Instead of "clauses" we use "c". Etc. </li>
     <li> DONE (renamed "max_added_2cl" to "max_added_2c")
     However "l" in "cl" refers to "clause-length"! </li>
    </ol>
   </li>
   <li> DONE
   Once the new names are in places (for all such functions), then also
   specification must be provided (not just some example). </li>
   <li> DONE (removed all uses of the words variables, clauses and literals)
   We also use standardised notations where possible:
    <ol>
     <li> "n" for the number of variables </li>
     <li> "c" for the number of clauses </li>
    </ol>
   </li>
   <li> DONE (added type-casts for all fields)
   And there is also a type mismatch: we get warnings
   "1: inaccurate integer conversion in coercion". </li>
  </ul>


  \bug DONE (types of columns specified)
  False treatment of "decisions"
  <ul>
   <li> Column "decisions" is treated as factor! This is obviously false.
   </li>
   <li> The cause of the problem is that the dataframe was never specified,
   but just some "data put into it". </li>
  </ul>

*/

