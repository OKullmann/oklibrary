// Oliver Kullmann, 21.11.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/general.hpp
  \brief Plans regarding boolean functions and generalisations


  \todo Create milestones


  \todo Connections to other modules
  <ul>
   <li> See
   OKlib/Satisfiability/FiniteFunctions/plans/general.hpp. </li>
   <li> Links within part ComputerAlgebra:
    <ol>
     <li> We have something in Cryptology (MG). </li>
    </ol>
   </li>
  </ul>


  \todo General plans
  <ul>
   <li> An important motivation is the task of, given a finite function,
   find a representation of it by generalised CNF/DNF, propositional logic,
   CSPs, alliances of active clause-sets etc. </li>
   <li> So from this point of view, finite functions are building blocks
   in representations of (generalised) satisfiability problem instances,
   and the task is to transform the whole representation into an alliance of
   active clause-sets. </li>
   <li> But finite functions are also interesting in their own right.
   Constructing smallest circuits or smallest branching programs for
   example are interesting tasks (involving themselves satisfiability
   problems). </li>
   <li> So various forms of general computational representations need to be
   supported, together with conversions to other forms. </li>
   <li> Such "conversions" might involve satisfiability problems in
   various forms. </li>
   <li> However, automata are not considered here, since we are studying
   here "combinatorial" (non-uniform) computations. </li>
   <li> Perhaps we have a module ComputerAlgebra/Automata, where various
   forms of automata are provided, and there then, for a given input size,
   various forms of representations of this finite function (e.g., as a
   circuit), are also provided. See "Module ComputerAlgebra/Automata" in
   ComputerAlgebra/plans/general.hpp. </li>
  </ul>


  \todo Quadratic functions
  <ul>
   <li> A boolean function is called "quadratic" if it is equivalent to
   a 2-DNF, and "dualy quadratic" if equivalent to a 2-CNF. </li>
   <li> We need to implement the recognition of quadratic and dual quadratic
   functions. </li>
   <li> We should have a dedicated file "Quadratic.mac". </li>
  </li>


  \todo Horn functions
  <ul>
   <li> A boolean function is called "Horn" if it is equivalent to a Horn CNF.
   </li>
   <li> We need to implement the recognition of Horn functions. </li>
   <li> We should have a dedicated file "Horn.mac". </li>
  </ul>


  \todo Finite functions with full application of partial assignments
  <ul>
   <li> Compare to "Smurfs" in
   Satisfiability/Lisp/FiniteFunctions/plans/BDDs.hpp. </li>
   <li> Consider a set D of values. </li>
   <li> A "finite function with full application", for short "fffa", is either
   an element of D (for a constant function, without variables), or some object
   f, where the operations are
    <ol>
     <li> var_fffa(f) for the set of variables of f on which f really depends,
     </li>
     <li> apply_pa_fffa(phi,f) for the application of a partial assignment phi
     to f, yielding again a fffa. </li>
    </ol>
   </li>
   <li> f is a constant iff var(f) = {} iff f is semantically a constant. </li>
   <li> Variables v in phi with v notin var(f) have no influence on phi * f.
   </li>
   <li> We have (psi * phi) * f = psi * (phi * f), where psi * phi is the
   composition of partial assignments. </li>
   <li> For hitting CNF and hitting DNF we can implement models. </li>
   <li> Even more efficient for permutations of {0,1}^n as boolean conditions
   {0,1}^2n -> {0,1}. </li>
  </ul>


  \todo And-inverter graphs (AIGs)
  <ul>
   <li> AIG's represent boolean functions; they are directed graphs with two
   types of nodes, terminal vertices, with outdegree zero and labelled by
   variables, and and-nodes, with out-degree one and arbitrary in-degree. </li>
   <li> Additionally edges can be labelled as "negations". </li>
   <li> So these are just general circuits, only using negation and binary
   conjunction. </li>
   <li> The computation starts at the terminal nodes, and moves backwards.
   </li>
   <li> How to represent such digraphs?
    <ol>
     <li> One possibility is the straight-line format, that is, we do not use
     explicit digraphs, but just "node-lists". </li>
     <li> Or we use general digraphs, where the vertices are either variables
     or otherwise and-nodes, and where edges are either "positive" or
     "negative". </li>
    </ol>
   </li>
   <li> See http://en.wikipedia.org/wiki/And-inverter_graph for general
   information. </li>
   <li> Regarding external sources, see (at least) "Aiger" and "ABC" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp. </li>
  </ul>


  \todo File formats
  <ul>
   <li> Lexicographical listing of all values:
    <ol>
     <li> The file format should be very close to the Dimacs-format, but
     there is no need for line-end-symbols. </li>
     <li> So we have an initial comment block (every line started with "c").
     </li>
     <li> Then the parameter line, for boolean functions "p bft n" ("boolean
     function table"), while for general finite functions with n inputs,
     1 output, and value set D = {1, ..., p} we use "p fft n p". </li>
     <li> After this parameter-line we have (exactly) 2^n resp. p^n many
     entries separated by (at least one) space-symbol, each representing a
     value for inputs in lexicographical order (from left to right, using
     the natural order on integers). </li>
     <li> For boolean functions the values are 0,1, while the default for
     finite functions is 1, ..., p. </li>
     <li> Perhaps by "fftz" instead of "fft" we use zero-based values, i.e.,
     D = {0, ..., p-1}. So fftz for p=2 is exactly the case bft. </li>
    </ol>
   </li>
   <li> For information on how to use Espresso and the file format involved
   see Buildsystem/ExternalSources/SpecialBuilds/docus/Espresso.hpp . </li>
   </li>
  </ul>

*/

