// Oliver Kullmann, 18.8.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef CONCEPTSVARIABLESWAECHTER_jakjdBCr420

#define CONCEPTSVARIABLESWAECHTER_jakjdBCr420

#include <utility>

#include <boost/concept_check.hpp>

#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Concepts_General.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Traits_Variables.hpp>

namespace Concepts_Variables {

  template <class Var>
  struct Variable_concept {
    Variable_concept();
    typedef typename Traits_General::MetaData<Var>::concept_tag concept_tag;

    void constraints() {
      boost::function_requires<Concepts_General::FullyConstructibleConcept<Var> >();
      boost::function_requires<Concepts_General::TotalOrderComparableConcept<Var> >();
      static_cast<bool>(v.null());
    }
    const Var v;
  };
  struct Variable_tag : virtual Concepts_General::Root_tag {};
  // "Var v;" creates a null variable
  // Order requirements:
  // -- null variables are equal:
  //    v.null() and w.null() -> v == w
  // -- any null variable is strictly less than any non-null variable:
  //    v.null() and not w.null() -> v < w
  // The null variable belongs to all collections (of the same type).
  // Property maps f for variables must work as maps, that is for
  // variables v, w with v == w we must always have f(v) == f(w)
  // (assuming v == w is defined).
  // Arbitrarily many null variables can be created.
  // For variable objects of type Var the functions ==, !=, <, >, <=, >=, Var::null() are constant (they are like int's) for null variables, and for all other variables objects if no invalidating reallocation happens.

  // Creation of non-null-variables only via the underlying variable domain VarDom, which is not accessible given the class Var alone!
  // More precisely, either a given object of type VarDom is needed, or one must create a new object (default-constructed).


  template <class VarD>
  struct VariableDomain_concept {
    VariableDomain_concept();
    typedef typename Traits_General::MetaData<VarD>::concept_tag concept_tag;

    typedef typename Traits_General::MetaData<VarD>::overflow_throw_property overflow_throw_property;
    typedef typename Traits_General::MetaData<VarD>::invalid_name_throw_property invalid_name_throw_property;
    typedef typename Traits_General::MetaData<VarD>::comparison_property comparison_property;
    typedef typename Traits_General::MetaData<VarD>::allocation_property allocation_property;
    typedef typename Traits_General::MetaData<VarD>::name_property name_property;
    typedef typename Traits_General::MetaData<VarD>::order_property order_property;

    typedef typename Traits_General::MetaData<VarD>::name_type name_type;
    typedef typename Traits_General::MetaData<VarD>::Var Var;

    void constraints() {
      boost::function_requires<boost::DefaultConstructibleConcept<VarD> >();
      boost::function_requires<Variable_concept<Var> >();
      boost::function_requires<boost::DefaultConstructibleConcept<name_type> >();
      boost::function_requires<boost::EqualityComparableConcept<name_type> >();
      static_cast<Var>(D(name));
      static_cast<const name_type&>(cD.name(v));
    }
    VarD D;
    const VarD cD;
    const name_type name;
    const Var v;
  };
  struct VariableDomain_tag : virtual Concepts_General::Root_tag {};
  // D(name_type()) == Var(),
  // name != name_type() => D(name) != Var().
  // D.name(D(name)) == name.
  // v.null() <=> D.name(v) == name_type().
  // Calling D.name(v) for a non-null variable v not created via D depends on VarD.
  // Creation and elimination of a new domain of type VarD does not affect existing domains of type VarD.
  // There is NO clear-function --- only destruction of domain objects!
  // Creation of variables can only throw an exception from the standard library.


  template <class VarD>
  struct VariableDomainWithSizeType_concept {
    // Refinement of VariableDomain_concept
    VariableDomainWithSizeType_concept();
    typedef typename Traits_General::MetaData<VarD>::concept_tag concept_tag;

    typedef typename Traits_General::MetaData<VarD>::size_type size_type;

    void constraints() {
      boost::function_requires<VariableDomain_concept<VarD> >();
      boost::function_requires<boost::ConvertibleConcept<typename Traits_General::MetaData<VarD>::overflow_throw_property, Traits_Variables::ThrowsTotalCapacityOverflow> >();
      boost::function_requires<Concepts_General::FullyConstructibleConcept<size_type> >();
      boost::function_requires<Concepts_General::TotalOrderComparableConcept<size_type> >();
      static_cast<size_type>(cD.max_size());
      static_cast<size_type>(s1 - s2);
      static_cast<size_type>(VarD::size_type_cast(ui));
    }
    const VarD cD;
    const size_type s, s1, s2;
    const unsigned int ui;
  };
  struct VariableDomainWithSizeType_tag : virtual VariableDomain_tag {};
  // Always s >= 0 holds.
  // s1 - s2 only defined for s1 >= s2 (then it is always correct).
  // If VarD::size_type_cast(ui) doesn't throw, then ui is faithfully represented with the result.


  template <class VarD>
  struct VariableDomainWithAllocation_concept {
    // Refinement of VariableDomainWithSizeType_concept
    VariableDomainWithAllocation_concept();
    typedef typename Traits_General::MetaData<VarD>::concept_tag concept_tag;

    typedef typename Traits_General::MetaData<VarD>::size_type size_type;

    void constraints() {
      boost::function_requires<VariableDomainWithSizeType_concept<VarD> >();
      D.reserve(s);
      static_cast<size_type>(cD.capacity());
      D.increase_capacity(s);
    }
    const VarD cD;
    VarD D;
    const size_type s;
    const unsigned int ui;
  };
  struct VariableDomainWithAllocation_tag : virtual VariableDomainWithSizeType_tag {};
  // Consider an object D of type VarD:
  // Always D.capacity() >= 1 holds.
  // Calling D.reserve(s) succesfully (i.e., without exception thrown), where D.capacity() was s0, guarantees, that s - s0 calls of VarD(name) with different names can be done without reallocation, and that D.capacity() >= s holds.
  // If D has just been default-created, then s0 = 1 can be used.
  // The capacity never decreases.
  // If D.increase_capacity(s) was succesful, then the capacity has been increased at least by s (exact arithmetic).


  template <class VarD>
  struct VariableDomainWithIterator_concept {
    // Refinement of VariableDomainWithSizeType_concept.
    VariableDomainWithIterator_concept();
    typedef typename Traits_General::MetaData<VarD>::concept_tag concept_tag;

    typedef typename Traits_General::MetaData<VarD>::size_property size_property;

    typedef typename Traits_General::MetaData<VarD>::iterator iterator;

    typedef typename Traits_General::MetaData<VarD>::name_type name_type;
    typedef typename Traits_General::MetaData<VarD>::size_type size_type;
    typedef typename Traits_General::MetaData<VarD>::Var Var;

    void constraints() {
      boost::function_requires<VariableDomainWithSizeType_concept<VarD> >();
      static_cast<size_type>(D.size()); // including the null variable
      static_cast<size_type>(D.n()); // excluding the null variable
      boost::function_requires<boost::Mutable_BidirectionalIteratorConcept<iterator> >();
      boost::function_requires<boost::ConvertibleConcept<typename iterator::value_type, Var> >();
      static_cast<iterator>(D.begin());
      static_cast<iterator>(D.end());
      static_cast<iterator>(D.first());
      static_cast<iterator>(D.find(name));
      static_cast<iterator>(v);
    }
    const VarD D;
    const Var v;
    const name_type name;
  };
  struct VariableDomainWithIterator_tag : virtual VariableDomainWithSizeType_tag {};
  // If D.size() < D.capacity(), then one call of D(name) is guaranteed not to invalidate any existing variable.
  // Always D.size() >= 1 and D.n() + 1 = Var::size().
  // VarD().size() == 1.
  // If D.find(name) != D.end(), then D(name) is the same as D, while otherwise D.size() is increased by one (except of the case where Traits_General::MetaData<VarD> is IndicesName, where the new size() is the maximum of the old size and the given index plus one).
  // After v = D(name) we have D.find(name) != D.end() and *D.find(name) == v.
  // D.find(name_type()) == D.begin().
  // If iterator i and iterator i+1 are dereferencable, then *i < *(i+1) holds.
  // *D.begin() == Var() (and since variables can be converted into iterators, and null variables are always equal, D.begin() never changes its value, except when reallocation happens; this implies that all valid iterators (as well as variables) don't change their values, if no reallocation happens).
  // D.first() == ++D.begin().


  template <class VarD>
  struct VariableDomainWithIteratorAndAllocation_concept {
    // Refinement of VariableDomainWithIterator_concept and VariableDomainWithAllocation_concept.
    VariableDomainWithIteratorAndAllocation_concept();
    typedef typename Traits_General::MetaData<VarD>::concept_tag concept_tag;

    void constraints() {
      boost::function_requires<VariableDomainWithIterator_concept<VarD> >();
      boost::function_requires<VariableDomainWithAllocation_concept<VarD> >();
    }
  };
  struct VariableDomainWithIteratorAndAllocation_tag : virtual VariableDomainWithIterator_tag, virtual VariableDomainWithAllocation_tag {};

}

#endif
