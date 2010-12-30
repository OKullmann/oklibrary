// Oliver Kullmann, 30.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/Bases/plans/RUcpBase.hpp
  \brief Plans regarding the computation of r_1-bases of clause-sets


  \todo r_1-bases
  <ul>
   <li> The first task is to compute r_1-bases (one could also speak of
   "UCP-bases"). </li>
   <li> The r_1_bases of a clause-set F are exactly those minimal F' <= F such
   that all C in F-F' can be derived from F' modulo subsumption by
   input-resolution. </li>
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
   <li> The problem with the Maxima-specification rand_rbase_cs is how to
   reproduce the Maxima-function random_permutation:
    <ol>
     <li> The Maxima-documentation says that the "Knuth shuffle algorithm"
     is used. </li>
     <li> And then we need to simulate the Maxima random-generator. </li>
     <li> This should all be possible without too much effort. </li>
     <li> However perhaps for now we just use examples with a unique base.
     </li>
     <li> Though we need to take these considerations into account for the
     design. </li>
    </ol>
   </li>
  </ul>

*/
