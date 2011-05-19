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
   reproduce the Maxima-function random_permutation? See "Random shuffling"
   and "Random number generation". </li>
   <li> Perhaps for now we just use examples with a unique base.
   </li>
   <li> DONE (we use the boost-mt19937-generator, and the std::random_shuffle
   algorithm)
   Though we need to take these considerations into account for the
   design. </li>
   <li> DONE
   The application is Reductions/Bases/RUcpBase.cpp. </li>
   <li> DONE
   Important for efficiency, that elimination of clauses and moving to
   a different clause (as partial assignment) can be done without much
   overhead. </li>
  </ul>


  \todo Random shuffling
  <ul>
   <li> How to reproduce the Maxima-function random_permutation? </li>
   <li> See "Sampling of r-bases" in
   ComputerAlgebra/Satisfiability/Lisp/Reductions/plans/RBases.hpp. </li>
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
     <li> The code for std::random_shuffle uses a slightly different
     algorithm (from libstdc++-3.0 at 
     http://mirrors-us.seosue.com/gcc/libstdc++/old-releases/libstdc++-3.0.tar.gz - 
     include/bits/stl_algo.h) as can be seen in ::random_shuffle_libcpp and
     this simulates std::random_shuffle. </li>
     <li> std::random_shuffle is not standardised. </li>
    </ol>
   </li>
   <li> The Maxima-related aspects should go to
   ComputerAlgebra/Satisfiability/Lisp/Reductions/plans/RBases.hpp. </li>
  </ul>


  \todo Random number generation
  <ul>
   <li> How to simulate the Maxima random-generator? </li>
   <li> Maxima implements MT19937, as does boost (in boost::mt19937). This
   includes the generation of a 32-bit integer number. </li>
   <li> Maxima scales the 32-bit value it generates using MT19937 by taking
   the number modulo the upper range limit:
   \verbatim
set_random_state(make_random_state(1));
create_list(mod(random(2**32),20),i, 1,20);
  [5,19,4,8,3,13,11,1,19,12,8,9,16,3,13,0,2,1,2,16]
set_random_state(make_random_state(1));
create_list(random(20),i, 1,20);
  [5,19,4,8,3,13,11,1,19,12,8,9,16,3,13,0,2,1,2,16]
   \endverbatim
   </li>
   <li> Maxima's random() function can be simulated by using boost::mt19937
   directly and the C++ mod operator. See the "Underlying MT19937 generated
   integers scaled using mod" output from RandomShuffle.cpp. </li>
   <li> Understanding how boost scales the result of MT19937 into range:
   <ul>
    <li> The Boost documentation doesn't say anything how to construct the
    distribution-object, so that out of that all the random_number_generator
    in the sensible way is generated? Ask on the mailing list. </li>
    <li> One can simply pass boost::mt19937 directly to
    random_number_generator. This produces the same results as when using
    uniform_distribution, as can be seen by the generator rg_wo_variate in
    Satisfiability/Reductions/Bases/RandomShuffle.cpp. </li>
    <li> boost::random_number_generator is as follows (boost 1.44):
    \verbatim
  result_type operator()(argument_type n)
  {
    typedef uniform_int<IntType> dist_type;
    return variate_generator<base_type&, dist_type>(_rng, dist_type(0, n-1))();
  }
    \endverbatim
    </li>
    <li> boost::uniform_distribution scales the random number generated
    into the correct range by scaling using division (see ::randn in
    Satisfiability/Reductions/Bases/RandomShuffle.hpp ). </li>
    <li> It is still not clear how either boost::random_number_generator or
    boost::uniform_distribution maps the full integer range into 1 to n.
    There doesn't seem to be any real description of this at
    http://www.boost.org/doc/libs/1_45_0/doc/html/boost_random/reference.html#boost_random.reference.concepts.uniform_random_number_generator .
    </li>
   </ul>
   <li> The Maxima-related aspects should go to
   ComputerAlgebra/Satisfiability/Lisp/Reductions/plans/RBases.hpp. </li>
   <li> DONE MG has asked further on the mailing list. </li>
   <li> DONE (Maxima implements MT19937 the same as boost)
   The code for Maxima's random number generation is in
   ExternalSources/builds/Maxima/ecl/maxima-5.21.1/src/rand-mt19937.lisp,
   but it is completely unclear how in Maxima out of "MT 19937"
   a random integer is constructed. The lisp code isn't easy to
   understand:
   \verbatim
(defun %random-integer (arg state)
  "Generates an integer greater than or equal to zero and less than Arg.
  Successive chunks are concatenated without overlap to construct integers
  larger than a single chunk. The return value has this property:
  If two integers are generated from the same state with Arg equal to 2^m and 2^n,
  respectively, then bit k is the same in both integers for 0 <= k < min(m,n).
  Each call to %RANDOM-INTEGER consumes at least one chunk; bits left over
  from previous chunks are not re-used."
  (declare (type (integer 1) arg) (type random-state state))
    (do*
      ((nchunks (ceiling (integer-length (1- arg)) random-chunk-length) (1- nchunks))
        (new-bits 0 (random-chunk state))
        (bits 0 (logior bits (ash new-bits shift)))
        (shift 0 (+ shift random-chunk-length)))
      ((= 0 nchunks)
        (rem bits arg))))
       \endverbatim
       </li>
       <li> MT19937 doesn't seem to specify how one maps the values it 
       produces to another integer range, and so likely this isn't 
       standardised. </li>
       <li> The Maxima-related aspects should go to
       ComputerAlgebra/Satisfiability/Lisp/Reductions/plans/RBases.hpp. </li>
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
   <li> The Maxima-related aspects should go to
   ComputerAlgebra/Satisfiability/Lisp/Reductions/plans/RBases.hpp. </li>
   <li> DONE (this is the role of the MT19937 "tempering transform")
   MT19937 doesn't seem to specify how one maps the values it
   produces to another integer range, and so likely this isn't
   standardised. </li>
  </ul>


  \todo Shuffling and sorting
  <ul>
   <li> For comparability with the Maxima-level we use the Mersenne twister;
   see "Random r_1-bases" above. </li>
   <li> Instead of the boost-classes for random numbers, the C++0x components
   should be used, once we move away from gcc-4.1.2. </li>
   <li> DONE (see Interfaces/InputOutput/SortByClauseLength.cpp)
   We need an application which sorts a clause-list according to length
   (shortest clauses first), keeping the order of clauses of equal length.
   </li>
   <li> DONE Another application applies a random permutations to a 
   clause-list. </li>
   <li> DONE These two applications are based on two CLS-adaptors. </li>
   <li> DONE Once we have that, the permutation-part should be removed from
   Bases/RUcpBase.cpp. </li>
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
  </ul>


  \todo Further improve implementation of Bases/RUcpGen.cpp
  <ul>
   <li> The algorithm should go into a (reusable) class in RUcpGen.hpp. </li>
   <li> Use messages. </li>
  </ul>


  \todo Create unit-tests for Bases/RUcpBase.cpp


  \todo Create unit-tests for Bases/RUcpGen.cpp


  \bug DONE (resetting now old state of contradicting_ucl)
  Testcase Intransitive2 fails
  <ul>
   <li> Failure as predicted by OK, while todo falsely removed by MG. </li>
  </ul>

*/
