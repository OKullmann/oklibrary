// Oliver Kullmann, 2.7.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/plans/milestones.hpp

  \module_version ComputerAlgebra 0.1.2.4 (9.3.2010)


  \par Version 0.1.3

  \par ComputerAlgebra/Satisfiability version 0.0.6
  (see ComputerAlgebra/Satisfiability/plans/milestones.hpp)

  \par ComputerAlgebra/Graphs/Lisp version 0.1.1
  (see ComputerAlgebra/Graphs/Lisp/plans/milestones.hpp)

  \par ComputerAlgebra/NumberTheory/Lisp version 0.1.3
  (see ComputerAlgebra/NumberTheory/Lisp/plans/milestones.hpp)

  \par ComputerAlgebra/Combinatorics version 0.0.4 : DONE
  (see ComputerAlgebra/Combinatorics/plans/milestones.hpp)

  \par ComputerAlgebra/Hypergraphs version 0.0.8 : DONE
  (see ComputerAlgebra/Hypergraphs/Lisp/plans/milestones.hpp)
  
  \par
   In ComputerAlgebra/DataStructures/Lisp/plans/HashMaps.hpp the following 
   topics are handled:
    - Introduce memoise function wrapper

  \par
   In ComputerAlgebra/CombinatorialMatrices/Lisp/plans/general.hpp the 
   following topics are handled:
    - Random invertible matrix

  \par
   In ComputerAlgebra/TestSystem/Lisp/plans/general.hpp the following topics
   are handled:
    - Outline of the test system

  \par
   In ComputerAlgebra/plans/Maxima.hpp the following topics are handled:
    - Why is Maxima file output so slow?
    - Stable sorting : DONE
    - Maxima seg-faults when loading a file setting memory-limits : DONE
    - Restricted recursion for memoised functions : DONE
    - Strings cause errors in evaluation of expressions : DONE

  \par
   Replacement of "some" by "some_s", and of "every" by "every_s"
   (supplying tests for all modified functions): DONE
   <ol>
    <li> %Algebra : DONE </li>
    <li> AutomatedTheoremProving : DONE </li>
    <li> CombinatorialMatrices : DONE </li>
    <li> Cryptology (MG) : DONE
     <ol>
      <li> directly under Lisp : DONE </li>
      <li> Lisp/Cryptanalysis : DONE </li>
      <li> Lisp/CryptoSystems : DONE </li>
     </ol>
    </li>
    <li> DataStructures : DONE </li>
    <li> %Graphs : DONE </li>
    <li> Hypergraphs : DONE </li>
    <li> LinearAlgebra : DONE </li>
    <li> MaximaInternals : DONE </li>
    <li> %NumberTheory : DONE </li>
    <li> Numerical : DONE </li>
    <li> Satisfiability : DONE
     <ol>
      <li> Autarkies : DONE </li>
      <li> Backdoors : DONE </li>
      <li> Backtracking : DONE </li>
      <li> BranchingTuples : DONE </li>
      <li> %ClauseSets : DONE </li>
      <li> ConflictCombinatorics : DONE </li>
      <li> ConstraintProblems : DONE </li>
      <li> Counting : DONE </li>
      <li> %Generators : DONE </li>
      <li> MinimalUnsatisfiability : DONE </li>
      <li> PropositionalLogic : DONE </li>
      <li> Reductions : DONE </li>
      <li> Resolutions : DONE </li>
      <li> Symmetries : DONE </li>
     </ol>
    </li>
    <li> TestSystem : DONE </li>
    <li> Trees : DONE </li>
   </ol>

  \par
   Replacement of "makelist" by "create_list" (and supplying tests for all
   modified functions): DONE
   <ol>
    <li> %Algebra : DONE </li>
    <li> AutomatedTheoremProving : DONE </li>
    <li> CombinatorialMatrices : DONE </li>
    <li> Cryptology (MG) : DONE
     <ol>
      <li> directly under Lisp : DONE </li>
      <li> Lisp/Cryptanalysis : DONE </li>
      <li> Lisp/CryptoSystems : DONE </li>
     </ol>
    </li>
    <li> DataStructures : DONE </li>
    <li> %Graphs : DONE </li>
    <li> Hypergraphs : DONE </li>
    <li> LinearAlgebra : DONE </li>
    <li> MaximaInternals : DONE </li>
    <li> %NumberTheory : DONE </li>
    <li> Numerical : DONE </li>
    <li> Satisfiability : DONE
     <ol>
      <li> Autarkies : DONE </li>
      <li> Backdoors : DONE </li>
      <li> Backtracking : DONE </li>
      <li> BranchingTuples : DONE </li>
      <li> %ClauseSets : DONE </li>
      <li> ConflictCombinatorics : DONE </li>
      <li> ConstraintProblems : DONE </li>
      <li> Counting : DONE </li>
      <li> %Generators : DONE </li>
      <li> MinimalUnsatisfiability : DONE </li>
      <li> PropositionalLogic : DONE </li>
      <li> Reductions : DONE </li>
      <li> Resolutions : DONE </li>
      <li> Symmetries : DONE </li>
     </ol>
    </li>
    <li> TestSystem : DONE </li>
    <li> Trees : DONE </li>
   </ol>


  \par Version 0.1.4

  \par ComputerAlgebra/Satisfiability version 0.0.7
  (see ComputerAlgebra/Satisfiability/plans/milestones.hpp)

  \par ComputerAlgebra/Graphs/Lisp version 0.1.3
  (see ComputerAlgebra/Graphs/Lisp/plans/milestones.hpp)
  
  \par ComputerAlgebra/Algebra version 0.0.6
  (see ComputerAlgebra/Algebra/plans/milestones.hpp)

  \par ComputerAlgebra/Cryptology version 0.0.6
  (see ComputerAlgebra/Cryptology/plans/milestones.hpp)

  \par
   In ComputerAlgebra/plans/Maxima.hpp the following topics are handled:
    - Memoised functions
    - Argument-length restriction
    - How to use function-parameters reliably??
    - Problems with errcatch
    - What is "equalp" ?
    - Maxima/CLisp bug for larger data-sets

  \par
   Replacement of "apply" by the correct version of generalised apply
   (see "Apply-functionality" in
   ComputerAlgebra/DataStructures/Lisp/plans/Lists.hpp), respectively
   by lunion or lintersection (see "Unions and intersections" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/SetSystems.hpp)
   <ol>
    <li> %Algebra : </li>
    <li> AutomatedTheoremProving : </li>
    <li> CombinatorialMatrices : </li>
    <li> Cryptology (MG)
     <ol>
      <li> directly under Lisp : DONE </li>
      <li> Lisp/Cryptanalysis : DONE </li>
      <li> Lisp/CryptoSystems : DONE </li>
     </ol>
    </li>
    <li> DataStructures : DONE </li>
    <li> %Graphs : </li>
    <li> Hypergraphs : </li>
    <li> LinearAlgebra : </li>
    <li> MaximaInternals : </li>
    <li> %NumberTheory : </li>
    <li> Numerical : </li>
    <li> Satisfiability :
     <ol>
      <li> Autarkies : </li>
      <li> Backdoors : </li>
      <li> Backtracking </li>
      <li> BranchingTuples </li>
      <li> %ClauseSets : </li>
      <li> ConflictCombinatorics : </li>
      <li> ConstraintProblems </li>
      <li> Counting : </li>
      <li> %Generators </li>
      <li> MinimalUnsatisfiability </li>
      <li> PropositionalLogic </li>
      <li> Reductions : </li>
      <li> Resolutions : </li>
      <li> Symmetries : </li>
     </ol>
    </li>
    <li> TestSystem : </li>
    <li> Trees : </li>
   </ol>

  \par
   "Generating list of elements 1 to n" in DataStructures/Lisp/plans/Lists.hpp

  \par
   Update the following milestones, towards version 0.2


  \par Version 0.2 "First phase completed"

  \par
   Towards milestone 0.3 the theme is is "REDESIGN MAXIMA SYSTEM"


  \par Version 0.2.1

  \par ComputerAlgebra/Mupad version 0.5.1

  \par
   In ComputerAlgebra/plans/Maxima.hpp the following topics are handled:
    - General design: Lists *here* are more fundamental than sets
    - Plan the redesign

  \par
   In ComputerAlgebra/plans/MaximaTechniques.hpp the following topics are
   handled:
    - Local variables
    - Declaring variables


  \par Version 0.2.2

  \par ComputerAlgebra/Satisfiability version 0.0.8
  (see ComputerAlgebra/Satisfiability/plans/milestones.hpp)

  \par ComputerAlgebra/Cryptology version 0.0.6
  (see ComputerAlgebra/Cryptology/plans/milestones.hpp)

  \par
   In ComputerAlgebra/Mupad/plans/general.hpp the following topics are handled:
    - Rewriting

  \par
   In ComputerAlgebra/plans/Maxima.hpp the following topics are handled:
    - Recovering of partial results after long (unsuccessful) computations
    - Debugging
    - Documentation
    - Handling of demos
    - Monitoring

  \par
   In ComputerAlgebra/plans/MaximaTechniques.hpp the following topics are
   handled:
    - Elementary arithmetic
    - List operations
    - Set operations


  \par Version 0.2.3

  \par
   In ComputerAlgebra/plans/general.hpp the following topics are handled:
    - Frobenius problem
    - Dynamical systems

  \par
   In ComputerAlgebra/plans/Maxima.hpp the following topics are handled:
    - Contexts
    - Lisp integration
    - Collaboration with the Maxima community

  \par
   In ComputerAlgebra/plans/MaximaTechniques.hpp the following topics are
   handled:
    - Bugs of Maxima and their corrections
    - Document important programming techniques
    - Partial compilation
    - Document simplification of expressions
    - Timing



  \par Version 0.3

  \par
   Development at Axiom/Aldor level started.



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.1   : 2.7.2007; quite a few Mupad-functions produced (but without higher design, and not connected to the main concepts of (generalised) satisfiability).
   - 0.1.1 : 5.1.2008; concentration on Maxima: created a file-inclusion-system similar to the C++ system, and provided easy installation and access; also created general documentation, and clarified the roles of Maxima/Lisp vs. Axiom/Aldor. On the way also basic functionality regarding graphs, hypergraphs and satisfiability has been established in Maxima.
   - 0.1.2 : 27.9.2008; established basic functionality for graphs, hypergraphs, combinatorial matrices and SAT problems at Maxima/Lisp level; further work on cryptosystems (established AES and basic SAT translation) and datastructures; various experiments on SAT problems; strengthening the general system (many bugs and problems with Maxima/Lisp identified and corrected); several pieces of documentation; most development spontaneous.

*/

