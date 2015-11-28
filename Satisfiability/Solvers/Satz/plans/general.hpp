// Oliver Kullmann, 19.4.2009 (Swansea)
/* Copyright 2009, 2010, 2011, 2013, 2015 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/Satz/plans/general.hpp
  \brief Plans for maintaining solvers from the Satz family

  Yet only satz215 is considered.


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
   <li> Introduce const (possibly completed). </li>
   <li> Make variables more local (possibly completed). </li>
   <li> Use type bool. </li>
   <li> At least some macros can be replaced by constants. </li>
  </ul>


  \todo Add Doxygen documentation

*/

