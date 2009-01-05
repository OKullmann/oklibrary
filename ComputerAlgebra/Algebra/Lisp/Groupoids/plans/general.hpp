// Oliver Kullmann, 6.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/plans/general.hpp
  \brief Plans in general regarding groupoids


  \todo Write tests


  \todo Forms of "constructive" groupoids
  <ul>
   <li> For implicit representations
   (see "Abstract representation of structures") we (still) assume
    <ol>
     <li> that representations of groupoid elements are given by terms (though
     there may be many terms), </li>
     <li> that we can decide equality, </li>
     <li> and that we can decide membership. </li>
    </ol>
   </li>
   <li> One can drop both (decidability) conditions, turning the groupoid
   operations into "promise functions" which only yield defined results
   if the inputs are actually groupoid elements. </li>
   <li> Independently we can consider that we have given a function f() which
   returns a "random element" of the finite groupoid:
    <ol>
     <li> The returned valued must be guaranteed to be an element. </li>
     <li> Strongest is guaranteed uniform distribution. </li>
     <li> In the extreme other case we don't have any guarantee. </li>
    </ol>
   </li>
   <li> Strengthenings of implicit representations:
    <ol>
     <li> A generating set is often at hand. </li>
     <li> Additionally a "rewrite algorithm" might be given, computing for
     each representation of a groupoid element a word in the generators
     representing that element. </li>
     <li> For such a rewrite algorithm one can require "canonicity", that
     is, for inputs representing the same element we obtain the same word
     (thus the rewrite algorithm also solves equality decision). </li>
    </ol>
   </li>
  </ul>


  \todo Straight-line programs
  <ul>
   <li> The most general form of a "straight-line program" to represent
   an element of a groupoid is just a Maxima term. </li>
   <li> Much more special, it is just a sequence of words, the first only
   in the given generators, and all further words are allowed to use all
   previous words, while the final word represents the element. </li>
  </ul>


  \todo Homomorphisms
  <ul>
   <li> The main notion of a homomorphism f: (G,*) -> (G',*') is the
   standard one: f: G -> G', f(x * y) = f(x) *' f(y). </li>
   <li> Considering a groupoid [G,compo] as a valued matrix [G,G,compo,G],
   we have the homomorphims of valued matrices, which in this context
   are apparently called "homotopy": H: (G,*) -> (G',*'), which are triples
   H = (f,g,h) where f,g,h: G -> G' such that f(x * y) = g(x) *' h(y). </li>
   <li> See "Valued matrices and their homo- and isomorphisms" in
   ComputerAlgebra/CombinatorialMatrices/Lisp/plans/Isomorphisms.hpp. </li>
   <li> The isomorphic homotopies are apparently called "isotopies". </li>
   <li> The notion of (ordinary) matrix homomorphism yield "principal
   homotopies" (resp. "principal isotopies"), which now assume that G = G',
   where then f = id_G is chosen, and the definiting equation is
   x * y = g(x) *' h(y). </li>
   <li> It seems that a homotopy can always be represented as the composition
   of a principal homotopy and a homomorphism. </li>
   <li> Additionally we can also consider G as a square matrix, which would
   lead to "square homotopies" H: (G,*) -> (G',*') as pairs H = (f,g) with
   f,g : G -> G' s.t. f(x * y) = g(x) *' g(y)", and square principal homotopies
   given by x * y = g(x) *' g(y). </li>
  </ul>

*/

