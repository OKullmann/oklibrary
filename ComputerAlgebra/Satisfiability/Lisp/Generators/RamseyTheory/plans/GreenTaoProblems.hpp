// Oliver Kullmann, 30.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/GreenTaoProblems.hpp
  \brief Plans for Maxima-generators related to Green-Tao problems


  \todo Connections
  <ul>
   <li> See Satisfiability/Transformers/Generators/plans/GreenTao.hpp
   for general plans at C++ level. </li>
   <li> See
   Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/general.hpp
   for experiments with GT-problems. </li>
   <li> See Satisfiability/Transformers/Generators/ and
   Satisfiability/Transformers/Generators/plans/general.hpp for several
   generators for vdW- and GT-problems. </li>
  </ul>


  \todo Standardisation
  <ul>
   <li> As we have it now in output_vanderwaerden, standardisation for
   all non-standardised clause-sets should be done explicitly. </li>
  </ul>


  \todo Statistics
  <ul>
   <li> As discussed in "Accompanying statistics" in
   Satisfiability/Lisp/Generators/plans/general.hpp in general, we need
   statistics for all main measurements (and also for all others, if
   possible). </li>
   <li> First the hypergraph measurements needs to be established; see
   "Statistics" in Hypergraphs/Lisp/plans/Generators.hpp. </li>
  </ul>


  \todo Statistics output for Green-Tao problems in the Dimacs-file
  <ul>
   <li> Care needs to be taken that these measurements are not too
   inefficient --- currently they are.
    <ol>
     <li> One bottleneck is standardisation, which could be done here
     more efficient by using arrays (instead of hash-tables). </li>
     <li> And also the statistics regarding variable and literal degrees
     could be computed more efficiently by using arrays, since here we
     have standardised clause-sets. </li>
    </ol>
   </li>
   <li> If different progression-lengths are involved, then for each
   involved hypergraph we should have data like standard_statistics_fcs. </li>
   <li> This should happen in the hypergraph module. </li>
   <li> The data for the whole clause-set could then be obtained by just
   adding up the numbers. </li>
   <li> As discussed above in todo "Statistics", there should be specialised
   functions (with standardised names) for computing the "standard" statistics.
   </li>
   <li> For Green-Tao data this doesn't help much, since there is no
   faster computation than just "measuring", but we should establish
   (and follow) a general scheme. </li>
   <li> DONE (the general computation now likely can't be improved much further)
   The computation of standard_statistics_fcs should be made much
   more efficient. </li>
  </ul>


  \todo Symmetry breaking for GT-problems
  <ul>
   <li> For a progression-length k (relevant for all parts i with k_i=k)
   we want to use the vertex occurring most often. </li>
   <li> Then the current implementations, which just chose prime 3 (let's
   call it "simple symmetry breaking"), are
   defective, since only for k=3 this is the right choice. </li>
   <li> For k=2 we just have a complete graph, and so all vertices are
   equal. </li>
   <li> For prime k >= 3 it seems that k itself occurs most often.
   This should be due to the exceptional status of k as basis of
   progressions of length k (their slopes doesn't need to be a multiple
   of product_primes(k), as it is otherwise the case). </li>
   <li> For non-prime k >= 3 the first impression is, that the vertex
   with index roughly 0.42*n for k=4 and roughly 0.3*n for k=6 occurs most
   often; this needs to investigated. </li>
   <li> So it seems that one needs a function at hypergraph level which for
   given k, n determines the vertex occurring most often. </li>
   <li> Let's call the resulting symmetry breaking "max-degree symmetry
   breaking". </li>
   <li> The only experiments yet to see a potential difference between simple
   and max-degree symmetry breaking ist greentao_3(3,4,4) (see
   RamseyTheory/GreenTaoProblems/plans/GreenTao_3-3-4-k.hpp). </li>
  </ul>


  \todo Arithmetic progressions for prime numbers
  <ul>
   <li> The following needs to go into the docus:
    <ol>
     <li> The theorem of Green-Tao: "When partitioning the set of prime numbers
     into m parts, then at least one part contains arithmetic progressions
     of arbitrary lengths". </li>
     <li> Via compactness, this should translate into the property that for m
     and k there is (a least) n such that for every partitioning of the first
     n prime numbers into m parts one of the parts contains an arithmetic
     progressions of length k. </li>
     <li> The corresponding numbers are "Green-Tao numbers", greentao(m,k).
     </li>
     <li> One should have greentao(m,k) >= vanderwaerden(m,k). </li>
    </ol>
   </li>
   <li> DONE
   We should create a function "greentao2_fcs(k,n)".
    <ol>
     <li> Via primes_first(n) we get the first n prime numbers. </li>
     <li> Finding the arithmetic progressions of length k can be done
     recursively. </li>
    </ol>
   </li>
   <li> DONE
   Of course, we also need the underlying hypergraphs. </li>
  </ul>

*/
