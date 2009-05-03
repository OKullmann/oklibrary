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


  \todo Representing boolean functions by their truth tables
  <ul>
   <li> The truth table is just a sequence of bits. So once the order
   is standardised (in such a way that it adding variables doesn't
   change the order), truth tables can be jused represented by natural
   numbers (including zero), plus the information how many variables are
   there. </li>
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

*/

