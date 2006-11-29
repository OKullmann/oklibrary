// Oliver Kullmann, 30.12.2005 (Swansea)

/*!
  \file Concepts/traits/concept_tag.hpp
  \brief Traits-metafunctions to extract the concept tag.
*/

#ifndef CONCEPTTAG_ialLkj7
#define CONCEPTTAG_ialLkj7

#include <boost/mpl/has_xxx.hpp>

namespace OKlib {
  namespace Concepts {
    namespace traits {

      /*!
        \class concept_tag
        \brief Traits metafunction, which returns no type in case concept_tag of T is not defined.
      */

      namespace implementation_concept_tag {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(concept_tag)
          template <bool has_concept_tag, typename T>
        struct get_concept_tag {};
        template <typename T>
        struct get_concept_tag<true, T> {
          typedef typename T::concept_tag type;
        };
      }
      template <class T>
      struct concept_tag : implementation_concept_tag::get_concept_tag<implementation_concept_tag::has_concept_tag<T>::value, T> {};

    }

  }

}

#endif
