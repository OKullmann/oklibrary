// Oliver Kullmann, 4.4.2008 (Swansea)
/* Copyright 2008, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Statistics.hpp
  \brief Plans regarding statistics for clause-sets


  \todo Comprehensive statistics
  <ul>
   <li> We need a tool which allows us to compute really *all* our statistics.
   </li>
   <li> See "Comprehensive statistics" in
   Interfaces/InputOutput/plans/general.hpp for the C++ implementation. </li>
   <li> We have extended_statistics_fcs, but this just collects a few
   statistics, and extensions would break applications. </li>
   <li> Likely extended_statistics_fcs should just stay as it is, and
   we need another, more general method. </li>
   <li> Introduce comprehensive_statistics_fcs(L,F), where L is a list of
   statistics functions f(F), taking an fcs F as argument. </li>
   <li> The output is a the collection of statistics-descriptions and values.
   </li>
   <li> This could either be a string, or it prints out all the values. </li>
   <li> The variables all_fcs_statistics is a list of all our statistics.
   </li>
   <li> comprehensive_statistics_all_fcs(F) :
     comprehensive_statistics_fcs(all_fcs_statistics,F). </li>
   <li> The output of f(F) is a pair [description-string,value]. </li>
   <li> We use a new file, "ComprehensiveStatistics.mac". </li>
   <li> There then also the wrappers for ordinary statistics (which just
   yield values) are provided. </li>
   <li> The wrappers have the name-suffix "_sw" (for "statistics wrapper").
   </li>
  </ul>


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

