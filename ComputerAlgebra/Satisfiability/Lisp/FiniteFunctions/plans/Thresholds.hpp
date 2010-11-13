// Oliver Kullmann, 29.12.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/Thresholds.hpp
  \brief Plans regarding boolean threshold functions


  \todo General threshold functions
  <ul>
   <li> With [Knuth, Vol. 4, Fascicle 0, Section 7.1.1] we consider the
   threshold functions
   f(x_1, ..., x_n) = 1 iff w_1 * x_1 + ... + w_n * x_n >= t
   for integer constants w_i. </li>
   <li> As constraints, these conditions are also known as "pseudo-boolean
   constraints". </li>
   <li> Since we are interested for example in CNF- and DNF-representations,
   we also consider the negations, which (essentially) can be represented
   by allowing "<=" instead of ">=". How to call them? The above as "upper"
   threshold functions, while these as "lower" threshold functions? </li>
   <li> Naming: threshold_bf(W), lthreshold_bf(W). </li>
   <li> However, likely the CNF/DNF-representations should be handled in
   module PseudoBoolean; see
   Satisfiability/Lisp/PseudoBoolean/plans/general.hpp. </li>
  </ul>


  \todo Cardinality constraints
  <ul>
   <li> Important special cases of threshold functions are obtained when
   using w_i = 1 for all i. </li>
   <li> So the conditions are x_1 + ... + x_n >= t resp. <= t. </li>
   <li> Naming: cardinality_bf(n,t) resp. lcardinality_bf(n,t). </li>
   <li> For the special case t=1 we use alo_bf(n) resp. amo_bf(n). </li>
   <li> Clause-set realisations are considered in
   Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp. </li>
   <li> The condition x_1 + ... + x_n = t is expressed by ecardinality_bf(n,t).
   See "The basic symmetric functions" below. </li>
  </ul>


  \todo The basic symmetric functions
  <ul>
   <li> The functions ecardinality_bf(n,t) run under different names:
    <ol>
     <li> E_t in [Wegener 87]: "exactly-t-function". </li>
     <li> S_t(x_1,...,x_n) in [Knuth, Volume 4, Fascicle 0]: "basic
     symmetric functions". </li>
    </ol>
    It seems there is no generally accepted name for them. How do we
    call them? </li>
   <li> From these basic symmetric functions every symmetric boolean
   function can be computed.
    <ol>
     <li> [Knuth] uses S_T, where T is a subset of {0,1,...,n}, to denote
     the symmetric function which is true iff the number of ones of the
     input is an element of T. (Actually, [Knuth] uses a list T, which
     we better replace by a set.) </li>
     <li> By the S_T we uniquely represent all 2^n symmetric functions. </li>
    </ol>
   </li>
   <li> Section 3.4 in [Wegener] presents short circuits for the basic
   symmetric functions.
    <ol>
     <li> Only one output is considered, i.e., the type is [n,1]. </li>
    </ol>
   </li>
   <li> All n+1 basic symmetric functions together are represented by Shannon's
   "symmetric switching cicuit".
    <ol>
     <li> This boolean function is of type [n,n+1], where the output j is the
     ecardinality_bf(n,j). </li>
     <li> We need to find literature on that. </li>
     <li> The circuit is of size O(n^2); are there better ways? Apparently
     not. </li>
     <li> The easiest representation seems to be as a boolean branching
     program with n+1 output nodes (for the possible count-values), using a
     staircase-grid-structure. </li>
     </li>
    </ol>
   </li>
  </ul>

*/

