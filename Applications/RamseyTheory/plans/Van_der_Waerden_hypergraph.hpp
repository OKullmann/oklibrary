// Oliver Kullmann, 31.10.2006 (Swansea)
/* Copyright 2006 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/RamseyTheory/plans/Van_der_Waerden_hypergraph.hpp
  \brief Plans on the module on computing van der Waerden numbers


  \todo Connections
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/VanderWaerdenProblems.hpp
   and
   ComputerAlgebra/Satisfiability/Lisp/Generators/VanderWaerdenProblems.mac.
   </li>
   <li> See
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/general.hpp
   and
   Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/general.hpp.
   </li>
   <li> See the various generators in Satisfiability/Transformers/Generators/
   (they should all be moved to here). </li>
  </ul>


  \todo Reactivate test system.


  \todo Update namespace.


  \todo New test system
  <ul>
   <li> Transform the tests to the new testsystem.
  </ul>


  \todo Update
  <ul>
   <li> Transfer the todo's from
   Applications/RamseyTheory/Van_der_Waerden_hypergraph.hpp to here (thereby
   updating them). </li>
   <li> Likely this file should be renamed to VanderWaerdenHypergraphs.hpp.
   </li>
   <li> The generation of hypergraphs should actually be part of
   Combinatorics/Hypergraphs. </li>
   <li> So this file then, after moving the (pure) hypergraph aspects,
   should become VanderWaerdenProblems.hpp. </li>
   <li> All the functionality like arithprog_primes_hg from the Maxima/Lisp
   level (and all the other functions there in
   ComputerAlgebra/Satisfiability/Lisp/Generators/VanderWaerdenProblems.mac)
   should be available here. </li>
   <li> Then likely all related generators in
   Satisfiability/Transformers/Generators/ should find its new home here. </li>
  </ul>


  \todo Generator
  <ul>
   <li> Write a class template for generating van der Waerden-hypergraphs
   (as model of a general hypergraph concept). </li>
  </ul>

*/

