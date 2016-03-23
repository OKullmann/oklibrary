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
  is currently the fastest implementation).


  \todo Analysing the power of removal of blocked clauses
  <ul>
   <li> For hypergraph k-colouring of a hypergraph G with hyperedges of sizes
   at least 2, via elimination of blocked clauses the following reduction is
   obtained, whose correctness is easy to prove directly: remove iteratively
   all vertices occurring at most k-1-times. </li>
   <li> The removal of vertices occurring at most k times from set-systems is
   implemented via reduce_bounded_ses(S). </li>
   <li> 2-colouring:
    <ol>
     <li> reduce_bounded_ses(pythtripses_le_byDickson(64),1) = {} </li>
     <li> 65 -> {{15,20,25},{15,36,39},{20,48,52},{25,60,65},{36,48,60},
     {39,52,65}} </li>
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
     {540,720,900},{585,780,975},{612,816,1020},{663,884,1105}} </li>
    </ol>
   </li>
   <li> 4-colouring:
    <ol>
     <li> 19750 -> {} </li>
     <li> 20000 -> not {} </li>
    </ol>
   </li>
  </ul>

*/
