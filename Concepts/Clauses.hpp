// Oliver Kullmann, 24.7.2005 (Swansea)

/*!
  \file Concepts/Clauses.hpp
  \brief Concepts for clauses.
  \todo Concepts for and-clauses, or-clauses, etc.
  \todo A fundamental issue here is the relation to the concept of
  partial assignments. It should be possible to easily switch between
  the concepts of partial assignments and clauses; perhaps the only
  difference is in the complexity of the operations: while clauses correspond
  to sets (using space proportionally to their size), partial assignments
  correspond to total maps (using a special value "unassigned"; supporting
  constant time access). As set-objects partial assignments and clause-sets
  are identical (for standard literals), namely they are sets of pairs of variables
  and (forbidden) values, interpreted as constituting the assignment or as literals.
*/

#ifndef CLAUSES_0TRFvc

#define CLAUSES_0TRFvc

namespace OKlib {

  namespace Concepts {

  }

}

#endif
