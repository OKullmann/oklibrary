// Oliver Kullmann, 10.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Colouring/plans/general.hpp
  \brief General plans regarding investigations on colouring problems
  
  
  \todo Connections
  <ul>
   <li> Ramsey theory yields a rich reservoir of (hypergraph) colouring
   problems; see Experimentation/Investigations/RamseyTheory/plans/general.hpp.
   </li>
  </ul>


  \todo Graph colouring benchmarks
  <ul>
   <li> http://mat.gsia.cmu.edu/COLOR04/ is perhaps the largest benchmark
   suite. </li>
   <li> The file-format has the following possible lines:
    <ol>
     <li> "p edges n m" </li>
     <li> "e n1 n2" or "e n1 n2 d" to enforce distance d for the colours of
     the endpoints (default=1) </li>
     <li> "f n1 c1 c2 c3 ..." to restrict the colour choice of vertex n1 </li>
     <li> "n n1 c1" for "multicolouring problems", stating that at least c1
     colours must be assigned to n1; likely best to abort with an error in
     this case. </li>
    </ol>
   </li>
   <li> [Tamura, Taga, Kitagawa, Banbara; Compiling finite linear {CSP} into
   {SAT}, Constraints, 2009, 14, 2, 254-272] can solve with their "order
   encoding" 44 of 119 benchmark problems from there (under the heading
   "Graph Coloring Instances". </li>
   <li> Apparently the information given on http://mat.gsia.cmu.edu/COLOR04/,
   for which problems the chromatic numbers are known, is not up-to-date.
   Is there somewhere better information? </li>
  </ul>

*/

