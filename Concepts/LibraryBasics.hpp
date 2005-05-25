// Oliver Kullmann, 25.5.2005 (Swansea)

#ifndef LIBRARYBASICS_oLkG5675

#define LIBRARYBASICS_oLkG5675

#include <boost/concept_check.hpp>
#include <boost/static_assert.hpp>

#include "ConceptsBase.hpp"
#include "ConceptsMetafunctions.hpp"

namespace OKlib {

  namespace Concepts {

    template <typename T>
    struct WithConcept {
      typedef typename ConceptTag<T>::type concept_tag;
      BOOST_STATIC_ASSERT(HasConcept<T>::type::value);
      void constraints() {}
    };
    struct WithConcept_tag : virtual ConceptsBase {};

    class WithConcept_Archetype {
      WithConcept_Archetype();
      WithConcept_Archetype(const WithConcept_Archetype&);
      WithConcept_Archetype& operator=(const WithConcept_Archetype&);
      ~WithConcept_Archetype();
      struct concept_tag_ : virtual ConceptsBase {};
    public :
      typedef concept_tag_ concept_tag;
    };
    

  }

}

#endif
