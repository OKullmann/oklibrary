// Oliver Kullmann, 9.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/NumberTheory/PrimeNumbers/plans/GrosswaldHagisFormula.hpp
  \brief Plans regarding the Grosswald-Hagis-formula for counting arithmetic progressions in primes


  \todo Connections
  <ul>
   <li> See RamseyTheory/GreenTaoProblems/plans/CountingProgressions.hpp. </li>
  </ul>


  \todo Improved organisation
  <ul>
   <li> Create proper classes and functions, putting them into
   PrimeNumbers/GrosswaldHagisFormula.hpp. </li>
   <li> See the requests below for further functionality. </li>
  </ul>


  \todo Improved usage
  <ul>
   <li> That automatically the output is copied to a file with some standard
   name seems useful. </li>
   <li> DONE
   And output independent of the computation should be done at once. </li>
   <li> The number of output digits should be an additional, optional
   parameter. </li>
   <li> DONE
   The number of primes found for both parts should be output. </li>
   <li> As an optional parameter we should allow a start prime, bigger than k,
   and then just the "infinitary" result is output, with full precision.
    <ol>
     <li> This could be handled like the optional input for k, namely as a pair
     of numbers, separated by a comma (without spaces). </li>
     <li> We need then to adopt the output; definitely we also want here the
     number of primes considered. </li>
    </ol>
   </li>
   <li> The application should catch, like the OKsolver_2002, signals SIGINT
   and SIGUSR1, and print out the current results (before aborting in the
   former case). </li>
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
   <li> This task would consist in proving a lower bound 1-eps for the
   remaining infinite product (assuming that it is less than 1), and in
   controlling the error in the computations (see "Controlling the error in
   the computations" below). </li>
   <li> First one needs to establish whether the factors in the infinite
   product are always smaller than 1, and whether they are strongly
   monotonically increasing. </li>
   <li> However, such considerations are best done at Maxima-level; see
   "Estimating the factors in the infinite Grosswald-Hagis-product" in
   ComputerAlgebra/RamseyTheory/Lisp/GreenTao/plans/Asymptotics.hpp. </li>
  </ul>


  \todo Controlling the error in the computations
  <ul>
   <li> Cutting off the infinite product at max_p, we obtain a finite
   computation; let's call the result of the finite product A_k, and B_{k,N}
   the result of the infinite product only up to N. </li>
   <li> We compute A_k precisely, but precise computation of B_{k,N} seems
   infeasible for larger N. </li>
   <li> Can we say something interesting about the error in the floating-point
   computation for B_{k,N} ? </li>
  </ul>


  \todo Write application tests


  \todo Enable parallelism
  <ul>
   <li> The simplest strategy is to just subdivide the set of prime numbers
   considered into intervals, and then to compute the products independently.
   </li>
   <li> For directly jumping to some (big) prime number we have the application
   PrimeNumbers/RankPrimes.cpp; see
   Structures/NumberTheory/PrimeNumbers/plans/RankPrimes.hpp for further
   plans to speed it up. </li>
   <li> Of course, this can be done in a simple way by just enabling as
   additional parameter the start-prime (bigger than k), and then outputting
   only the second ("infinite") product, with full precision. </li>
   <li> For this we just need to write a simple application, which computes
   the command-line calls for the subordinated computation, and which reads
   the result-files and computes the final result. </li>
  </ul>


  \todo Alternative computation via logarithms
  <ul>
   <li> An alternative computations computes the logarithm of the product,
   and finally uses exponentiation. </li>
   <li> Then each factor becomes a summand, no multiplication or divisions
   have to be carried out, and the only costly operation is to compute
   log(p) and log(p-1). </li>
   <li> This only for the infinite product. </li>
   <li> One would expect a loss of precision, which however should be
   negligible, while the computation might become faster (it depends on how
   fast logarithms are computed). </li>
  </ul>


  \todo Handle several k-values : DONE
  <ul>
   <li> An input like
   \verbatim
> GrosswaldHagisFormula-O3-DNDEBUG 5,20 1000000000
   \endverbatim
   would be good, saying that we wish results for 5 <= k <= 20. </li>
   <li> To make parsing easy, no spaces are allowed for that pair. </li>
  </ul>


  \todo DONE (when using time, then the output needs to be copied to a file)
  Potential bug
  <ul>
   <li> For "> time GrosswaldHagisFormula-O3-DNDEBUG 3,20 1000000000"
   the additional data for C_11 was not printed:
   \verbatim
C_10 = 336.034327307194497
Finite and the infinite part: 2796.2965153468151887, 0.12017120697427801113
1 - first remaining factor: 3.5999999736000001242e-17
C_11 = 511.42228312047417728
C_12 = 1312.3197146277008806
Finite and the infinite part: 13882.452232084007257, 0.094530828753350440844
1 - first remaining factor: 5.499999967000000099e-17
   \endverbatim
   </li>
   <li> How could this be? Perhaps just something in the environment? </li>
   <li> Again:
   \verbatim
> time GrosswaldHagisFormula-O3-DNDEBUG 11,20 4000000000 | tee GH_1120_4
Precision in bits: 416
The first prime number not taken into account: 4000000007

C_11 = 511.42228230839231811
C_12 = 1312.3197120808120353
   \endverbatim
   Also with other max_p, and with other intervals: if 11 is included, then the
   statistical data is not printed, while everything else is printed?! </li>
   <li> But the same with k=31; or for the interval from 2-50 with 41 (while
   the others are printed)?! </li>
   <li> This disappears if "time" is not used, so it seems to be a bug of
   "time"! </li>
   <li> And when copying the output to a file, via "tee", then the output
   shows up there. So it seems definitely to be a "time"-bug (and we should
   always copy the output to files). </li>
  </ul>

*/
