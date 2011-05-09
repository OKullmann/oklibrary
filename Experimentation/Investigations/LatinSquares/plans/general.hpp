// Oliver Kullmann, 28.12.2008 (Swansea)
/* Copyright 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LatinSquares/plans/general.hpp
  \brief On investigations regarding latin squares and variations


  \todo Strictly pandiagonal latin squares
  <ul>
   <li> See spdl_abn_ls(n,a,b) in
    ComputerAlgebra/CombinatorialMatrices/Lisp/LatinSquares/BasicNotions.mac.
    </li>
    <li> SAT-translations are easy to do (basically the same as latin squares
    themselves), and we should provide them. </li>
    <li> For n >= 4 divisible by 2 or 3, is it known that SPLS(n)=0 ? </li>
  </ul>


  \todo Counting the number of n-jective relations
  <ul>
   <li> Let mj_n(m) be the number of n-jective relations on a set with
   m elements ("mj" like "multijection"). </li>
   <li> See (currently) "Module ComputerAlgebra/Sets" in
   ComputerAlgebra/plans/general.hpp. </li>
   <li> So mj_0(m) = 2, mj_1(m) = m and mj_2(m) = m!, while mj_3(m) is
   the (total) number of latin squares (quasigroups) of order n. </li>
   <li> Conjecture: mj_k(3) = 3 * 2^(k-1) for k >= 1 (verified for k <= 5).
    <ol>
     <li> The formula works also for k=0 if ceil is used. </li>
     <li> Alternatively, one can use recursion a(n) = 2 * a(n-1) with initial
     values a(0) = 2 and a(1) = 3. </li>
     <li> This sequence is 2,3,6,12,24,48, ... </li>
     <li> Identical with A042950. </li>
    </ol>
   </li>
   <li> mj_k(4) : 2,4,24,576 (k=0,1,2,3)
    <ol>
     <li> eis_search yields ["A000336","A071302"]. </li>
     <li> A000336 is the function f: NN -> NN, f(n) = n for n <= 4,
     while f(n) = f(n-1) * f(n-2) * f(n-3) * f(n-4) for n >= 5. </li>
     <li> This yields 1, 2, 3, 4, 24, 576, 165888. </li>
     <li> A071302 is 1/2 * (number of square matrices M of order n over
     ZZ_3 such that M * M^t = Id). </li>
     <li> This yields 1, 4, 24, 576, 51840. </li>
     <li> The function allnjective_rel(M,n) won't be able to compute
     the next term allnjective_rel(setn(4),4), since the number of
     injections is roughly 109 * 10^12. </li>
     <li> But with a C++ implementation, which runs through the injections
     one by one, we can compute it. </li>
    </ol>
   </li>
  </ul>


  \todo Isomorphismen
  <ul>
   <li> We should investigate the isotopism classes of the set of
   n-jective relations, that is, for k-ary relations we allow
   k bijections (for each place one. </li>
   <li> We should also investigate parastrophisms, that is, the k!
   permutations of the places of the relations. </li>
   <li> And, of course, also ordinary isomorphisms, that is, having
   the k isotopism-bijections all equal. </li>
  </ul>

*/

