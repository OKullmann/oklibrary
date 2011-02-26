// Oliver Kullmann, 20.9.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
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


  \todo Prepare benchmarks for SAT 2011
  <ul>
   <li> See "Benchmarks for SAT 2011" in
   Experimentation/Investigations/plans/SATCompetition.hpp. </li>
   <li> DONE
   We should have two subclasses, related to vdw_2(3,k) and vdw_2^pd(3,k).
   </li>
   <li> DONE
   Ordinary instances:
    <ol>
     <li> VanDerWaerden_pd_2-3-31_930.cnf is a hard satisfiable instance,
     VanDerWaerden_2-3-31_931.cnf a hard unknown instance (see
     RamseyTheory/VanderWaerdenProblems/plans/3-k/31.hpp). </li>
     <li> All first unsatisfiable instances for 12 <= k <= 19. </li>
     <li> Perhaps also the last satisfiable instances for 20 <= k <= 30. </li>
    </ol>
   </li>
   <li> DONE
   Palindromic instances:
    <ol>
     <li> For complete solvers hard but solved unsatisfiable instances are
     VanDerWaerden_pd_2-3-23_507.cnf and VanDerWaerden_pd_2-3-23_508.cnf
     (see RamseyTheory/VanderWaerdenProblems/plans/3-k/23.hpp). </li>
     <li> Harder but still solved unsatisfiable instances are
     VanDerWaerden_pd_2-3-24_569.cnf and VanDerWaerden_pd_2-3-24_593.cnf
     (see RamseyTheory/VanderWaerdenProblems/plans/3-k/24.hpp). </li>
     <li> The hardest but still solved unsatisfiable instances are
     VanDerWaerden_pd_2-3-25_587.cnf and VanDerWaerden_pd_2-3-25_607.cnf
     (see RamseyTheory/VanderWaerdenProblems/plans/3-k/24.hpp). </li>
     <li> For k=24,25 we also need n=594 resp. n=608 (even harder unsatisfiable
     instances). </li>
     <li> For k=26 we should include the conjecturedly unsatisfiable
     instances. </li>
     <li> We also should include the relevant satisfiable instances. </li>
     <li> And perhaps the two borderline sat-instances for k=39. </li>
    </ol>
   </li>
   <li> DONE
   Generators:
    <ol>
     <li> Generating statically linked generators by
     \verbatim
Generators> oklib cleanall
Generators> LDFLAGS="-static" oklib all
     \endverbatim
     </li>
     <li> The relevant generators are VanderWaerdenCNF-O3-DNDEBUG and
     PdVanderWaerdenCNF-O3-DNDEBUG. </li>
     <li> We should provide a simple generation-script, just containing all
     calls to the generators. </li>
    </ol>
   </li>
   <li> README file:
    <ol>
     <li> Yet missing the links to the OKlibrary. </li>
    </ol>
   </li>
   <li> Pdf file:

   </li>
  </ul>


  \todo Create milestones
  <ul>
   <li> Perhaps the initial version number is 0.2. </li>
   <li> Next milestones concentrate on vdw_2(3,k). </li>
  </ul>


  \todo DONE (as stated)
  Organisation
  <ul>
   <li> We should create a sub-directory "3-k" for investigations into
   vdw_2(3,k). </li>
   <li> There we have file "k.hpp" for special k-values (for example in 30.hpp
   the problems related to vdw_2(3,30) are investigated). </li>
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


  \todo Palindromic versions
  <ul>
   <li> For every parameter tuple t one can consider the problem where the
   solutions are required to be palindromes. </li>
   <li> This is equivalent to replacing the hypergraphs arithprog_hg(k,n) by
   palindromise_vdw_hg(arithprog_hg(k,n)) (where the k's run through t). </li>
   <li> Boolean problems are generated by
   output_pd_vanderwaerden2nd_stdname(k1,k2,n). </li>
   <li> So we consider pdvanderwaerden(t)=(n_0,n_1), where n_1 is the smallest
   n such that for all n' >= n no palindromic solution w.r.t. t exists, while
   n_0 is the largest n such that for all n' <= n a palindromic solution
   w.r.t. t exists. </li>
   <li> This determines satisfiabibility of all n (w.r.t. t). </li>
   <li> Compare "Palindromic numbers" in
   ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/Numbers.hpp. </li>
   <li> Transversal extensions are not interesting in their palindromic
   versions, since for k=2 for even n>= 2 no vertex may be chosen, while for
   odd n exactly the central vertex has to be chosen. </li>
   <li> However the transversal numbers of the hypergraphs are likely of
   interest. </li>
   <li> The palindromic versions of vdW-problems seem to be of great interest,
   and so perhaps they should have its own submodule here. </li>
   <li> There for as many core-tuples as possible numbers should be determined.
   </li>
   <li> Likely it should have a submodule "Hypergraphs", where the transversal
   hypergraph is studied, but also already the determination of the number
   of hyperedges in arithprog_pd_hg(k,n) seems non-trivial. </li>
   <li> DONE
   Due to the importance of palindromic numbers, perhaps they deserve
   a designated system, like pdvanderwaerden(t), parallel to vanderwaerden(t),
   where we have now pairs (for the above vanderwaerden_g(t,"pdsat") and
   vanderwaerden_g(t,"pd")). </li>
  </ul>


  \todo Overview on performance of SAT solvers
  <ul>
   <li> We need an overview how SAT solvers perform on the different van der
   Waerden problems. </li>
  </ul>

*/

