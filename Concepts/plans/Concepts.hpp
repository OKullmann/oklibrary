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
   - Concepts/std_Basics.hpp : DONE
   - Concepts/std_Container.hpp : DONE
   - Concepts/std_Iterators.hpp : DONE

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

  \todo Develop a general strategy for creating generic tests:
  <ul>
   <li> In the current scheme with basic tests for one, two, three objects:
    <ul>
     <li> Shouldn't there be a template parameter for the number of objects? </li>
     <li> Where is the syntax check applied? </li>
     <li> In general, how to avoid overlapping tests (or should this be
          ignored in general, and handled only in special cases?!) ? </li>
    </ul>
   </li>
   <li> As proposed somewhere in the specific concept (test) files, should the
   specific parts of the test-names (for example, the concept to be tested)
   become namespaces? Similar for the concept-tags and archetypes? Thus having
   new sub-namespaces (e.g.)
    - Concepts::tags
    - Concepts::archetypes
    - Concepts::tests::special_concept
   </li>
  </ul>

  \todo Warnings:
  Eliminate all warnings.

  \todo Tests:
  Move to the new test system.

  \todo Technical problems when formulating concepts:
  <ul>
   <li> What is the difference for example between
    \code bool b = static_cast<bool>(e); \endcode
   and
    \code bool b = e; \endcode ?!
   I would assume that here there is none. And in general?
   </li>
  </ul>

  \todo Complete the concepts and tests belonging to the standard:
  <ul>
   <li> Concepts/std_Basics.hpp and tests </li>
   <li> Concepts/std_Container.hpp and tests </li>
   <li> Concepts/std_Iterators.hpp and tests </li>
  </ul>
   
*/

/*!
  \namespace OKlib::Concepts
  \brief Concept definitions and accompanying generic tests
*/

namespace OKlib {
  namespace Concepts {
  }
}
