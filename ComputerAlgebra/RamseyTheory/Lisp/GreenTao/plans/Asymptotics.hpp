// Oliver Kullmann, 10.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/GreenTao/plans/Asymptotics.hpp
  \brief Plans regarding asymptotic estimations


  \todo Connections
  <ul>
   <li> See
   Structures/NumberTheory/PrimeNumbers/plans/GrosswaldHagisFormula.hpp. </li>
  </ul>


  \todo Estimating the factors in the infinite Grosswald-Hagis-product
  <ul>
   <li> An estimation for factor_C_gh_inf(k) is needed, so that for a given
   max_p it can be estimated, how large possibly the contribution of the
   remaining factors (with p > max_p) could be. </li>
  </ul>


  \todo Improving the Grosswald-Hagis estimation
  <ul>
   <li> Using the higher logarithmic integrals Lih(x,m) should result in
   more precise computation --- it should be possible to replace the log's
   in the denominator of the formula by such higher logarithmic integrals.
   </li>
   <li> A first guess is that the factor n^2/log(n)^(k-2)) can be replaced by
   n * Lih(n,k-2). </li>
   <li> This is implemented by
   \verbatim
ghLih_nhyp_arithprog_primes_hg(k,n,decimal_digits) :=
 block([fpprec:decimal_digits],
  if k-1 > length(C_gh_values) then unknown
  elseif k=3 then bfloat(C_gh_values[k-1])/2/(k-1)*n*Li_hp(n,decimal_digits)
  else bfloat(C_gh_values[k-1])/2/(k-1) * n * Lih_hp(n,k-2,decimal_digits))$
   \endverbatim
   </li>
   <li> A little experiment:
    <ol>
     <li> nhyp_arithprog_primes_hg(3,1000) = 40510. </li>
     <li> gh_nhyp_arithprog_primes_hg(3,1000) = 47784.1056309392. </li>
     <li> float(ghLih_nhyp_arithprog_primes_hg(3,1000,30)) = 58625.55716538247.
     </li>
     <li> Doesn't look encouraging. </li>
     <li> The values from the logarithmic integral are bigger than
     x/log(x)^(k-2), and since the approximation using log's is already too
     big, it only gets worse. </li>
     <li> nhyp_arithprog_primes_hg(5,1000) = 912. </li>
     <li> gh_nhyp_arithprog_primes_hg(5,1000) = 1574.23961989466. </li>
     <li> float(ghLih_nhyp_arithprog_primes_hg(5,1000,30)) = 4641.782914341983.
     </li>
     <li> So the first guess doesn't look correct. </li>
    </ol>
   </li>
   <li> Perhaps things would be different if considering the original
   formula, which uses parameter x instead of n, where x stands for all
   primes <= x (while n stands for the number of primes). </li>
   <li> Using then
   \verbatim
ghorigLih_nhyp_arithprog_primes_hg(k,n,decimal_digits) :=
 block([fpprec:decimal_digits],
  if k-1 > length(C_gh_values) then unknown
  else block([p : unrank_primes(n)],
    realpart(bfloat(C_gh_values[k-1])/2/(k-1)*p*Lih_hp(p,k,decimal_digits))))$
   \endverbatim
   </li>
   <li> The little experiment repeated:
    <ol>
     <li> ghorig_nhyp_arithprog_primes_hg(3,1000) = 28613.07161089571. </li>
     <li> float(ghorigLih_nhyp_arithprog_primes_hg(3,1000,30)) =
     56190.661401401. </li>
     <li> ghorig_nhyp_arithprog_primes_hg(5,1000) = 558.1633788252357. </li>
     <li> float(ghorigLih_nhyp_arithprog_primes_hg(5,1000,30)) =
     13317.17349502113. </li>
     <li> Doesn't look right, at least not for the small n we are considering.
     </li>
    </ol>
   </li>
  </ul>

*/
