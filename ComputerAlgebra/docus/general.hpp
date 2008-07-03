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
     <li> The role model for all "data types" here is set theory.
      <ol>
       <li> For example, a graph is a pair "[V,E]" of vertex-set V and a set
       E of 2-element subsets of the vertex-set. </li>
       <li> Faithful to the set-theoretic approach, a "vertex" can be
       anything. </li>
       <li> For a "general graph" we use a map from the set of edge-labels
       to the 1- or 2-element subsets. </li>
       <li> Similarly, a clause-set is a set of clauses, etc. </li>
      </ol>
     </li>
     <li> However, the fundamental objects now are lists:
      <ol>
       <li> In set theory the fundamental object is a set, an unordered
       structure, since the mathematical world is eternal, objects are
       not created, but exist. </li>
       <li> Computation on the other hand fundamentally relies
       on order, so the most basic object is accordingly the list. </li>
      </ol>
     </li>
     <li> "Objects stand for themselves":
      <ol>
       <li> As in set theory, a list can represent many things, and
       interpretations must be added to give it meaning. </li>
       <li> So for example "[{},{}]" can be
        <ul>
         <li> the graph with no vertices, </li>
         <li> the hypergraph with no vertices and no hyperedges, </li>
         <li> as well as the formal clause-set with no variables and no
         clauses. </li>
        </ul>
       </li>
       <li> Objects are not adorned to fix their meaning. </li>
      </ol>
     </li>
     <li> The language is untyped, and this is (taken for this level(!))
     "a feature, not a bug".
      <ol>
       <li> No type checking of any kind is performed, but the algorithms
       are formulated as "pure generic algorithms" based on established
       conventions how to access the data. </li>
       <li> Thus the algorithms are open for more general applications than
       perhaps originally anticipated. </li>
       <li> For defining the concepts, and for error checking, predicates
       are provided which check whether objects are well-formed according
       to the notion of a "graph", a "clause-set" etc.; but these predicates
       are not used for programming. </li>
      </ol>
     </li>
     <li> "Flow with Lisp", that is, put things in a natural way into
     lists, and neither worry about efficiency nor about abstraction
     and concept-hierarchies. </li>
     <li> No polymorphism is employed, but concrete representations
     (exploiting the generality of lists and terms) together with
     conversion functions.
      <ol>
       <li> A central point here is that <strong>abstraction must come
       later</strong>. </li>
       <li> First we make concrete  experiences with the
       <em>real</em> multiplicity of concrete types. </li>
       <li> Abstraction is then added at the Aldor and C++ level. </li>
      </ol>
     </li>
     <li> The intention of this level is that of quick and elegant
     programming, for prototyping and experimentation.
      <ol>
       <li> It shall provide the specificational basis for the whole
       library. </li>
       <li> No simulations of programming concepts which where not
       anticipated with Lisp. </li>
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
     <li> It seems that most development will happen at the basic level (the
     Maxima/Lisp level) or at the full level, while the Axiom/Aldor
     level is about planning, formulating and prototyping abstraction.
      <ol>
       <li> For example, at the Maxima/Lisp level we develop a systematic
       and fine-grained, but non-abstract account of all graph- and hypergraph
       concepts, and a good selection on algorithms operating with these
       concepts. </li>
       <li> After seeing this picture (for the first time!), we look at
       common principles, common methods, common interfaces. </li>
       <li> That is, the multitude of concrete concepts is organised into
       a network of abstract concepts. </li>
      </ol>
     </li>
     <li> Currently we are in the phase of building up the basic level, and
     thus work at the Axiom/Aldor level is slow-going. </li>
    </ul>
   </li>
   <li> The third level, the C++ level which is considered as the
   "full level", is no longer in the realm of computer algebra, and
   here in principal the full spectrum can be expressed, from hardware
   access to abstract concept hierarchies:
    <ol>
     <li> Especially algorithms can be fully fine-tuned. </li>
     <li> And construction/destruction of objects can be fully controlled. </li>
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
   <li> The usage of Sage is restricted to provide interfaces between the
   different systems. </li>
  </ol>
  The underlying mathematical concepts are the same for all three levels,
  but their realisations might differ considerably in the three levels.


  <h2> Usage </h2>

  <ul>
   <li> See ComputerAlgebra/docus/Maxima.hpp for the Maxima/Lisp system. </li>
  </ul>


  <h2> Tutorials </h2>

  <ul>
   <li> Tutorials regarding specific computer algebra systems are found at
   their docus-pages. </li>
   <li> A tutorial based on the well-known textbook "A Course in Combinatorics"
   can be accessed in docus/CourseCombinatorics_LintWilson/index.hpp. </li>
  </ul>

*/

