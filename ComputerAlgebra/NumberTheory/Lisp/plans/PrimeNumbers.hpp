// Oliver Kullmann, 7.6.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/NumberTheory/Lisp/plans/PrimeNumbers.hpp
  \brief Plans regarding prime numbers


  \todo Connections
  <ul>
   <li> See Experimentation/Investigations/plans/AdditiveNumberTheory.hpp.
   </li>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/Generators/Generators.mac. </li>
  </ul>


  \todo Enumerating prime numbers
  <ul>
   <li> We should ask whether Maxima plans some support for the functions
   we are considering. </li>
   <li> In the e-mail we should give a link to the web page with
   ComputerAlgebra/NumberTheory/Lisp/PrimeNumbers.mac. </li>
   <li> Compare Structures/NumberTheory/PrimeNumbers/plans/RankPrimes.hpp.
   </li>
  </ul>


  \todo count_primes
  <ul>
   <li> There is an enormous literature with estimations of count_primes. </li>
   <li> We should implement such estimations. </li>
   <li> DONE
   Most basic is approx_count_primes_0 and approx_count_primes_1. </li>
   <li> Approximation via the logarithmic integral:
    <ol>
     <li> DONE
     Perhaps we should integrate starting from 0, not from 2? </li>
     <li> DONE
     This yields +1.045, for the prime number 2. </li>
     <li> DONE (works with the build-in function)
     However then the numerical evaluation does not work. </li>
     <li> DONE
     The integral from 0 to x is given by expintegral_li(x). </li>
     <li> The documentation for expintegral_li(x) says
     "expintegral_li (<n>,<z>)", however two arguments are not accepted?
     Ask the Maxima mailing list. </li>
     <li> So currently the higher logarithmic integrals Lih(x,m) can not
     be evaluated using Maxima (and thus we integrate starting with 2). </li>
    </ol>
   </li>
  </ul>


  \todo product_primes
  <ul>
   <li> What is known about this function? </li>
  </ul>

*/

