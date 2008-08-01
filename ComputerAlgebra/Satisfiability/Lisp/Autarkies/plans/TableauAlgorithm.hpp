// Oliver Kullmann, 1.8.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/TableauAlgorithm.hpp
  \brief Plans regarding exploitation of tableau algorithms for finding autarkies


  \todo find_aut_tableau_cl
  <ul>
   <li> Currently, this algorithm realises only the obvious variables not
   usable in an autarky. </li>
   <li> While all variables involved in the refutation found can be used. </li>
   <li> So a stronger version of find_aut_cl_tableau_cl is needed. </li>
   <li> A question is whether this just improves find_aut_cl_tableau_cl, or
   whether this makes find_aut_cl_tableau_cl so complicated, that we better
   have two versions. </li>
  </ul>

*/

