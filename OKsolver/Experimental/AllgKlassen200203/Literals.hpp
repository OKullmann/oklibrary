// Oliver Kullmann, 1.3.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef LITERALSWAECHTER

#define LITERALSWAECHTER

#include <string>
#include <cstdlib>
#include <vector>
#include <utility>

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

#include <Transitional/OKsolver/Experimental/AllgKlassen200203/Variables.hpp>
#include <Transitional/OKsolver/Experimental/AllgKlassen200203/Auxiliary.hpp>

namespace Literals {

  template <class PartialAssignments, class SingletonVarSet, template <class Var, class Dom> class InfoPolicyTemplate>
  class VariableValuePairs : public InfoPolicyTemplate<typename PartialAssignments::Var, typename PartialAssignments::Dom> {

    BOOST_CLASS_REQUIRE(PartialAssignments, ConceptDefinitions, BasicPartialAssignment_concept);
    BOOST_CLASS_REQUIRE(SingletonVarSet, ConceptDefinitions, SingletonSetOfVariables_concept);
    BOOST_STATIC_ASSERT((boost::is_convertible<typename SingletonVarSet::Var, typename PartialAssignments::Var>::value));

    typedef typename PartialAssignments::Var Var;
    typedef typename PartialAssignments::Dom Dom;
    typedef typename Dom::iterator dom_iterator;
    typedef InfoPolicyTemplate<Var, Dom> InfoPolicy;

    Var v;
    dom_iterator d;

  public :

    typedef typename InfoPolicy::DeliveredConcept Concept;
    typedef PartialAssignments Spec;
    typedef SingletonVarSet VarSet;
    
    VariableValuePairs() {}
    VariableValuePairs(Var v, Dom val) : v(var), d(val) {}
    
    friend inline bool operator ==(VariableValuePairs lhs, VariableValuePairs rhs) { return (lhs.v.null() and rhs.v.null()) or (lhs.d == Dom::end() and rhs.d == Dom::end()) or (lhs.v == rhs.v and lhs.d == rhs.d); }
    friend inline bool operator !=(VariableValuePairs lhs, VariableValuePairs rhs) { return not (lhs == rhs); }
    friend inline bool operator <(VariableValuePairs lhs, VariableValuePairs rhs) { return (lhs.v < rhs.v) or (lhs.v == rhs.v and lhs.d < rhs.d); }
    friend inline bool operator >(VariableValuePairs lhs, VariableValuePairs rhs) { return rhs < lhs; }
    friend inline bool operator <=(VariableValuePairs lhs, VariableValuePairs rhs) { return lhs < rhs or lhs == rhs; }
    friend inline bool operator >=(VariableValuePairs lhs, VariableValuePairs rhs) { return lhs > rhs or lhs == rhs; }

    bool invariant_true() const { return v.null(); }
    bool invariant_false() const { return not v.null() and d == Dom::end(); }
    bool invariant() const { return v.null() or d == Dom::end(); }
    bool not_invariant() const { return not v.null() and not d == Dom::end(); }
    Auxiliary::Evaluation_status eval_status() const {
      if (v.null())
	return Auxiliary::inv_true;
      else if (d == Dom::end())
	return Auxiliary::inv_false;
      else
	return Auxiliary::not_inv;
    }

    VariableValuePairs& apply(const Spec& phi) {
      if (invariant())
	return *this;
      else
	return apply_non_invariant(phi);
    }
    VariableValuePairs& apply_non_invariant(const Spec& phi) {
      assert(not_invariant());
      const dom_iterator val = phi[v];
      if (val != Dom::end())
	if (val == d)
	  d = Dom::end();
	else
	  v = Var();
      return *this;
    }

    VarSet var() const {
      if (invariant())
	return VarSet();
      else
	return VarSet(v);
    }
    Dom val() const {
      assert(not_invariant());
      return *d;
    }
  };

  template <class Lit>
  Lit operator *(const typename Lit::Spec& phi, Lit x) {
    return x.apply(phi);
  }

  // -------------------
  // Info policy classes
  // -------------------

  template <class Var, class Dom>
  class InfoPolicyEmpty {
  public :
    //typedef ConceptDefinitions::VariableIndexInfoPolicy_tag Concept;
  protected :
    typedef ConceptDefinitions::ValueLiteral_tag DeliveredConcept;
    ~InfoPolicyEmpty() {}
    class Dummy {};
    typedef Dummy InfoReferenceType;
    typedef Dummy InfoValueType;
    typedef Dummy InfoPointerType;
    // get_info not implemented
    // get_info_pointer not implemented
    static void clear_info() {}
    // ES FEHLT NOCH VERSCHIEDENES
  };

  
}

// DEPRICATED

#include "Values.hpp"

namespace Literals {

  // --------------------------------------------
  // Literals as integers
  // --------------------------------------------

  struct Occurrences {
    int pos, neg;
  };

  template <typename Info, typename Index>
  class InfoPolicyVector {
  public :
    typedef Info InfoValueType;
    typedef Info& InfoReferenceType;
  private :
    struct centered_vector {
      typedef typename std::vector<Info> InfoVectorType;
      InfoVectorType v;
      typename InfoVectorType::iterator centre;
      centered_vector() : v(1), centre(v.begin()) {}
    };
    static centered_vector info_vector;
  protected :
    struct Link {
      static void new_variable() {
	// ATTENTION: invalidates all existing access to info containers
	const typename centered_vector::InfoVectorType::difference_type d = info_vector.centre - info_vector.v.begin();
	info_vector.v.push_back(Info());
	info_vector.v.push_back(Info());
	info_vector.centre = info_vector.v.begin() + d + 1;
      }
      static void reserve(Index max) {
	info_vector.v.reserve(2 * max + 1);
      }
    };
    Info& get_info(Index i) const {
       return *(info_vector.centre + i);
    }
    ~InfoPolicyVector() {}
  };
  template <typename Info, typename Index>
  typename InfoPolicyVector<Info,Index>::centered_vector InfoPolicyVector<Info,Index>::info_vector;


  template <typename Index>
  struct InfoPolicyVectorOccurrences : InfoPolicyVector<Occurrences, Index> {};
  

  template <typename Index, template <typename Index> class InfoPolicyVar, template <typename Index> class InfoPolicyLit, typename Name>
  class LiteralsAsIntegers : public InfoPolicyLit<Index> {
  public :

    typedef Variables::VariablesAsIndices<InfoPolicyVar<Index>, Name, typename InfoPolicyLit<Index>::Link > Var;
    typedef Values::BooleanValues Val;

    LiteralsAsIntegers() : lit(0) {};
    LiteralsAsIntegers(Var var, Val val) : lit((val == Val(Val::zero)) ? var.index : - var.index) {}
    
    LiteralsAsIntegers(const LiteralsAsIntegers& x) : lit(x.lit) {}

    operator bool() const { return lit != 0; }

    Var var() const {
      return Var(std::abs(lit), Var::avoid_confusion);
    }

    Val val() const {
      if (lit > 0)
	return Val(Val::zero);
      else
	return Val(Val::one);
    }

    operator std::pair<const Var, Val>() const {
      return std::make_pair(var(), val());
    }

    LiteralsAsIntegers& operator ++() { lit = -lit; return *this; }
    LiteralsAsIntegers operator ++(int) {
      LiteralsAsIntegers x(*this); lit = -lit; return x;
    }

    LiteralsAsIntegers& add(Val v) {
      if (v == Val(Val::one))
	lit = -lit;
      return *this;
    }
    LiteralsAsIntegers operator +(Val v) const {
      LiteralsAsIntegers y(*this); y.add(v); return y;
    }

    bool operator < (LiteralsAsIntegers x) const {
      return lit < x.lit;
    }

    bool operator == (LiteralsAsIntegers x) const {
      return lit == x.lit;
    }
    bool operator != (LiteralsAsIntegers x) const {
      return not (*this == x);
    }

    typename InfoPolicyLit<Index>::InfoReferenceType operator *() const {
      return get_info(lit);
    }
    
  private :
    Index lit;
  };

  template <class Lit>
  typename Lit::Var Var(Lit x) {
    return x.var();
  }
  template <class Lit>
  typename Lit::Val Val(Lit x) {
    return x.val();
  }

  typedef LiteralsAsIntegers<int, Variables::InfoPolicyIndexVectorOccurrences, InfoPolicyVectorOccurrences, std::string> LitIntOccString;

}




#endif

