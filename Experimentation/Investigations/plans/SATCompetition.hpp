// Oliver Kullmann, 28.11.2010 (Swansea)
/* Copyright 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/SATCompetition.hpp
  \brief Plans regarding SAT competitions


  \todo Update and completion of plans regarding SAT 2011
  <ul>
   <li> The SAT competitions are nearly finished now (except of the PB
   evaluation, which will be repeated). </li>
   <li> The benchmarks were created, submitted, and are documented in
   Experimentation/Benchmarks/docus/SAT2011.hpp. </li>
   <li> We need the results of the competition on our instances available in
   the library. </li>
   <li> We should also provide full documentation of the AES instances
   as a LaTeX document with a description of the cipher. This should fit in
   with the generation of instances for SAT2012, discussed in "Benchmarks for
   SAT 2012". </li>
   <li> All results of the competition should be made available (in edited
   form, of course) on our benchmark sites. </li>
   <li> Also links to competition pages are needed there. </li>
   <li> Plus general comments and explanations. </li>
   <li> The successful solvers need to be installed. </li>
   <li> All todos need to be cleaned-up; most of them removed (possible
   with transfer to docus). </li>
  </ul>


  \todo Benchmarks for SAT 2012
  <ul>
   <li> The deadline for:
    <ol>
     <li> SAT Challenge benchmark submission is 11/4/2012;
     http://baldur.iti.kit.edu/SAT-Challenge-2012/submission.html . </li>
     <li> PseudoBoolean registration is 13/4/2012;
     PseudoBoolean benchmark submission is 20/4/2012;
     http://www.cril.univ-artois.fr/PB12/ . </li>
     <li> MaxSAT evaluation submission is 23/3/2012;
     http://maxsat.ia.udl.cat/introduction/ . </li>
     <li> DONE (nothing to submit)
     QBF evaluation registration is Wednesday 29th February 2012. </li>
     <li> DONE (nothing to submit)
     QBF benchmark submission is Thursday 15th March 2012. </li>
    </ol>
   </li>
   <li> MG has registered for both the SAT challenge and
   PseudoBoolean competition (so only the submission deadlines are
   relevant). </li>
   <li> The specifications of each of the formats can be found at:
    <ul>
     <li> SAT benchmarks -
     http://www.satcompetition.org/2011/format-benchmarks2011.html
     (the 2012 rules at
     http://baldur.iti.kit.edu/SAT-Challenge-2012/rules.html refer to 2011).
     </li>
     <li> PseudoBoolean benchmarks -
     http://www.cril.univ-artois.fr/PB12/format.pdf . </li>
     <li> MaxSAT benchmarks - http://maxsat.ia.udl.cat/rules/ and
     http://maxsat.ia.udl.cat/requirements/ . </li>
     <li> QBFeval benchmarks - http://www.qbflib.org/qdimacs.html.
     </li>
    </ul>
   </li>
   <li> The categories for SAT 2012, the PseudoBoolean and MaxSAT competitions
   are (up to trivial renaming) the same as in 2011, discussed in
   'Categories / "tracks" for SAT 2011' below. </li>
   <li> Best in every series to have a mixture, from known to unknown
   instances, from not-too-hard to likely-very-hard. So for each series
   we need:
    <ul>
     <li> an "easy" instance taking around 1 minute to solve; </li>
     <li> several "medium-easy" instances taking 5-15 minutes to solve; </li>
     <li> several "medium" instances taking 15-60 minutes to solve; </li>
     <li> one or two "medium-hard" instances taking 60-180 minutes to solve; </li>
     <li> a "hard" instance taking a day or more to solve. </li>
    </ul>
   </li>
   <li> The SAT Challenge 2012 requires a 1 to 2 page (max 2) PDF document
   in the IEEE proceeding style, available at
   http://www.ieee.org/conferences_events/conferences/publishing/templates.html .
   These descriptions will be posted on the website. </li>
   <li> DONE
   We should consider the format, and what instances we can provide
   for the QBF evaluation. </li>
   <li> For the formats see below; it should be checked that these haven't
   changed from 2011. </li>
   <li> For the categories see below;  it should be checked that these haven't
   changed from 2011. </li>
  </ul>


  \todo Categories / "tracks" for SAT 2011
  <ul>
   <li> This todo should be reviewed and moved into the docus, with
   up to date links for SAT2012. </li>
   <li> SAT competition:
    <ul>
     <li> <em>Random uniform k-SAT</em> instances where only generators are
     allowed. </li>
     <li> <em>Applications</em> where instances of industrial and practical
     interest are required.
     </li>
     <li> <em>Crafted</em> where instances designed to be hard for SAT solvers,
     representing difficult puzzles, mathematical problems etc are required.
     One should specify whether these instances are SAT or UNSAT. </li>
    </ul>
   </li>
   <li> PseudoBoolean competition:
    <ul>
     <li> Pseudo Boolean Satisfaction, <em>PBS</em>, which involves deciding
     satisfiability for a set of PB constraints. </li>
     <li> Pseudo Boolean Optimisation, <em>PBO</em>, which involves finding a
     model which optimises a given objective function. </li>
     <li> Weighted boolean optimisation, <em>WBO</em> where pseudo boolean
     constraints have weights and then a solution must be
     found which minimises the sum of the weight of the falsified
     constraints. </li>
     <li> For PBS and PBO, there are the following categories:
      <ul>
       <li> DEC - pure decision problems, where the solver must only
       find a solution but there is no objective function, and no weights.
       </li>
       <li> OPT - an objective function is present which must be
       optimised but no weights are given to constraints. </li>
       <li> SOFT - all weights in the weighted boolean optimisation
       problem are "soft", i.e., one doesn't use the notion of a
       "hard" constraint which must be satisfied. </li>
       <li> PARTIAL - there is at least one "hard" weighted constraint
       in the weighted boolean optimisation problem, which must be
       satisfied. </li>
      </ul>
      For each of these, there are the following sub-categories:
      <ul>
       <li> SMALLINT - the sum of the coefficients in any constraint is
       less than 2^21. </li>
       <li> BIGINT - the sum of the coefficients in at least one constraint
       is greater than or equal to 2^21. </li>
       <li> LIN - all constraints are linear. </li>
       <li> NCL - at least one constraint is non-linear. </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> MaxSAT competition:
    <ul>
     <li> <em>Unweighted MaxSAT</em> where the benchmark is
     a standard Dimacs CNF and the solver tries to find an assignment
     which minimises the number of falsified clauses. </li>
     <li> <em>Partial MaxSAT</em> where the benchmark is a standard
     CNF file augmented with "hard" and "soft" indicators which are
     added as prefixes to each clause, such that the solver must
     find an assignment which minimises the number of falsified
     clauses but satisifies all "hard" clauses. </li>
     <li> <em>Weighted MaxSAT</em> where each clause in the Dimacs
     file is prefixed with a positive integer weight and then the
     solver must find an assignment which minimises the sum of the
     weights of the clauses falsified by the assignment. </li>
     <li> <em>Partial Weighted MaxSAT</em> where clauses are
     weighted as in weighted MaxSAT but there is a special "top" weight
     which can be used to denote "hard" clauses which must be satisfied
     as in partial MaxSAT. </li>
    </ul>
   </li>
  </ul>

*/

