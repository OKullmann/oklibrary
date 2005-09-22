// Oliver Kullmann, 15.5.2005

// Concepts according to
// ISO/IEC 14882: 2003
// Section 24.1 and Section 24.3.1
// PLUS const-correctness.

/*!
  \file std_Iterators.hpp
  \brief Iterator concepts from ISO/IEC 14882: 2003 (plus const-correctness).
*/

#ifndef STDITERATORS_pOpt5185

#define STDITERATORS_pOpt5185

#include <iterator>

#include <boost/concept_check.hpp>

#include "ConceptsBase.hpp"
#include "std_Basics.hpp"
#include "Basics.hpp"

namespace OKlib {

  namespace Concepts {

    /*!
      \class InputIterator
      \brief Concept InputIterator according to the standard.

      Refines concepts FullyEqualityComparable, Assignable, CopyConstructible.
      \todo "reference" and "pointer" are never explained in the standard, and the requirements on iterator concepts don't mention them, but they suddenly show up in the iterator traits (24.3.1) ?
      \todo Can expressions "a -> m" be expressed ?
    */

    template <typename Iterator>
    struct InputIterator {
      typedef typename std::iterator_traits<Iterator>::value_type value_type;
      typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
      typedef typename std::iterator_traits<Iterator>::difference_type difference_type;

      typedef typename std::iterator_traits<Iterator>::reference reference;
      typedef typename std::iterator_traits<Iterator>::pointer pointer;
      void constraints() {
        boost::function_requires<FullyEqualityComparable<Iterator> >();
        boost::function_requires<Assignable<Iterator> >();
        boost::function_requires<CopyConstructible<Iterator> >();
        static_cast<value_type>(*a);
        static_cast<Iterator&>(++r);
        (void) r++;
        static_cast<value_type>(*r++);
      }
      const Iterator a, b;
      Iterator& r;
    };
    struct InputIterator_tag : virtual ConceptsBase_tag {};

    class InputIterator_Archetype {
      InputIterator_Archetype();
      typedef InputIterator_Archetype self;
    protected :
      struct convertible_to_bool {
        operator bool() {}
      };
    public:
      typedef std::input_iterator_tag iterator_category;
      struct value_type {};
      typedef std::ptrdiff_t difference_type;
      typedef void pointer; // ToDo: left unspecified in the standard ?
      struct reference {
        operator value_type() {}
      };
      self& operator=(const self&) {}
      convertible_to_bool operator==(const self&) const {}
      convertible_to_bool operator!=(const self&) const {}
      reference operator*() const {}
      self& operator++() {}
      struct internal {
        value_type operator* () {}
      };
      internal operator++(int) {}
    };

    // --------------------------------------------------------------------------------------------------------------------

  }

}

#endif

