// Oliver Kullmann, 26.12.2005 (Swansea)

/*!
  \file Iterators/ImplicitConversionsBoost.hpp
  \brief Extending boost iterators by implicit conversions

  \todo Once we can use directory structures, likely classes should be seperated into
  files so that each file includes only one boost header file.

  \todo One should test, whether these constructions hamper run-time and run-space
  efficiency (with compiler optimisation turned on they should not).
*/

#ifndef IMPLICITCONVERSIONSBOOST_88uJJnhhg
#define IMPLICITCONVERSIONSBOOST_88uJJnhhg

#include <boost/iterator/counting_iterator.hpp>

namespace OKlib {

  namespace Iterators {

    /*!
      \class counting_iterator
      \brief counting_iterator<Incrementable> extends boost::counting_iterator<Incrementable>
      by providing an implicit conversion from Incrementable to counting_iterator.

      \todo Example where this extension is really useful?
    */

    template <class Incrementable, class CategoryOrTraversal = boost::use_default, class Difference = boost::use_default>
    struct counting_iterator : boost::counting_iterator<Incrementable, CategoryOrTraversal, Difference> {
      typedef boost::counting_iterator<Incrementable, CategoryOrTraversal, Difference> base_type;
      counting_iterator() {}
      counting_iterator(Incrementable x) : base_type(x) {}
      counting_iterator(const base_type& ci) : base_type(ci) {}
    };

  }

}

#endif
