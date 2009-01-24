// Oliver Kullmann, 24.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/BoundedDeficiency/plans/general.hpp
  \brief Plans for algorithms handling problems of bounded deficiency (or, better, for algorithms guided by the maximal-deficiency-parameter)


  \todo Overview on fundamental tasks
  <ul>
   <li> Deciding SAT for (boolean and non-boolean) clause-sets via backtracking
   (bounded by deficiency) as given by [Szeider] and [Kullmann 2009]. </li>
   <li> Finding a maximal autarky for (boolean and non-boolean) clause-sets
   via search through partial assignments (bounded by deficiency) as given by
   [Kullmann 2009]. </li>
   <li> Using the matroid approach as in [Kullmann 2000]. </li>
   <li> Discuss the relations to module
   Satisfiability/Lisp/MinimalUnsatisfiability. </li>
   <li> Discuss the relations to module
   Satisfiability/Lisp/Autarkies. </li>
   <li> The deficiency for boolean clause-sets can be computed by
   max_deficiency_fcs in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Hypergraphs.mac; we need
   also the computation for non-boolean clause-sets. </li>
  </ul>

*/
