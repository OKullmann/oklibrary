// Oliver Kullmann, 13.9.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/CategoriesCls/plans/general.hpp
  \brief On investigations regarding categories of clause-sets


  \todo Links
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Categories/plans/general.hpp.
   </li>
  </ul>


  \todo Satisfiability of products
  <ul>
   <li> The first form of the conjecture (falsified below) is that for a 
   finite family of unsatisfiable flcls's, none of them containing the empty
   clause, their product is unsatisfiable as well. </li>
   <li> Considering the product of full clause-sets:
   \verbatim
prl(L) := product_flcls(map(fcls2flcls, map(full_fcs,L)))$
testl(L) := current_satsolver(flcls2fcls(prl(L)))$

testl([0,0]);
  false;
testl([0,1]);
  true;
testl([1,1]);
  false;
testl([1,2]);
  true;
testl([1,3]);
  true;
testl([1,4]);
  true;
testl([1,5]);
  true;
testl([1,6]);
  true;

prl([1,2]);
[
  [{[-1,-2],[-1,-1],[-1,1],[-1,2],[1,-2],[1,-1],[1,1],[1,2]},
   lambda([x],map('apply,[lambda([x],-x),lambda([x],-x)],map("[",x)))],
  {{[-1,-2],[-1,-1]},{[-1,-2],[-1,1]},{[-1,-1],[-1,2]},{[-1,1],[-1,2]},
  {[1,-2],[1,-1]},{[1,-2],[1,1]},{[1,-1],[1,2]},{[1,1],[1,2]}}
]
flcls2fcls(prl([1,2]));
  [{1,2,3,4},{{-4,-3},{-4,-2},{-3,-1},{-2,-1},{1,2},{1,3},{2,4},{3,4}}]
all_sat_fcs(flcls2fcls(prl([1,2])));
  {{-4,-1,2,3},{-3,-2,1,4}}
% Solutions interpreted:
[1,1],[1,-1],[-1,2],[-1,-2]
-2,-3,4,1
[-1,-1],[-1,1],[1,-2],[1,2]
2,3,-4,-1

length(all_sat_fcs(flcls2fcls(prl([1,2]))));
  2
length(all_sat_fcs(flcls2fcls(prl([1,3]))));
  18
length(all_sat_fcs(flcls2fcls(prl([1,4]))));
  110
length(all_sat_fcs(flcls2fcls(prl([1,5]))));
  570
length(all_sat_fcs(flcls2fcls(prl([1,6]))));
  2702
length(all_sat_fcs(flcls2fcls(prl([1,7]))));
  12138

map(factor,[2,18,110,570,2702]);
  [2, 2*3^2, 2*5*11, 2*3*5*19, 2*7*193]
eis_search(2,18,110,570,2702);
  ["A038721"]
eis_search_name(2,18,110,570,2702);
  A038721 - k=2 column of A038719.
eis_search(1,9,55,285,1351);
  ["A016269"]
eis_search_name(1,9,55,285,1351);
A016269 - Number of monotone Boolean functions of n variables with 2 mincuts. 
Also number of Sperner systems with 2 blocks.

eis_details(A016269);

A016269 - Number of monotone Boolean functions of n variables with 2 mincuts. 
Also number of Sperner systems with 2 blocks.

UNSIGNED TERMS
  1, 9, 55, 285, 1351, 6069, 26335, 111645, 465751, 1921029, 7859215, 
  31964205, 129442951,522538389, 2104469695, 8460859965, 33972448951, 
  136276954149, 546269553775, 2188563950925, 8764714059751

OFFSET
  0, 2

FORMULA
  G.f.: 1/((1-2x)(1-3x)(1-4x)). a(n) = (2^n)*(2^n - 1)/2 - 3^n + 2^n.
  a(n)=sum{0<=i,j,k,<=n, i+j+k=n, 2^i*3^j*4^k}. - Hieronymus Fischer 
   (Hieronymus.Fischer(AT)gmx.de), Jun 25 2007
  a(n)=2^(n+1)*(1+2^(n+2))-3^(n+2). - Hieronymus Fischer 
   (Hieronymus.Fischer(AT)gmx.de), Jun 25 2007
  a(n) = 3*StirlingS2(n+1,4) + StirlingS2(n+1,3). - Ross La Haye 
   (rlahaye(AT)new.rr.com), Jan 10 2008

MAPLE PROGRAM
  with(combinat):a:=n->stirling2(n,4)-stirling2(n-1,4): seq(a(n), n=4..24); 
  - Zerinvary Lajos (zerinvarylajos(AT)yahoo.com), Oct 05 2007

CROSS-REFERENCES
  Equals (1/2) A038721(n+1). First differences of A000453. 
  Partial sums of A027650.
  Pairwise sums of A099110.
  Odd part of A019333. Cf. A000392, A032263.
  Adjacent sequences: A016266 A016267 A016268 this_sequence A016270 A016271 
    A016272
  Sequence in context: A058852 A068970 A141530 this_sequence A005770 A030053 
    A072844

REFERENCES
  L. Comtet, Advanced Combinatorics, Reidel, 1974, p. 292, #8, s(n,2).

HYPERLINKS
  K. S. Brown, Dedekind's problem [http://www.mathpages.com/home/kmath030.htm]
  Vladeta Jovovic, Illustration for A016269, A047707, A051112-A051118
[http://www.research.att.com/~njas/sequences/a047707.pdf]
  Index entries for sequences related to Boolean functions
[http://www.research.att.com/~njas/sequences/Sindx_Bo.html#Boolean]
  Goran Kilibarda and Vladeta Jovovic, Antichains of Multisets
[http://www.cs.uwaterloo.ca/journals/JIS/index.html], J. Integer Seqs., 
  Vol. 7, 2004.

COMMENTS
  Half the number of 2 X (n+2) binary arrays with both a path of adjacent 1's 
  and a path of adjacent 0's from top row to bottom row. 
    - Ron Hardin (rhh(AT)cadence.com), Mar 21 2002
  As (0,0,1,9,55,...) this is the third binomial transform of cosh(x)-1. 
  It is the binomial transform of A000392, when this has two leading zeros. 
  Its e.g.f. is then exp(3x)cosh(x)-exp(3x) and a(n)=(4^n-2*3^n+2^n)/2. 
    - Paul Barry (pbarry(AT)wit.ie), May 13 2003
  Let P(A) be the power set of an n-element set A. Then a(n-2) = the number of
  pairs of elements {x,y} of P(A) for which either 
   0) x and y are disjoint and for which x is not a subset of y and y is
      not a subset of x, or 
   1) x and y are intersecting but for which x is not a subset of y and y is 
      not a subset of x. - Ross La Haye (rlahaye(AT)new.rr.com), Jan 10 2008

testl(2,2);
  false;
testl(2,3);
  false;
testl(3,3);
  false;

testl([2,2,2]);
  false;
testl([2,2,3]);
  false;

testl([2,2,2,2]);

   \endverbatim
   </li>
   <li> So perhaps unit-clauses are also problematic, not just
   empty clauses? </li>
   <li> For the special case of products of full clauses, the conjecture is
   that testl(L) yields true iff L contains a 0 and a non-0 or a 1 and a non-1.
   </li>
   <li> The new conjecture for the general case then could be: A product of
   literal-based formal clause-sets, where all clauses are of length at least
   2, is unsatisfiable iff at least one part is unsatisfiable. </li>
  </ul>

*/
