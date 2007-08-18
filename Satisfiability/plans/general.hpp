// Oliver Kullmann, 16.6.2007 (Swansea)

/*!
  \file Transitional/Satisfiability/plans/general.hpp
  \brief Contains all super-modules regarding (generalised) satisfiability
  issues


  \todo Milestones
  <ul>
   <li> It seems that at least for now, there are no milestones at the level
   of super-module Satisfiability. </li>
   <li> Having milestones also at this level seems artificial (for now).
   One wants to say directly that Solvers or Heuristics should advance,
   without introducing artificial combination-milestones. </li>
   <li> However, like namespace administration, there are todos also at
   this level? Perhaps for now they go directly to Transitional/plans? </li>
   <li> The tree structure of the planning system (setting milestones must
   follow the hierarchies: the "manager" cannot directly give orders
   to the "workers") is a bit of a hindrance here, but in general it
   seems appropriate. </li>
  </ul>


  \todo Update namespaces.

*/

/*!
  \namespace OKlib::Satisfiability
  \brief The part of the OKlibrary concerned with (generalised) satisfiability

  Alias "Sat".
*/

namespace OKlib {
  namespace Satisfiability {
  }
  namespace Sat = Satisfiability;
}

