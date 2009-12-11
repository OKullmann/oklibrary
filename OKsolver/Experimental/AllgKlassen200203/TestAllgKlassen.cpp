// Oliver Kullmann, 21.2.2003 (Swansea)
/* Copyright 2003 - 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/OKsolver/Experimental/AllgKlassen200203/TestAllgKlassen.cpp
  \brief Test old experimental classes
  \deprecated Old code
*/

#include <vector>
#include <iostream>
#include <cassert>
#include <limits>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <functional>
#include <iterator>

#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

#include <OKlib/General/StringHandling.hpp>
#include <OKlib/General/StreamHandling.hpp>
#include <OKlib/General/Matrices.hpp>
#include <OKlib/General/ErrorHandling.hpp>
#include <OKlib/General/FunctionHandling.hpp>
#include <OKlib/General/Algorithms.hpp>

#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Variables.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Literals.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/LitSets.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Clauses.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/ClauseSets.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/PartialAssignments.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/OutputCls.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/InputCls.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/TransformationsBiclique.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/ConceptDefinitions.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Domains.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/AlgebraicStructures.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/SetsOfVariables.hpp>

#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Tests_General.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Tests_Variables.hpp>

using namespace std;

// ------------------------------------------------------------------
// DEPRECATED :
// ------------------------------------------------------------------

template <class C, class T>
class Test {
public :
  typedef C Class;
  typedef T Tag;
  Test() {}
  void test() {}
};
template <class C>
void RunTest() {
  Test<C, typename C::Concept>().test();
}

template <class C>
class Test<C, ConceptDefinitions::Root_tag> {
public :
  typedef C Class;
  typedef ConceptDefinitions::Root_tag Tag;
  Test() {
    cout << "\nTest\nClass " << typeid(C).name() << "\n";
  }
};

// ------------------------------------------------------------------
// Tests for variables
// ------------------------------------------------------------------

template <class Var>
class Test<Var, ConceptDefinitions::Variable_tag> : Test<Var, ConceptDefinitions::Root_tag> {
  // It is assumed that the name type of Var is compatible with std::string
  // (the underlying assumption is, that the name type is not critical).
  // Furthermore it is assume, that at least number_new many new variables
  // can be created.
public :
  typedef Var Class;
  typedef ConceptDefinitions::Variable_tag Tag;
  typedef vector<Var> var_vector;
  static const int number_new_max = 100;
  virtual ~Test() {}
  Test() : number_new_variables(0) {
    cout << "Tag " << typeid(Tag).name() << "\n";
    boost::function_requires<ConceptDefinitions::Variable_concept<Var> >();

    test_null();
    test_creation();

    Var::clear();
    number_new_variables = 0;
  }
  void test() {}

protected :

  var_vector vv;
  // clear after usage
  int number_new_variables;
  ostringstream out;
  // clear after usage

private :
  void test_emptyness() {
    test_emptyness_info();
  }
  virtual void test_emptyness_info() {}
  void test_null() {
    fill_n(back_inserter(vv), 10, Var());
    assert(adjacent_find(vv.begin(), vv.end(), not_equal_to<Var>()) == vv.end());
    assert(adjacent_find(vv.begin(), vv.end(), less<Var>()) == vv.end());
    assert(adjacent_find(vv.begin(), vv.end(), greater<Var>()) == vv.end());
    for (typename var_vector::iterator i = vv.begin(); i != vv.end(); ++i) {
      assert(i -> null());
      assert(i -> name().empty());
    }
    {
      Var v(vv[0]);
      assert(v == vv[0]);
      assert(v.null());
      assert(v.name().empty());
      Var w;
      w = v;
      assert(w == vv[0]);
      assert(w.null());
      assert(w.name().empty());
      const Var v1 = v;
      out << v1;
      assert(out.str() == Auxiliary::null_variable_tag);
      out.str("");
    }
    vv.clear();
  }
  void test_creation() {
    vv.push_back(Var());
    for (int i = 1; i <= number_new_max; ++i)
      vv.push_back(Var("v" + StringHandling::toString(i)));
    number_new_variables += number_new_max;
    assert(adjacent_find(vv.begin(), vv.end(), equal_to<Var>()) == vv.end());
    for (typename var_vector::size_type i = 1; i != vv.size(); ++i) {
      assert(not vv[i].null());
      const string name = "v" + StringHandling::toString(i);
      assert(vv[i].name() == name);
      out << vv[i];
      assert(out.str() == name);
      out.str("");
    }
    var_vector vvc = vv;
    assert(vvc == vv);
    for (typename var_vector::size_type i = 1; i != vvc.size(); ++i) {
      assert(not vvc[i].null());
      const string name = "v" + StringHandling::toString(i);
      assert(vvc[i].name() == name);
      out << vvc[i];
      assert(out.str() == name);
      out.str("");
    }
    out << vvc[0];
    assert(out.str() == Auxiliary::null_variable_tag);
    out.str("");

    vv.clear();
    number_new_variables = 0;
  }
};

template <class Var>
class Test<Var, ConceptDefinitions::VariableWithHistory_tag> :  public virtual Test<Var, ConceptDefinitions::Variable_tag> {
public :
  typedef Var Class;
  typedef ConceptDefinitions::VariableWithCounting_tag Tag;
  Test() {
    test_order();
  }
private :
  void test_order() {
    Test<Var, ConceptDefinitions::Variable_tag>::vv.push_back(Var());
    for (int i = 1; i <= Test<Var, ConceptDefinitions::Variable_tag>::number_new_max; ++i)
      Test<Var, ConceptDefinitions::Variable_tag>::vv.push_back(Var(StringHandling::toString(i) + "V"));
    assert((adjacent_find(Test<Var, ConceptDefinitions::Variable_tag>::vv.begin(), Test<Var, ConceptDefinitions::Variable_tag>::vv.end(), greater_equal<Var>()) == Test<Var, ConceptDefinitions::Variable_tag>::vv.end()));
    Test<Var, ConceptDefinitions::Variable_tag>::vv.clear();
    Var::clear();
  }
};

template <class Var>
class Test<Var, ConceptDefinitions::VariableWithCounting_tag> :  public virtual Test<Var, ConceptDefinitions::VariableWithHistory_tag> {
public :
  typedef Var Class;
  typedef ConceptDefinitions::VariableWithCounting_tag Tag;
  typedef typename Var::size_type size_type;
  Test() {
    cout << "Tag " << typeid(Tag).name() << "\n";
    boost::function_requires<ConceptDefinitions::VariableWithCounting_concept<Var> >();

    test_emptyness_start();
    test_creation_null();
    test_creation();
    test_emptyness_end();
  }
private :
  void test_emptyness_start() {
    assert(Var::size() == 1 and Var::n() == 0);
  }
  void test_creation_null() {
    Test<Var, ConceptDefinitions::VariableWithHistory_tag>::vv.assign(20, Var());
    assert(Var::size() == 1 and Var::n() == 0);
    Test<Var, ConceptDefinitions::VariableWithHistory_tag>::vv.clear();
  }
  void test_creation() {
    Var::reserve(Test<Var, ConceptDefinitions::VariableWithHistory_tag>::number_new_max / 3);
    assert((Var::capacity() >= Test<Var, ConceptDefinitions::VariableWithHistory_tag>::number_new_max / 3));
    Var::reserve(Test<Var, ConceptDefinitions::VariableWithHistory_tag>::number_new_max);
    const size_type cap = Var::capacity();
    assert((cap >= Test<Var, ConceptDefinitions::VariableWithHistory_tag>::number_new_max));
    for (int i = 0; i < Test<Var, ConceptDefinitions::VariableWithHistory_tag>::number_new_max; ++i)
      Test<Var, ConceptDefinitions::VariableWithHistory_tag>::vv.push_back(Var("Var" + StringHandling::toString(i)));
    Test<Var, ConceptDefinitions::VariableWithHistory_tag>::number_new_variables += Test<Var, ConceptDefinitions::VariableWithHistory_tag>::number_new_max;
    assert(cap == Var::capacity());
    assert((Var::size() == Test<Var, ConceptDefinitions::VariableWithHistory_tag>::number_new_variables + 1 and Var::n() == Test<Var, ConceptDefinitions::VariableWithHistory_tag>::number_new_variables));
    Test<Var, ConceptDefinitions::VariableWithHistory_tag>::vv.clear();
    Var::clear();
    Test<Var, ConceptDefinitions::VariableWithHistory_tag>::number_new_variables += 0;
    assert(cap == Var::capacity());
  }
  void test_emptyness_end() {
    assert(Var::size() == 1 and Var::n() == 0);
  }
};

template <class Var>
class Test<Var, ConceptDefinitions::VariableWithInfo_tag> : public virtual Test<Var, ConceptDefinitions::VariableWithCounting_tag> {
public :
  // It is assumed that for a v of type V the expression v -> occ is defined
  // and value_type(a) will set occ
  // (the underlying assumption is, that the info type is not critical).
  typedef Var Class;
  typedef ConceptDefinitions::VariableWithInfo_tag Tag;
  typedef typename Var::value_type value_type;
  Test() {
    cout << "Tag " << typeid(Tag).name() << "\n";
    boost::function_requires<ConceptDefinitions::VariableWithInfo_concept<Var> >();

    test_emptyness_start();
    test_creation_null();
    test_creation_2();
    test_creation();
    Var::clear();
    Test<Var, ConceptDefinitions::VariableWithCounting_tag>::number_new_variables = 0;
    test_emptyness_end();
  }
private :
  int current_occ;
  void test_emptyness_start() {
    for (Var v = Var::begin(); v != Var::end(); ++v)
      count();
    assert(count.c == 1);
    count.reset();
    assert(Var::find("").null());
    assert(Var::find("v") == Var::end());
    assert(Var::begin().null());
    assert((--Var::end()).null());
    *Var() = value_type(77);
    assert(Var() -> occ == 77);
    Var::begin() -> occ = 88;
    assert((--Var::end()) -> occ == 88);
    assert((*Var()).occ == 88);
    current_occ = 88;
    // QUESTION: What is more efficient: (*Var()).occ or Var() -> occ ?!
  }
  void test_creation_null() {
    Test<Var, ConceptDefinitions::VariableWithCounting_tag>::vv.assign(20, Var());
    for (Var v = Var::begin(); v != Var::end(); ++v)
      count();
    assert(count.c == 1);
    count.reset();
    Test<Var, ConceptDefinitions::VariableWithCounting_tag>::vv.clear();
    const Var v = Var::find("");
    assert(v.null());
    assert(v -> occ == current_occ);
  }
  void test_creation_2() {
    {
      Var v1("a"), v2("b");
      assert(Var::size() == 3 and Var::n() == 2);
      assert(Var::begin() == Var() and ++Var::begin() == v1 and ++++Var::begin() == v2);
      assert(--Var::end() == v2);
    }
    Var::find("a") -> occ = 111;
    assert(Var::find("a") -> occ == 111);
    Var::find("b") -> occ = 222;
    assert(Var::find("b") -> occ == 222);
    assert(Var::find("a") -> occ == 111);
    Var::clear();
    Test<Var, ConceptDefinitions::VariableWithCounting_tag>::number_new_variables = 0;
    current_occ = 0;
    assert(Var::find("a") == Var::end() and Var::find("b") == Var::end());
    assert(Var() -> occ == 0);
  }
  void test_creation() {
    Var() -> occ = -1;
    current_occ = -1;
    for (int i = 0; i < Test<Var, ConceptDefinitions::VariableWithCounting_tag>::number_new_max; ++i)
      Var("v" + StringHandling::toString(i));
    Test<Var, ConceptDefinitions::VariableWithCounting_tag>::number_new_variables += Test<Var, ConceptDefinitions::VariableWithCounting_tag>::number_new_max;
    for (Var v = Var::begin(); v != Var::end(); ++v)
      count();
    assert(count.c == Var::size());
    count.reset();
    const Var end = Var::end();
    for (Var v = ++Var::begin(); v != end; ++v)
      v -> occ = count();
    count.reset();
    for (Var v = ++Var::begin(); v != end; ++v)
      assert(v -> occ == distance(Var::begin(), v) - 1);
    assert(Var::begin() -> occ == current_occ);
    for (int i = 0; i < Test<Var, ConceptDefinitions::VariableWithCounting_tag>::number_new_max; ++i)
      assert(Var::find("v" + StringHandling::toString(i)) -> occ == i);
    {
      typename Test<Var, ConceptDefinitions::VariableWithCounting_tag>::var_vector vvc;
      for (Var v = ++Var::begin(); v != Var::end(); ++v)
	vvc.push_back(v);
      for (typename Test<Var, ConceptDefinitions::VariableWithCounting_tag>::var_vector::size_type i = 0; i != vvc.size(); ++i)
	assert(vvc[i] -> occ == i);
    }
    assert((Var::n() == Test<Var, ConceptDefinitions::VariableWithCounting_tag>::number_new_variables));
    {
      typedef vector<typename Var::value_type> VV;
      const int m = 4;
      BOOST_STATIC_ASSERT((m <=  Test<Var, ConceptDefinitions::VariableWithCounting_tag>::number_new_max));
      VV V(m);
      Algorithms::copy_n(Var::begin(), m, V.begin());
      count.reset();
      for (typename VV::const_iterator i = ++V.begin(); i != V.end(); ++i)
	assert(i -> occ == count());
      Algorithms::copy_backward_n(Var::end(), m, V.end());
      count.reset(Var::n() - m);
      for (typename VV::const_iterator i = V.begin(); i != V.end(); ++i)
	assert(i -> occ == count());
      count.reset();
    }
  }
  void test_emptyness_end() {
    assert(++Var::begin() == Var::end());
  }

protected :
  FunctionHandling::Counter<> count;
  // reset after use
};


// A MORE GENERAL STRUCTUR IS NEEDED HERE:
template<>
class Test<Variables::VarCharOccInt, Variables::VarCharOccInt::Concept> : Test<Variables::VarCharOccInt, ConceptDefinitions::Root_tag> {
public :
  typedef Variables::VarCharOccInt Var;
  typedef Var Class;
  typedef ConceptDefinitions::VariableWithInfo_tag Tag;
  Test() {
    cout << "Tag " << typeid(Tag).name() << "\n";
    cout << "Testing for Overflow\n";
    boost::function_requires<ConceptDefinitions::VariableWithInfo_concept<Var> >();

    const int start = 5000;
    const std::size_t size_char = std::numeric_limits<signed char>::max();
    for (int i = start; i < start + size_char - 1; ++i) {
       Var v(i);
       assert(v.name() == i);
    }
    assert(Var::size() == size_char);
    {
      bool thrown = false;
      try {
	Var v(1);
      }
      catch(const Variables::Overflow_Variables& e) {
	thrown = true;
      }
      assert(thrown);
    }
  }
  void test() {}
};
template<>
class Test<Variables::VarRefCharInt, Variables::VarRefCharInt::Concept> : Test<Variables::VarRefCharInt, ConceptDefinitions::Root_tag> {
public :
  typedef Variables::VarRefCharInt Var;
  typedef Var Class;
  typedef Var::Concept Tag;
  Test() {
    cout << "Tag " << typeid(Tag).name() << "\n";
    cout << "Testing for Overflow\n";
    boost::function_requires<ConceptDefinitions::VariableWithHistory_concept<Var> >();

    const int start = 5000;
    const std::size_t size_char = std::numeric_limits<signed char>::max();
    for (int i = start; i < start + size_char; ++i) {
       Var v(i);
       assert(v.name() == i);
    }
    {
      bool thrown = false;
      try {
	Var v(1);
      }
      catch(const Variables::Overflow_Variables& e) {
	thrown = true;
      }
      assert(thrown);
    }
  }
  void test() {}
};


// ------------------------------------------------------------------
// Tests for partial assignments
// ------------------------------------------------------------------

template <class Pass>
class Test<Pass, ConceptDefinitions::BasicPartialAssignment_tag> : Test<Pass, ConceptDefinitions::Root_tag> {
  // Assumes Pass::Var::NameType is std::string.
public :
  typedef Pass Class;
  typedef ConceptDefinitions::BasicPartialAssignment_tag Tag;
  typedef typename Pass::Var Var;
  typedef typename Pass::Dom Dom;
  virtual ~Test() {}
  Test() {
    cout << "Tag " << typeid(Tag).name() << "\n";
    boost::function_requires<ConceptDefinitions::BasicPartialAssignment_concept<Pass> >();

    create_variables();
    test_empty_assignment();
    test_assignments();

    Var::clear();
  }
  void test() {}
protected :
  static const int number_variables = 100;
  BOOST_STATIC_ASSERT(number_variables >= 4);
  static const int left = 10;
  static const int right = 20;
  BOOST_STATIC_ASSERT(0 <= left and left <= right and right <= number_variables);
  typedef vector<Var> Var_vec;
  Var_vec vv;
private :
  void create_variables() {
    for (int i = 0; i < number_variables; ++i)
      Var v(StringHandling::toString(i));
    // without "v" we get a SEGMENTATION FAULT with g++ 3.2.1
    // (with 3.3 also without "v" (!))
  }
  void test_empty_assignment() {
    Pass phi;
    for (Var v = ++Var::begin(); v != Var::end(); ++v)
      assert(phi[v] == Dom::end());
    Pass psi(phi);
    for (Var v = ++Var::begin(); v != Var::end(); ++v)
      assert(psi[v] == Dom::end());
    Pass tau;
    tau = psi;
    for (Var v = ++Var::begin(); v != Var::end(); ++v)
      assert(tau[v] == Dom::end());
  }
  void test_assignments() {
    Pass phi;
    for (Var v = ++++Var::begin(); v != Var::end(); ++v)
      phi[v] = Dom::begin();
    assert(phi[++Var::begin()] == Dom::end());
    for (Var v = ++++Var::begin(); v != Var::end(); ++v)
      assert(phi[v] == Dom::begin());
    for (Var v = ----Var::end(); v != Var::end(); ++v)
      phi[v] = Dom::end();
    assert(phi[----Var::end()] == Dom::end() and phi[--Var::end()] == Dom::end());
    {
      Pass psi(phi); 
      for (Var v = ++Var::begin(); v != Var::end(); ++v)
	assert(psi[v] == phi[v]);
      psi.clear();
      for (Var v = ++Var::begin(); v != Var::end(); ++v)
	assert(psi[v] == Dom::end());
    }
    for (Var v = ++++Var::begin(); v != ----Var::end(); ++v)
      assert(phi[v] == Dom::begin());
    for (int i = left; i < right; ++i)
      phi[Var::find(StringHandling::toString(i))] = Dom::max();
    for (int i = left; i < right; ++i)
      assert(phi[Var::find(StringHandling::toString(i))] == Dom::max());
    assert(phi[++Var::begin()] == Dom::end());
    assert(phi[----Var::end()] == Dom::end() and phi[--Var::end()] == Dom::end());
    assert(phi[++++Var::begin()] == Dom::begin());
  }
};

// ------------------------------------------------------------------
// Tests for variable sets
// ------------------------------------------------------------------

template <class VarSet>
class Test<VarSet, ConceptDefinitions::SetOfVariables_tag> : Test<VarSet, ConceptDefinitions::Root_tag> {
  // Assumes VarSet::Var::NameType is std::string.
public :
  typedef VarSet Class;
  typedef ConceptDefinitions::SetOfVariables_tag Tag;
  typedef typename VarSet::Var Var;
  virtual ~Test() {}
  Test() {
    cout << "Tag " << typeid(Tag).name() << "\n";
    boost::function_requires<ConceptDefinitions::SetOfVariables_concept<VarSet> >();

    test_empty_set();
    test_unit_sets();
    Var::clear();
  }
  void test() {}
private :
  void test_empty_set() {
    VarSet vs;
    assert(vs.empty());
    assert(vs.size() == 0);
    assert(vs.begin() == vs.end());
    VarSet vs2(vs);
    assert(vs2.empty());
    assert(vs2.size() == 0);
    assert(vs2.begin() == vs2.end());
  }
  void test_unit_sets() {
    const Var v("abc");
    VarSet vs(v);
    assert(not vs.empty());
    assert(vs.size() == 1);
    assert(vs.begin() != vs.end());
    assert(++vs.begin() == vs.end());
    assert(*vs.begin() == v);
    VarSet vs2(vs);
    assert(not vs2.empty());
    assert(vs2.size() == 1);
    assert(vs2.begin() != vs2.end());
    assert(++vs2.begin() == vs2.end());
    assert(*vs2.begin() == v);
    vs2.clear();
    assert(vs2.empty());
    assert(vs2.size() == 0);
    assert(vs2.begin() == vs2.end());
    assert(not vs.empty());
    assert(vs.size() == 1);
    assert(vs.begin() != vs.end());
    assert(++vs.begin() == vs.end());
    assert(*vs.begin() == v);
    vs2 = vs;
    assert(*vs2.begin() == v);
  }
};

template <class VarSet>
class Test<VarSet, ConceptDefinitions::SingletonSetOfVariables_tag> : Test<VarSet, ConceptDefinitions::SetOfVariables_tag> {
  // Assumes VarSet::Var::NameType is std::string.
public :
  typedef VarSet Class;
  typedef ConceptDefinitions::SingletonSetOfVariables_tag Tag;
  typedef typename VarSet::Var Var;
  virtual ~Test() {}
  Test() {
    cout << "Tag " << typeid(Tag).name() << "\n";
    boost::function_requires<ConceptDefinitions::SingletonSetOfVariables_concept<VarSet> >();

    test_conversion();
    Var::clear();
  }
  void test() {}
private :
  void test_conversion() {
    const Var v("100");
    VarSet vs(v);
    assert(vs == v);
    vs = VarSet();
    assert(vs == Var());
    const Var w("xyz");
    const VarSet vs2(w);
    assert(vs == Var());
    assert(vs2 == w);
  }
};

// ------------------------------------------------------------------------
// DEPRICATED FROM HERE

template <class FinDom>
void test_finite_domains() {
  
  cout << "\ntest_finite_domains(): " << typeid(FinDom).name() << "\n";

  boost::function_requires<ConceptDefinitions::FiniteDomain_concept<FinDom> >();

  assert(FinDom() == FinDom::min());
  assert(FinDom::size() == distance(FinDom::begin(), FinDom::end()));

  typedef typename FinDom::iterator iterator;

  {
    assert(iterator() == FinDom::end());

    FinDom x;

    assert(x == FinDom::min());
    if (FinDom::size() > 1) {
      assert(x < *++iterator(x));
      assert(x == FinDom::min());
    }
    assert(x == *FinDom::begin());

    x = FinDom::max();
    assert(x == FinDom::max());
    if (FinDom::size() > 1) {
      assert(x > *--iterator(x));
      assert(x == FinDom::max());
    }
    assert(x == *--FinDom::end());
  }

  // running through all elements of the linear order:
  FunctionHandling::Counter<> count;
  for (iterator v = FinDom::begin();;) {
    count();
    const FinDom w = *v;
    if (++v != FinDom::end())
      assert(w < *v);
    else
      break;
  }
  assert(count.c == FinDom::size());
  count.reset();
  for (iterator v = FinDom::end(); (v != FinDom::begin()) and (--v, true);) {
    count();
    assert(*v != FinDom::max() or v == FinDom::max());
  }
  assert(count.c == FinDom::size());
  count.reset();
  for (iterator v = FinDom::begin(); v != FinDom::end(); ++v) {
    count();
    // running through all other elements:
    FunctionHandling::Counter<> count2;
    for (iterator w = FinDom::begin(); w != FinDom::end(); ++w)
      if (w == v)
	continue;
      else
	count2();
    assert(count2.c == FinDom::size() - 1);
  }
  assert(count.c == FinDom::size());

  if (FinDom::size() >= 2) {
    typedef typename FinDom::set f_set;
    typedef vector<f_set> vector_t;
    const int N = 10;
    vector_t V(N);
    count.reset();
    for (iterator v = FinDom::begin(); v != ----FinDom::end(); ++v)
      for (typename vector_t::iterator i = V.begin(); i != V.end(); ++i) {
	assert(i -> insert(*v) == FinDom::no_critical_change);
	count();
      }
    assert(count.c == N * (FinDom::size() - 2));
    count.reset();
    for (typename vector_t::iterator i = V.begin(); i != V.end(); ++i) {
      assert(i -> insert(*(----FinDom::end())) == FinDom::forced_assignment);
      assert(i -> unit() == *(--FinDom::end()));
      count();
    }
    assert(count.c == N);
    for (typename vector_t::iterator i = V.begin(); i != V.end(); ++i)
      assert(i -> insert(*(--FinDom::end())) == FinDom::contradiction_created);

    fill(V.begin(), V.end(), f_set());
    count.reset();
    for (iterator v = FinDom::begin(); v != --iterator(FinDom::max()); ++v)
      for (typename vector_t::iterator i = V.begin(); i != V.end(); ++i) {
	assert(i -> insert(*v) == FinDom::no_critical_change);
	count();
      }
    assert(count.c == N * (FinDom::size() - 2));
    for (typename vector_t::iterator i = V.begin(); i != V.end(); ++i) {
      assert(i -> insert(*(--iterator(FinDom::max()))) == FinDom::forced_assignment);
      assert(i -> unit() == FinDom::max());
    }
  }

  // output
  {
    stringstream s1;
    stringstream s2;
    for (iterator i = FinDom::begin(); i != FinDom::end(); ++i) {
      s1 << *i;
      s2 << distance(FinDom::begin(), i);
      assert(s1.str() == s2.str());
      s1.str("");
      s2.str("");
    }
  }
}

template <class SignRing>
void test_signring() {

  cout << "\ntest_signring(): " << typeid(SignRing).name() << "\n";

  boost::function_requires<ConceptDefinitions::SignRing_concept<SignRing> >();

  const SignRing Positive = SignRing::Positive();
  const SignRing Negative = SignRing::Negative();
  const SignRing Null = SignRing::Null();

  assert(Negative < Null and Null < Positive);
  assert(Negative != Null and Negative != Positive and Null != Positive);
  assert(Null == SignRing());
  assert(Negative == SignRing::identity_plus());
  assert(Positive == SignRing::identity_times());
  typedef vector<SignRing> VecSigns;
  const int N = 10;
  VecSigns V(N);
  assert(*min_element(V.begin(), V.end()) == Null);
  assert(*max_element(V.begin(), V.end()) == Null);
  assert(accumulate(V.begin(), V.end(), Positive) == Positive);
  assert(accumulate(V.begin(), V.end(), Null) == Null);
  assert(accumulate(V.begin(), V.end(), Negative) == Null);
  assert(accumulate(V.begin(), V.end(), Negative, multiplies<SignRing>()) == Negative);
  assert(accumulate(V.begin(), V.end(), Null, multiplies<SignRing>()) == Null);
  assert(accumulate(V.begin(), V.end(), Positive, multiplies<SignRing>()) == Null);

  typedef typename SignRing::iterator iterator;
  FunctionHandling::Counter<> count;
  for (iterator i = SignRing::begin(); i != SignRing::end(); ++i)
    count();
  assert(count.c == SignRing::size());

  // output
  {
    stringstream s1;
    stringstream s2;
    for (iterator i = SignRing::begin(); i != SignRing::end(); ++i) {
      s1 << *i;
      s2 << static_cast<int>(distance(SignRing::begin(), i)) - 1;
      assert(s1.str() == s2.str());
      s1.str("");
      s2.str("");
    }
  }
}

template <class Bool>
void test_boolean() {

  cout << "\ntest_boolean(): " << typeid(Bool).name() << "\n";

  boost::function_requires<ConceptDefinitions::BooleanDomain_concept<Bool> >();

  const Bool False(Bool::False());
  const Bool True(Bool::True());

  assert(False == Bool(false) and True == Bool(true));

  assert(Bool::min() == False);
  assert(Bool::max() == True);

  assert(True); assert(not False);

  assert(Bool(False and True) == False);
  assert(Bool(True and True) == True);
  assert(Bool(False or True) == True);
  assert(Bool(False or False) == False);

  // output
  {
    stringstream s1;
    stringstream s2;
    s1 << boolalpha; s2 << boolalpha;
    s1 << Bool(false);
    s2 << false;
    assert(s1.str() == s2.str());
    s1 << Bool(true);
    s2 << true;
    assert(s1.str() == s2.str());
  }
}

template <class Lit>
void test_literals_01() {

  // boost::function_requires<ConceptDefinitions::Literal_concept<Lit> >();
  
  cout << "\ntest_literals_01(): " << typeid(Lit).name() << "\n";
  Lit x, x1;
  assert(x == x1);
  assert(not x and not x1 and x.var() == Var(x) and Var(x) == x1.var() and x1.var() == Var(x1));
  typename Lit::Var v("abcd");
  ostringstream out;
  out << v;
  assert(out.str() == "abcd");
  out.str("");
  Lit y(v, Values::pos);
  out << y.var() << " " << Var(y);
  assert(out.str() == "abcd abcd");
  out.str("");
  typename Lit::Var w("hijk");
  Lit z(w, Values::neg);
  out << Literals::Var(z) << " " << Literals::Val(z);
  assert(out.str() == "hijk 1");
  (*y).pos = 7; (*y).neg = 999;
  (*z).pos = 8; (*z).neg = 9999;
  assert((*y).pos == 7 and (*y).neg == 999 and (*z).pos == 8 and (*z).neg == 9999);;
  Lit a = x;
  (*a).pos = 777;
  assert((*a).pos == 777 and (*x).pos == 777);
  ++y;
  assert(Var(y) == v); assert(Val(y) == Values::neg);
  assert(a == a++); assert(a == x);
  a = z;
  assert(a == z and a == a);
  assert(z == a++); // Attention: a == a++ is undefined!
  assert(a != z);
  assert(z + Values::neg == a and z + Values::pos == z);
  assert(a != z);
  assert(a + Val(a) + Values::neg < a + Val(a));
  assert(Lit(Var(a), Val(a)) == a + Val(a));
  assert(Lit(Var(a), ++Val(a)) == a + Val(a) + Values::neg);
}

template <class Value>
void test_BooleanValues() {
  cout << "\ntest_BooleanValues(): " << typeid(Value).name() << "\n";
  Value v(Value::zero);
  ostringstream out;
  out << v; out << (++v); out << v; out << v++; out << v;
  assert(out.str() == "01110");
  v.add(Values::neg); assert(v == Values::neg);
  v.add(Values::neg).add(Values::neg); assert(v == Values::neg);
  assert(Values::neg + Values::neg == Values::pos);
  assert(v + v == Values::pos); assert(v == Values::neg);
  assert(Values::pos + Values::neg == Values::neg + Values::pos);
}

template <class LitSet>
void test_Literal_set() {
  cout << "\ntest_Literal_set(): " << typeid(LitSet).name() << "\n";
  typedef typename LitSet::Lit Lit;
  LitSet ls, ls1;
  assert(ls.size() == 0);
  assert(ls.empty());
  typename Lit::Var v1("abcd");
  typename Lit::Var v2("1");
  typename Lit::Var v3("zhi");
  Lit r1(v1, Values::neg);
  Lit r2(v2, Values::neg);
  Lit r3(v3, Values::neg);
  ls.add(r1);
  ls.add(r2);
  ls.add(r3);
  assert(ls.size() == 3);
  assert(not ls.empty());
  ls.erase(r1);
  assert(ls.size() == 2);
  assert(not ls.empty());
  Lit r4(v1, Values::pos);
  Lit r5(v2, Values::pos);
  Lit r6(v3, Values::pos);
  ls1.add(r4);
  ls1.add(r5);
  ls1.add(r6);
  ls.add(ls1);
  assert(ls1.size() == 3);
  assert(ls.size() == 5);
  ls1=ls;
  assert(ls1 == ls);
  assert(ls1.size() == 5);
  assert(ls.size() == 5);
  ls.erase(r2);
  ls.erase(r5);
  ls.erase(r3);
  ls.erase(r2);
  assert(ls.size() == 2);
  ls1.erase(ls);
  assert(ls1.size() == 3);
  assert(not ls.contains(r3) and ls1.contains(r3));
  assert(ls.contains(r4) and ls1.contains(r2));
  assert( not (ls < ls1));
  assert(ls1 < ls);
  assert(*ls.begin() == r4 and *(++ls.begin()) == r6);
}


void test_OutputCls() {
  cout << "\ntest_OutputCls()" << "\n";
  {
    cout << "Test empty output" << endl;
    ostringstream sout;
    {
      OutputCls::Dimacs_output_literal_stream_strings out(sout, 0, 0);
      assert(out.clause_number() == 0);
      assert(out.literal_occurrences() == 0);
      assert(out.min_clause_length() == out.max_size);
      assert(out.max_clause_length() == 0);
      assert(out.max_size == std::numeric_limits<unsigned long int>::max());
      assert(out.parameter_n() == 0);
      assert(out.parameter_c() == 0);
    }
    assert(sout.str() == "p cnf 0 0\n");
  }
  {
    cout << "Test two empty clauses" << endl;
    ostringstream sout;
    {
      OutputCls::Dimacs_output_literal_stream_strings out(sout, 1, 1);
      out.new_clause();
      out.new_clause();
      assert(out.clause_number() == 2);
      assert(out.literal_occurrences() == 0);
      assert(out.min_clause_length() == 0);
      assert(out.max_clause_length() == 0);
      assert(out.max_size == std::numeric_limits<unsigned long int>::max());
      assert(out.parameter_n() == 1);
      assert(out.parameter_c() == 1);
    }
    assert(sout.str() == "p cnf 1 1\n 0\n 0\n");
  }
  {
    cout << "Test three clauses" << endl;
    ostringstream sout;
    {
      OutputCls::Dimacs_output_literal_stream_strings out(sout,5,3);
      out.comment_line("Test1");
      out.comment_line("Test2");
      out.new_clause();
      out.add("-1").add("-5");
      out.new_clause();
      out.add("gh").add("- jk").add("- 9");
      out.new_clause();
      out.add("1").add("jk");
      assert(out.clause_number() == 3);
      assert(out.literal_occurrences() == 7);
      assert(out.min_clause_length() == 2);
      assert(out.max_clause_length() == 3);
      assert(out.max_size == std::numeric_limits<unsigned long int>::max());
      assert(out.parameter_n() == 5);
      assert(out.parameter_c() == 3);
    }
    assert(sout.str() == "c Test1\nc Test2\np cnf 5 3\n-1 -5 0\ngh - jk - 9 0\n1 jk 0\n");
  }
  
}

void test_TransformationBiclique() {
  cout << "\ntest_TransformationBiclique()" << "\n";

  stringstream cnf;
  class Skip_lines {
    stringstream& ss;
  public :
    Skip_lines(stringstream& ss) : ss(ss) {}
    string operator() (unsigned int s) {
      ss.seekp(0);
      StreamHandling::line_advance(ss, s);
      ostringstream result;
      result << ss.rdbuf();
      return result.str();
    }
  };
  Skip_lines skip(cnf);

  cout << "Test of function symmetric_conflict_number_to_SAT\n";

  cout << "Test matrix of dimension 1" << endl;;
  {
    OutputCls::Adapter_Dimacs_Output out(cnf);
    const unsigned int dim = 1;
    typedef Matrices::QuadMatrix<unsigned int, unsigned int, dim> Arr;
    const Arr::array_type a = {
      0
    };
    Arr A(a);
    const unsigned int max_bc = 1;
    TransformationsBiclique::symmetric_conflict_number_to_SAT(A, dim, max_bc, out);
    assert(out.clause_number() == 0);
    assert(out.literal_occurrences() == 0);
    assert(out.min_clause_length() == numeric_limits<OutputCls::Adapter_Dimacs_Output::size_type>::max());
    assert(out.max_clause_length() == 0);
    assert(out.get_number_variables() == 0);
    assert(out.get_number_clauses() == out.clause_number());
    assert(skip(5) == "p cnf 0 0\n");
    cnf.str("");
  }

  cout << "Test matrix of dimension 2 with 0 bicliques" << endl;;
  {
    OutputCls::Adapter_Dimacs_Output out(cnf);
    const unsigned int dim = 2;
    typedef Matrices::QuadMatrix<unsigned int, unsigned int, dim> Arr;
    const Arr::array_type a = {
      0, 1,
      1, 0
    };
    Arr A(a);
    const unsigned int max_bc = 0;
    TransformationsBiclique::symmetric_conflict_number_to_SAT(A, dim, max_bc, out);
    assert(out.clause_number() == 1);
    assert(out.literal_occurrences() == 0);
    assert(out.min_clause_length() == 0);
    assert(out.max_clause_length() == 0);
    assert(out.get_number_variables() == 0);
    assert(out.get_number_clauses() == out.clause_number());
    assert(skip(6) == "p cnf 0 1\n 0\n");
    cnf.str("");
 }
 
  cout << "Test matrix of dimension 2 with 1 biclique" << endl;;
  {
    OutputCls::Adapter_Dimacs_Output out(cnf);
    const unsigned int dim = 2;
    typedef Matrices::QuadMatrix<unsigned int, unsigned int, dim> Arr;
    const Arr::array_type a = {
      0, 1,
      1, 0
    };
    Arr A(a);
    const unsigned int max_bc = 1;
    TransformationsBiclique::symmetric_conflict_number_to_SAT(A, dim, max_bc, out);
    assert(out.clause_number() == 2);
    assert(out.literal_occurrences() == 4);
    assert(out.min_clause_length() == 2);
    assert(out.max_clause_length() == 2);
    assert(out.get_number_variables() == 2);
    assert(out.get_number_clauses() == out.clause_number());
    assert(skip(6) == "p cnf 2 2\nR0C1N0B0 R0C1N0B1 0\n-R0C1N0B0 -R0C1N0B1 0\n");
    cnf.str("");
  }

  cout << "Test matrix of dimension 5" << endl;
  {
    OutputCls::Adapter_Dimacs_Output out(cnf);
    const unsigned int dim = 5;
    typedef Matrices::QuadMatrix<unsigned int, unsigned int, dim> Arr;
    const Arr::array_type a = {
      0,2,1,1,1,
      2,0,1,1,1,
      1,1,0,2,2,
      1,1,2,0,2,
      1,1,2,2,0
    };
    Arr A(a);
    const unsigned int max_bc = 4;
    TransformationsBiclique::symmetric_conflict_number_to_SAT(A, dim, max_bc, out);
    assert(out.clause_number() == 1886);
    assert(out.literal_occurrences() == 5104);
    assert(out.min_clause_length() == 2);
    assert(out.max_clause_length() == 8);
    assert(out.get_number_variables() == 112);
    assert(out.get_number_clauses() == out.clause_number());
    assert(StringHandling::deleteCharacter(skip(9), '-').size() == string("p cnf 112 1886\n").size() + out.literal_occurrences() * string("R0C0N0B0 ").size() + out.clause_number() * string("0\n").size());
    cnf.str("");
  }

  cout << "Test matrix of dimension 10" << endl;
  {
    OutputCls::Adapter_Dimacs_Output out(cnf);
    const unsigned int dim = 10;
    typedef Matrices::QuadMatrix<unsigned int, unsigned int, dim> Arr;
    const Arr::array_type a = {
      0,2,2,2,2,2,2,2,2,2,
      2,0,2,2,2,2,2,2,2,2,
      2,2,0,2,2,2,2,2,2,2,
      2,2,2,0,2,2,2,2,2,2,
      2,2,2,2,0,2,2,2,2,2,
      2,2,2,2,2,0,2,2,2,2,
      2,2,2,2,2,2,0,2,2,2,
      2,2,2,2,2,2,2,0,2,2,
      2,2,2,2,2,2,2,2,0,2,
      2,2,2,2,2,2,2,2,2,0
    };
    Arr A(a);
    const unsigned int max_bc = 6;
    assert(TransformationsBiclique::number_variables_of_transformation(A, dim, max_bc, out) == 1080);
    assert(TransformationsBiclique::number_clauses_of_transformation(A, dim, max_bc, out) == 145350);
    //TransformationsBiclique::symmetric_conflict_number_to_SAT(A, dim, max_bc, out);
  }
  cnf.str("");

  cout << "Test matrix of dimension 10 (Petersen graph)" << endl;
  {
    OutputCls::Adapter_Dimacs_Output out(cnf);
    const unsigned int dim = 10;
    typedef Matrices::QuadMatrix<unsigned int, unsigned int, dim> Arr;
    const Arr::array_type a = {
      0,1,1,1,2,2,2,2,2,2, // 0
      1,0,2,2,1,2,2,2,1,2, // 1
      1,2,0,2,2,2,1,1,2,2, // 2
      1,2,2,0,2,1,2,2,2,1, // 3
      2,1,2,2,0,1,2,1,2,2, // 4
      2,2,2,1,1,0,1,2,2,2, // 5
      2,2,1,2,2,1,0,2,1,2, // 6
      2,2,1,2,1,2,2,0,2,1, // 7
      2,1,2,2,2,2,1,2,0,1, // 8
      2,2,2,1,2,2,2,1,1,0  // 9
    };
    Arr A(a);
    const unsigned int max_bc = 6;
    assert(TransformationsBiclique::number_variables_of_transformation(A, dim, max_bc, out) == 900);
    assert(TransformationsBiclique::number_clauses_of_transformation(A, dim, max_bc, out) == 101865);
    // TransformationsBiclique::symmetric_conflict_number_to_SAT(A, dim, max_bc, out);
    cnf.str("");
  }

  cout << "Test of class Symmetric_conflict_number_to_SAT\n";

  cout << "Test matrix of dimension 1" << endl;;
  {
    OutputCls::Adapter_Dimacs_Output out(cnf);
    const unsigned int dim = 1;
    typedef Matrices::QuadMatrix<unsigned int, unsigned int, dim> Arr;
    const Arr::array_type a = {
      0
    };
    Arr A(a);
    const unsigned int max_bc = 1;
    TransformationsBiclique::Symmetric_conflict_number_to_SAT<Arr, OutputCls::Adapter_Dimacs_Output> scn(A, max_bc, out);
    assert(scn.number_of_conflicts == 0);
    assert(scn.number_of_variables == 0);
  }

  cout << "Test matrix of dimension 10 (Petersen graph)" << endl;
  {
    OutputCls::Adapter_Dimacs_Output out(cnf);
    const unsigned int dim = 10;
    typedef Matrices::QuadMatrix<unsigned int, unsigned int, dim> Arr;
    const Arr::array_type a = {
      0,1,1,1,2,2,2,2,2,2, // 0
      1,0,2,2,1,2,2,2,1,2, // 1
      1,2,0,2,2,2,1,1,2,2, // 2
      1,2,2,0,2,1,2,2,2,1, // 3
      2,1,2,2,0,1,2,1,2,2, // 4
      2,2,2,1,1,0,1,2,2,2, // 5
      2,2,1,2,2,1,0,2,1,2, // 6
      2,2,1,2,1,2,2,0,2,1, // 7
      2,1,2,2,2,2,1,2,0,1, // 8
      2,2,2,1,2,2,2,1,1,0  // 9
    };
    Arr A(a);
    const unsigned int max_bc = 6;
    TransformationsBiclique::Symmetric_conflict_number_to_SAT<Arr, OutputCls::Adapter_Dimacs_Output> scn(A, max_bc, out);
    assert(scn.number_of_conflicts == (3 * 1 + 6 * 2) * 10 / 2);
    assert(scn.number_of_variables == ((1 + 3 + 5 + 7 + 7 + 11 + 12) + 4 * 2 * 2 * max_bc) + (scn.number_of_conflicts - (3 * 1 + 6 * 2)) * 2 * max_bc);
    // n = 862
    //assert(TransformationsBiclique::number_clauses_of_transformation(A, dim, max_bc, out) == 101865);
    // TransformationsBiclique::symmetric_conflict_number_to_SAT(A, dim, max_bc, out);
    cnf.str("");
  }
}

template <class Cl>
void test_Clauses() {
  cout << "\ntest_clause(): " << typeid(Cl).name() << "\n";
  Cl c1,c2;
  //check for empty clauses;
  assert( c1.size() == 0 );
  assert( c1.empty() and c2.empty() );
  typedef typename Cl::Lit Lit;
  typename Lit::Var v1("abcd");
  typename Lit::Var v2("1");
  typename Lit::Var v3("zhi");
  Lit r1(v1, Values::neg);
  Lit r2(v2, Values::neg);
  Lit r3(v3, Values::neg);
  c1.insert(r1);
  c1.insert(r2);
  c1.insert(r3);
  assert(c1.size() == 3);
  assert(not c1.empty());
  c1.erase(r1);
  assert(*c1.begin() == r3 and *(++c1.begin()) == r2);
  assert(c1.size() == 2);
  assert(not c1.empty());
  Lit r4(v1, Values::pos);
  Lit r5(v2, Values::pos);
  Lit r6(v3, Values::pos);
  c2.insert(r4);
  c2.insert(r5);
  c2.insert(r6);
  assert(c2.size() == 3);
  assert(*c2.begin() == r4 and *(++c2.begin()) == r5);
  c2=c1;
  assert(c2 == c1);
  assert(c2.size() == 2);
  assert(c1.size() == 2);
  c1.erase(r2);
  c1.erase(r5);
  c1.erase(r3);
  c1.erase(r2);
  assert(c1.size() == 0);
  assert( not (c2 < c1));
  assert(c1 < c2);
  assert(*c2.begin() == r3 and *(++c2.begin()) == r2);
}

template <class Cls>
void test_Clausesets() {
  cout << "\ntest_clauseset(): " << typeid(Cls).name() << "\n";
  Cls F1, F2;
  assert(F1.size() == 0 and F1.empty() and F1.unsat() == Auxiliary::unknown and F1.sat() == Auxiliary::satisfiable and F1 == F2);
  assert(F2.size() == 0);
  assert(F1.empty() and F2.empty());
  typedef typename Cls::Cl Cl;
  Cl C1,C2;
  typedef typename Cls::Cl::Lit Lit;
  typename Lit::Var v1("abcd");
  typename Lit::Var v2("1");
  typename Lit::Var v3("zhi");
  Lit r1(v1, Values::neg);
  Lit r2(v2, Values::neg);
  Lit r3(v3, Values::neg);
  C1.insert(r1);
  C1.insert(r2);
  C1.insert(r3);
  assert(C1.size() == 3);
  F1.insert(C1);
  assert(F1.size() == 1 and F1.unsat() == Auxiliary::unknown and F1.sat() == Auxiliary::unknown and F1 != F2);
  Lit r4(v1, Values::pos);
  Lit r5(v2, Values::pos);
  Lit r6(v3, Values::pos);
  C2.insert(r4);
  C2.insert(r5);
  C2.insert(r6);
  assert(C2.size() == 3);
  F1.insert(C1);
  F1.insert(C2);
  assert(F1.size() == 2 and F1.unsat() == Auxiliary::unknown and F1.sat() == Auxiliary::unknown);
  F1.insert(Cl());
  assert(F1.size() == 3 and F1.unsat() == Auxiliary::unsatisfiable and F1.sat() == Auxiliary::unknown);
}

template <class Pass>
void test_PartialAssignments() {
  cout << "\ntest_partial_assignments(): " << typeid(Pass).name() << "\n";
  typedef typename Pass::Lit Lit;
  typedef typename Lit::Var Var;
  typedef typename Lit::Var Val;
  typedef typename Clauses::ClAsSets<Lit> Cl;
  Cl C1;
  C1.insert(Lit(Var("abc"), Values::pos)).insert(Lit(Var("def"), Values::neg)).insert(Lit(Var("xyz"), Values::pos));
  assert(C1.size() == 3);
  Pass phi;
  assert (phi.empty());
  Lit x(Var("abc"), Values::neg);
  Pass phi1(x);
  assert(phi1[x] == Auxiliary::falsified);
  assert(phi1[++x] == Auxiliary::satisfied);
  assert(C1.find(x) != C1.end());
  phi1[Literals::Var(x)] = Values::pos;
  assert(phi1[x] == Auxiliary::falsified);
  ++phi1[Literals::Var(x)];
  assert(phi1[x] == Auxiliary::satisfied);
  assert(phi1[(Lit(Var("def"), Values::neg))] == Auxiliary::undefined);
  Pass phi2((Lit(Var("abc"), Values::neg))); // parse error without the additional brackets ????
  assert(phi1 == phi2);
  Pass phi3(C1.begin(), C1.end());
  assert(phi3[Var("abc")] == Values::pos and phi3[Var("def")] == Values::neg and phi3[Var("xyz")] == Values::pos);
}

template <class Cls, class Pass>
void test_apply() {
  cout << "\ntest_apply(): " << "\n";
  typedef typename Pass::Lit Lit;
  typedef typename Lit::Var Var;
  typedef typename Lit::Var Val;
  typedef typename Cls::Cl Cl;
  Cls CS;
  Cl C1;
  C1.insert(Lit(Var("abc"), Values::pos)).insert(Lit(Var("def"), Values::neg)).insert(Lit(Var("xyz"), Values::pos));
  assert(C1.size() == 3);
  CS.insert(C1);
  assert(CS.size()==1);
  Lit x(Var("abc"), Values::neg);
  Pass phi(x);
  CS.apply(phi);
  assert(CS.empty());

}

void test_InputCls() {
  cout << "\ntest_InputCls(): " << "\n";

  typedef InputCls::Construct_Cls_dynamic<ClauseSets::ClsAsSets_ClAsSets_LitIntOccString> Formula1_type;
  typedef Formula1_type::Cls Cls;
  Cls F;
  Formula1_type Formula1(F);

  {
    // empty input
    istringstream in("");
    bool thrown = false;
    try {
      DIMACS_Input(in, Formula1);
    }
    catch(const InputCls::ReadError& e) {
      thrown = true;
      assert(e.what() == "DIMACS_Input(std::istream& in, Formula& F) : line 1");
    }
    assert(thrown);
  }
  {
    // non-comment before parameter line
    istringstream in("c\na");
    bool thrown = false;
    try {
      DIMACS_Input(in, Formula1);
    }
    catch(const InputCls::SyntaxError& e) {
      thrown = true;
      assert(e.what() == "DIMACS_Input(std::istream& in, Formula& F) : Non-comment line before parameter line; line 2, first token \"a\"");
    }
    assert(thrown);
  }
  {
    // missing cnf declaration
    istringstream in("c\nc gg\np cnx 6 7");
    bool thrown = false;
    try {
      DIMACS_Input(in, Formula1);
    }
    catch(const InputCls::MissingCnfError& e) {
      thrown = true;
      assert(e.what() == "DIMACS_Input(std::istream& in, Formula& F) : line 3, parameter line has token \"cnx\" instead of \"cnf\"");
    }
    assert(thrown);
  } 
  {
    // non-integer parameter values
    istringstream in("c\nc gg\np cnf u 7");
    bool thrown = false;
    try {
      DIMACS_Input(in, Formula1);
    }
    catch(const InputCls::ParameterError& e) {
      thrown = true;
      assert(e.what() == "DIMACS_Input(std::istream& in, Formula& F) : invalid parameter values; line 3");
    }
    assert(thrown);
  }
  {
    // non-integer parameter values
    istringstream in("c\nc gg\np cnf 6 7x");
    bool thrown = false;
    try {
      DIMACS_Input(in, Formula1);
    }
    catch(const InputCls::AfterParameterError& e) {
      thrown = true;
      assert(e.what() == "DIMACS_Input(std::istream& in, Formula& F) : line 3, rest of line after parameter values is not empty, but is \"x\"");
    }
    assert(thrown);
  }

  typedef OutputCls::Adapter_Dimacs_Output Formula2_type;
  ostringstream out;
  Formula2_type Formula2(out);

//   {
//     // copy of comments
//     istringstream in("c 1\nc\nc 3\np cnf 0 0\n");
//     InputCls::DIMACS_Input(in, Formula2);
//     cout << out.str() << endl;
//     assert(out.str() == "c 1\nc\nc 3\np cnf 0 0\n");
//   }
}


int main() {

  cout << "\n**************************************************\n";

  try {

    // OLD TESTS

    /* do not work
    test_finite_domains<Domains::FiniteDomain<unsigned int, 1> >();
    test_finite_domains<Domains::FD2>();
    test_finite_domains<Domains::FD3>();
    test_finite_domains<Domains::FD4>();
    */
    test_signring<AlgebraicStructures::SignRing>();
    test_boolean<Domains::Boolean>();
    /*
    test_finite_domains<Domains::Boolean>();
    */

    assert((boost::is_same<ConceptDefinitions::VariableWithInfo_tag, Variables::VarIntOccString::Concept>::value));
    assert((boost::is_same<ConceptDefinitions::VariableWithCounting_tag, Variables::VarIntEmptyString::Concept>::value));
    assert((boost::is_same<ConceptDefinitions::VariableWithInfo_tag, Variables::VarCharOccInt::Concept>::value));

    RunTest<Variables::VarIntOccString>();
    RunTest<Variables::VarIntEmptyString>();
    RunTest<Variables::VarCharOccInt>();
    RunTest<Variables::VariablesAsNames<string> >();
    RunTest<Variables::VarRefIntString>();
    RunTest<Variables::VarRefCharInt>();

    RunTest<SetsOfVariables::SingletonVarSet<Variables::VarIntOccString> >();
    RunTest<SetsOfVariables::SingletonVarSet<Variables::VarRefIntString> >();
 
    /* failing test
    RunTest<PartialAssignments::PassViaMap<Variables::VarIntOccString, Domains::Boolean> >();
    */

    test_literals_01<Literals::LitIntOccString>();
    test_BooleanValues<Values::BooleanValues>();
    test_Literal_set<LitSets::LitSet>();
    test_Clauses<Clauses::ClAsSets_LitIntOccString>();
    test_Clausesets<ClauseSets::ClsAsSets_ClAsSets_LitIntOccString>();
    using namespace PartialAssignments;
    test_PartialAssignments<PartialAssignments::PassAsMaps_LitIntOccString>();
    test_apply<ClauseSets::ClsAsSets_ClAsSets_LitIntOccString, PartialAssignments::PassAsMaps_LitIntOccString>();
    test_OutputCls();
    test_InputCls();
    test_TransformationBiclique();

    // NEW TESTS

    Tests_General::RunTest<Variables::VariablesAsIntegers_DomainWithNameAdministration<int, std::string> >();
    Tests_General::RunTest<Variables::VariablesAsIntegers_DomainWithNameAdministration<int, int> >();
    Tests_General::RunTest<Variables::VariablesAsIntegers_DomainWithNameAdministration<int, double> >();
    Tests_General::RunTest<Variables::VariablesAsIntegers_DomainWithNameAdministration<signed char, std::string> >();
  }
  catch (const ErrorHandling::Error& e) {
    cerr << ErrorHandling::Error2string(e) << "\n";
    return 1;
  }
  catch (const std::exception& e) {
    cerr << ErrorHandling::Error2string(e) << "\n";
    return 1;
  }

  cout << endl;
}

