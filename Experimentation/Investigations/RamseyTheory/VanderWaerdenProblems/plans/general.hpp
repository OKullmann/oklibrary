// Oliver Kullmann, 20.9.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/general.hpp
  \brief On investigations into van-der-Waerden problems


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/general.hpp. </li>
   <li> See VanderWaerden.cpp (to be updated) for a C++ generator. </li>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/VanderWaerdenProblems.hpp.
   </li>
   <li> External pages:
    <ol>
     <li> http://www.st.ewi.tudelft.nl/sat/waerden.php contains lower bounds
     and methods how to get them. </li>
     <li> http://users.encs.concordia.ca/~ta_ahmed/vdw.html contains the list
     of known numbers (except of transversal vdW-numbers). </li>
    </ol>
   </li>
  </ul>


  \todo Boolean van der Waerden numbers
  <ul>
   <li> Via
   \verbatim
> VanderWaerden-O3-DNDEBUG k n > VanDerWaerden_2-k-k_n.cnf
   \endverbatim
   diagonal boolean problems can be generated, and via
   \verbatim
> VanderWaerden-O3-DNDEBUG k1 k2 n > VanDerWaerden_2-k1-k2_n.cnf
   \endverbatim
   non-diagonal problems. </li>
   <li> In Maxima we have "output_vanderwaerden2_stdname(k,n)" (in
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/VanderWaerdenProblems.mac)
   as well as "output_vanderwaerden2nd_stdname(k1,k2,n)".
   </li>
   <li> vanderwaerden_2(3) = 9 (density = 3.6) </li>
   <li> vanderwaerden_2(4) = 35 (density = 10.7) </li>
   <li> vanderwaerden_2(5) = 178 (density = 43.5)
    <ol>
     <li> Relatively easy for OKsolver-2002 (710s, 109703 nodes). </li>
     <li> march_pl needs only 51529 nodes and 238 seconds, so the "double
     look-ahead" seems to be successful here. </li>
     <li> minisat1 needs 213 seconds, so also clause-learning seems applicable
     here; minisat2 is even faster (21 restarts, 829170 conflicts, 90s). </li>
     <li> satz215 doesn't solve it in 30 minutes. </li>
    </ol>
   </li>
   <li> For vanderwaerden_2(6) see
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_2-6-6.hpp.
   </li>
  </ul>


  \todo Script for searching for a vdW-number
  <ul>
   <li> Input is the vdW-parameter-tuple, and the start value for n. </li>
   <li> Then the script runs through n, n+1, ..., until the instance
   is unsatisfiable. </li>
   <li> See
   ExperimentSystem/ControllingLocalSearch/plans/PointOfUnsatisfiability.hpp.
   </li>
   <li> For problems with two parts only the SAT solver is needed as
   parameter, while for more parts also the translation method is needed. </li>
   <li> A complication arises when using incomplete algorithms for finding a
   satisfying assignment.
    <ol>
     <li> The main problem then is the number of runs and the cutoff. </li>
     <li> And at a certain point one has to try a complete solver. </li>
     <li> Perhaps all this needs a second script; so we have one script for
     running one complete solver, and one script for running one incomplete
     solver. </li>
    </ol>
   </li>
   <li> The script RunVdW3k is a first very simple tool, just using a fixed
   number of runs and a fixed cutoff. </li>
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

