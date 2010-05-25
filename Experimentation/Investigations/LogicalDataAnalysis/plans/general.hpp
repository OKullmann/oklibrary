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
   <li> Consider n (boolean) "input" variables, one "output" variables,
   and m choices of boolean vectors of length n+1 (with repetition). </li>
   <li> So there are (2^(n+1))^m possible outcomes. </li>
   <li> The event NC ("no contradiction") is given by the m-tuples
   (a_1, ..., a_m), where a_i, a_j coinciding on the first n bits implies they
   are also coinciding on the last. </li>
   <li> NC is the disjoint union of the events NC_i for 1 <= i <= m, where NC_i
   is the subset of NC consisting of outcomes (a_1,...,a_m) such that after
   removal of the final bit we have exactly i (different) vectors. </li>
   <li> |NC_i| = binom(2^n, i) * S_i * 2^i, where S_i is the number of
   surjections from {1,...,m} to {1,...,i}. </li>
   <li> We have S_i = stirling2(m,i) * i!. </li>
   <li> So the exact number of non-contradictory input matrices and their
   probability is given by
   \verbatim
nccount_boolmat(n, m) := sum(
 binomial(2^n,i) * stirling2(m,i)*i! * 2^i,
 i, 1, m)$
ncprob_boolmat(n,m) := nccount_boolmat(n, m) / (2^(n+1))^m$

stirling2m[n,m] := stirling2(n,m)$
nccountm_boolmat[n, m] := sum(
 binomial(2^n,i) * stirling2m[m,i]*i! * 2^i,
 i, 1, m)$
ncprobm_boolmat(n,m) := nccountm_boolmat[n, m] / (2^(n+1))^m$

float(ncprobm_boolmat(10,50));
  .5497968110387601
float(ncprobm_boolmat(20,1000));
  .7880606667585897

plot_ncprob(n,m) := block([L : create_list(i,i,1,m)],
  plot2d([discrete, L, map(lambda([i],ncprobm_boolmat(n,i)), L)]))$

plot_ncprob(10,120);
   \endverbatim
   Memoisation is used here to help a bit with speed (the computation is
   quite slow). </li>
   <li> However for simplification one better doesn't use it:
   \verbatim
nccount_boolmat(n,1);
  2^(n+1)

nccount_boolmat(1,m), simpsum;
  'sum(binomial(2,i)*2^i*i!*stirling2(m,i),i,1,m)
   \endverbatim
   </li>
   <li> Since in each summand n is involved only in the binomial coefficient,
   likely a different organisation of the computation is possible, which might
   yield a more efficient computation. </li>
   <li> MG has a recursive formula? </li>
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

