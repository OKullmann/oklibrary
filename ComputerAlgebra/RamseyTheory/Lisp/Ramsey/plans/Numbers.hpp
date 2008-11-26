// Oliver Kullmann, 26.11.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/Ramsey/plans/Numbers.hpp
  \brief Plans regarding Ramsey numbers


  \todo Ramsey numbers
  <ul>
   <li> In RamseyTheory/Lisp/Ramsey/Numbers.mac we need to provide everything
   known about Ramsey numbers. </li>
   <li> A "Ramsey parameter tuple" is a tupe [[q_1,...,q_s],r], where s
   is the number of colours (all q_i and r are natural numbers). </li>
   <li> The function ramsey_number(par_tuple) computes a pair, consisting of
   a lower and an upper bound on the Ramsey number for this tuple. </li>
   <li> This function is now available, but needs 
    <ul>
     <li> More bounds added. </li>
     <li> References for the bounds. </li>
     <li> Potentially better calculation for unknown bounds based on smaller q 
     etc. </li>
    </ul>
   </li>
  </ul>

*/

