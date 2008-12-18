// Oliver Kullmann, 21.11.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/general.hpp
  \brief Plans regarding boolean functions and generalisations


  \todo Connections to other modules
  <ul>
   <li> See
   Transitional/Satisfiability/FiniteFunctions/plans/general.hpp. </li>
   <li> Links within part ComputerAlgebra:
    <ol>
     <li> We have something in Cryptology (MG). </li>
    </ol>
   </li>
  </ul>


  \todo Basic concepts
  <ul>
   <li> The main point is the construction of representations of finite
   functions via generalised CNF/DNF, propositional logic, CSPs, alliances
   of active clause-sets etc. </li>
   <li> And also general computational representations need to be supported,
   together with conversions to other forms. </li>
   <li> Such "conversions" can involve themselves satisfiability problems in
   various forms. </li>
   <li> The notion of a "finite function":
    <ol>
     <li> A triple [n,m,f], where n is the number of inputs, m the number
     of outputs, and f the function, which accepts lists of length n,
     and returns lists of length m. </li>
     <li> Let's speak of a "n x m" finite function. </li>
     <li> The default is boolean values; shall this be "false, true", or
     "0, 1", or perhaps "-1, +1" ? </li>
     <li> If a quadruple [n,m,f,D] is used, then for input and output
     the set of values is D. </li>
     <li> And if a quintuple [n,m,f,D,C] is used, then the values for the
     inputs are taken from D, while the values for the output are taken
     from C. </li>
     <li> Abbreviations: "bf" for "boolean function", "ff" for "finite
     function" (in general). </li>
     <li> An important construction of a finite function is from a
     "gate list", which is a list L consistings of pairs [I,f], where
     f is an n x m ff, while I is a list of length n specifying the
     inputs of n, which are either pairs [i,j] for the j-th function in L
     with output number j, or are numbers i for the i-th input. </li>
     <li> For the output we need to specify a list with pairs [i,j]
     or i (as before). </li>
     <li> Perhaps, when the function has only one output, then a singleton
     list [i] can be used. </li>
    </ol>
   </li>
  </ul>


  \todo Boolean circuits
  <ul>
   <li> Of interest is finding boolean circuits (%w.r.t. different bases) for
   given boolean functions. </li>
   <li> See the work of Alexander S. Kulikov for example. </li>
   <li> Section 7.1.2 in [Knuth, The Art of Computer Programming: Introduction
   to Combinatorial Algorithms and Boolean Functions, 2008] contains a nice
   overview and concrete methods for computing optimal circuits (over the
   full base). </li>
  </ul>


  \todo Representations of fundamental functions:
  <ul>
   <li> We need the standard names for the 16 binary functions (which are
   then provided as constants).
    <ol>
     <li> Input are ordered by counting, that is, 00, 01, 10, 11. </li>
     <li> A function is then given by a 4-bit word, e.g., "or" is 0111. </li>
     <li> With Table 1 in Section 7.1.1 in [Knuth, The Art of Computer
     Programming: Introduction to Combinatorial Algorithms and Boolean
     Functions, 2008] we choose the following names. </li>
     <li> 0000: constant 0 (b2_0), 1111: constant 1 (b2_1) </li>
     <li> 0011: left projection (b2_lp), 0101: right projection (b2_rp) </li>
     <li> 1100: left complement (b2_lc), 1010: right complement (b2_rc) </li>
     <li> 0001: conjunction (b2_and), 0111: disjunction (b2_or) </li>
     <li> 1101: implication (b2_i), 0010: nonimplication (b2_ni) </li>
     <li> 1011: converse implication (b2_ci), 0100: converse nonimplication
     (b2_cni) </li>
     <li> 1110: nonconjunction (b2_nand), 1000: nondisjunction (b2_nor) </li>
     <li> 0110: exclusive disjunction (b2_xor), 1001: equivalence (b2_eq).
     </li>
    </ol>
   </li>
   <li> We also need all the functions in the context of adding,
   multiplication, subtraction and division. </li>
   <li> Cardinality constraints:
    <ol>
     <li> At least k out of n boolean variables are true. </li>
     <li> At most k out of n boolean variables are true. </li>
     <li> Exactly k out of n boolean variables are true. </li>
    </ol>
   </li>
  </ul>

*/

