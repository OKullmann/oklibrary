// Oliver Kullmann, 26.2.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Combinatorics/Lisp/Enumeration/docus/general.hpp
  \brief Central docus-file for enumeration at the Maxima/Lisp level


  <h1> Enumeration of basic combinatorial objects </h1>


  <h2> Generalities </h2>

  Load all enumeration-functionality (specifically) by
  \verbatim
oklib_load("OKlib/ComputerAlgebra/Combinatorics/Lisp/Enumeration/include.mac");
  \endverbatim

  The basic concepts one finds in ComputerAlgebra/Combinatorics/Lisp/Enumeration/Basics.mac.
  <ul>
   <li> The basic situation is that one has given a finite set M(p), where
   p is some parameter (perhaps a list). </li>
   <li> For example the set of k-element subsets of an n-element set. </li>
   <li> The basic task is <strong>counting</strong>, that is determining the
   size of M(p). </li>
   <li> Next comes to specify interesting <strong>linear orders</strong> of
   M(p), typically lexicographical and colexicographical order. </li>
   <li> The simplest form of computing such orders is by simply listing M(p)
   in the order. </li>
   <li> Finding the index in these lists of an element of M(p) is called
   <strong>ranking</strong>, while for an index (in these list) finding the
   corresponding object of M(p) is called <strong>unranking</strong>.
   <li> Indices start with 0 (as with list-indices). </li>
  </ul>


  <h2> Enumerating subsets </h2>

  The functionality is loaded (specifically) by
  \verbatim
(%i1) oklib_load("OKlib/ComputerAlgebra/Combinatorics/Lisp/Enumeration/Subsets.mac");
  \endverbatim
  See ComputerAlgebra/Combinatorics/Lisp/Enumeration/Subsets.mac for the
  definitions and specifications.


  <h3> k-subsets </h3>

  Counting of k-subsets (subsets of length k) happens, of course, by using
  binomial coefficients, but for completeness we also provide special
  functions:
  \verbatim
(%i2) count_ksubsets_s({1,3,5,6},2);
(%o2) 6
(%i3) count_ksubsets(4,2);
(%o3) 6
  \endverbatim


  <h4> Lexicographical ordering </h4>

  The list of all k-subsets of a set in lexicographical order:
  \verbatim
(%i4) lex_ksubsets_l({1,-1,2,-2},2);
(%o4) [{-2,-1},{-2,1},{-2,2},{-1,1},{-1,2},{1,2}]
(%i5) lex_ksubsets_ll([1,-1,2,-2],2);
(%o5) [{-1,1},{1,2},{-2,1},{-1,2},{-2,-1},{-2,2}]
  \endverbatim

  Ranking and unranking is available for standardised base-sets
  {1, ..., n}:
  \verbatim
(%i6) lex_ksubsets_l(setn(4),2);
(%o6) [{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}]
(%i7) rank_lex_ksubsets({1,4},4);
(%o7) 3
(%i8) unrank_lex_ksubsets(3,4,2);
(%o8) {1,4}
  \endverbatim


  <h4> Colexicographical ordering </h4>

  The list of all k-subsets of a set in colexicographical order:
  \verbatim
(%i9) colex_ksubsets_l({1,-1,2,-2},2);
(%o9) [{-2,-1},{-2,1},{-1,1},{-2,2},{-1,2},{1,2}]
(%i10) colex_ksubsets_ll([1,-1,2,-2],2);
(%o10) [{-1,1},{1,2},{-1,2},{-2,1},{-2,-1},{-2,2}]
  \endverbatim

  Ranking and unranking is available for standardised base-sets
  {1, ..., n}:
  \verbatim
(%i11) colex_ksubsets_l(setn(4),2);
(%o11) [{1,2},{1,3},{2,3},{1,4},{2,4},{3,4}]
(%i12) rank_colex_ksubsets({2,3});
(%o12) 3
(%i13) unrank_colex_ksubsets(3,4,2);
(%o13) {2,3}
  \endverbatim



*/
