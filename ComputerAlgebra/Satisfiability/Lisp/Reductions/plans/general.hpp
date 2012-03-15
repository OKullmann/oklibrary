// Oliver Kullmann, 31.1.2008 (Swansea)
/* Copyright 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Lisp/Reductions/plans/general.hpp
  \brief Plans for Maxima-components regarding SAT-reductions (in general)

  Packaging of reductions into some "preprocessing" is handled in
  Satisfiability/Lisp/Preprocessing.


  \todo DONE (function hardness_wpi_cs has been provided)
  Computing the hardness of a clause-set representation
  <ul>
   <li> NEEDS UPDATE. </li>
   <li> See "Hardness of boolean function representations" in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp.
   </li>
   <li> To compute the hardness of a clause-set F we can do the following:
   \verbatim
/* Computing the hardness of a clause-set F with its prime implicates F_PI
   as input. */
hardness_wpi_cs(F,F_PI) := block([max_k : 0, count : 0, maxcount : length(F_PI)],
  maxcount : length(F_PI),
  for C in F_PI do (
    count : count + 1,
    if mod(count, 50) = 0 then
      print(sconcat("[",count,"/",maxcount,"] Hardness(F) >= ", max_k," ...")),
    for k : 0 while generalised_ucp(apply_pa(comp_sl(C),F),k) # {{}} do
      max_k : max(max_k, k + 1)),
  return(max_k))$

hardness_cs(F) := hardness_wpi_cs(F,min_resolution_closure_cs(F)[1])$
   \endverbatim
   </li>
   <li> DONE (see Hardness.mac)
   A new file Hardness.mac should be created in this module and
   these functions transferred there with tests. </li>
  </ul>


  \todo Create milestones


  \todo Write docus

*/

