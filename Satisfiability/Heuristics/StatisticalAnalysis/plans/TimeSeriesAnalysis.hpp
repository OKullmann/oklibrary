// Oliver Kullmann, 15.11.2005 (Swansea)
/* Copyright 2005 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Heuristics/StatisticalAnalysis/plans/TimeSeriesAnalysis.hpp
  \brief %Tools for the analysis of the progress made by a SAT solver

  A complete SAT solver can, with every new node processed, compute the number
  of total assignments realised to be unsatisfiable. So one can query, say,
  every second this total number (or the proportion of the whole search space
  covered) and, via a time series analysis, make a prediction how long it will
  take to solve the problem.


  \todo Counting falsifying assignments found
  <ul>
   <li> The computation of the current counts is as follows:
    <ol>
     <li> To every leaf of the search tree it is assigned its depth (the
     number of decision variables invoked on the path from the root to this
     node; the root has depth zero). </li>
     <li> If at depth d we find a contradiction, then we add 2^{-d} to the
     current fraction of falsifying assignments (once this
     number reaches 1, then the problem has been recognised as unsatisfiable;
     this fraction has type double). </li>
     <li> This corresponds to the consideration of the hitting clause-set
     associated to the r_k-compressed resolution tree. </li>
     <li> It gives a lower bound on the number of falsifying assignment. </li>
    </ol>
   </li>
   <li> More precisely we could add 2^{-|C|} for the clause C falsified at the
   leaves --- this would give (in the end) an upper bound on the number
   of falsifying assignments (an upper bound only because of the overlaps).
   </li>
   <li> So we have actually two time series to analyse (one "pessimistic", one
   "optimistic"). </li>
   <li> Counting the number of falsifying assignment for the
   clause-set given by the falsified clauses we can get a precise number.
   Is this interesting? </li>
  </ul>


  \todo Alternative sampling
  <ul>
   <li> Instead of measuring the fraction of the search space covered every
   second, it is more natural, easier and more precise to just have record the
   time when a new increase happens. </li>
   <li> So there is a class ProgressPrediction with member functions
   void insert(unsigned int depth)
   (for inserting the depth of the leaf where we found a contradiction) and
   double total_time() const
   for computing the expected total running time. </li>
   <li> Furthermore there should be a const member function for returning a
   const reference to the timing object. </li>
   <li> Now the easiest implementation just has a double mu in the
   ProgressPrediction object, initialised to 0, and with every insert(d) it
   adds 2^{-d} to mu. </li>
   <li> And total_time returns current_total_time / mu. </li>
   <li> While this model just compresses the whole time series into one
   average, there could be less compression, taking averages over intervals
   and then performing linear regression (possibly storing the whole sequence
   of measurements). </li>
   <li> One could use these measurements together with taking measurements
   every second by storing the results obtained every second, and analysing
   whether there is a trend in the data, just using linear regression. </li>
  </ul>


  \todo Using the predictions
  <ul>
   <li>SAT algorithms should use this prediction power for example
   as follows (but these algorithms should go into a different module). </li>
   <li> Optimising parameter values (like clause weights) by running the
   algorithm for some time, and then changing the parameter values,
   trying it again, etc. </li>
   <li> This form of optimisation should be possible also for a list of
   problems (to be "learned"), or for a class of problems given by some
   generator. </li>
   <li> Deciding about a restart. </li>
  </ul>

  \todo Software components
  <ul>
   <li> The Boost.Time_series library seems to be a good tool. </li>
  </ul>

*/

