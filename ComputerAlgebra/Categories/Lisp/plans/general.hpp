// Oliver Kullmann, 25.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Categories/Lisp/plans/general.hpp
  \brief Plans in general for categories in Maxima/Lisp


  \todo Basic notions
  <ul>
   <li> There are the two representations by pre-categories and by hom-sets.
   </li>
   <li> Sets are either implicitly or explicitly given, and their elements
   have a concrete representation or an abstract one (require an equality
   predicate). See "Abstract representation of structures" in
   ComputerAlgebra/Algebra/Lisp/plans/general.hpp. </li>
   <li> A "small category as precategory" is a triple
   [[obj,mor,d,c],compo,id] such that [obj,mor,d,c] is a precategory
   (compare "Graph concepts" in
   ComputerAlgebra/Graphs/Lisp/plans/Basic.hpp). </li>
   <li> A "large category as precategory" has obj, mor only implicitely
   given, and objects have an abstract representation, but morphisms have a
   concrete representation. </li>
   <li> While for a "extralarge category as precategory" the morphisms have
   an abstract representation. </li>
   <li> A "small category via homsets" is a quadruple [O,mor,compo,id]:
    <ol>
     <li> O is a (Maxima) set. </li>
     <li> mor(x,y) for two objects x,y is a (Maxima) set. </li>
     <li> compo(x,y,z) yields a binary function com(g,f), where f: x -> y
     and g: y -> z, which in turn produces a morphism from x -> z. </li>
     <li> id(x) yields a morphism x -> x. </li>
    </ol>
   </li> For a "large category via homsets" all sets are implicitly given,
   and objects have an abstract representation, but morphisms have a concrete
   representation. </li>
   <li> And for an "extralarge category via homsets" also morphisms have an
   abstract representation. </li>
   <li> It would be good if for the precategory-representation the homsets
   could be added, and for the homset-representation dom and cod. </li>
   <li> Instead of test-predicates we have "check-predicates", which for
   example for given composable morphisms check associativity and identity.
   </li>
  </ul>


  \todo Abbreviations
  <ul>
   <li> "scat" for small categories via homsets, "lcat" and "elcat" for large
   resp. extralarge categories via homsets. </li>
   <li> "cat_p, lcat_p, elcat_p" for the precategory-versions. </li>
  </ul>


  \todo Concrete categories
  <ul>
   <li> The category of sets, Csets_sm, is a large category via homsets, with
   set maps as morphisms. </li>
   <li> While Csets_mp is an extralarge category with maps as morphisms. </li>
   <li> "Cset2" denotes similarly the category of pair of sets. </li>
   <li> And "Csetp" has pointed sets as objects. </li>
   <li> Crel_sr  is a large category with sets as objects, and set-relations
   as morphisms, while Crel_p uses binary predicates. </li>
   <li> For a "concrete category" over a given category, only the forgetful
   functor on the objects needs to be specified, while as morphisms we always
   use the ones in the given category, and only a selection criterion needs
   to be provided? </li>
  </ul>


  \todo Functors
  <ul>
   <li> A functor is a pair of maps, where normally maps are functions (mp).
   </li>
   <li> So the category Cscat of small categories is a large category (via
   homsets), and Cscat_p is also a large category via homsets. </li>
   <li> Given a functor F, via obj(F) we obtain the object map, and via
   mor(F) we obtain the morphism map; but since functors are pairs, also
   F[1] and F[2] can be used. </li>
   <li> For small categories we might also consider maps as set-maps. </li>
  </ul>


  \todo Natural transformations
  <ul>
   <li> Given functors F, G, a natural transformation eta: F -> G is a map,
   which takes objects X as arguments, and eta(X) is a morphism from F(X)
   to G(X). </li>
  </ul>

*/

