// Oliver Kullmann, 2.10.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Groupoids/plans/Transformations.hpp
  \brief On investigations regarding transformation semigroups


  \todo Hypergraph transformations I
  <ul>
   <li> Consider a natural number n >= 0, N = {1,...,n}, the powerset P =
   powerset(P), and the set MS_n =powerset(P) of all set-systems on N. </li>
   <li> Let c_0: MS -> MS be direct complementation: c_0(S) = P - S. </li>
   <li> Let c_1: MS -> MS be elementwise complementation: c_1(S): c_1(S) =
   { N - T : T in S }. </li>
   <li> Let s: MS -> MS be the closure under subsumption: s(S) = union_{T in S}
   powerset(T). </li>
   <li> What is the closure of {c_0, c_1, s} under composition? </li>
   <li>
   \verbatim
oklib_include("OKlib/ComputerAlgebra/Algebra/Lisp/Groupoids/Closures.mac")$
oklib_include("OKlib/ComputerAlgebra/Algebra/Lisp/Groupoids/Semigroups/TransformationMonoids.mac")$
oklib_include("OKlib/ComputerAlgebra/Hypergraphs/Lisp/SetSystems.mac")$
oklib_include("OKlib/ComputerAlgebra/DataStructures/Lisp/HashMaps.mac")$

det_cl(n) := block([N : setn(n), P, MS, c0, c0l, c1,c1l, s,sl, h, MSA, C],
 P : powerset(N),
 MS : powerset(P),
 c0 : lambda([S], setdifference(P,S)),
 c1 : lambda([S], ecomp(S,N)),
 s : lambda([S], subset_closure(S)),
 h : osm2hm(ll2osm(listify(MS),create_list(i,i,1,2^(2^n)))),
 c0l : map(lambda([S],ev_hm(h,c0(S))), listify(MS)),
 c1l : map(lambda([S],ev_hm(h,c1(S))), listify(MS)),
 sl : map(lambda([S],ev_hm(h,s(S))), listify(MS)),
 C : closure_bydef_grd(trf_l_compo, {c0l,c1l,sl}),
 length(C)
)$
create_list(det_cl(n),n,0,3);
  [2,28,28,28]
   \endverbatim
   </li>
   <li> So it seems as soon as the base set contains at least one element,
   the closure contains exactly 28 elements. One now needs to determine
   them. </li>
   <li> The above transformations c0,c1,s in list-form are
   \verbatim
[3,4,1,2]
[1,4,3,2]
[1,2,3,3]
   \endverbatim
   </li>
   <li> In Gap:
   \verbatim
LoadPackage("monoid");
c0a := Transformation([3,4,1,2]);
c1a := Transformation([1,4,3,2]);
sa := Transformation([1,2,3,3]);
Ca := Monoid(c0a,c1a,sa);
GeneratorsOfMonoid(Ca);
  [Transformation([3,4,1,2]),Transformation([1,4,3,2]),Transformation([1,2,3,3])]
Size(Ca);
  28
Centre(Ca);
  [ Transformation( [ 1, 2, 3, 4 ] ) ]
Idempotents(Ca);
  [Transformation([1,1,3,1]),Transformation([1,1,3,3]),Transformation([1,1,3,4]),
  Transformation([1,2,3,1]),Transformation([1,2,3,3]),Transformation([1,2,3,4]),
  Transformation([1,3,3,1]),Transformation([1,3,3,3]),Transformation([1,3,3,4])]
   \endverbatim
   </li>
   <li> Finding a presentation:
    <ol>
     <li> First by using Gap directly:
     \verbatim
phi_Ca := IsomorphismFpMonoid(Ca);
FCa := Range(phi_Ca);
ma := GeneratorsOfMonoid(FCa);
sa = PreImageElm(phi_Ca, ma[1])[1];
c1a = PreImageElm(phi_Ca, ma[2])[1];
c0a = PreImageElm(phi_Ca, ma[3])[1];

# So m1 = sa, m2 = c1a, m3 = c0a.

RelationsOfFpMonoid(FCa);
[ [ m1^2, m1 ],
  [ m2^2, <identity ...> ],
  [ m3*m2, m2*m3 ],
  [ m3^2, <identity ...> ],
  [ m1*m2*m1*m2, m1*m2*m1 ],
  [ m1*m2*m3*m1, m1*m2*m3 ],
  [ m1*m2*m3^2, m1*m2 ],
  [ m1*m3*m1*m2, m1*m3*m1 ],
  [ m2*m1*m2*m1, m1*m2*m1 ],
  [ m2*m1*m2^2, m2*m1 ],
  [ m2*m1*m3^2, m2*m1 ],
  [ m3*m1*m2^2, m3*m1 ],
  [ m3*m1*m3*m1, m1*m3*m1*m3 ],
  [ m3*m1*m3^2, m3*m1 ],

  [ m1*m2*m1*m3*m1, m1*m2*m1*m3 ],
  [ m2*m3*m1*m2*m1, m3*m1*m2*m1 ] ]
     \endverbatim
     Note the consistent use of "monoid" here; it is also possible to use
     "semigroup" (possibly yielding a different representation), but then
     also a "semigroup" needs to be constructed. </li>
     <li> These relations just translated:
     \verbatim
s^2 = s
c1^2 = id
c1 c0 = c0 c1
c0^2 = id
c1 s c1 s = s c1 s
s c0 c1 s = c0 c1 s
  superfluous
c1 s c0 s = s c0 s
s c1 s c1 = s c1 s
  superfluous
  superfluous
  superfluous
s c0 s c0 = c0 s c0 s
s c0 s c1 s = c0 s c1 s
s c1 s c0 c1 = s c1 s c0
  superfluous
     \endverbatim
     <li> Now "bottom-up", trying to guess a complete set of relations. </li>
     <li> Using three generators m[1] = c0, m[2] = c1, m[3] = s together with
     the four basic relations, expressing that c0,c1 are involutions which
     commute and that s is idempotent:
     \verbatim
f3 := FreeMonoid(3);
g := GeneratorsOfMonoid(f3);
r1 := [g[1]^2, g[1]^0];
r2 := [g[2]^2, g[2]^0];
r3 := [g[1]*g[2], g[2]*g[1]];
r4 := [g[3]*g[3], g[3]];
C0 := f3 / [r1,r2,r3,r4];
Size(C0);
  ??
     \endverbatim
     </li>
     <li> Using the transversal-operator Tr = c0 s c1 and
     closure under superset-formation s' = c1 s c1, adding
     the relation Tr^2 = s':
     \verbatim
r5 := [(g[2]*g[3]*g[1])^2, g[2]*g[3]*g[2]];
C1 := f3 / [r1,r2,r3,r4,r5];
Size(C1);
  ??
     \endverbatim
     (Note that Gap composes transformations from left to right.)
     </li>
     <li> Adding s' s = s s':
     \verbatim
r6 := [ g[3]*g[2]*g[3]*g[2], g[2]*g[3]*g[2]*g[3] ];
C2 := f3 / [r1,r2,r3,r4,r5,r6];
Size(C2);
  44
     \endverbatim
     </li>
     <li> Adding c1 s c0 s = s c0 s:
     \verbatim
r7 := [ g[3]*g[1]*g[3]*g[2], g[3]*g[1]*g[3] ];
C3 := f3 / [r1,r2,r3,r4,r5,r6,r7];
Size(C3);
  32
     \endverbatim
     </li>
     <li> Adding s c0 s c0 = c0 s c0 s:
     \verbatim
r8 := [ g[1]*g[3]*g[1]*g[3], g[3]*g[1]*g[3]*g[1] ];
C4 := f3 / [r1,r2,r3,r4,r5,r6,r7,r8];
Size(C4);
  28
     \endverbatim
     </li>
     <li> So we got a representation with 8 equations; checking independence:
     \verbatim
Size(f3 / [r1,r2,r3,r4,r5,r6,r8]);
  36
Size(f3 / [r1,r2,r3,r4,r5,r7,r8]);
  32
Size(f3 / [r1,r2,r3,r4,r6,r7,r8]);
  ??
Size(f3 / [r1,r2,r3,r5,r6,r7,r8]);
  28
Size(f3 / [r1,r2,r5,r6,r7,r8]);
  ??
Size(f3 / [r1,r3,r5,r6,r7,r8]);
  ??
Size(f3 / [r2,r3,r5,r6,r7,r8]);
  ??
     \endverbatim
     So r4 is derivable. </li>
     <li> The conjecture is that [r1,r2,r3,r5,r6,r7,r8] is a minimal system
     of equations (relations) representing C. </li>
     <li> Trying the relations computed by Gap above:
     \verbatim
C16 := f3 / [ [ g[1]^2, g[1] ], 
  [ g[2]^2, g[2]^0 ], 
  [ g[3]*g[2], g[2]*g[3] ], 
  [ g[3]^2, g[2]^0 ], 
  [ g[1]*g[2]*g[1]*g[2], g[1]*g[2]*g[1] ], 
  [ g[1]*g[2]*g[3]*g[1], g[1]*g[2]*g[3] ],
  [ g[1]*g[2]*g[3]^2, g[1]*g[2] ], 
  [ g[1]*g[3]*g[1]*g[2], g[1]*g[3]*g[1] ], 
  [ g[2]*g[1]*g[2]*g[1], g[1]*g[2]*g[1] ], 
  [ g[2]*g[1]*g[2]^2, g[2]*g[1] ], 
  [ g[2]*g[1]*g[3]^2, g[2]*g[1] ],
  [ g[3]*g[1]*g[2]^2, g[3]*g[1] ], 
  [ g[3]*g[1]*g[3]*g[1], g[1]*g[3]*g[1]*g[3] ], 
  [ g[3]*g[1]*g[3]^2, g[3]*g[1] ], 
  [ g[1]*g[2]*g[1]*g[3]*g[1], g[1]*g[2]*g[1]*g[3] ],
  [ g[2]*g[3]*g[1]*g[2]*g[1], g[3]*g[1]*g[2]*g[1] ] ];
Size(C16);
  28
     \endverbatim
     </li>
     <li> The last four relations can be left out without changing the
     size, but then removing the (new) last one increases the size to 32.
     </li>
    </ol>
   </li>
   <li> To understand the monoid Ca, Prof. Mitchell proposes to use the
   function "DisplayEggBoxesOfSemigroup", which displays the eggbox diagram
   of the D-classes (strong orbits of the semigroup on itself by left and
   right multiplication) of the semigroup C. </li>
  </ul>


  \todo Hypergraph transformations Ib
  <ul>
   <li> Now instead of closure under subset-formation we consider elimination
   of subsumed subsets. </li>
   <li>
   \verbatim
det1b_cl(n) := block([N : setn(n), P, MS, c0, c0l, c1,c1l, s,sl, h, MSA, C],
 P : powerset(N),
 MS : powerset(P),
 c0 : lambda([S], setdifference(P,S)),
 c1 : lambda([S], ecomp(S,N)),
 m : lambda([S], min_elements(S)),
 h : osm2hm(ll2osm(listify(MS),create_list(i,i,1,2^(2^n)))),
 c0l : map(lambda([S],ev_hm(h,c0(S))), listify(MS)),
 c1l : map(lambda([S],ev_hm(h,c1(S))), listify(MS)),
 ml : map(lambda([S],ev_hm(h,m(S))), listify(MS)),
 C : closure_bydef_grd(trf_l_compo, {c0l,c1l,ml}),
 length(C)
)$

heap_size : 3/2*10^9;
:lisp (ext:set-limit 'ext:heap-size $heap_size)
oklib_monitor : true;

for i : 0 thru 4 do print(i, det1b_cl(i));
Current size of closure is  2
0 2
Current size of closure is  3
Current size of closure is  9
Current size of closure is  23
Current size of closure is  28
1 28
Current size of closure is  3
Current size of closure is  9
Current size of closure is  25
Current size of closure is  68
Current size of closure is  74
2 74
Current size of closure is  3
Current size of closure is  9
Current size of closure is  25
Current size of closure is  68
Current size of closure is  76
3 76
Current size of closure is  3
Current size of closure is  9
Current size of closure is  25
Current size of closure is  68
Current size of closure is  76
4 76
   \endverbatim
   (where 28=2^2*7, 74=2*37, 76=2^2*19).
   </li>
   <li> So here the conjecture is that as soon as the base-set contains at
   least three elements, then the closure contains exactly 76 elements,
   and the monoids for different such base-sets are canonically isomorphic.
   </li>
   <li> See below for a direct Gap-presentation of these 3 transformations for
   n=3 (named c0,c1,m):
   \verbatim
Cb := Monoid(c0,c1,m);
Size(Cb);
  76
   \endverbatim
   </li>
   <li> Finding a presentation:
    <ol>
     <li> First by using Gap directly:
     \verbatim
phi_Cb := IsomorphismFpMonoid(Cb);
FCb := Range(phi_Cb);
mb := GeneratorsOfMonoid(FCb);
m = PreImageElm(phi_Cb, mb[1])[1];
c1 = PreImageElm(phi_Cb, mb[2])[1];
c0 = PreImageElm(phi_Cb, mb[3])[1];

# So mb1 = m, mb2 = c1, mb3 = c0.

RelationsOfFpMonoid(FCb);
[ [ m1^2, m1 ], 
  [ m2^2, <identity ...> ], 
  [ m3*m2, m2*m3 ], 
  [ m3^2, <identity ...> ], 
  [ m1*m2*m1, m1*m2 ], 
  [ m1*m2*m3^2, m1*m2 ],
  [ m2*m1*m2^2, m2*m1 ], 
  [ m2*m1*m3^2, m2*m1 ], 
  [ m3*m1*m2^2, m3*m1 ], 
  [ m3*m1*m3^2, m3*m1 ], 
  [ m1*m3*m1*m3*m1, m3*m1*m3*m1 ],
  [ m1*m2*m3*m1*m2*m3*m1, m1*m3*m1*m2*m3*m1 ], 
  [ m2*m1*m3*m1*m2*m3*m1, m1*m3*m1*m2*m3*m1 ],
  [ m3*m1*m3*m1*m2*m3*m1, m1*m3*m1*m2*m3*m1 ] ]
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Hypergraph transformations II
  <ul>
   <li> Additionally, let m: MS -> MS be subsumption elimination: m(S) =
   {T in S : there is no T' in S with T < T'}. </li>
   <li>
   \verbatim
det2_cl(n) := block([N : setn(n), P, MS, c0, c0l, c1,c1l, s,sl, m,ml, h, MSA, C],
 P : powerset(N),
 MS : powerset(P),
 c0 : lambda([S], setdifference(P,S)),
 c1 : lambda([S], ecomp(S,N)),
 s : lambda([S], subset_closure(S)),
 m : lambda([S], min_elements(S)),
 h : osm2hm(ll2osm(listify(MS),create_list(i,i,1,2^(2^n)))),
 c0l : map(lambda([S],ev_hm(h,c0(S))), listify(MS)),
 c1l : map(lambda([S],ev_hm(h,c1(S))), listify(MS)),
 sl : map(lambda([S],ev_hm(h,s(S))), listify(MS)),
 ml : map(lambda([S],ev_hm(h,m(S))), listify(MS)),
 if oklib_monitor and oklib_monitor_level >= 1 then (
  print("c0l = ",c0l),
  print("c1l = ",c1l),
  print("sl = ", sl),
  print("ml = ", ml)
 ),
 C : closure_bydef_grd(trf_l_compo, {c0l,c1l,sl,ml}),
 length(C)
)$

heap_size : 5*10^9;
:lisp (ext:set-limit 'ext:heap-size $heap_size)
oklib_monitor : true;

for i : 0 thru 4 do print(i, det2_cl(i));

Current size of closure is  2
0 2

Current size of closure is  4
Current size of closure is  16
Current size of closure is  80
Current size of closure is  176
1 176

Current size of closure is  4
Current size of closure is  16
Current size of closure is  83
Current size of closure is  350
Current size of closure is  404
2 404

Current size of closure is  4
Current size of closure is  16
Current size of closure is  83
Current size of closure is  421
Current size of closure is  656
3 656

Current size of closure is  4
Current size of closure is  16
Current size of closure is  83
Current size of closure is  434
Current size of closure is  966
Current size of closure is  972
XXX
4 XXX
   \endverbatim
   (where 176=2^4*11, 404=2^2*101, 656=2^4*41). </li>
   <li> In Gap:
    <ol>
     <li> n=1:
     \verbatim
c0 := Transformation([3,4,1,2]);
c1 := Transformation([1,4,3,2]);
s := Transformation([1,2,3,3]);
m := Transformation([1,2,2,4]);
M1 := Monoid(c0,c1,s,m);
Size(M1);
  176
     \endverbatim
     </li>
     <li> n=2:
     \verbatim
c0 := Transformation([5,12,15,16,1,14,13,10,11,8,9,2,7,6,3,4]);
c1 := Transformation([1,14,15,8,5,12,7,4,11,16,9,6,13,2,3,10]);
s := Transformation([1,2,3,5,5,6,5,5,9,3,5,5,6,5,5,9]);
m := Transformation([1,2,2,2,2,2,2,2,2,10,10,13,13,14,16,16]);
M2 := Monoid(c0,c1,s,m);
Size(M2);
  404;
     \endverbatim
     </li>
     <li> n=3:
     \verbatim
c0 := Transformation([9,136,199,230,245,252,255,256,1,254,253,250,251,248,249,242,247,246,243,244,237,240,241,226,239,
 238,235,236,233,234,227,232,231,228,229,214,221,224,225,194,223,222,219,220,217,218,211,216,215,
 212,213,206,209,210,195,208,207,204,205,202,203,196,201,200,197,198,167,182,189,192,193,130,191,
 190,187,188,185,186,179,184,183,180,181,174,177,178,163,176,175,172,173,170,171,164,169,168,165,
 166,151,158,161,162,131,160,159,156,157,154,155,148,153,152,149,150,143,146,147,132,145,144,141,
 142,139,140,133,138,137,134,135,72,103,118,125,128,129,2,127,126,123,124,121,122,115,120,119,116,
 117,110,113,114,99,112,111,108,109,106,107,100,105,104,101,102,87,94,97,98,67,96,95,92,93,90,91,
 84,89,88,85,86,79,82,83,68,81,80,77,78,75,76,69,74,73,70,71,40,55,62,65,66,3,64,63,60,61,58,59,52,
 57,56,53,54,47,50,51,36,49,48,45,46,43,44,37,42,41,38,39,24,31,34,35,4,33,32,29,30,27,28,21,26,25,
 22,23,16,19,20,5,18,17,14,15,12,13,6,11,10,7,8]);
c1 := Transformation([1,226,239,240,113,110,103,40,9,72,47,16,79,106,43,12,75,50,19,82,237,230,167,136,199,174,143,206,
 233,170,139,202,177,146,209,112,109,102,39,8,71,46,15,78,105,42,11,74,49,18,81,236,229,166,135,
 198,173,142,205,232,169,138,201,176,145,208,241,114,111,104,41,10,73,48,17,80,107,44,13,76,51,20,
 83,238,231,168,137,200,175,144,207,234,171,140,203,178,147,210,99,108,101,38,7,70,45,14,77,100,37,
 6,69,36,5,68,235,228,165,134,197,172,141,204,227,164,133,196,163,132,195,254,255,128,125,118,55,
 24,87,62,31,94,121,58,27,90,65,34,97,252,245,182,151,214,189,158,221,248,185,154,217,192,161,224,
 127,124,117,54,23,86,61,30,93,120,57,26,89,64,33,96,251,244,181,150,213,188,157,220,247,184,153,
 216,191,160,223,256,129,126,119,56,25,88,63,32,95,122,59,28,91,66,35,98,253,246,183,152,215,190,
 159,222,249,186,155,218,193,162,225,2,123,116,53,22,85,60,29,92,115,52,21,84,3,4,67,250,243,180,
 149,212,187,156,219,242,179,148,211,130,131,194]);
s := Transformation([1,2,3,29,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,25,25,24,24,25,24,24,25,29,31,31,32,31,31,32,9,9,9,9,9,9,
 9,9,9,9,9,9,9,9,9,9,59,56,55,55,56,55,55,59,60,62,62,63,62,62,66,29,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
 9,25,25,24,24,25,24,24,25,29,31,31,32,31,31,32,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,59,56,55,55,56,55,
 55,59,123,125,125,126,125,125,129,3,29,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,25,25,24,24,25,24,24,25,29,
 31,31,32,31,31,32,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,59,56,55,55,56,55,55,59,60,62,62,63,62,62,66,29,
 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,25,25,24,24,25,24,24,25,29,31,31,32,31,31,32,9,9,9,9,9,9,9,9,9,9,
 9,9,9,9,9,9,59,56,55,55,56,55,55,59,123,125,125,126,125,125,129]);
m := Transformation([1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,130,130,130,130,187,187,190,190,191,
 193,193,187,187,190,190,191,193,193,130,187,187,190,190,191,193,193,187,187,190,190,191,193,193,
 130,130,187,187,190,190,191,193,193,187,187,190,190,191,193,193,130,187,187,190,190,191,193,193,
 187,187,190,190,191,193,193,194,194,211,243,243,253,253,216,225,225,250,250,253,253,223,225,225,
 211,243,243,253,253,216,225,225,250,250,253,253,223,225,225,226,242,243,243,253,253,247,256,256,
 250,250,253,253,254,256,256,242,243,243,253,253,247,256,256,250,250,253,253,254,256,256]);
M3 := Monoid(c0,c1,s,m);
Size(M3);
  656;
     \endverbatim
     </li>
     <li> n=4: Now we need to compute the four transformations directly
     in Gap (instead of copy-and-paste them from Maxima, as above). Of course,
     we have now 2^16=65536 elements in the base-set, but this should be
     possible. </li>
    </ol>
   </li>
  </ul>

*/

