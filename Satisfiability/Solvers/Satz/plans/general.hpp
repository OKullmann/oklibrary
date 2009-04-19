// Oliver Kullmann, 19.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/Satz/plans/general.hpp
  \brief Plans for maintaining solvers from the Satz family


  \todo Updating the exit codes for Satz215
  <ul>
   <li> According to Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp
   the exit codes need to be updated to the DIMACS standard. </li>
   <li> In order to follow the given style, we just introduce new
   exit-code macros
    <ol>
     <li> EXITCODE_PARAMETER_FAILURE </li>
     <li> EXITCODE_INPUT_ERROR </li>
     <li> EXITCODE_VERIFICATION_FAILED </li>
     <li> EXITCODE_SAT </li>
     <li> EXITCODE_UNSAT </li>
    </ol>
   </li>
   <li> And the exit-status must be stored, so that it can be returned
   at the end of the main program.
    <ol>
     <li> In order to stay with the pre-C99 style, we just declare an
     additional variable at the beginning of the main-block, called
     exit_value and of type int. </li>
    </ol>
   </li>

*/

