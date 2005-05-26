// Oliver Kullmann, 25.5.2005 (Swansea)

#ifndef LIBRARYBASICS_oLkG5675

#define LIBRARYBASICS_oLkG5675

#include <boost/concept_check.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
// This file is guaranteed to include the last three files, so that OKLIB_HAS_CONCEPT_TAG and OKLIB_MODELS_CONCEPT always works.

#include "ConceptsBase.hpp"
#include "ConceptsMetafunctions.hpp"
#include "Basics.hpp"
// This file is guaranteed to include the last three files.

namespace OKlib {

  namespace Concepts {

    template <typename T>
    struct WithConceptTag {
      BOOST_STATIC_ASSERT(HasConceptTag<T>::value);
      void constraints() {}
    };
    struct WithConceptTag_tag : virtual ConceptsBase_tag {};

    class WithConceptTag_Archetype {
      WithConceptTag_Archetype();
      WithConceptTag_Archetype(const WithConceptTag_Archetype&);
      WithConceptTag_Archetype& operator=(const WithConceptTag_Archetype&);
      ~WithConceptTag_Archetype();
      struct concept_tag_ : virtual ConceptsBase_tag {};
    public :
      typedef concept_tag_ concept_tag;
    };

    template <typename T>
    struct BasicRequirements {
      void constraints() {
        ::boost::function_requires<WithConceptTag<T> >();
        ::boost::function_requires<ConstCorrect<T> >();
      }
    };
    struct BasicRequirements_tag : virtual WithConceptTag_tag, virtual ConstCorrect_tag {};

    class BasicRequirements_Archetype : public WithConceptTag_Archetype {};

    // -------------------------------------------------------------------------------------------------------------------------------
    
#define OKLIB_MODELS_CONCEPT_REQUIRES(C, concept) ::boost::function_requires<concept<C> >();

#define OKLIB_MODELS_CONCEPT_TAG(C, concept) BOOST_STATIC_ASSERT((::OKlib::Concepts::IsTagModel< C, concept ## _tag >::value));

  }

}

#endif
