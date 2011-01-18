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
   <li> DONE
   The application is Reductions/Bases/RUcpBase.cpp. </li>
   <li> DONE
   Important for efficiency, that elimination of clauses and moving to
   a different clause (as partial assignment) can be done without much
   overhead. </li>
   <li>
   OKlib::Satisfiability::Reductions::UnitClausePropagation::CLSAdaptorUcpW
   basically fulfills these requirements:
    <ol>
     <li> DONE (used after extension of functionality)
     The assignment can be just placed into the partial assignment,
     without the need for further actions. </li>
     <li> DONE 
     Once a propagation has been performed, then nothing needs to be
     reset, since the watched literals per clause are always valid. </li>
     <li> For removing a clause the interface has to be changed, but
     otherwise it should be a simple operation, just removing the two
     watched literals. </li>
     <li> DONE (shouldn't be necessary)
     Perhaps we have to use the underlying clause-class
     OKlib::Satisfiability::ProblemInstances::Clauses::WatchedLiterals_mono ?
     </li>
    </ol>
   </li>
   <li> The problem with the Maxima-specification rand_rbase_cs in
   ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac is how to
   reproduce the Maxima-function random_permutation:
    <ol>
     <li> The Maxima-documentation says that the "Knuth shuffle algorithm"
     is used. This seems to be the following
     \verbatim
rand_perm(L) := block([n : length(L)],
 if n <= 1 then return(L),
 for i : 1 thru n-1 do block([r : random(n-i+1), s],
   if oklib_monitor then print(r),
   s : L[i],
   L[i] : L[i+r],
   L[i+r] : s
 ),
 L)$
N : 20;
L : create_list(i,i,1,N)$
set_random(1);
random_permutation(L);
set_random(1);
rand_perm(L);
     \endverbatim
     In both cases we get [6,14,3,12,20,19,2,10,5,8,1,15,17,11,16,4,7,18,13,9],
     while for N=10 in both cases we get [6,7,2,3,1,9,10,4,8,5]. In the latter
     case the sequence of random numbers is (5,5,4,3,1,3,3,2,1). </li>
     <li> While the C++ standard doesn't say anything about how the shuffling
     is performed?
      <ol>
       <li> The above algorithm rand_perm is implemented as ::random_shuffle in
       Satisfiability/Reductions/Bases/RandomShuffle.cpp. </li>
       <li> It differs from the result of std::random_shuffle. </li>
       <li> Ask on the Boost mailing list. </li>
      </ol>
     </li>
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


  \todo Create application tests for Bases/RUcpBase.cpp
  <ul>
   <li> We need to consider all extreme cases, plus a good mixture of normal
   cases. </li>
   <li> Once adding an assignment to the Ucp-object creates directly a
   contradiction, this is not cleared, and so it should be possible to create
   false output in this way? </li>
  </ul>


  \todo Write application tests for Bases/RUcpGen.cpp


  \todo Shuffling and sorting
  <ul>
   <li> We need an application which sorts a clause-list according to length,
   and then applies random permutations (or the identity) to the blocks of
   clauses of equal length. </li>
   <li> Then normally input F should be first processed via this
   sorting-shuffling. </li>
   <li> Sorting should be an option. </li>
   <li> For comparability with the Maxima-level we should use the Mersenne
   twister; see "Random r_1-bases" in
   Satisfiability/Reductions/Bases/plans/UcpBase.hpp. </li>
  </ul>


  \todo Improve efficiency of Bases/RUcpGen.cpp
  <ul>
   <li> Instead of recreating again and again a new Ucp-object U and filling it
   from scratch with the current F0, we should just update U by the clauses C
   added to F0 (if some change happened). </li>
   <li> Likely we just need to call the clause-member-function to add the
   clause, with a resetting of the assignment before that. </li>
  </ul>


  \todo Improve efficiency of Bases/RUcpBase.cpp
  <ul>
   <li> Instead of refilling the clause-set for UCP after every removal, only
   the changes needed (removing one clause, re-adding one clause) should be
   performed. </li>
   <li> For removing a clause the interface has to be changed, but
   otherwise it should be a simple operation, just removing the two
   watched literals. </li>
  </ul>


  \todo Further improve implementation of Bases/RUcpBase.cpp
  <ul>
   <li> The algorithm should go into a (reusable) class in RUcpBase.hpp. </li>
   <li> Use messages. </li>
   <li> Instead of the boost-classes for random numbers the C++0x components
   should be used, once we move away from gcc-4.1.2. </li>
   <li> Perhaps seed=0 should mean that no permutation is performed. </li>
   <li> Perhaps actually no permutation might be performed here at all, but
   left to an external application (as with
   Satisfiability/Reductions/Bases/RUcpGen.cpp)? </li>
  </ul>


  \todo Create unit-tests for Bases/RUcpBase.cpp


  \todo Create unit-tests for Bases/RUcpGen.cpp

*/
