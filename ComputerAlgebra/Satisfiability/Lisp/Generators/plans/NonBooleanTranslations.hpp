// Oliver Kullmann, 18.4.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
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
   <li> See
   Experimentation/Investigations/BooleanTranslations/plans/general.hpp for
   investigations. </li>
  </ul>


  \todo Add standardised translations
  <ul>
   <li> Standardised translations from non-boolean to boolean clause-sets/lists
   should be implemented. </li>
   <li> For translations from non-boolean formal clause-lists, it seems natural
   to take the ordering given by the variable list, and the ordering given by
   the domain and then define the new ordering on the domain lexicographically
   based first on the variable ordering and then on the domain. </li>
   <li> Such a (variable,domain) lexicographical ordering would allow
   additional variables to be added to the problem at the end of the variable
   ordering without requiring renumbering. </li>
   <li> Perhaps two versions could be supplied, with one ordering first by 
   variable then domain, and then vice versa (then allowing the domain to be
   easily extended)? </li>
   <li> See 
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/RamseyProblems.mac 
   for an example of use. </li>
  </ul>


  \todo Generalised canonical translation
  <ul>
   <li> See Satisfiability/Lisp/FiniteFunctions/plans/TseitinTranslation.hpp
   for the boolean case. </li>
   <li> Consider a generalised DNF clause-sets F (i.e., with literals
   "variable = value"). </li>
   <li> The natural generalisation of the canonical translation has new
   variables for all clauses C of F, set to be equivalent to DNF({C}). </li>
   <li> This naturally needs the direct encoding, using one boolean variable
   for the literal "variable = value". </li>
  </ul>


  \todo DONE Translating non-boolean clause-sets into boolean clause-sets
  <ul>
   <li> DONE
   Write nbfcsfd2fcs_alo(FF) and nbfcsfd2fcs_aloamo(FF), translating
   non-boolean clause-sets into boolean clause-sets by using new boolean
   variables nbv(v,e) for old variables v and values e, adding in the first
   case only at-least-one-clauses for each v, and in the second case also
   adding the at-most-one-clauses. </li>
  </ul>


  \todo DONE Add translations for non-boolean clause-lists with uniform domain
  <ul>
   <li> See ComputerAlgebra/RamseyTheory/Lisp/RamseyProblems.mac and
   ramsey_nbfclud for possible use. </li>
  </ul>

*/

