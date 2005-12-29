// Oliver Kullmann, 26.5.2005

/*!
  \file Concepts/Variables.hpp
  \brief Concepts for Variables.
  \todo Is a generalisation of the Variables-concept useful, where there is no default
  constructor, and no handling of singular values? (Perhaps with the name "GeneralIndex" ?)
  \todo It seems now artificial to me that variables are ordered by < : There is no default
  meaning --- it could be the order as found in the input, or the order in the quantifier
  prefix, etc. So only refinements extend == to a linear order, using special
  predicate names, and there are refinements using <, which then is purely
  implementation-defined (for pointers for example, while for variables with index
  it's the index).
  \todo Variables can have *domains*, which are types (with values), for example bool
  or enumerations. Partial assignments restrict these domains further. Refinements
  allow only special domains, for example an enumeration with N values, or
  bool.
  \todo There are variable categories
   - Parameter (the "default")
   - Universal
   - Existential.
  For a variable type Var the category is given with
  OKlib::Variables::Traits::category_type<Var>::type,
  which can be Parameter, Universal, Existential or Varyingly.
  With OKlib::Variables::category(v) we get an enumerated value of type
  enum Variable_categories {is_parameter = 0, is_existential = 1, is_universal = 2}.
  The types Parameter, Universal, Existential or Varyingly are nearly numeric constants in the
  MPL sense, only that in the MPL we must have an integral value, while we have an
  enumeration. So with
  OKlib::Variables::Traits::category_type<Var>::type::value
  we get the compile-time constant.
  The default of OKlib::Variables::category(v) is
  OKlib::Variables::Traits::category_type<Var>::type::value, i.e.,
  namespace OKlib { namespace Variables {
  template <typename Var>
  Variable_categories category(Var v) {
    return OKlib::Variables::Traits::category_type<Var>::type::value;
  }}}
  This should be placed in OKlib/Variables/category.hpp.
  We need the types for overloading, while we need the value to have a compile-time
  constant.
  \todo Active clause-sets use "variable structures" to manage their variables, allowing
    for example to run through them, etc.
    These variable structures should have a type member showing which combination
    is possible (in principle).
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
        operator bool() { return bool(); }
      };
    public :
      typedef Variables_tag concept_tag;
      convertible_to_bool operator ==(const Variables_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator !=(const Variables_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator <(const Variables_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator >(const Variables_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator <=(const Variables_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator >=(const Variables_Archetype&) const { return convertible_to_bool(); }
      operator bool() const { return bool(); }
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
