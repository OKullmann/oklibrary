// Oliver Kullmann, 12.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/general.hpp
  \brief Plans for Maxima-components related to conflict-patterns of clause-sets


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo Hermitian rank
  <ul>
   <li> It seems we should use hermitian_rank_charpoly instead of
   hermitian_rank for hermitian_rank_cs. </li>
   <li> We need the computation of the characteristic polynomial of a
   clause-set; see ComputerAlgebra/LinearAlgebra/Lisp/plans/general.hpp. </li>
   <li> Of course, there are many graphs associated with a clause-set,
   but it seems that only the characteristic polynomial of the conflict
   multigraph is of interest? So that we can speak of "the" characteristic
   polynomial? </li>
  </ul>


  \todo Relations to other modules
  <ul>
   <li> See Experimentation/Investigations/plans/general.hpp. </li>
  </ul>


  \todo mcind_cs, cind_cs:
  <ul>
   <li> DONE The naming should reflect the special method used (here using
   transversals). </li>
   <li> Likely the best is just to refer to all possible ways of
   computing maximal independent sets of a hypergraph (or, of a graph;
   likely one can take advantage of this more special situation). </li>
   <li> It seems not to be worth to develop an algorithm which computes
   directly all indepedent sets? </li>
  </ul>
  
*/

