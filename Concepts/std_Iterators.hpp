// Oliver Kullmann, 15.5.2005

// Concepts according to
// ISO/IEC 14882: 2003
// Section 24.1 and Section 24.3.1

#ifndef STDITERATORS_pOpt5185

#define STDITERATORS_pOpt5185

#include <iterator>

#include <boost/concept_check.hpp>

#include "ConceptsBase.hpp"

namespace OKlib {

  namespace Concepts {

    template <typename Iterator>
    struct InputIterator {
      typedef typename std::iterator_traits<Iterator>::value_type value_type;
      typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
      typedef typename std::iterator_traits<Iterator>::difference_type difference_type;

      typedef typename std::iterator_traits<Iterator>::reference reference;
      typedef typename std::iterator_traits<Iterator>::pointer pointer;
      // ToDo: "reference" and "pointer" is never explained in the standard, and the requirements on iterator concepts don't mention them, but they suddenly show up in the iterator traits (24.3.1) ?
      void constraints() {
        Iterator u(a);
        u = a;
        static_cast<bool>(a == b); // ToDo: is this exactly "is convertible" ?
        static_cast<bool>(a != b);
        static_cast<value_type>(*a);
        static_cast<Iterator&>(++r);
        (void) r++;
        static_cast<value_type>(*r++);
      }
      const Iterator a, b; // ToDo: It seems the standard forgot to add constness ?
      Iterator& r;
    };
    // ToDO: Can expressions "a -> m" be expressed ?
    struct InputIterator_tag : virtual ConceptsBase {};

    template <typename T>
    class InputIterator_Archetype {
    private:
      typedef InputIterator_Archetype self;
      struct internal {
        T operator* () {}
      };
    public:
      typedef std::input_iterator_tag iterator_category;
      typedef T value_type;
      typedef std::ptrdiff_t difference_type;
      typedef void pointer; // ToDo: left unspecified in the standard ?
      struct reference {
        operator T() {}
      };
      struct convertible_to_bool {
        operator bool() {}
      };
      self& operator=(const self&) {}
      convertible_to_bool operator==(const self&) const {}
      convertible_to_bool operator!=(const self&) const {}
      reference operator*() const {}
      self& operator++() {}
      internal operator++(int) {}
    };

    // --------------------------------------------------------------------------------------------------------------------

  }

}

#endif

