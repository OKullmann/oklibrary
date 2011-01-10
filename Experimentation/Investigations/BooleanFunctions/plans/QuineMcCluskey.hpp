// Oliver Kullmann, 10.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/QuineMcCluskey.hpp
  \brief On investigations around the Quine-McCluskey algorithm


  \todo Computing all prime clauses for a full clause-set
  <ul>
   <li> This is a basic task, which one needed to fully understand. </li>
   <li> As one result of MGs PhD thesis we should get a "best and optimal"
   implementation. </li>
   <li> See OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp
   for the C++ level. </li>
   <li> And see "min_2resolution_closure_cs" in
   ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/PrimeImplicatesImplicants.hpp
   for the Maxima-level (discussing the open problem regarding the complexity,
   where one considers complexity in the input-clause-set).
   </li>
   <li> Testcases:
    <ol>
     <li> Two extreme cases are considered in "Understanding the costs of the
     current implementation" in
     OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp. </li>
     <li> We need also intermediate cases. </li>
     <li> Perhaps generalised AES Sboxes are good examples. </li>
    </ol>
   </li>
   <li> See there also "Hitting clause-sets" for possible generalisation to
   hitting clause-sets. </li>
  </ul>

*/

