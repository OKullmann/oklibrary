// Oliver Kullmann, 9.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/NumberTheory/PrimeNumbers/plans/RankPrimes.hpp
  \brief Plans regarding ranking and unranking of prime numbers


  \todo Create application test


  \todo Enhance the table
  <ul>
   <li> For ranks from 10^6 to 50*10^6
   http://primes.utm.edu/lists/small/millions/ was used. </li>
  </ul>


  \todo Better error codes
  <ul>
   <li> Are there some standard codes? </li>
  </ul>


  \todo Write wrapper script
  <ul>
   <li> We should provide two wrapper scripts "RankPrimes" and "UnrankPrimes",
   which provide the path to the standard data file, and set the second
   parameter. </li>
   <li> The problem is how to access the make-variables "bin_dir" (for the
   executable) and "OKlib" (for the data file). </li>
   <li> Perhaps the script invokes "oklib" with a new parameter type,
   which just calls some application --- in this way the (local) environment
   gets enriched by all the make variables. </li>
  </ul>


  \todo Use Messages

*/

