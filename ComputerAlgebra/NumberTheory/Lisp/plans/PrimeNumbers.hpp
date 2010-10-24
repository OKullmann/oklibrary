// Oliver Kullmann, 7.6.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
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
   <li> See ComputerAlgebra/Hypergraphs/Lisp/Generators/GreenTao.mac.
   </li>
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
     <li> DONE (likely corrected by Maxima community)
     The documentation for expintegral_li(x) says
     "expintegral_li (<n>,<z>)", however two arguments are not accepted?
     Ask the Maxima mailing list. </li>
     <li> So currently the higher logarithmic integrals Lih(x,m) can not
     be evaluated using Maxima (and thus we integrate starting with 2). </li>
     <li> It is actually questionable whether to start with 0 or 2: Apparently
     some call it "European" to start with "2", and "American" to start with
     0. </li>
     <li> And it's not so clear whether actually the indefinite integrals over
     the interval [0,2] exist for m>=2. </li>
     <li> Hans Werner Borchers <hwborchers@googlemail.com> writes:
     \verbatim
I had some time today to search, and after using Wolfram's Online
Integrator and the NIST Digital Library of Mathematical Functions, I
came up with the following solution,
    \int 1/log(x)^m = - Em(-log(x))/log(x)^{m-1} ,
where Em is a generalized exponential integral, that is available in
the 'gsl' package as
    expint_En(m, x)
And as 'En(.,x)' is available in R, so all the higher logarithmic
integrals can be computed as well.  Of course, you have to be careful
to avoid all their singularities.
     \endverbatim
     (see Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp). </li>
     <li> So the Maxima functions would be
     \verbatim
Lih_n(x,m) := -expintegral_e(m,-log(x))/log(x)^(m-1) - (-expintegral_e(m,-log(2))/log(2)^(m-1));
     \endverbatim
     also integrating from 2. </li>
     <li> This looks like a better computation. </li>
     <li> Hans Werner Borchers also mentions formulas like
     \verbatim
    \int 1/\log(t)^2 dt = -t/\log(t) + li(t)

    \int 1/\log(t)^3 dt = 1/2 * ( -t/\log(t)^2 - t/\log(t) + li(t) )
     \endverbatim
     which we need to check. </li>
    </ol>
   </li>
  </ul>


  \todo product_primes
  <ul>
   <li> What is known about this function? </li>
  </ul>

*/

