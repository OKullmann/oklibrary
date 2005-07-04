// Oliver Kullmann, 15.5.2005

/*!
  \file Iterators.hpp
  \brief Concepts for iterators extending the concepts from the standard.
*/

#ifndef ITERATORS_jnNzWz31

#define ITERATORS_jnNzWz31

#include <boost/concept_check.hpp>
#include <boost/concept_archetype.hpp>

#include "ConceptsBase.hpp"
#include "std_Iterators.hpp"

namespace OKlib {

  namespace Concepts {

    /*!
      \class MultiPassInputIterator<Iterator>
      \brief Concept MultiPassInputIterator according to Boost.
    */

    template <typename Iterator>
    struct MultiPassInputIterator {
      // According to [The Boost Graph Library, ISBN 0-201-72914-8], Section 16.3 (page 291).
      void constraints() {
        boost::function_requires<InputIterator<Iterator> >();
        // No further syntactical requirements.
      }
    };
    struct MultiPassInputIterator_tag : virtual InputIterator_tag {};

    struct MultiPassInputIterator_Archetype : InputIterator_Archetype {};

  }

}

#endif
