// Oliver Kullmann, 14.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Transformers/Generators/plans/LinInequal.hpp
  \brief Plans regarding encoding linear inequalities as CNF


  \todo DONE
  Create application
  <ul>
   <li> DONE (restored and updated old code)
   The application was contained (as commented code)
   in the old Transformers/Generators/Test.cpp, which was removed in
   commit 7ad4d9916dea861abab631f2904e49841a86892f (10.11.2007). </li>
   <li> DONE (completed the main application)
   Now one needs to find out what the code is doing:
    <ol>
     <li> The parameters from the command line are unsigned integers, where
     the last is the "level", while the others are "capacities". </li>
     <li> From standard input first unsigned integers L, B are read, where
     L apparently yields variables V1, ..., VL; B should be the upper bound.
     </li>
     <li> Then apparently from standard input triples
     (identifier, choice, preference) are read, where choice and preference
     are unsigned integers. </li>
    </ol>
   </li>
  </ul>


  \todo Complete description of the LinInequal.cpp
  <ul>
   <li> It remains to describe what happens if two or more parameters are
   given. </li>
   <li> The main point is to find out what LinInequal::Assignment is doing.
   </li>
  </ul>


  \todo Create Doxygen documentation
  <ul>
   <li> It remains to document LinInequal::Assignment. </li>
  </ul>


  \todo Write docus

*/
