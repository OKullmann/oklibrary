// Oliver Kullmann, 15.5.2005 (Swansea)

/*!
  \file Concepts/std_Iterators_Tests.hpp
  \brief Tests for validating whether a class models one of the five iterator concepts from the standard.

  No tests for output iterators.

  \todo Write the test functions:
   - InputIterator has only one exploitable (additional) semantical property (besides the
     properties of ConstructibleCAEq), namely that if i == j for iterators i,j and (i,j) is in
     the domain of == (which can be empty), where i, j are dereferencable, that then
     *i and *j are substitution-equivalent. (Note that a model of InputIterator in general is not
     a model of EqualitySubstitutable.) If value_type is a model is EqualityComparable,
     then we have
     (i == j, (i,j) in the domain of == and i, j dereferencable => *i == *j),
     and this should be put into an axiom.
     Note: It seems to be possible, that although == is syntactically allowed, application always
     leads to undefined behaviour.
   - For MultiPassInputIterator we have EqualitySubstitutable and thus i == j and i, j dereferencable implies ++i == ++j.
   - ForwardIterator has default constructors, and i == j iff either i, j are not dereferencable or
     both are dereferencable and *i is the same object as *j (this is stronger than *i == *j).
     The domain of == is total (note, that there is no such thing as a "valid iterator": If ++ or
     * is applied on non-dereferencable iterators, then the result is undefined in general, and
     thus the whole program is broken).
   - BidirectionalIterator: additionally --++i == i and ++--i == i (the latter not in the standard?)
   - RandomAccessIterator: "i+n" has the same meaning as operation ++ applied n-times to i
     (and so on).
     Furthermore we have LinearOrder.
*/

#ifndef STDITERATORSTESTS_oopl5Bb
#define STDITERATORSTESTS_oopl5Bb

#include <Transitional/Concepts/std_Iterators.hpp>

namespace OKlib {
  namespace Concepts {

  }

}

#endif
