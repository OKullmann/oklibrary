// Oliver Kullmann, 25.5.2005 (Swansea)

#ifndef CONCEPTSMETAFUNCTIONS_olNb4T

#define CONCEPTSMETAFUNCTIONS_olNb4T

#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits.hpp>

#include "ConceptsBase.hpp"

namespace OKlib {

  namespace Concepts {

    template <class T>
    struct ConceptTag {
      typedef typename T::concept_tag type;
    };

    template <class T, class concept_tag>
    class IsModel {
      typedef typename ConceptTag<T>::type t_concept_tag;
    public :
      typedef typename 
      ::boost::mpl::or_< ::boost::is_same<t_concept_tag, concept_tag>, ::boost::is_base_and_derived<concept_tag, t_concept_tag> >::type type;
    };

    template <class T>
    class HasConcept {
      typedef typename ConceptTag<T>::type t_concept_tag;
    public :
      typedef typename
      ::boost::is_base_and_derived< ::OKlib::Concepts::ConceptsBase, t_concept_tag>::type type;
    };
  }

};

#endif
