// Oliver Kullmann, 6.12.2006 (Swansea)

/*!
  \file AtomicConditions/plans/AtomicConditions.hpp
  \brief Plans for atomic conditions

  \todo Further trivial atomic conditions
  - Perhaps Mono<int n>, where n >= 1. Contains an integer m,
    which in case m >= 1 means the singleton {m}, while
    in case m <= -1 the co-singleton {1,...,n} - {-m} is meant.
    Invariant -n <= m <= n.

  \todo First prototype of general atomic condition
  Very first prototype using std::set (for an arbitrary value-type)
  or std::vector<bool> (for value-type {1,...,n}).

*/

/*!
  \namespace OKlib::AtomicConditions
  \brief Components for atomic conditions
*/

namespace OKlib {
  namespace AtomicConditions {
  }
}

