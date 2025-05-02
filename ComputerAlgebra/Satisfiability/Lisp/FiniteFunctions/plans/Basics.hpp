// Oliver Kullmann, 29.1.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
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
     <li> By package functs we have logor, logand and logxor; we have
     implemented these functions ourselves as and_ibo, or_ibo, xor_ibo. </li>
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


  \todo Algebraic normal form
  <ul>
   <li> The algebraic normal form of an n x 1 boolean function f is as a
   sum of monomials in variables x_1, ..., x_n over the field (boolean ring)
   ZZ_2. </li>
   <li> One can also say that these are exactly the representations by
   polynomials in variables x_1, ..., x_n, since multiplication is idempotent,
   and thus only mononials need to be considered (using only powers 0 and 1).
   </li>
   <li> Every boolean function has exactly one such algebraic normal form,
   for which we should use the abbreviation "anf". </li>
   <li> How to represent anf's:
    <ol>
     <li> One could use Maxima terms together with ordinary addition and
     multiplication. </li>
     <li> For evaluation values 0, 1 are substituted into the variables, and
     the sum is evaluated modulo 2. </li>
     <li> However then we have the problem that for example natural numbers
     are not variables anymore, and renaming needs to happen. </li>
     <li> So this shouldn't be the main form, but for the main form we just
     use hypergraphs, where the hyperedges represent the monomials. </li>
     <li> In this way we are dealing with the boolean algebra over some
     variable set V where:
      <ul>
       <li> the elements of the algebra are hypergraphs over the variable set.
       </li>
       <li> the empty hypergraph is the empty summation (0); </li>
       <li> the empty hyperedge is the empty product (1); </li>
       <li> multiplication (conjunction) is union; </li>
       <li> addition is symmetric difference. </li>
      </ul>
     </li>
     <li> Calling this "anf", while the sum of monomials in the Maxima-sense
     is called "manf". </li>
     <li> A function "anf2manf" performs the translation, where we need various
     forms, like anf2manf_hg, if the input is a hypergraph, anf2manf_ohg, if
     the input is an ordered hypergraph. </li>
     <li> For variables in the manf-form we use terms anf(v), where v is the
     original vertex. </li>
     <li> eval_manf(t, S) evaluates such a term, where S is the list of
     substitutions in appropriate form. </li>
    </ol>
   </li>
   <li> Direct computation of manf for n x 1 bf f:
    <ol>
     <li> The direct formula is as the sum over the vector v in f^-1(1) of
     products (x_1 + v_1 + 1) * ... * (x_n + v_n + 1), using variables x_i
     and arithmetic in ZZ_2. </li>
     <li> This is true since each such product is exactly 1 iff x = v. </li>
     <li> How can we evaluate these products more efficiently, taking
     idempotence of multiplication into account? </li>
     <li> This argument shows existence of anf's, and uniqueness follows since
     there are only 2^(2^n) different anf's, and each thus must represent a
     different boolean function (of which there are exactly 2^(2^n)). </li>
    </ol>
   </li>
   <li> Computing an ANF for a CNF via splitting:
    <ul>
     <li> To compute an ANF we can use the identity:
     \verbatim
     f(x_1,x_2,...,x_n) = f(0,x_2,...,x_n) + x_1(f(0,x_2,...,x_n) + f(1,x_2,...,x_n))
     \endverbatim
     where "+" is XOR and multiplication is conjunction. </li>
     <li> So we have the following Maxima function:
     \verbatim
fcl2anf(FF) :=
  if FF[2] = [] then {{}}
  elseif member({},FF[2]) then {}
  else block([x_eq_0,x_eq_1],
    x_eq_0 : fcl2anf([rest(FF[1]),apply_pa_cl({-FF[1][1]},FF[2])]),
    x_eq_1 : fcl2anf([rest(FF[1]),apply_pa_cl({FF[1][1]},FF[2])]),
    symmdifference(x_eq_0,
      map(lambda([E], cons(FF[1][1],E)), symmdifference(x_eq_0,x_eq_1))))$

# Simple examples:
fcl2anf([[],[{}]]);
  0
fcl2anf([[],[]]);
  1
fcl2anf([[1,2],[{-1},{2}]]);
  anf(1)*anf(2)+anf(2)
     \endverbatim
     </li>
    </ul>
   </li>
   <li> Alternative computation:
    <ol>
     <li> The proof of uniqueness of anf for f: One has to show that an
     anf is zero iff its the empty hypergraph. Assume we have hyperedges.
     Choose a minimal hyperedge E (w.r.t. set-inclusion). Setting the
     variables in E to 1 and the other variables to 0, this hyperedge evaluates
     to 1, while the other hyperedges evaluate to 0. Thus E is a hyperedge
     in the anf of f. </li>
     <li> So we get the following algorithm: For f compute its DNF hypergraph
     representation (for example by fulldnf2dnfhg_ofcs2ohg(FF)), collect the
     minimal hyperedge into the result-anf, subtract these hyperedges
     from f, in the algebraic interpretation over ZZ_2, and repeat with the
     new f. </li>
     <li> The algorithmic problem is the subtraction: How to do this
     efficiently, without having to use the whole f ? </li>
    </ol>
   </li>
   <li> Perhaps these functions go into a new file, AlgebraicNormalForm.mac.
   There will be further functions, like extracting the degree (the maximal
   size of a hyperedge). </li>
   <li> The standard translation of a formula p in ANF into CNF has:
    <ul>
     <li> the variables of p; </li>
     <li> a new variable y_i for each product X_i in p; </li>
     <li> the primes clauses for y_i <-> X_i for each product X_i in p; </li>
     <li> clauses representing the sum (XOR) of all y_i (this could be prime
     clauses, or a splitting of the sum using new variables). </li>
    </ul>
   We should implement this translation, and provide references.
   </li>
   <li> Investigating algebraic normal forms and their translations
   is discussed in "Algebraic normal form" in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp.
   </li>
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
