// Oliver Kullmann, 1.11.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LatinSquares/plans/Sudoku.hpp
  \brief On investigations regarding Sudoku problems (standard, and extended)

  Besides experimentations in creating and solving big Sudoku problems (with
  or without the uniqueness constraint) one should harness the popularity
  of Sudoku to teach (generalised) satisfiability.
  
  
  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Sudoku.hpp.
   </li>
   <li> See Applications/LatinSquares/plans/Sudoku.hpp. </li>
  </ul>


  \todo Finding hard instances for box-dimension 3
  <ul>
   <li> A source for "hard" instances is [extreme Su Doku; Wayne Gould].
    <ol>
     <li> The cover-text says "... Wayne Gould, who'll take you deep inside his
     genius psyche.", and "..., he has been named by Time Magazine as one of
     hte 100 people who shaped the world.". </li>
     <li> One sees that enlightenment is needed. </li>
     <li> As one can see in
     ComputerAlgebra/Satisfiability/Lisp/Generators/docus/Sudoku.hpp,
     apparently most problems only need one r_2-reduction, while perhaps the
     most difficult instance in that collection needs five r_2-reductions
     (without backtracking, and thus also uniqueness is confirmed). </li>
     <li> So likely this source is without further interest. </li>
    </ol>
   </li>
   <li> A better source is [Denis Berthier, The Hidden Logic of Sudoku, 2007]
    <ol>
     <li> See the collections at http://www.carva.org/denis.berthier . </li>
     <li> This source only consider these "humanoid rules" (AI-nonsense). </li>
     <li> Perhaps all these rules are covered by r_2 for the strong encoding?
     </li>
     <li> In principle the all-different constraint can not be covered, but
     perhaps this doesn't happen for these small problems (box-dimension 3)?
     </li>
    </ol>
   </li>
   <li> We should search for problems which are as hard as possible.
    <ol>
     <li> See
     "Sampling of minimally uniquely satisfiable problems" in
     ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Sudoku.hpp. </li>
     <li> The "EasterMonster" (see
     Satisfiability/Lisp/Generators/docus/Sudoku.hpp) needs 9 nodes with
     the OKsolver_2002; this is the hardest example yet encountered. </li>
     <li> There should be harder *unsatisfiable* problems. </li>
     <li> While for the uniquely satisfiable problems actually it would be
     good if the OKsolver could enumerate all solution --- since here this
     would yield the proof of uniqueness, and only this would be comparable
     to the rule-based approach, which also always shows uniqueness. </li>
     <li> At least we need to extract the solution, add it negated to the
     conditions, and see what the complexity of the refutation is. </li>
     <li> This refutation also contains the direct proof that the given total
     assignment is the unique solution. </li>
     <li> Perhaps it is then enough to search for short (tree-like) resolution
     refutations of the original sdk-clause-set ("empty field") plus the
     negated (total) satisfying assignment. </li>
     <li> The refutation of the "EasterMonster" with negated solution actually
     doesn't get any harder than finding the solution; see
     Satisfiability/Lisp/Generators/docus/Sudoku.hpp. </li>
     <li> See below for the "human editing". </li>
    </ol>
   </li>
   <li> For the "human" solution of such problems, "explanations" are needed
   for the solution, which is just a demonstration of the sequence of forced
   assignments.
    <ol>
     <li> In [extreme Su Doku; Wayne Gould] the condition "without reductio ad
     absurdum" is stated. Can this be made precise? </li>
     <li> In [Denis Berthier The Hidden Logic of Sudoku, 2007] one finds a
     more systematic approach, and also an attempt at defining what "without
     guessing" could mean. </li>
     <li> Perhaps r_2-reduction would not be eligible here, and perhaps
     one should use full-strength bijectivity-constraints? </li>
     <li> Of course, one can simulate r_2-reduction via resolution, which
     is "forward reasoning"; so perhaps one should search for short
     resolution derivations of the forced assignments. </li>
     <li> Actually, the last remark seems crucial, showing that all that talk
     about "trial and error" and so on is nonsense: Every forced assignment
     can be demonstrated by r_k for k big enough, and this "trial-and-error"
     derivation can be translated into a "direct logical reasoning", namely
     a resolution refutation (of the unit-clause corresponding to the
     forced assignment)! </li>
     <li> One should do so for example for the above "EasterMonster"-example,
     where [Denis Berthier] claims "no resolution rule is yet known to be
     applicable to this puzzle". </li>
     <li> He speaks of "rules", while the resolution proof is not a general
     rule; so well, perhaps some form of automated generalisation is possible?
     </li>
     <li> The full refutation of the EasterMonster with negation is just 9
     nodes (and 100 r_2-reductions). Of course a lot of unit-propagations are
     involved. A good representation of this refutation would be valuable.
     </li>
    </ol>
   </li>
  </ul>

  \todo Finding hard instances for box-dimension >3
  <ul>
   <li> Some puzzles of box-dimension greater than 3 are available on these
   webpages:
    <ol>
     <li> http://www.setbb.com/phpbb/viewtopic.php?mforum=sudoku&p=10870 </li>
     <li> http://www.menneske.no/sudoku/eng/ </li>
    </ol>
   </li>
  </ul>

*/

