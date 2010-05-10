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


  \todo Copy output to a file


  \todo Information on convergence rate
  <ul>
   <li> In [Grosswald, Hagis, 1979] it is stated that they determined how
   many digits are definitely correct --- what estimation did they use? </li>
   <li> Are the authors still around, so that we can ask them? </li>
   <li> And is the paper "[E. Grosswald, Arithmetic progressions of primes,
   to appear]" available? </li>
   <li> Optimally, the program would compute the number of definitely correct
   digits, and also then the most precise rounded value. </li>
   <li> First one needs to establish whether the factor in the infinite product
   are always smaller than 1, and whether they are strongly monotonically
   increasing. </li>
   <li> However, such considerations are best done at Maxima-level; see
   "Estimating the factors in the infinite Grosswald-Hagis-product" in
   ComputerAlgebra/RamseyTheory/Lisp/GreenTao/plans/Asymptotics.hpp. </li>
  </ul>


  \todo Write application tests


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
