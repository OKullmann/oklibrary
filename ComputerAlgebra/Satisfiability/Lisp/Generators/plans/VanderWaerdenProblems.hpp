// Oliver Kullmann, 20.9.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/VanderWaerdenProblems.hpp
  \brief Plans for Maxima-generators related to van der Waerden problems (and generalisations)


  \todo Connections
  <ul>
   <li> See RamseyTheory/plans/Van_der_Waerden_hypergraph.hpp. </li>
  </ul>


  \todo Statistics
  <ul>
   <li> As discussed in "Accompanying statistics" in
   Satisfiability/Lisp/Generators/plans/general.hpp in general, we need
   statistics for all main messurements (and also for all others, if
   possible). </li>
   <li> First the hypergraph measurements needs to be established; see
   "Statistics" in Hypergraphs/Lisp/plans/Generators.hpp. </li>
  </ul>


  \todo More than two parts
  <ul>
   <li> Create non-boolean clause-sets for the van der Waerden problems with
   more than two parts. </li>
   <li> Compare "Generalised colouring problems" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/general.hpp. </li>
  </ul>


  \todo Arithmetic progressions for prime numbers
  <ul>
   <li> The theorem of Green-Tao: "When partitioning the set of prime numbers
   into m parts, then at least one part contains arithmetic progressions
   of arbitrary lengths". </li>
   <li> Via compactness, this should translate into the property that for m
   and k there is (a least) n such that for every partitioning of the first
   n prime numbers into m parts one of the parts contains an arithmetic
   progressions of length k. </li>
   <li> The corresponding numbers are "Green-Tao numbers", greentao(m,k). </li>
   <li> One should have greentao(m,k) >= vanderwaerden(m,k). </li>
   <li> DONE
   We should create a function "greentao2_fcs(k,n)".
    <ol>
     <li> Via primes_first(n) we get the first n prime numbers. </li>
     <li> Finding the arithmetic progressions of length k can be done
     recursively. </li>
    </ol>
   </li>
   <li> DONE
   Of course, we also need the underlying hypergraphs. </li>
   <li> And we need the non-boolean versions. </li>
  </ul>

*/

