// Oliver Kullmann, 26.5.2005

#ifndef VARIABLES_095tYb

#define VARIABLES _095tYb

#include "LibraryBasics.hpp"

namespace OKlib {

  namespace Concepts {

    struct Variables_tag : virtual BasicRequirements_tag, FullyConstructible_tag {};
    template <typename Var>
    struct Variables {
      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(Var, Variables);

        OKLIB_MODELS_CONCEPT_REQUIRES(Var, BasicRequirements);
        OKLIB_MODELS_CONCEPT_TAG(Var, BasicRequirements);
        OKLIB_MODELS_CONCEPT_REQUIRES(Var, FullyConstructible);
        OKLIB_MODELS_CONCEPT_TAG(Var, FullyConstructible);
        OKLIB_MODELS_CONCEPT_REQUIRES(Var, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_TAG(Var, FullyConstructible);
        // ToDo: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
      }
    };

    class Variables_Archetype {
    protected :
      struct convertible_to_bool {
        operator bool() {}
      };
    public :
      typedef Variables_tag concept_tag;
      convertible_to_bool operator ==(const Variables_Archetype&) const {}
    };
    
  }

}

#endif
