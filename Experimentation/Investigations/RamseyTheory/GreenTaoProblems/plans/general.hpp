// Oliver Kullmann, 14.11.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/general.hpp
  \brief On investigations into Green-Tao problems (including Green-Tao numbers)


  \todo Prepare benchmarks for SAT 2011
  <ul>
   <li> See "Benchmarks for SAT 2011" in
   Experimentation/Investigations/plans/SATCompetition.hpp for the general
   plans. </li>
   <li> See "SAT 2011 competition" in
   RamseyTheory/GreenTaoProblems/plans/GreenTao_2-3-k.hpp for the solver
   performances. </li>
   <li> DONE
   For now we only do the naturally boolean problems. </li>
   <li> DONE
   README file:
    <ol>
     <li> DONE
     Yet missing the links to the OKlibrary. </li>
    </ol>
   </li>
   <li> DONE (README is enough) Pdf file: </li>
  </ul>


  \todo Standardisation on notation : DONE
  <ul>
   <li> For Green-Tao and for Van-der-Waerden numbers we use the following
   notation from now on. </li>
   <li> The number of parts is the index of the function name ("greentao"
   or "vanderwaerden"). </li>
   <li> We always write out this index. </li>
   <li> For diagonal problems, we have then exactly one argument, the length
   of the arithmetic progression in each part, and we add "d" to the
   function-name. </li>
   <li> While for non-diagonal problems we have as many arguments as the
   index, each the length of the arithmetic progression in the corresponding
   part. </li>
  </ul>


  \todo Trivial Green-Tao numbers : DONE (handling this elsewhere)
  <ul>
   <li> greentao_p(k) for natural numbers p,k >= 0 is the smallest natural
   number n >= 0 such that partitioning the first n prime numbers into p
   parts is guaranteed to contain an arithmetic progression of size k
   in some part.
    <ol>
     <li> greentao_2(0) = 0 </li>
     <li> greentao_2(1) = 1 </li>
     <li> greentao_2(2) = 3 </li>
    </ol>
   </li>
   <li> greentao_2(k1,k2) for k1,k2 >= 0 is the smallest natural number
   n >= 0 such that partitioning the first n prime numbers into 2 parts is
   guaranteed to contain either an an arithmetic progression of size k1
   in the first part or an an arithmetic progression of size k2 in the
   second part.
    <ol>
     <li> greentao_2(0,k) = 0 </li>
     <li> greentao_2(1,k) is the smallest n such that the first n prime numbers
     contain an arithmetic progression of size k: see "Finding the first
     arithmetic progression" in
     Investigations/RamseyTheory/GreenTaoProblems/plans/AdditiveNumberTheory.hpp
     . </li>
     <li> greentao_2(2,k) is the smallest n such that after removing any
     prime from the first n prime numbers we always have an arithmetic
     progression of size k. See "The first arithmetic progression allowing a
     missing number" in
     Investigations/RamseyTheory/GreenTaoProblems/plans/AdditiveNumberTheory.hpp
     . </li>
    </ol>
   </li>
  </ul>


  \todo greentao_2(3) = 23
  <ul>
   <li> greentao_2(3) = 23 (partitioning the first 23 prime numbers into 2
   parts, one part is guaranteed to contain an arithmetic progression of
   size 3, while using a smaller initial segment of prime numbers won't do).
   </li>
   <li> Trivial (for OKsolver-2002, and likely for any solver). </li>
  </ul>


  \todo DONE (the basic algorithm works not too bad now)
  Better creation of problems
  <ul>
   <li> For n in this magnitude the Maxima computation of the hypergraph is
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
  </ul>


  \todo Best local search solver : DONE (there is no general best algorithm)
  <ul>
   <li> From the ubcsat-suite rnovelty+ seems best. </li>
   <li> See what's in ubcsat-1.1.0. </li>
   <li> And then we should try walksat from Kautz, which has several
   options (amongst them rnovelty+).
    <ol>
     <li> Again, rnovelty+ seems far best. </li>
    </ol>
   </li>
  </ul>


  \todo Connecting different n
  <ul>
   <li> We should find out what the falsified clause for the above nearly
   satisfying assignment for n=33000 is --- if m is the maximum variable
   (index) in the clause then we have a satisfying assignment for n = m-1.
    <ol>
     <li> This holds in general for such monotone sequences of clause-sets.
     </li>
     <li> We should write a little C++ program, which takes the assignment
     returned by Ubcsat (output by using option "-r best") and the clause-set,
     and outputs the falsified clauses. </li>
    </ol>
   </li>
  </ul>


  \todo Phase transition
  <ul>
   <li> One should study the density of the clause-sets (and the "threshold")
   here.
    <ol>
     <li> The density 3.5 for unsatisfiable k=4 is somewhat
     similar to the random 3-SAT threshold (around 4.25 --- though for
     larger n). </li>
     <li> The density 8.8 for unsatisfiable k=4 is similar to
     the random 4-SAT threshold (around 9.8). </li>
     <li> The random 5-SAT threshold is around 20. </li>
     <li> One could guess that the unsatisfiability-density comes closer
     to the random-k-SAT threshold density? </li>
     <li> Then one needed to figure out how many k-progressions are in the
     first n primes; see "The distribution of arithmetic progression amongst
     primes" in Experimentation/Investigations/plans/AdditiveNumberTheory.hpp.
     </li>
     <li> It would be interesting to study random complement-invariant
     k-SAT clause-sets (choose a random k-clause-set, and take the union
     with the complement)! </li>
     <li> I (OK) would assume that the van-der-Waerden clause-sets are much
     more redundant than the Gree-Tao clause-sets, and that the latter are
     much closer to random clause-sets. </li>
     <li> A general conjecture is that we have the Ramsey property if the
     density goes to infinity for each fixed size k of the structure required
     to exist (arithmetic progressions, cliques, etc.; in this general
     form likely one should find a counter-example, but perhaps it holds
     if the the structures "spread a bit"). </li>
    </ol>
   </li>
  </ul>


  \todo Faster generation of arithmetic progression of primes
  <ul>
   <li> A major bottleneck is the time needed to create Green-Tao problems.
   </li>
   <li> Via local search we might even investigate greentao_2(6), but here
   n might go into the millions, and we need a much faster generator. </li>
   <li> In Satisfiability/Transformers/Generators/GTSat and GTdSat we have
   C++ generators for boolean problems and for the direct translation for
   non-boolean problems. </li>
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
  </ul>


  \todo Faster local search
  <ul>
   <li> For greentao it seems the only structure which can be exploited is
   the complement-invariance.
    <ol>
     <li> More precisely, we have a hypergraph colouring problem. </li>
     <li> So we have complement-invariant clause-sets, and furthermore we
     have a PN-clause-set (so regarding space, we can save one half of the
     clauses, and the clauses need only contain positive literals, i.e.,
     variables, not literals). </li>
     <li> It seems desirable to have a specialised local search solver for
     hypergraph colouring (as in instance of generalised SAT); since local
     search solvers only use total assignments, the non-stability of
     hypergraph colouring under partial assignments is no hindrance (while
     we have stability under autarkies). </li>
     <li> The more colours are to be used, the bigger the savings. </li>
     <li> And one would assume the various heuristics are influenced by
     the restriction to hypergraph colouring. </li>
     <li> First we should only implement adaptnovelty+ and rnovelty+, and
     this first at the Maxima/Lisp level, followed directly by a  C++
     implementation; see
     ComputerAlgebra/Satisfiability/Lisp/LocalSearch/plans/HypergraphColouring.hpp.
     </li>
    </ol>
   </li>
   <li> For vanderwaerden there is much more structure which could be exploited
   (using "virtual" clause-sets). </li>
   <li> We should try to understand why the different local search algorithms
   behave so differently on the various problem classes.
    <ol>
     <li> See chapter 6 in [Hoos, Stuetzle, Stochastic Local Search] for
     background information on the algorithms involved. </li>
     <li> For the van der Waerden problems and the Green-Tao problems it should
     be possible to gain quite good quantitative experimental understanding.
     </li>
     <li> See chapter 4 in [Hoos, Stuetzle, Stochastic Local Search] for
     material on statistical evaluation. </li>
    </ol>
   </li>
   <li> "Meta-heuristics":
    <ol>
     <li> General meta-heuristics are needed, which can be adapted to the
     specific problems. </li>
     <li> A natural first example would be first to identify the best solver
     from the suite, then trying to optimise it, and then search for solutions
     by starting with, say (just an example) 1000 seeds, running them a bit,
     filtering out the 100 most promising ones, running them further, filtering
     out the 10 best, running them, finally filtering out the best one (or more
     --- depending on the number of processes to be run!). </li>
     <li> Of course, this all automatic (with good monitoring). </li>
     <li> One needs to gain quantitative understanding of the local search
     process, so that progress can be evaluated; see above. </li>
     <li> All algorithms and programs are written in a natural generative
     style, but specific to the problem set (van der Waerden and Green-Tao
     problems here --- even them treated individually). </li>
     <li> Perhaps the whole thing is written in R first, using Ubcsat; see
     ExperimentSystem/ControllingLocalSearch/plans/general.hpp. </li>
     <li> And (of course) also at the Maxima/Lisp level, this time using
     Maxima/Lisp local search algorithms; see
     ComputerAlgebra/Satisfiability/Lisp/LocalSearch/plans/general.hpp. </li>
    </ol>
   </li>
  </ul>


  \todo Survey propagation
  <ul>
   <li> If the Green-Tao problems are similar to random problems, then
   perhaps survey propagation is also successful on them ?! </li>
   <li> Survey propagation is successful for greentao_2(5,5) (see
   Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_2-5-5.hpp)
   but yet we found no other problems. </li>
   <li> Could it be a question of size (the bigger the better for SP)?
   see greentao_2(3,7) (in
   Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_2-3-k.hpp).
   </li>
  </ul>


  \todo Literature
  <ul>
   <li> To search for literature, we can search on the Internet for the
   sequence (1,3,23,512) (greentao_2(i) for i=1,2,3,4). </li>
   <li> Likely this sequence is not in that Internet database, and
   we should submit it (once our article has appeared; or perhaps
   the report is enough). </li>
   <li> Actually there is sequence A134050: 1, 1, 3, 23, 512, 34939 ?!? </li>
   <li> For index 0 this sequence has value 1, while we have value 0; it
   seems unbelievable that this rather easily calculated sequence should
   coincide with greentao_2(i). </li>
   <li> We can also investigate greentao_2(i,i+1) fuer i >= 1; the values
   we know are
    <ol>
     <li> greentao_2(1,2) = 2 </li>
     <li> greentao_2(2,3) = 7 </li>
     <li> greentao_2(3,4) = 79 </li>
    </ol>
    There is A128293 with first five values 1, 2, 7, 79, 4235 ?!? </li>
    <li> Again, for i=0 the first value for us would be 0; and again it seems
    unbelievable that this sequence should have something to do with ours.
    </li>
  </ul>

*/

