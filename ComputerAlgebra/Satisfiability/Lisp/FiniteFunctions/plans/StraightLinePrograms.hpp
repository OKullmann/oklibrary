// Oliver Kullmann, 3.5.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
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


  \todo Other translations (than the Tseitin translation)
  <ul>
   <li> See [Manolios, Vroon; SAT 2007].
    <ol>
     <li> As types of circuits "Reduced Boolean Circuits" (RBCs),
     "And-Inverter Graphs" (AIGs) and "NICE dags" are mentioned. </li>
     <li> The basis for a nice-dag is given by negation, ite (if-then-else),
     conjunction (n-ary?) and equivalence (n-ary?). And then there are
     several restrictions similar to RBCs, to maximise "sharing". </li>
     <li> A quadratic-time algorithm to translate such nice-dags into
     CNF is presented, and implemented in their system "BAT" (see "BAT" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/ModelChecking.hpp). </li>
     <li> The "Jackson-Sheridan" algorithm is mentioned, translating an RBC
     into CNF. </li>
     <li> And they also mention the "Brummayer-Biere" algorithm, from AIGs
     to CNF. </li>
    </ol>
   </li>
   <li> See [Een, Mishchenko, Soerensson; SAT 2007]
    <ol>
     <li> AIGs are considered: gates are binary conjunction, negation
     and the constant true, where negation is handled by "signing" edges. </li>
     <li> First rewrite-rules on AIGs are systematically investigated. </li>
     <li> And then the CNF-translation happens via "technology mapping",
     some form of optimisation process. </li>
    </ol>
   </li>
  </ul>

*/

