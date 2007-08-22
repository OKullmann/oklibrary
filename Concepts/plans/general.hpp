// Oliver Kullmann, 29.12.2005 (Swansea)

/*!
  \file Concepts/plans/Concepts.hpp
  \brief Plans for the concepts module

  \todo Split the module:
  The module Concepts is becoming too big and one is loosing overlook.
  So it should split, with a recursive make under directory Concepts,
  into sub-modules
  <ul>
   <li> SAT
    <ul>
     <li> Concepts/plans/ActiveClauseSets.hpp </li>
     <li> Concepts/plans/AtomicConditions.hpp </li>
     <li> Concepts/plans/ClauseSets.hpp </li>
     <li> Concepts/plans/Clauses.hpp </li>
     <li> Concepts/plans/Literals.hpp </li>
     <li> Concepts/plans/PartialAssignments.hpp </li>
     <li> Concepts/Variables.hpp </li>
     <li> Concepts/JoinedConceptTags.hpp </li>
     <li> Concepts/ResultElements.hpp </li>
    </ul>
   </li>
   <li> Graphs (or "Combinatorics" ?)
    <ul>
     <li> Concepts/plans/BipartiteGraphs.hpp </li>
     <li> Concepts/plans/Hypergraphs.hpp </li>
    </ul>
   </li>
   <li> Numbers (or "Algebra" ?, "Mathematics" ?, "Math" ?)
    <ul>
     <li> Concepts/plans/BigIntegers.hpp </li>
    </ul>
   </li>
   <li> Std
    <ul>
     <li> Concepts/std_Basics.hpp </li>
     <li> Concepts/std_Container.hpp </li>
     <li> Concepts/std_Iterators.hpp </li>
    </ul>
   </li>
   <li> StdExt
    <ul>
     <li> Concepts/ConceptsBase.hpp </li>
     <li> Concepts/plans/Basics.hpp </li>
     <li> Concepts/plans/LibraryBasics.hpp </li>
     <li> Concepts/plans/InputOutput.hpp </li>
     <li> Concepts/plans/Iterators.hpp </li>
    </ul>
   </li>
   <li> Refactoring (perhaps more generally "Languages" ? or "Lang" ??)
    <ul>
     <li> Concepts/plans/RefactoringIncludeHandling.hpp </li>
    </ul>
   </li>
   <li> Meta
    <ul>
     <li> Concepts/plans/ConceptsMetafunctions.hpp </li>
    </ul>
   </li>
  </ul>
  with Concepts/plans containing general Concepts/plans/Concepts.hpp and
  Concepts/plans/milestones.hpp.
  Would be good to have refactoring-tools ready for the necessary renamings.

  \todo Cannibalise the approach from 200203:
  Transfer if appropriate concepts from
  OKsolver/Experimental/AllgKlassen200203/ConceptDefinitions.hpp,
  and once finished, mark this file to be obsolete.

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
