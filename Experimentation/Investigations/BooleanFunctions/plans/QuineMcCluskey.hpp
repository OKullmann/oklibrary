// Oliver Kullmann, 10.1.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/QuineMcCluskey.hpp
  \brief On investigations around the Quine-McCluskey algorithm


  \todo Computing all prime clauses for a full clause-set
  <ul>
   <li> This is a basic task, which one needed to "fully" understand. </li>
   <li> As one result of MGs PhD thesis we should get a "best and optimal"
   implementation (choosing appropriately, depending on the input, from the
   various algorithms). </li>
   <li> See OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp
   for the C++ level. </li>
   <li> And see "min_2resolution_closure_cs" in
   Satisfiability/Lisp/Primality/plans/PrimeImplicatesImplicants.hpp
   for the Maxima-level (discussing the problem regarding the precise
   complexity, where one considers complexity in the input-clause-set).
   </li>
   <li> The (dedicated) module for prime-clause-related computations at
   Maxima-level is Satisfiability/Lisp/Primality/plans/general.hpp; see
   "Computations of all prime clauses for full clause-sets" there. </li>
   <li> Testcases:
    <ol>
     <li> Two extreme cases are considered in "Understanding the costs of the
     current implementation" in
     OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp. </li>
     <li> We need also intermediate cases. </li>
     <li> Perhaps generalised AES Sboxes are good examples. </li>
    </ol>
   </li>
   <li> See also "Hitting clause-sets" in
   Satisfiability/Lisp/Primality/plans/PrimeImplicatesImplicants.hpp for
   possible generalisation to hitting clause-sets. </li>
  </ul>

*/

