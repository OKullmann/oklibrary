// Oliver Kullmann, 25.9.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AutarkySearch/plans/EvaluatePartialAssignments.hpp
  \brief Evaluation of partial assignments, computing a heuristical
  value of how close the assignment is to an autarky for a given
  clause-set.

  The main use for such evaluation is in the direct search for
  autarkies via local search.


  \todo The two most basic measures are the number of critical clauses
  and the number of variables of (assigned) variables involved in critical
  clauses. Since this gives preferences to small partial assignments, one
  can standardise these numbers by dividing through the number of
  all touched clauses resp. the number of all assigned variables.


  \todo The main design issue for evaluation concepts is how to
  integrate the evaluation process into the updating mechanism (so
  that unnecessary runs over the clause-set can be avoided).

*/

