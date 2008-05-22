// Oliver Kullmann, 2.10.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef TESTSVARIABLESWAECHTER_jahd516344fRdx

#define TESTSVARIABLESWAECHTER_jahd516344fRdx

#include <vector>
#include <functional>
#include <string>
#include <cassert>
#include <algorithm>
#include <iostream>

#include <boost/concept_check.hpp>

#include <Transitional/General/StringHandling.hpp>
#include <Transitional/General/Algorithms.hpp>

#include <Transitional/OKsolver/Experimental/AllgKlassen200203/Tests_General.hpp>

#include <Transitional/OKsolver/Experimental/AllgKlassen200203/Traits_General.hpp>
#include <Transitional/OKsolver/Experimental/AllgKlassen200203/Traits_Variables.hpp>
#include <Transitional/OKsolver/Experimental/AllgKlassen200203/Concepts_Variables.hpp>
#include <Transitional/OKsolver/Experimental/AllgKlassen200203/Variables.hpp>


namespace Tests_General {

  namespace Helper_VariableDomain {

    template <class VarD>
    void test_capacity(VarD& Dom, const int n, Concepts_Variables::VariableDomain_tag) {}

    template <class VarD>
    void test_capacity(VarD& Dom, const int n, Concepts_Variables::VariableDomainWithSizeType_tag) {
      if (n <= 0) return;
      assert(Dom.size_type_cast((unsigned int)(n)) <= Dom.max_size());
    }

    // ATTENTION: ONLY FOR SAFE ALLOCATION!!
    template <class VarD, typename Iterator, typename name_type>
    struct fill_variables {
      bool operator ()(VarD& Dom, Iterator begin, const Iterator end, const int n) { return false; }
    };
    template <class VarD, typename Iterator>
    struct fill_variables<VarD, Iterator, std::string> {
      bool operator ()(VarD& Dom, Iterator begin, const Iterator end, const int n) {
	const std::string prefix = "Var ";
	for (int i = 0; i < n; ++i, ++begin)
	  *begin = Dom(prefix + StringHandling::toString(i));
	return true;
      }
    };
    template <class VarD, typename Iterator>
    struct fill_variables<VarD, Iterator, int> {
      bool operator ()(VarD& Dom, Iterator begin, const Iterator end, const int n) {
	for (int i = 1; i <= n; ++i, ++begin)
	  *begin = Dom(i);
	return true;
      }
    };
    // VERSIONS FOR UNSAFE ALLOCATION NEEDED!
    // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    template <class VarD, typename Iterator>
    void test_order(const Iterator begin, const Iterator end, const VarD& Dom, Traits_Variables::OrderNameInherited) {
      for (Iterator i = begin; i != end; ++i)
	for (Iterator j = ++Iterator(i); j != end; ++j)
	  if (*i < *j) {
	    assert(Dom.name(*i) < Dom.name(*j));
	    assert(not(*i == *j) and *i != *j and *i <= *j and *j > *i and not(*i >= *j));
	  }
	  else {
	    assert(*j < *i);
	    assert(Dom.name(*j) < Dom.name(*i));
	    assert(not(*j == *i) and *j != *i and *j <= *i and *i > *j and not(*j >= *i));
	  }
    }

    template <class VarD, typename Iterator>
    void test_order(const Iterator begin, const Iterator end, const VarD&, Traits_Variables::OrderByCreation) {
      for (Iterator i = begin; i != end; ++i)
	for (Iterator j = ++Iterator(i); j != end; ++j) {
	  assert(*i < *j);
	  assert(not(*i == *j) and *i != *j and *i <= *j and *j > *i and not(*i >= *j));
	}
    }

    template <class T>
    void test_equal(T x) {
      assert(x == x and not (x != x) and not (x < x) and (x <= x) and not (x > x) and (x >= x));
    }
    template <class T>
    void test_less(T a, T b) {
      assert(a < b and not(b < a) and not (a == b) and not(b == a) and (a != b) and (b != a) and not(a > b) and (b > a) and (a <= b) and not(b <= a) and not(a >= b) and (b >= a));
    }
  }

  // Partial specialisations of class template Test to test
  // Concepts_Variables::VariableDomain_concept

  template <class VarD>
  class Test<VarD, Concepts_Variables::VariableDomain_tag> : virtual public Test<VarD, Concepts_General::Root_tag> {
  public :
    typedef VarD Class;
    typedef Concepts_Variables::VariableDomain_tag Tag;

    typedef typename Traits_General::MetaData<VarD>::Var Var;
    typedef typename Traits_General::MetaData<VarD>::name_type name_type;

    typedef typename Traits_General::MetaData<VarD>::comparison_property comparison_property;
    typedef typename Traits_General::MetaData<VarD>::allocation_property allocation_property;
    typedef typename Traits_General::MetaData<VarD>::name_property name_property;
    typedef typename Traits_General::MetaData<VarD>::order_property order_property;

    Test() {
      using namespace std;
      std::cout << "Tag " << typeid(Tag).name() << "\n";
      boost::function_requires<Concepts_Variables::VariableDomain_concept<VarD> >();

      std::cout << "null_variables?;";
      test_variables_null();
      // Given type Var, test the creation and properties of null variables.

      std::cout << "default name?;";
      test_default_name();

      std::cout << "variables_creation?;";
      test_variables_creation();

      std::cout << "different domains?;";
      test_different_domains();

      std::cout << "allocation properties?;";
      test_allocation();

      std::cout << "\n";
    }

  protected :
    typedef std::vector<Var> var_vector;
    typedef typename std::vector<Var>::iterator vv_iterator;
    typedef typename std::vector<Var>::const_iterator vv_const_iterator;

  private :

    void all_null(const var_vector& var_v) {
      using namespace std;
      using namespace Algorithms;
      const vv_const_iterator begin(var_v.begin());
      const vv_const_iterator end(var_v.end());
      const std::pair<vv_const_iterator, vv_const_iterator> end_pair(end, end);
      
      assert(pair1_find_if(begin, end, not2(equal_to<Var>())) == end_pair);
      assert(pair1_find_if(begin, end, not_equal_to<Var>()) == end_pair);
      assert(pair1_find_if(begin, end, less<Var>()) == end_pair);
      assert(pair1_find_if(begin, end, greater<Var>()) == end_pair);
      assert(pair1_find_if(begin, end, not2(less_equal<Var>())) == end_pair);
      assert(pair1_find_if(begin, end, not2(greater_equal<Var>())) == end_pair);
      for (typename var_vector::const_iterator i = var_v.begin(); i != var_v.end(); ++i) {
	assert(i -> null());
	assert(*i == Var());
      }
    }

    void test_variables_null() {
      const int number_var = 500;
      {
	var_vector var_v(number_var);
	all_null(var_v);
	var_vector var_v2(var_v);
	all_null(var_v2);
      }
      {
	var_vector var_v(number_var);
	all_null(var_v);
	var_vector var_v2(number_var);
	std::copy(var_v.begin(), var_v.end(), var_v2.begin());
	all_null(var_v2);
      }
    } // seems okay: 17.11.2003

    void test_default_name() {
      assert(Var() == VarD()(name_type()));
    }

    void test_variables_creation() {
      using namespace std;
      VarD Dom;
      const int number_var = 100;
      var_vector var_v; var_v.reserve(number_var);
      create_variables(Dom, var_v, number_var);

      // variables not null :
      // USE ALGORITHMS:: ... XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
      for (vv_const_iterator i = var_v.begin(); i != var_v.end(); ++i)
	assert(not (i -> null()));
      assert(find_if(var_v.begin(), var_v.end(), bind2nd(equal_to<Var>(), Var())) == var_v.end());

      // variables all different :
      for (vv_const_iterator i = var_v.begin(); i != var_v.end(); ++i) {
	Helper_VariableDomain::test_equal(*i);
	for (vv_const_iterator j = ++vv_const_iterator(i); j != var_v.end(); ++j)
	  if (*i < *j)
	    Helper_VariableDomain::test_less(*i, *j);
	  else
	    Helper_VariableDomain::test_less(*j, *i);
      }

      // order by creation or by name :
      Helper_VariableDomain::test_order(var_v.begin(), var_v.end(), Dom, typename Traits_General::MetaData<VarD>::order_property());
    }

    void test_different_domains() {
      // testing of the different cases for comparison_property
      // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    }

    void test_allocation() {
      // testing of the different cases for allocation_property
      // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    }

  protected :

    void create_variables(VarD& Dom, var_vector& var_v, const int n) {
      Helper_VariableDomain::test_capacity(Dom, n, typename Traits_General::MetaData<VarD>::concept_tag());
      //  Now we assume that n is not too big for VarD.
      var_v.resize(n);
      // Here if reserve is available, we should use it, and use fill_variables under the assumption of safe allocation, while otherwise we use fill_variables for unsafe allocation!
      // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
      if (not Helper_VariableDomain::fill_variables<VarD, typename var_vector::iterator, name_type>()(Dom, var_v.begin(), var_v.end(), n))
	var_v.resize(0);
    }
  };


  template <class VarD>
  class Test<VarD, Concepts_Variables::VariableDomainWithSizeType_tag> : virtual public Test<VarD, Concepts_Variables::VariableDomain_tag> {
  public :
    typedef VarD Class;
    typedef Concepts_Variables::VariableDomainWithSizeType_tag Tag;

    
    Test() {
      std::cout << "Tag " << typeid(Tag).name() << "\n";
      boost::function_requires<Concepts_Variables::VariableDomainWithSizeType_concept<VarD> >();
    }

  private :

  };

  template <class VarD>
  class Test<VarD, Concepts_Variables::VariableDomainWithAllocation_tag> : virtual public Test<VarD, Concepts_Variables::VariableDomainWithSizeType_tag> {
  public :
    typedef VarD Class;
    typedef Concepts_Variables::VariableDomainWithAllocation_tag Tag;
    Test() {
      std::cout << "Tag " << typeid(Tag).name() << "\n";
      boost::function_requires<Concepts_Variables::VariableDomainWithAllocation_concept<VarD> >();
    }
  };

  template <class VarD>
  class Test<VarD, Concepts_Variables::VariableDomainWithIterator_tag> : virtual public Test<VarD, Concepts_Variables::VariableDomainWithSizeType_tag> {
  public :
    typedef VarD Class;
    typedef Concepts_Variables::VariableDomainWithIterator_tag Tag;
    Test() {
      std::cout << "Tag " << typeid(Tag).name() << "\n";
      boost::function_requires<Concepts_Variables::VariableDomainWithIterator_concept<VarD> >();
    }
  };

  template <class VarD>
  class Test<VarD, Concepts_Variables::VariableDomainWithIteratorAndAllocation_tag> : virtual public Test<VarD, Concepts_Variables::VariableDomainWithAllocation_tag>, virtual public Test<VarD, Concepts_Variables::VariableDomainWithIterator_tag>{
  public :
    typedef VarD Class;
    typedef Concepts_Variables::VariableDomainWithIteratorAndAllocation_tag Tag;
    Test() {
      std::cout << "Tag " << typeid(Tag).name() << "\n";
      boost::function_requires<Concepts_Variables::VariableDomainWithIteratorAndAllocation_concept<VarD> >();
    }
  };


}



#endif

