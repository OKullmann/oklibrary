// Oliver Kullmann, 11.6.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/Generators/plans/VanderWaerden.hpp
  \brief Plans on generators for vdW-hypergraphs


  \todo DONE
  Transfer Applications/RamseyTheory/plans/Van_der_Waerden_hypergraph.hpp
  to here


  \todo Generator
  <ul>
   <li> Write a class template for generating van der Waerden-hypergraphs
   (as model of a general hypergraph concept). </li>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/Generators/VanderWaerden.mac for
   the basic function specifications. </li>
   <li> DONE Write a generator for the levels (used in the incremental
   approach). </li>
  </ul>


  \todo Update namespace usage


  \todo New test system
  <ul>
   <li> Transfer the tests to the new testsystem. </li>
   <li> Write tests for all components. </li>
  </ul>


  \todo Update
  <ul>
   <li> Transfer the todo's from
   ypergraphs/Generators/plans/VanderWaerden.hpp to here (thereby updating
   them). </li>
   <li> See class IteratorHandling::Arithmetical_progression in
   General/IteratorHandling.hpp. </li>
   <li> DONE
   Likely this file should be renamed to VanderWaerdenHypergraphs.hpp.
   </li>
   <li> DONE
   The generation of hypergraphs should be part of
   Combinatorics/Hypergraphs/Generators. </li>
   <li> DONE
   So the content of this file should be moved to
   Combinatorics/Hypergraphs/Generators/RamseyTheory/plans/VanderWaerdenProblems.hpp. </li>
   <li> All the functionality like arithprog_primes_hg from the Maxima/Lisp
   level (and all the other functions there in
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/VanderWaerdenProblems.mac)
   should be available there. </li>
  </ul>

*/

