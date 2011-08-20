// Matthew Gwynne, 22.9.2009 (Swansea)
/* Copyright 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/SolverMonitoring/plans/Minisat2.hpp
  \brief General plans monitoring Minisat2


  \todo Move read_minisat2_output functionality to wrapper script
  <ul>
   <li> This is to be achieved with "Extraction tools" in
   ExperimentSystem/SolverMonitoring/plans/general.hpp. </li>
   <li> Once the extraction scripts are in place, the "read_minisat_output"
   function can be replaced with function similar to read_oksolver_mon
   which just calls read.table and specifies the column types. </li>
   <li> An additional (optional) parameter should be a "parameter" column
   name, which specifies which column is the key parameter being considered
   in the experiment data:
    <ul>
     <li> We could consider the possibility of multiple parameters, but
     it is best to only do this when we encounter the need. </li>
     <li> The data.frame would then be ordered based on the selected
     column. </li>
     <li> Such parameters are extracted from the filenames of solver
     output by the extraction scripts, see "Extraction tools" in
     ExperimentSystem/SolverMonitoring/plans/general.hpp. </li>
    </ul>
   </li>
   <li> We also need function for plotting standard graphs and computing
   standard statistics from each solvers output. </li>
   <li> Some solvers output a lot of data; functions to extract key data
   (columns) for inclusion in the experiment plans is also necessary. </li>
  </ul>

*/
