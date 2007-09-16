// Oliver Kullmann, 10.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Generators/plans/AMO.hpp
  \brief Plans regarding the AMO-constraint ("at most one") for
  boolean variables


  \todo AMO(L) (V is a set of boolean literals) as a (strong) active clause-set
  is very simple: As soon as one of the literals is set to true, all
  others must be set to false. This seems to be worth doing, and should
  be superior to other solutions. Of course, a problem is the interface
  for heuristics; easily boolean encodings can be simulated.


  \todo Overview on the different representations via clause-sets:
  <ol>
   <li> Of course, the canonical one (via binary clauses). </li>
   <li> AMO(L) is equivalent to UPPER(L, 1) (see LinearInequalities/plans/CardinalityConstraints.hpp). </li>
   <li> See the CFV07-submission "Efficient CNF encoding for selecting at most
   1 from N" for further encodings. </li>
   <li> There was a report in the Speckenmeyer-Institut. </li>
  </ol>


  \todo Likely the ALO-constraint ("at least one") on boolean variables
  is not worth considering on its own?
  <ul>
   <li> ALO(L) is equivalent to LOWER(L, 1) (see LinearInequalities/plans/CardinalityConstraints.hpp). </li>
  </ul>
  
*/

