// Oliver Kullmann, 3.5.2009 (Swansea)
/* Copyright 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/BranchingPrograms.hpp
  \brief Plans regarding branching programs (for finite functions)


  \todo Connections
  <ul>
   <li> In ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/BDDs.hpp
   we discuss BDDs. </li>
   <li> We need to find out how to organise these topics. </li>
  </ul>


  \todo The notion of "branching program" (BP)
  <ul>
   <li> A source for many constructions is [Wegener 2000, Branching Programs
   and Binary Decision Diagrams]. </li>
   <li> It seems best to consider only one output here. </li>
   <li> For p input values, q output values and n inputs (variables?)
   a branching program is a directed acyclic graph with exactly one source
   (designated), exactly q sinks (labelled with the output values), where
   every non-sink vertex is labelled by an input (variable?), and every such
   vertex has exactly p outgoing edges (labelled with the input values). </li>
  </ul>


  \todo Translations into (boolean circuits):
  <ul>
   <li> First let's consider only boolean circuits; the task is, given
   a branching program P of type [n,2,2], construct a boolean circuit. </li>
   <li> Theorem 2.1.3 in [Wegener 2000] yields a construction, using
   three circuits per node, by translating the ternary ite(x,y,z)
   ("if-then-else") into (x and y) or (not x and z), that is, using one
   binary disjunction, one binary conjunction, and one nonimplication. </li>
   <li> Attractive seems to me the following construction:
    <ol>
     <li> First we provide the "literals", that is, for each of the n inputs
     we provide its negation (by n negation gates). </li>
     <li> We introduce for each edge e in P a conjunction gate c(e),
     which has as inputs the incoming edges to the source of v plus
     the literal labelling e. </li>
     <li> So c(e) is the boolean function which is true for a total
     assignment f iff f passes through e. </li>
     <li> Now for the output gate we have two possibilities:
      <ul>
       <li> Either we consider the original 0-sink, and connect all its
       ingoing edges to a nor-gate. </li>
       <li> Or we consider the 1-sink, and connect all its ingoing
       edges to an or-gate. </li>
      </ul>
     These two possibilities reflect the double encoding of the output
     P(f) for a total assignment f: P(f) = 1 iff f doesn't reach 0
     iff f reaches 1. </li>
     <li> For the resulting circuit one should then perform the
     simplification which removes gates which don't have a connection to
     the output. </li>
     <li> We have fewer gates, and we have a simple structure; the price
     for this is using or's and and's of higher arity, which seems harmless
     to me. </li>
    </ol>
   </li>
  </ul>

*/

