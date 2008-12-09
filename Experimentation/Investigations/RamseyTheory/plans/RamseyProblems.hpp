// Oliver Kullmann, 19.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/plans/RamseyProblems.hpp
  \brief On investigations into %Ramsey problems


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/RamseyTheory/Lisp/Ramsey/plans/general.hpp. </li>
   <li> See Ramsey.cpp (to be updated) for a C++ generator. </li>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/RamseyProblems.hpp.
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
  </ul>


  \todo How to handle experimental data?
  <ul>
   <li> How to store the data? </li>
   <li> For the ubcsat-algorithms we need some script to run through all
   of them; see ExperimentSystem/plans/RunUBCSAT.hpp. </li>
  </ul>


  \todo Data sheets
  <ul>
   <li> All the numbers and all known information should be available
   on some html-page. </li>
  </ul>


  \todo NR([3,3],2)
  <ul>
   <li> Satisfiability for n=5 is trivial for any SAT solver. </li>
   <li> So is unsatisfiability for n=6. </li>
  </ul>


  \todo NR([4,4],2)
  <ul>
   <li> Satisfiability for n=17:
    <ol>
     <li> Trivial for OKsolver. </li>
     <li> Grasp aborts by default (10000s exceeded). </li>
     <li> Trivial for minisat. </li>
     <li> Trivial for march_pl. </li>
    </ol>
   </li>
   <li> Unsatisfiability for n=18:
    <ol>
     <li> OKsolver
      <ul>
       <li> Running it with
       \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D14 -F Ramsey_4_2_18.cnf
       \endverbatim
       seems to indicate that it will take roughly 15 days (on csltok). </li>
       <li> However, this seems to be too optimistic: The first 15 nodes
       at depth 14 (16384 nodes altogether) were solved all with around
       100,000 nodes in the subtrees, however with node 16 the solver
       falls into a deep hole, and yet 500,000,000 nodes have been processed
       there. </li>
       <li> A still deeper hole waited at node 19, and after 16.35 days and
       4,095,825,666 nodes (altogether; 3620490 single nodes, 16,220,739,545
       2-reductions, 22,409,527 pure literals, 0 autarkies; 1,412,661.8
       seconds) it was aborted. </li>
       <li> Interestingly it seems that cs-wsok here is quite a bit slower
       than csltok:
        <ol>
         <li>
         \verbatim
  1:110200,  208.1, 3408483.1
  2:100554,  418.0, 3423838.0
         \endverbatim
         but there is a lot of variability
         \verbatim
  1:110200,  161.6, 2647165.1
  2:100554,  302.3, 2476385.0
         \endverbatim
         (which is strange?) versus on csltok:
         \verbatim
  1:110200,   82.5, 1350942.2
  2:100554,  162.8, 1333822.4
         \endverbatim
         where actually csltok has a slower cpu?? </li>
         <li> After 3299m 19s, on csltok 487,747,031 nodes
         were processed, that makes 2460 nodes per second. </li>
         <li> After 254392.7 s, on cs-wsok 740,136,935 nodes
         where processed, that makes 2900 nodes per second. </li>
         <li> So it seems that cs-wsok was only initially that slow
         --- perhaps cache behaviour? But still, given that csltok
         runs with 1400 MHz and cs-wsok with 2100 MHz, executation
         on cs-wsok seems slow. </li>
        </ol>
       </li>
      </ul>
     </li>
     <li> Grasp aborts by default (10000s exceeded). </li>
     <li> minisat seems hopeless (after one hour). </li>
     <li> march_pl seems hopeless (after one hour). </li>
    </ol>
   </li>
  </ul>


  \todo NR([5,5],2)
  <ul>
   <li> Known is 43 <= NR([5,5],2) <= 49. </li>
   <li> So we get 1-2 million clauses; we can try with a local search
   solver. </li>
   <li> OKsolver can handle n=33 in ~ 3 minutes, but has problems with
   n=34. </li>
   <li> march_pl seems to have problems already for n=30. </li>
   <li> minisat can handle n=30 in 2 seconds; n=35 ? </li>
   <li> UnitMarch: n=30 in 3 seconds, but n=33 unsolved after 20 minutes.
   </li>
   <li> The list of algorithms from ubcsat:
    <ol>
     <li> "ubcsat-okl -alg gsat -runs 100 -i Ramsey_5_2_n.cnf": trivial for
     n=30; n = 35 needs 300 - 30000 steps. Using "-cutoff 2000" seems
     reasonable. </li>
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
       <li> cutoff = 10 000 000 and noimprove = 1 000 000: 100 rounds,
       maximum 3 500 000 steps, average 1 700 000 steps, minimum 7
       falsified clauses. </li>
       <li> cutoff = 20 000 000, noimprove = 3 000 000: 100 rounds, maximum
       10 500 000 steps, average 4 300 000 steps, minimum 1 falsified clause
       (seed 18745606). </li>
       <li> One round, cutoff = 20 000 000, with previous best seed: didn't
       improve the result. </li>
       <li> So let's try 500 runs, with cutoff = 20 000 000, noimprove =
       2 000 000:
       \verbatim
BestStep_Mean = 1121189.316000
Steps_Mean = 3121190.316000
Steps_Max = 7623900.000000
PercentSuccess = 0.00
BestSolution_Mean = 34.696000
BestSolution_Median = 35.000000
BestSolution_Min = 7.000000
BestSolution_Max = 76.000000
       \endverbatim
       </li>
       <li> Alright, with noimprove = 3 000 000:
        <ol>
         <li>
         \verbatim
BestStep_Mean = 1596309.594000
Steps_Mean = 4590310.592000
Steps_Max = 14784949.000000
PercentSuccess = 0.20
BestSolution_Mean = 33.976000
BestSolution_Median = 35.000000
BestSolution_Min = 0.000000
BestSolution_Max = 61.000000
         \endverbatim
         </li>
         <li> With seed = 391532901 a solution was found (in round
         293, using 964281 steps)!
          <ul>
           <li> The command line instruction to reproduce this is:
           \verbatim
ubcsat -alg samd -seed 391532901 -cutoff 1000000 -i Ramsey_5_2_40.cnf
           \endverbatim
           </li>
           <li> A solution is output by adding "-solve". </li>
           <li> Perhaps this solution should be stored; the question is
           where? Perhaps this is similar to data for minimally unsatisfiable
           clause-sets etc., which finally is too much to put into source
           control, but should exist as additional packages. </li>
           <li> Of course, there is the whole complex of "database
           questions"; see Experimentation/Database/plans/general.hpp
           <li> We need to find out whether this solution is of interest
           (i.e., "unknown") or not. </li>
           <li> So we need to find out about the known solutions for n=40
           in the literature, and about classifications of solutions (when
           solutions are considered to be the "same"). </li>
          </ul>
         </li>
         <li> So, in general perhaps one should just use a cutoff of
         1 000 000, and use more rounds? </li>
        </ol>
       </li>
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
       <li> One round, cutoff = 20 000 000, with previous best seed: didn't
       improve the result. </li>
       <li> So let's try cutoff = 20 000 000, noimprove = 3 000 000.
       \verbatim
> ubcsat-okl -alg samd -cutoff 20000000 -noimprove 3000000 -runs 100 -i Ramsey_5_5_2_41.cnf
       
       \endverbatim
       </li>
       <li> Removing the noimprove-value, actually a solution was found:
       \verbatim
> ubcsat-okl -alg samd -cutoff 20000000 -runs 100 -i Ramsey_5_5_2_41.cnf
      1 0    54     387633   20000000  451534902
      2 0    55    4031001   20000000 2277530380
      3 1     0   17699250   17699250  147704963
       \endverbatim
       Was this pure chance? (By the way, that was using ubcsat-1.1.0,
       and we should check whether the seed is the same for 1.0.0.) </li>
      </ol>
     </li>
     <li> n=42
      <ol>
       <li> On cscharon (64 bit, ubcsat version 1.1.0):
       \verbatim
> ubcsat-okl -alg samd -cutoff 20000000 -runs 100 -i Ramsey_5_5_2_42.cnf
       \endverbatim
       on 7 runs minimum was 77, maximum 86 falsified clauses.
       </li>
       <li> 500 runs with a cutoff 100000 yields between 83 and 107 falsified
       clauses: Initially it's very easy, but then improvement is very hard,
       and only very few runs are better --- but then they are much
       better, while the rest is very close together. We need better data:
        <ol>
         <li> cutoff = 1000, 5000 runs:
         \verbatim
Clauses = 1701336
Variables = 861
TotalLiterals = 17013360
FlipsPerSecond = 1938
BestStep_Mean = 875.772200
Steps_Mean = 1000.000000
Steps_Max = 1000.000000
PercentSuccess = 0.00
BestSolution_Mean = 130.212200
BestSolution_Median = 130.000000
BestSolution_Min = 108.000000
BestSolution_Max = 160.000000
         \endverbatim
         </li> cutoff = 10000, 5000 runs: (cs-wsok)
         \verbatim
Clauses = 1701336
Variables = 861
TotalLiterals = 17013360
FlipsPerSecond = 2276
BestStep_Mean = 7167.124200
Steps_Mean = 10000.000000
Steps_Max = 10000.000000
PercentSuccess = 0.00
BestSolution_Mean = 106.278800
BestSolution_Median = 106.000000
BestSolution_Min = 85.000000
BestSolution_Max = 126.000000
         \endverbatim
         </li>
         <li> Alternatively with gsat-tabu:
         \verbatim
> ubcsat-okl -alg gsat-tabu -i Ramsey_5_5_2_42.cnf -runs 5000 -cutoff 10000
Clauses = 1701336
Variables = 861
TotalLiterals = 17013360
FlipsPerSecond = 1721
BestStep_Mean = 7232.085000
Steps_Mean = 10000.000000
Steps_Max = 10000.000000
PercentSuccess = 0.00
BestSolution_Mean = 105.163400
BestSolution_Median = 105.000000
BestSolution_Min = 85.000000
BestSolution_Max = 124.000000
         \endverbatim
         </li>
         Might be slightly better, but seems to be quite a bit slower. </li>
         <li> cutoff = 100,000, 5000 runs: (cs-wsok)
         \verbatim
Clauses = 1701336
Variables = 861
TotalLiterals = 17013360
FlipsPerSecond = 2010
BestStep_Mean = 59262.030800
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 94.735400
BestSolution_Median = 95.000000
BestSolution_Min = 82.000000
BestSolution_Max = 111.000000
         \endverbatim
         only a bit progress.
         </li>
         <li> Alternatively with gsat-tabu:
         \verbatim
Clauses = 1701336
Variables = 861
TotalLiterals = 17013360
FlipsPerSecond = 1646
BestStep_Mean = 59331.269400
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 93.784200
BestSolution_Median = 94.000000
BestSolution_Min = 75.000000
BestSolution_Max = 143.000000
         \endverbatim
         might have a higher variability? </li>
         <li> cutoff = 1,000,000, 500 runs: (cscarme)
         \verbatim
Clauses = 1701336
Variables = 861
TotalLiterals = 17013360
FlipsPerSecond = 2135
BestStep_Mean = 477504.9
Steps_Mean = 1000000
/compsci/saturn/staff/csoliver/OKplatform/bin/ubcsat-okl: line 12: 26989 Segmentation fault
         \endverbatim
         where the data was then analysed using R, copying the output to
         a file "ExpRamsey1000000" starting like
         \verbatim
sat min optsteps maxsteps seed
      1 0    90     153638    1000000 1649159937
      2 0    92     284850    1000000 1886480441
      3 0    86     636866    1000000 2139335150
         \endverbatim
         and where then analysis is very simple, %e.g.
         \verbatim
R1 = read.table("ExpRamsey1000000")
> min(R1$min)
[1] 77
> max(R1$min)
[1] 106
> median(R1$min)
[1] 88
> mean(R1$min)
[1] 87.8
         \endverbatim
         we see, again very little progress.
         </li>
         <li> cutoff = 10,000,000, 500 runs (cs-wsok) </li>
         <li> cutoff = 40,000,000, 50 runs (cscharon) </li>
        </ol>
       </li>
      </ol>
     </li>
     <li> n=43:
      <ol>
       <li> Also if this is unsatisfiable, one would only expect gradual
       differences compared to n=42 for local-search algorithms. </li>
       <li> cutoff = 10,000,000, 50 runs (csoberon)
       \verbatim
Clauses = 1925196
Variables = 903
TotalLiterals = 19251960
FlipsPerSecond = 1947
BestStep_Mean = 4888045.6
Steps_Mean = 10000000
/compsci/saturn/staff/csoliver/OKplatform/bin/ubcsat-okl: line 12:  9651 Segmentation fault
       \endverbatim
       and furthermore
       \verbatim
> min(R2$min)
[1] 112
> max(R2$min)
[1] 126
> median(R2$min)
[1] 117.5
> mean(R2$min)
[1] 117.38
       \endverbatim
       </li>
       <li> cutoff = 100,000,000, 50 runs (csoberon) </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo NR([6,6],2)
  <ul>
   <li> Known is 102 <= NR([6,6],2) <= 165. </li>
   <li> These clause-sets are too big (for direct representation). </li>
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
   of R5 on R5SAT (what are the really different solutions?).
    <ol>
     <li> For this we need to compute the automorphism group of R5; see
     "Automorphisms of Ramsey clause-sets" in
     RamseyTheory/Lisp/Ramsey/plans/general.hpp. </li>
     <li> One solution (as graph; see "Representing solutions as graphs" below)
     is the C^5, and we have all hamiltonian cycles of the K_5 as similar
     solutions under the operation of the S_5: This yields 4! / 2 = 12
     solutions, so actually all solutions are isomorphic to C^5. </li>
     <li> We need to divide by 2 here since we are considering "cyclic
     permutations" (so that [1,2,3,4,5] is the same as [1,5,4,3,2]). </li>
    </ol>
   </li>
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
   <li> See "Symmetry breaking" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/RamseyProblems.hpp.
   </li>
   <li> We should try to figure out the automorphism groups of the clause-sets.
   See "Automorphisms of Ramsey clause-sets" in
   RamseyTheory/Lisp/Ramsey/plans/general.hpp. </li>
   <li> The goal is to find as many as possible assignments which can
   be made "w.l.o.g."; and also additional short clauses are of interest. </li>
  </ul>


  \todo Representing solutions as graphs
  <ul>
   <li> A solution for the parameter-values ([[p,q],2]; n) (that is,
   a labelling of the K_n with colours 0,1 such that no clique of size p
   of colour 0 and no clique of size q of colour 1 exists), corresponds to
   a graph with n vertices not containing a clique of size p or an independent
   set of size q. </li>
   <li> This graph representation brings with it a notion of isomorphic
   solutions by using graph isomorphisms. </li>
   <li> If p = q, then as an additional symmetry we have the correspondence
   between a graph and its complement. </li>
   <li> The underlying reasoning is as follows:
    <ol>
     <li> We consider the general problem of when two solutions of a
     satisfiability problem are to be considered "similar". </li>
     <li> The strongest possibility (detecting the most similarities) seems
     to use the automorphism group of the set of all (total) solutions,
     and then to consider two solutions as similar iff they are in the
     same orbit under this operation (i.e., there is an automorphism turning
     the one solution into the other). </li>
     <li> This considers only the solution space, not the representation. </li>
     <li> Perhaps most common is to consider the automorphism group of the
     representation (and its operation), which yields a subgroup of the
     automorphism group of the solutions. </li>
     <li> For the Ramsey problems presented as clause-sets we know the
     automorphisms given by S_n, and S_n x ZZ_2 for symmetric problems. </li>
     <li> In the graph-case we are considering here this yields exactly
     to consider two solutions as similar iff their graphs are isomorphic,
     where for symmetric problems we consider also a graph and its
     complement as similar. </li>
    </ol>
   </li>
   <li> For the %Ramsey parameter tuple [[3,3],2] and n = 5 we have the
   solution C^5 (the cycle with 5 edges), which is self-dual (and this
   is the only solution; see above). </li>
   <li> We should have an extensive catalogue as possible for the known
   solution types (if possible, augmented by the information how many
   solutions in total they represent). </li>
  </ul>


  \todo Self-dual solutions
  <ul>
   <li> For the Ramsey parameter tuple [[3,3],2] we have a self-dual
   solution (a graph isomorphic to its complement). </li>
   <li> Are there self-dual solutions for all [[p,p],2] ?? </li>
   <li> We need to check the known cases! </li>
   <li> For [[5,5],2] and n=42 the number of edges (861) of K_n is odd, so
   if the conjecture NR([[5,5],2]) = 43 is true, then there would be
   no self-dual solution here. </li>
  </ul>


  \todo "Visualising" solutions
  <ul>
   <li> Via SAT solvers we can compute certain solutions for problems somewhat
   smaller than the interesting sizes. </li>
   <li> The task is to "look" at these solutions, extract some structure, and
   then to systematically search for "such" solutions. </li>
   <li> Of course, everything theoretically known needs to be explored. </li>
   <li> From the practical side, there has been work (e.g., Marijn Heule) for
   van der Waerden problems. </li>
   <li> See "Representing solutions as graphs" above; perhaps that's the
   best what we can do for those problems? But if we know more about the
   symmetries between solutions (i.e., the automorphism group of the solution
   space, then we might obtain further compression. </li>
  </ul>


  \todo Better SAT solvers
  <ul>
   <li> These problems seem to be quite structured, so that quite something
   is known (at least more than for Van der Waerden numbers), but SAT solvers
   apparently can't exploit any of it. </li>
   <li> What about constraint solvers? What do they use, and how succesful
   are they? </li>
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
     would be very natural to apply it here:
      <ol>
       <li> The clause-set F_R([q_1, ..., q_s], r, n) can be expressed then
       by just s clauses together with the symmetric group S_n. </li>
       <li> However it is not available. </li>
       <li> One should have a look at those papers. </li>
       <li> A point here is that S_n does act faithful but not
       strongly faithful on the clauses (i.e., on the underlying hypergraph)
       --- can this be repaired or improved (S_n is a rather large group here)?
       </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo Understanding local search algorithms
  <ul>
   <li> First one needs to establish which algorithm from the Ubcsat
   collection performs best (hopefully sooner or later we can use
   also version 1.1.0); see above. </li>
   <li> For this best algorithm we should also try to optimise the
   parameters. </li>
   <li> Important to study the search landscapes; see "Examing search
   landscapes" in
   ComputerAlgebra/Satisfiability/Lisp/LocalSearch/plans/general.hpp. </li>
   <li> The goal is at least to understand why the best algorithm is best,
   and perhaps to further improve this algorithm. </li>
  </ul>

*/

