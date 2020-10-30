// Oliver Kullmann, 7.12.2015 (Swansea)
/* Copyright 2015, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/Tau.hpp
  \brief Plans regarding the tau-heuristics in the context of the TawSolver


  \todo A complete update is needed
  <ul>
   <li> The best implementation (and method) for computing the binary
        tau-function is in GenericSAT/BranchingTuples.hpp.
   <li> Possibly that makes the older considerations, directly related to
        computing the tau-function, useless. </li>
  </ul>


  \todo Test cases (benchmarks)
  <ul>
   <li> We have now stored the two "default" benchmarks/testcases. </li>
   <li> More are needed. </li>
   <li> Especially we need cases of success! </li>
  </ul>


  \todo Documenting tau-computation in version 2.6
  <ul>
   <li> That is essentially the same as Projections::Tau and
   Projections::Min_Tau in
   OKlib/Satisfiability/Solvers/TawSolver/Tau.hpp . </li>
   <li> These two components also need documentation. </li>
   <li> Newton method: https://en.wikipedia.org/wiki/Newton%27s_method </li>
   <li> Iteration: x_n+1 = x_n - f(x_n) / f'(x_n), for f(x) = 0. </li>
   <li> chi(a,b)(x) = x^-a + x^-b; x is sought with chi(a,b(x) = 1. </li>
   <li> The lower bound 4^(1/(a+b)) <= tau(a,b) is used. </li>
  </ul>


  \todo Implementing LnTau and Min_lntau
  <ul>
   <li> As Projections::Tau, but using ln(tau(a,b)). </li>
   <li> Hopefully this has computational advantages. </li>
   <li> Can the strict convexity (see Handbook) be exploited? </li>
   <li> lchi(a,b)(x) = exp(-a*x) + exp(-b*x). </li>
   <li> lchi'(a,b)(x) = -a*exp(-a*x) - b*exp(-b*x). </li>
   <li> So Newton-iteration is
     x_n+1 = x_n + (exp(-a*x)+exp(-b*x)-1) / (a*exp(-a*x)+b*exp(-b*x)). </li>
   <li> Lower bound is 1/(a+b) * ln(4) <= ln(tau(a,b)). </li>
   <li> Using std::exp and std::log. </li>
   <li> Or should one go for log_2(tau(a,b)) ? Then one could use std::exp2
   and std::log2. </li>
   <li> One could also try 1/ln(tau(a,b)). </li>
  </ul>


  \todo Benchmarks
  <ul>
   <li> We need a couple of benchmarks for the tawSolver, so that we can check
   the effect on run-times (hopefully improvements) and the search-statistics
   (hopefully basically no change). </li>
   <li> There are the SAT2014 benchmarks (in subdirectory data); they show
   that currently the tau-computation slows down the solver by an order of
   magnitude. </li>
  </ul>

*/

