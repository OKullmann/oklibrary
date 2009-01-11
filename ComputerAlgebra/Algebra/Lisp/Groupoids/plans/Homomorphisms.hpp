// Oliver Kullmann, 11.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/plans/Homomorphisms.hpp
  \brief Plans on groupoid homomorphisms


  \todo Homotopisms
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

