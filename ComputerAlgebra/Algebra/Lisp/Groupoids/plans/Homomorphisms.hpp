// Oliver Kullmann, 11.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/plans/Homomorphisms.hpp
  \brief Plans on groupoid homomorphisms


  \todo Complete the implementation
  <ul>
   <li> Write simple tests. </li>
   <li> Complete the implementation according to "XXX". </li>
   <li> Write little helper functions for working with
   all_isotopismisomorphism_classes_actively_grd. </li>
  </ul>


  \todo Organisation
  <ul>
   <li> Perhaps we should subdivide the topic into four sub-topics:
   (Ordinary) homomorphisms, isomorphisms, homotopisms, isotopisms. </li>
  </ul>


  \todo Naming conventions
  <ul>
   <li> Perhaps "homomorphism_bydef_grd" should better be called
   "homomorphism_grd_p". </li>
   <li> Similarly for the other functions. </li>
  </ul>


  \todo Isomorphisms
  <ul>
   <li> In is_isomorphic_bydef_grd we constructed the bijections directly;
   we should gather all similar constructions in ComputerAlgebra, and
   we should provide general facilities for that (see also
   all_isomorphisms_classes_actively_grd). </li>
   <li> Reducing groupoid-isomorphism to graph isomorphism works via the
   Cayley graphs? </li>
   <li> Again we need backtracking algorithms, which should fit into a
   general framework. </li>
  </ul>


  \todo All isomorphism types of small groupoids
  <ul>
   <li> We need representatives: Perhaps best the lexicographically smallest
   elements (using row-wise order). </li>
   <li> For the basic properties (associativity, unitality, commutativity,
   unique solvability, non-trivial automorphism group; together with all
   combinations) we need counts and representatives. </li>
   <li> We need also to build up catalogues. </li>
  </ul>


  \todo Transport via homotopies
  <ul>
   <li> Given a groupoid [V,compo] and three bijections f,g,h: V -> V',
   we obtain a composition on V' we transporting compo(x,y) = z to
   compo(f(x),g(y)) = h(z). </li>
   <li> Once we have that, we can also compute all isotopism classes
   ("actively"). </li>
  </ul>


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


  \todo Induced congruence relation
  <ul>
   <li> Compare "Congruence relations" in
   ComputerAlgebra/Algebra/Lisp/Groupoids/plans/general.hpp. </li>
   <li> What induced_congruence_grd computes, is just the equivalence
   relation induced by a map. </li>
   <li> We should develop more general notions, likely at the level of
   (pure) sets. </li>
   <li> Perhaps this should all be located in "Quotients.mac". </li>
  </ul>

*/

