// Oliver Kullmann, 6.10.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/docus/general.hpp
  \brief Central docus-file regarding computer algebra


  <h2> General principles </h2>

  The computer-algebra systems are taken as "programming languages with
  associated libraries". Thus the underlying languages are taken
  seriously(!), and they constitute two different abstraction levels:
  <ol>
   <li> The "basic level" is the "Lisp level", that is, CLisp with Maxima:
    <ul>
     <li> The language is untyped, and this is (taken for this level(!))
     "a feature, not a bug". </li>
     <li> No type checking of any kind is performed, but the algorithms
     are formulated as "pure generic algorithms" based on established
     conventions how to access the data. </li>
     <li> Thus the algorithms are open for more general applications than
     perhaps originally anticipated. </li>
     <li> As much as possible it should be possible to use terms (with
     symbolic constants) instead of fixed data representations. </li>
     <li> "Flow with Lisp", that is, put things in a natural way into
     lists, and neither worry about efficiency nor about abstraction
     and concept-hierarchies. </li>
     <li> No polymorphism is employed, but concrete representations
     (exploiting the generality of lists and terms) together with
     conversion functions. </li>
     <li> The intention of this level is that of quick and elegant
     programming, for prototyping and experimentation. </li>
     <li> No simulations of programming concepts which where not
     anticipated with Lisp. </li>
     <li> Furthermore, the role model for all data types is set theory.
      <ol>
       <li> For example, a graph is a pair of vertex-set and a set of 2-element
       subsets of the vertex-set. </li>
       <li> Faithful to the set-theoretic approach, a "vertex" can be
       anything. </li>
       <li> For a "general graph" we use a map from the set of edge-labels
       to the 1- or 2-element subsets. </li>
       <li> Similarly, a clause-set is a set of clauses, etc. </li>
      </ol>
     </li>
    </ul>
   </li>
   <li> The "extended level" is the "Aldor level", that is, Aldor
   with Axiom:
    <ul>
     <li> Aldor supports dependent types and concept hierarchies, and
     so here now abstraction is a central issue, with proper
     concept hierarchies and strict typing. </li>
     <li> What still is abstracted away, compared to the "full level",
     are fine-grained algorithmic choices and especially considerations
     regarding memory management and construction/destruction of objects. </li>
    </ul>
   </li>
   <li> The third level, the C++ level which is considered as the
   "full level", is no longer in the realm of computer algebra, and
   here in principal the full spectrum can be expressed, from hardware
   access to abstract concept hierarchies:
    <ol>
     <li> Especially algorithms can be fully fine-tuned. </li>
     <li> And construction/destruction of objects can be fully controled. </li>
    </ol>
   </li>
   <li> So, especially for SAT algorithms the general plan is to
    <ol>
     <li> first develop them at the simple Lisp-level, </li>
     <li> then to develop the fundamental abstractions at the Aldor-level, </li>
     <li> and finally to explore all possibilities at the C++ level. </li>
    </ol>
    However, development can also start at the C++ level, and mimic these
    steps by first starting with the "C core part" of C++, and then
    adding abstraction and refinements.
   </li>
  </ol>
  The underlying mathematical concepts are the same for all three levels,
  but their realisations might differ considerably in the three levels.


  <h2> Usage </h2>

  <ul>
   <li> See ComputerAlgebra/docus/Maxima.hpp for the Maxima/Lisp system. </li>
  </ul>

*/

