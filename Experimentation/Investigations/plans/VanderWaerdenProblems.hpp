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
     <li> n = 600 (598 variables, 5918 clauses) didn't finish after one
     hour with OKsolver-2002. </li>
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
    </ol>
   </li>
   <li> One should study the density of the clause-sets (and the "threshold")
   here. </li>
   <li> One can also consider non-diagonal Green-Tao numbers. </li>
   <li> One needs to investigate whether bounds are known. </li>
  </ul>

*/

