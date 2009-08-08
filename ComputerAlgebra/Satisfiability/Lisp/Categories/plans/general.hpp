// Oliver Kullmann, 11.7.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Categories/plans/general.hpp
  \brief General plans for categories of clause-sets


  \todo Relations to other modules
  <ul>
   <li> See ComputerAlgebra/Categories/Lisp/plans/general.hpp for the
   general framework. </li>
   <li> Symmetries (see
   ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp)
   specifically handles isomorphisms. </li>
  </ul>


  \todo Fundamental notions
  <ul>
   <li> Likely homomorphisms are special cases of substitutions; see
   "Applying substitutions" in
   ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp. </li>
   <li> A morphism f: F -> G would then be a substitution which maps the
   literals from F to the literals from G, and which translates the clauses
   of F into the clauses of G.
    <ol>
     <li> Abbreviations like "morcls" (morphism for clause-sets). </li>
     <li> It seems that the morphism itself just maps literals to literals
     (or variables to variables in case of variable-based categories; see
     below), while the action on the clauses is just induced. </li>
     <li> The two main conditions for clauses C are that f(C) equals a
     clause in the codomain, or, more generally, there exists a clause D
     in the codomain with D <= f(C). </li>
    </ol>
   </li>
   <li> We have variable-based and literal-based categories, which are
   concrete categories of the categories of var-sets resp. lit-sets.
    <ol>
     <li> A var-set is just a set, while a lit-set is a ZZ_2-set. </li>
     <li> A ZZ_2-set is a pair [L,opr], according to
     ComputerAlgebra/Algebra/Lisp/Groupoids/Operations/plans/general.hpp,
     where opr(e,l) for e in {0,1} and l in L yields an element of L. </li>
     <li> This uses ZZ_2 = cyclic_ugrp(2) (see
     ComputerAlgebra/Algebra/Lisp/Groupoids/Groups/CyclicGroups.mac), and so
     actually one can also use opr(e,l), where e is not just 0 or 1, but an
     arbitrary integer (interpreted as 0 or 1 iff its even or odd). </li>
     <li> Could we also just use as set L of integers as a literal set, which
     is interpreted via 1*l = -l ? Likely we should better employ a conversion
     function here. </li>
    </ol>
   </li>
  </ul>

*/

