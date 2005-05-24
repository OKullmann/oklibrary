// Oliver Kullmann, 24.5.2005 (Swansea)

#ifndef STDBASICSTESTS_8455tGb

#define STDBASICSTESTS_8455tGb

#include "std_Basics.hpp"

namespace OKlib {

  namespace Concepts {

    namespace Archetype_tests {

      template class EqualityComparable<EqualityComparable_Archetype>;

      template class LessThanComparable<LessThanComparable_Archetype>;

      template class Destructible<Destructible_Archetype>;

      template class CopyConstructible<CopyConstructible_Archetype>;

      template class Assignable<Assignable_Archetype>;

      

    }

  }

}

#endif
