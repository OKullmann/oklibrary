// Matthew Gwynne 20.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LogicalDataAnalysis/plans/general.hpp
  \brief General plans regarding investigations on logical data analysis  
  

  \todo Create milestones.


  \todo Computing the probability of a "contradictory input matrix"
  <ul>
   <li> Consider n>=0 (boolean) "input" variables, one "output" variables,
   and m>=0 choices of boolean vectors of length n+1 (with repetition). </li>
   <li> So there are (2^(n+1))^m possible outcomes. </li>
   <li> The event NC ("no contradiction") is given by the m-tuples
   (a_1, ..., a_m), where a_i, a_j coinciding on the first n bits implies they
   are also coinciding on the last. </li>
   <li> NC is the disjoint union of the events NC_i for 0 <= i <= m, where NC_i
   is the subset of NC consisting of outcomes (a_1,...,a_m) such that after
   removal of the final bit we have exactly i (different) vectors. </li>
   <li> So
   \verbatim
nccount_boolmat(n, m) := sum(nccountext_boolmat(n, m, i), i, 0, m)$
ncprob_boolmat(n,m) := nccount_boolmat(n, m) / (2^(n+1))^m$
   \endverbatim
   ("ext" for "exact"). </li>
   <li> |NC_i| = binom(2^n, i) * S_i * 2^i, where S_i is the number of
   surjections from {1,...,m} to {1,...,i}. </li>
   <li> We have S_i = stirling2(m,i) * i!. </li>
   <li> So
   \verbatim
nccountext_boolmat(n, m, i) := binomial(2^n,i) * stirling2(m,i)*i! * 2^i$

float(ncprob_boolmat(10,50));
  .5497968110387601
   \endverbatim
   </li>
   <li> Memoised versions to speed computations up (for repeated evaluations,
   while for a single computation it doesn't help):
   \verbatim
nccountm_boolmat[n, m] := sum(nccountextm_boolmat[n, m, i], i, 0, m)$
ncprobm_boolmat(n,m) := nccountm_boolmat[n, m] / (2^(n+1))^m$
nccountextm_boolmat[n, m, i] := binomial(2^n,i) * stirling2(m,i)*i! * 2^i$

float(ncprobm_boolmat(20,1000));
  .7880606667585897

plot_ncprob(n,m) := block([L : create_list(i,i,0,m)],
  plot2d([discrete, L, map(lambda([i],ncprobm_boolmat(n,i)), L)]))$

plot_ncprob(10,120);
   \endverbatim
   </li>
   <li> However for simplification one better doesn't use memoisation:
   \verbatim
nccount_boolmat(n,0);
  1
nccount_boolmat(n,1);
  2^(n+1)

nccount_boolmat(1,m), simpsum;
  'sum(binomial(2,i)*2^i*i!*stirling2(m,i),i,0,m)
   \endverbatim
   </li>
   <li> Since in each addend n is involved only in the binomial coefficient,
   likely a different organisation of the computation is possible, which might
   yield a more efficient computation. </li>
   <li> A recursive computation for nccountextm_boolmat(n,m,i) is as follows:
   \verbatim
nccountext_rec_boolmat[n,m,i] := 
 if m=0 then if i=0 then 1 else 0
 elseif i=0 then 0
 else i*nccountext_rec_boolmat[n,m-1,i] + 
      2*(2^n-(i-1)) * nccountext_rec_boolmat[n,m-1,i-1]$

nccount_rec_boolmat[n, m] := sum(nccountext_rec_boolmat[n, m, i], i, 0, m)$

ncprob_rec_boolmat(n,m) := nccount_rec_boolmat[n, m] / (2^(n+1))^m$

float(ncprob_rec_boolmat(10,50));
  .5497968110387601
   \endverbatim
   </li>
   <li> A different recursive formula is (valid for m, n >= 0):
   \verbatim
nccountm_boolmat_rec(n,m) := nccountm_boolmat_rec_r[n, m, 0]$
nccountm_boolmat_rec_r[n,m,c] := if m = 0 then 1 else 
    c* nccountm_boolmat_rec_r[n,m-1,c] + 
    2* (2^n - c) * nccountm_boolmat_rec_r[n,m-1,c+1]$

ncprobm_boolmat_rec(n,m) := nccountm_boolmat_rec(n,m) / (2^(n+1))^m$
   \endverbatim
   </li>
   <li> For all natural numbers m', n >= 0, for all 0 <= c <= m' and 
   for all matrices M' (over {0,1}) of size m'*(n+1), if we have that
   <ol>
    <li> no two rows in M' contradict (i.e., are identical except for the last
    element), and </li>
    <li> the number of different vectors in M' is c, </li>
   </ol>
   then for all m >= 0, we have that the number of matrices (over {0,1}) of 
   size (m'+m)*(n+1) where
   <ul>
    <li> no two rows contradict, and </li>
    <li> the first m' rows are the rows of M' </li>
   </ul>
   is nccountm_boolmat_rec_r(n,m,c).

OK: This doesn't define anything. The sentence has the structure
  If A, then B is nccountm_boolmat_rec_r(n,m,c).
Such a sentence only makes sense if A is some conjecture, the Riemann
conjecture for example.
However in this case A itself makes no sense; the fundamental problem is
a grammatical problem --- it is *completely* ambiguous, what the scope
of the various quantifiers is (M' is used in A as well as in B !)
Likely this ambiguity is inherent, since the whole definition-approach
is misguided.
If a combinatorial function like nccountm_boolmat_rec_r(n,m,c) is to be
defined, then first the domains of n,m,c are to be specified, then a set
C(n,m,c) depending only on n,m,c is to be defined, and finally
 nccountm_boolmat_rec_r(n,m,c) := |C(n,m,c)|.
Phrases like "for all" are not to be used in definitions, only in statements
("definitions" are not true of false, while statements are).

   </li>
   <li>
   To derive such a formula, we consider there are two possibilities for the 
   (m'+1)-th row, either
   <ol>
    <li> the (m'+1)-th row repeats one of the vectors in M', of which there 
    are c different vectors to repeat, or </li>
    <li> the (m'+1)-th row neither repeats nor contradicts any row in M', 
    of which there are c rows to repeat, and c rows to contradict with, and
    so (2^(n+1)-2*c) such valid vectors for the (m'+1)-th row in this case. 
    </li>
   </ol>
   Therefore, we have that 
   <ol>
    <li> in case 1, the number of matrices (over {0,1}) of size (m'+m)*(n+1)
    where 
    <ul>
     <li> no two rows contradict, </li>
     <li> the first m' rows are the rows of M', and </li>
     <li> the (m'+1)-th row is a repetition of a row in M' </li>
    </ul>
    is c * nccountm_boolmat_rec_r(n,m-1,c). That is, for every one of the
    c valid vectors V in the (m'+1)-th row, the number of matrices M (over 
    {0,1}) of size ((m'+1)+(m-1))*(n+1) where 
    <ul>
     <li> the first rows m'+1 rows of M are the rows of append(M',V), and 
     </li>
     <li> no two rows in M contradict </li>
    </ul>
    is nccountm_boolmat_rec_r(n,m-1,c), as V repeats a vector in M', and so 
    append(M',V) contains c different vectors.
    </li>  
    <li> in case 2, the number of matrices (over {0,1}) of size (m'+m)*(n+1)
    where 
    <ul>
     <li> no two rows contradict, </li>
     <li> the first m' rows are the rows of M', </li>
     <li> the (m'+1)-th row is not a repetition of a row in M', and </li>
     <li> the (m'+1)-th row does not contradict with any row in M' </li>
    </ul>
    is (2^(n+1) - 2*c) * nccountm_boolmat_rec_r(n,m-1,c+1). That is, for every
    such vector V in the (m'+1)-th row (of which there are (2^(n+1) - 2*c) 
    possibilities), the number of matrices M (over {0,1}) of size 
    ((m'+1)+(m-1))*(n+1) where 
    <ul>
     <li> the first rows m'+1 rows of M are the rows of append(M',V), and 
     </li>
     <li> no two rows in M contradict </li>
    </ul>
    is nccountm_boolmat_rec_r(n,m-1,c+1), as V does not repeat a vector in M', 
    and so append(M',V) contains c+1 different vectors.
    </li>
   </ol>
   Finally, we take the sum of the two cases to produce the total number
   of such matrices.
   </li>
   <li> It seems the direct formula is more efficient to compute. </li>
   <li> A nice approximative formula should be developed (perhaps based on
   the simple approximation formula regarding the birthday paradox).
    <ol>
     <li> Choosing p from q with repetition, the chance of no repetition
     is approximately exp(-p^2/(2q)) for p "small" w.r.t. q. </li>
     <li> In our situation one could approximately say that NC should have a
     probability similar to the event that no two equal vector of length n+1
     occur. </li>
     <li> This yields
     \verbatim
approx_ncprob_0(n,m) := float(exp(-m^2/2^(n+1)))$

approx_ncprob_0(10,50);
  .2950226561744428
approx_ncprob_0(20,1000);
  .6207436040675001

plot_approx_ncprob_0(n,m_max) := plot2d(approx_ncprob_0(n,m), [m,1,m_max])$
     \endverbatim
     How can we keep on old gnuplot-window (so that there we can compare the
     two plots in a simple way)? </li>
     <li> We can plot both side by side in the following way
     \verbatim
plot_approx_vs_exact_ncprob_0(n,m_max) := block(
  [exact_data : create_list([i,float(ncprobm_boolmat(n,i))],i,1,m_max)], 
  plot2d([approx_ncprob_0(n,m),[discrete,exact_data]], [m,1,m_max],
    [legend,"approx","exact"]))$
     \endverbatim
     </li>
     <li> We can plot the difference and the ratio of the approximation
     function to the real value like so
     \verbatim
diff_approx2exact_ncprob_0(n,m) := 
  float(ncprobm_boolmat(n,m)) - approx_ncprob_0(n,m)$
plot_diff_approx2exact_ncprob_0(n,m_max) := block(
  [diff_data : 
     create_list(
       [m,diff_approx2exact_ncprob_0(n,m)],m,1,m_max)], 
  plot2d([discrete,diff_data], [legend, "approx - exact"]))$

ratio_approx2exact_ncprob_0(n,m) := 
  float(ncprobm_boolmat(n,m)) / approx_ncprob_0(n,m)$
plot_ratio_approx2exact_ncprob_0(n,m_max) := block(
  [diff_data : 
     create_list(
       [m,ratio_approx2exact_ncprob_0(n,m)],m,1,m_max)], 
  plot2d([discrete,diff_data], [legend, "approx - exact"]))$
     \endverbatim
     </li>
     <li> Looking at plot_diff_approx2exact_ncprob_0(5,20) we seem to
     get some kind of gaussian distribution, suggesting that perhaps
     there is an additional exponential term which should 
     be added for the approximation. </li>
     <li> The following function, given n will find the first m (for all m > ms)
     such that f(m+1) is less than f(m) (i.e,. will find the maximum)
     \verbatim
find_maxima_2(f,n) := block([result : undef],
  for m : 1 while result = undef do if f(n,m) > f(n,m+1) then result : m,
  return(result))$
     \endverbatim
     which yields
     \verbatim
create_list(find_maxima_2(diff_approx2exact_ncprob_0,1,n),n,1,15);
[2,3,5,7,9,13,19,27,38,53,75,106,151,213,301]
     \endverbatim
     This can be used to gain some understanding of how the difference
     between the approximation and the exact value changes with n.
     </li>
    </ol>
   </li>
  </ul>

*/

