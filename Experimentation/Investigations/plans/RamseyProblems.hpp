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
   <li> See Ramsey.cpp (to be updated). </li>
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
      </ol>
     </li>
    </ol>
   </li>
   <li> For the ubcsat-algorithms we need some script to run through all
   of them. </li>
   <li> q = 6:
    <ol>
     <li> Known is 102 <= NR([6,6],2) <= 165. </li>
     <li> These clause-sets are too big (for direct representation). </li>
    </ol>
   </li>
  </ul>


  \todo Better SAT solvers
  <ul>
   <li> These problems seem to be quite structured, so that quite something
   is known (at least more than for Van der Waerden numbers), but SAT solvers
   apparently can't exploit any of it. </li>
   <li> By symmetry breaking we should achieve something. 
    <ol>
     <li> We should try to figure out the automorphism groups of the
     hypergraphs and the clause-sets. </li>
    </ol>
   </li>
   <li> Active clause-sets should be attractive,
    <ol>
     <li> Incorporating some of the knowledge we have for these problems. </li>
     <li> They should also be able to better detect symmetries. </li>
    </ol>
   </li>

*/

