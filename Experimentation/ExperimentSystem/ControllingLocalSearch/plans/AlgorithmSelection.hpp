// Oliver Kullmann, 27.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/ControllingLocalSearch/plans/AlgorithmSelection.hpp
  \brief %Tools for choosing good local search algorithms from a collection

  Especially we consider running Ubcsat, while tools are often written in R.


  \todo Connections
  <ul>
   <li>
   Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/AlgorithmSelection.hpp
   is a case study. </li>
   <li> See ExperimentSystem/ControllingLocalSearch/plans/DataCollection.hpp
   for first tools. </li>
  </ul>


  \todo Selecting the best algorithm
  <ul>
   <li> According to my experience (OK), for every (narrow) example
   class (like %Ramsey formulas) there is one best algorithm in the
   Ubcsat selection, which is also relatively easy recognisable. </li>
   <li> First we need to develop a method, and then we should try to
   automatise it. </li>
   <li> The starting point is to find out on a single instance which
   algorithm performs best. </li>
   <li> Then we consider three single instances, an "easy" one, a
   "middle easy" and a "harder" one, and see whether the predictions
   scale, where at least for the evaluation of our method we consider
   all algorithms. </li>
  </ul>

*/

