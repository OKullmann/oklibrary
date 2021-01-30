// Oliver Kullmann, 30.1.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/benchmarks/SATcompetitions.hpp
  \brief The TawSolver on benchmarks from the SAT competitions


  \todo SAT 2014
  <ul>
   <li> The natural starting point is the SAT competition
   http://www.satcompetition.org/ , and all instances should be run, with
   a time-out of say 3600s (or shorter, if this takes too long). </li>
   <li> Currently tawSolver does not allow a timeout, but it can be achieved
   as follows:
   \verbatim
> timeout --signal=SIGINT 3600 tawSolver File
   \endverbatim
   (interrupt here after 3600s).
   </li>
   <li> Running through the crafted instances from SAT2014, with timeout of
   600sec; data/History (sc14_craftet_600). Solved instances:
    <ol>
     <li> Satisfiable: "289-*", "Chvatal-*", "Composite-*", "mod2-*",
     "prime-*", "Q3inK11-*", "rbsat-*", "rnd-*", "sgen3-*", "sgen4-*",
     "toughsat-*" , "VanDerWaerden-*". </li>
     <li> Unsatisfiable: "edges-*", "sgen6-*". </li>
    </ol>
   </li>
   <li> Now running the same with ttawSolver. XXX
   </li>
   <li> Running through the applications-instances from SAT2014, with timeout
   of 600 sec (sc14_app_600). Only satisfiable instances solved:
   "complete-*", "stable-*". </li>
   <li> Now running the same with ttawSolver. XXX </li>
  </ul>

*/
