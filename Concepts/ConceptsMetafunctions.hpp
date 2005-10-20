// Oliver Kullmann, 25.5.2005 (Swansea)

#ifndef CONCEPTSMETAFUNCTIONS_olNb4T

#define CONCEPTSMETAFUNCTIONS_olNb4T

/*!
  \file ConceptsMetafunctions.hpp
  \brief Metafunctions concerning the relations between concepts, concept tags and models; used mainly internally.
*/

#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/has_xxx.hpp>

#include "Logical.hpp"

#include "ConceptsBase.hpp"
#include "Traits.hpp"

namespace OKlib {

  namespace Concepts {

    /*!
      \class IsTagModel
      \brief Boolean metafunction: true iff concept tag of T is derived from concept_tag.
    */

    template <class T, class concept_tag>
    class IsTagModel {
      typedef typename ::OKlib::Concepts::concept_tag<T>::type t_concept_tag;
    public :
      typedef typename 
      ::boost::mpl::or_< ::boost::is_same<t_concept_tag, concept_tag>, ::boost::is_base_and_derived<concept_tag, t_concept_tag> >::type type;
      OKLIB_META_VALUE_T
    };

     // -------------------------------------------------------------------------------------------------------------------

    /*!
      \class IsConceptTag
      \brief Boolean metafunction: true iff Tag is strictly derived from ::OKlib::Concepts::ConceptsBase_tag.
    */

    template <class Tag>
    struct IsConceptTag : ::boost::is_base_and_derived< ::OKlib::Concepts::ConceptsBase_tag, Tag> {};

    /*!
      \class HasConceptTag
      \brief Boolean metafunction: true iff T has a nested type concept_tag which is a concept tag.
    */

    namespace implementation_has_concept_tag {
      BOOST_MPL_HAS_XXX_TRAIT_DEF(type)
      template <bool has_concept_tag, typename T>
      struct is_concept_tag : ::boost::mpl::bool_<false> {};
      template <typename T>
      struct is_concept_tag<true, T> : IsConceptTag<typename concept_tag<T>::type> {};
    }
    template <class T>
    struct HasConceptTag : implementation_has_concept_tag::is_concept_tag<implementation_has_concept_tag::has_type<concept_tag<T> >::value, T> {};

  }

}

#endif
