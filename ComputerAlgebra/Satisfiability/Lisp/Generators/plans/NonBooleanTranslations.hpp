// Oliver Kullmann, 18.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/NonBooleanTranslations.hpp
  \brief Plans for translations from non-boolean clause-sets to boolean clause-sets


  \todo Connections to other modules
  <ul>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/Colouring.mac (and
   ComputerAlgebra/Hypergraphs/Lisp/plans/Colouring.hpp) for translations
   of colouring problems to SAT problems. </li>
  </ul>


  \todo Translating non-boolean clause-sets into boolean clause-sets
  <ul>
   <li> DONE
   Write nbfcsfd2fcs_alo(FF) and nbfcsfd2fcs_aloamo(FF), translating
   non-boolean clause-sets into boolean clause-sets by using new boolean
   variables nbv(v,e) for old variables v and values e, adding in the first
   case only at-least-one-clauses for each v, and in the second case also
   adding the at-most-one-clauses. </li>
  </ul>

*/

