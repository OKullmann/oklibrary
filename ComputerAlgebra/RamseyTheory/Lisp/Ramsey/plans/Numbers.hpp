// Oliver Kullmann, 26.11.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/Ramsey/plans/Numbers.hpp
  \brief Plans regarding %Ramsey numbers


  \todo %Ramsey numbers
  <ul>
   <li> A "Ramsey parameter tuple" is a tupe [[q_1,...,q_s],r], where s
   is the number of colours (all q_i and r are natural numbers). </li>
   <li> The function ramsey_number(par_tuple) computes a pair, consisting of
   a lower and an upper bound on the %Ramsey number for this tuple. </li>
   <li> This function is now available, but needs 
    <ul>
     <li> More bounds added: for this see the running report of [Radziszowski,
     Small %Ramsey Numbers]. </li>
     <li> References for the bounds; compare "Bibtex-database" in
     Transitional/plans/Annotations.hpp. </li>
     <li> The known general upper and lower bounds need to be integrated:
      <ol>
       <li> The system must be extensible. </li>
       <li> Perhaps one "bound function" delivers:
        <ul>
         <li> the function which from the given parameter tuple computes
         the other parameter tuple for which bounds are needed </li>
         <li> and the function which from these other bounds computes
         the bounds. </li>
        </ul>
        Then ramsey_number recursively invokes all registered bound functions,
        and uses minimum resp. maximum.
       </li>
       <li> Potentially lower and upper bounds are mixed, so we always compute
       intervals. </li>
       <li> The system needs to general enough, so that also the values given
       by ramsey_hm can be checked (possibly improved).
        <ol>
         <li> It might be good to extend the ramsey_hm to contain information
         on how currently the best bounds are obtained. </li>
         <li> One possibility is for the interesting small parameter values
         just to use comments. </li>
        </ol>
       </li>
      </ol>
     </li>
    </ul>
   </li>
   <li> Important that the system developed here can later be generalised to
   yield bounds on all sorts of combinatorial parameters.
    <ol>
     <li> Compare
     ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/uhit_def.hpp
     for another catalogue of data (and instances). </li>
    </ol>
   </li>
  </ul>

*/

