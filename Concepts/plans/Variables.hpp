// Oliver Kullmann, 26.5.2005

/*!
  \file Concepts/plans/Variables.hpp
  \brief Plans for concepts for Variables.

  \todo Create doxygen-documentation.

  \todo Transfer tests to the new test system.

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

