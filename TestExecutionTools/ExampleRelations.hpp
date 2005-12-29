// Oliver Kullmann, 27.12.2005 (Swansea)

/*!
  \file ExampleRelations.hpp
  \brief Examples for binary predicates.
*/

#ifndef EXAMPLERELATIONS_jjHg56

#define EXAMPLERELATIONS_jjHg56

#include <functional>

namespace OKlib {

  namespace TestExecutionTools {

    /*!
      \class Id
      \brief Identity relation.

      Id<T> is nearly the same as std::equal_to<T> (with the only differences,
      that we are precise about the argument type (which is const T&), that the
      bracket-operator member function is const, and that the default constructor
      is explicitely defined so that const objects of type Id can be defined).
    */

    template <typename T>
    struct Id : std::binary_function<const T&, const T&, bool> {
      Id() {}
      bool operator ()(const T& x, const T& y) const {
        return x == y;
      }
    };

    /*!
      \class Total
      \brief Total relation.

      See Id.
    */

    template <typename T>
    struct Total : std::binary_function<const T&, const T&, bool> {
      Total() {}
      bool operator ()(const T& x, const T& y) const {
        return true;
      }
    };

    /*!
      \class Empty
      \brief Empty relation.

      See Id.
    */

    template <typename T>
    struct Empty : std::binary_function<const T&, const T&, bool> {
      Empty() {}
      bool operator ()(const T& x, const T& y) const {
        return false;
      }
    };

  }

}

#endif
