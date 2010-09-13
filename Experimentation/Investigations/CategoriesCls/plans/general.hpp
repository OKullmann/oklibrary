// Oliver Kullmann, 13.9.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/CategoriesCls/plans/general.hpp
  \brief On investigations regarding categories of clause-sets


  \todo Links
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Categories/plans/general.hpp.
   </li>
  </ul>


  \todo Satisfiability of products
  <ul>
   <li> The conjecture is that for a finite family of unsatisfiable flcls's,
   none of them containing the empty clause, their product is unsatisfiable
   as well. </li>
   <li> Considering the product of full clause-sets:
   \verbatim
prl(L) := product_flcls(map(fcls2flcls, map(full_fcs,L)))$
testl(L) := current_satsolver(flcls2fcls(prl(L)))$

testl([0,0]);
  false;
testl([0,1]);
  true;
testl([1,1]);
  false;
testl([1,2]);
  true;

prl([1,2]);
[
  [{[-1,-2],[-1,-1],[-1,1],[-1,2],[1,-2],[1,-1],[1,1],[1,2]},
   lambda([x],map('apply,[lambda([x],-x),lambda([x],-x)],map("[",x)))],
  {{[-1,-2],[-1,-1]},{[-1,-2],[-1,1]},{[-1,-1],[-1,2]},{[-1,1],[-1,2]},
  {[1,-2],[1,-1]},{[1,-2],[1,1]},{[1,-1],[1,2]},{[1,1],[1,2]}}
]
flcls2fcls(prl([1,2]));
  [{1,2,3,4},{{-4,-3},{-4,-2},{-3,-1},{-2,-1},{1,2},{1,3},{2,4},{3,4}}]
all_sat_fcs(flcls2fcls(prl([1,2])));
  {{-4,-1,2,3},{-3,-2,1,4}}
% Solutions interpreted:
[1,1],[1,-1],[-1,2],[-1,-2]
-2,-3,4,1
[-1,-1],[-1,1],[1,-2],[1,2]
2,3,-4,-1

testl(2,2);
  false;
testl(2,3);
  false;
testl(3,3);
  false;

testl([2,2,2]);
  false;
testl([2,2,3]);
  false;
   \endverbatim
   </li>
   <li> The failure for testl([0,1]) was expected, however not that for
   testl([1,2]). Perhaps unit-clauses are also problematic, not just
   empty clauses? </li>
   <li> The new conjecture then would be: A product of literal-based formal
   clause-sets, where all clauses are of length at least 2, is unsatisfiable
   iff at least one part is unsatisfiable. </li>
  </ul>

*/
