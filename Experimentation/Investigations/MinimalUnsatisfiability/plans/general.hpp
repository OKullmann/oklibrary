// Oliver Kullmann, 3.10.2010 (Swansea)
/* Copyright 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/MinimalUnsatisfiability/plans/general.hpp
  \brief Plans regarding minimal unsatisfiable clause-sets


  \todo Connections
  <ul>
   <li> W.r.t. the classification of unsatisfiable hitting clause-sets see
   Experimentation/Investigations/UHittingCatalogue/plans/general.hpp. </li>
   <li> For the Maxima functions see
   Satisfiability/Lisp/MinimalUnsatisfiability/MinVarDegrees.mac
   </li>
  </ul>


  \todo Hitting proof systems
  <ul>
   <li> See Satisfiability/Lisp/ConflictCombinatorics/HittingProofSystem.mac
   for first implementations. </li>
   <li> Likely we should introduce a new submodule; perhaps at base level
   (i.e., one level up). </li>
   <li> Regarding stcs2hitref:
    <ol>
     <li> Can we extend it, so that it works with extended splitting trees?
     </li>
     <li> Can we characterise when tree hitting clause-sets are being
     produced? </li>
     <li> And can we always find, for appropriate splitting tree, the best
     hitting refutation? </li>
    </ol>
   </li>
  </ul>


  \todo Understanding the recursion for non-Mersenne numbers
  <ul>
   <li> Understanding the recursion on potential degree-pairs:
    <ol>
     <li> In [Kullmann Zhao, 2012] the recursion on potential degree-pairs
     is introduced. </li>
     <li> As a special case we get nonmersenne(k) = nonmersenne_rec3[k].
     We should understand this better. </li>
     <li> For which k do we have only the "canonical pair" [e,i],
     i:index_nonmersenne(k), e:nonmersenne(k-i+1),
     in possible_degree_pairs_nm(k,nonmersenne(k)):
     \verbatim
analyse(k) := block([nm : nonmersenne(k), p],
 p : possible_degree_pairs_nm(k,nm),
 if length(p)=1 then print(k)
)$

for k : 1 thru 1000 do analyse(k);
2
3
5
6
12
13
27
28
58
59
121
122
248
249
503
504
     \endverbatim
     </li>
     <li> First case where we have more than one element: k=4.
     \verbatim
possible_degree_pairs_nm(4,6);
  [[[2,4],[3,7,1,6]],[[3,3],[2,7,2,7]]]
     \endverbatim
     </li>
     <li> It seems that we have only the canonical elements precisely for
     k = 2^m-m and k = 2^m-m+1, for m >= 2. </li>
    </ol>
   </li>
   <li> DONE (see [Kullmann Zhao, SAT 2011])
   How to prove that nonmersenne_rec[k] and
   nonmersenne_law(k) in
   Satisfiability/Lisp/MinimalUnsatisfiability/MinVarDegrees.mac
   are the same functions? </li>
   <li> DONE (it is index_nonmersenne_rec(k); (see [Kullmann Zhao, SAT 2011])
   Can we determine which i in the recursive formula
   nm(k) = max_{i in {2,...,k} min(2*i, nm(k-i+1)+i)
   are relevant?
    <ol>
     <li> Analysing the patterns:
     \verbatim
for k : 2 thru 70 do print(k, analyse_nonmersenne_rec(k));

2 [[2,1,"ab"]]
3 [[3,1,"b"]]
4 [[3,"a"],[4,1,"b"]]
5 [[4,2,"ab"]]
6 [[5,2,"b"]]
7 [[5,3,"ab"],[6,2,"b"]]
8 [[6,3,"b"],[7,2,"b"]]
9 [[6,4,"ab"],[7,3,"b"],[8,2,"b"]]
10 [[7,4,"b"],[8,3,"b"],[9,2,"b"]]
11 [[7,"a"],[8,4,"b"],[9,3,"b"],[10,2,"b"]]
12 [[8,5,"ab"]]
13 [[9,5,"b"]]
14 [[9,6,"ab"],[10,5,"b"]]
15 [[10,6,"b"],[11,5,"b"]]
16 [[10,7,"ab"],[11,6,"b"],[12,5,"b"]]
17 [[11,7,"b"],[12,6,"b"],[13,5,"b"]]
18 [[11,8,"ab"],[12,7,"b"],[13,6,"b"],[14,5,"b"]]
19 [[12,8,"b"],[13,7,"b"],[14,6,"b"],[15,5,"b"]]
20 [[12,9,"ab"],[13,8,"b"],[14,7,"b"],[15,6,"b"],[16,5,"b"]]
21 [[13,9,"b"],[14,8,"b"],[15,7,"b"],[16,6,"b"],[17,5,"b"]]
22 [[13,10,"ab"],[14,9,"b"],[15,8,"b"],[16,7,"b"],[17,6,"b"],[18,5,"b"]]
23 [[14,10,"b"],[15,9,"b"],[16,8,"b"],[17,7,"b"],[18,6,"b"],[19,5,"b"]]
24 [[14,11,"ab"],[15,10,"b"],[16,9,"b"],[17,8,"b"],[18,7,"b"],[19,6,"b"],[20,5,"b"]]
25 [[15,11,"b"],[16,10,"b"],[17,9,"b"],[18,8,"b"],[19,7,"b"],[20,6,"b"],[21,5,"b"]]
26 [[15,"a"],[16,11,"b"],[17,10,"b"],[18,9,"b"],[19,8,"b"],[20,7,"b"],[21,6,"b"],[22,5,"b"]]
27 [[16,12,"ab"]]
28 [[17,12,"b"]]
29 [[17,13,"ab"],[18,12,"b"]]
30 [[18,13,"b"],[19,12,"b"]]
31 [[18,14,"ab"],[19,13,"b"],[20,12,"b"]]
32 [[19,14,"b"],[20,13,"b"],[21,12,"b"]]
33 [[19,15,"ab"],[20,14,"b"],[21,13,"b"],[22,12,"b"]]
34 [[20,15,"b"],[21,14,"b"],[22,13,"b"],[23,12,"b"]]
35 [[20,16,"ab"],[21,15,"b"],[22,14,"b"],[23,13,"b"],[24,12,"b"]]
36 [[21,16,"b"],[22,15,"b"],[23,14,"b"],[24,13,"b"],[25,12,"b"]]
37 [[21,17,"ab"],[22,16,"b"],[23,15,"b"],[24,14,"b"],[25,13,"b"],[26,12,"b"]]
38 [[22,17,"b"],[23,16,"b"],[24,15,"b"],[25,14,"b"],[26,13,"b"],[27,12,"b"]]
39 [[22,18,"ab"],[23,17,"b"],[24,16,"b"],[25,15,"b"],[26,14,"b"],[27,13,"b"],[28,12,"b"]]
40 [[23,18,"b"],[24,17,"b"],[25,16,"b"],[26,15,"b"],[27,14,"b"],[28,13,"b"],[29,12,"b"]]
41 [[23,19,"ab"],[24,18,"b"],[25,17,"b"],[26,16,"b"],[27,15,"b"],[28,14,"b"],[29,13,"b"],[30,12,"b"]]
42 [[24,19,"b"],[25,18,"b"],[26,17,"b"],[27,16,"b"],[28,15,"b"],[29,14,"b"],[30,13,"b"],[31,12,"b"]]
43 [[24,20,"ab"],[25,19,"b"],[26,18,"b"],[27,17,"b"],[28,16,"b"],[29,15,"b"],[30,14,"b"],[31,13,"b"],
 [32,12,"b"]]
44 [[25,20,"b"],[26,19,"b"],[27,18,"b"],[28,17,"b"],[29,16,"b"],[30,15,"b"],[31,14,"b"],[32,13,"b"],
 [33,12,"b"]]
45 [[25,21,"ab"],[26,20,"b"],[27,19,"b"],[28,18,"b"],[29,17,"b"],[30,16,"b"],[31,15,"b"],[32,14,"b"],
 [33,13,"b"],[34,12,"b"]]
46 [[26,21,"b"],[27,20,"b"],[28,19,"b"],[29,18,"b"],[30,17,"b"],[31,16,"b"],[32,15,"b"],[33,14,"b"],
 [34,13,"b"],[35,12,"b"]]
47 [[26,22,"ab"],[27,21,"b"],[28,20,"b"],[29,19,"b"],[30,18,"b"],[31,17,"b"],[32,16,"b"],[33,15,"b"],
 [34,14,"b"],[35,13,"b"],[36,12,"b"]]
48 [[27,22,"b"],[28,21,"b"],[29,20,"b"],[30,19,"b"],[31,18,"b"],[32,17,"b"],[33,16,"b"],[34,15,"b"],
 [35,14,"b"],[36,13,"b"],[37,12,"b"]]
49 [[27,23,"ab"],[28,22,"b"],[29,21,"b"],[30,20,"b"],[31,19,"b"],[32,18,"b"],[33,17,"b"],[34,16,"b"],
 [35,15,"b"],[36,14,"b"],[37,13,"b"],[38,12,"b"]]
50 [[28,23,"b"],[29,22,"b"],[30,21,"b"],[31,20,"b"],[32,19,"b"],[33,18,"b"],[34,17,"b"],[35,16,"b"],
 [36,15,"b"],[37,14,"b"],[38,13,"b"],[39,12,"b"]]
51 [[28,24,"ab"],[29,23,"b"],[30,22,"b"],[31,21,"b"],[32,20,"b"],[33,19,"b"],[34,18,"b"],[35,17,"b"],
 [36,16,"b"],[37,15,"b"],[38,14,"b"],[39,13,"b"],[40,12,"b"]]
52 [[29,24,"b"],[30,23,"b"],[31,22,"b"],[32,21,"b"],[33,20,"b"],[34,19,"b"],[35,18,"b"],[36,17,"b"],
 [37,16,"b"],[38,15,"b"],[39,14,"b"],[40,13,"b"],[41,12,"b"]]
53 [[29,25,"ab"],[30,24,"b"],[31,23,"b"],[32,22,"b"],[33,21,"b"],[34,20,"b"],[35,19,"b"],[36,18,"b"],
 [37,17,"b"],[38,16,"b"],[39,15,"b"],[40,14,"b"],[41,13,"b"],[42,12,"b"]]
54 [[30,25,"b"],[31,24,"b"],[32,23,"b"],[33,22,"b"],[34,21,"b"],[35,20,"b"],[36,19,"b"],[37,18,"b"],
 [38,17,"b"],[39,16,"b"],[40,15,"b"],[41,14,"b"],[42,13,"b"],[43,12,"b"]]
55 [[30,26,"ab"],[31,25,"b"],[32,24,"b"],[33,23,"b"],[34,22,"b"],[35,21,"b"],[36,20,"b"],[37,19,"b"],
 [38,18,"b"],[39,17,"b"],[40,16,"b"],[41,15,"b"],[42,14,"b"],[43,13,"b"],[44,12,"b"]]
56 [[31,26,"b"],[32,25,"b"],[33,24,"b"],[34,23,"b"],[35,22,"b"],[36,21,"b"],[37,20,"b"],[38,19,"b"],
 [39,18,"b"],[40,17,"b"],[41,16,"b"],[42,15,"b"],[43,14,"b"],[44,13,"b"],[45,12,"b"]]
57 [[31,"a"],[32,26,"b"],[33,25,"b"],[34,24,"b"],[35,23,"b"],[36,22,"b"],[37,21,"b"],[38,20,"b"],
 [39,19,"b"],[40,18,"b"],[41,17,"b"],[42,16,"b"],[43,15,"b"],[44,14,"b"],[45,13,"b"],[46,12,"b"]]
58 [[32,27,"ab"]]
59 [[33,27,"b"]]
60 [[33,28,"ab"],[34,27,"b"]]
61 [[34,28,"b"],[35,27,"b"]]
62 [[34,29,"ab"],[35,28,"b"],[36,27,"b"]]
63 [[35,29,"b"],[36,28,"b"],[37,27,"b"]]
64 [[35,30,"ab"],[36,29,"b"],[37,28,"b"],[38,27,"b"]]
65 [[36,30,"b"],[37,29,"b"],[38,28,"b"],[39,27,"b"]]
66 [[36,31,"ab"],[37,30,"b"],[38,29,"b"],[39,28,"b"],[40,27,"b"]]
67 [[37,31,"b"],[38,30,"b"],[39,29,"b"],[40,28,"b"],[41,27,"b"]]
68 [[37,32,"ab"],[38,31,"b"],[39,30,"b"],[40,29,"b"],[41,28,"b"],[42,27,"b"]]
69 [[38,32,"b"],[39,31,"b"],[40,30,"b"],[41,29,"b"],[42,28,"b"],[43,27,"b"]]
70 [[38,33,"ab"],[39,32,"b"],[40,31,"b"],[41,30,"b"],[42,29,"b"],[43,28,"b"],[44,27,"b"]]

     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Understanding S_2(k)
  <ul>
   <li> Printing the values:
   \verbatim
for k : 2 thru 74 do block(
 [i : index_Sma_S2_rec(k)], printf(true, "~2d ~3d ~3d ~2d~%", k, Sma_S2_bydef(k), i, Sma_S2_bydef(i)+i-k-1))$
 2   4   1  0
 3   4   2  2
 4   6   2  1
 5   8   2  0
 6   8   3  0
 7   8   4  2
 8  10   4  1
 9  12   4  0
10  12   5  2
11  14   5  1
12  16   5  0
13  16   6  0
14  16   7  0
15  16   8  2
16  18   8  1
17  20   8  0
18  20   9  2
19  22   9  1
20  24   9  0
21  24  10  0
22  24  11  2
23  26  11  1
24  28  11  0
25  28  12  2
26  30  12  1
27  32  12  0
28  32  13  0
29  32  14  0
30  32  15  0
31  32  16  2
32  34  16  1
33  36  16  0
34  36  17  2
35  38  17  1
36  40  17  0
37  40  18  0
38  40  19  2
39  42  19  1
40  44  19  0
41  44  20  2
42  46  20  1
43  48  20  0
44  48  21  0
45  48  22  0
46  48  23  2
47  50  23  1
48  52  23  0
49  52  24  2
50  54  24  1
51  56  24  0
52  56  25  0
53  56  26  2
54  58  26  1
55  60  26  0
56  60  27  2
57  62  27  1
58  64  27  0
59  64  28  0
60  64  29  0
61  64  30  0
62  64  31  0
63  64  32  2
64  66  32  1
65  68  32  0
66  68  33  2
67  70  33  1
68  72  33  0
69  72  34  0
70  72  35  2
71  74  35  1
72  76  35  0
73  76  36  2
74  78  36  1
   \endverbatim
  </ul>


  \todo Maximal number of MUS's of a clause-set
  <ul>
   <li> Consider a clause-set F, and let nmus(F) be the number of minimally
   unsatisfiable clause-sets of F, that is,
   \verbatim
nmus(F) := length(all_min_usat_cores_bydef_cs(F))$
   \endverbatim
   </li>
   <li> Sperner's theorem yields nmus(F) <= binom(c(F),c(F)/2). Is this sharp?
   (Question asked by Joao Marques-Silva, 29.5.2012).
    <ol>
     <li> For simplicity consider only even m := c(F). </li>
     <li> What is needed is a clause-set F of size m, such that taking any
     subset of size m/2 we get a minimally unsatisfiable clause-set. </li>
     <li> Let's assume that the subsets F' are unsatisfiable hitting
     clause-set. </li>
      <ol>
       <li> Let's further assume that F' is k-uniform. </li>
       <li> Then m/2 * 2^(-k) = 1, that is, 2^k = m/2, i.e., k = log_2(m)-1.
       </li>
       <li> Consider thus integer b >= 0, and let m := 2^(b+1), and thus
       k = b. </li>
       <li> Thus we need an unsatisfiable clause-set of size 2^(b+1),
       where all clauses have size b, and where every subset of size 2^b is
       hitting. </li>
       <li> b <= 1: impossible. </li>
       <li> Doesn't seem to exist. </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>

*/
