// Oliver Kullmann, 20.9.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/plans/VanderWaerdenProblems.hpp
  \brief On investigations into van-der-Waerden problems


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
   <li> Is there not also, at least, the symmetry about the number line? i.e. 
   if there is a colouring of the numbers such that there is no arithmetic 
   progression of size k, then reversing the colouring should also have this 
   property. </li>  
   <li> For small problems one needs to determine the full automorphism group
   of the clause-sets. </li>
  </ul>


  \todo Overview on performance of SAT solvers
  <ul>
   <li> We need an overview how SAT solvers perform on the different van der
   Waerden problems. </li>
  </ul>

*/

