// Oliver Kullmann, 1.11.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LatinSquares/Sudoku/plans/general.hpp
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


  \todo Translations to boolean CNFs based on the direct encoding
  <ul>
   <li> In ComputerAlgebra/Satisfiability/Lisp/Generators/Sudoku.mac we
   provide three translations:
    <ol>
     <li> output_weak_sdk_stdname </li>
     <li> output_dual_weak_sdk_stdname </li>
     <li> output_strong_sdk_stdname </li>
    </ol>
   </li>
   <li> In general we have four types of clauses for the direct encoding:
    <ol>
     <li> primary binary clauses: different numbers per line </li>
     <li> primary long clauses: every field gets a number </li>
     <li> dual binary clauses: at most one number per field </li>
     <li> dual long clauses: every line contains all numbers </li>
    </ol>
   </li>
   <li> Thus we get 2^4=16 possible combinations, but not all combinations
   are valid:
    <ol>
     <li> Long clauses are needed, otherwise no field getting a number would
     be a solution. </li>
     <li> And at least all primary or all dual clauses are needed. </li>
     <li> So we have 3 + 3 + 1 = 7 valid possibilities. </li>
     <li> How to call them? The primary clauses (only) has been called already
     "weak", only the dual clauses "dual weak", and primary and dual together
     "strong". Remain the 4 mixed cases. </li>
     <li> Perhaps "weak pb", "weak pl", "dual weak pb" and "dual weak pl"
     ("p" for "plus") ? </li>
    </ol>
   </li>
   <li> Previous experimentations would suggest that "weak pl" is a good
   combination. </li>
  </ul>


  \todo Finding hard instances for box-dimension 3
  <ul>
   <li> A source for "hard" instances is [extreme Su Doku; Wayne Gould].
    <ol>
     <li> The cover-text says "... Wayne Gould, who'll take you deep inside his
     genius psyche.", and "..., he has been named by Time Magazine as one of
     the 100 people who shaped the world.". </li>
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
   <li> We should search for problems which are as "hard" as possible.
    <ol>
     <li> See
     "Sampling of minimally uniquely satisfiable problems" in
     ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Sudoku.hpp. </li>
     <li> The "EasterMonster" (see
     Satisfiability/Lisp/Generators/docus/Sudoku.hpp) needs 9 nodes with
     the OKsolver_2002; this is the hardest example yet encountered. </li>
     <li> The right approach should be to compute the t-hardness, p-hardness
     and w-hardness!
      <ol>
       <li> For uniquely satisfying solutions these hardnesses are given by
       the hardness of the unsatisfiable problems given by setting (just) one
       variable to a wrong value. </li>
       <li> This needed to be computed for all known instances. </li>
       <li> p-hardness 1 means that by directly applying the most basic rules
       all fields are filled. </li>
       <li> p-hardness 2 and hardness 1 means that one needs to fill in one at
       least one false number, and then apply "basic" reasoning. </li>
       <li> Note that in general a hardness value k is not just the worst-case,
       but also determines the best-case for reasoning: by setting or
       forbidding values for fields (i.e., assigning a value to a variable) we
       need precisely r_{k-1} to refute the false values and solve all fields
       (one after another). </li>
       <li> See "Hardness of Sudoku problems" in
       Investigations/BooleanFunctions/plans/Hardness/general.hpp. </li>
       <li> This should replace the following older discussion, based on
       solver-runs. </li>
      </ol>
     </li>
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


  \todo Tools for investigations with concrete Sudoku instances
  <ul>
   <li> cnf_puzzle   
    \verbatim
NAME
     cnf_puzzle
SYNOPSIS
     cnf_puzzle [OPTIONS] empty puzzle
OPTIONS
     -c FILE
     Add the clauses in FILE to the output.
DESCRIPTION
     cnf_puzzle outputs a Sudoku puzzle in Dimacs-cnf-format
     according to the direct encoding. The file "empty" is a 
     cnf-file in Dimacs format representing an empty Sudoku puzzle.
     "puzzle" is a string representing a concrete instance of a
     Sudoku puzzle. The puzzle-string contains 81 digits with
     values between 0 and 9. 0 represents an empty cell. The 
     fixed fields of the puzzle are specified by non-zero values.
     If position 1 <= i <= 81 contains non-zero value k then the final Sudoku
     puzzle output has fixed field ((i-1)/9+1, (i-1)%9+1) with value k, where
     / is integer division and % is the modulus operator.

     An optional file argument allows arbitrary other clauses to
     be added to the output. (The intention being to allow the
     negation of a satisfying assignment).
     \endverbatim
     <ol>
      <li> It would be good if optionally instead of the string "empty"
      the script could read from standard input. </li>
      <li> Also good if there would be an option "--strict", which only
      accepts the string as specified above, while otherwise space-symbols
      in the string are ignored, and missing values at the end are
      automatically added as zeros. </li>
      <li> Is "FILE" also in Dimacs-format? The point here is about the
      preamble, the initial comments plus the parameter-line. </li>
      <li> DONE (good enough for the beginning)
      Perhaps allowing arbitrary extra clauses to be appended to the 
      output is a bad idea. Really we only want to allow precisely the 
      negation of the (unique?) satisfying assignment? </li>
      <li> DONE (good enough for the beginning)
      As above, the script is not especially flexible because it has to
      be called once per puzzle. Nicer would be to allow multiple concrete 
      puzzles as input but then how to control the output so that different
      puzzles are output to different files? </li>
      <li> Larger dimensions:
       <ol>
        <li> Possibly the script should be able to handle puzzles of larger 
        dimension but then a different format for the puzzle input is 
        needed. </li>
        <li> We should have a look how at the various web-pages these larger
        instances are specified. </li>
       </ol>
      </li>
     </ol>
    </li>
  </ul>

*/

