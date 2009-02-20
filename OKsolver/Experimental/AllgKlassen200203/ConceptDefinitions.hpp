// Oliver Kullmann, 27.7.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/Experimental/AllgKlassen200203/ConceptDefinitions.hpp
  \brief First approaches for concepts in the framework of the library
  \deprecated To be moved (if appropriate) to OKlib.
*/


#ifndef CONCEPTDEFINITIONSWAECHTER

#define CONCEPTDEFINITIONSWAECHTER

#include <ostream>

#include <boost/concept_check.hpp>
#include <boost/concept_archetype.hpp>

#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Auxiliary.hpp>

// DEPRICATED (to be eliminated)

namespace ConceptDefinitions {

  // -----------------------------------------------------------
  // Basic Concepts
  // -----------------------------------------------------------

  template <typename T>
  struct FullyConstructibleConcept {
    FullyConstructibleConcept();
    void constraints() {
      boost::function_requires<boost::DefaultConstructibleConcept<T> >();
      boost::function_requires<boost::CopyConstructibleConcept<T> >();
      boost::function_requires<boost::AssignableConcept<T> >();
    }
  };
    
  template <typename T>
  struct OutputStreamableConcept {
    OutputStreamableConcept();
    void constraints() {
      o << v;
    }
    T v;
    std::ostream o;
  };

  // -----------------------------------------------------------
  // Order Concepts
  // -----------------------------------------------------------

  template <class T>
  struct TotalOrderComparableConcept {
    TotalOrderComparableConcept();
    void constraints() {
      boost::function_requires<boost::LessThanComparableConcept<T> >();
      boost::function_requires<boost::EqualityComparableConcept<T> >();
      // For a, b of type T we must have:
      // If not (a < b) and not (b < a) then a == b.
    }
  };

  // -----------------------------------------------------------
  // Concept Tags
  // -----------------------------------------------------------

  struct Root_tag {};

  // -----------------------------------------------------------
  // Container Concepts
  // -----------------------------------------------------------

  template <class C>
  struct FixedGlobalContainer_concept {
    // Using
    // C c;
    // creates the global container if necessary.
    FixedGlobalContainer_concept();
    typedef typename C::Concept Concept;
    typedef typename C::size_type size_type;
    typedef typename C::iterator iterator;
    void constraints() {
      boost::function_requires<TotalOrderComparableConcept<C> >();
      boost::function_requires<FullyConstructibleConcept<C> >();
      static_cast<size_type>(C::size());
      boost::function_requires<boost::BidirectionalIteratorConcept<iterator> >();
      boost::function_requires<boost::ConvertibleConcept<typename iterator::value_type, C> >();
      boost::function_requires<boost::ConvertibleConcept<typename iterator::difference_type, size_type> >();
      static_cast<iterator>(C::begin());
      static_cast<iterator>(C::end());
      boost::function_requires<boost::ConvertibleConcept<C, iterator> >();
    }
  };
  struct FixedGlobalContainer_tag {};

  // -----------------------------------------------------------
  // Domain Concepts
  // -----------------------------------------------------------

  template <class T>
  struct FiniteDomain_concept : FixedGlobalContainer_concept<T> {
    FiniteDomain_concept();
    typedef typename T::Concept Concept;
    typedef typename T::iterator iterator;
    typedef typename T::set set;
    typedef typename T::insertion_status insertion_status;
    void constraints() {
      boost::function_requires<FixedGlobalContainer_concept<T> >();
      static_cast<T>(T::min());
      static_cast<T>(T::max());
      boost::function_requires<FullyConstructibleConcept<set> >();
      static_cast<insertion_status>(T::forced_assignment);
      static_cast<insertion_status>(T::contradiction_created);
      static_cast<insertion_status>(T::no_critical_change);
      static_cast<insertion_status>(s.insert(e));
      // MUST immediately be checked!
      static_cast<T>(s.unit());
      // MUST immediately be applied after a forced assignment was created
      o << e; // as index from 0 to T::size() - 1
    }
    set s;
    T e;
    std::ostream o;
  };
  struct FiniteDomain_tag : FixedGlobalContainer_tag {};
  // iterator() == T::end();
  // T::min() == T()
  // T::size() >= 1, T::begin() != T::end()

  template <class B>
  struct BooleanDomain_concept : FiniteDomain_concept<B> {
    BooleanDomain_concept();
    void constraints() {
      boost::function_requires<FiniteDomain_concept<B> >();
      boost::function_requires<boost::ConvertibleConcept<B, bool> >();
      static_cast<B>(B::True()); // B::True == B::max()
      static_cast<B>(B::False()); // B::False == B::min()
      B(false); B(true); // explicit
      // output via stream inserter as boolean (compatible with output
      // as indices if boolean-output flag not set (the default))
    }
  };
  struct BooleanDomain_tag : FiniteDomain_tag {};
  
    
  // -----------------------------------------------------------
  // Algebraic Concepts
  // -----------------------------------------------------------

  template <class S>
  struct Semiring_concept {
    Semiring_concept();
    typedef typename S::Concept Concept;
    void constraints() {
      boost::function_requires<FixedGlobalContainer_concept<S> >();
      // S() depends on the ring!
      static_cast<S&>(x += y);
      static_cast<S&>(x *= y);
      static_cast<S>(x + y);
      static_cast<S>(x * y);
      static_cast<S>(S::identity_plus());
      static_cast<S>(S::identity_times());
      o << x;
      // (S, +, identity_plus()) is a commutative monoid
      // (S, *, identity_times()) is a monoid
      // x * (y + z) = x * y + x * z
      // (x + y) * z = x * z + y * z\\
      // identity_plus() * x = identity_plus()
    }
    S x, y;
    std::ostream o;
  };
  struct Semiring_tag : FixedGlobalContainer_tag {};

  template <class S>
  struct CommutativeSemiring_concept {
    CommutativeSemiring_concept();
    typedef typename S::Concept Concept;
    void constraints() {
      boost::function_requires<Semiring_concept<S> >();
    }
  };
  struct CommutativeSemiring_tag : Semiring_tag {};

  template <class S>
  struct SignRing_concept : CommutativeSemiring_concept<S> {
    SignRing_concept();
    void constraints() {
      boost::function_requires<CommutativeSemiring_concept<S> >();
      static_cast<S>(S::Negative());
      static_cast<S>(S::Null());
      static_cast<S>(S::Positive());
      // output via stream inserter as integers -1, 0, 1
      // S() == S::Null()
      // S::Negative() < S::Null() < S::Positive()
      // For "S a, b;":
      // a + b == max(a, b)
      // a * b == min(a, b)
    }
  };
  struct SignRing_tag : CommutativeSemiring_tag {};
  
  // -----------------------------------------------------------
  // Variable Concepts
  // -----------------------------------------------------------

  template <class Var>
  struct Variable_concept {
    Variable_concept();
    typedef typename Var::Concept Concept;
    void constraints() {
      boost::function_requires<FullyConstructibleConcept<Var> >();
      boost::function_requires<TotalOrderComparableConcept<Var> >();
      boost::function_requires<boost::DefaultConstructibleConcept<NameType> >();
      Var::clear();
      static_cast<bool>(v.null());
      Var(name);
      static_cast<const NameType&>(v.name());
      o << v;
    }
    typedef typename Var::NameType NameType;
    const NameType name;
    const Var v;
    std::ostream o;
  };
  struct Variable_tag {};
  // "Var v;" creates a null variable
  // Var().name() == NameType()
  // Var(name).name() == name
  // o << v has the same effect as o << v.name() except of the case
  // v.null(), where the effect is the same as
  // o << Auxiliary::null_variable_tag
  // Order requirements:
  // -- null variables are equal:
  //    v.null() and w.null() -> v == w
  // -- any null variable is strictly less than any non-null variable:
  //    v.null() and not w.null() -> v < w
  // Var::clear() means a complete reset (including the info value of the
  // null variable) if there is global access to the variables;
  // otherwise it might be a no-op.
  
  template <class Var>
  struct VariableWithHistory_concept {
    VariableWithHistory_concept();
    void constraints() {
      boost::function_requires<Variable_concept<Var> >();
    }
  };
  struct VariableWithHistory_tag : Variable_tag {};
  // Order requirements:
  // -- non-null variables are ordered according to their construction times.

  template <class Var>
  struct VariableWithCounting_concept {
    VariableWithCounting_concept();
    void constraints() {
      boost::function_requires<VariableWithHistory_concept<Var> >();
      static_cast<size_type>(Var::size()); // including the null variable
      static_cast<size_type>(Var::n()); // excluding the null variable
      Var::reserve(s);
      static_cast<size_type>(Var::capacity());
    }
    typedef typename Var::size_type size_type;
    const size_type s;
  };
  struct VariableWithCounting_tag : VariableWithHistory_tag {};
  // Always Var::size() >= 1 and Var::n() + 1 = Var::size();
  // After Var::clear() it holds Var::size() == 1.
  
  template <class Var>
  struct VariableWithInfo_concept {
    VariableWithInfo_concept();
    void constraints() {
      boost::function_requires<VariableWithCounting_concept<Var> >();
      boost::function_requires<boost::Mutable_BidirectionalIteratorConcept<Var> >();
      static_cast<Var>(Var::begin());
      static_cast<Var>(Var::end());
      static_cast<Var>(Var::find(name));
    }
    Var v;
    typedef typename Var::NameType NameType;
    NameType name;
  };
  struct VariableWithInfo_tag : Variable_tag {};
  // Var::end() is a singular variable (without a name, can not be
  // dereferenced, different from all other variables).
  // Var::find("") == Var::begin()
  // after Var v(name) we have Var::find(name) == v.
  // if never a variable with name name_ was created, then
  // Var::find(name_) == Var::end()
  
  template <class Policy>
  struct VariableIndexInfoPolicy_concept {
    VariableIndexInfoPolicy_concept();
    typedef typename Policy::Concept Concept;
    void constraints() {
      
      struct D : Policy {
	typedef typename Policy::DeliveredConcept DeliveredConcept;
	typedef typename Policy::InfoValueType InfoValueType;
	typedef typename Policy::InfoReferenceType InfoReferenceType;
	typedef typename Policy::InfoPointerType InfoPointerType;
	typedef typename Policy::Index Index;
	D() {
          Policy::new_info();
	  reserve_info(Index());
	  static_cast<InfoReferenceType>(get_info(Index()));
	  static_cast<InfoPointerType>(get_info_pointer(Index()));
	  void clear_info();
	}
      };

      typedef typename D::Index Index;
      typedef typename D::InfoValueType InfoValueType;
      boost::function_requires<FullyConstructibleConcept<Index> >();
      // there are more requirements on Index
      boost::function_requires<FullyConstructibleConcept<InfoValueType> >();
    }
  };
  struct VariableIndexInfoPolicy_tag {};
  // clear_info() is called, when Var::clear() is called.


  // -----------------------------------------------------------
  // Sets of Variables Concepts
  // -----------------------------------------------------------

  template <class VarSet>
  struct SetOfVariables_concept {
    SetOfVariables_concept();
    typedef typename VarSet::Concept Concept;
    typedef typename VarSet::Var Var;
    typedef typename VarSet::iterator iterator;
    typedef typename VarSet::const_iterator const_iterator;
    typedef typename VarSet::size_type size_type;
    void constraints() {
      boost::function_requires<Variable_concept<Var> >();
      boost::function_requires<FullyConstructibleConcept<VarSet> >();
      VarSet(v);
      boost::function_requires<boost::ForwardIteratorConcept<const_iterator> >();
      boost::function_requires<boost::ForwardIteratorConcept<iterator> >();
      static_cast<iterator>(cvs.begin());
      static_cast<iterator>(cvs.end());
      boost::function_requires<boost::ConvertibleConcept<typename iterator::value_type, Var> >();
      static_cast<size_type>(cvs.size());
      static_cast<bool>(cvs.empty());
      vs.clear();
    }
    VarSet vs;
    const Var v;
    const VarSet cvs;
  };
  struct SetOfVariables_tag {};
  // if iterator i is dereferencable, then (not i -> null())
  // thus in VarSet(v) it is not v.null() assumed.

  template <class VarSet>
  struct SingletonSetOfVariables_concept {
    SingletonSetOfVariables_concept();
    typedef typename VarSet::Var Var;
    void constraints() {
      boost::function_requires<SetOfVariables_concept<VarSet> >();
      boost::function_requires<boost::ConvertibleConcept<VarSet, Var> >();
    }
  };
  struct SingletonSetOfVariables_tag : SetOfVariables_tag {};
  // If for VarSet vs we have vs.empty(), then Var(vs).null();
  // otherwise Var(vs) is the single element of vs.
  // Thus vs.size() <= 1.
  
  // -----------------------------------------------------------
  // Partial Assignments Concepts
  // -----------------------------------------------------------

  template <class AbstrSpec>
  struct AbstractSpecifier_concept {
    AbstractSpecifier_concept();
    typedef typename AbstrSpec::Concept Concept;
    void constraints() {
      boost::function_requires<FullyConstructibleConcept<AbstrSpec> >();
    };
  };
  struct AbstractSpecifier_tag {};

  template <class Spec>
  struct SpecifierWithVariables_concept {
    SpecifierWithVariables_concept();
    typedef typename Spec::VarSet VarSet;
    void constraints() {
      boost::function_requires<AbstractSpecifier_concept<Spec> >();
      boost::function_requires<SetOfVariables_concept<VarSet> >();
      static_cast<VarSet>(phi.var());
    };
    Spec phi;
  };
  struct SpecifierWithVariables_tag : AbstractSpecifier_tag {};
  
  template <class PartAssig>
  struct BasicPartialAssignment_concept {
    BasicPartialAssignment_concept();
    typedef typename PartAssig::Var Var;
    typedef typename PartAssig::Dom Dom;
    void constraints() {
      boost::function_requires<AbstractSpecifier_concept<PartAssig> >();
      boost::function_requires<Variable_concept<Var> >();
      boost::function_requires<FiniteDomain_concept<Dom> >();
      static_cast<typename Dom::iterator&>(phi[v]);
      static_cast<typename Dom::iterator>(psi[v]);
      phi.clear();
    }
    PartAssig phi;
    const PartAssig psi;
    Var v;
  };
  struct BasicPartialAssignment_tag : AbstractSpecifier_tag {};
  // "PartAssign phi;" creates the empty partial assignments
  // phi[Var()] is undefined


  // -----------------------------------------------------------
  // Literals Concepts
  // -----------------------------------------------------------

  template <class Lit>
  struct GeneralLiteral_concept {
    GeneralLiteral_concept();
    typedef typename Lit::Concept Concept;
    typedef typename Lit::Spec Spec;
    void constraints() {
      boost::function_requires<FullyConstructibleConcept<Lit> >();
      boost::function_requires<TotalOrderComparableConcept<Lit> >();
      boost::function_requires<AbstractSpecifier_concept<Spec> >();
      static_cast<bool>(x.invariant_true()); // fast
      static_cast<bool>(x.invariant_false()); // not so fast
      static_cast<bool>(x.invariant());
      static_cast<bool>(x.not_invariant());
      static_cast<Auxiliary::Evaluation_status>(x.eval_status());
      static_cast<Lit&>(y.apply(phi));
      static_cast<Lit&>(y.apply_non_invariant(phi));
      static_cast<Lit>(phi * x);
    }
    const Spec phi;
    const Lit x;
    Lit y;
  };
  struct GeneralLiteral_tag {};
  // x.invariant() <-> not x.not_invariant()
  // x.open() <-> not x.invariant_false() and not x.invariant_true()
  // x.eval_status == Auxiliary::inv_false <-> x.invariant_false()
  // x.eval_status == Auxiliary::inv_true <-> x.invariant_true()
  // x.var().empty() <-> not x.open()
  // x.invariant_false() and y.invariant_true -> x != y
  // not x.open() -> x == x.apply(phi)
  // Lit().invariant_true()
  // Application of y.apply_non_invariant(phi) assumes y.not_invariant().

  template <class Lit>
  struct LiteralWithVariables_concept {
    LiteralWithVariables_concept();
    typedef typename Lit::Spec::VarSet VarSet;
    void constraints() {
      boost::function_requires<GeneralLiteral_concept<Lit> >();
      boost::function_requires<SpecifierWithVariables_concept<Lit> >();
      static_cast<VarSet>(x.var());
    }
    const Lit x;
  };
  struct LiteralWithVariables_tag : GeneralLiteral_tag {};

  template <class Lit>
  struct AtomicLiteral_concept {
    AtomicLiteral_concept();
    typedef typename Lit::Spec::VarSet VarSet;
    void constraints() {
      boost::function_requires<LiteralWithVariables_concept<Lit> >();
      boost::function_requires<SingletonSetOfVariables_concept<VarSet> >();
    }
  };
  struct AtomicLiteral_tag : LiteralWithVariables_tag {};

  template <class Lit>
  struct ValueLiteral_concept {
    ValueLiteral_concept();
    typedef typename Lit::Spec Spec;
    typedef typename Lit::VarSet VarSet;
    typedef typename Spec::Dom Dom;
    typedef typename Spec::Var Var;
    void constraints() {
      boost::function_requires<AtomicLiteral_concept<Lit> >();
      boost::function_requires<BasicPartialAssignment_concept<Spec> >();
      boost::function_requires<boost::ConvertibleConcept<typename VarSet::Var, Var> >();
      Lit(v, d);
      static_cast<Dom&>(x.val());
      static_cast<const Dom>(y.val());
    }
    const Var v;
    const Dom d;
    Lit x;
    const Lit y;
  };
  struct ValueLiteral_tag : AtomicLiteral_tag {};
  // For Lit(v, d) we must have not v.null().
  // For literals x, y:
  // x.invariant_false() and y.invariant_false() -> x == y.
  // x.invariant_true() and y.invariant_true() -> x == y
  // For x.val() we must have x.not_invariant().


  template <class Lit>
  struct ValueLiteralWithInfo_concept {
    ValueLiteralWithInfo_concept();
    typedef typename Lit::value_type value_type;
    typedef typename Lit::reference reference;
    void constraints() {
      boost::function_requires<ValueLiteral_concept<Lit> >();
      static_cast<reference>(*x);
      Lit::initialise_infos();
    }
    Lit x;
  };
  struct ValueLiteralWithInfo_tag : ValueLiteral_tag{};

  template <class Policy>
  struct LiteralsAsPairsInfoPolicy_concept {
    LiteralsAsPairsInfoPolicy_concept();
    typedef typename Policy::Concept Concept;

    void constraints() {

      struct D : Policy {
	
      };
    }
  };

  // -----------------------------------------------------------
  // Global functions
  // -----------------------------------------------------------

  // Var(x), Var(C), Var(F), Var(phi) : Mengen
  // im Falle x ist ein atomares Literal: Menge ist "Einer-Menge"
  
}

#endif
