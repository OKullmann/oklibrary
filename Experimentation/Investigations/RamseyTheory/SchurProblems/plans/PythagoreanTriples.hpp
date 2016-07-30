// Oliver Kullmann, 23.3.2016 (Swansea)
/* Copyright 2016 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/SchurProblems/plans/PythagoreanTriples.hpp
  \brief Plans regarding colourings of Pythagorean triples (and generalisations)

  The hypergraph of all Pythagorean triples, as set-system, is obtained via
  pythtripses_le_byDickson(n) (vertices are natural numbers 1,...,n); this
  is currently the fastest implementation.

  The corresponding Ramsey-numbers are pyth(r), r >= 1, the smallest n >= 1
  such that the pythgripses_le(n) is not r-colourable:
  pyth(1) = 5, pyth(2) = 7825, while it is not known whether pyth(r) for r >= 3
  is finite.

  All functions available are currently in
  ComputerAlgebra/NumberTheory/Lisp/Quadratics.mac .


  \todo Growth-rate of number of PT-triples
  <ul>
   <li> The growth-rate of the sequence n >= 0 -> count_pythtrip_le_byfact(n)
   should be determined in good detail; this is https://oeis.org/A224921 .
   </li>
   <li> The above method, via summing up the factorisation-formula, is quite
   effective, but are there more efficient ones? </li>
   <li> For example, on OK's laptop we get
     count_pythtrip_le_byfact(100000) = 161436
   in 20 sec, and
     count_pythtrip_le_byfact(1000000) = 1980642
   in 212 sec. </li>
   <li> There are rather precise asymptotic formulas; see
   Satisfiability/Transformers/Generators/Pythagorean.cpp. </li>
  </ul>


  \todo Analysing the power of removal of blocked clauses
  <ul>
   <li> For hypergraph k-colouring of a hypergraph G with hyperedges of sizes
   at least 2, via elimination of blocked clauses the following reduction is
   obtained, whose correctness is easy to prove directly: remove iteratively
   all vertices occurring at most k-1-times. </li>
   <li> For hypergraph, the result obtained is called the "k-core". </li>
   <li> The removal of vertices occurring at most k-1 times from set-systems is
   implemented via reduce_bounded_ses(S). </li>
   <li> 2-colouring:
    <ol>
     <li> reduce_bounded_ses(pythtripses_le_byDickson(64),1) = {} </li>
     <li> 65 -> {{15,20,25},{15,36,39},{20,48,52},{25,60,65},{36,48,60},
     {39,52,65}}

     6 elements. </li>
    </ol>
   </li>
   <li> 3-colouring:
    <ol>
     <li> 1104 -> {} </li>
     <li> 1105 -> {{120,160,200},{120,225,255},{120,288,312},{160,300,340},
     {160,384,416},{200,375,425},{200,480,520},{225,300,375},{225,540,585},
     {255,340,425},{255,612,663},{288,384,480},{288,540,612},{300,720,780},
     {312,416,520},{312,585,663},{340,816,884},{375,900,975},{384,720,816},
     {416,780,884},{425,1020,1105},{480,900,1020},{520,975,1105},
     {540,720,900},{585,780,975},{612,816,1020},{663,884,1105}}

     27 elements. </li>
    </ol>
   </li>
   <li> 4-colouring:
    <ol>
     <li> 19824 -> {} </li>
     <li> 19825 -> 3147 elements. </li>
    </ol>
   </li>
   <li> 5-colouring: (using the C++ program; via e.g.
   "./Pythagorean 128316 3 0 format=D 5 W sb=off - | less"
    <ol>
     <li> 128315 -> {} </li>
     <li> 128316 -> 22590 elements. </li>
    </ol>
   </li>
   <li> 6-colouring:
    <ol>
     <li> 637324 -> {} </li>
     <li> 637325 -> 73372 elements. </li>
    </ol>
   </li>
   <li> 7-colouring:
    <ol>
     <li> 2551019 -> {} </li>
     <li> 2551020 -> 272523 elements. </li>
    </ol>
   </li>
   <li> A conjecture much simpler than the full PT-Conjecture
   is that for all k there exists n with
   reduce_bounded_ses(pythtripses_le_byDickson(n),k) != {}.
   </li>
   <li> The sequences are 64,1104,19824,128315,637324 and
   65,1105,19825,128316,637325. </li>
  </ul>


  \todo Growth-rate after removal of blocked clauses
  <ul>
   <li> k=1:
    <ol>
     <li> n=7825: before 9472, after 7336, quotient 1.29.. </li>
     <li> n=10^5: before 161436, after 135608, quotient 1.19.. </li>
     <li> n=10^6: before 1980642, after 1735281, quotient 1.14. </li>
     <li> n=10^7: before 23471475, after 21134845, quotient 1.11. </li>
     <li> n=10^8: before 271360653, after 248990796, quotient 1.09. </li>
    </ol>
   </li>
   <li> It seems clear that the quotient is going down, asymptotically --
   does is go to 1 ? </li>
  </ul>

*/
