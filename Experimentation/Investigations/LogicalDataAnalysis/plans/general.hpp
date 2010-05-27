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
   <li> However for simplification one better doesn't use it:
   \verbatim
nccount_boolmat(n,0);
  1
nccount_boolmat(n,1);
  2^(n+1)

nccount_boolmat(1,m), simpsum;
  'sum(binomial(2,i)*2^i*i!*stirling2(m,i),i,0,m)
   \endverbatim
   </li>
   <li> Since in each summand n is involved only in the binomial coefficient,
   likely a different organisation of the computation is possible, which might
   yield a more efficient computation. </li>
   <li> A recursive formula for the above is (valid for m, n >= 0):
   \verbatim
nccountm_boolmat_rec(n,m) := nccountm_boolmat_rec_r[n, m, 0]$
nccountm_boolmat_rec_r[n,m,c] := if m = 0 then 1 else 
    c* nccount_boolmat_rec_r[n,m-1,c] + 
    2* (2^n - c) * nccount_boolmat_rec_r[n,m-1,c+1]$

ncprobm_boolmat_rec(n,m) := nccountm_boolmat_rec(n,m) / (2^(n+1))^m$
   \endverbatim
   where nccountm_boolmat_rec_r[n,m,c] computes the number of 
   non-contradictory input matrices of size o+m which extend any given
   fixed o-length prefix with c unique vectors (for o > c).
OK: This doesn't define anything, since "o" is unspecified: If a variable
is not a parameter, then it must be a bound variable, that is, it must
be introduced by "there exists" or "for all".
So MG needs to make this statement precise, by using formal language.
Also do NOT use "unique" other in statements like "there exists a unique
x with the property P(x)" (above it apparently is applied to several
objects, and then "unique" can mean several things).
   </li>
   <li> Given any matrix M' (over {0,1}) of size m'*(n+1) for m',n >= 0
   for arbitrary  natural number m' > 0, such that no two rows
   in M conflict, and the number of different vectors in M is c, then the 
   number of matrices (over {0,1}) of size (m'+m)*(n+1) for m >= 0, where
   the first m' rows are the rows of M', is nccountm_boolmat_rec_r(n,m,c).
OK: The first row speaks of "M'", but then M' is not mentioned anymore in
this paragraph??
MG, please do NOT use "any" (since the usage of mathematical language
is too shaky, and "any" can mean "for all" and "there exists".
   </li>
   When calculating this, we have two possibilities for the m'+1th row, 
   either
   <ol>
    <li> it repeats one of the vectors in M', of which there are c unique 
    vectors to repeat and so there are c choices for the m+1th row. Then
    we must also make choices for the remaining vectors, and so in total, 
    there are c * nccountm_boolmat_rec_r(n,m-1,c) non-contradictory matrices
    with M' as the starting block and where the m'+1th row repeats a vector
    in M'. </li>
OK: Do NOT use "it" --- to what does "it" refer??
    <li> it doesn't repeat one of the vectors in M', and nor does it conflict
    with any vectors in M'. As there are c unique vectors to repeat or 
    conflict with and so there are 2^(n+1) - 2*c choices for the m+1th row. 
    Then we must also make choices for the remaining vectors, and so in total, 
    there are (2^(n+1) - 2*c) * nccountm_boolmat_rec_r(n,m-1,c+1) 
    non-contradictory matrices with M' as the starting block and where the 
    m'+1th row neither repeats or conflicts with any vector in M'. </li>
   </ol>
   </li>
   <li> It seems the direct formula is more efficient to compute. </li>
   <li> A nice approximative formula should be developed (perhaps based on
   the simple approximation formula regarding the birthday paradoxon).
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
    </ol>
   </li>
  </ul>

*/

