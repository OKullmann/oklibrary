// Oliver Kullmann, 26.5.2005

/*!
  \file Variables.hpp
  \brief Concepts for Variables.
  \todo Is a generalisation of the Variables-concept useful, where there is no default constructor, and one handling of singular values? (Perhaps with the name "GeneralIndex" ?)
*/

#ifndef VARIABLES_095tYb

#define VARIABLES_095tYb

#include "TypeTraits.hpp"

#include "LibraryBasics.hpp"
#include "Traits.hpp"

namespace OKlib {

  namespace Concepts {

    struct Variables_tag : virtual BasicRequirements_tag, virtual FullyConstructible_tag,  virtual EqualitySubstitutable_tag, virtual LinearOrder_tag {};

    template <typename Var>
    struct Variables {
      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(Var, Variables);

        OKLIB_MODELS_CONCEPT_REQUIRES(Var, BasicRequirements);
        OKLIB_MODELS_CONCEPT_TAG(Var, BasicRequirements);
        OKLIB_MODELS_CONCEPT_REQUIRES(Var, FullyConstructible);
        OKLIB_MODELS_CONCEPT_TAG(Var, FullyConstructible);
        OKLIB_MODELS_CONCEPT_REQUIRES(Var, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_TAG(Var, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_REQUIRES(Var, LinearOrder);
        OKLIB_MODELS_CONCEPT_TAG(Var, LinearOrder);

        static_cast<bool>(v); static_cast<bool>(vc);
      }

      Var v;
      const Var vc;
    };

    class Variables_Archetype {
    protected :
      struct convertible_to_bool {
        operator bool() {}
      };
    public :
      typedef Variables_tag concept_tag;
      convertible_to_bool operator ==(const Variables_Archetype&) const {}
      convertible_to_bool operator !=(const Variables_Archetype&) const {}
      convertible_to_bool operator <(const Variables_Archetype&) const {}
      convertible_to_bool operator >(const Variables_Archetype&) const {}
      convertible_to_bool operator <=(const Variables_Archetype&) const {}
      convertible_to_bool operator >=(const Variables_Archetype&) const {}
      operator bool() const {}
    };
    
    // --------------------------------------------------------------------------------------------------------------------------------------

    struct VariablesWithIndex_tag : virtual Variables_tag {};
    
    template <typename Var>
    struct VariablesWithIndex {

      typedef typename index_type<Var>::type index_type;
      BOOST_STATIC_ASSERT(::OKlib::MetaProgramming::is_unqualified_signedunsigned_integral<index_type>::value);

      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(Var, VariablesWithIndex);

        OKLIB_MODELS_CONCEPT_REQUIRES(Var, Variables);
        OKLIB_MODELS_CONCEPT_TAG(Var, Variables);

        static_cast<index_type>(v);
        static_cast<index_type>(vc);
      }

      Var v;
      const Var vc;
    };

    struct VariablesWithIndex_Archetype : Variables_Archetype {
      typedef VariablesWithIndex_tag concept_tag;
      typedef int index_type; // ToDo: Can this be made more generic?
      operator index_type() const {}
    };

    // --------------------------------------------------------------------------------------------------------------------------------------

    struct VariablesAsIndex_tag : virtual VariablesWithIndex_tag {};
    
    template <typename Var>
    struct VariablesAsIndex {

      typedef typename index_type<Var>::type index_type;

      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(Var, VariablesAsIndex);

        OKLIB_MODELS_CONCEPT_REQUIRES(Var, VariablesWithIndex);
        OKLIB_MODELS_CONCEPT_TAG(Var, VariablesWithIndex);

        Var(i);
        Var(ic);
      }

      index_type i; const index_type ic;
    };

    struct VariablesAsIndex_Archetype : VariablesWithIndex_Archetype {
      typedef VariablesAsIndex_tag concept_tag;
      VariablesAsIndex_Archetype() {}
      explicit VariablesAsIndex_Archetype(index_type) {}
    };
  }

}

#endif
