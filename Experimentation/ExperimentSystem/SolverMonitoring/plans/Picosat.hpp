// Matthew Gwynne, 22.9.2009 (Swansea)
/* Copyright 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/SolverMonitoring/plans/Picosat.hpp
  \brief General plans monitoring Picosat


  \todo Move read_picosat_output functionality to wrapper script
  <ul>
   <li> The functionality currently presented by read_picosat_output
   should go to a wrapper script, such as "picosat-okl", which
   causes picosat to output it's results in a form readable by
   read.table in R. </li>
   <li> Better however is to write a script, which examines a complete
   directory of solver-outputs, and creates an R-readable file containing
   all data, where this file is stored in the directory. </li>
   <li> This is better since then we have the original output as well as
   the extracted data. And it leaves the look-and-feel of the solver intact.
   </li>
   <li> Remark: with the Ubcsat-wrapper it's different, since there we only
   need to supply default parameter (which can also be overwritten if needed).
   </li>
   <li> See "Improved handling of file-names" in
   ExperimentSystem/SolverMonitoring/plans/Minisat2.hpp. </li>
  </ul>

*/

