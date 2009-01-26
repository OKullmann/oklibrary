// Oliver Kullmann, 21.11.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
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


  \todo General plans
  <ul>
   <li> An important motivation is the task of, given a finite function,
   find a representation of it by generalised CNF/DNF, propositional logic,
   CSPs, alliances of active clause-sets etc. </li>
   <li> So from this point of view, finite functions are as building blocks
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


  \todo Notions of "finite functions"
  <ul>
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
  </ul>


  \todo Straight-line programs
  <ul>
   <li> An important construction of a finite function is from a
   "gate list", which is a list L consistings of pairs [I,f], where
   f is an n x m ff, while I is a list of length n specifying the
   inputs of n, which are either pairs [i,j] for the i-th function in L
   with output number j, or are numbers i for the i-th input. </li>
   <li> These "general circuits" are normally called "straight-line
   programs". </li>
   <li> For the output we need to specify a list with pairs [i,j]
   or i (as before). </li>
   <li> Perhaps, when the function has only one output, then a singleton
   list [i] can be used. </li>
   <li> When using an input [i,j], then i must be smaller than the index
   of the gate (for which this is an input); in other words, the gates
   are nodes of a dag (directed acyclic graph), and we have given (via
   the ordering of L) a topological sorting. </li>
   <li> More precisely, the gates can only be properly understood as nodes
   if they have only one output. For m >= 2 outputs, perhaps the node is
   splitting into one "computational node", into which all the inputs go,
   and this node has m outgoing arcs to the "output nodes", where the
   respective single outputs can be obtained. </li>
   <li> We don't need to admit constant inputs, since these can be
   simulated by source-gates. </li>
  </ul>


  \todo Boolean circuits
  <ul>
   <li> Of interest is finding boolean circuits (%w.r.t. different bases) for
   given boolean functions. </li>
   <li> Boolean circuits are likely best modelled as boolean straight-line
   programs, where now best the gates come from a given set (for example
   the set B_2 of all binary boolean functions). </li>
   <li> See the work of Alexander S. Kulikov for example. </li>
   <li> Section 7.1.2 in [Knuth, The Art of Computer Programming: Introduction
   to Combinatorial Algorithms and Boolean Functions, 2008] contains a nice
   overview and concrete methods for computing optimal circuits (over the
   full base). </li>
   <li> Perhaps Heribert Vollmer is interested in such investigations? </li>
  </ul>

*/

