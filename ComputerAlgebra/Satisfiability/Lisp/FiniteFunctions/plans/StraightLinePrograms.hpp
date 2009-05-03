// Oliver Kullmann, 3.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/StraightLinePrograms.hpp
  \brief Plans regarding straight-line programs ("circuits")


  \todo The notion of a "straight-line program"
  <ul>
   <li> An important construction of a finite function is from a
   "gate list", which is a list L consisting of pairs [I,f], where
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


  \todo The notion of a "boolean circuit"
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
   <li> Another important topic, generalising the considerations under
   "Simplifications" in
   ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/general.hpp,
   is simplification of boolean circuits. </li>
  </ul>


  \todo Translation of boolean circuits into boolean CNF-clause-sets
  <ul>
   <li> It seems there is basically only one translation known, the "Tseitin
   translation". And for every other form of representations of
   finite functions the CNF-translation seems always to work via
   first constructing a circuit, and then representing this circuit
   as CNF. </li>
   <li> See
   Satisfiability/Transformers/Generators/plans/TseitinTransformation.hpp
   for the general considerations. </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/TseitinTranslation.hpp
   for the special case of propositional formulas. </li>
   <li> For each gate g in a circuit C, one represents the inputs i
   by variables v_i, the output by variable o, and then represents
   the equivalence o <-> g( ... v_i ...) by a CNF. </li>
   <li> The conjunction of all these CNFs yields the whole CNF, where
   once the values for all inputs are given, all other variables have
   a unique value so that the whole CNF becomes true. </li>
   <li> This also works for non-boolean circuits and various generalised
   forms of non-boolean CNF-clause-sets. </li>
  </ul>


  \todo Other translations
  <ul>
   <li> Likely we should have a module Transformers/Propositional, for the
   various translations from propositional logic into CNF's etc. </li>
   <li> See [Manolios, Vroon; SAT 2007] for a recent new translation. </li>
   <li> in [Een, Mishchenko, Soerensson; SAT 2007] several translations from
   different types of circuits to SAT are presented. </li>
  </ul>

*/

