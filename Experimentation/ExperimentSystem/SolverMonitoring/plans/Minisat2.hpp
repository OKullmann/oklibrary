// Matthew Gwynne, 22.9.2009 (Swansea)
/* Copyright 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/SolverMonitoring/plans/Minisat2.hpp
  \brief General plans monitoring Minisat2


  \todo Improving read_minisat_output : MG, please update
  <ul>
   <li> The simplification time is missing. </li>
   <li> The column-names are too long; planning is needed, and standardisation,
   if possible, with the other extraction-tools. </li>
   <li> This is to be achieved with "Extraction tools" in
   ExperimentSystem/SolverMonitoring/plans/general.hpp. </li>
  </ul>


  \todo Improved handling of file-names : MG, please update
  <ul>
   <li> Typically, filenames contain an important parameter, like
   "VanDerWaerden_pd_2-6-7_350.cnf_OUT" containing 350, the number of
   vertices. </li>
   <li> There should be a stronger version of read_minisat_outputs, which
   allows to create a mask for extracting such a parameter, storing it as
   "param" (an integer) in the dataframe. </li>
   <li> The order of rows then should follow increasing parameter values. </li>
   <li> This is to be achieved with "Extraction tools" in
   ExperimentSystem/SolverMonitoring/plans/general.hpp. </li>
  </ul>


  \todo Move read_minisat2_output functionality to wrapper script :
  MG, please update
  <ul>
   <li> This is to be achieved with "Extraction tools" in
   ExperimentSystem/SolverMonitoring/plans/general.hpp. </li>
  </ul>


  \bug DONE (specifications improved; additional todos added)
  Bad specifications and documentation
  <ul>
    <li> DONE (used simple examples; specified data types; improved
    specification; created additional todos)
    See comments in SolverMonitoring/Minisat2.R, marked by "???".
   </li>
   <li> DONE The same also applies at other places. </li>
  </ul>

*/

