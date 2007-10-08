// Oliver Kullmann, 16.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Heuristics/StatisticalAnalysis/plans/BacktrackingProbing.hpp
  \brief Submodule for tools supporting probing as proposed by Donald Knuth


  \todo Literature review : NEEDS UPDATE
  <ul>
   <li> What is the original article by Knuth? </li>
   <li> In Section 5.3 in [Foundations of %Algorithms
   Using C++ Pseudocode; Neapolitan, Naimipour 2004] one finds
   a simple form. </li>
   <li> See my SAT-handbook article on heuristics (this should
   yield the most powerful form). </li>
  </ul>


  \todo Write first outlines
  <ul>
   <li> According to my (OK) handbook article:
    <ol>
     <li> Given a tree T with probability distribution p
     on the outgoing edges of every node (direction towards
     the leaves), such that no edge has probability 0,
     consider the following finite probability space:
      <ul>
       <li> The outcomes are the leaves of the tree. </li>
       <li> The probability of an outcome is the product of the
       probabilities along the path from the root to the leaf. </li>
      </ul>
     </li>
     <li> Now consider as random variable X the probability itself (so
     X(w) = probability of event {w} for a leaf w). The expected value
     of X is E(X) = |lvs(T)|^{-1} (the construction works for every
     finite probability space, and yields the reciprocal value of the
     number of elements with non-zero probability). </li>
     <li> So estimating E(X) yields an estimation of the number of
     leaves of T. Such an estimation happens simply by probing. </li>
     <li> The less spread p has on the leaves, the less probing is necessary. </li>
    </ol>
   </li>
   <li> As explained in my handbook-article, the probabilities p are best induced
   by the tau-function from the distance function which governs the heuristics
   (since this is just the best what we have to predict the complexity). </li>
   <li> One needs to test how precise the estimation of tree sizes for SAT
   solving are (and how much it helps). </li>
   <li> Most attractive seems to use this method to evaluate the quality
   of distance functions: For normal-size branching trees we can completely
   calculate the distribution of X: A good distance functions
   yields a very concentrated distribution (around E(X)), while a bad
   distance functions is more similar to the uniform distribution.
   <li> This calculation is simple achieved as follows:
    <ol>
     <li> At each node with branching tuple t=(d_1, ..., d_m), compute the
     probabilities
      <center> p_i = tau(t)^(-d_i). </center>
     </li>
     <li> At each leaf dump the product of the p_i along the path from the root
     to that leaf to a file. </li>
     <li> At the end this file then represents the whole probability space:
     Exploring it in R (for example, simply viewing it) reveals how good
     this distance was adopted to this particular tree. </li>
    </ol>
   </li>
   <li> It should suffice to perform the calculations with 64-bit doubles. </li>
   <li> For evaluation purposes, one should also consider the canonical
   probability distribution on the tree (the "correct distribution", such
   that each leave gets the same probability). </li>
   <li> For detailed investigations visualisations of the tree are interesting;
   see GraphDrawing/plans/Trees.hpp. </li>
  </ul>

*/
