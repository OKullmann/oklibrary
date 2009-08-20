// Oliver Kullmann, 4.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/SolverMonitoring/plans/OKsolver.hpp
  \brief General plans monitoring OKsolver_2002


  \todo Function plot_oksolver_mon_nodes
  <ul>
   <li> Currently it is hardly visible that one observation node might
   have associated 100 nodes, and another observation might have this
   as well, while the former really advances only one observation level,
   while the latter advances more observation levels, say 10, and thus
   in effect the number of associated nodes for it is closer to
   100 / 10 = 10. </li>
  </ul>


  \todo Function summary_oksolver
  <ul>
   <li> It would be good if one could just say "summary(E)". For this the
   data frames needed to be "marked". </li>
   <li> The R-standard is to have "summary" just outputting text, while by
   "plot" one gets plots; now we have it combined? </li>
  </ul>

*/

