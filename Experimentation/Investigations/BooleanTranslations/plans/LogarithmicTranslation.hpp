// Oliver Kullmann, 17.1.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanTranslations/plans/Logarithmic.hpp
  \brief On investigations regarding the logarithmic translations

  The standard nested translation is computed by nbfclud2fcl_logarithmic(FF)
  at Maxima-level.


  \todo Strong forms
  <ul>
   <li> In case the number k of colours is not a power of 2, then there are
   "remaining" clauses, and with this clauses one can for example perform
   subsumption resolutions with the main clauses as well as with other
   "remainining" clauses. </li>
   <li> For example k=3:
    <ol>
     <li> The three main clauses are {-1,-2},{-1,2},{1,-2}, while the
     remaining clause is {1,2}. </li>
     <li> So here the main clauses can be simplified to {-1,-2},{2},{1} (which
     would yield the strong reduced translation). </li>
    </ol>
   </li>
   <li> For example k=5:
    <ol>
     <li> We have main clauses {-1,-2,-3},{-1,-2,3},{-1,2,-3},{-1,2,3},
     {1,-2,-3}, and remaining clauses {1,-2,3},{1,2,-3},{1,2,3}. </li>
     <li> So {1,2,-3},{1,2,3} can (and should) be simplified to the single
     clause {1,2}, where then main clauses 3,4,5 can then be
     simplified to {2,-3},{2,3},{1,-3}. </li>
     <li> Furthermore then remaining clause 6 can be simplified to {1,3}. </li>
     <li> So main clause 2 can be simplified to {-2,3}, while new main clause
     5 can be further simplified to {1}. </li>
     <li> The result is five main clauses {-1,-2,-3},{-2,3},{2,-3},{2,3},{1},
     and two remaining clauses {1,3},{1,2}. </li>
     <li> So now the five main clauses are minimally unsatisfiable themselves.
     </li>
     <li> Using a different order on the 8 full clauses over {1,2,3}, one might
     get a different result (since then we have a different partition into
     main and remaining clauses). </li>
    </ol>
   </li>
  </ul>

*/
