// Oliver Kullmann, 3.3.2006 (Swansea)

/*!
  \file Transitional/plans/milestones.hpp
  \brief Milestones for Transitional
*/

/*!
  \page CurrentVersions Current versions

  - Transitional \transitional_version_full
  - OKlibrary \oklib_version_full
*/

/*!
  \page PageTargetsTransitional Targets for Transitional

  \section StartingVersion Future milestones

  \subsection VersionTransitional014 0.1.4 : TO BE REACHED \deadline_014
  \par Buildsystem version 0.1.5 (MH (OK))
  (see Buildsystem/plans/milestones.hpp)
  \par Concepts version 0.1.2 (OK (MH))
  (see Concepts/plans/milestones.hpp)
  \par Messages version 0.7.5 (OK)
  (see Messages/plans/milestones.hpp)
  \par TestSystem version 0.1.4 (OK)
  (see TestSystem/plans/milestones.hpp)
  \par InputOutput version 0.0.8 (OK (MH))
  (see Transitional/InputOutput/plans/milestones.hpp)
  \par AtomicConditions version 0.0.3 (OK (MH))
  (see AtomicConditions/plans/milestones.hpp)
  \par Literals version 0.0.4 (OK (MH))
  (see Literals/plans/milestones.hpp)
  \par PartialAssignments version 0.0.5 (OK (MH))
  (see PartialAssignments/plans/milestones.hpp)
  \par InjectivityConstraints version 0.0.3 (OK, MH)
  (see InjectivityConstraints/plans/milestones.hpp)
  \par LatinSquares version 0.0.5 (OK, MH)
  (see LatinSquares/plans/milestones.hpp)
  \par Expand the following milestones regarding SAT-algorithms

  \subsection VersionTransitional015 0.1.5 : TO BE REACHED \deadline_015
  \par Buildsystem version 0.1.6 (MH, OK)
  (see Buildsystem/plans/milestones.hpp)
  \par Concepts version 0.1.3 (OK (MH))
  (see Concepts/plans/milestones.hpp)
  \par Graphs version  0.0.3 (OK (MH))
  (see Graphs/plans/milestones.hpp)
  \par HypergraphColouring version 0.0.5 (OK (MH))
  (see HypergraphColouring/plans/milestones.hpp)
  \par OKsolver_2_0 version 0.0.5 (OK (MH))
  (see OKsolver_2_0/plans/milestones.hpp)
  \par QuantumPhysics version 0.0.8 (OK)
  (see QuantumPhysics/plans/milestones.hpp)
  \par Messages version 0.7.6 (OK)
  (see Messages/plans/milestones.hpp)
  \par Plans
  All modules have a milestones-file, and the milestones
  are integrated into the overall milestones (here).
  \par Update
  Update the subsequent milestones.

  \subsection VersionTransitional016 0.1.6 : TO BE REACHED \deadline_016
  \par Messages version 0.7.7 (OK)
  (see Messages/plans/milestones.hpp)
  \par TestSystem version 0.1.5 (OK)
  (see TestSystem/plans/milestones.hpp)
  \par ComplexitySystem version 0.1 (MH, OK)
  (see ComplexitySystem/plans/milestones.hpp)
  \par Concepts version 0.1.4 (OK (MH))
  (see Concepts/plans/milestones.hpp)
  \par Cryptanalysis version 0.0.3 (OK)
  (see Cryptanalysis/plans/milestones.hpp)

  \subsection VersionTransitional017 0.1.7
  \par Buildsystem version 0.2 (MH, OK)
  (see Buildsystem/plans/milestones.hpp)
  \par Messages version 0.7.8 (OK)
  (see Messages/plans/milestones.hpp)
  \par TestSystem version 0.2 (OK)
  (see TestSystem/plans/milestones.hpp)
  \par ComplexitySystem version 0.2 (MH, OK))
  (see ComplexitySystem/plans/milestones.hpp)

  \subsection VersionTransitional018 0.1.8
  \par Buildsystem version 0.3
  (see Buildsystem/plans/milestones.hpp)
  \par Messages version 0.7.9 (OK)
  (see Messages/plans/milestones.hpp)


  \subsection VersionTransitional019 0.1.9
  \par Buildsystem version 0.4
  (see Buildsystem/plans/milestones.hpp)
  \par Messages version 0.8 (OK)
  (see Messages/plans/milestones.hpp)

  \subsection VersionTransitional0110 0.1.10
  \par Buildsystem version 0.7
  (see Buildsystem/plans/milestones.hpp)

  \subsection VersionTransitional0111 0.1.11
  \par Buildsystem version 0.5
  (see Buildsystem/plans/milestones.hpp)
  \par Messages version 0.8
  (see Messages/plans/milestones.hpp)

  \subsection VersionTransitional0112 0.1.12
  \par Messages version 0.9
  (see Messages/plans/milestones.hpp)

  \subsection VersionTransitional0113 0.1.13
  \par Buildsystem version 1.0
  (see Buildsystem/plans/milestones.hpp)

  \section VersionTransitional020 0.2.0 Build and test system basically finished, and basic concepts for generalised SAT solving established
  \par Messages version 1.0
  (see Messages/plans/milestones.hpp)
  \par ProgramOptions version ???
  \par ErrorHandling version ???
  \par General goals:
   - Transition to new test system complete (all old tests transferred to the new test system).
   - Except of warnings coming from Boost, no compiler warnings from our library.


  \section VersionTransitional030 0.3.0 Complexity system ready
  Complexity system completed and (partially) used

  \section VersionTransitional040 0.4.0 Concepts ready
  Basic concepts all written (C++ and library-infrastructure-related),
  and the generic test system framework established and in use.

  \section VersionTransitional050 0.5.0 Documentation ready
  Documentation completed (regarding the basics)

  \section VersionTransitional060 0.6.0 Package management ready
  Extending the build system regarding package management (other
  users)

  \section VersionTransitional100 1.0.0 Foundations completed
  A solid foundation is provided, now we can start.
*/

/*!
   \page VersionHistoryTransitional Version history Transitional

   \par 0.1.0 3/3/2006

   Initial version number (here we just started versioning, while there
   has been work on the library for around 2 years).

   \par 0.1.1 11/12/2006

   - the buildsystem-architecture cleaned-up, improved documentation-building
   - new test-system further cleaned-up (especially regarding log-messages)
   - module Messages further expanded and improved
   - started with concepts and prototypes for literals and atomic conditions
   - planning activities on many parts of the library.

   \par 0.1.2 14/1/2007

   - the buildsystem-architecture further improved, improved building of external libraries
     and documentation
   - module Messages further expanded, new utilities
   - further clean-up of the new test system
   - concepts for literals and atomic conditions expanded.

   \par 0.1.3 8/2/2007

   - moving the buildsystem towards a specified and testable system
   - initial clean-up of module Messages
   - strengthened the new test system by systematically using message-classes.

*/

/*!
  \page VersionHistoryOKlibrary Version history OKlibrary

  \par 0.0.1 3/3/2006

  Initial version number (library basically empty).
*/



/*!
  \namespace OKlib
  \brief All components of the OKlibrary
*/

namespace OKlib {
}

