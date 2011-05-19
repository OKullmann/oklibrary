// Oliver Kullmann, 19.4.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/Satz/plans/general.hpp
  \brief Plans for maintaining solvers from the Satz family

  Yet only satz215 is considered.


  \todo Improve the statistics output
  <ul>
   <li> The output should confirm to the Dimacs-output form. </li>
   <li> The labels for the various numbers should be improved resp.
   introduced at all:
    <ol>
     <li> "NB_BRANCHE" should mean the number of branches, that is,
     the number of nodes - 1; but what then is NB_BACK? It is always
     a bit more than half of NB_BRANCHE. </li>
    </ol>
   </li>
   <li> And we need additional documentation on these numbers. </li>
  </ul>


  \todo Improve range of statistic measurements
  <ul>
   <li> Yet for the four basic statistics variables, <code> NB_UNIT, NB_MONO,
   NB_BRANCHE, NB_BACK </code> just type <code>long</code> is used. </li>
   <li> A typedef should be introduced. </li>
   <li> The type should become unsigned. </li>
   <li> And it should be a 64-bit type. </li>
  </ul>


  \todo Improve file output
  <ul>
   <li> We should use the same conventions as with the OKsolver_2002. </li>
  </ul>


  \todo Correct memory handling
  <ul>
   <li> With Sudoku problems of box-dimension 6 we get a memory access error:
   \verbatim
> satz215 Strong_Sudoku_Box_dim_6.cnf
/h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform/bin/satz215: line 12: 13463 Speicherzugriffsfehler  (core dumped) /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/SAT/Satz/satz215/satz215 $*
   \endverbatim
   </li>
  </ul>


  \todo Use signals as with OKsolver_2002
  <ul>
   <li> The solver should react as OKsolver_2002 to the signals SIGINT and
   SIGUSR1. </li>
  </ul>


  \todo Provide application tests


  \todo Update to C99
  <ul>
   <li> Improve naming (functions, variables, types, macros). </li>
   <li> Improve comments. </li>
   <li> Introduce const. </li>
   <li> Make variables more local. </li>
   <li> Use type bool. </li>
   <li> Add least some macros can be replaced by constants. </li>
  </ul>


  \todo Add Doxygen documentation

*/

