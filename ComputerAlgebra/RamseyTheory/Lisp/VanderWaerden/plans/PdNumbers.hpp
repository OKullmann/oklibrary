// Oliver Kullmann, 12.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/PdNumbers.hpp
  \brief Plans regarding palindromic van der Waerden numbers


  \todo Palindromic numbers
  <ul>
   <li> As discussed in "Palindromic versions" in
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/general.hpp
   we should provide the smallest n_1 such that for n' >= n_1 no palindromic
   solution exists, and the largest n_0, such that for all n' <= n_0
   palindromic solutions do exist. </li>
   <li> So the "palindromic number" is actually a pair of numbers (namely
   (n_0,n_1)). </li>
   <li> Naming:
    <ol>
     <li> pdvanderwaerden(t) </li>
     <li> How to access the parts (sat- resp. unsat-related)? At Maxima-level
     we could simply use pdvanderwaerden(t)[1 / 2]. It works, however it is
     not very expressive. </li>
     <li> DONE (palindromic versions are something special, a kind of
     "universal variation", and so they deserve their own treatmeant)
     Or perhaps we should provide g_vanderwaerden(t,p), where p is some
     parameter, perhaps a string, which selects which form of a generalised
     vdW-number is considered. </li>
     <li> DONE (not chosen)
     Or perhaps vanderwaerden_g(t,p), in this case
     vanderwaerden_g(t,"pd"). This seems best. </li>
    </ol>
   </li>
   <li> How to name this quantity in mathematical formulas?
    <ol>
     <li> DONE (not chosen) "pdvdw_m(t)" </li>
     <li> "vdw_m^{pd}(t)": this seems better, since it is a variation on
     vdW-numbers; and in general variations of Ramsey-type numbers are
     indicated by an upper index. </li>
     <li> Note that this "number" is actually a pair. </li>
     <li> How to access the two components of the pairs? Lower and upper index
     are already used. One could write "vdw_m^{pd}(t)_{1 / 2}" (index 1 for
     the sat-part, index 2 for the unsat-part) ? Correct, but not very
     expressive. </li>
     <li> In a context where "w(m; t_1,..., t_m)" is used for "vdw_m(t)", one
     can use "w^{pd}(m; t_1,...,t_m)" for vdw_m^pd(t), and
     "w^{pd}_{1 / 2}(m; t_1,...,t_m)" for the sat- resp. unsat-component. </li>
     <li> Again, indices "1", "2" are not much telling. "0", "1" for "unsat",
     "sat" would be better, however then we have a clash with the ordering of
     the components (and their real indices). </li>
    </ol>
   </li>
   <li> From a solution for n a solution for n-2 can be derived by removing
   vertices n and 1, and then subtracting 1 from the remaining vertices. </li>
   <li> Thus the sequence of bits for n=1,... (always for a given tuple t)
   is first all 1's, then 0 and 1 strictly alternating, and then all 0,s
   (where the lenght of the alternating sequence could be zero). </li>
   <li> DONE (integrated)
   Dual versions (regarding satisfiability):
    <ol>
     <li> So we also need to consider the largest n such that all n' <= n yield
     satisfiable problems. </li>
     <li> At the Maximap-level we could name that
     vanderwaerden_g(t,"pdsat"). </li>
     <li> In mathematical formulas we could use "svdw_m^{pd}(t)". </li>
     <li> The two numbers svdw_m^{pd}(t), vdw_m^{pd}(t) completely determine
     satisfiability for all n (for this t). </li>
    </ol>
   </li>
  </ul>

*/
