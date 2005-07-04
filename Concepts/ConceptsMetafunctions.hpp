// Oliver Kullmann, 25.5.2005 (Swansea)

#ifndef CONCEPTSMETAFUNCTIONS_olNb4T

#define CONCEPTSMETAFUNCTIONS_olNb4T

/*!
  \file ConceptsMetafunctions.hpp
  \brief Metafunctions concerning the relations between concepts and models
*/

#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits.hpp>

#include "Logical.hpp"

#include "ConceptsBase.hpp"
#include "Traits.hpp"

namespace OKlib {

  namespace Concepts {

    /*!
      \class IsTagModel<T, concept_tag>
      \brief Boolean metafunction: true iff concept tag of T is derived from concept_tag.
    */

    template <class T, class concept_tag>
    class IsTagModel {
      typedef typename ::OKlib::Concepts::concept_tag<T>::type t_concept_tag;
    public :
      typedef typename 
      ::boost::mpl::or_< ::boost::is_same<t_concept_tag, concept_tag>, ::boost::is_base_and_derived<concept_tag, t_concept_tag> >::type type;
      OKLIB_META_VALUE_T;
    };

     // -------------------------------------------------------------------------------------------------------------------

    /*!
      \class IsConceptTag<Tag>
      \brief Boolean metafunction: true iff Tag is strictly derived from ::OKlib::Concepts::ConceptsBase_tag.
    */

    template <class Tag>
    struct IsConceptTag : ::boost::is_base_and_derived< ::OKlib::Concepts::ConceptsBase_tag, Tag> {};

    /*!
      \class HasConceptTag<T>
      \brief Boolean metafunction: true iff T has a nested typ concept_tag which is a concept tag.
    */

    template <class T>
    class HasConceptTag {
      typedef typename concept_tag<T>::type t_concept_tag;
    public :
      typedef typename
      IsConceptTag<t_concept_tag>::type type;
      OKLIB_META_VALUE_T;
    };

  }

};

#endif
