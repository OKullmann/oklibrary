// Oliver Kullmann, 10.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Transitional/OKsolver/plans/general.hpp
  \brief Plans for the deprecated module containing old experimental code
  \deprecated To be dissolved soon.


  \todo Submodule "Experimental 200203"
  <ul>
   <li> AllgKlassen200203/Variables.hpp goes to
   Satisfiability/ProblemInstances/Variables. </li>
   <li> AllgKlassen200203/SetsOfVariables.hpp goes to
   Satisfiability/ProblemInstances/Variables. </li>
   <li> AllgKlassen200203/Concepts_Variables.hpp goes to
   Satisfiability/ProblemInstances/Variables. </li>
   <li> AllgKlassen200203/Tests_Variables.hpp goes to
   Satisfiability/ProblemInstances/Variables. </li>
   <li> AllgKlassen200203/Traits_Variables.hpp goes to
   Satisfiability/ProblemInstances/Variables. </li>
   <li> AllgKlassen200203/Domains.hpp goes to
   Satisfiability/ProblemInstances/Literals
   (or perhaps ProblemInstances/Domains ?). </li>
   <li> AllgKlassen200203/Literal.hpp goes to
   Satisfiability/ProblemInstances/Literals.hpp. </li>
   <li> AllgKlassen200203/Values.hpp goes to
   Satisfiability/ProblemInstances/Literals.hpp. </li>
   <li> AllgKlassen200203/Clauses.hpp goes to
   Satisfiability/ProblemInstances/Clauses. </li>
   <li> AllgKlassen200203/LitSets.hpp goes to
   Satisfiability/ProblemInstances/Clauses. </li>
   <li> AllgKlassen200203/ClauseSets.hpp goes to
   Satisfiability/ProblemInstances/ClauseSets. </li>
   <li> AllgKlassen200203/PartialAssignments.hpp goes to
   Satisfiability/Assignments. </li>
   <li> AllgKlassen200203/Auxiliary.hpp goes to
   Satisfiability/ProblemInstances or perhaps Satisfiability/Interfaces. </li>
   <li> AllgKlassen200203/AlgebraicStructures.hpp goes to
   Structures/Algebra. </li>
   <li> AllgKlassen200203/Concepts goes to
   Satisfiability/Interfaces. </li>
   <li> AllgKlassen200203/InputCls.hpp goes to
   Satisfiability/Interfaces. </li>
   <li> AllgKlassen200203/OutputCls.hpp goes to
   Satisfiability/Interfaces. </li>
   <li> AllgKlassen200203/Concepts_General.hpp goes to
   Concepts. </li>
   <li> AllgKlassen200203/ConceptDefinitions.hpp goes to
   Concepts. </li>
   <li> AllgKlassen200203/Test.cpp is distribued over
   the appropriate modules. </li>
   <li> AllgKlassen200203/Tests_General.hpp goes to TestSystem. </li>
   <li> AllgKlassen200203/Traits_General.hpp goes to the appropriate
   traits-directory. </li>
   <li> AllgKlassen200203/TransformationsBiclique.hpp goes to
   Satisfiability/Transformers/Generators.
    <ol>
     <li> We will also have a module for computing biclique-partitions ---
     perhaps it should go there. </li>
     <li> Update the code according to the OKlibrary coding standard, and
     create a demo program.
      <ul>
       <li> It seems that function
       TransformationsBiclique::symmetric_conflict_number_to_SAT is an
       old version, while class
       TransformationsBiclique::Symmetric_conflict_number_to_SAT is the
       newer version? </li>
       <li> DOXYGEN ERROR: function
       TransformationsBiclique::symmetric_conflict_number_to_SAT
       does not show up in the documentation?? </li>
       <li> Use the concept of a clause-set adaptor from
       Interfaces/InputOutput/ClauseSetAdaptors.hpp, instead of the concept
       of "F" as in function symmetric_conflict_number_to_SAT. </li>
       <li> DONE Update the includes from Transitional/General. </li>
       <li> DONE Remove the non-standard reference to macro "DEBUG". </li>
       <li> DONE Augment the include guard by a random string. </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> AllgKlassen200203/UnitPropagation goes to
   Satisfiability/Reductions/UnitClausePropagation. </li>
   <li> AllgKlassen200203/makefile will be replaced. </li>
  </ul>

*/
