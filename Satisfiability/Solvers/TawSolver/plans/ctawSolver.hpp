// Oliver Kullmann, 4.3.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/ctawSolver.hpp
  \brief Plans regarding c|m|w|p tawSolver


  \todo Support for large (unsigned) integral types
  <ul>
   <li> Compare Satisfiability/Quantification/MAUT/Counting.hpp . </li>
   <li> Boost seems here the best source for us. </li>
   <li> It seems that we can use the fixed-sze integer- (and floating-point)
        types. </li>
   <li> Signed integral types are needed for general literal-weights
        (i.e., with possible negative values). </li>
   <li> The output should report on the used type for counting. </li>
   <li> We also support float64 and float80 for counting. </li>
   <li> For arbitrary precision we use GMP (see below). </li>
  </ul>


  \todo ptawSolver ("projection")
  <ul>
   <li> Certain variables are marked as part of the projection, and the number
        of remaining such variables is kept up-to-date on a path in the
        splitting tree; if m are left, and we found a satisfying partial
        assignment, then 2^m solutions are to be counted. </li>
   <li> Following the "Model Counting Competition 2021" (but only partially)
        the DIMACS format is extended by considering lines e,g,
          c p show 1 3 0
        which (here) says that variables 1 and 3 are to be added to the
        projection set. </li>
   <li> Such lines can occur everywhere where clauses can occur, and their
        union is considered. </li>
   <li> The example in the document shows a c-p-show line with a final 0;
        perhaps we are liberal here: if there is a final 0, then no line-break
        is required, otherwise the line ends exactly at the line-break. </li>
   <li> Since the solver is already specialised, we do not require any special
        form of type-information in the file (we just ignore comments). </li>
   <li> A problem here is with UnitClausePropagation: we need to tell it
        optionally to maintain the original n-value (needed for counting),
        and not to discard c-p-show lines, but to collect them into one line
        after the p-cnf line. </li>
  </ul>


  \todo wtawSolver ("weighted")
  <ul>
   <li> For each literal x a weight w(x) in RR is used. </li>
   <li> So using float64, float80, or arbitrary precision, by using
        integer arithmetic via GMP (treating say 0.123 and (123, 3),
        with 3 the negated exponent to base 10).
        Another possibility should be to use the Boost fixed-precision types.
   </li>
   <li> When a satisfying partial assignment phi with unset variable-set V
        is found, then
          prod_{x \in phi^{-1}(1)} w(x)  *  prod_{v in V} (w(v) + w(-v))
        is added.
   </li>
   <li> If w(v) + w(-v) = 1, then this factor can be discarded. </li>
   <li> We allow w(x) < 0. </li>
   <li> If w(x) = 0, then every branch with x->1 can be aborted.
        We can treat this case as a forced assignment x -> 0.
   </li>
   <li> The standard case is w(x) = w(-x) = 1. </li>
   <li> If in the input only for one literal w(x) is specified, then
        w(-x) := 1 - w(x) (and here the sum is explicitly set to 1, to avoid
        rounding-errors). </li>
   <li> If for some variable neither w(v) nor w(-v) is specified, then
         w(v) := w(-v) := 1. </li>
   <li> We could combine w with p to "wp" (discarding a variable can not be
        handled -- we needed to have w(v) + w(-v) = 1, but then one of the
        literals contributes wrongly when being part of a partial satisfying
        assignment).
        But one can use w(v) = w(-v) = 0.5, and then the total count obtained
        has to be multiplied with 2^m for m the number of these variables.
        So indeed projection can be simulated.
   </li>
   <li> In the output we report e.g.
          "approx float64, approx float80, precise",
        and whether there are case which don't add to 1 (other than w(x)=w(-x)
        =1), and whether there are negative w(x).
        Best likely to report the number of such cases. </li>
   <li> For arbitrary precision we should also allow fractions; normally
        using mpz, but when fractions are present, likely mpq (rational
        arithmetic) is best. </li>
   <li> For the product used in the evaluation of a satisfying partial
        assignment, likely best in a preprocessing step to bundle equal
        sums, so that one can use powers for larger equal parts of the
        product. </li>
  </ul>


  \todo Improved distance
  <ul>
   <li> The distance d_0 "sum of weigthed new clauses", which aims at ucp, is
        to be augmented by a distance d_1 which gets the bigger the more
        clauses are satisfied and the longer the remaining clauses are
        ("chances of satisfiability"). </li>
   <li> For example on inputs in 2-CNF the first distance is very weak. </li>
   <li> More generally, d_0 assumes that autarkies can be reduced, which is
        not the case for #SAT. </li>
  </ul>

*/
