// Oliver Kullmann, 15.5.2005

#ifndef ITERATORS_jnNzWz31

#define ITERATORS_jnNzWz31

#include <boost/concept_check.hpp>
#include <boost/concept_archetype.hpp>

#include "ConceptsBase.hpp"
#include "std_Iterators.hpp"

namespace OKlib {

  namespace Concepts {

    template <typename Iterator>
    struct MultiPassInputIterator {
      // According to [The Boost Graph Library, ISBN 0-201-72914-8], Section 16.3 (page 291).
      void constraints() {
        boost::function_requires<InputIterator<Iterator> >();
        // No further syntactical requirements.
      }
    };
    struct MultiPassInputIterator_tag : virtual InputIterator_tag {};

    template <typename T>
    struct MultiPassInputIterator_Archetype : InputIterator_Archetype<T> {};

  }

}

#endif
