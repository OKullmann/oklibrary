// Oliver Kullmann, 24.5.2005 (Swansea)

#ifndef BASICSTESTS_oLzW151

#define BASICSTESTS_oLzW151

#include "Basics.hpp"

namespace OKlib {

  namespace Concepts {

    namespace Archetype_tests {
      //ToDo: Move to Testobjects

      template class FullyEqualityComparable<FullyEqualityComparable_Archetype>;

      template class FullyLessThanComparable<FullyLessThanComparable_Archetype>;

      template class LinearOrder<LinearOrder_Archetype>;

      template class EqualitySubstitutable<EqualitySubstitutable_Archetype>;
    }

  }

}

#endif
