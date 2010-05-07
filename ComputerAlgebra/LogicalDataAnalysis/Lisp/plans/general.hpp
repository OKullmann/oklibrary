// Oliver Kullmann, 7.8.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/LogicalDataAnalysis/Lisp/plans/general.hpp
  \brief General plans regarding the logical analysis of data


  \todo %Matrices
  <ul>
   <li> We need various functions for handling matrices, which generalise
   the "truth tables" found in the literature. </li>
   <li> Basis is the concept of a combinatorial matrix (see
   ComputerAlgebra/CombinatorialMatrices/Lisp/Basics.mac). </li>
   <li> So a "data frame" is a combinatorial matrix M, where M[1] (the
   set of row names) yields the set of "cases", M[2] (the set of column
   names) yields the sets of "variables", and where the values are
   column-specific (that is, every variables has a domain). </li>
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
     <li> Displaying matrices in the usual style (for inspection) is done
     via using disp2d. </li>
     <li> Considering the example of Table 4 (page 44) in [Schneider, Wagemann,
     2007], we get the "session"
     \verbatim
SW4b : matrix([0,0,0,1],[0,0,1,1],[0,1,0,1],[0,1,1,1],[1,0,0,0],[1,0,1,1],[1,1,0,0],[1,1,1,0])$
disp2d(SW4b);
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

SW10b : SW4b;
SW10b[4,4] : 0;
SW10b[6,4] : 0;

SW10 : tt2cvm(SW10b);
SW10F : clvar_com2fcs(SW10);

min_2resolution_closure_cs(SW10F[2]);
  {{-4,1},{-4,2,3},{-3,-1,4},{-2,-1,4}}
all_minequiv_bvs_cs(SW10F[2]);
  [{{-4,1},{-4,2,3},{-3,-1,4},{-2,-1,4}}]

dual_min_2resolution_closure_cs(SW10F[2]);
  {{-4,-3,-2},{-4,-1},{1,2,4},{1,3,4}}
dual_all_minequiv_bvs_cs(SW10F[2]);
  [{{-4,-3,-2},{-4,-1},{1,2,4},{1,3,4}}]
     \endverbatim
     </li>
     <li> For the non-boolean case we can have uniform or function domains
     (as in ComputerAlgebra/Satisfiability/Lisp/ClauseSets/NonBoolean.mac).
     </li>
     <li> So here we should again have just (combinatorial) matrices, and
     (combinatorial) matrices with domains (as we have it for nb-clause-sets).
     </li>
    </ol>
   </li>
   <li> Perhaps we could use combinatorial matrices over {0,1}:
    <ol>
     <li> The row-names would then be the case-names. </li>
     <li> While the column-names would be the variable-names. </li>
     <li> The data-matrix from [The Outcomes of Homeless Mobilization;
     Cress, Snow, The American Journal of Sociology, 2000], Table 3 would
     then be available as follows:
     \verbatim
CS : mrc2ocom(matrix(
 [1,1,1,1,1,1,1,1,1,1],
 [1,0,1,1,1,1,1,1,1,1],
 [1,1,1,0,1,1,1,1,1,1],
 [1,1,1,0,1,1,1,0,1,1],
 [1,1,1,1,1,1,0,0,1,1],
 [1,1,1,0,1,1,1,0,1,0],
 [1,0,0,1,1,1,1,0,1,0],
 [0,1,0,1,0,1,0,0,0,1],
 [0,1,0,0,0,1,0,0,0,1],
 [0,0,1,1,0,0,0,1,0,0],
 [0,0,0,0,0,1,0,0,0,1],
 [0,0,0,0,0,1,0,0,0,1],
 [0,1,0,1,0,0,0,0,0,0],
 [0,0,0,0,0,0,0,0,0,0],
 [0,0,0,0,0,0,0,0,0,0]),
 ["PUH", "AOS", "OUH", "TUH", "PUEJ", "DtUH", "HCRP", "BUH", "DnUH", "HF", "HUH", "HU", "MUH", "HPU", "MC"],
 ["Via", "DisT", "SymA", "CSup", "DiagF", "ProgF", "Rep", "Res", "Ri", "Re"])$

ocom_p(CS,{0,1});
  true

CS[3]("TUH", "SymA");
 1
     \endverbatim
     </li>
     <li> Via
     \verbatim
ttcom2cvm(M) := subst(-1,0,M)$
CSc : ttcom2cvm(CS);
     \endverbatim
     we obtain a "clause-variable" matrix. To obtain a clause-set, we need
     to wrap the variable-strings ("Via" etc.) by a variable-producing
     function, for example the generic wrapper "gv":
     \verbatim
FF : clvar_w_ocom2fcl(CSc, gv);
fcl_p(clvar_ocom2fcl(CSc));
  false
     \endverbatim
     The error is due to the Maxima-bug when handling strings; so currently we
     can't have variables like gv("Via"). </li>
     <li> Though in principle it seems that using such variables is reasonable;
     only perhaps we typically avoid the distinction between small and capital
     letters, just using only small letters. </li>
     <li> An alternative would be to introduce dedicated variables
     (like Via), however then we would get easily naming-conflicts. </li>
    </ol>
   </li>
  </ul>


  \todo Representing partial boolean functions
  <ul>
   <li> Given a set V of variables, a partial boolean function f could be
   represented by a pair [T,F], where T,F are disjoint sets of total
   assignments over V, representing the true resp. false points of f. </li>
   <li> However, falsifying assignments are the domain of CNF, not DNF,
   and so the proper generalisation, allowing arbitrary clause-sets T,F over
   V, seems to be a triple [V,T,F], such that T,F are clause-sets over V,
   and such that the satisfying assignment of T as DNF are disjoint from
   the falsifying assignments of F as CNF. </li>
   <li> So T directly represents the sufficient conditions, while F directly
   represents the necessary conditions. </li>
   <li> An example:
    <ol>
     <li> Let V={1,2}. </li>
     <li> Let [T,F] be [{{1,2}},{{-1,-2}}], saying that <1->1, 2->1> -> true,
     and <1->0, 2->0> -> false. </li>
     <li> The (here unique) triple-representation is
     [{1,2}, {{1,2}}, {{1,2}}]. </li>
    </ol>
   </li>
  </ul>


  \todo Analysing the extension-landscape
  <ul>
   <li> The goal is to represent a partial boolean function using as few as
   many case-distinctions (clauses), either as DNF or as CNF. </li>
   <li> The reason for this minimisation is that the cases represent a
   fundamentally different domain, which needs a handling on its own, and
   thus for economic principles we would like to have as few cases as
   possible. </li>
   <li> Given a partial boolean function P = [V,T,F] in triple representation,
   we do not make any assumptions on the possible extensions. </li>
   <li> The immediate goal is to find shortest CNF or CNF G for P. </li>
   <li> These CNF/DNF are naturally (total) boolean functions over V, however
   the goal is not to find "good" or "reasonable" extensions, but to explore
   the landscape. </li>
   <li> This could mean the following (now considering only DNF or only CNF
   representations, and assuming that we want to have K cases):
    <ol>
     <li> Find all G (as above) of length K. </li>
     <li> The total assignments which satisfy/falsify all G are the
     sufficient/necessary assumptions we have to make in order to achieve
     (only) K cases. </li>
     <li> The total assignments which satisfy/falsify no G are the
     sufficient/necessary assumptions we have to avoid in order to achieve
     (only) K cases. </li>
     <li> These two assumption-types seem most natural. The remaining
     assumptions, satisfying/falsifying some G, are all possible for a
     K-representation; they are not independent of each other, and one could
     create a directed graph with the dependency relations. </li>
     <li> In the first two cases one gets sets of total assignments,
     representing boolean functions, and again one can seek for short
     representations of these boolean functions (here now we don't have the
     problem of extension, but all we need is a usable representation of these
     boolean functions). </li>
    </ol>
   </li>
  </ul>
  
*/

