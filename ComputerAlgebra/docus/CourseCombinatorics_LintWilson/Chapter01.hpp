// Oliver Kullmann, 3.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/docus/CourseCombinatorics_LintWilson/Chapter01.hpp
  \brief Chapter "Graphs" from [A Course in %Combinatorics]


  <h2> On the notion of "graphs" </h2>

  Documentation:
  <ul>
   <li> See ComputerAlgebra/Graphs/Lisp/docus/general.hpp for docus on graphs
   at the Maxima/Lisp level. </li>
  </ul>


  <h2> Basic operations and properties for graphs </h2>


  <h2> On the automorphism group of the Petersen graph </h2>

  <ul>
   <li> The Petersen graph is available as follows:
   \verbatim
knesergraph_g(5,2);
  [{{1,2},{1,3},{1,4},{1,5},{2,3},{2,4},{2,5},{3,4},{3,5},{4,5}},
         {{{1,2},{3,4}},{{1,2},{3,5}},{{1,2},{4,5}},{{1,3},{2,4}},{{1,3},{2,5}},{{1,3},{4,5}},
          {{1,4},{2,3}},{{1,4},{2,5}},{{1,4},{3,5}},{{1,5},{2,3}},{{1,5},{2,4}},{{1,5},{3,4}},
          {{2,3},{4,5}},{{2,4},{3,5}},{{2,5},{3,4}}}]
petersen_graph();
  ?GRAPH
is_isomorphic(petersen_graph(), g2mg(knesergraph_g(5,2)));
  true
   \endverbatim
   </li>
   <li> In the "grape"-package of GAP the Kneser-graphs are (likely incorrectly)
   called "JohnsonGraph"; for example
   \verbatim
> gap
gap> LoadPackage("grape");
gap> Size(AutGroupGraph(JohnsonGraph(5,2)));
120
gap> Size(AutGroupGraph(JohnsonGraph(40,2)));
815915283247897734345611269596115894272000000000
gap> Factorial(40);
815915283247897734345611269596115894272000000000
   \endverbatim
   </li>
   <li> Thus one expects the automorphism group of knesergraph_g(n,2) to
   be (naturally) isomorphic to the S_40 ?! </li>
   <li> Investigating closer the automorphism group of the Petersen graph:
   \verbatim
gap> AutGroupGraph(JohnsonGraph(5,2));
Group([ (3,4)(6,7)(8,9), (2,3)(5,6)(9,10), (2,5)(3,6)(4,7), (1,2)(6,8)(7,9) ])
   \endverbatim
   (note that GAP uses the cyle-representation of permutations). </li>
   <li> The corresponding permutations in list-form are
   \verbatim
t1 : [1,2,4,3,5,7,6,9,8,10]$
t2 : [1,3,2,4,6,5,7,8,10,9]$
t3 : [1,5,6,7,2,3,4,8,9,10]$
t4 : [2,1,3,4,5,8,9,6,7,10]$
   \endverbatim
   Computing the closure, we see that this should be the S_5:
   \verbatim
length(closure_bydef_grd(transformation_l_compo, {t1,t2,t3,t4}));
  120
   \endverbatim
   </li>
  </ul>

*/

