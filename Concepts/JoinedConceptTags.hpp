// Oliver Kullmann, 9.12.2006 (Swansea)

/*!
  \file Concepts/JoinedConceptTags.hpp
  \brief Concept tags for (fundamental) types in case the type models several concepts
*/

#ifndef JOINEDCONCEPTTAGS_74TTba14kfbr
#define JOINEDCONCEPTTAGS_74TTba14kfbr

#include <Transitional/Concepts/Variables.hpp>
#include <Transitional/Concepts/Literals.hpp>

namespace OKlib {
  namespace Concepts {

    /*!
      \class VariablesLiterals_tag
      \brief Tags for types which are used as variables and as literals

      For examples int models Variables as well as Literals.
    */

    template <class Var_tag, class Lit_tag>
    struct VariablesLiterals_tag : virtual Var_tag, virtual Lit_tag {
      OKLIB_MODELS_CONCEPT_TAG_T(Var_tag, OKlib::Concepts::Variables)
      OKLIB_MODELS_CONCEPT_TAG_T(Lit_tag, OKlib::Concepts::Literals)
    };

  }
}

#endif
