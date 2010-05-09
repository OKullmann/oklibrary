// Oliver Kullmann, 9.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/NumberTheory/PrimeNumbers/plans/GrosswaldHagisFormula.hpp
  \brief Plans regarding the Grosswald-Hagis-formula for counting arithmetic progressions in primes


  \todo Handle several k-values
  <ul>
   <li> An input like
   \verbatim
> GrosswaldHagisFormula-O3-DNDEBUG 5,20 1000000000
   \endverbatim
   would be good, saying that we wish results for 5 <= k <= 20. </li>
   <li> To make parsing easy, no spaces are allowed for that pair. </li>
  </ul>


  \todo Information on convergence rate
  <ul>
   <li> In [Grosswald, Hagis, 1979] it is stated that they determined how
   many digits are definitely correct --- what estimation did they use? </li>
   <li> Are the authors still around, so that we can ask them? </li>
   <li> And is the paper "[E. Grosswald, Arithmetic progressions of primes,
   to appear]" available? </li>
   <li> Optimally, the program would compute the number of definitely correct
   digits, and also then the most precise rounded value. </li>
  </ul>


  \todo Write application tests

*/
