// Oliver Kullmann, 29.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/Basics.hpp
  \brief Plans regarding the basics of boolean functions


  \todo Notions of "finite functions"
  <ul>
   <li> For a "finite function" f implicitly a domain D of values is given,
   with D = {0,1} for "boolean function", and furthermore we have an implicitly
   associated size-domain S <= NN_0, such that for every n in S f accepts
   lists of length n over D as inputs, and returns lists of length m_n (for
   some fixed m_n) over D as results. </li>
   <li> DONE (a finite function is just a function f, while n and m are not
   specified, but actually can be variable)
   A triple [n,m,f], where n is the number of inputs, m the number
   of outputs, and f the function, which accepts lists of length n,
   and returns lists of length m. </li>
   <li> Let's speak of a "n x m" finite function if we have n inputs and m
   outputs. </li>
   <li> DONE (0,1 is most common)
   The default is boolean values; shall this be "false, true", or
   "0, 1", or perhaps "-1, +1" ? </li>
   <li> DONE (n,m,D are only given implicitly)
   If a quadruple [n,m,f,D] is used, then for input and output
   the set of values is D. </li>
   <li> And if a quintuple [n,m,f,D,C] is used, then the values for the
   inputs are taken from D, while the values for the output are taken
   from C. </li>
   <li> Abbreviations: "bf" for "boolean function", "ff" for "finite
   function" (in general). </li>
   </li>
   <li> Input and output conventions:
    <ol>
     <li> To make it more useful, perhaps instead of using standardised input
     and output names through natural numbers, we should use variables?! </li>
     <li> Or should the renaming be done separately? </li>
     <li> As above, inputs and outputs are lists of values; using variables,
     inputs and outputs would be total assignments w.r.t. the variables used.
     </li>
     <li> Perhaps the basic functions are standardised boolean functions,
     operating on lists, while the various realisations of these functions
     use variables appropriately. </li>
    </ol>
   </li>
   <li> An especially important case, realised for example by signed
   clause-sets, is that of a "finite relation", a map f: D^n -> {0,1}. </li>
   <li> Definitely we have the need for "formal boolean functions", which are
   associated with variables; see "Basic operations for boolean functions" in
   Satisfiability/Lisp/FiniteFunctions/plans/BDDs.hpp. </li>
  </ul>


  \todo Representing boolean functions by their truth tables
  <ul>
   <li> The truth table is just a sequence of bits. So once the order
   is standardised (in such a way that adding variables doesn't change the
   order), truth tables can be represented by natural numbers (including
   zero), plus the information how many variables are there. </li>
   <li> Most natural seems here to use binary counting, with leading
   digit last (compare
   ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/general.hpp). </li>
   <li> Given such an encoding, the composition of boolean functions can
   be achieved by bitwise operations:
    <ol>
     <li> By package functs we have logor, lorand and logxor. </li>
     <li> We should also provide the other 13 basic functions (see
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/BasicFunctions.hpp).
     </li>
     <li> For negation one needs to know the number of bits. </li>
    </ol>
   </li>
   <li> See "Using different algorithms for different parameter values"
   in Satisfiability/Heuristics/plans/general.hpp for some application of
   such algorithms (in general SAT solving). </li>
  </ul>


  \todo DONE (we just use full clause-sets)
  Boolean functions represented by sets of satisfied/falsified inputs
  <ul>
   <li> An n-ary boolean function can be represented by a set of lists of
   length n, with elements 0,1. </li>
   <li> This as "dnf-representation" is the set of inputs mapped to 1, and as
   "cnf-representation" mapped to 0. </li>
   <li> How to call such representations? There is the notion of "minterms"
   and "maxterms", but I (OK) find this confusing. Let's just call it the
   "positive full representation" and the "negative full representation"
   ("full" as with "full clauses"). </li>
   <li> Abbreviations "pfrbf" and "nfrbf". </li>
   <li> So for example the pfrbf of the binary and is {[1,1]}, while the
   nfrbf is {[0,0],[0,1],[1,0]}. </li>
   <li> Shall we make a distinction between sets and lists? </li>
   <li> These sets are just re-coding of the full DNF and the full CNF
   representations; so do we need this type of representation at all? </li>
   <li> Perhaps it is convenient (and intuitive) to have it. </li>
   <li> However as DNF-clause-sets the above is {{1,2}}, while as
   CNF-clause-set we have {{-1,-2},{-1,2},{1,-2}} (using the positions as
   variables). </li>
   <li> So we better avoid this doubling of entities. </li>
  </ul>

*/
