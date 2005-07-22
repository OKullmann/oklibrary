// Oliver Kullmann, 5.6.2005 (Swansea)

#ifndef TRAITS_khBnBt6

#define TRAITS_khBnBt6

#include <boost/mpl/has_xxx.hpp>

namespace OKlib {

  namespace Concepts {

    /*!
      \class concept_tag
      \brief Traits metafunction, which returns no type in case concept_tag of T is not defined.
    */

    namespace implementation_concept_tag {
      BOOST_MPL_HAS_XXX_TRAIT_DEF(concept_tag);
      template <bool has_concept_tag, typename T>
      struct get_concept_tag {};
      template <typename T>
      struct get_concept_tag<true, T> {
        typedef typename T::concept_tag type;
      };
    }
    template <class T>
    struct concept_tag : implementation_concept_tag::get_concept_tag<implementation_concept_tag::has_concept_tag<T>::value, T> {};

    // -------------------------------------------------------------------------------------------------------------------

    template <typename T>
    struct index_type {
      typedef typename T::index_type type;
    };

    // -------------------------------------------------------------------------------------------------------------------

    template <typename T>
    struct string_type {
      typedef typename T::string_type type;
    };

    // -------------------------------------------------------------------------------------------------------------------

    template <typename T>
    struct floating_point_type {
      typedef typename T::floating_point_type type;
    };

    // -------------------------------------------------------------------------------------------------------------------

    template <typename T>
    struct natural_number_type {
      typedef typename T::natural_number_type type;
    };

    // -------------------------------------------------------------------------------------------------------------------

  }

}

#endif
