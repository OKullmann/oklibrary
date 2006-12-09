// Oliver Kullmann, 7.12.2006 (Swansea)

/*!
  \file AtomicConditions/TrivialAtomicConditions.hpp
  \brief Typedefs for trivial models of concept AtomicConditions

  \todo Test AC_bool semantically.
*/

#ifndef TRIVIALATOMICCONDITIONS_jBffRql9
#define TRIVIALATOMICCONDITIONS_jBffRql9

#include <Transitional/AtomicConditions/traits/value_type.hpp>
#include <Transitional/AtomicConditions/eval.hpp>

namespace OKlib {
  namespace AtomicConditions {

    /*!
      \typedef AC_bool
      \brief Boolean values as atomic conditions
    */

    typedef bool AC_bool;

    namespace traits {

      template <>
      struct value_type<AC_bool> {
        typedef bool type;
      };
    }

    template <>
    inline bool eval(const AC_bool& ac, const bool& b) {
      return ac == b;
    }

  }

  namespace Concepts {
    namespace traits {
      
      template <>
      struct concept_tag<OKlib::AtomicConditions::AC_bool> {
        typedef OKlib::Concepts::AtomicCondition_tag type;
      };

    }
  }
}

#endif
