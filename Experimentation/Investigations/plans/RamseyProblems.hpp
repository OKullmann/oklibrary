// Oliver Kullmann, 19.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/RamseyProblems.hpp
  \brief On investigations into %Ramsey problems


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/RamseyTheory/Lisp/Ramsey/plans/general.hpp. </li>
   <li> See Ramsey.cpp (to be updated) for a C++ generator. </li>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/plans/general.hpp.
   </li>
  </ul>


  \todo Generation of problem instances
  <ul>
   <li> Generation of CNF representations:
    <ol>
     <li> By the Maxima system, via "output_ramsey2_stdname(q,r,n)" we
     create "Ramsey_q_r_n.cnf", for NR([q,q],r). </li>
     <li> However, output_ramsey2_stdname(5,2,40) yields a segmentation
     fault on a 32-bit machine (not enough memory, when introducing
     the colouring symbols); so we need to use the following (C++)
     application. </li>
     <li> More generally (and faster), the application "Ramsey" can generate
     also non-diagonal problems, via
     \verbatim
Ramsey-O3-DNDEBUG q1 q2 r n | ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_q1_q2_r_n.cnf
     \endverbatim
     for NR([q1,q2],r). </li>
    </ol>
   </li>
   <li> What about constraint solvers? </li>
  </ul>


  \todo Bounds on NR([q,q],2)
  <ul>
   <li> How to handle the following data in a more systematic fashion,
   using tools from the OKlibrary? </li>
   <li> q = 3:
    <ol>
     <li> Satisfiability for n=5 is trivial for any SAT solver. </li>
     <li> So is unsatisfiability for n=6. </li>
    </ol>
   </li>
   <li> q = 4:
    <ol>
     <li> Satisfiability for n=17:
      <ul>
       <li> Trivial for OKsolver. </li>
       <li> Grasp aborts by default (10000s exceeded). </li>
       <li> Trivial for minisat. </li>
       <li> Trivial for march_pl. </li>
      </ul>
     </li>
     <li> Unsatisfiability for n=18:
      <ul>
       <li> OKsolver can't solve it in ~ 5 hours. </li>
       <li> Grasp aborts by default (10000s exceeded). </li>
       <li> minisat seems hopeless (after one hour). </li>
       <li> march_pl seems hopeless (after one hour). </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> q = 5:
    <ol>
     <li> Known is 43 <= NR([5,5],2) <= 49. </li>
     <li> So we get 1-2 million clauses; we can try with a local search
     solver. </li>
     <li> OKsolver can handle n=33 in ~ 3 minutes, but has problems with
     n=34. </li>
     <li> march_pl seems to have problems already for n=30. </li>
     <li> minisat can handle n=30 in 2 seconds; n=35 ? </li>
     <li> The list of algorithms from ubcsat:
      <ol>
       <li> "ubcsat -alg gsat -runs 100 -i Ramsey_5_2_n.cnf": trivial for n=30;
       n = 35 needs 300 - 30000 steps. Using "-cutoff 2000" seems reasonable.
       </li>
       <li> gwsat: trivial for n=30; n = 35 ? </li>
       <li> hsat : n = 35 needs ~ 3000 steps; n = 40 ? </li>
       <li> adaptnovelty+: n = 35 ? </li>
       <li> saps: n = 35 in 600 steps. Using "-cutoff 2000" seems reasonable;
       looks strong. n = 40 ? </li>
       <li> rsaps: similar; but looks bleak for n = 40 (cutoffs 10000 or
       30000 yield nearly the same). </li>
       <li> samd performs yet best, and also scales to higher cutoffs,
       but very slowly: With n=40, cutoff = 10 000 000, in 12 rounds 9 outcomes
       were 30-something, one was 46, and we had 23 (the minimum) and 27
       (regarding the falsified clauses). </li>
      </ol>
     </li>
     <li> Investigating samd:
      <ol>
       <li> n=40:
        <ol>
         <li> cutoff = 10 000 000 and noimprove = 1 000 000 </li>
         <li> cutoff = 20 000 000, noimprove = 3 000 000: 100 rounds, maximum
         10 500 000 steps, average 4 300 000 steps, minimum 1 falsified clause
         (seed 18745606), 2912 flips per second. </li>
         <li> One round, cutoff = 20 000 000, with previous best seed. </li>
        </ol>
       </li>
       <li> n=41:
        <ol>
         <li> cutoff =  10 000 000, with 40 rounds most results were
         fiftyish to sixtyish, but one outlier reached 28 (the minimum). </li>
         <li> cutoff = 20 000 000, noimprove = 2 000 000:
         100 rounds has a maximum of 8 000 000 steps, average of
         3 300 000 steps, and 2261 flips per second. The optimum reached was
         13 falsified clauses, which seems very good. </li>
         <li> noimprove = 3 000 000: 100 rounds, maximum 12 600 000 steps,
         average 5 000 000 steps, 2294 flips per second, minimum 12 falsified
         clauses (seed 1833218426) </li>
         <li> One round, cutoff = 20 000 000, with previous best seed. </li>
        </ol>
       </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> For the ubcsat-algorithms we need some script to run through all
   of them; see ExperimentSystem/plans/RunUBCSAT.hpp. </li>
   <li> q = 6:
    <ol>
     <li> Known is 102 <= NR([6,6],2) <= 165. </li>
     <li> These clause-sets are too big (for direct representation). </li>
    </ol>
   </li>
  </ul>


  \todo Investigating the parameter tuple [[3,3],2]
  <ul>
   <li> Creating the relevant instances: by
   \verbatim
R5: ramsey2_ofcs(3,2,5);
R6: ramsey2_ofcs(3,2,6);
   \endverbatim
   we create the two relevant (ordered formal) clause-sets. </li>
   <li> Satisfying assignments:
   \verbatim
R5SAT : setify(all_sat_ofcs(R5))$
length(R5SAT);
  12
   \endverbatim
   </li>
   <li> Now we need to investigate the operation of the automorphism group
   of R5 on R5SAT (what are the really different solutions?). </li>
   <li> For this we need to compute the automorphism group of R5; see
   "Automorphisms of Ramsey clause-sets" in
   RamseyTheory/Lisp/Ramsey/plans/general.hpp. </li>
   <li> Via
   \verbatim
length(all_aut_ofcs(R5));
  13
   \endverbatim
   we see that there are only the obvious autarkies. </li>
  </ul>


  \todo Autarkies
  <ul>
   <li> We should investigate autarkies of Ramsey-clause-sets. </li>
   <li> It could be that for smaller n interesting autarkies exist, and so
   they could serve for providing lower bounds. </li>
  </ul>


  \todo Blocked clauses
  <ul>
   <li> We should investigate blocked clauses of Ramsey-clause-sets. </li>
  </ul>


  \todo Symmetry breaking
  <ul>
   <li> An important way of making the problems simpler for SAT solvers
   is to add symmetry-breaking clauses. </li>
   <li> We should try to figure out the automorphism groups of the clause-sets.
   See "Automorphisms of Ramsey clause-sets" in
   RamseyTheory/Lisp/Ramsey/plans/general.hpp. </li>
   <li> The goal is to find as many as possible assignments which can
   be made "w.l.o.g."; and also additional short clauses are of interest. </li>
  </ul>


  \todo "Visualising" solutions
  <ul>
   <li> Via SAT solvers we can compute certain solutions for problems somewhat
   smaller than the interesting sizes. </li>
   <li> The task is to "look" at these solutions, extract some structure, and
   then to systematically search for "such" solutions. </li>
   <li> Of course, everything theoretically known needs to be explored. </li>
  </ul>


  \todo Better SAT solvers
  <ul>
   <li> These problems seem to be quite structured, so that quite something
   is known (at least more than for Van der Waerden numbers), but SAT solvers
   apparently can't exploit any of it. </li>
   <li> Active clause-sets should be attractive.
    <ol>
     <li> Incorporating some of the knowledge we have for these problems. </li>
     <li> They should also be able to better detect symmetries. </li>
    </ol>
   </li>
   <li> Compressed representation of clauses:
    <ol>
     <li> If that ZAP systems, which allows to use "annotated clauses", clauses
     plus a permutation group acting on the variables, is open-source, then it
     would be very natural to apply it here (since the clause-set F_R([q_1, ...,
     q_s], r, n) can be expressed then by just s clauses together with the
     symmetric group S_n). </li>
     <li> A point here is that S_n does act faithful but not
     stronlgy faithful on the clauses (i.e., on the underlying hypergraph) ---
     can this be repaired or improved (S_n is a rather large group here)? </li>
    </ol>
   </li>
  </ul>

*/

