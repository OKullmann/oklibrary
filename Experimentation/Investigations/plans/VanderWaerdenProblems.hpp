// Oliver Kullmann, 20.9.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/VanderWaerdenProblems.hpp
  \brief On investigations into van-der-Waerden-like problems


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/general.hpp. </li>
   <li> See VanderWaerden.cpp (to be updated) for a C++ generator. </li>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/plans/VanderWaerdenProblems.hpp.
   </li>
  </ul>


  \todo Van der Waerden numbers
  <ul>
   <li> Via
   \verbatim
VanderWaerden-O3-DNDEBUG k n > VanderWaerden_2_k_n.cnf
   \endverbatim
   boolean problems can be generated quickly. </li>
   <li> In Maxima we have "output_vanderwaerden2_stdname(k,n)" (in
   ComputerAlgebra/Satisfiability/Lisp/Generators/VanderWaerdenProblems.mac. </li>
   <li> vanderwaerden(2,3) = 9 (density = 3.6) </li>
   <li> vanderwaerden(2,4) = 35 (density = 10.7) </li>
   <li> vanderwaerden(2,5) = 178 (density = 43.5)
    <ol>
     <li> Relatively easy for OKsolver-2002 (710s, 109703 nodes). </li>
     <li> march_pl needs only 51529 nodes and 238 seconds, so the "double
     look-ahead" seems to be successful here. </li>
     <li> minisat needs 213 seconds, so also clause-learning seems applicable
     here. </li>
    </ol>
   </li>
   <li> vanderwaerden(2,6) = 1132 (density = 225.6) </li>
    <ol>
     <li> n = 1000
      <ul>
       <li> ubcsat-irots seems best. </li>
       <li> But finding a solution seems hopeless; less than 1400 falsified
       clauses seems to require billions of steps --- the difference between
       100,000 and 10,000,000 steps is very small. </li>
       <li> We need to look into the Heule-et-al article. </li>
      </ul>
     </li>
     <li> n = 1132
      <ol>
       <li> OKsolver-2002
       \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D30 VanderWaerden_2_6_1132.cnf
       \endverbatim
       looks hopeless (no node of depth 30 solved in one hour). </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo Symmetry breaking for van der Waerden numbers
  <ul>
   <li> As for any hypergraph colouring problems, one can choose any variable
   and set it to true (or to false), due to the symmetry between 0 and 1. </li>
   <li> This is best "told" the solver, so that it can use this for the
   branching variable at the root. </li>
   <li> No other symmetry breaking seems possible without conditioning. </li>
   <li> For small problems one needs to determine the full automorphism group
   of the clause-sets. </li>
  </ul>


  \todo Overview on performance of SAT solvers
  <ul>
   <li> We need an overview how SAT solvers perform on the different van der
   Waerden problems. </li>
  </ul>


  \todo Green-Tao numbers
  <ul>
   <li> greentao(2,3)
    <ol>
     <li> greentao(2,3) = 23 (partitioning the first 23 prime numbers into 2
     parts, one part is guaranteed to contain an arithmetic progression of
     size 3, while using a smaller initial segment of prime numbers won't do).
     </li>
     <li> Trivial (for OKsolver-2002, and likely for any solver). </li>
    </ol>
   </li>
   <li> greentao(2,4)
    <ol>
     <li> greentao(2,4) > 400 (trivial for OKsolver-2002). </li>
     <li> greentao(2,4) > 420 with 6683 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 430 with 22267 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 440 with 77791 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 450 with 349914 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 460 with 12777 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 470 with 59506 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 471 with 58889 nodes (OKsolver-2002). </li>
     <li> greentao(2,4) > 472 with 370222 nodes (OKsolver-2002). </li>
     <li> For this n, rnovelty finds a solution quickly (~ 100000 steps).
     </li>
     <li> n = 473: stopped after 884587 nodes (OKsolver). </li>
     <li> But found satisfiable with rnovelty. </li>
     <li> And rnovelty+ is even better. </li>
     <li> n = 480: stopped after 1536394 nodes (OKsolver). </li>
     <li> n = 500: Running it with
     \verbatim
OKsolver_2002-O3-DNDEBUG -M -D18 -F GreenTao_2_4_500.cnf
     \endverbatim
     found a solution after 11 hours (38,937,288 nodes; roughly 2% of the
     search space, looking at depth 18). </li>
     <li> Found satisfiable with rnovelty+. </li>
     <li> n = 510 found satisfiable with rnovelty+. </li>
     <li> n = 511 found satisfiable with rnovelty+. </li>
     <li> n = 512: rnovelty+ yields constantly 1 falsified clause
     (also with "-runs 500 -cutoff 10000000"). </li>
     <li>
     \verbatim
OKplatform> OKsolver_2002-O3-DNDEBUG -M -D16 GreenTao_2_4_512.cnf
     \endverbatim
     seems to need ~ 6 days (csltok). </li>
     <li> n = 515: rnovelty+ yields constantly 1 falsified clause. </li>
     <li> n = 520: rnovelty+ yields constantly 1 falsified clause. </li>
     <li> n = 530: rnovelty+ yields constantly 1 falsified clause. </li>
     <li> n = 540: rnovelty+ yields constantly 1 falsified clause. </li>
     <li> n = 550: rnovelty+ yields constantly 2 falsified clauses. </li>
     <li> n = 600: rnovelty+ yields constantly 6 falsified clauses. </li>
     <li> Minisat looks weak on these instances. </li>
     <li> And Grasp likely doesn't work here neither. </li>
     <li> But march_pl seems stronger on the satisfiable instances than
     OKsolver-2002, though n=500 seems also difficult. </li>
     <li> DONE (the basic algorithm works not too bad now)
     For n in this magnitude the Maxima computation of the hypergraph is
     already very slow --- a more intelligent algorithm for finding the
     arithmetic progression amongst the prime numbers is needed (likely we
     cannot exploit the speciality of prime numbers, but we do it for arbitrary
     lists of numbers).
      <ol>
       <li> DONE
       One could use memoisation in the form that for every n we store
       the additional hyperedges (k-progressions). </li>
       <li> DONE (doesn't improve a single computation, but several)
       This would it make rather quick, without imposing big memory
       burdens. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> greentao(2,5)
    <ol>
     <li> Trivial for n=5000:
     \verbatim
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_5_5000.cnf
Clauses = 26470
Variables = 5000
TotalLiterals = 132350
FlipsPerSecond = 254850
BestStep_Mean = 509.700000
Steps_Mean = 509.700000
Steps_Max = 538.000000
PercentSuccess = 100.00
     \endverbatim
     Density = 5.294
     </li>
     <li> Also OKsolver-2002 solves it in 2157 nodes. </li>
     <li> Also very easy for march_pl. </li>
     <li> Still trivial for n=10000:
     \verbatim
> ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_5_10000.cnf
Clauses = 85596
Variables = 10000
TotalLiterals = 427980
FlipsPerSecond = 126621
BestStep_Mean = 1772.700000
Steps_Mean = 1772.700000
Steps_Max = 1912.000000
PercentSuccess = 100.00
     \endverbatim
     Density = 8.5596
     </li>
     <li> Also OKsolver-2002 solves it in 4089 nodes. </li>
     <li> march_pl has more problems, but also solves it in
     about a minute. </li>
     <li> Still trivial for n=15000:
     \verbatim
ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_5_15000.cnf
Clauses = 170830
Variables = 15000
TotalLiterals = 854150
FlipsPerSecond = 85313
BestStep_Mean = 4521.600000
Steps_Mean = 4521.600000
Steps_Max = 4785.000000
PercentSuccess = 100.00
     \endverbatim
     Density = 11.38866666666667
     </li>
     <li> For OKsolver-2002 it might be hard (or one has to wait a bit longer,
     since processing of the nodes now takes quite some time). </li>
     <li> march_pl is at least quicker, and finds a solution rather fast (9091
     nodes). Perhaps the difference is just that there are (nearly) no
     2-reductions possible, where march_pl reacts to this and gets quicker,
     while OKsolver-2002 always checks all possibilities. </li>
     <li> And yes, that's the case: OKsolver-2002 solves it with 4634 nodes,
     without backtracking, and with just 17 2-reductions. </li>
     <li> Still trivial for n=20000:
     \verbatim
ubcsat-okl -alg rnovelty+ -runs 10 -cutoff 10000000 -i GreenTao_2_5_20000.cnf
Clauses = 279592
Variables = 20000
TotalLiterals = 1397960
FlipsPerSecond = 115071
BestStep_Mean = 25775.900000
Steps_Mean = 25775.900000
Steps_Max = 80685.000000
PercentSuccess = 100.00
     \endverbatim
     Density = 13.9796
     </li>
    </ol>
   </li>
   <li> One should study the density of the clause-sets (and the "threshold")
   here.
    <ol>
     <li> The density 3.5 for unsatisfiable k=4 is somewhat
     similar to the random 3-SAT threshold (around 4.25 --- though for
     larger n). </li>
     <li> The (assumed) density 8.8 for unsatisfiable k=4 is similar to
     the random 4-SAT threshold (around 9.8). </li>
     <li> The random 5-SAT threshold is around 20. </li>
     <li> One could guess that the unsatisfiability-density comes closer
     to the random-k-SAT threshold density? </li>
     <li> Then one needed to figure out how many k-progressions are in the
     first n primes. </li>
     <li> It would be interesting to study random complement-invariant
     k-SAT clause-sets (choose a random k-clause-set, and take the union
     with the complement)! </li>
     <li> I (OK) would assume that the van-der-Waerden clause-sets are much
     more redundant than the Gree-Tao clause-sets, and that the latter are
     much closer to random clause-sets. </li>
    </ol>
   </li>
   <li> One can also consider non-diagonal Green-Tao numbers. </li>
   <li> One needs to investigate whether bounds are known. </li>
  </ul>


  \todo Faster generation of arithmetic progression of primes
  <ul>
   <li> A major bottleneck is the time needed to create Green-Tao problems.
   </li>
   <li> Via local search we might even investigate greentao(2,6), but here
   n might go into the millions, and we need a much faster generator. </li>
   <li> In Transitional/Structures/NumberTheory a C++ program should be
   written with the functionality of arithprog_primes.
    <ol>
     <li> Perhaps we only output this hypergraph as a positive clause-set,
     in DIMACS format, into a file. </li>
     <li> Some components should go to Transitional/Combinatorics/Hypergraphs.
     </li>
     <li> Another program then standardises the variable names and adds the
     complemented clauses (producing then the same output as yet
     output_greentao2_stdname(k,n)). </li>
     <li> Such functionality shall likely go to
     Transitional/Satisfiability/Transformers. </li>
     <li> Best we provide this functionality as Unix tools as well as at
     library level. </li>
    </ol>
   </li>
   <li> And also the sequences length(arithprog_primes_finish[k,n]) for fixed k
   and length(arithprog_primes(k,n)) for fixed k should be of interest.
    <ol>
     <li> Shall this go into a PostgreSQL database, or into a simple file,
     containing lines
     "no., prime, count of sequences ending with prime, cumulative count".
     ? The file looks alright (and can be easily expanded). </li>
     <li> We should also provide column headings, so that it can be directly
     read into R. </li>
     <li> But also Maxima should have no problems reading these files. </li>
     <li> These files need to be provided in a data section of the OKlibrary.
     </li>
    </ol>
   </li>
   <li> Enumerating the primes:
    <ol>
     <li> Gmp has a "next_prime" function, by which we first create the complete
     list of primes and the corresponding boolean array (for the primality
     predicate). </li>
     <li> Optionally, if one of the randomised tests is not secure, then the
     whole computation is checked via a simple sieve of Erathostenes. </li>
    </ol>
   </li>
  </ul>


  \todo Faster local search
  <ul>
   <li> For greentao it seems the only structure which can be exploited is
   the complement-invariance. </li>
   <li> For vanderwaerden there is much more structure which could be exploited
   (using "virtual" clause-sets). </li>
   <li> We should try to understand why the different local search algorithms
   behave so differently on the various problem classes. </li>
  </ul>

*/

