// Oliver Kullmann, 7.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/LogicalDataAnalysis/plans/general.hpp
  \brief General plans regarding the logical analysis of data


  \todo Matrices
  <ul>
   <li> We need various functions for handling matrices, which generalise
   the "truth tables" found in the literature. </li>
   <li> Basis is the concept of a combinatorial matrix (see
   ComputerAlgebra/CombinatorialMatrices/Lisp/Basics.mac). </li>
   <li> So a "data frame" is a combinatorial matrix M, where M[1] (the
   set of row names) yields the set of "cases", M[2] (the set of column
   names) yields the sets of "variables", and where the values, which
   are column-specific (that is, every variables has a domain). </li>
   <li> This seems to be best understood as a clause-variable matrix,
   where variables can be boolean or non-boolean.
    <ol>
     <li> For the boolean case the values are -1,0,+1, and the translations
     from matrices to clause-sets and back are given by clvar_com2fcs and
     clvar_fcs2com (where for the former actually any values which can be
     positive, negative or zero are allowed). </li>
     <li> In QCA (yet) only "full configurations" (corresponding to full
     clauses) are considered, and encoded via 0 and 1; the translation
     function
     \verbatim
tt2cvm(M) := m2com(subst(-1,0,M))$
     \endverbatim
     is to be used. </li>
     <li> Displaying matrices in the usual style (for inspection) can be done
     via
     \verbatim
disp_2d(x) := block([display2d:true], disp(x))$
     \endverbatim
     </li>
     <li> Considering the example of Table 4 (page 44) in [Schneider, Wagemann,
     2007], we get the "session"
     \verbatim
SW4b : matrix([0,0,0,1],[0,0,1,1],[0,1,0,1],[0,1,1,1],[1,0,0,0],[1,0,1,1],[1,1,0,0],[1,1,1,0])$
disp_2d(SW4b);
SW4 : tt2cvm(SW4b);
SW4F : clvar_com2fcs(SW4);

min_2resolution_closure_cs(SW4F[2]);
  {{-4,-3,1},{-4,1,2},{-2,3,4},{-1,4}}
all_minequiv_bvs_cs(SW4F[2]);
  [{{-4,-3,1},{-4,1,2},{-2,3,4},{-1,4}}]

dual_min_2resolution_closure_cs(SW4F[2]);
  {{-4,-2,-1},{-4,-1,3},{-3,2,4},{1,4}}
dual_all_minequiv_bvs_cs(SW4F[2]);
  [{{-4,-2,-1},{-4,-1,3},{-3,2,4},{1,4}}]
     \endverbatim
     </li>
     <li> For the non-boolean case we can have uniform or function domains
     (as in ComputerAlgebra/Satisfiability/Lisp/ClauseSets/NonBoolean.mac).
     </li>
     <li> So here we should actually have just matrices, and matrices with
     domains (as we have it for nb-clause-sets). </li>
    </ol>
   </li>
  </ul>
  
*/

