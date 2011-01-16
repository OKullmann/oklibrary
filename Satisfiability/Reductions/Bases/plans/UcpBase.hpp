// Oliver Kullmann, 30.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/Bases/plans/UcpBase.hpp
  \brief Plans regarding the computation of r_1-bases of clause-sets

  r_1-bases (or "UCP-bases") of a clause-set F are those minimal F' <= F such
  that all C in F-F' can be derived from F' modulo subsumption by
  input-resolution.


  \todo Random r_1-bases
  <ul>
   <li> The application is Reductions/Bases/RUcpBase.cpp. </li>
   <li> Important for efficiency, that elimination of clauses and moving to
   a different clause (as partial assignment) can be done without much
   overhead. </li>
   <li> It seems that
   OKlib::Satisfiability::Reductions::UnitClausePropagation::CLSAdaptorUcpW
   basically fulfills these requirements:
    <ol>
     <li> The assignment can be just placed into the partial assignment,
     without the need for further actions. </li>
     <li> Once a propagation has been performed, then nothing needs to be
     reset, since the watched literals per clause are always valid. </li>
     <li> For removing a clause the interface has to be changed, but
     otherwise it should be a simple operation, just removing the two
     watched literals. </li>
     <li> Perhaps we have to use the underlying clause-class
     OKlib::Satisfiability::ProblemInstances::Clauses::WatchedLiterals_mono ?
     </li>
    </ol>
   </li>
   <li> The problem with the Maxima-specification rand_rbase_cs in
   ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac is how to
   reproduce the Maxima-function random_permutation:
    <ol>
     <li> The Maxima-documentation says that the "Knuth shuffle algorithm"
     is used. </li>
     <li> While the C++ standard doesn't say anything about how the shuffling
     is performed? </li>
     <li> And then we need to simulate the Maxima random-generator.
      <ol>
       <li> The Boost documentation doesn't say anything how to construct the
       distribution-object, so that out of that all the random_number_generator
       in the sensible way is generated? Ask on the mailing list. </li>
       <li> Then it is completely unclear how in Maxima out of "MT 19937"
       a random integer is constructed. Ask on the Maxima mailing list. </li>
      </ol>
     </li>
     <li> Perhaps for now we just use examples with a unique base.
     </li>
     <li> DONE (we use the boost-mt19937-generator, and the std::random_shuffle
     algorithm)
     Though we need to take these considerations into account for the
     design. </li>
    </ol>
   </li>
  </ul>

*/
