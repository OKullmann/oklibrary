// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/plans/Literals.hpp
  \brief Plans for literals ("concrete")


  \todo Boolean literals, and more general literals
  <ul>
   <li> We have a notion of "boolean variables" and "boolean literals" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac;
   seems sufficient, and needs documentation.
    <ol>
     <li> The universal set of values is {false,true}. </li>
     <li> Given a boolean literal x, the associated condition is
   \verbatim
lambda([f], is(f(v) = is(x > 0)))
   \endverbatim
     </li>
    </ol>
   </li>
   <li> A "monosigned literal" is a pair [v,e], where v is the variable
   (just a symbol) and e the value.
    <ol>
     <li> var_m([v,e]) = v and val_m([v,e]) = e. </li>
     <li> Interpreted as a CNF-literal (a "negative monosigned literal"),
     the associated condition is
     \verbatim
lambda([f], is(f(v) # e))
     \endverbatim
     while for a DNF-literal (a "positive monosigned literal") we have
     \verbatim
lambda([f], is(f(v) = e))
     \endverbatim
     </li>
    </ol>
   </li>
   <li> A "signed literal" is a pair [v,S], where S is a set
   of values.
    <ol>
     <li> var_s([v,S]) = v and val_s([v,S]) = S. </li>
     <li> For a "negative signed literal" (CNF-literal) the associated condition
     is
     \verbatim
lambda([f], not elementp(f(v), S))
     \endverbatim
     while for the "positive signed literal" (DNF-literal) it is
     \verbatim
lambda([f], elementp(f(v), S))
     \endverbatim
     </li>
    </ol>
   </li>
   <li> We need to provide the standard promotion of literals (to more
   general literals). </li>
   <li> Compare "Domain association and allowed total assignments"
   in ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp. </li>
   <li> DONE (we use concrete representations, exactly as in the mathematical
   definition)
   Perhaps there is a third entry? With tags? No. </li>
  </ul>

*/

