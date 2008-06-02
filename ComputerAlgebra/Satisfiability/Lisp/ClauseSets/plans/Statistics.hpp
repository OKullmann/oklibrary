// Oliver Kullmann, 4.4.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Statistics.hpp
  \brief Plans regarding statistics clause-sets


  \todo Further statistics
  <ul>
   <li> DONE A map from literals to literal-degrees. </li>
   <li> DONE A map from variables to variable-degrees. </li>
   <li> DONE (as a sorted list)
   Inversely, for every variable- resp. literal degree the number
   of variables resp. literals with that degree. </li>
   <li> DONE Tests whether a clause-set is variable-regular or literal-regular.
   </li>
  </ul>


  \todo Naming
  <ul>
   <li> variable_degree_cs should become var_deg_cs. </li>
   <li> variable_degrees_cs perhaps should become var_degs_cs; or
   perhaps var_deg_hm_cs. </li>
   <li> min_variable_degree_cs should become min_var_deg_cs. </li>
   <li> max_variable_degree_cs should become max_var_deg_cs. </li>
   <li> variableregularcsp should become varregcsp. </li>
   <li> mean_variable_degree_cs should become mean_var_deg_cs. </li>
   <li> variable_degrees_list_cs perhaps sould become var_deg_l_cs. </li>
  </ul>

*/

