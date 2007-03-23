// Oliver Kullmann, 29.12.2005 (Swansea)

/*!
  \file Concepts/plans/Concepts.hpp
  \brief Plans for the concepts module

  \todo Update doxygen-documentation and plans

   - Concepts/AtomicConditions.hpp : DONE
   - Concepts/Basics.hpp : DONE
   - Concepts/ConceptsBase.hpp : DONE
   - Concepts/ConceptsMetafunctions.hpp : DONE
   - Concepts/InputOutput.hpp : DONE
   - Concepts/Iterators.hpp : DONE
   - Concepts/JoinedConceptTags.hpp : DONE
   - Concepts/LibraryBasics.hpp : DONE
   - Concepts/Literals.hpp : DONE
   - Concepts/RefactoringIncludeHandling.hpp : DONE
   - Concepts/ResultElements.hpp : DONE
   - Concepts/plans/Variables.hpp : DONE
   - Concepts/std_Basics.hpp :
   - Concepts/std_Container.hpp :
   - Concepts/std_Iterators.hpp :

   - Concepts/plans/ActiveClauseSets.hpp : DONE
   - Concepts/plans/Assignments.hpp :
   - Concepts/plans/BigIntegers.hpp :
   - Concepts/plans/BipartiteGraphs.hpp :
   - Concepts/plans/ClauseSets.hpp :
   - Concepts/plans/Clauses.hpp :
   - Concepts/plans/Hypergraphs.hpp :
   - Concepts/plans/PartialAssignments.hpp : DONE

  \todo Develop concepts:
   - atomic conditions
   - literals
   - partial assignments
   - active clause-sets
   - clauses and clause-sets.

  \todo Warnings:
  Eliminate all warnings.

  \todo Tests:
  Move to the new test system.

  \todo Formulating concepts:
  What is the difference for example between
  bool b = static_cast<bool>(e);
  and
  bool b = e;
  ?! I would assume that here there is none. And in general?
*/

/*!
  \namespace OKlib::Concepts
  \brief Concept definitions and accompanying generic tests
*/

namespace OKlib {
  namespace Concepts {
  }
}
