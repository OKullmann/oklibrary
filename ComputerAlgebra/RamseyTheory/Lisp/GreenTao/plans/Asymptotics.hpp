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


  \todo DONE (providing proven intervals containing the true value)
  Controlling the error of C_gh_hp(k,max_p,decimal_digits)
  <ul>
   <li> We assume that each rational number is correctly rounded to a bfloat,
   and that the result of a bfloat-product is correctly rounded. </li>
   <li> Furthermore we assume the "machine-error", eps, is 10^(-fpprec+1).
   </li>
   <li> It seems obvious that all multiplications stay within the normalised
   range (of bfloat). </li>
   <li> Then a rather generous upper bound on the absolute value of the
   relative error is, that with each multiplication to the already established
   relative error we add 4*eps. </li>
   <li> Thus, since for C_gh_inf_hp(k,max_p) we perform pi(max_p)-1
   multiplications, the absolute value of the relative error for
   C_gh_inf_hp(k,max_p) is < eps + (pi(max_p)-1)*4*eps. </li>
   <li> Altogether the relative error for C_gh_hp(k,max_p,decimal_digits) is
   then less than
     teps = 5*eps + (pi(max_p)-1)*4*eps = eps * (5 + 4*(pi(max_p)-1)).
   </li>
   <li> So C_gh_hp(k,max_p,decimal_digits) should return a pair [r,int],
   where r is the bfloat-result, while int = [r*(1-teps), r*(1+teps)]. </li>
   <li> We can use pi(max_p) here, since we have available the number of primes
   <= max_p. </li>
   <li> This pair-computation should be done by C_gh_inf_hp also. </li>
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
