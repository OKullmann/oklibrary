// Oliver Kullmann, 9.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/NumberTheory/PrimeNumbers/plans/RankPrimes.hpp
  \brief Plans regarding ranking and unranking of prime numbers


  \todo Connections
  <ul>
   <li> In OKlib/Combinatorics/Hypergraphs/Generators/GreenTao.hpp we have
   Generators::First_prime_numbers for listing the first n prime numbers. </li>
   <li> This function should move here. </li>
  </ul>


  \todo Create application test


  \todo Create unit tests


  \todo Enhance the table
  <ul>
   <li> For ranks from 10^6 to 50*10^6
   http://primes.utm.edu/lists/small/millions/ was used. </li>
   <li> All the primes ranks up to 10^12 are available at
   http://primes.utm.edu/nthprime/index.php . </li>
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


  \todo Improved usage of Gmp-facilities
  <ul>
   <li> See "Application RankPrimes is slower with version 4.3.0/4.3.1"
   in Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp
   for a slow-down with supposedly faster newer versions of Gmp. </li>
   <li> Propose to the Gmp mailing list that for relatively small primes,
   say the first 10^12 ones, randomness is once and for all taken out of
   it by just using some fixed seed (with a fixed pseudo-random generator),
   and figuring out the maximum number of rounds need to make all results
   correct (of course, then one can attempt to optimise the seed). </li>
   <li> Likely, since mpz_nextprime now uses sieving (starting with 4.3.0),
   it could be much faster when it would be given a whole interval to
   work with, not just the next prime number. Mention also this to the Gmp
   mailing list (with the application of ranking and unranking primes). </li>
  </ul>

*/

