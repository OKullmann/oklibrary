// Oliver Kullmann, 27.1.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/general.hpp
  \brief Plans for Maxima-components regarding autarkies


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo Basic functionality
  <ul>
   <li> Check whether a partial assignment is an autarky:
    <ol>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac. </li>
     <li> Use analyse_pa. </li>
    </ol>
   </li>
  </ul>


  \todo Balanced autarkies


  \todo Pure autarkies


  \todo Autarky search via running through all total assignments
  <ul>
   <li> First implement the autarky extraction as discussed in
   Autarkies/Search/plans/AnalyseTotalAssignment.hpp. </li>
   <li> Then implement a simple search through all total
   assignments. </li>
   <li> We need a module on local search where we implement the basic
   algorithms (of course, this will be slow, but that doesn't matter
   here). </li>
   <li> Then we can experiment with different connections to
   local search (as discussed in
   Autarkies/Search/plans/AnalyseTotalAssignment.hpp. </li>
  </ul>


  \todo Implement the translation of USAT to LEAN according to our
  article with Victor and Mirek


  \todo The autarky monoid
  <ul>
   <li> Implement the full representation, all essential autarkies
   together with the composition of partial assignments. </li>
   <li> Is there some computer-algebra-support for representing
   semigroups (and monoids)? </li>
  </ul>

*/

