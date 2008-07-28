// Oliver Kullmann, 2.7.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/plans/milestones.hpp

  \module_version ComputerAlgebra 0.1.1.4 (2.7.2008)


  \par Version 0.1.2

  \par ComputerAlgebra/Satisfiability version 0.0.6
  (see ComputerAlgebra/Satisfiability/plans/milestones.hpp)

  \par ComputerAlgebra/Graphs/Lisp version 0.1 : DONE
  (see ComputerAlgebra/Graphs/Lisp/plans/milestones.hpp)
  
  \par ComputerAlgebra/Algebra version 0.0.5 : DONE
  (see ComputerAlgebra/Algebra/plans/milestones.hpp)

  \par ComputerAlgebra/Cryptology version 0.0.5 : DONE
  (see ComputerAlgebra/Cryptology/plans/milestones.hpp)

  \par
   In ComputerAlgebra/plans/Maxima.hpp the following topics are handled:
    - File load and include
    - How to eliminate the annotation of lists : DONE
    - New module "Trees" : DONE
    - New supermodule %NumberTheory : DONE

  \par
   In ComputerAlgebra/TestSystem/Lisp/plans/general.hpp the following topics
   are handled:
    - Outline of the test system

  \par
   Replacement of "some" by "some_s", and of "every" by "every_s" (supplying
   tests for all modified functions):
   <ol>
    <li> %Algebra : </li>
    <li> AutomatedTheoremProving : </li>
    <li> CombinatorialMatrices : </li>
    <li> Cryptology (MG)
     <ol>
      <li> directly under Lisp </li>
      <li> Lisp/Cryptanalysis </li>
      <li> Lisp/CryptoSystems : </li>
     </ol>
    </li>
    <li> DataStructures : </li>
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
   Replacement of "makelist" by "create_list" (and supplying tests for all
   modified functions):
   <ol>
    <li> %Algebra : DONE </li>
    <li> AutomatedTheoremProving : DONE </li>
    <li> CombinatorialMatrices : DONE </li>
    <li> Cryptology (MG)
     <ol>
      <li> directly under Lisp </li>
      <li> Lisp/Cryptanalysis </li>
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
    <li> Satisfiability :
     <ol>
      <li> Autarkies : DONE </li>
      <li> Backdoors : DONE </li>
      <li> Backtracking </li>
      <li> BranchingTuples </li>
      <li> %ClauseSets : DONE </li>
      <li> ConflictCombinatorics : DONE </li>
      <li> ConstraintProblems </li>
      <li> Counting : DONE </li>
      <li> %Generators </li>
      <li> MinimalUnsatisfiability </li>
      <li> PropositionalLogic </li>
      <li> Reductions : DONE </li>
      <li> Resolutions : DONE </li>
      <li> Symmetries : DONE </li>
     </ol>
    </li>
    <li> TestSystem : DONE </li>
    <li> Trees : DONE </li>
   </ol>

  \par
   In ComputerAlgebra/plans/general.hpp the following topics are handled:
    - Transfer completed todos to the docus : DONE
    - Naming conventions : DONE
    - Input checking : DONE
    - Aldor integration : DONE (moved)
    - Documentation : DONE
    - %Test system : DONE


  \par Version 0.1.3

  \par ComputerAlgebra/Satisfiability version 0.0.7
  (see ComputerAlgebra/Satisfiability/plans/milestones.hpp)

  \par ComputerAlgebra/Graphs/Lisp version 0.1.3
  (see ComputerAlgebra/Graphs/Lisp/plans/milestones.hpp)
  
  \par
   In ComputerAlgebra/plans/Maxima.hpp the following topics are handled:
    - Maxima/CLisp bug
    - Plan the redesign

  \par
   Update the following milestones, towards version 0.2


  \par Version 0.2 THEME is "REDESIGN MAXIMA SYSTEM"

  \par
   In ComputerAlgebra/plans/Maxima.hpp the following topics are handled:
    - General design: Lists *here* are more fundamental than sets
    - Debugging
    - Documentation
    - Handling of demos
    - Monitoring
    - Contexts


  \par Version 0.2.1

  \par ComputerAlgebra/Mupad version 0.5.1

  \par
   In ComputerAlgebra/plans/Maxima.hpp the following topics are handled:
    - Recovering of partial results after long (unsuccessful) computations
    - Lisp integration
    - Collaboration with the Maxima community

  \par
   In ComputerAlgebra/plans/MaximaTechniques.hpp the following topics are
   handled:
    - Local variables
    - Declaring variables
    - Function application
    - List operations
    - Document important programming techniques
    - Partial compilation
    - Document simplification of expressions
    - Timing


  \par Version 0.2.2

  \par ComputerAlgebra/Satisfiability version 0.0.8
  (see ComputerAlgebra/Satisfiability/plans/milestones.hpp)

  \par ComputerAlgebra/Cryptology version 0.0.6
  (see ComputerAlgebra/Cryptology/plans/milestones.hpp)

  \par
   In ComputerAlgebra/Mupad/plans/general.hpp the following topics are handled:
    - Rewriting


  \par Version 0.2.3

  \par
   In ComputerAlgebra/plans/general.hpp the following topics are handled:
    - Frobenius problem
    - Dynamical systems


  \par Version 0.3

  \par
   Development at Axiom/Aldor level started.



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.1   : 2.7.2007; quite a few Mupad-functions produced (but without higher design, and not connected to the main concepts of (generalised) satisfiability).
   - 0.1.1 : 5.1.2008; concentration on Maxima: created a file-inclusion-system similar to the C++ system, and provided easy installation and access; also created general documentation, and clarified the roles of Maxima/Lisp vs. Axiom/Aldor. On the way also basic functionality regarding graphs, hypergraphs and satisfiability has been established in Maxima.

*/

