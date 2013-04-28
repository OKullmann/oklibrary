// Oliver Kullmann, 4.9.2009 (Swansea)
/* Copyright 2009, 2010, 2012, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/UHittingCatalogue/plans/general.hpp
  \brief Plans regarding building the catalogue of unsatisfiable non-singular hitting clause-sets of low deficiency


  \todo Scope of the catalogue
  <ul>
   <li> Regarding the deficiency:
    <ol>
     <li> For deficiencies k=1,2 we definitely have all isomorphism types
     (counts 1 and 2). </li>
     <li> According to unpublished results of XSZ and me, also for k=3 we have
     all isomorphism types (count 25). </li>
     <li> For k=4 we should collect everything, in order to build up also here
     a complete collection. </li>
     <li> This is the most important goal; currently there are 274 elements.
     <li> We have closure under full variable-disjoint glueing for k <= 4,
     i.e., length(hm2sm(vardisjoint_glueing_uhit_def(4))) = 0. </li>
     </li>
    </ol>
   </li>
   <li> Regarding n:
    <ol>
     <li> For n <= 4 the catalogue is complete (determined computationally).
     </li>
     <li> Perhaps we should aim at getting everything for n <= 5 --- however
     there are might be millions of clause-sets. </li>
     <li> Counts: n=0 -> 1, n=1 -> 0, n=2 -> 1, n=3 -> 6, n=4 -> 368. </li>
     <li> DONE
     So let's just run all_unsinghitting(5, H5):
      <ol>
       <li> Aborted after, say, 2 weeks. </li>
       <li> Found 86505 (different) instances, with deficiencies
       \verbatim
L5 : analyse_isorepo_defset(H5)$
map(first,L5);
  [3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27]
       \endverbatim
       and with counts
       \verbatim
map(length,map(second,L5));
  [1,74,523,2082,5323,9986,14504,16793,15592,11439,6466,2717,812,162,20,2,1,1,1,1,1,1,1,1,1]
       \endverbatim
       </li>
       <li> Definitely up to deficiency 4 and from deficiency 18 on we enter
       the new clause-sets into the catalogue. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Regarding elements with extreme n:
    <ol>
     <li> For a given deficiency k, also all elements with minimal n, and all
     elements with maximal n (conjectured 3+4*(k-2)) and maximal n-1 should be
     collected. </li>
    </ol>
   </li>
   <li> Regarding closure under partial assignments:
    <ol>
     <li> We have closure under the operation of partial assignments for
     n <= 9 (determined by closure_uhit_def_pass(9, H)). </li>
     <li> closure_uhit_def_pass(10, H) was aborted after 5 1/2 days, without
     producing new results. </li>
     <li> closure_nonsingular_splitting_uhit(10,H) (which only consider
     elementary partial assignments) returns 0. </li>
     <li> Running closure_nonsingular_splitting_uhit(59,H) yields no new
     clause-sets (i.e., we have closure of the current catalogue under
     elementary partial assignments followed by complete
     singularity-reduction).
     </li>
    </ol>
   </li>
  </ul


  \todo Connections
  <ul>
   <li> W.r.t. general investigations into minimal unsatisfiability see
   Experimentation/Investigations/MinimalUnsatisfiability/plans/general.hpp.
   </li>
  </ul>


  \todo Integer sequences of interest
  <ul>
   <li> Once the finiteness-conjecture has been established, the sequence
   k >= 1 -> number of isomorphism types of nonsingular unsatisfiable hitting
   clause-sets of deficiency k is of great interest. Its first three terms are
   1,2,25. </li>
   <li> An easier sequence is
   n >= 0 -> number of isomorphism types of nonsingular unsatisfiable hitting
   clause-sets with n variables.
   Its first 5 terms are 1,0,1,6,368. </li>
   <li> We should submit these sequences to OEIS, once we have something
   published on them. </li>
   <li> For a given deficiency k, the maximal min-variable-degree is also of
   interest; once we get the journal version of the SAT2011 paper published,
   such sequences should be submitted to OEIS. </li>
  </ul>


  \todo Deficiency = 3, n = 5
  <ul>
   <li> It should be possible to completely determine these instances. </li>
   <li>
   \verbatim
oklib_monitor:true;
oklib_monitor_level:1;
all_unsinghitting_def(3,5,'Result_3_5);
   \endverbatim
   </li>
   <li> What does the monitor-output mean? Several times the same clause-sets
   are output? It seems that the isomorphism check takes only place
   afterwards. But we don't need to see these outputs. </li>
   <li> We need the output into a file. </li>
   <li> We need to incorporate the already known instances (from the
   catalogue). </li>
   <li> And we need the possibility to stop and re-start the whole
   computation, so that we have a chance of finally finishing this case.
   </li>
   <li> See "derived_hitting_cs_pred_isoelim" in
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp.
   </li>
  </ul>


  \todo Primes
  <ul>
   <li> From the catalogue we determine the primes for deficiency k via
   sublist(all_uhit_def(k), primeuhitting_p). </li>
   <li> For deficiency 1 we have exactly one prime, namely {{}}.
   <li> For deficiency 2 we have exactly one prime, namely musatd2_cs(3) =
   {{-3,-2,-1},{-3,1},{-2,3},{-1,2},{1,2,3}}. </li>
   For deficiency 3 we have exactly two primes, namely
   \verbatim
sublist(all_uhit_def(3), primeuhitting_p);
[
  {{-4,-3},{-4,-1,2,3},{-4,1,3},{-3,-2,-1,4},{-2,-1,3},{-2,1,4},{2,4}},
  {{-4,-3},{-4,2,3},{-3,-2,1,4},{-3,-1,4},{-2,3},{-1,2,3,4},{1,2,4}}
]
   \endverbatim
   </li>
   <li> The factors of length 2 are precisely the 2-subsumption pairs, so we
   need to eliminate them:
   \verbatim
L : sublist(all_uhit_def(3), redtsrp)$
length(L);
  3
   \endverbatim
   There is one composite element in this list, namely
   \verbatim
L[3];
{{-7,-6,-5,-4},{-7,-6,4},{-7,-5,6},{-7,-4,5},{-7,4,5,6},{-3,-2,-1,7},{-3,1,7},{-2,3,7},
         {-1,2,7},{1,2,3,7}}
   \endverbatim
   This is a clause-set realising the maximal number of variables, according to
   our conjecture. It has a full variable. </li>
   <li> A first guess was that a non-singular unsatisfiable hitting
   clause-set not containing a 2-subsumption or a full variable is prime.
   Checking this conjecture:
   \verbatim
conjecture_primeuhit(k) := sublist(sublist(sublist(all_uhit_def(k), redtsrp), lambda([F], not full_var_csp(F))), lambda([F], not primeuhitting_p(F)));
   \endverbatim
   So the conjecture is true for the elements of the catalogue of deficiency k
   iff the empty list is returned (otherwise a counterexample is returned):
   \verbatim
for k in uhit_def[0,"info"] do if not emptyp(conjecture_primeuhit(k)) then print(k);
   \endverbatim
   With the current catalogue it's false for k=3,4. But for all deficiencies
   there are counterexamples. </li>
  </ul>

*/
